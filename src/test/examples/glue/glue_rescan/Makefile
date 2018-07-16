subdir = src/test/glue/glue_rescan
top_builddir = ../../../../../
include $(top_builddir)/src/Makefile.global


# shared library parameters
NAME=glue
SO_MAJOR_VERSION=1
SO_MINOR_VERSION=1

OBJS=  glue_rescan.o

all: all-lib 

# Shared library stuf
include $(top_srcdir)/src/Makefile.shlib

install: installdirs all
ifeq ($(enable_shared), yes)
	$(INSTALL_SHLIB) $(shlib) $(DESTDIR)$(pkglibdir)/libglue$(DLSUFFIX)
else
	@echo "*****"; \
	 echo "* libglue was not installed due to lack of shared library support."; \
	 echo "*****"
endif

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(pkglibdir)

uninstall:
	rm -f $(DESTDIR)$(pkglibdir)/libglue$(DLSUFFIX)

clean distclean maintainer-clean: clean-lib
	rm -f $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
