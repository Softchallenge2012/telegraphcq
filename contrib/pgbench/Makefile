# $Header: /project/eecs/db/cvsroot/postgres/contrib/pgbench/Makefile,v 1.1.1.2 2003/04/17 22:33:53 sailesh Exp $

subdir = contrib/pgbench
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

PROGRAM = pgbench
OBJS	= pgbench.o

PG_CPPFLAGS = -I$(libpq_srcdir)
PG_LIBS = $(libpq)

DOCS = README.pgbench README.pgbench_jis

include $(top_srcdir)/contrib/contrib-global.mk
