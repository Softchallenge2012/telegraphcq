# $Header: /project/eecs/db/cvsroot/postgres/contrib/dblink/Makefile,v 1.1.1.2 2003/04/17 22:33:50 sailesh Exp $

subdir = contrib/dblink
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

MODULE_big = dblink
PG_CPPFLAGS = -I$(libpq_srcdir)
OBJS	= dblink.o
SHLIB_LINK = $(libpq)

DATA_built = dblink.sql 
DOCS = README.dblink
REGRESS = dblink

include $(top_srcdir)/contrib/contrib-global.mk
