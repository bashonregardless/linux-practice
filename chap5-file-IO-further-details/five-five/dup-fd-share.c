/*
 * (Also refer fig 5-2)
 * Duplicate file descriptors share the same file offset value
 * and status flags in their shared open file description.
 * However, the new file descriptor has its own set of file
 * descriptor flags, and its close-on-exec flag ( FD_CLOEXEC )
 * is always turned off.
 */
//#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <tlpi_hdr.h>

int
main (int argc, char **argv)
{
  /* command example:
   * command filename [s][number-of-bytes]
   *
   * (Refer Listing 4-3)
   * Each of these operations consists of a letter followed by
   * an associated value (with no separating space):
   * 1. soffset: Seek to byte offset from the start of the file.
   * 2. TODO change status flags (At the moment of first writing this
   *	program, I am not adding this feature). 
   */ 

  int fd, dupFd;
  off_t fileOffset, dupFileOffset;

  if ((fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	errExit("open");

  if ((fileOffset = lseek(fd, 0, SEEK_CUR)) == -1)
	errExit("lseek");

  if ((dupFd = dup(fd)) == -1)
	errExit("dup");

  if ((dupFileOffset = lseek(dupFd, 0, SEEK_CUR)) == -1)
	errExit("lseek");

  printf("On first executing of the command value of:\n"
	  "\tfile offset is: %lld\n" 
	  "\tduplicated descriptor file offset is: %lld\n\n",
	  (long long) fileOffset, (long long) dupFileOffset);

  exit(EXIT_SUCCESS);
}
