/*-------------------------------------------------------------------------
 *
 * nodeHashGroup.c
 *	  Routines to do windowed hash-based group-by.
 *	  Currently just collects tuples and performs count and returns
 *	  timing information.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Window
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * DESCRIPTION
 *	  This is supposed to do a hash-based group-by.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeHashGroup.c,v 1.16 2004/03/05 21:10:15 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeHashGroup.h"
#include "executor/nodeHashAgg.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include <sys/time.h>
#include <sys/types.h>
#include <fmgr.h>
#include "utils/datum.h"
#include "utils/window_agg.h"

#include "nodes/print.h"
#include "rqueue/rqdest.h"

#include "executor/execState.h"

/* For now create some windowed functions inline. Later we will figure
   out how to integrate them into the catalog. */

static AggHashEntry lookup_hash_entry(HashAgg * node, TupleTableSlot *slot);

static TupleTableSlot *exec_hashed_window_agg(HashGroup * node);

static void initialize_aggregates(HashAggState * aggstate,
					  AggStatePerAgg peragg,
					  AggStatePerGroup pergroup);

static void initialize_one_aggregate(HashAggState * aggstate,
						 AggStatePerAgg peraggstate,
						 AggStatePerGroup pergroupstate);

static TupleTableSlot *advance_aggregates(HashAggState * aggstate,
				   AggStatePerGroup pergroup,
				   AggHashEntry entry);

static void advance_transition_function(HashAggState * aggstate,
							AggStatePerAgg peraggstate,
							AggStatePerGroup pergroupstate,
							Datum newVal, bool isNull);

/*
 * Initialize one aggregate for a new set of input values.
 *
 * When called, CurrentMemoryContext should be the per-query context.
 */
static void
initialize_one_aggregate(HashAggState * aggstate,
						 AggStatePerAgg peraggstate,
						 AggStatePerGroup pergroupstate)
{
	Aggref	   *aggref = peraggstate->aggref;
	FunctionCallInfoData fcinfo;

#ifdef AGG_DEBUG_MSGS
	elog(DEBUG1, "initialize_one_aggregate");
#endif

	/*
	 * Start a fresh sort operation for each DISTINCT aggregate.
	 */
	if (aggref->aggdistinct)
	{
		/*
		 * In case of rescan, maybe there could be an uncompleted sort
		 * operation?  Clean it up if so.
		 */
		if (peraggstate->sortstate)
			tuplesort_end(peraggstate->sortstate);

		peraggstate->sortstate =
			tuplesort_begin_datum(peraggstate->inputType,
								  peraggstate->sortOperator,
								  false);
	}

	/*
	 * (Re)set transValue to the initial value.
	 *
	 * Note that when the initial value is pass-by-ref, we must copy it (into
	 * the aggcontext) since we will pfree the transValue later.
	 */
	if (peraggstate->initValueIsNull)
		pergroupstate->transValue = peraggstate->initValue;
	else
	{
		MemoryContext oldContext;

		oldContext = MemoryContextSwitchTo(aggstate->aggcontext);
		pergroupstate->transValue = datumCopy(peraggstate->initValue,
											  peraggstate->transtypeByVal,
											  peraggstate->transtypeLen);
		MemoryContextSwitchTo(oldContext);
	}
	pergroupstate->transValueIsNull = peraggstate->initValueIsNull;


	/*
	 * Call an initialization function, actually the transition function
	 * with the first parameter's null set to TRUE;
	 */

	MemSet(&fcinfo, 0, sizeof(fcinfo));
	fcinfo.flinfo = &peraggstate->transfn;
	fcinfo.nargs = 2;
	fcinfo.arg[0] = PointerGetDatum(pergroupstate);
	fcinfo.argnull[0] = TRUE;	/* This indicates that the aggregate is
								 * being called initially, and not on a
								 * transition so we can allocate items. */

	fcinfo.arg[1] = PointerGetDatum(peraggstate);
	fcinfo.argnull[1] = FALSE;

	/* Set the transition values according to what the function returns */
	pergroupstate->transValue = FunctionCallInvoke(&fcinfo);
	pergroupstate->transValueIsNull = fcinfo.isnull;

	/*
	 * If the initial value for the transition state doesn't exist in the
	 * pg_aggregate table then we will let the first non-NULL value
	 * returned from the outer procNode become the initial value. (This is
	 * useful for aggregates like max() and min().)  The noTransValue flag
	 * signals that we still need to do this.
	 */
	pergroupstate->noTransValue = peraggstate->initValueIsNull;
}

