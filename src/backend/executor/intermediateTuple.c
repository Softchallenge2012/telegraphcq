/*-------------------------------------------------------------------------
 *
 * intermediateTuple.c
 *
 *	  Routines to work with intermediate tuple format.
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/intermediateTuple.c,v 1.33 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <stdio.h>
#include <assert.h>

#include "postgres.h"
#include "miscadmin.h"

#include "access/heapam.h"
#include "access/printtup.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeEddy.h"
#include "rqueue/resultq.h"
#include "utils/bit.h"

extern bool UseStaticPlan;

int
intermediateHeapTuple_determineSize(char *data, char *extra)
{
	return INTERMEDIATEHEAPTUPLESIZE;
}

bool
intermediateHeapTuple_serialize(char *src, char *dest, int destsize, char *extra)
{
	int			size = intermediateHeapTuple_determineSize(src, extra);

	memcpy(dest, src, size);
	return true;
}

bool
intermediateHeapTuple_deserialize(char *src, char **dest, char *extra)
{
	int			size = intermediateHeapTuple_determineSize(src, extra);

	if (*dest == NULL)
		*dest = palloc(size);
	memcpy(*dest, src, size);
	return true;
}

struct queue_serializer intermediateHeapTuple_serializer = {
	intermediateHeapTuple_determineSize,
	intermediateHeapTuple_serialize,
	intermediateHeapTuple_deserialize
};

int
IHT_pointer_determineSize(char *data, char *extra)
{
	int size = sizeof(IntermediateHeapTuple *);

	return size;
}

bool
IHT_pointer_serialize(char *src, char *dest, int destsize, char *extra)
{
	int			size = IHT_pointer_determineSize(src, extra);

	memcpy(dest, src, size);
	return true;
}

bool
IHT_pointer_deserialize(char *src, char **dest, char *extra)
{
	int			size = IHT_pointer_determineSize(src, extra);

	memcpy(*dest, src, size);
	return true;
}


struct queue_serializer IHT_pointer_serializer = {
	IHT_pointer_determineSize,
	IHT_pointer_serialize,
	IHT_pointer_deserialize
};

void
make_iht_queue(Queue ** q, int size)
{
	struct queue_lock *ql = NULL;

	make_lock_nolock(&ql);
	make_queue(q, ql, size);
}



void
debugitup(IntermediateHeapTuple tuple)
{
	int			i;

	printf("============================================================\n");
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (tuple->t_base_tuples[i] != NULL)
		{
			printf("        Tuple(%d) :\n",i);
#ifndef DISABLE_CQ_SUPPORT
			debugtup(tuple->t_base_tuples[i], tuple->t_base_tuple_descs[i], NULL);
#endif
		}
	}
}

void
debugitup_base_tuples_descs(IntermediateHeapTuple tuple, TupleDesc *base_tuple_descs)
{
	int			i;

	printf("============================================================\n");
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (tuple->t_base_tuples[i] == NULL)
			printf("        Tuple(%d) is NULL\n", i);
		else
			debugtup(tuple->t_base_tuples[i], base_tuple_descs[i], NULL);
	}
}

void
debugitup_debug(IntermediateHeapTuple tuple)
{
	int			i;

	elog(DEBUG1, "============================================================");
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (tuple->t_base_tuples[i] != NULL)
		{
			elog(DEBUG1, "        Tuple(%d) :\n",i);
#ifndef DISABLE_CQ_SUPPORT
			debugtup_debug(tuple->t_base_tuples[i], tuple->t_base_tuple_descs[i], NULL);
#endif
		}
	}
	elog(DEBUG1,"Ready: ");
	BitArrayPrintDebug(tuple->t_ready,8);
	elog(DEBUG1,"Done: ");
	BitArrayPrintDebug(tuple->t_done,8);
	elog(DEBUG1,"QC: ");
	BitArrayPrintDebug(tuple->t_queriesCompleted,8);
	
}

void
debugitup_lev(IntermediateHeapTuple tuple, int lev)
{
	int			i;

	elog(lev, "============================================================");
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (tuple->t_base_tuples[i] == NULL)
			elog(lev, "        Tuple(%d) is NULL", i);
		else
		{
#ifndef DISABLE_CQ_SUPPORT
			debugtup_lev(tuple->t_base_tuples[i], tuple->t_base_tuple_descs[i], NULL,lev);
#endif
		}
	}
	elog(lev,"Ready: ");
	BitArrayPrintLev(tuple->t_ready,8,lev);
	elog(lev,"Done: ");
	BitArrayPrintLev(tuple->t_done,8,lev);
	elog(lev,"QC: ");
	BitArrayPrintLev(tuple->t_queriesCompleted,8,lev);
	
}


IntermediateHeapTuple
CreateIntermediateHeapTuple()
{
	int			i;
	IntermediateHeapTuple retval;

	retval = (IntermediateHeapTuple) palloc(sizeof(IntermediateHeapTupleData));

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		retval->t_base_tuples[i] = 0;
#ifndef DISABLE_CQ_SUPPORT
		retval->t_base_tuple_descs[i] = 0;
#endif
	}
	BitArrayRealClearAll(retval->t_sources, TCQMAXSOURCES);
#ifndef DISABLE_CQ_SUPPORT
	BitArrayRealClearAll(retval->t_ready, TCQMAXOPERATORS);
	BitArrayRealClearAll(retval->t_done, TCQMAXOPERATORS);
	BitArraySetAll(retval->t_queriesCompleted, TCQMAXQUERIES);
#endif

	return retval;
}


/*
 * ClearBaseTuples()
 *
 * To unset all the base tuple/descriptor pointers for an already-allocated IHT
 * Also clears up the bit arrays 
 *
 * - TCQ SR
 */
