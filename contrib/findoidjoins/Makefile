# $Header: /project/eecs/db/cvsroot/postgres/contrib/findoidjoins/Makefile,v 1.1.1.2 2003/04/17 22:33:51 sailesh Exp $

subdir = contrib/findoidjoins
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = findoidjoins
OBJS	= findoidjoins.o

PG_CPPFLAGS = -I$(libpq_srcdir)
PG_LIBS = $(libpq)

SCRIPTS = make_oidjoins_check
DOCS = README.findoidjoins

include $(top_srcdir)/contrib/contrib-global.mk
