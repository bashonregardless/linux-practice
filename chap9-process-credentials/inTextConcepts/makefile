.SHELL = /bin/sh

CC = gcc
CFLAGS = -std=c99 -D_XOPEN_SOURCE=600 \
	        -D_DEFAULT_SOURCE \
		-g -I${TLPI_INCL_DIR} \
		-pedantic \
		-Wall \
		-W \
		-Wmissing-prototypes \
		-Wno-sign-compare \
		-Wimplicit-fallthrough \
		-Wno-unused-parameter

INCLUDEFLAGS = -I
INCLUDEDIRS =  ../../tlpi-book/lib

irrevocablyDropPrivileges.out: irrevocablyDropPrivileges.o
	$(CC) $(CFLAGS)  $(INCLUDEFLAGS) $(INCLUDEDIRS) $^ $(INCLUDEDIRS)/error_functions.o -o $@

irrevocablyDropPrivileges.o: irrevocablyDropPrivileges.c
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) -c $^
