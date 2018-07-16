#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for utils/mmgr
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/mmgr/Makefile,v 1.6 2004/03/24 08:11:19 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/utils/mmgr
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = aset.o mcxt.o portalmem.o shmctx.o shmset.o pallocJudy.o

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
