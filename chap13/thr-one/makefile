include ../../Makefile.inc

thr-one.out: thr-one.o
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) $(INCLUDEDIRS)/error_functions.o thr-one.o -o $@

thr-one.o: copy.c
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) -DBUF_SIZE=1 -c $^ -o $@
