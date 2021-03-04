#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <tlpi_hdr.h>

int
main(int argc, char *argv[])
{
  int fd;
  ssize_t numWrites;
  off_t startOffset;

  if ((fd = open(argv[1], O_RDWR | O_APPEND)) == -1)
	errExit("open");

  // move the file offset to start of file
  if ((startOffset = lseek(fd, 0, SEEK_SET)) == -1)
	errExit("seek");

  if ((numWrites = write(fd, 
		"This is probably wrong. You are trying to write a string directly",
		32)) == -1)
	errExit("write");
  /* BAD PRACTICE! Second argument to write is a string constant.
   * This is not the right way to use constants in C.
   *
   * Q. How do you declare a constant in C?
   * A. Use define macro.
   */

  if (close(fd) == -1)
	errExit("close");

  exit(EXIT_SUCCESS);
}

/* WHY?
 *
 * (See open(2) manual page)
 * O_APPEND
 *     The file is opened in append mode.  Before each write(2), the file offset is positioned at  the  end  of
 *     the file, as if with lseek(2).  O_APPEND may lead to corrupted files on NFS filesystems if more than one
 *     process appends data to a file at once.  This is because NFS does not support appending to  a  file,  so
 *     the client kernel has to simulate it, which can't be done without a race condition.
 */
