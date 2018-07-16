/*-------------------------------------------------------------------------
 *
 * planq.h
 *
 * Author: Owen Cooper
 *
 * TBD: Needs more documentation
 *
 * Copyright (C) 2003, Regents of the University of California
 *
 * $Id: planq.h,v 1.9 2004/08/07 01:01:38 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef PLANQ_H
#define PLANQ_H

#include "c.h"

#include "lib/stringinfo.h"
#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"
#include "nodes/pg_list.h"
#include "utils/palloc.h"

typedef struct planqentry
{
	MemoryContext mctx;
	Plan	   *plan;
	Query	   *querytree;		/* NULL for CQ cases */
	List	   *rangetable;		/* NULL for non CQ cases */
  List       *pqe_list;                 /* list of queries for WITH or RECURSIVE_NEXT */
	bool isRecursive;
	bool isLast;
	uint16		queueNo;		/* queue no */
	StringInfo	querystr;		/* query string */
}	planqentry;

int			planqentry_determineSize(char *data, char *extra);
bool		planqentry_serialize(char *src, char *dest, int size, char *extra);
bool		planqentry_deserialize(char *src, char **dest, char *extra);

extern struct queue_serializer planqentry_serializer;


#endif   /* PLANQ_H */
