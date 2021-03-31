#include <fcntl.h>
#include <unistd.h>

#include <dirent.h>
#include <stddef.h>
#include <string.h>

#include <errno.h>

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
  struct dirent *dirst;
  int name_max;
  char *base = "/proc/";
  DIR *procdirstream;
  int countdirs;

  /* (See https://www.gnu.org/software/libc/manual/html_node/Reading_002fClosing-Directory.html)
   * 
   * To distinguish between an end-of-directory condition or an error,
   * you must set errno to zero before calling readdir. To avoid 
   * entering an infinite loop, you should stop reading from the directory
   * after the first error.
   */
  errno = 0; 

  if ((procdirstream = opendir(base)) == NULL) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  name_max = pathconf("/proc", _PC_NAME_MAX);
  if (name_max == -1)         /* Limit not defined, or error */
	name_max = 855;         /* Take a guess */

  countdirs = 0;
  while (1) {
	/* (See https://www.gnu.org/software/libc/manual/html_node/Reading_002fClosing-Directory.html)
	 *
	 * readdir_r allows you to provide your own buffer for the struct dirent,
	 * but it is less portable than readdir, and has problems with very long 
	 * filenames (see below). We recommend you use readdir, but do not share 
	 * DIR objects.
	 *
	 * Portability Note: readdir_r is deprecated. It is recommended to use 
	 * readdir instead of readdir_r for the following reasons:
	 */
	dirst = readdir(procdirstream);
	/* Caution: The pointer returned by readdir points to a buffer within 
	 * the DIR object. The data in that buffer will be overwritten by the
	 * next call to readdir. You must take care, for instance, to copy the
	 * d_name string if you need it later.
	 *
	 * Because of this, it is not safe to share a DIR object among multiple 
	 * threads, unless you use your own locking to ensure that no thread calls
	 * readdir while another thread is still using the data from the previous 
	 * call. In the GNU C Library, it is safe to call readdir from multiple 
	 * threads as long as each thread uses its own DIR object.
	 */
	
	/* If there are no more entries in the directory or an error is detected,
	 * readdir returns a null pointer. The following errno error conditions are 
	 * defined for this function:
	 *
	 * EBADF
	 * The dirstream argument is not valid.
	 */
	if (dirst == NULL) {
	  if (errno == 0)
		printf("End of dir stream reached\n");
	  else {
		strerror(errno);
		exit(EXIT_FAILURE);
	  }
	  break;
	}

	if((strcmp(dirst->d_name, ".") != 0) && (strcmp(dirst->d_name, "..") != 0))
	  continue; /* Skip . and .. */

	/* (Refer Stackoverflow bookmarked:
	 * Convert integer to be used in strcat)
	 */
	/* FIXME is the code poriton to allocate size correct? */
	snprintf(pathname, sizeof("/proc/") + sizeof(name_max) + 1,
		"%s%s/status", base, *dirst->d_name);

	if ((pfd = open(pathname, O_RDONLY)) == -1) {
	  printf("%s \n", strerror(errno));
	  exit(EXIT_FAILURE);
	}
	if (close(pfd) == -1) {
	  errExit("close");
	}

	countdirs++;
	printf("%s\n", pathname);
  }

  printf("%d directories found\n", countdirs);
  /* close directory stream */
  if (closedir(procdirstream) == -1) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

