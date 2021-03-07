#include <fcntl.h>

#include <limits.h>
#include <errno.h>

#include <tlpi_hdr.h>

/* long is a signed interger type.
 * What is its range?
 * TODO Q. Does it depend on ISA(machine) (or equivalently,
 * does it vary with architecture)?
 */
long 
getNum(char *nptr, int base)
{
  int errno;
  long val;
  char *endptr, *str;

  str = nptr;
  errno = 0; /* to distinguish success/failure after call */

  val = strtol(str, &endptr, base);

  /* check for various possible errors */

  /* The strtol() function returns the result of the conversion,
   * unless the value would underflow or overflow.  If an 
   * underflow occurs, strtol() returns LONG_MIN.  If an 
   * overflow occurs, strtol()  returns LONG_MAX.  In both cases,
   * errno is set to ERANGE.  Precisely the same holds for
   * strtoll() (with LLONG_MIN and LLONG_MAX instead of LONG_MIN
   * and LONG_MAX).
   */
  if ((errno == ERANGE && (val == LONG_MIN || val == LONG_MAX)) ||
	 /* The implementation may also set errno to EINVAL in case
	  * no conversion was performed (no digits seen, and 0 returned)
	  */
	  (errno != 0 && val ==0)) {
	perror("strtol");
	exit(EXIT_FAILURE);
  }

   /* If there were no digits at all, strtol() stores the original
   * value of nptr in *endptr (and returns 0).
   */
  if (endptr == str) {
	fprintf(stderr, "No digits were found\n");
	exit(EXIT_FAILURE);
  }

  /* if we got here, strtol() successfully parsed a number */

  /* In particular, if *nptr is not '\0' but **endptr is '\0' on 
   * return, the entire string is valid.
   */
  printf("strtol returned %ld\n", val);

  /* If  endptr  is not NULL, strtol() stores the address of the
   * first invalid character in *endptr.
   */
  if (*endptr != '\0') /* Not necessarily an error... */
	printf("Further characters after number: %s\n\n", endptr);

  return val;
}

int
main (int argc, char **argv)
{
  /* command example:
   *
   * command oldfd newfd
   */ 
  if (argc < 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s oldfd newfd\n", argv[0]);
  
  int oldFdVal, oldFd, newFdVal, newFd;

  /* long is a signed interger type.
   * Cast it to int because in dup command 
   * fd arg has type int.
   * 
   * `
   * #include <unstid.h>
   * dup2(int oldfd, int newfd)
   * `
   * Returns (new) file descriptor on success, or -1 on error.
   */

  /* parse command line argument string and convert it
   * to number(long). The result is casted to integer.
   */
  oldFdVal = (int) getNum(argv[1], 10);
  newFdVal = (int) getNum(argv[2], 10);

  /* The dup2() system call makes a duplicate of the file descriptor
   * given in oldfd using the descriptor number supplied in newfd. 
   * If the file descriptor specified in newfd is already open,
   * dup2() closes it first.
   * (Any error that occurs during this close is silently ignored).
   * The closing and reuse of newfd are performed atomically, which 
   * avoids the possibility that newfd is reused between the two 
   * steps in a signal handler or a parallel thread that allocates
   * a file descriptor.
   */

  /* If oldfd is not a valid file descriptor, then dup2() fails
   * with the error EBADF and newfd is not closed. If oldfd is
   * a valid file descriptor, and oldfd and newfd have the same 
   * value, then dup2() does nothing - newfd is not closed, and
   * dup2() returns the newfd as its function result.
   */

  /* A further interface that provides some extra flexibility for
   * duplicating file descriptors is the fcntl() F_DUPFD operation:
   * 
   * `newfd = fcntl(oldfd, F_DUPFD, startfd);`
   */

  /* (Stackoverflow bookmarked:
   * Q. FIXME How to check if a given file descriptor stored in a variable 
   * is still valid?)
   * 
   * fcntl(fd, F_GETFD) is the canonical cheapest way to check that
   * fd is a valid open file descriptor. If you need to batch-check
   * a lot, using poll with a zero timeout and the events member
   * set to 0 and checking for POLLNVAL in revents after it returns 
   * is more efficient.
   *
   * With that said, the operation "check if a given resource handle 
   * is still valid" is almost always fundamentally incorrect. After
   * a resource handle is freed (e.g. a fd is closed), its value may 
   * be reassigned to the next such resource you allocate. If there 
   * are any remaining references that might be used, they will wrongly 
   * operate on the new resource rather than the old one. Thus, the 
   * real answer is probably: If you don't already know by the logic 
   * of your program, you have major fundamental logic errors that need 
   * to be fixed.
   */

  if (oldFd == newFd)
	printf("%d already open. oldFd and newFd have same value\n");
  exit(EXIT_SUCCESS);

  close(newFd);
  if ((newFd = fcntl(oldFd, F_DUPFD, newFd)) == -1)
	errExit("fcntl");

