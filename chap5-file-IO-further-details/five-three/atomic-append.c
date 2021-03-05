#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>
#include <tlpi_hdr.h>

#define BUF_SIZE 1000000

int
main(int argc, char *argv[])
{
  pid_t pid; 
  pid = getpid();
  printf("pid: [%ld]\n", (long) pid);
  if (argc < 3 || strcmp(argv[1], "--help") == 0)
  {
	usageErr("Usage: %s filename num-bytes [x]\n", argv[0]);
  }

  int fd, openFlags;
  ssize_t numWrites;
  off_t fileOffset;
  char buf[BUF_SIZE];

  //openFlags = 0;
  /* 
   * CAUTION!
   *
   * Property of OR GATE:
   * To keep input unchanged OR it with 0.
   * ORing with 1 results in output of 1, no matter what the input.
   */
  /*
   * CAUTION!
   *
   * `*argv[argc-1] == 'x'` is wrong because
   * *argv[argc-1] will give first character of argument string
   * at position argc-1 in array of pointers argv.
   *
   * See strcmp() for comparing to strings(or character) the right way.
   *
   */
  openFlags = O_RDWR | O_CREAT | ((*argv[argc-1]) == 'x' ? O_APPEND : 0);
  printf("openFlags: %d\n", openFlags);
  printf("addtional argument value: %c\n", *argv[argc-1]);

  /*
   * CAUTION!
   * If file is opened with only perm flag S_IWUSR, then trying
   * to access it will throw 
   * [EACCESS Permission denied] open
   * error.
   */
  if ((fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR)) == -1)
	errExit("open");

  if (*argv[argc-1] == 'x')
  {
	if ((fileOffset = lseek(fd, 0, SEEK_END)) == -1)
	  errExit("seek");
	printf("fileOffset changed to %lld\n", (long long) fileOffset);
  }

  if ((numWrites = write(fd, buf, BUF_SIZE)) == -1)
	errExit("write");
  printf("[%ld]: Written %ld bytes\n", (long) pid, (long) numWrites);

  if(close(fd) == -1)
	errExit("close");

  exit(EXIT_SUCCESS);
}
