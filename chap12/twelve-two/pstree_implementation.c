/* Write a program that draws a tree showing the hierarchical 
 * parent-child relationships of all processes on the system,
 * going all the way back to init.
 */
/* Use information in /proc/PID/status files */

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>

#include "tlpi_hdr.h"

#define PATHNAME_BUF_SIZE 256

struct node {
  int children_count;
  pid_t pid;
  pid_t ppid;
  char *name;
  struct node *next;
};

/*
 * TODO:
 * Create Process P1
 * Scan some dirs
 * Create process P2, a child of P1
 * Scan some more dirs
 * Create another process P2, a child of P1
 * Scan some more dirs
 * Terminate P2 and the P1. Also Terminate P1 and then P2
 * Scan remaning dirs
 * Print tree
 *
 *
 * DATA:
 * The data used to lookup a node may change between the time 
 * a lookup is performed on the node and an operation
 * (accessing a file, inserting it to data structure) is performed
 * on the node . Data is in the form of directory stream.
 * The name of directories in this stream can be accessed. To
 * do this and to call next directory in the stream we make a 
 * readdir() sys call.
 *
 * Directory names coincide with the PIDs of the processes, 
 * which can be found in file /proc/PID/status.
 *
 * Directory names pose a particular problem:
 * They can be adjusted to any value upto 2^22. Verify on your sys?
 * making it possible to accomodate very large number of processes.
 *
 * There is a possibility that a processes parent (and thus, the
 * /proc/PID directory disappears during the scan of all
 * /proc/PID directories.
 *
 * Q. Which data structure to use to handle data of such nature?
 *
 * Choice of data structures:
 *  
 * Operation on the node include:
 * 	search
 * 	delete
 * 	insert
 * 	update
 */
/* lookupnode returns pointer to node if found, otherwise
 * it returns -1.
 */
int lookupnode(pid_t);

//struct node root = NULL;

struct node *addnode(pid_t, pid_t, char *);

void readfdata(char *);

void trim(char *);

int lookupnode(pid_t pid) { }

/* copied code */
void trim(char * s) 
{
  char * p = s;
  int l = strlen(p);

  while(isspace(p[l - 1])) p[--l] = 0;
  while(* p && isspace(* p)) ++p, --l;

  memmove(s, p, l + 1);
}

struct node *addnode(pid_t pid, pid_t ppid, char *name)
{
  struct node *pproc_node;

  if ((pproc_node = malloc(sizeof(struct node))) == NULL)
	errExit("malloc in addnode()");
  
  pproc_node->pid = pid;
  pproc_node->ppid = ppid;
  pproc_node->name = name;

  pproc_node->next = NULL;

  return pproc_node;
}

void readfdata(char *pathname)
{
  pid_t ppid, pid;
  char *token;
  char *saveptr;
  char line[128];
  FILE *fst;

  /* Open file stream */
  if ((fst = fopen(pathname, "r")) == NULL) {
	printf("%s \n", strerror(errno));
	return; /* Continue if fopen fails */
  }

  pid = -1;
  ppid = -1;
  /* Do some work on file, a line at a time. */
  while (fgets(line, sizeof line, fst) != NULL) {
	/* Tokenize
	 * delimiter ":" */
	if ((token = strtok_r(line, ":", &saveptr)) == NULL) {
	  printf("No token\n");
	  continue; /* Skip if no token found */
	}

	/* If token is Pid */
	if (strcmp(token, "Pid") == 0) {
	  /* Get next token */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Skip if no token found */
	  }
	  trim(token);
	  pid = getInt(token, GN_ANY_BASE, "pid");
	  printf("Pid: string %s or integer %ld\n", token, (long) pid);
	  //addnode(pid, ppid, name);
	}

	/* If token is PPid */
	if (strcmp(token, "PPid") == 0) {
	  /* Get next token */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Skip if no token found */
	  }
	  trim(token);
	  ppid = getInt(token, GN_ANY_BASE, "ppid");
	  printf("PPid: string %s or integer %ld\n", token, (long) ppid);
	}

	/* If token is "Name"*/
	if (strcmp(token, "Name") == 0) {
	  /* Get next token */
	  if ((token = strtok_r(NULL, ":", &saveptr)) == NULL) {
		printf("No token\n");
		continue; /* Skip if no token found */
	  }
	  trim(token);
	  printf("Name: %s\n", token);
	}
  }
  /* Close file stream */
  if (fclose(fst) == -1)
	errExit("close");
  printf("Child Pid: %ld\n", (long) getpid());
}

