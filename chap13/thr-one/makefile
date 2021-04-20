include ../../Makefile.inc

# TODO remove $(INCLUDEFLAGS) and $(INCLUDEDIRS) variable
# See CFLAGS definition in Makefile.inc
thr-one.out: thr-one.o
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) $(INCLUDEDIRS)/error_functions.o thr-one.o -o $@

thr-one.o: copy.c
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) -DBUF_SIZE=1 -c $^ -o $@

.PHONY: copy_sync
	
copy_sync: copy_sync.out

copy_sync.out: copy_sync.o
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) $(INCLUDEDIRS)/error_functions.o copy_sync.o -o $@

copy_sync.o: copy_sync.c
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(INCLUDEDIRS) -DBUF_SIZE=1024 -c $^ -o $@