void
ClearBaseTuples(IntermediateHeapTuple iht) 
{
  
  int i;
  
  for (i = 0; i < __MAX_RELATIONS__; i++)
    {
      iht->t_base_tuples[i] = NULL;
#ifndef DISABLE_CQ_SUPPORT
      iht->t_base_tuple_descs[i] = NULL;
#endif
    }
  
  BitArrayRealClearAll(iht->t_sources, TCQMAXSOURCES);
#ifndef DISABLE_CQ_SUPPORT
  BitArrayRealClearAll(iht->t_ready, TCQMAXOPERATORS);
  BitArrayRealClearAll(iht->t_done, TCQMAXOPERATORS);
  BitArraySetAll(iht->t_queriesCompleted, TCQMAXQUERIES);
#endif
  
}


void
AddBaseTuples(IntermediateHeapTuple iht_to, IntermediateHeapTuple iht_from)
{
	int			i;

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iht_from->t_base_tuples[i] != NULL)
		{
			/*
			 * iht_to base tuple at this position better be NULL
			 */
			if (iht_to->t_base_tuples[i] != NULL) 
			{
				elog(DEBUG1,"Houston, we have a problem ... i = %d", i);
				elog(DEBUG1,"IHT To: sources %x",iht_to->t_sources[0]);
				debugitup_debug(iht_to);
				elog(DEBUG1,"IHT From: sources %x",iht_from->t_sources[0]);
				debugitup_debug(iht_from);
				Assert(iht_to->t_base_tuples[i] == NULL);
			}

			iht_to->t_base_tuples[i] = iht_from->t_base_tuples[i];

#ifndef DISABLE_CQ_SUPPORT
			iht_to->t_base_tuple_descs[i] = iht_from->t_base_tuple_descs[i];
#endif
		}
	}

	BitArrayOrSelf(iht_to->t_sources, iht_from->t_sources, TCQMAXSOURCES);

#ifndef DISABLE_CQ_SUPPORT
	BitArrayOrSelf(iht_to->t_done, iht_from->t_done, TCQMAXOPERATORS);

	if (UseOperatorPrecedence || (UseStaticPlan && UseTupleLineage)) 
	{
		BitArrayOrSelf(iht_to->t_queriesCompleted,
					   iht_from->t_queriesCompleted,
					   TCQMAXQUERIES);
	}
#endif

	/* Timestamp for duplicate elimination purposes - Amol */
	if (iht_to->t_timestamp < iht_from->t_timestamp)
		iht_to->t_timestamp = iht_from->t_timestamp;
}



/* AddBaseTuplesAllowingOverlap()
*
* A less strict form of the above function, that allows 
* the two IHT's to both have non-NULL pointers for a particular position,
* as long as they point to the same base heaptuple
* 
*  - TCQ SR
*
*/
void
AddBaseTuplesAllowingOverlap(IntermediateHeapTuple iht_to, IntermediateHeapTuple iht_from)
{
	int			i;

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iht_from->t_base_tuples[i] != NULL)
		{

			if (iht_to->t_base_tuples[i] != NULL) 
			{
			  if(iht_from->t_base_tuples[i] == iht_to->t_base_tuples[i]) {
			    /* Same pointer for this field, so don't care*/

			  }
			  else { /* No way to resolve conflict, so complain */

				elog(DEBUG1,"Houston, we have a problem ... i = %d", i);
				elog(DEBUG1,"IHT To: sources %x",iht_to->t_sources[0]);
				debugitup_debug(iht_to);
				elog(DEBUG1,"IHT From: sources %x",iht_from->t_sources[0]);
				debugitup_debug(iht_from);
				Assert(iht_to->t_base_tuples[i] == NULL);

			  }

			}

			else {
			iht_to->t_base_tuples[i] = iht_from->t_base_tuples[i];

#ifndef DISABLE_CQ_SUPPORT
			iht_to->t_base_tuple_descs[i] = iht_from->t_base_tuple_descs[i];
#endif
			}
		}
	}

	BitArrayOrSelf(iht_to->t_sources, iht_from->t_sources, TCQMAXSOURCES);

#ifndef DISABLE_CQ_SUPPORT
	BitArrayOrSelf(iht_to->t_done, iht_from->t_done, TCQMAXOPERATORS);

	if (UseOperatorPrecedence || (UseStaticPlan && UseTupleLineage)) 
	{
		BitArrayOrSelf(iht_to->t_queriesCompleted,
					   iht_from->t_queriesCompleted,
					   TCQMAXQUERIES);
	}
