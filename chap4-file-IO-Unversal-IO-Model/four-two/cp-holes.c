#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include "tlpi_hdr.h"

int
main (int argc, char *argv[])
{
  int srcFile, tgtFile, srcOpenFlags, tgtOpenFlags,
	  tgtPerms;

  ssize_t numWrite;

  off_t tgtEnd;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s old-file new-file\n", argv[0]);

  srcOpenFlags = O_RDONLY;
  tgtOpenFlags = O_CREAT | O_WRONLY;
  tgtPerms = S_IWUSR;
  
  if ((srcFile = open(argv[argc-2], srcOpenFlags)) == -1)
	errExit("open source");

  if ((tgtFile = open(argv[argc-1], tgtOpenFlags, tgtPerms)) == -1)
	errExit("open target");

  if ((tgtEnd = lseek(srcFile, 0, SEEK_END) == -1))
	errExit("seek");
	
  if ((numWrite = write(tgtFile, srcFile, tgtEnd)) == -1)
	errExit("write");

  exit(EXIT_SUCCESS);
}
