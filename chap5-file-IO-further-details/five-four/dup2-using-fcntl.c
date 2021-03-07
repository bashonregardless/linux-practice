#include <fcntl.h>

#include <limits.h>
#include <error.h>

#include <tlpi_hdr.h>

/* long is a signed interger type.
 * What is its range?
 * TODO Q. Does it depend on ISA(machine) (or equivalently,
 * does it vary with architecture)?
 */
long 
getNum(char *nptr, int base)
{
  int errorno;
  long val;
  char *endptr, *str;

  str = nptr;
  errorno = 0; /* to distinguish success/failure after call */

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
  if ((errorno == ERANGE && (val == LONG_MIN || val == LONG_MAX)) ||
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
  
  int oldFd, newFd;
  //long val;

  /* long is a signed interger type.
   * Cast it to int because in dup command 
   * fd arg has type int.
   * 
   * dup2(int oldfd, int newfd) 
   */
  oldFd = (int) getNum(argv[1], 10);
  /* parse command line argument string and convert it
   * to number(long). The result is casted to integer.
   */
}
