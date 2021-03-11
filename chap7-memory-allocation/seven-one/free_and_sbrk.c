/*
 * Also run this program with command
 *
 * `strace command options
 *
 * to see the system calls used by malloc().
 *
 * Q. Find out when malloc employs mmap() sys call and
 * when it employs sbrk() sys call for allocation of
 * memory, or, if it even does employ these sys calls?
 * 
 * NOTE the answer to above question is key to solving
 * question seven-two.
 */
#include <unistd.h>
#include <stdlib.h>

#include <tlpi_hdr.h>

#define BLK_PTR_ARR_LEN 100000
int
main(int argc, char *argv[])
{
  int noOfBlocks, freeStep, freeMin, freeMax, blockSize;
  /* CAUTION!
   *
   * void *ptr[argv[1]];
   *
   * gives error: size of array ‘ptr’ has non-integer 
   */
  void *ptr[BLK_PTR_ARR_LEN];
  intptr_t increment;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s #blocks block-size [step [min [max]]]\n", argv[0]);

  noOfBlocks =  getInt(argv[1], GN_GT_0, "num-allocs");
  if (noOfBlocks > BLK_PTR_ARR_LEN)
	cmdLineErr("num-allocs > %d\n", BLK_PTR_ARR_LEN);

  blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

  freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
  freeMin = (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
  freeMax = (argc > 5) ? getInt(argv[5], GN_GT_0, "max") : noOfBlocks;

  if (freeMax > noOfBlocks)
	cmdLineErr("freeMax > numAllocs\n");

  /*
   * The current limit of the heap is referred to as the
   * program break.
   */
  /*
   * `void *sbrk(intptr_t increment );`
   *
   * A call to sbrk() adjusts the program break by adding increment
   * to it. (On Linux, sbrk() is a library function implemented on 
   * top of brk().) The intptr_t type used to declare increment is 
   * an integer data type. On success, sbrk() returns the previous
   * address of the program break. In other words, if we have increased
   * the program break, then the return value is a pointer to the
   * start of the newly allocated block of memory.
   */
  /*
   * The call sbrk(0) returns the current setting of the program break
   * without changing it. This can be useful if we want to track the
   * size of the heap, perhaps in order to monitor the behavior of a
   * memory allocation package.
   */
  printf("Initial program break: %10p\n", sbrk(0));
  printf("Allocating %d*%d bytes\n", noOfBlocks, blockSize);

  /* allocate memory */
  for (int i = 0; i < noOfBlocks; i++) {
	/*
	 * `
	 * #include <stdlib.h>
	 *
	 * void *malloc(size_t size );
	 * `
	 *
	 * The malloc() function allocates size bytes from the heap and 
	 * returns a pointer to the start of the newly allocated block of
	 * memory. The allocated memory is not initialized.
	 */
	ptr[i] = malloc(blockSize);
	/*
	 * If memory could not be allocated (perhaps because we reached 
	 * the limit to which the program break could be raised), then 
	 * malloc() returns NULL and sets errno to indcate the error.
	 * Although the possibility of failure in allocating memory is
	 * small, all calls to malloc(), and the related functions that
	 * we describe later, should check for this error return.
	 */
	if (ptr[i] == NULL)
	  errExit("malloc");
	
	printf("program break after call to malloc for block number %d: %10p\n", i, sbrk(0));
  }

  /* Q. Shouldn't using sbrk(increment) ajust program break
   * to increment, which is exactly what happend after
   * allocating memory?
   */
  printf("Program break after allocating all the blocks is %10p\n",
	  sbrk(0));

  printf("Freeing blocks from %d to %d in steps of %d\n",
	  freeMin, freeMax, freeStep);

  /* 
   * Consider the fact that C allows us to create pointers to any location in the
   * heap, and modify the locations they point to, including the length, previous free block,
   * and next free block pointers maintained by free() and malloc(). Add this to the preced-
   * ing description, and we have a fairly combustible mix when it comes to creating
   * obscure programming bugs. For example, if, via a misdirected pointer, we acciden-
   * tally increase one of the length values preceding an allocated block of memory, and
   * subsequently deallocate that block, then free() will record the wrong size block of
   * memory on the free list. Subsequently, malloc() may reallocate this block, leading to
   * a scenario where the program has pointers to two blocks of allocated memory that
   * it understands to be distinct, but which actually overlap. Numerous other pictures
   * of what could go wrong can be drawn.
   *
   * To avoid these types of errors, we should observe the following rules:
   *
   * 1. After we allocate a block of memory, we should be careful not to touch any
   * bytes outside the range of that block. This could occur, for example, as a result
   * of faulty pointer arithmetic or off-by-one errors in loops updating the contents of
   * a block.
   *
   * 2. It is an error to free the same piece of allocated memory more than once. With
   * glibc on Linux, we often get a segmentation violation ( SIGSEGV signal). This is
   * good, because it alerts us that we’ve made a programming error. However,
   * more generally, freeing the same memory twice leads to unpredictable behavior.
   *
   * 3.We should never call free() with a pointer value that wasn’t obtained by a call to
   * one of the functions in the malloc package.
   *
   * 4. If we are writing a long-running program (e.g., a shell or a network daemon
   * process) that repeatedly allocates memory for various purposes, then we
   * should ensure that we deallocate any memory after we have finished using it.
   * Failure to do so means that the heap will steadily grow until we reach the limits
   * of available virtual memory, at which point further attempts to allocate mem-
   * ory fail. Such a condition is known as a memory leak.
   */
  /* free memory */
  for (int j = freeMin - 1; j < freeMax; j = j + freeStep) {
	free(ptr[j]);
  }

  /* Q. How do I find out program break after freeing blocks */
  printf("Program break after freeing blocks is %10p\n",
	  sbrk(0));

  exit(EXIT_SUCCESS);
}

/* 
 * Output of command 
 *` ./free_and_sbrk.out 10 1024`
 * 
 * Initial program break:  0x1de9000
 * Allocating 10*1024 bytes
 * program break after call to malloc for block number 0:  0x1e0b000
 * program break after call to malloc for block number 1:  0x1e0b000
 * program break after call to malloc for block number 2:  0x1e0b000
 * program break after call to malloc for block number 3:  0x1e0b000
 * program break after call to malloc for block number 4:  0x1e0b000
 * program break after call to malloc for block number 5:  0x1e0b000
 * program break after call to malloc for block number 6:  0x1e0b000
 * program break after call to malloc for block number 7:  0x1e0b000
 * program break after call to malloc for block number 8:  0x1e0b000
 * program break after call to malloc for block number 9:  0x1e0b000
 * Program break after allocating all the blocks is  0x1e0b000
 * Freeing blocks from 1 to 10 in steps of 1
 * Program break after freeing blocks is  0x1e0b000
 *
 * Observe that
 * program break after call to malloc for block number 0 to 9 are same.
 * This is because
 * malloc() doesn’t employ sbrk()
 * to adjust the program break on each call, but instead periodically 
 * allocates larger chunks of memory from which it passes back small 
 * pieces to the caller.
 *
 */
/*
 * Output of command 
 * `./free_and_sbrk.out 10 10240`
 *
 * Initial program break:   0x7eb000
 * Allocating 10*10240 bytes
 * program break after call to malloc for block number 0:   0x80d000
 * program break after call to malloc for block number 1:   0x80d000
 * program break after call to malloc for block number 2:   0x80d000
 * program break after call to malloc for block number 3:   0x80d000
 * program break after call to malloc for block number 4:   0x80d000
 * program break after call to malloc for block number 5:   0x80d000
 * program break after call to malloc for block number 6:   0x80d000
 * program break after call to malloc for block number 7:   0x80d000
 * program break after call to malloc for block number 8:   0x80d000
 * program break after call to malloc for block number 9:   0x80d000
 * Program break after allocating all the blocks is   0x80d000
 * Freeing blocks from 1 to 10 in steps of 1
 * Program break after freeing blocks is   0x80d000
 *
 * Observe that
 * program break after call to malloc for block number 0 to 9 are same.
 * This is similar to output of command
 * `./free_and_sbrk.out 10 1024`
 *
 * This is because
 * malloc() doesn’t employ sbrk()
 * to adjust the program break on each call, but instead periodically 
 * allocates larger chunks of memory from which it passes back small 
 * pieces to the caller.
 *
 */
/*
 * Output of command 
 * `./free_and_sbrk.out 10 102400`
 *
 * Initial program break:  0x134f000
 * Allocating 10*102400 bytes
 * program break after call to malloc for block number 0:  0x1371000
 * program break after call to malloc for block number 1:  0x13a3000
 * program break after call to malloc for block number 2:  0x13a3000
 * program break after call to malloc for block number 3:  0x13d5000
 * program break after call to malloc for block number 4:  0x13d5000
 * program break after call to malloc for block number 5:  0x1407000
 * program break after call to malloc for block number 6:  0x1407000
 * program break after call to malloc for block number 7:  0x1439000
 * program break after call to malloc for block number 8:  0x1439000
 * program break after call to malloc for block number 9:  0x146b000
 * Program break after allocating all the blocks is  0x146b000
 * Freeing blocks from 1 to 10 in steps of 1
 * Program break after freeing blocks is  0x1371000
 *
 * Observe that
 * program break after call to malloc for block number 0 to 9 are same
 * for some consecutive calls, but changes periodically.
 * This is different from output of command
 * `./free_and_sbrk.out 10 1024` and `./free_and_sbrk 101 102400`
 *
 * This is because
 * malloc() doesn’t employ sbrk()
 * to adjust the program break on each call, but instead periodically 
 * allocates larger chunks of memory from which it passes back small 
 * pieces to the caller.
 *
 * (Refer Stackoverflow bookmarked:
 * Program break doesnt change after calling malloc in a loop?)
 * VERIFY this? --> glibc overallocates a lot, so the value has to be 
 * large enough for it to see it. And the default M_MMAP_THRESHOLD seem
 * to be 128 * 1024 (not necessarily).
 * So you have to pick a value large enough, but below mmap threshold 
 * to see a difference in glibc.
 */
 *
