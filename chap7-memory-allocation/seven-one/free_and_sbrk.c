#include <unistd.h>
#include <stdlib.h>

#include <tlpi_hdr.h>

int
main(int argc, char *argv[])
{
  int noOfBlocks, freeStep, min, max;
  size_t blockSize;
  void *ptr[];
  intptr_t increment;

  if (argc < 3 || strcmp(argv[1], "--help"))
	usageErr("%s #blocks block-size [step [min [max]]]", argv[0]);

  noOfBlocks = argv[1];
  blockSize = argv[2];

  freeStep = (argc == 3) ? argv[3] : 1;
  min = (argc == 4) ? argv[4] : 0;
  max = (argc == 5) ? argv[4] : noOfBlocks;

  /* allocate memory */
  for (int i = 0; i < noOfBlocks; i++) {
	ptr[i] = malloc(blockSize);
  }
  increment = noOfBlocks * blockSize;
  printf("Total bytes allocated is %d\n", increment);
  /* Q. Shouldn't using sbrk(increment) ajust program break
   * to increment, which is exactly what happend after
   * allocating memory?
   */
  printf("Program break after allocating all the blocks is %p\n",
	  sbrk(increment));

  /* free memory */
  for (int j = min; j < max; j = j + freeStep) {
	free(ptr[j]);
  }

  /* Q. How do I find out program break after freeing blocks */
  printf("Program break after freeing blocks is %p\n",
	  sbrk(increment));

  exit(EXIT_SUCCESS);
}