int
main(int args, char **argv)
{
  DIR *procdirst;
  char *base, proc_dirname[PATHNAME_BUF_SIZE],
  pathname[PATHNAME_BUF_SIZE];
  /* Conceptual(C)
   * TODO declaration of string as given in code below
   * `
   * char *proc_dirname, *pathname;
   * `
   * gives warning: proc_dirname, pathname may be uninitialized
   */
  struct dirent *dirst;
  struct stat sb;
  int dircount, *status;
  pid_t childPid;

  /* TODO without declaration and initialization step
   * `
   * int errno;
   * errno = 0;
   * `
   * errExit() function does the right thing. How?
   *
   * NOTE that following headers are declared in the
   * file tlpi_hdr.h
   * 	errno
   * 	stdlib
   * 	stdio
   * 	unistd
   * 	sys/types
   * 	string
   */

  base = "/proc/";
  if ((procdirst = opendir(base)) == NULL) {
	/* TODO Is it true that I don't have to pass errno
	 * explicitly to errExit()?
	 * If the above statement is true, then during dynamic
	 * linking phase this was taken care of.
	 */
	errExit("opendir");
  }

  switch (childPid = fork()) {
	case -1:
	  errExit("fork");

	case 0:
	  printf("Inside child\n");
	  printf("Child Pid: %ld\n", (long) getpid());
	  break;

	default:
	  printf("Inside parent\n");
	  waitpid(childPid, status, 0);
  }
  
  dircount = 0;
  errno = 0;
  while ((dirst = readdir(procdirst)) != NULL) { /* Read from directory stream */
	if((strcmp(dirst->d_name, ".") == 0) || (strcmp(dirst->d_name, "..") == 0))
	  continue; /* Skip . and .. */

	/* Form /proc/PID directory pathname. Concatenate using snprintf() */
	snprintf(proc_dirname, PATHNAME_BUF_SIZE, "%s%s", base, dirst->d_name);

	if (stat(proc_dirname, &sb) == -1) { /* NOTE stat syscall also sets errno */
	  printf("stat proc_dirname: %s \tError: %s\n", proc_dirname, strerror(errno));
	  continue;
	}

	if ((sb.st_mode & S_IFMT) != S_IFDIR) /* Check for file type */
	  continue; /* Skip if not a dir */

	/* Form /proc/PID/status pathname. Concatenate using snprintf() */
	snprintf(pathname, PATHNAME_BUF_SIZE, "%s/status", proc_dirname); 

	if (stat(pathname, &sb) == -1) {
	  printf("stat pathname: %s \tError: %s\n", pathname, strerror(errno));
	  continue;
	}

	if ((sb.st_mode & S_IFMT) != S_IFREG) /* Check for file type */
	  continue; /* Skip if not a regular file */

	printf("%s\n", pathname);
	
	if (dircount == 20) {
	  printf("Child Pid: %ld\n", (long) childPid);
	}

	/* Do something with file */

	readfdata(pathname);
	printf("\n");

	dircount++;

	/* TODO Is it really required to set errno back to zero after having
	 * moved readdir() in while conditional?
	 */
	//errno = 0; 
	/* Reset errno because
	NOTE errno could also be set by api such as stat() */
	/* TODO DONE
	 * * NOTE keeping `errno = 0` initalization outside while loop
	 * sets errno and takes path of errExit() in
	 * readdir() termination code below.
	 * This is probably because, some funtion (for e.g, stat()) below 
	 * in this loop sets errno to non-zero value.
	 * DONE: Moving readdir() sys call in while conditional and readdir()
	 * termination code outside while loop solves this problem.
	 */
  } 

  /* Hanlde termination of readdir() */
  if (errno == 0) /* End of dir stream reached */
	printf("\n\nEnd of dir strean\n\n");
  else
	errExit("readdir"); 

  if (closedir(procdirst) == -1)
	errExit("closedir");

  exit(EXIT_SUCCESS);
}
