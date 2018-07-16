subdir = src/test/examples/web
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global


# shared library parameters
NAME=web
SO_MAJOR_VERSION=1
SO_MINOR_VERSION=1

OBJS=  web.o 

all: all-lib jars/WebQueryServer.jar

# Shared library stuf
include $(top_srcdir)/src/Makefile.shlib

install: all installdirs
ifeq ($(enable_shared), yes)
	        $(INSTALL_SHLIB) $(shlib) $(DESTDIR)$(pkglibdir)/libweb$(DLSUFFIX)
else
	        @echo "*****"; \
         echo "* libgeocode was not installed due to lack of shared library support."; \         echo "*****"
endif
	rm -rf ${prefix}/bin/WebQueryServer
	rm -rf ${prefix}/WebFiles
	mkdir ${prefix}/bin/WebQueryServer
	mkdir  ${prefix}/WebFiles

	cp -r jars/*.jar ${prefix}/bin/WebQueryServer
	cp runwqs.sh runhget.sh ${prefix}/bin
	cp WebFiles/*.hget WebFiles/*.jsc ${prefix}/WebFiles

installdirs:
	        $(mkinstalldirs) $(DESTDIR)$(pkglibdir)

uninstall:
	rm -f $(DESTDIR)$(pkglibdir)/libweb$(DLSUFFIX)

	


clean distclean maintainer-clean: clean-lib
	rm -f $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

ifeq (depend,$(wildcard depend))
include depend
endif
