#include <fcntl.h>
#include <unistd.h>

#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>

#include <pwd.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

#include <ctype.h>

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
  int pfd, dfd, countdirs;
  char pathname[100], dirname[256];
  struct dirent *dirst;
  char *base = "/proc/";
  DIR *procdirstream;
  FILE *fst;
  struct stat sb;
  uid_t uid;
  char line[128];
  char *token;
  char *saveptr;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s username\n", argv[0]);
  
  uid = userIdFromName(argv[1]);

  if ((procdirstream = opendir(base)) == NULL) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  if ((dfd = dirfd(procdirstream)) == -1) {
	exit(EXIT_FAILURE);
  }

  countdirs = 0;
  while (1) {
	/* (See https://www.gnu.org/software/libc/manual/html_node/Reading_002fClosing-Directory.html)
	 * 
	 * To distinguish between an end-of-directory condition or an error,
	 * you must set errno to zero before calling readdir. To avoid 
	 * entering an infinite loop, you should stop reading from the directory
	 * after the first error.
	 */
	errno = 0;

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
	if (dirst == NULL)
	  break;
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
	if((strcmp(dirst->d_name, ".") == 0) || (strcmp(dirst->d_name, "..") == 0))
	  continue; /* Skip . and .. */

	/* (Refer Stackoverflow bookmarked:
	 * Convert integer to be used in strcat)
	 */
	/* TODO is the code portion to allocate size correct? */
	snprintf(pathname, sizeof(dirname),
		"%s%s/status", base, dirst->d_name);

	if (stat(pathname, &sb) == -1) {
	  printf("%s stat\n", strerror(errno));
	  continue; /* Continue if stat call fails */
	}

	printf("%s\n", pathname);

	/* FIXME conditional check fails for file "/proc/fb/" */
	if ((sb.st_mode & S_IFMT) == S_IFREG) {
	  /* Handle  file */
	  if ((fst = fopen(pathname, "r")) == NULL) {
		printf("%s \n", strerror(errno));
		continue; /* Continue if fopen fails */
	  }

	  /* Do some work on file, a line at at time. */
	  while (fgets(line, sizeof line, fst) != NULL) {
		/* Tokenize
		 * delimiter ":" */
		if ((token = strtok_r(line, ":", &saveptr)) == NULL) {
		  printf("No token\n");
		  continue; /* Continue if no token found */
		}

		/* If token is either "Name" or "Uid" */
		if (strcmp(token, "Uid") == 0 || strcmp(token, "Name") == 0) {
		  printf("%s: ", token);

		  /* Get next token */
		  token = strtok_r(NULL, ":", &saveptr);

		  printf("%s\n", token);
		}
	  }

	  /* Close file stream */
	  if (fclose(fst) == -1)
		errExit("close");

	  printf("\n\n");
	}
  }

  /* check  any  error */
  if (errno != 0) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  /* close directory stream */
  if (closedir(procdirstream) == -1) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