static void
initialize_aggregates(HashAggState * aggstate,
					  AggStatePerAgg peragg,
					  AggStatePerGroup pergroup)
{
	int			aggno;

#ifdef AGG_DEBUG_MSGS
	elog(DEBUG1, "initialize_aggregates");
#endif

	for (aggno = 0; aggno < aggstate->numaggs; aggno++)
	{
		AggStatePerAgg peraggstate = &peragg[aggno];
		AggStatePerGroup pergroupstate = &pergroup[aggno];

		initialize_one_aggregate(aggstate, peraggstate, pergroupstate);
	}
}

static AggHashEntry
lookup_hash_entry(HashAgg * node, TupleTableSlot *slot)
{
	HashAggState *aggstate = node->aggstate;
	AggHashEntry entry;
	bool		isnew;

	entry = (AggHashEntry) LookupTupleHashEntry(aggstate->hashtable,
												slot,
												&isnew);
	if (isnew)
		initialize_aggregates(aggstate, aggstate->peragg, entry->pergroup);

	return entry;
}

/*
 * Given a new input value, advance the transition function of an aggregate.
 *
 * When called, CurrentMemoryContext should be the context we want the
 * transition function result to be delivered into on this cycle.
 */
static void
advance_transition_function(HashAggState * aggstate,
							AggStatePerAgg peraggstate,
							AggStatePerGroup pergroupstate,
							Datum newVal, bool isNull)
{
	FunctionCallInfoData fcinfo;
/* 	MemoryContext oldContext; */

#ifdef AGG_DEBUG_MSGS
	elog(DEBUG1, "advance_transition_function");
#endif

/* 	oldContext = MemoryContextSwitchTo(aggstate->tmpcontext->ecxt_per_tuple_memory); */

	/* OK to call the transition function */
	MemSet(&fcinfo, 0, sizeof(fcinfo));

	fcinfo.flinfo = &peraggstate->transfn;
	fcinfo.nargs = 4;
	fcinfo.arg[0] = PointerGetDatum(pergroupstate);
	fcinfo.argnull[0] = FALSE;
	fcinfo.arg[1] = PointerGetDatum(peraggstate);
	fcinfo.argnull[1] = FALSE;
	fcinfo.arg[2] = newVal;
	fcinfo.argnull[2] = FALSE;

	/*
	 * The tuple is passed in so the aggregate can get the timestamp
	 * value, if needed
	 */

	fcinfo.arg[3] = PointerGetDatum(aggstate->tmpcontext->ecxt_scantuple);
	fcinfo.argnull[3] = FALSE;

	newVal = FunctionCallInvoke(&fcinfo);

	pergroupstate->transValue = newVal;
	pergroupstate->transValueIsNull = fcinfo.isnull;

/* 	MemoryContextSwitchTo(oldContext); */
}

/*
 * Advance all the aggregates for one input tuple.	The input tuple
 * has been stored in tmpcontext->ecxt_scantuple, so that it is accessible
 * to ExecEvalExpr.  pergroup is the array of per-group structs to use
 * (this might be in a hashtable entry).
 *
 * When called, CurrentMemoryContext should be the per-query context.
 */
