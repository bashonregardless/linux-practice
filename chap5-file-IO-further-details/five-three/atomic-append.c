#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>
#include <tlpi_hdr.h>

#define BUF_SIZE 1000000

int
main(int argc, char *argv[])
{
  printf("pid: [%ld]\n", (long) getpid());
  if (argc < 3 || strcmp(argv[1], "--help") == 0)
  {
	usageErr("Usage: %s filename num-bytes [x]\n", argv[0]);
  }

  int fd, openFlags;
  ssize_t numWrites;
  off_t fileOffset;
  //pid_t pid; 
  char buf[BUF_SIZE];

  //openFlags = 0;
  openFlags = O_RDWR | O_CREAT | ((*argv[argc-1]) == 'x' ? O_APPEND : 1);

  /*
   * CAUTION!
   * If file is opened with only perm flag S_IWUSR, then trying
   * to access it will throw 
   * EACCESS Permission denied] open
   * error.
   */
  if ((fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR)) == -1)
	errExit("open");

  /*
   * CAUTION!
   *
   * `*argv[argc-1] == 'x'` is wrong because
   * *argv[argc-1] will give first character of argument string
   * at position argc-1.
   *
   * See strcmp()
   */
  if (*argv[argc-1] == 'x')
  {
	if ((fileOffset = lseek(fd, 0, SEEK_END)) == -1)
	  errExit("seek");
	printf("fileOffset changed to %lld\n", (long long) fileOffset);
  }

  if ((numWrites = write(fd, buf, BUF_SIZE)) == -1)
	errExit("write");
  printf("Written %ld bytes\n", (long) numWrites);

  if(close(fd) == -1)
	errExit("close");

  exit(EXIT_SUCCESS);
}
