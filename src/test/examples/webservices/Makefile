#-------------------------------------------------------------------------
#
# Makefile for JDBC driver
#
# Copyright (c) 2001, PostgreSQL Global Development Group
#
# $Header: /project/eecs/db/cvsroot/postgres/src/test/examples/webservices/Makefile,v 1.1 2004/08/10 01:55:50 owenc Exp $
#
#-------------------------------------------------------------------------

subdir = src/test/examples/webservices
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

minorversion := $(shell echo $(VERSION) | sed 's/^[0-9][0-9]*\.\([0-9][0-9]*\).*$$/\1/')
majorversion := $(shell echo $(VERSION) | sed 's/^\([0-9][0-9]*\)\..*$$/\1/')

properties := -Dmajor=$(majorversion) -Dminor=$(minorversion) \
		-Dfullversion=$(VERSION) \
		-Ddef_pgport=$(DEF_PGPORT) \
		-Denable_debug=$(enable_debug)

all:
	make -C webservice_rescan  
	$(ANT) -buildfile $(srcdir)/build.xml all \
	  -Dinstall.directory=$(javadir) $(properties)

install: installdirs
	make -C webservice_rescan install 
	$(ANT) -buildfile $(srcdir)/build.xml install \
	  -Dinstall.directory=$(javadir) $(properties)

installdirs:
	$(mkinstalldirs) $(javadir) 

uninstall:
	make -C webservice_rescan uninstall 
	$(ANT) -buildfile $(srcdir)/build.xml uninstall \
	  -Dinstall.directory=$(javadir)

clean distclean maintainer-clean:
	make -C webservice_rescan clean
	$(ANT) -buildfile $(srcdir)/build.xml clean

