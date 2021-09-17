include ../../Makefile.inc

pstree.out: pstree_implementation.o
	$(CC) $(CFLAGS) $^ $(TLPI_INCL_DIR)/error_functions.o $(TLPI_INCL_DIR)/get_num.o -o $@

pstree.o:
