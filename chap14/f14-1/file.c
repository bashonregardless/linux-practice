/*
 * filesize: 1B
 * filename format: xNNNNNN where NNNNNN is a six digit number.
 * create in random order, delete in increasing numerical order.
 *
 * command: `progname NF dirname`
 *
 * find:
 * time req for diff value of NF (#files) and
 * time req for diff file systems?
 *
 * read q...
 */

#include <unistd.h>
#include <fcntl.h>

#include <dirent.h>

int
main (int argc, char **argv)
{
  char *dirname = argv[1];
  
  if ((dirstream = opendir(dirname)) == NULL)  {
	strerror(errno);
	exit(EXIT_FAILURE);
  }

  openfg = O_CREAT | O_RDONLY;

}
