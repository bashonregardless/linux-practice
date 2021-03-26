/* This code is written to test the functionality of
 * initgroups() api.
 *
 * DONE FIXME the code fails at initgroups() call.
 *
 * NOTE make use of errno and other methods available
 * to identify the cause of error.
 */
/* TODO After the above problem is fixed, proceed with the
 * solution to problem.
 */
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define _BSD_SOURCE
#include <grp.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#define groupsize 100
int
main (int argc, char **argv)
{
  gid_t group, grouplist[groupsize];
  int gid_size;
  group = 123;

  /* A privileged process can use setgroups() and initgroups() to 
   * change its set of supplementary group IDs.
   * 
   * Is your process priviledged?
   */
  /* ERRORS:
   *   ENOMEM Insufficient memory to allocate group information structure.
   *   EPERM  The calling process has insufficient privilege.  See the underlying system call setgroups(2)
   */
  if (initgroups(argv[1], group) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  if ((gid_size = getgroups(groupsize, grouplist)) == -1)
	exit(EXIT_FAILURE);

  for (int i = 0; i < gid_size; i++)
	printf("gid: %ld\n", (long) grouplist[i]); 
  exit(EXIT_SUCCESS);
}