static TupleTableSlot *
advance_aggregates(HashAggState * aggstate,
				   AggStatePerGroup pergroup,
				   AggHashEntry entry)
{
	ExprContext *econtext = aggstate->tmpcontext;
	int			aggno;
	bool		new_result;
	ProjectionInfo *projInfo;

#ifdef AGG_DEBUG_MSGS
	elog(DEBUG1, "advance_aggregates");
#endif

	new_result = FALSE;

	for (aggno = 0; aggno < aggstate->numaggs; aggno++)
	{
		AggStatePerAgg peraggstate = &aggstate->peragg[aggno];
		AggStatePerGroup pergroupstate = &pergroup[aggno];

		Aggref	   *aggref = peraggstate->aggref;
		Datum		newVal;
		bool		isNull;

#ifdef AGG_DEBUG_MSGS
		elog(DEBUG1, "ExecEvalExprSwitchContext: target = %p", aggref->target);
#endif
		newVal = ExecEvalExprSwitchContext(aggref->target, econtext,
										   &isNull, NULL);

		if (aggref->aggdistinct)
		{
			elog(FATAL, "Does not support distinct aggregates");
			return NULL;
		}
		else
		{
			advance_transition_function(aggstate, peraggstate,
										pergroupstate,
										newVal, isNull);

			/*
			 * We have a new aggregate value, form a new result tuple and
			 * return it.
			 */
			if (!pergroupstate->transValueIsNull)
				new_result = TRUE;
		}
	}

	/* Generate a result tuple if the aggregate function returns a value */
	if (new_result)
	{
		econtext = aggstate->csstate.cstate.cs_ExprContext;
		projInfo = aggstate->csstate.cstate.cs_ProjInfo;

		/*
		 * Clear the per-output-tuple context for each group
		 */
		ResetExprContext(econtext);

		for (aggno = 0; aggno < aggstate->numaggs; aggno++)
		{
			AggStatePerGroup pergroupstate = &pergroup[aggno];

			econtext->ecxt_aggvalues[aggno] = pergroupstate->transValue;
			econtext->ecxt_aggnulls[aggno] = pergroupstate->transValueIsNull;
		}

		econtext->ecxt_scantuple = aggstate->tmpcontext->ecxt_scantuple;

		if (entry->shared.firstTuple)
		{
			heap_freetuple(entry->shared.firstTuple);
		}
		
		/* Replace it with a new tuple with new sequence number */
		entry->shared.firstTuple =
			heap_copytuple(econtext->ecxt_scantuple->val);
			
		return ExecProject(projInfo, NULL);

	}

	return NULL;				/* Nothing to return */
}

/*
 * Dumps the resulting tuple values from the stored hash table in
 * ASCII to the file, to test the result of extraction and
 * installation.
 */
void
ExecDumpHashGroup(HashGroup * node, FILE *fp)
{

	HashAggState *aggstate = node->agg.aggstate;
	TupleHashIterator *hashiter = &aggstate->hashiter;
	AggHashEntry entry;
	ExprContext *econtext;
	ProjectionInfo *projInfo;
	TupleTableSlot *firstSlot;
	TupleTableSlot *resultSlot;
	int			aggno;

	int			nprint = 0;

	elog(DEBUG1, "Printing %d ", nprint);
	ResetTupleHashIterator(hashiter);

	firstSlot = aggstate->csstate.css_ScanTupleSlot;
	econtext = aggstate->csstate.cstate.cs_ExprContext;
	projInfo = aggstate->csstate.cstate.cs_ProjInfo;

	entry = (AggHashEntry) ScanTupleHashTable(aggstate->hashtable,
											  hashiter);

	while (entry != NULL)
	{

		ExecStoreTuple(entry->shared.firstTuple,
					   firstSlot,
					   InvalidBuffer,
					   false);

		for (aggno = 0; aggno < aggstate->numaggs; aggno++)
		{
			AggStatePerGroup pergroupstate = &(entry->pergroup)[aggno];

			econtext->ecxt_aggvalues[aggno] = pergroupstate->transValue;
			econtext->ecxt_aggnulls[aggno] = pergroupstate->transValueIsNull;
		}

		econtext->ecxt_scantuple = firstSlot;
		resultSlot = ExecProject(projInfo, NULL);

		f_print_slot(resultSlot, fp);

		nprint++;
		elog(DEBUG1, "Printing %d ", nprint);

		entry = (AggHashEntry) ScanTupleHashTable(aggstate->hashtable,
												  hashiter);

	}

}

static TupleTableSlot *
getNextTuple_HashGroup(HashGroup *node)
{
	if (IsPushPlan(node))
	{
		if (IsInputDone(node)) return NULL;
		return ((HashAgg *) node)->aggstate->csstate.cstate.cs_OuterTupleSlot;
	}

	return ExecProcNode(outerPlan(node), (Plan *) node);
}

