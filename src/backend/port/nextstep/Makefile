#-------------------------------------------------------------------------
#
# Makefile for NeXTStep 3.3 specific stuff
#
# $Header: /project/eecs/db/cvsroot/postgres/src/backend/port/nextstep/Makefile,v 1.2 2004/03/24 08:11:04 chungwu Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/port/nextstep
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

OBJS = dynloader.o port.o

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
