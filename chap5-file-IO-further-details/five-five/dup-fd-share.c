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

#include <limits.h>
#include <errno.h>

#include <tlpi_hdr.h>

long
getNum (char *nptr, int base)
{
  long val;
  char *endptr, *str; 
  int errno;

  str = nptr;
  errno = 0; /* to distinguish success/failure after call */

  /* TODO Understand pointer concept and use of & operator. & used
   * directly after declaration and not allocation.
   */
  val = strtol(nptr, &endptr, base);

  /* check for various possible errors */

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	perror("strol");
	exit(EXIT_FAILURE);
  }

  if (endptr == str) {
	fprintf(stderr, "No digits were found\n");
	exit(EXIT_FAILURE);
  }

  /* if we got here, strtol() successfully parsed a number */

  printf("strtol returned %ld\n", val);

  if (*endptr == '\0') /* Not necessarily an error... */
	printf("Further characters after number: %s\n\n", endptr);

  return val;
}

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
   * 2. TODO change status flags (At the moment of first writing
   * 	this program, I am not adding this feature). 
   */ 

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s filename [s][nuumber-of-bytes]");

  int fd, dupFd, fileStatusFlags, dupFileStatusFlags;
  off_t fileOffset, dupFileOffset;

  if ((fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	errExit("open");

  if ((fileOffset = lseek(fd, 0, SEEK_CUR)) == -1)
	errExit("lseek");

  if ((dupFd = dup(fd)) == -1)
	errExit("dup");

  if ((dupFileOffset = lseek(dupFd, 0, SEEK_CUR)) == -1)
	errExit("lseek");

  /* If you want to break a string literal onto multiple lines,
   * you can concatenate multiple strings together, one on each
   * line.
   */
  printf("On first executing the command, value of:\n"
	  "\tfile offset is: %lld\n" 
	  "\tduplicated descriptor file offset is: %lld\n\n",
	  (long long) fileOffset, (long long) dupFileOffset);

  /* Verify File status flags sharing */
  
  if ((fileStatusFlags = fcntl(fd, F_GETFL)) == -1)
	errExit("fcntl");

  if ((dupFileStatusFlags = fcntl(fd, F_GETFL)) == -1)
	errExit("fcntl");

  printf("On first executing the command, value of:\n"
	  "\tfile status flag is: %d\n" 
	  "\tduplicated descriptor file status flag is: %d\n\n",
	  fileStatusFlags, dupFileStatusFlags);

  /* change offset and verify new offset */

  int ap;
  off_t offsetVal;

  for (ap = 2; ap < argc; ap++) {
	switch (argv[ap][0]) {
	  case 's': /* Change file offset */

		/* warning: this statement may fall through [-Wimplicit-fallthrough=] 
		 * `offsetVal = getNum(argv[ap][1], 10);`
		 *
		 * Note: & operator is necessary in 1st argument to getNum()
		 */
		offsetVal = getNum(&argv[ap][1], 10);
		
		if ((fileOffset = lseek(fd, offsetVal, SEEK_CUR)) == -1)
		  errExit("lseek");

		printf("After changing offset of original file with lseek, value of:\n"
			"\tfile offset is: %lld\n" 
			"\tduplicated descriptor file offset is: %lld\n\n",
			(long long) fileOffset, lseek(dupFd, 0, SEEK_CUR));

		/* TODO Case where status flags are changed from command line */

		break;
	  default:
		cmdLineErr("Argument must start with [s]: %s\n", argv[ap]);
	}
  }

  if (close(fd) == -1)
	errExit("close");
  if (close(dupFd) == -1)
	errExit("close");

  exit(EXIT_SUCCESS);
}