#endif

	/* Timestamp for duplicate elimination purposes - Amol */
	if (iht_to->t_timestamp < iht_from->t_timestamp)
		iht_to->t_timestamp = iht_from->t_timestamp;
}



void
AddBaseTuple(IntermediateHeapTuple iht, HeapTuple ht, TupleDesc td, int sourceID, unsigned int timestamp)
{
	Assert(iht->t_base_tuples[sourceID] == NULL);

	iht->t_base_tuples[sourceID] = ht;

#ifndef DISABLE_CQ_SUPPORT
	iht->t_base_tuple_descs[sourceID] = td;
#endif


	BitArraySetBit(iht->t_sources, sourceID);
	/* Timestamp for duplicate elimination purposes - Amol */
	if (iht->t_timestamp < timestamp)
		iht->t_timestamp = timestamp;
}


/*
 * Copy all the base tuples for this intermediate tuple into the provided memorycontext.
 */
void
copy_base_tuples_into_context(IntermediateHeapTuple iht, MemoryContext mc)
{
	int			i;

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		HeapTuple	heapTuple = iht->t_base_tuples[i];
		HeapTuple	newHeapTuple;

		if (heapTuple != NULL)
		{
			int			tupleSize;

			tupleSize = MAXALIGN(sizeof(HeapTupleData)) + heapTuple->t_len;
			newHeapTuple = (HeapTuple) MemoryContextAlloc(mc, tupleSize);

			if (newHeapTuple == NULL)
				elog(ERROR, "Insufficient memory for hash table.");

			memcpy((char *) newHeapTuple, (char *) heapTuple, sizeof(HeapTupleData));

			newHeapTuple->t_datamcxt = mc;

			newHeapTuple->t_data = (HeapTupleHeader) (((char *) newHeapTuple) + MAXALIGN(sizeof(HeapTupleData)));

			memcpy((char *) newHeapTuple->t_data,
				   (char *) heapTuple->t_data,
				   heapTuple->t_len);

			iht->t_base_tuples[i] = newHeapTuple;
		}
	}
}

void
copyIntermediateHeapTuple(IntermediateHeapTuple target,
						  IntermediateHeapTuple source)
{
	int			i;

	for (i = 0; i < TCQMAXSOURCES; i++)
	{
		target->t_base_tuples[i] = source->t_base_tuples[i];

#ifndef DISABLE_CQ_SUPPORT
		target->t_base_tuple_descs[i] = source->t_base_tuple_descs[i];
#endif
	}

	BitArrayClone(target->t_sources, source->t_sources, TCQMAXSOURCES);

#ifndef DISABLE_CQ_SUPPORT
	BitArrayClone(target->t_ready, source->t_ready, TCQMAXOPERATORS);
	BitArrayClone(target->t_done, source->t_done, TCQMAXOPERATORS);
	BitArrayClone(target->t_queriesCompleted,
				  source->t_queriesCompleted,
				  TCQMAXQUERIES);
#endif

	target->t_timestamp = source->t_timestamp;
	target->isAggTuple = source->isAggTuple;
}

void
initFromBaseTuple(IntermediateHeapTuple iht, HeapTuple ht, TupleDesc td,
				  int sourceID)
{
	int			i;

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		iht->t_base_tuples[i] = 0;

#ifndef DISABLE_CQ_SUPPORT
		iht->t_base_tuple_descs[i] = 0;
#endif
	}
	BitArrayRealClearAll(iht->t_sources, TCQMAXSOURCES);

#ifndef DISABLE_CQ_SUPPORT
	BitArrayRealClearAll(iht->t_ready, TCQMAXOPERATORS);		/* Be safe first ! */
	BitArrayRealClearAll(iht->t_done, TCQMAXOPERATORS);
	BitArraySetAll(iht->t_queriesCompleted, TCQMAXQUERIES);
#endif

	iht->t_base_tuples[sourceID] = ht;
#ifndef DISABLE_CQ_SUPPORT
	iht->t_base_tuple_descs[sourceID] = td;
#endif
	BitArraySetBit(iht->t_sources, sourceID);
	iht->isAggTuple=FALSE;
}

void
inheritCQBitmaps(IntermediateHeapTuple tgt, IntermediateHeapTuple src)
{
#ifndef DISABLE_CQ_SUPPORT
	BitArrayClone(tgt->t_ready, src->t_ready, TCQMAXOPERATORS);
	BitArrayClone(tgt->t_done, src->t_done, TCQMAXOPERATORS);
	BitArrayClone(tgt->t_queriesCompleted, src->t_queriesCompleted, TCQMAXQUERIES);
#endif
}

void
freeBaseTuple(IntermediateHeapTuple iht, int sourceID) 
{
	HeapTuple ht = iht->t_base_tuples[sourceID];
	
	Assert(NULL != iht);
	Assert(NULL != ht);
	
	heap_freetuple(ht);
}