static TupleTableSlot *
exec_hashed_window_agg(HashGroup * gnode)
{
	HashAgg * node = ((HashAgg *) gnode);
	HashAggState *aggstate = node->aggstate;
	TupleTableSlot *outerslot,
			   *result;
	AggHashEntry entry;
	ExprContext *tmpcontext = aggstate->tmpcontext;

/* #define PRINT_RESULT */
		
#ifdef PRINT_RESULT
	int seqnum;
	bool isNull;
#endif

	if (gnode->frozen)
	{
		outerslot = getNextTuple_HashGroup(gnode);

		if (TupIsNull(outerslot))
		{
			return outerslot;
		}
		else
		{
			elog(DEBUG1, "Damn, frozen node, and we got a tuple.! Sleeping.");
			sleep(1000);
			elog(ERROR, "Waited too long, exiting");
		}
	}

	outerslot = getNextTuple_HashGroup(gnode);

	if (gnode->frozen)
	{
		if (TupIsNull(outerslot))
		{
			return outerslot;
		}
		else
		{
			elog(DEBUG1, "Damn, return frozen node, and we got a tuple.! Sleeping.");
			sleep(1000);
			elog(ERROR, "Waited too long, exiting");
		}
	}

	if (TupIsNull(outerslot))
	{
		return outerslot;
	}
	
	ResetExprContext(tmpcontext);

	tmpcontext->ecxt_scantuple = outerslot;

#ifdef PRINT_RESULT
	if (gnode->debug)
	{
		seqnum = heap_getattr(outerslot->val, 1,
							  outerslot->ttc_tupleDescriptor, &isNull);

		if (parentPlan(node))
		{
			elog(DEBUG1, "%d: Agg input: %d",
				 ((FluxProd_FT *) parentPlan(node))->m_role,
				 seqnum);
		}
	}
#endif	
	
	entry = lookup_hash_entry(node, outerslot);

#ifdef NOT_USED

	seqnum = heap_getattr(outerslot->val, 1,
						  outerslot->ttc_tupleDescriptor, &isNull);

	if (parentPlan(node))
	{
		elog(DEBUG1, "%d: Agg lookup: %d",
			 ((FluxProd_FT *) parentPlan(node))->m_role,
			 seqnum);
		print_slot(outerslot);

#ifdef NOT_USED
		if ((((FluxProd_FT *) parentPlan(node))->fprod.node_id == 1) &&
			(seqnum == 255))
		{
			elog(DEBUG1, "Waiting for debug ... ");
			sleep(10);
		}
#endif
		
	}
#endif

	result = advance_aggregates(aggstate, entry->pergroup, entry);

	if (IsPushPlan(node))
	{
		SetInputDone(node); /* We consumed an input here */
	}
	
	if (!TupIsNull(result))
	{

#ifdef PRINT_RESULT_TUPLE
		elog(DEBUG1, "Agg result: ");
		print_slot(result);
#endif

#ifdef PRINT_RESULT
		
		if (gnode->debug)
		{
			seqnum = heap_getattr(result->val, 1,
								  result->ttc_tupleDescriptor, &isNull);

			if (parentPlan(node)) 
			{
				elog(DEBUG1, "%d: Agg result: %d",
					 ((FluxProd_FT *) parentPlan(node))->m_role,
					 seqnum);
			}
		}
#endif

/* 		return result; */
	}

	/* Return something to the scheduler so it can do timing */

/* 	return outerslot; */

	return result;
}

/* ---------------------------------------
 *	 ExecHashGroup
 *
 *		Does hash-based grouping and counting.
 * ------------------------------------------
 */

TupleTableSlot *
ExecHashGroup(HashGroup * node)
{

	TupleTableSlot *slot;
	
	if (IsPushPlan(node) && parentPlan(node))
	{
		/* We do this so the FluxProd at top eventually get scheduled */
		ExecProcNode(parentPlan(node), (Plan *) node);

		if (!IsInputDone(parentPlan(node)))
		{
			/* No space to generate new inputs */
			return NULL;
		}
	}
	
	/* Its a windowed aggregation */
	slot = exec_hashed_window_agg(node);

	if (IsPushPlan(node) && parentPlan(node) && (!TupIsNull(slot)))
	{
		SetInputNew(parentPlan(node));
	}

	return slot;
}

