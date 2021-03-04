/* This is minimal reproducible code.
 * 
 * Command to produce executable:
 * gcc -Wall -I ../../tlpi-book/lib/ -c tee-practice.c
 * gcc -o executable-name.out program-name.o path/to/tlpi-book/error-functions.o */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define MAX_READ 20

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

static void             /* Print "usage" message and exit */
usageError(char *progName, char *msg, int opt)
{
    if (msg != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
    fprintf(stderr, "Usage: %s [-p arg] [-x]\n", progName);
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
  int fileFd, openFlags, filePerms, opt;
  ssize_t numRead;
  char buffer[MAX_READ + 10], *pstr;

  openFlags = 0;
  pstr = NULL;

  if (argc == 1) {
	usageErr("t-practice: missing filename argument\n");
  }

  // command example: tee_practice tfile
  if (argc > 3 || strcmp(argv[1], "--help") == 0)
	usageErr("Usage error\nDid you supply the filename?\n");

  while((opt = getopt(argc, argv, ":a")) != -1) {
	if (opt == '?')
	  printf("; optopt =%3d (%c)", optopt, printable(optopt));
	printf("\n");

	switch (opt) {
	  case 'a': pstr = optarg;
				openFlags = O_APPEND;
				break;
	  case ':': usageError(argv[0], "Missing argument", optopt);
				break;
	  case '?': usageError(argv[0], "Unrecognized option", optopt);
				break;
	  default:  fatal("Unexpected case in switch()");
	}
  }

  // Source of error(SOE): using |= operator with openFlags.
  // default value of openFlags automatic variable is compiler specific.
  // Don't forget to Initialize openFlags variable
  openFlags |= O_RDWR | O_CREAT;
  filePerms = S_IRUSR | S_IWUSR;
  fileFd = open(argv[argc-1], openFlags, filePerms);

  if (fileFd == -1)
	errExit("open");

  // TODO bytes overflow to shell and get executed as next command
  // Change this code to read everything in the input: see stackoverflow question
  if ((numRead = read(STDIN_FILENO, buffer, MAX_READ)) == -1)
	errExit("read");

  /*(Page 66)
	Printing system data type values:

	When printing values of one of the numeric system data types shown in Table 3-1
	(e.g., pid_t and uid_t), we must be careful not to include a representation depen-
	dency in the printf() call. A representation dependency can occur because C’s argu-
	ment promotion rules convert values of type short to int, but leave values of type int
	and long unchanged. This means that, depending on the definition of the system
	data type, either an int or a long is passed in the printf() call. However, because
	printf() has no way to determine the types of its arguments at run time, the caller
	must explicitly provide this information using the %d or %ld format specifier. The
	problem is that simply coding one of these specifiers within the printf() call creates
	an implementation dependency.

	The usual solution is to use the %ld specifier and
	always cast the corresponding value to long
	*/
  printf("numRead: %ld\n", (long) numRead);
  printf("fileFd: %ld\n", (long) fileFd);

  buffer[numRead] = '\0';

  if (write(STDOUT_FILENO, buffer, MAX_READ) == -1)
	errExit("write");
  //printf("\n");
  
  if (write(fileFd, buffer, MAX_READ) == -1)
	errExit("write");

  exit(EXIT_SUCCESS);
}
