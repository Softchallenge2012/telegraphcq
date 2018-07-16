#-------------------------------------------------------------------------
#
# Makefile for src/interfaces
#
# Copyright (c) 1994, Regents of the University of California
#
# $Header: /project/eecs/db/cvsroot/postgres/src/interfaces/Makefile,v 1.3 2004/08/10 01:55:49 owenc Exp $
#
#-------------------------------------------------------------------------

subdir = src/interfaces
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

DIRS := libpq ecpg 

ALLDIRS := $(DIRS) libpgtcl python jdbc

ifeq ($(with_tcl), yes)
DIRS += libpgtcl
endif

ifeq ($(with_python), yes)
DIRS += python
endif

ifeq ($(with_java), yes)
DIRS += jdbc utils
endif


all install installdirs uninstall dep depend distprep:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@ || exit; done

clean:
	@for dir in $(DIRS); do $(MAKE) -C $$dir $@; done

distclean maintainer-clean:
	@for dir in $(ALLDIRS); do $(MAKE) -C $$dir $@; done
