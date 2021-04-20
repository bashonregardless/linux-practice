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
  char *base, *proc_dirname, *pathname;
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
	if ((dirst = readdir(procdirst)) == NULL) {
	  if (errno = 0)
		printf("\n\nEnd of dir strean\n\n");
	  else
		errExit("readdir %s", dirst->d_name); 
	}

	snprintf(proc_dirname, PATHNAME_BUF_SIZE, "%s/%s", base, dirst->d_name);

	if (stat(proc_dirname, &sb) == -1) 
	  errExit("stat proc_dirname");

	if ((sb.st_mode & S_IFMT) != S_IFREG) /* Check for file type */
	  continue; /* Continue if not a regular file */

	snprintf(pathname, PATHNAME_BUF_SIZE, "%s/status", proc_dirname); 

	if (stat(pathname, &sb) == -1) 
	  errExit("stat file pathname");

	if ((sb.st_mode & S_IFMT) != S_IFREG) /* Check for file type */
	  continue; /* Continue if not a regular file */

	printf("%s\n", pathname);
	
  }

  exit(EXIT_SUCCESS);
}
