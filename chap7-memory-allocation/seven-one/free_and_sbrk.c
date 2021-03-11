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
	usageErr("%s #blocks block-size [step [min [max]]]", argv[0]);

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
