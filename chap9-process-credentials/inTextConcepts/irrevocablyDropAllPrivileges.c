#include <sys/types.h>
#include <fcntl.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
  int fd;
  ssize_t numRead;

  off_t tgtCurOffset, tgtEnd;

  printf("getuid() return value:%d, "
	  "geteuid() return value:%d, "
	  "Before 1st open\n\n", getuid(), geteuid());

  if ((fd = open(argv[1], O_RDONLY)) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  printf("After call to first open()\n\n");

  /* get length of file.  */
  // get current offset
  if ((tgtCurOffset = lseek(fd, 0, SEEK_CUR)) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  // get file size
  if ((tgtEnd = lseek(fd, 0, SEEK_END))== -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }
  // tgtEnd is length of file.

  // restore offset
  if ((tgtCurOffset = lseek(fd, tgtCurOffset - tgtEnd, SEEK_END)) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  char *buf[tgtEnd];
  // read from owned by root file
  if ((numRead = read(fd, buf, tgtEnd)) == -1) 
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  if (write(STDOUT_FILENO, buf, numRead) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  if (setuid(getuid()) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  printf("getuid() return value:%d, "
	  "geteuid() api return value:%d, "
	  "Before 2nd open\n", getuid(), geteuid());

  if ((fd = open(argv[1], O_RDONLY)) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }

  printf("getuid() return value:%d, "
	  "geteuid() api return value:%d, "
	  "After 2nd open\n", getuid(), geteuid());

  if (close(fd) == -1)
  {
	printf("%s\n", strerror(errno));
	exit(EXIT_FAILURE);
  }
}

/*
   Behavior of this program under various values of IDs:

   After running make for the first time,
   Output of ex command `r! ls -n %:h`.
   This command reads in the output of shell command
   `ls -n .` 

   total 44
   -rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
   -rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
   -rwxrwxr-x 1 1000 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
   -rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
   -rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot

   Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

   1000
   Permission denied
   1000


   Making the executable irrevocablyDropAllPrivileges.out priviledged
   (i.e, changing its real id to 0)
   by running shell command `sudo chown root irrevocablyDropAllPrivileges.out`

   total 44
   -rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
   -rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
   -rwxrwxr-x 1    0 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
   -rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
   -rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot

   Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

   1000
   Permission denied
   1000


   Making irrevocablyDropAllPrivileges.out a set-user-id program
   (i.e, turning on its s bit)
   by running shell command `chmod u+s irrevocablyDropAllPrivileges.out`

   Output of ex command `r! ls -n %:h`

   total 44
   -rw-rw-r-- 1 1000 1000   446 Mar 27 15:20 irrevocablyDropAllPrivileges.c
   -rw-rw-r-- 1 1000 1000  7200 Mar 27 16:28 irrevocablyDropAllPrivileges.o
   -rwsrwxr-x 1    0 1000 20736 Mar 27 16:28 irrevocablyDropAllPrivileges.out
   -rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
   -rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot

   Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

   1000
   1000

   NOTICE that "Permission denied" line has disappeared from the output.
   This tells us that the executable file irrevocablyDropAllPrivileges.out
   was successfully able to open the file ownedbyroot.




   After running make again.



   Output of ex command `r! ls -n %:h`

total 52
-rw-rw-r-- 1 1000 1000  3754 Mar 27 17:59 irrevocablyDropAllPrivileges.c
-rw-rw-r-- 1 1000 1000  9128 Mar 27 17:59 irrevocablyDropAllPrivileges.o
-rwsrwxr-x 1 1000 1000 25272 Mar 27 17:59 irrevocablyDropAllPrivileges.out
-rw-rw-r-- 1 1000 1000   579 Mar 27 14:40 makefile
-rw------- 1    0 1000    85 Mar 27 14:25 ownedbyroot

Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

1000
Permission denied
ERROR [EACCES Permission denied] open source


  Making the executable irrevocablyDropAllPrivileges.out priviledged
(i.e, changing its real id to 0)
  by running shell command `sudo chown root irrevocablyDropAllPrivileges.out`


  Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

  1000
  Permission denied
  ERROR [EACCES Permission denied] open source

  NOTE that a program (irrevocablyDropAllPrivileges.out) owned by root
  was still not able to open fd to file ownedbyroot.


  Making irrevocablyDropAllPrivileges.out a set-user-id program.

  Output of command `./irrevocablyDropAllPrivileges.out ownedbyroot`

  I am owned by root. A non priviledged program should not be able to read my content.
  1000
  1000

  NOTE that a program (irrevocablyDropAllPrivileges.out) owned by root
  was able to open fd to file ownedbyroot only after it became a
  set-user-id program.

  */
