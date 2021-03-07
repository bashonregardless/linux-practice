#include <fcntl.h>

#include <unistd.h>

#include <limits.h>
#include <tlpi_hdr.h>

int
main (int argc, char *argv[])
{
  if (argc < 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s oldfd\n", argv[0]);

  // command dup1-using-fcntl oldfd
  int newFd, oldFd;
  char *endptr;
  long val;

  int errorno = 0; /* To distinguish success/failure after call */
  
  //if ((oldFd = (int) strtol(argv[1], endptr, 10)) == 0 && *endptr == argv[1])
	//errExit("Supplied fd value is not an integer");


  /* Converting string to number */

  val = oldFd = (int) strtol(argv[1], &endptr, 10);

  // check for various possible errors
  if ((errorno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errorno != 0 && val == 0)) {
	perror("strol");
	exit(EXIT_FAILURE);
  }

  if (endptr == argv[1]) {
	fprintf(stderr, "No digits were found\n");
	exit(EXIT_FAILURE);
  }

  /* If we got here, strtol() successfully parsed a number */

  printf("strtol() returned %ld\n", val);

  if (*endptr != '\0')
	printf("Further characters after number: %s\n\n", endptr);
  
  

  // duplicate fd given in argument to command using fcntl
  if ((newFd = fcntl(oldFd, F_DUPFD, 3)) == -1)
	errExit("fcntl");
  printf("duplicated fd %d on fd %d\n", oldFd, newFd);

  if (close(newFd) == -1)
	errExit("close");

  exit(EXIT_SUCCESS);
}
