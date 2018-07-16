# $Header: /project/eecs/db/cvsroot/postgres/contrib/oid2name/Makefile,v 1.1.1.2 2003/04/17 22:33:52 sailesh Exp $

subdir = contrib/oid2name
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = oid2name
OBJS	= oid2name.o

PG_CPPFLAGS = -I$(libpq_srcdir)
PG_LIBS = $(libpq)

DOCS = README.oid2name

include $(top_srcdir)/contrib/contrib-global.mk