/* -----------------
 * ExecInitHashGroup
 *
 * -----------------
 */
bool
ExecInitHashGroup(HashGroup * node, EState *estate, Plan *parent)
{
	int			aggno;

	if (IsPushPlan(node))
	{

		ExecInitHashAgg((HashAgg *) node, estate, parent);
		/* Initialize the parent plan */
		ExecInitNode(parentPlan(node), estate, (Plan *) node);
		SetInputDone(node);

		/* Set the outerslot to be the result slot of lower */
		((HashAgg *) node)->aggstate->csstate.cstate.cs_OuterTupleSlot =
			ExecGetTupSlot(outerPlan(node));
	}
	else
	{
		ExecInitHashAgg((HashAgg *) node, estate, parent);
	}
	
	/*
	 * This slot is used to temporarily store tuples for use in
	 * computation of intermediate aggregate *RESULTS*. The stored tuple
	 * inside will be used again.
	 */

	/* XXX I don't think this should be done, but since FluxProd is always
	   above it, it should be okay */
	node->agg.aggstate->csstate.cstate.cs_ResultTupleSlot->ttc_shouldFree = false;

	/*
	 * The tmpcontext->ecxt_scantuple stores the tuple from below. Thus,
	 * that is the slot that needs to be allocated and deallocated over
	 * and over again.
	 */

	/* Make sure we have the right number of window specifications */
	Assert(node->numaggs == node->agg.aggstate->numaggs);

	/* Set the window specifications appropriately */
	for (aggno = 0; aggno < node->numaggs; aggno++)
	{
		AggStatePerAgg peraggstate = &(node->agg.aggstate->peragg[aggno]);

		peraggstate->w_spec = &(node->wndSpecs[aggno]);

		/* XXX Sorry for the hack, need to fix catalog ... no time */
		if (peraggstate->transfn.fn_addr == int4count_w)
		{
/* 		peraggstate->transfn.fn_addr = int4count_w; */
			peraggstate->getstatefn.fn_addr = int4count_w_getstate;
			peraggstate->getsizefn.fn_addr = int4count_w_getsize;
		}
		else if (peraggstate->transfn.fn_addr == first_url_agg)
		{
			peraggstate->getstatefn.fn_addr = first_url_getstate;
			peraggstate->getsizefn.fn_addr = first_url_getsize;
		}
		else
		{
			elog(ERROR, "We currently don't allow this agg!");	
		}
	}

	node->frozen = false;

	node->debug = false;
	
	return TRUE;
}

int
ExecCountSlotsHashGroup(HashGroup * node)
{
	/* HashAgg will do the climb up or down the tree */
	return ExecCountSlotsHashAgg((HashAgg *) node);
}

#define PushBackHashEntry(entry, iter) ((iter)->next_entry = (TupleHashEntry) entry);

/*
 * Prepares the operator for obtaining the state.  Memory context
 * indicates the context in which to allocate for transfering
 * state. Returns true if ready.
 */
bool
ExecGetStateInitHashGroup(HashGroup * node)
{
	TupleHashIterator *hashiter = &(node->agg.aggstate->hashiter);

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup get state init");
#endif
		 
	node->parentDone = false;

	ExecGetStateInit(parentPlan(node));
	
	/* Need to walk the table */
	ResetTupleHashIterator(hashiter);

	node->frozen = true;
	
	return TRUE;
}

/*
 * This gets the next buffer's worth of state to transfer.
 * Returns true if we are done, false otherwise.
 */
