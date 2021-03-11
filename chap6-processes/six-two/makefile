# This makefile looks ovedone. If it is, find out the correct approach to
# write makefile for small project like this one?

CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors
INCLUDE_DIRS = ../../tlpi-book/lib/
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRECTORIES))
objs = longjump-into-returned-function.o error_functions.o

longjump-into-returned-function.out: $(objs)
	$(CC) $(CFLAGS) $(CPPFLAGS) longjump-into-returned-function.o ../../tlpi-book/lib/error_functions.o -o $@

longjump-into-returned-function.o: tlpi_hdr.h

error_functions.o: tlpi_hdr.h

VPATH  = ../../tlpi-book/lib/

