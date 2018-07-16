#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for optimizer/plan
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/Makefile,v 1.10 2004/01/22 21:17:31 garrett Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/optimizer/plan
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = createplan.o initsplan.o planmain.o planner.o setrefs.o subselect.o \
	bindings.o convertoeddy.o eddy.o converttostream.o converttohashgroup.o \
	converttofsched.o converttofsched_dup.o createeddy.o converttosymhj.o

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
