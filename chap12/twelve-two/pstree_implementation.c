/* Write a program that draws a tree showing the hierarchical 
 * parent-child relationships of all processes on the system,
 * going all the way back to init.
 */
/* Use information in /proc/PID/status files */

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#include "tlpi_hdr.h"

#define PATHNAME_BUF_SIZE 256

struct node {
  int children_count;
  pid_t pid;
  pid_t ppid;
  char *proc_name;
  struct node* next;
};

int
main(int args, char **argv)
{
  DIR *procdirst;
  char *base, proc_dirname[PATHNAME_BUF_SIZE],
  pathname[PATHNAME_BUF_SIZE];
  /* Conceptual(C)
   * TODO declaration of string as given in code below
   * `
   * char *proc_dirname, *pathname;
   * `
   * gives warning: proc_dirname, pathname may be uninitialized
   */
  struct dirent *dirst;
  struct stat sb;
  /* TODO without declaration and initialization step
   * `
   * int errno;
   * errno = 0;
   * `
   * errExit() function does the right thing. How?
   *
   * NOTE that following headers are declared in the
   * file tlpi_hdr.h
   * 	errno
   * 	stdlib
   * 	stdio
   * 	unistd
   * 	sys/types
   * 	string
   */
  base = "/proc/";
  if ((procdirst = opendir(base)) == NULL) {
	/* TODO Is it true that I don't have to pass errno
	 * explicitly to errExit()?
	 * If the above statement is true, then during dynamic
	 * linking phase this was taken care of.
	 */
	errExit("opendir");
  }

  while (1) {
	errno = 0;
	/* TODO
	 * NOTE keeping `errno = 0` initalization outside while loop
	 * sets errno and takes path of errExit() in
	 * readdir() error checking code below.
	 * This is probably because, some funtion below in this loop
	 * sets errno to non-zero value.
	 */

	/* Read from directory stream */
	if ((dirst = readdir(procdirst)) == NULL) {
	  if (errno == 0) {
		printf("\n\nEnd of dir strean\n\n");
		break;
	  }
	  else
		errExit("readdir"); 
	}

	if((strcmp(dirst->d_name, ".") == 0) || (strcmp(dirst->d_name, "..") == 0))
	  continue; /* Skip . and .. */

	/* Form /proc/PID directory pathname. Concatenate using snprintf() */
	snprintf(proc_dirname, PATHNAME_BUF_SIZE, "%s%s", base, dirst->d_name);

	if (stat(proc_dirname, &sb) == -1) {
	  printf("Cannot open file.: stat proc_dirname: %s\n", proc_dirname);
	  continue;
	}

	if ((sb.st_mode & S_IFMT) != S_IFDIR) /* Check for file type */
	  continue; /* Skip if not a dir */

	/* Form /proc/PID/status pathname. Concatenate using snprintf() */
	snprintf(pathname, PATHNAME_BUF_SIZE, "%s/status", proc_dirname); 

	if (stat(pathname, &sb) == -1) {
	  printf("Cannot open file.: stat file pathname: %s\n", pathname);
	  continue;
	}

	if ((sb.st_mode & S_IFMT) != S_IFREG) /* Check for file type */
	  continue; /* Skip if not a regular file */

	printf("%s\n", pathname);
  }

  if (closedir(procdirst) == -1)
	errExit("closedir");

  exit(EXIT_SUCCESS);
}
