#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for access/heap
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/access/heap/Makefile,v 1.6 2004/03/24 08:08:05 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/access/streamheap
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS =  heapam.o hio.o tuptoaster.o

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

