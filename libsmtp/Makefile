# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

top_srcdir = .
srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
infodir = $(prefix)/info
libdir = $(prefix)/lib/gnudl
mandir = $(prefix)/man/man1
# subdirs = smtp mime examples
subdirs = smtp mime examples

CC = gcc
CPPFLAGS = 
CFLAGS = $(CPPFLAGS) -g -O2
LDFLAGS = 
LIBS = -lglib 
INSTALL = /usr/bin/install -c


all:
	@for dir in ${subdirs}; do \
	  (cd $$dir && $(MAKE) all) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

install:
	@for dir in ${subdirs}; do \
	  (cd $$dir && $(MAKE) install) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
clean:
	/bin/rm -f *~
	@for dir in ${subdirs}; do \
	  (cd $$dir && $(MAKE) clean) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
distclean: clean
	/bin/rm -f Makefile config.h config.status config.cache config.log
	@for dir in ${subdirs}; do \
	  (cd $$dir && $(MAKE) distclean) \
	  || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

# automatic re-running of configure if the ocnfigure.in file has changed
#${srcdir}/configure: configure.in aclocal.m4
#	cd ${srcdir} && autoconf # autoheader might not change config.h.in, so touch a stamp file

#${srcdir}/config.h.in: stamp-h.in
#${srcdir}/stamp-h.in: configure.in aclocal.m4
#	cd ${srcdir} && autoheader
#	echo timestamp > ${srcdir}/stamp-h.in

config.h: stamp-h
stamp-h: config.h.in config.status
	./config.status
Makefile: Makefile.in config.status
	./config.status
config.status: configure
	./config.status --recheck

