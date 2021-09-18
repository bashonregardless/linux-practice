#include <stdlib.h>
#include <stdio.h>

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
