#include <unistd.h>
#include "tlpi_hdr.h"

int
main()
{
  uid_t process_uid;
  printf("%d\n", getuid());
  if (setuid(getuid()) == -1)
	errExit("setuid");
}
