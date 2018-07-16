/*-------------------------------------------------------------------------
 *
 * ingresq.h
 * Author: Mehul Shah
 *
 *	Copyright (c) 2002, Regents of the University of California
 *
 *	These are functions for allocating and free result queues for
 *	wrappers (ingres) operators.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/include/rqueue/ingresq.h,v 1.9 2004/03/24 08:11:29 chungwu Exp $
 *
 *
 *-------------------------------------------------------------------------
 */

#ifndef INGRESQ_H
#define INGRESQ_H

#include "access/tupdesc.h"
#include "rqueue/resultq.h"

typedef int IngresQIdx;

/* See comments in the definitions */
Queue	   *getIngresQ(IngresQIdx idx);
Queue	   *getIngresQ_byName(char *name);
IngresQIdx	allocIngresQ(int size, char *name);
void		freeIngresQ(IngresQIdx idx);
TupleDesc	getTupleDesc(IngresQIdx idx);
void		setTupleDesc(IngresQIdx idx, TupleDesc tdesc);
int			getIngresQueueRefCount(int qidx);
void		IngresQInsertTuple(Queue *, TupleDesc, Datum *, char *);

#endif   /* INGRESQ_H */
