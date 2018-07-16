#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/heap
#
# Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/access/streamheap/Makefile,v 1.4 2004/03/24 08:10:49 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/access/streamheap
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = streamheapam.o shio.o 

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

