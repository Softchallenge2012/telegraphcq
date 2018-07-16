#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for rqueue
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/rqueue/Makefile,v 1.8 2004/03/24 08:11:06 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/rqueue
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = controlq.o resultq.o rqdest.o planq.o ingresq.o

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
