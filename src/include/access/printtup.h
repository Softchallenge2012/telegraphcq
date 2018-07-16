/*-------------------------------------------------------------------------
 *
 * printtup.h
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: printtup.h,v 1.7 2004/07/15 20:39:57 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PRINTTUP_H
#define PRINTTUP_H

#include "tcop/dest.h"

extern DestReceiver *printtup_create_DR(bool isBinary);

extern void debugSetup(DestReceiver *self, int operation,
		   const char *portalName, TupleDesc typeinfo);
extern void debugtup(HeapTuple tuple, TupleDesc typeinfo,
		 DestReceiver *self);

extern void f_debugtup(HeapTuple tuple, TupleDesc typeinfo,
		   DestReceiver *self, FILE *fp);

/* XXX these are really in executor/spi.c */
extern void spi_dest_setup(DestReceiver *self, int operation,
			   const char *portalName, TupleDesc typeinfo);
extern void spi_printtup(HeapTuple tuple, TupleDesc tupdesc,
			 DestReceiver *self);

extern void
			debugtup_debug(HeapTuple tuple, TupleDesc typeinfo, DestReceiver *self);
void			debugtup_lev(HeapTuple tuple, TupleDesc typeinfo, DestReceiver *self, int lev);

#endif   /* PRINTTUP_H */