bool
ExecGetStateNextHashGroup(HashGroup * node, char *buf, int bsize)
{
	/* First word indicates data size, and second word indidcates end of state */
#define HASHGROUP_HEADERSIZE 2*sizeof(int32)
	
	HashAggState *aggstate = node->agg.aggstate;
	TupleHashIterator *hashiter = &(aggstate->hashiter);
	int32		entrysize = aggstate->entrysize;
	int32		curr_pos = INTALIGN(HASHGROUP_HEADERSIZE);
	int32		entrydatasize = 0;
	int			aggno;
	int			tsize;
	AggStatePerGroup pergroup;

/*	AggStatePerAgg	 peragg; */
	int			numentries = 0;
	AggHashEntry entry;

	int         done;

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup get state next.");
	elog(DEBUG1, "Entrysize: %d", entrysize);
#endif
	
	if (!node->parentDone)
	{
		if (!ExecGetStateNext(parentPlan(node), buf, bsize))
			return FALSE;

		node->parentDone = true;
		return FALSE;
	}
	
	entry = (AggHashEntry) ScanTupleHashTable(aggstate->hashtable,
											  hashiter);

	while (entry != NULL)
	{

		entrydatasize = entrysize;

		/* TBD TBD TBD: Mehul, this code is probably broken with the new
		 * serializer. You've got to fix it
		 */
		tsize = tuple_serializer.determineSize((char *) entry->shared.firstTuple, NULL);

		entrydatasize += tsize;

		for (aggno = 0; aggno < aggstate->numaggs; aggno++)
		{
			/* peragg	= node->agg.aggstate->peragg[aggno] */
			pergroup = &(entry->pergroup[aggno]);
			entrydatasize += pergroup->as_size;
		}

		/* Cannot fit the data into rest of page */
		if (!(curr_pos + entrydatasize < bsize))
			break;

		/* Copy the entry into the buffer */
		memcpy(buf + curr_pos, entry, entrysize);
		curr_pos += entrysize;

		/* TBD TBD TBD Mehul - here too */
		tuple_serializer.serialize((char *) entry->shared.firstTuple,
								   buf + curr_pos,
								   bsize - curr_pos,
								   NULL);

		/* XXX This depends on the serializer */
		curr_pos += sizeof(int32) + tsize;

		for (aggno = 0; aggno < aggstate->numaggs; aggno++)
		{
			pergroup = &(entry->pergroup[aggno]);
			memcpy(buf + curr_pos, pergroup->agg_state, pergroup->as_size);
			curr_pos += pergroup->as_size;
			/* elog(DEBUG1, "AS SIZE: %d", pergroup->as_size); */
		}

		numentries++;

		entry = (AggHashEntry) ScanTupleHashTable(aggstate->hashtable,
												  hashiter);

	}

	/*
	 * Otherwise we have an entry that we cannot fit onto a page. XXX Have
	 * not yet coded ways to break apart entries.
	 */
	Assert(curr_pos != sizeof(int32));

	/* Indicate the size of data placed into buffer */
	memcpy(buf, &curr_pos, sizeof(int32));

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup: num entries this page: %d", numentries);
#endif

	if (entry == NULL) 
	{
		/* This is the last buffer to be sent */
		done = TRUE;
		memcpy(buf + sizeof(int32), &done, sizeof(int));
		return TRUE;			/* We are done */
	}
	
	/* Indicate futher space still needed to transfer state. */
	done = FALSE;
	memcpy(buf + sizeof(int32), &done, sizeof(int));
	
	/* Otherwise push the entry back, we'll need it again */
	PushBackHashEntry(entry, hashiter);
	return FALSE;
}

bool
ExecGetStateCloseHashGroup(HashGroup * node)
{
	node->frozen = false;

	node->debug = true;

	/* 
	elog(DEBUG1, "Dumping data to testfile");
	
	FILE *fp = fopen("TestFile", "w+");

	ExecDumpHashGroup(node, fp);

	fclose(fp);

	elog(DEBUG1, "Sleeping ... to check the files.");
	sleep(1000);
	*/
	
	return ExecGetStateClose(parentPlan(node));
}

/* Prepares the operator for installing the state */
bool
ExecInstallStateInitHashGroup(HashGroup * node)
{
	
#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup install state init");
#endif
	
	node->parentDone = false;

	node->frozen = true;
	
	return ExecInstallStateInit(parentPlan(node));
}

/*
 * Passes in the next page worth of stuff, current memory context
 * should be set to the hashtable context.
 */
