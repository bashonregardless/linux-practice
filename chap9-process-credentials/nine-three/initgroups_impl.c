/* This code is written to test the functionality of
 * initgroups() api.
 *
 * FIXME the code fails at initgroups() call.
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

#define groupsize 100
int
main (int argc, char **argv)
{
  gid_t group, grouplist[groupsize];
  int gid_size;
  group = 123;
  if (initgroups(argv[1], group) == -1)
	exit(EXIT_FAILURE);
  if ((gid_size = getgroups(groupsize, grouplist)) == -1)
	exit(EXIT_FAILURE);

  for (int i = 0; i < gid_size; i++)
	printf("gid: %ld\n", (long) grouplist[i]); 
  exit(EXIT_SUCCESS);
}
