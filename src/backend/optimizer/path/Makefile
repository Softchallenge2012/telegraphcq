#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for optimizer/path
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/path/Makefile,v 1.3 2004/03/24 08:10:57 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/optimizer/path
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = allpaths.o clausesel.o costsize.o indxpath.o \
       joinpath.o joinrels.o orindxpath.o pathkeys.o tidpath.o

all: SUBSYS.o

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) SUBSYS.o $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

clean: 
	rm -f SUBSYS.o $(OBJS)

ifeq (depend,$(wildcard depend))
include depend
endif
