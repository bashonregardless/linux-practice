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
#include "get_num.h"
#include "ugid_functions.h"

#include <ctype.h>

void readfdata(char *pathname, uid_t uid);

void trim(char * s);

void trim(char * s) {
  char * p = s;
  int l = strlen(p);

  while(isspace(p[l - 1])) p[--l] = 0;
  while(* p && isspace(* p)) ++p, --l;

  memmove(s, p, l + 1);
}

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

void readfdata(char *pathname, uid_t uid)
{
  pid_t pid;
  char *token, *name;
  char *saveptr;
  long realuid;
  char line[128];
  FILE *fst;

  /* Open file stream */
  if ((fst = fopen(pathname, "r")) == NULL) {
	printf("%s \n", strerror(errno));
	return; /* Continue if fopen fails */
  }

  pid = -1;
  name = NULL;
  realuid = -1;
  /* Do some work on file, a line at a time. */
  while (!(pid != -1 && name != NULL && realuid != -1)
	  && fgets(line, sizeof line, fst) != NULL) {
	/* Tokenize
	 * 	   * delimiter ":" */
	if ((token = strtok_r(line, ":", &saveptr)) == NULL) {
	  printf("No token\n");
	  continue; /* Continue if no token found */
	}

	/* If token is either "Name" or "Uid" */
	if (strcmp(token, "Pid") == 0) {
	  /* Get next token */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Continue if no token found */
	  }
	  trim(token);
	  pid = getInt(token, GN_GT_0, "pid");
	}

	if (strcmp(token, "Name") == 0) {
	  /* Get next token */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Continue if no token found */
	  }
	  name = token;
	}

	if (strcmp(token, "Uid") == 0) {
	  /* Tokenize
	   * delimiter: "\t" */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Continue if no token found */
	  }

	  if ((token = strtok_r(token, "\t", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Continue if no token found */
	  }
	  trim(token);
	  realuid = getInt(token, GN_ANY_BASE, "realuid");

	  if (uid == realuid) {
		if (pid != -1 && name != NULL) {
		  printf("Pid: %ld\nName: %s\n", (long) pid, name);
		  printf("\n\n");
		  break;
		}
	  }
	}
  }
  /* Close file stream */
  if (fclose(fst) == -1)
	errExit("close");
}

int
main (int argc, char **argv)
{
  char pathname[100], dirname[256];
  struct dirent *dirst;
  char *base = "/proc/";
  DIR *procdirstream;
  struct stat sb;
  uid_t uid;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
	usageErr("%s username\n", argv[0]);
  
  uid = -1;
  uid = userIdFromName(argv[1]);

  /* open dir stream */
  if ((procdirstream = opendir(base)) == NULL) {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  while (1) {
	/* To check for errors */
	errno = 0;

	/* read from dir stream	 */
	dirst = readdir(procdirstream);
	if (dirst == NULL)
	  break;
	
	if((strcmp(dirst->d_name, ".") == 0) || (strcmp(dirst->d_name, "..") == 0))
	  continue; /* Skip . and .. */

	/* Form pathname to be used */
	/* TODO is the code portion to allocate size correct? */
	snprintf(pathname, sizeof(dirname),
		"%s%s/status", base, dirst->d_name);

	if (stat(pathname, &sb) == -1) {
	  printf("%s @ stat\n", strerror(errno));
	  continue; /* Continue if stat call fails */
	}

	//printf("%s\n", pathname);

	if ((sb.st_mode & S_IFMT) != S_IFREG) /* Check for file type */
	  continue; /* Continue if not a regular file */

	/* Handle regular file */

	/* Do some work on file, a line at a time. */
	readfdata(pathname, uid);
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

