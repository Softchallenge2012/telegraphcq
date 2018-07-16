subdir = src/test/examples/webservices/webservice_rescan
top_builddir = ../../../../..
include $(top_builddir)/src/Makefile.global


# shared library parameters
NAME=webservices
SO_MAJOR_VERSION=1
SO_MINOR_VERSION=1

OBJS=  webservices_rescan.o 

all: all-lib 

# Shared library stuf
include $(top_srcdir)/src/Makefile.shlib

install: all installdirs
ifeq ($(enable_shared), yes)
	        $(INSTALL_SHLIB) $(shlib) $(DESTDIR)$(pkglibdir)/lib$(NAME)$(DLSUFFIX)
else
	        @echo "*****"; \
         echo "* libgeocode was not installed due to lack of shared library support."; \         echo "*****"
endif

installdirs:
	        $(mkinstalldirs) $(DESTDIR)$(pkglibdir)

uninstall:
	rm -f $(DESTDIR)$(pkglibdir)/lib$(NAME)$(DLSUFFIX)

	


clean distclean maintainer-clean: clean-lib
	rm -f $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
