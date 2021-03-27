#include <sys/types.h>
#include <fcntl.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "tlpi_hdr.h"

int
main(int argc, char **argv)
{
  int fd;
  uid_t process_uid;
  printf("%d\n", getuid());

  if ((fd = open(argv[1], O_RDONLY)) == -1)
	printf("%s\n", strerror(errno));
  if (setuid(getuid()) == -1)
	errExit("setuid");
  printf("%d\n", getuid());

  if (close(fd) == -1)
	errExit("close");
}

/*
 * Behavior of this program under various values of IDs:
 *
 * After running make for the first time,
 * Output of ex command `r! ls -n %:h`.
 * This command reads in the output of shell command
 * `ls -n .` 
 * 
total 44
-rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
-rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
-rwxrwxr-x 1 1000 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
-rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
-rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot

* Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`
*
1000
Permission denied
1000
 *
 *
 * Making the executable irrevocablyDropAllPrivileges.out priviledged
 * (i.e, changing its real id to 0)
 * by running shell command `sudo chown root irrevocablyDropAllPrivileges.out`
 *
total 44
-rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
-rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
-rwxrwxr-x 1    0 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
-rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
-rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot
 *
 * Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`
 *
1000
Permission denied
1000
*
*
* Making irrevocablyDropAllPrivileges.out a set-user-id program
* (i.e, turning on its s bit)
* by running shell command `chmod u+s irrevocablyDropAllPrivileges.out`
*
* Output of ex command `r! ls -n %:h`
*
total 44
-rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
-rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
-rwsrwxr-x 1    0 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
-rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
-rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot
*
* Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`
*
1000
1000
*
* NOTICE that "Permission denied" line has disappeared from the output.
* This tells us that the executable file irrevocablyDropAllPrivileges.out
* was successfully able to open the file ownedbyroot.
 */
