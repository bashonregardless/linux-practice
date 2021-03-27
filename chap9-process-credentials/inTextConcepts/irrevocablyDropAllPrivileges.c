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
}

/*
 * Behavior of this program under various values of IDs:
 *
 */
