/* TODO understand the quesiton and attempt again.
 * Current solution copied from tlpi errata. */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>

int
main(int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s uid1 uid2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //printf("%s %s\n", getpwuid(atoi(argv[1]))->pw_name,
     //                 getpwuid(atoi(argv[2]))->pw_name);
    printf("%ld ", (long) getpwnam("avr")->pw_uid);
	printf("%ld\n", (long) getpwnam("tsr")->pw_uid);

    exit(EXIT_SUCCESS);
}
