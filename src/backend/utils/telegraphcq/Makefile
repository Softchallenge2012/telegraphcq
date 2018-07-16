#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for utils/time
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/Makefile,v 1.9 2004/07/15 20:39:57 owenc Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/utils/telegraphcq
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global


OBJS = csv.o tcq_usleep.o quikwrap.o tcq_wtime.o


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
