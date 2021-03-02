/* This is minimal reproducible code.
 * 
 * Command to produce executable:
 * gcc program-name.c */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
//#include "tlpi_hdr.h"

#define MAX_READ 20

#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H


#ifdef __GNUC__

    /* This macro stops 'gcc -Wall' complaining that "control reaches
       end of non-void function" if we use the following functions to
       terminate main() or some other non-void function. */

#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

#endif

void errMsg(const char *format, ...);

void errExit(const char *format, ...) NORETURN ;

void usageErr(const char *format, ...) NORETURN ;

typedef enum { FALSE, TRUE } Boolean;

static char *ename[] = {
    /*   0 */ "", 
    /*   1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO", 
    /*   7 */ "E2BIG", "ENOEXEC", "EBADF", "ECHILD", 
    /*  11 */ "EAGAIN/EWOULDBLOCK", "ENOMEM", "EACCES", "EFAULT", 
    /*  15 */ "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV", 
    /*  20 */ "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE", 
    /*  25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE", 
    /*  30 */ "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE", 
    /*  35 */ "EDEADLK/EDEADLOCK", "ENAMETOOLONG", "ENOLCK", "ENOSYS", 
    /*  39 */ "ENOTEMPTY", "ELOOP", "", "ENOMSG", "EIDRM", "ECHRNG", 
    /*  45 */ "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG", "EUNATCH", 
    /*  50 */ "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO", 
    /*  56 */ "EBADRQC", "EBADSLT", "", "EBFONT", "ENOSTR", "ENODATA", 
    /*  62 */ "ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE", 
    /*  67 */ "ENOLINK", "EADV", "ESRMNT", "ECOMM", "EPROTO", 
    /*  72 */ "EMULTIHOP", "EDOTDOT", "EBADMSG", "EOVERFLOW", 
    /*  76 */ "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD", 
    /*  81 */ "ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART", 
    /*  86 */ "ESTRPIPE", "EUSERS", "ENOTSOCK", "EDESTADDRREQ", 
    /*  90 */ "EMSGSIZE", "EPROTOTYPE", "ENOPROTOOPT", 
    /*  93 */ "EPROTONOSUPPORT", "ESOCKTNOSUPPORT", 
    /*  95 */ "EOPNOTSUPP/ENOTSUP", "EPFNOSUPPORT", "EAFNOSUPPORT", 
    /*  98 */ "EADDRINUSE", "EADDRNOTAVAIL", "ENETDOWN", "ENETUNREACH", 
    /* 102 */ "ENETRESET", "ECONNABORTED", "ECONNRESET", "ENOBUFS", 
    /* 106 */ "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS", 
    /* 110 */ "ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH", 
    /* 114 */ "EALREADY", "EINPROGRESS", "ESTALE", "EUCLEAN", 
    /* 118 */ "ENOTNAM", "ENAVAIL", "EISNAM", "EREMOTEIO", "EDQUOT", 
    /* 123 */ "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED", "ENOKEY", 
    /* 127 */ "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED", 
    /* 130 */ "EOWNERDEAD", "ENOTRECOVERABLE", "ERFKILL", "EHWPOISON"
};

#define MAX_ENAME 133

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif

static void
terminate(Boolean useExit3)
{
    char *s;

    /* Dump core if EF_DUMPCORE environment variable is defined and
       is a nonempty string; otherwise call exit(3) or _exit(2),
       depending on the value of 'useExit3'. */

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void
outputError(Boolean useErr, int err, Boolean flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);       /* Flush any pending stdout */
    fputs(buf, stderr);
    fflush(stderr);           /* In case stderr is not line-buffered */
}

void
usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);           /* Flush any pending stdout */

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);           /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

void
errExit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

int
main(int argc, char *argv[])
{
  int fileFd;
  ssize_t numRead;
  char buffer[MAX_READ + 10];

  // command example: tee_practice tfile
  if (argc != 2 || strcmp(argv[1], "--help") == 0)
	usageErr("Usage error\nDid you supply the filename?\n");

  fileFd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if (fileFd == -1)
	errExit("open");

  // TODO bytes overflow to shell and get executed as next command
  // Change this code to read everything in the input: see stackoverflow question
  if ((numRead = read(STDIN_FILENO, buffer, MAX_READ)) == -1)
	errExit("read");

  printf("numRead: %ld\n", numRead);

  buffer[numRead] = '\0';

  if (write(STDOUT_FILENO, buffer, MAX_READ) == -1)
	errExit("write");
  
  if (write(fileFd, buffer, MAX_READ) == -1)
	errExit("write");

  exit(EXIT_SUCCESS);
}
