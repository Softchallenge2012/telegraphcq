subdir = src/test/examples/tcq-sample
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global


# shared library parameters
NAME= tcqsample
SO_MAJOR_VERSION=1
SO_MINOR_VERSION=1

OBJS=  measurements.o

all: all-lib 

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
