/* Write a program that draws a tree showing the hierarchical 
 * parent-child relationships of all processes on the system,
 * going all the way back to init.
 */
/* Use information in /proc/PID/status files */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
  char *proc_name;
};

int
main(int args, char **argv)
{

  pid_t ppid, pid;
  exit(EXIT_SUCCESS);
}
