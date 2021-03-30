#include <fcntl.h>
#include <unistd.h>

#include <dirent.h>
#include <stddef.h>

#include <pwd.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

/* copied code */
uid_t           /* Return UID corresponding to 'name', or -1 on error */
userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0')  /* On NULL or empty string */
        return -1;                      /* return an error */

    u = strtol(name, &endptr, 10);      /* As a convenience to caller */
    if (*endptr == '\0')                /* allow a numeric string */
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

int
main (int argc, char **argv)
{
  int pfd;
  char pathname[100];
  struct dirent *entryp, *result;
  int name_max;
  size_t len;
  char *base = "/proc/";

  /* Since  POSIX.1  does  not  specify  the  size of the d_name field,
   * and other nonstandard fields may precede that field within the dirent
   * structure, portable applications that use readdir_r() should allocate 
   * the buffer whose address is passed in entry as follows:
   */
  /* (Refer Page 217 of tlpi)
   * FIXME Q. Should I also check return value of pathconf() to
   * distinguish between an indeterminate return and an error return?
   */
  name_max = pathconf(base, _PC_NAME_MAX);
  if (name_max == -1)         /* Limit not defined, or error */
	name_max = 255;         /* Take a guess */
  len = offsetof(struct dirent, d_name) + name_max + 1;
  entryp = malloc(len);

  while (1) {
	readdir_r(opendir(base), entryp, &result);
	if (result == NULL)
	  break;

	if((strcmp(result->d_name, ".") != 0) && (strcmp(result->d_name, "..") != 0))
	{
	  /* (Refer Stackoverflow bookmarked:
	   * Convert integer to be used in strcat)
	   */
	  snprintf(pathname, sizeof("/proc/") + sizeof(len) + 1,
		  "%s%d/status", base, *result->d_name);

	  if ((open(pathname, O_RDONLY)) == -1) {
		printf("%s, \n", strerror(errno));
		exit(EXIT_FAILURE);
	  }
	}
	printf("%s\n", pathname);
  }

  exit(EXIT_SUCCESS);
}

