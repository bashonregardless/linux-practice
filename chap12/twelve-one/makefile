include ../../Makefile.inc

# Phony target "lcreds" is here only as an illustration
# to the concept of GNU make that
# from shell make can be run as
#
# $ make lcreds
# 
# or, more generally as
#
# $ make [PHONY target name]
.PHONY: lcreds

lcreds: p_id_cmdn_si.out

p_id_cmdn_si.out: p_id_cmdn_si.c
	$(CC) $(CFLAGS) $^ $(TLPI_INCL_DIR)/error_functions.o $(TLPI_INCL_DIR)/get_num.o -o $@
