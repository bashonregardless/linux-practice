# Makefile to build all programs in all subdirectories
#
# DIRS is a list of all subdirectories containing makefiles
# (The library directory is first so that the library gets built first)
#

# TODO
# On trying to run make with a directory missing in the list missing from
# project sends make in infinite loop. (SEE file named make-op for output
# of previous run.
# One possible solution found in
# Stackoverflow bookmarked: Why does this simple Makefile go into an infinite
# loop?
DIRS = 	lib \
    	acl altio \
	cap \
	daemons dirs_links \
	filebuff fileio filelock files filesys getopt \
	inotify \
	loginacct \
	memalloc \
	mmap \
	pgsjc pipes pmsg \
	proc proccred procexec procpri procres \
	progconc \
	psem pshm pty \
	shlibs \
	signals sockets \
	svipc svmsg svsem svshm \
	sysinfo \
	syslim \
	threads time timers tty \
	users_groups \
	vdso \
	vmem \
	xattr

# The "namespaces" and "seccomp" directories are deliberately excluded from
# the above list because much of the code in those directories requires a
# relatively recent kernel and userspace to build. Nevertheless, each of
# those directories contains a Makefile.

BUILD_DIRS = ${DIRS}


# Dummy targets for building and clobbering everything in all subdirectories

# (Refer https://www.gnu.org/software/bash/manual/html_node/Exit-Status)
#
# A non-zero exit status indicates failure. This seemingly counter-intuitive 
# scheme is used so there is one well-defined way to indicate success and 
# a variety of ways to indicate various failure modes. When a command 
# terminates on a fatal signal whose number is N, Bash uses the value 
# 128+N as the exit status.
#
# If a command is not found, the child process created to execute it 
# returns a status of 127. If a command is found but is not executable,
# the return status is 126.
all: 	
	@ echo ${BUILD_DIRS}
	@ for dir in ${BUILD_DIRS}; do \
		(cd $${dir}; ${MAKE}) ; \
		if test $$? -ne 0; then \
			break; fi; \
		done

allgen: 
	@ for dir in ${BUILD_DIRS}; do \
		(cd $${dir}; ${MAKE} allgen) ; \
	done

clean: 
	@ for dir in ${BUILD_DIRS}; do \
		(cd $${dir}; ${MAKE} clean) ; \
	done
