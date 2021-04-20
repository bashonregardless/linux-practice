include ../../Makefile.inc

.PHONY: lcreds

lcreds: p_id_cmdn_si.out

p_id_cmdn_si.out: p_id_cmdn_si.c
	$(CC) $(CFLAGS) $^ $(TLPI_INCL_DIR)/error_functions.o $(TLPI_INCL_DIR)/get_num.o -o $@

.PHONY: pstree

pstree: pstree.out

pstree.out: pstree_implementation.c
	$(CC) $(CFLAGS) $^ $(TLPI_INCL_DIR)/error_functions.o $(TLPI_INCL_DIR)/get_num.o -o $@
