#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for the port-specific subsystem of the backend
#
# These files are used in other directories for portability on systems
# with broken/missing library files.

# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/port/Makefile,v 1.2 2004/03/24 08:11:36 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/port
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

ifdef LIBOBJS
all: libpgport.a
endif

libpgport.a: $(LIBOBJS)
	$(AR) crs $@ $^

clean distclean maintainer-clean:
	rm -f libpgport.a $(LIBOBJS)
