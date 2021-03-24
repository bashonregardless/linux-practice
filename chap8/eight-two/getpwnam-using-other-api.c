#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

int
main (int argc, char *argv[])
{
  if (argc != 2) {
	printf("UsageErr: %s loginName\n", argv[0]);
	exit(EXIT_FAILURE);
  }
		
  struct passwd *pw;

  while ((pw = getpwent()) != NULL) {
	if (strcmp(argv[1], pw->pw_name) == 0) {
	  printf("pw_name: %s\n", pw->pw_name);
	  printf("password: %s\n", pw->pw_passwd);
	  printf("uid: %ld\n", (long) pw->pw_uid);
	  printf("gid: %ld\n", (long) pw->pw_gid);
	  printf("gecos: %s\n", pw->pw_gecos);
	  printf("home_dir: %s\n", pw->pw_dir);
	  printf("shell: %s\n", pw->pw_shell);
	}
  }

  endpwent();

  exit(EXIT_SUCCESS);
}  


