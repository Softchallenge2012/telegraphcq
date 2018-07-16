#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for telegraphcq
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/Makefile,v 1.15 2005/12/14 20:49:24 phred Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/telegraphcq
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = telegraphcq.o wrapch.o dyncatalog.o fluxroutines.o triageq.o \
       histogram.o wavelet.o shedding.o strmsource.o \
       window.o hifiglue.o punct.o

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
