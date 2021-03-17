# 10.3 Variables Used by Implicit Rules
#
# The recipes in built-in implicit rules make liberal use of certain
# predefined variables. You can alter the values of these variables 
# in the makefile, with arguments to make, or in the environment 
# to alter how the implicit rules work without redefining the rules
# themselves. You can cancel all variables used by implicit rules 
# with the ‘-R’ or ‘--no-builtin-variables’ option.

# (16.1 General Conventions for Makefiles)
#
# Every Makefile should contain this line:
#
# `SHELL = /bin/sh`
#
# to avoid trouble on systems where the SHELL variable might be 
# inherited from the environment. (This is never a problem with 
# GNU make.)
SHELL = /bin/sh
# Different make programs have incompatible suffix lists and
# implicit rules, and this sometimes creates confusion or misbehavior.
# So it is a good idea to set the suffix list explicitly using
# only the suffixes you need in the particular Makefile, like this:
#
# `
# .SUFFIXES:
# .SUFFIXES: .c .o
# `
#
# The first line clears out the suffix list, the second introduces
# all suffixes which may be subject to implicit rules in this 
# Makefile.
.SUFFIXES:#clears out the suffix list
.SUFFIXES: .c .o

# (16.1 General Conventions for Makefiles)
#
# Don't assume that . is in the path for command execution.
# When you need to run programs that are a part of your package 
# during the make, please make sure that it uses ./ if the 
# program is built as part of the make or $(srcdir)/ if the 
# file is an unchanging part of the source code. Without 
# one of these prefixes, the current search path is used.
#
# The distinction between ./ (the build directory) and $(srcdir)/
# (the source directory) is important because users can build 
# in a separate directory using the '--srcdir' option to configure.
# A rule of the form:
#
# `
# foo.1 : foo.man sedscript
#         sed -f sedscript foo.man > foo.1
# `
#
# will fail when the build directory is not the source directory,
# because foo.man and sedscript are in the source directory.
#
# When using GNU make, relying on 'VPATH' to find the source file 
# will work in the case where there is a single dependency file,
# since the make automatic variable '$<' will represent the source 
# file wherever it is. (Many versions of make set '$<' only in 
# implicit rules.) A Makefile target like
#
# `
# foo.o : bar.c
#         $(CC) -I. -I$(srcdir) $(CFLAGS) -c bar.c -o foo.o
# `
#
# should instead be written as
#
# `
# foo.o : bar.c
#         $(CC) -I. -I$(srcdir) $(CFLAGS) -c $< -o $@
# `
#
# in order to allow 'VPATH'  to work correctly. When the target
# has multiple dependencies, using an explicit '$(srcdir)'
# is the easiest way to make the rule work well. For example, 
# the target above for foo.1 is best written as:
#
# `
# foo.1 : foo.man sedscript
#         sed -f $(srcdir)/sedscript $(srcdir)/foo.man > $@
# `
CC = gcc

# (REFER 16.3 Variables for Specifying Commands)
# If there are C compiler options that must be used for proper 
# compilation of certain files, do not include them in CFLAGS.
# Users expect to be able to specify CFLAGS freely themselves.
# Instead, arrange to pass the necessary options to the C compiler
# independently of CFLAGS, by writing them explicitly in the 
# compilation commands or by defining an implicit rule, like this:
#
# `
# CFLAGS = -g
# ALL_CFLAGS = -I. $(CFLAGS)
# .c.o:
#         $(CC) -c $(CPPFLAGS) $(ALL_CFLAGS) $<
#`
# Put CFLAGS last in the compilation command, after other variables
# containing compiler options, so the user can use CFLAGS to
# override the others.
#
# CFLAGS should be used in every invocation of the C compiler, 
# both those which do compilation and those which do linking.


# Every Makefile should define the variable INSTALL, which
# is the basic command for installing a file into the system.
#
# Every Makefile should also define the variables INSTALL_PROGRAM
# and INSTALL_DATA. (The default for INSTALL_PROGRAM should 
# be $(INSTALL); the default for INSTALL_DATA should be 
# ${INSTALL} -m 644.) Then it should use those variables as the
# commands for actual installation, for executables and non-executables
# respectively. Minimal use of these variables is as follows:
#
#`
# $(INSTALL_PROGRAM) foo $(bindir)/foo
# $(INSTALL_DATA) libfoo.a $(libdir)/libfoo.a
# `
CFLAGS = -g
ALL_CFLAGS = -Wall -Wextra -pedantic-errors

INCLUDE_DIRS = ../../tlpi-book/lib/

# (REFER 16.3 Variables for Specifying Commands)
# Use CPPFLAGS in any compilation command that runs the preprocessor
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
objs = free_and_sbrk.o error_functions.o get_num.o

# (Refer 4.4.3 The Function wildcard)
#
# Thus, a makefile to compile all C source files in the directory
# and then link them together could be written as follows:
#
# `
# objects := $(patsubst %.c,%.o,$(wildcard *.c))
#
# foo : $(objects)
#         cc -o foo $(objects)
# `
#
# This takes advantage of the implicit rule for compiling C programs,
# so there is no need to write explicit rules for compiling the files. 
free_and_sbrk.out: $(objs)
	$(CC) $(CFLAGS) $(CPPFLAGS) free_and_sbrk.o ../../tlpi-book/lib/get_num.o ../../tlpi-book/lib/error_functions.o -o $@
	
free_and_sbrk.o: tlpi_hdr.h

error_functions.o: tlpi_hdr.h

VPATH = ../../tlpi-book/lib/
