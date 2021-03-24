#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

int
main (int argc, char *argv[])
{
  if (argc != 2)
	printf("UsageErr: %s loginName\n", argv[0]);
  exit(EXIT_FAILURE);
		
  struct passwd *pw;

  while ((pw = getpwent()) != NULL) {
	if (strcmp(argv[1], pw->pw_name) == 0)
	  printf("%s: ", pw->pw_name);
  }

  endpwent();

  exit(EXIT_SUCCESS);
}  


