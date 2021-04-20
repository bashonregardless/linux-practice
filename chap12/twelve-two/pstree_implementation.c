/* Write a program that draws a tree showing the hierarchical 
 * parent-child relationships of all processes on the system,
 * going all the way back to init.
 */
/* Use information in /proc/PID/status files */

#include <fcntl.h>
#include <dirent.h>

#include "tlpi_hdr.h"

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
  DIR *dirst;
  char *base;
  struct *dirent;
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
  if ((dirst = opendir(base)) == NULL) {
	/* TODO Is it true that I don't have to pass errno
	 * explicitly to errExit().
	 * If the above statement is true, then during dynamic
	 * linking phase this was taken care of.
	 */
	errExit("opendir");
  }

  while (1) {
	snprintf(pathname, PATHNAME_BUF_SIZE, "%s/%s", base, ); 
	if ((dirent = readdir(dirst)) == NULL) {
	  if (errno = 0)
		printf("\n\nEnd of dir strean\n\n");
	  else
		errxit("readdir %s",); 
	}
  }

  exit(EXIT_SUCCESS);
}