bool
ExecInstallStateNextHashGroup(HashGroup * node, char *buf, int bsize)
{
	HashAggState *aggstate = node->agg.aggstate;
	int32		dsize;
	int32		curr_pos = INTALIGN(HASHGROUP_HEADERSIZE);
	int32		entrysize = aggstate->entrysize;
	TupleHashTable hashtable = aggstate->hashtable;
	MemoryContext ocxt;
	AggStatePerGroup pergroup;
	AggHashEntry entry;
	int			aggno;
	int			tsize;

	bool        done;
	int			numentries = 0;

	if (!node->parentDone)
	{
		if (!ExecInstallStateNext(parentPlan(node), buf, bsize))
			return FALSE;

		node->parentDone = true;
		return FALSE; /* Not done, but need the next page */
	}

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup install state next");
	elog(DEBUG1, "Entrysize: %d", entrysize);
#endif
	
	memcpy(&dsize, buf, sizeof(int32));
		
	ocxt = MemoryContextSwitchTo(hashtable->tablecxt);

	while (curr_pos < dsize)
	{

		entry = palloc(entrysize);

		memcpy(entry, buf + curr_pos, entrysize);

		curr_pos += entrysize;

		/* TBD TBD TBD Mehul - here too */
		tuple_serializer.deserialize(buf + curr_pos,
									 (char **) &(entry->shared.firstTuple),
									 NULL);

		/* Find out the amount we just withdrew ... */
		tsize = tuple_serializer.determineSize((char *) entry->shared.firstTuple, NULL);

		/* XXX This depends on the serializer */
		curr_pos += sizeof(int32) + tsize;

		for (aggno = 0; aggno < aggstate->numaggs; aggno++)
		{
			pergroup = &(entry->pergroup[aggno]);
			/* elog(DEBUG1, "AS SIZE: %d", pergroup->as_size); */
			pergroup->agg_state = palloc(pergroup->as_size);
			memcpy(pergroup->agg_state, buf + curr_pos, pergroup->as_size);
			curr_pos += pergroup->as_size;
		}

		InsertTupleHashEntry(hashtable, (TupleHashEntry) entry);
		
#ifdef NOT_USED
		if ((rentry = InsertTupleHashEntry(hashtable, (TupleHashEntry) entry)) != NULL)
		{
			elog(ERROR, "Got an entry with matching hashkeys ... sleeping");
			elog(FATAL, "Entry already exists while installing state.");
		}
#endif
		
		numentries++;
	}

	MemoryContextSwitchTo(ocxt);

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup: Entries installed: %d", numentries);
#endif

	memcpy(&done, buf + sizeof(int32), sizeof(int32));

#ifdef DRECOVERY
	if (done)
	{
		elog(DEBUG1, "Hashgroup: Not done yet.");
	}
	else
	{
		elog(DEBUG1, "Hashgroup: Done installing state.");
	}
#endif
	
	return done;
}

/* Nothing to do here */
bool
ExecInstallStateCloseHashGroup(HashGroup * node)
{
	node->frozen = false;

	node->debug = true;
	
	ExecInstallStateClose(parentPlan(node));

#ifdef DRECOVERY
	elog(DEBUG1, "Hashgroup: install state close.");
#endif

	return true;
}

/* Resets to the initial state */
bool
ExecResetStateHashGroup(HashGroup * node)
{

	/* Code borrowed from ExecReScanAgg */
	HashAgg    *agg_node = (HashAgg *) node;
	HashAggState *aggstate = node->agg.aggstate;
	ExprContext *econtext = aggstate->csstate.cstate.cs_ExprContext;

	aggstate->agg_done = false;
	MemSet(econtext->ecxt_aggvalues, 0, sizeof(Datum) * aggstate->numaggs);
	MemSet(econtext->ecxt_aggnulls, 0, sizeof(bool) * aggstate->numaggs);

	/* BEGIN: CHECK */
	MemoryContextReset(aggstate->aggcontext);

	if ((agg_node->aggstrategy == AGG_HASHED) ||
		(agg_node->aggstrategy == AGG_HASHED_WINDOW));
	{
		build_hash_table(agg_node);
		aggstate->table_filled = false;
	}
	/* END: CHECK */

	return true;
}

/* ------------------------
 *		ExecEndHashGroup
 *
 * -----------------------
 */
void
ExecEndHashGroup(HashGroup * node)
{
	ExecEndHashAgg((HashAgg *) node);
}
