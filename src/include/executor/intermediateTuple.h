/*-------------------------------------------------------------------------
 *
 * intermediateTuple.h
 *
 * Contains the definiton of intermediate tuple format as well as the
 * functions to operate on them.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: intermediateTuple.h,v 1.31 2005/06/13 02:59:28 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef INTERMEDIATETUPLE_H
#define INTERMEDIATETUPLE_H

#include "postgres.h"
#include "misceddy.h"

#include "access/htup.h"
#include "access/tupdesc.h"
#include "rqueue/resultq.h"
#include "telegraphcq/telegraphcq.h"
#include "utils/palloc.h"
#include "utils/bit.h"
/*
 * Intermediate tuples will currently be represented as an array of pointers to HeapTuples.
 * They will probably also be allocated in the same context as HeapTuples.
 *
 * For simplicity, we will set it to be a static array of __MAX_RELATIONS__ pointers; this
 * can be later changed to be dynamically allocated itself based on the query.
 *
 * @author Amol
 *
 */
#define __MAX_RELATIONS__  TCQMAXSOURCES

void		make_iht_queue(Queue ** q, int size);

bool		intermediateHeapTuple_serialize(char *src, char *dest, int destsize, char *extra);
int			intermediateHeapTuple_determineSize(char *data, char *extra);
bool		intermediateHeapTuple_deserialize(char *src, char **dest, char *extra);

extern struct queue_serializer intermediateHeapTuple_serializer;

extern int  IHT_pointer_determineSize(char *data, char *extra);
extern bool IHT_pointer_serialize(char *src, char *dest, int destsize, char *extra);
extern bool IHT_pointer_deserialize(char *src, char **dest, char *extra);

extern struct queue_serializer IHT_pointer_serializer;

typedef struct IntermediateHeapTupleData
{
	HeapTuple	t_base_tuples[__MAX_RELATIONS__];

#ifndef DISABLE_CQ_SUPPORT
	TupleDesc	t_base_tuple_descs[__MAX_RELATIONS__];
#endif

	MemoryContext t_datamcxt;	/* memory context of allocation */

#ifndef DISABLE_CQ_SUPPORT
	bits8		t_ready[BASIZE(TCQMAXOPERATORS)];
	bits8		t_done[BASIZE(TCQMAXOPERATORS)];
	bits8		t_queriesCompleted[BASIZE(TCQMAXQUERIES)];
#endif
	bits8		t_sources[BASIZE(TCQMAXSOURCES)];
	bits8		t_isrc;

	double		t_timestamp;	/* Timestamp for duplicate elimination --    * Amol */
  Timestamp event_timestamp; /* Timestamp of the complex event this IHTD represents  */
 

#ifdef NOT_USED
    /* track path of individulal tuples -- OSC debugging */
	int iSrc;
	int allClearedOutputCount;
	int modulesVisited[TCQMAXOPERATORS];
	int modulesVisitedNum;
#endif 
  bool isAggTuple;      /* proxima estacion: output */
}	IntermediateHeapTupleData;

typedef IntermediateHeapTupleData *IntermediateHeapTuple;

#define INTERMEDIATEHEAPTUPLESIZE	MAXALIGN(sizeof(IntermediateHeapTupleData))

/*
 * print
 */
extern void debugitup(IntermediateHeapTuple tuple);
extern void debugitup_base_tuples_descs(IntermediateHeapTuple tuple, TupleDesc *base_tuple_descs);
extern void debugitup_debug(IntermediateHeapTuple tuple);
extern void debugitup_lev(IntermediateHeapTuple tuple, int lev);

/*
 * miscellaneous operations
 */
extern IntermediateHeapTuple CreateIntermediateHeapTuple(void);

extern void ClearBaseTuples(IntermediateHeapTuple iht);

extern void AddBaseTuples(IntermediateHeapTuple iht_to, IntermediateHeapTuple iht_from);

extern void AddBaseTuplesAllowingOverlap(IntermediateHeapTuple iht_to, IntermediateHeapTuple iht_from);

extern void AddBaseTuple(IntermediateHeapTuple iht, HeapTuple ht, TupleDesc td, int sourceID, unsigned int timestamp);

extern void copy_base_tuples_into_context(IntermediateHeapTuple iht, MemoryContext mc);

extern void copyIntermediateHeapTuple(IntermediateHeapTuple target, IntermediateHeapTuple source);

extern void initFromBaseTuple(IntermediateHeapTuple iht, HeapTuple ht, TupleDesc td,
				  int sourceID);

extern void inheritCQBitmaps(IntermediateHeapTuple tgt, IntermediateHeapTuple src);

extern void freeBaseTuple(IntermediateHeapTuple iht, int sourceID);

#endif   /* INTERMEDIATETUPLE_H */
