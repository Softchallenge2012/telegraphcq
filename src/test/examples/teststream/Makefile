subdir = src/test/examples/teststreams
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global


# shared library parameters
NAME= wrapper
SO_MAJOR_VERSION=1
SO_MINOR_VERSION=1

OBJS=  teststream.o teststream1.o teststream2.o teststream3.o

all: all-lib teststreamsource 

teststreamsource: teststreamsource.c
	cc -g $(CCFLAGS) -o teststreamsource teststreamsource.c

# Shared library stuf
include $(top_srcdir)/src/Makefile.shlib

install: all installdirs install-lib

.PHONY: installdirs

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(libdir) $(DESTDIR)$(includedir)

clean distclean maintainer-clean: clean-lib
	rm -f $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
