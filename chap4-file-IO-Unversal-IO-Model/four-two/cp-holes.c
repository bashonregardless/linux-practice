/* 
 * The current program does a simple copy from one file to another.
 * TODO Complete the program to copy holes as well.
 */

#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include "tlpi_hdr.h"

int
main (int argc, char *argv[])
{
  int srcFd, tgtFd, srcOpenFlags, tgtOpenFlags,
	  tgtPerms;

  ssize_t numRead, numWrite;

  off_t tgtCurOffset, tgtEnd;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
	usageErr("%s old-file new-file\n", argv[0]);

  srcOpenFlags = O_RDONLY;
  tgtOpenFlags = O_CREAT | O_WRONLY;
  tgtPerms = S_IWUSR;
  
  // open fd to source file
  if ((srcFd = open(argv[argc-2], srcOpenFlags)) == -1)
	errExit("open source");

  // open fd to target file
  if ((tgtFd = open(argv[argc-1], tgtOpenFlags, tgtPerms)) == -1)
	errExit("open target");

  /*
	//The following call retrieves the current location of the file offset without
	//changing it
	curr = lseek(fd, 0, SEEK_CUR);

	// file offset is moved to last byte of file.
	lseek(fd, -1, SEEK_END);
	*/
  /* get length of file.
	 NOTE that this moves offset of the file.
	 To keep the offset same, restore it to previous value
	 */
  // get current offset
  if ((tgtCurOffset = lseek(srcFd, 0, SEEK_CUR)) == -1)
	errExit("seek curr");
  /* 
	(Page 66)
	Printing system data type values:
	We make one exception to the technique of Printing system data type values.
	Because the off_t data type is the size of long long in some compilation 
	environments, we cast off_t values to this type and use the %lld specifier.
	*/
  printf("tgtCurOffset: %lld\n", (long long) tgtCurOffset);

  // get file size
  if ((tgtEnd = lseek(srcFd, 0, SEEK_END))== -1)
	errExit("seek end");
  printf("tgtEnd: %lld\n", (long long) tgtEnd);

  // restore offset
  if ((tgtCurOffset = lseek(srcFd, tgtCurOffset - tgtEnd, SEEK_END)) == -1)
	errExit("seek restore curr");
  printf("tgtCurOffset(resotred): %lld\n", (long long) tgtCurOffset);

  /* 
	(Stackoverflow bookmarked: How can I get a file's size in C?)
	Q. How can I get a file's size in C? 
	A. 
	 1. Using standard library:
	 Assuming that your implementation meaningfully supports SEEK_END:

	 fseek(f, 0, SEEK_END); // seek to end of file
	 size = ftell(f); // get current file pointer
	 fseek(f, 0, SEEK_SET); // seek back to beginning of file
	 // proceed with allocating memory and reading the file

	 2. Linux/POSIX:
	 You can use stat (if you know the filename), or fstat (if you have the file descriptor).

	 Here is an example for stat:

	 #include <sys/stat.h>
	 struct stat st;
	 stat(filename, &st);
	 size = st.st_size;
	 */

  char buffer[tgtEnd];
	
  /* Transfer data until we encounter end of input or an error */
  while((numRead = read(srcFd, buffer, tgtEnd)) > 0)
	if ((numWrite = write(tgtFd, buffer, tgtEnd)) == -1)
	  fatal("couldn't write whole buffer");
  if (numRead == -1)
	errExit("read");

  /* 
   * It is usually good practice to close unneeded file descriptors explicitly, since this
   * makes our code more readable and reliable in the face of subsequent modifica-
   * tions. Furthermore, file descriptors are a consumable resource, so failure to close a
   * file descriptor could result in a process running out of descriptors. This is a partic-
   * ularly important issue when writing long-lived programs that deal with multiple
   * files, such as shells or network servers.
   */

  /*
   * Just like every other system call, a call to close() should be bracketed with error-
   * checking code.
   *
   * This catches errors such as attempting to close an unopened file descriptor or close
   * the same file descriptor twice, and catches error conditions that a specific file sys-
   * tem may diagnose during a close operation.
   */
  // close fds
  if (close(srcFd) == -1)
	errExit("close srcFd");
  if (close(tgtFd) == -1)
	errExit("close tgtFd");

  exit(EXIT_SUCCESS);
}
