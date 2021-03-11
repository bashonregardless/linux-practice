CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors
INCLUDE_DIRS = ../../tlpi-book/lib/
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
objs = free_and_sbrk.o error_functions.o get_num.o

free_and_sbrk.out: $(objs)
	$(CC) $(CFLAGS) $(CPPFLAGS) free_and_sbrk.o ../../tlpi-book/lib/get_num.o ../../tlpi-book/lib/error_functions.o -o $@
	
free_and_sbrk.o: tlpi_hdr.h

error_functions.o: tlpi_hdr.h

VPATH = ../../tlpi-book/lib/
