/*-------------------------------------------------------------------------
 *
 * shio.h
 *	  TelegraphCQ streamheap access method input/output definitions.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: shio.h,v 1.5 2003/07/17 05:18:49 sailesh Exp $ *
 *
 *-------------------------------------------------------------------------
 */
#ifndef SHIO_H
#define SHIO_H

#include "access/htup.h"

extern void StreamPutHeapTuple(Relation relation, Buffer buffer,
				   HeapTuple tuple);

/*extern Buffer StreamGetBufferForTuple(Relation relation, Size len,
						Buffer otherBuffer);*/
extern Buffer StreamGetBufferForTuple(Relation relation, Size len);


#endif   /* SHIO_H */
