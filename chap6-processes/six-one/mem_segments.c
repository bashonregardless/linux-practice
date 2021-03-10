/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 6-1 */

#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];            /* Uninitialized data segment */
int primes[] = { 2, 3, 5, 7 };  /* Initialized data segment */

static int
square(int x)                   /* Allocated in frame for square() */
{
    int result;                 /* Allocated in frame for square() */

    result = x * x;
    return result;              /* Return value passed via register */
}

static void
doCalc(int val)                 /* Allocated in frame for doCalc() */
{
    printf("The square of %d is %d\n", val, square(val));

    if (val < 1000) {
        int t;                  /* Allocated in frame for doCalc() */

        t = val * val * val;
        printf("The cube of %d is %d\n", val, t);
    }
}

int
main(int argc, char *argv[])    /* Allocated in frame for main() */
{
    static int key = 9973;      /* Initialized data segment */
    static char mbuf[10240000]; /* Uninitialized data segment */
    char *p;                    /* Allocated in frame for main() */

    p = malloc(1024);           /* Points to memory in heap segment */

    doCalc(key);

    exit(EXIT_SUCCESS);
}

/*
 * Outout of shell command `size mem_segments.o` is
 *
 *  text	   data	    bss	    dec	    hex	filename
 *   334	     20	10240000	10240354	 9c4162	mem_segments.o
 *
 *
 * Output of shell command `ls -l mem_segments.o` is
 *
 * -rw-rw-r-- 1 harshvardhan 2.2K Mar  9 17:54 mem_segments.o
 *
 *
 * Output of shell command `ls -l mem_segments.out` is
 *
 * -rwxrwxr-x 1 harshvardhan 19K Mar  9 19:45 mem_segments.out*
 *
 *
 * Although the program contains an array (mbuf) that is around
 * 10MB (NOTE that file/Data sizes are measured in binary system,
 * where 1KB = 1024B) in size, the executable file is much smaller 
 * than this.
 * We observe from the outputs of the two shell commands that
 * the executable file mem_segments.out has size 19K.
 *
 * This is because (Refer Section 6.3 Pg-114):
 * The uninitialized data segment contains global and static variables
 * that are not explicitly initialized. Before starting the program,
 * the system initializes all memory in this segment to 0. For historical 
 * reasons, this is often called the bss segment, a name derived from 
 * an old assembler mnemonic for.block started by symbol.
 * The main reason for placing global and static variables that are
 * initialized into a separate segment from those that are uninitialized
 * is that, when a program is stored on disk, it is not necessary to allocate 
 * space for the uninitialized data. Instead, the executable merely 
 * needs to record the location and size required for the uninitialized 
 * data segment, and this space is allocated by the program loader at 
 * run time.
*/
