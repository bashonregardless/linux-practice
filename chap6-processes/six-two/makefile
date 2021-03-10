CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors
INCLUDE_DIRECTORIES = ../../tlpi-book/lib/
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRECTORIES))
objs = longjump-into-returned-function.o error-functions.o

longjump-into-returned-function.out: $(objs)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(objs) -o $@

longjump-into-returned-function.o: tlpi_hdr.h

error-functions.o: tlpi_hdr.h

VPATH  = ../../tlpi-book/lib/
vpath %.h ../../tlpi-book/lib/

