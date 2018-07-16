/*-------------------------------------------------------------------------
 *
 * nodeStem.c
 *	  Routine for SteMs -- SteMs mainly support two operations -- build and getnext()
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * Concerns :
 *	  -- What happens to the timestamps when there is demotion/promotion ?
 *	  -- If the current partition in memory is to be swapped out, then
 *		 we have the problem that it might be too big, and our repartitioning
 *		 strategy doesn't like that. One solution might be to create the
 *		 required number of partitions before dumping the current partition
 *		 to disk.
 *
 *
 *	$Id: nodeSteM.c,v 1.49 2006/02/06 01:19:46 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *		ExecInitSteM	- initialize node and subnodes
 *		ExecGetNextSteM - get the next match
 *		ExecBuildSteM	- build a tuple into the SteM
 *		ExecEndSteM		- shutdown node and subnodes
 */
#include "postgres.h"
#include "access/printtup.h"

#include <sys/types.h>
#include <math.h>
#include <assert.h>

#include "executor/execdebug.h"
#include "executor/nodeHash.h"
#include "executor/nodeHashjoin.h"
#include "executor/nodeSteM.h"
#include "executor/nodeEddy.h"
#include "miscadmin.h"
#include "parser/parse_expr.h"
#include "parser/parsetree.h"
#include "utils/memutils.h"
#include "utils/lsyscache.h"

/*#define ENABLE_DISK_BASED */
#define __MAX_PATITIONS_ALLOWED__ 8

/* #define AMOLDEBUGON 1  */

uint32		hashFunc(Datum key, int len, bool byVal);
void		printSteMHashJoinTable_debug(SteMHashJoinTable shjt);
static void printBucket(SteMHashJoinTable shjt, int bucket_no, TupleDesc *base_tuple_descs);

/** Some function to deal with partitioninginformation structure */
void		init_partitioning_information(SteMState * stemstate);
inline int			get_partition_number(SteMState * stemstate, int bucketno);
inline bool		bucket_in_memory(SteMState * stemstate, int bucketno);
inline BufFile    *get_partition_file(SteMState * stemstate, int bucketno);
bool		repartitioning_required(SteMState * stemstate);
void		repartition(SteMState * stemstate);
void		print_partitioning_information(SteMState * stemstate);
void		print_pending_tuples_information(SteMState * stemstate);

static void delete_stemhashjointuple(SteMHashJoinTuple shj_tuple);


void		ExecSteMRemovePartition(SteMState * stemstate, int partition_no);
void		ExecSteMFetchPartition(SteM * stem, int new_partition_no);
void		ExecSteMRemoveOldAndFetchNewPartition(SteM * stem, int new_partition_no);

void		ExecSteMSaveSteMHashTuple(SteMHashJoinTuple stemHashTuple, BufFile *file);
void		ExecSteMSaveSteMHashTupleGivenIntermediateTuple(IntermediateHeapTuple htup, int bucketno, BufFile *file);
SteMHashJoinTuple ExecSteMGetSavedSteMHashTuple(BufFile *file, MemoryContext iContext, MemoryContext bContext);

static HeapTuple ExecHashJoinGetSavedBaseTuple(BufFile *file, MemoryContext bContext);

bool stem_contains_tuple(SteM * stem, ExprContext *econtext);

/** XXX NOT USED */
IntermediateHeapTuple ExecHashJoinGetSavedIntermediateTuple(BufFile *file,
						 MemoryContext iContext, MemoryContext bContext);
void		ExecHashJoinSaveIntermediateTuple(IntermediateHeapTuple iheapTuple, BufFile *file);

/** Defined in nodeSQEddy.c */
inline int	convertBitArraytoInteger(BitArray ba);

/* ----------------------------------------------------------------
 *		ExecInitSteM
 *
 *		Init routine for SteM node
 * ----------------------------------------------------------------
 */
bool
ExecInitSteM(SteM * node, EState *estate, Plan *parent)
{
	SteMState  *stemstate;
	int			i;

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create state structure
	 */
	stemstate = makeNode(SteMState);
	node->stemstate = stemstate;
	stemstate->stemhashtable = NULL;
	stemstate->sm_NeedNewOuter = true;
	stemstate->sm_CurTuple = NULL;
	stemstate->sm_CurBucketNo = 0;
	stemstate->sm_OuterTupleSlot = NULL;
	stemstate->sm_tuplesProbed = 0;
	stemstate->sm_tuplesReturned = 0;
	stemstate->sm_tuplesInserted = 0;
	stemstate->sm_promotion_mode = false;
	stemstate->oldest_tuple = NULL;
	stemstate->newest_tuple = NULL;

	stemstate->pendingProbeTuples = NULL;
	stemstate->current_partition_in_memory = 0;
	stemstate->num_total_pending_probes = 0;

	/*
	 * The partitioning structure itself also has to be initialized. This
	 * is done later as it depends on the number of buckets.
	 */

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &stemstate->cstate);

#define HASH_NSLOTS 1

	/** We need to initialize
	 *		result slot
	 *		We also need to initialize a temporary tuple slot for storing
	 *			matched inner tuples
	 *		NOTE/XXX: We seem to be needing another tupleSlot for storing a pending
	 *			probe tuple that is now being used as the outer tuple. Unfortunately,
	 *			there doesn't seem to be any tupleslot that can be used here, so for
	 *			the moment being, we will create a new tupleslot
	 */

#ifndef DISABLE_CQ_SUPPORT
	if (ExecUseNewSlots((Plan *) node, parent)) /* Cannot use executor
												 * TupleTable */

	{
		stemstate->cstate.cs_ResultTupleSlot = MakeTupleTableSlot();

		stemstate->sm_HashTupleSlot = MakeTupleTableSlot();
		stemstate->sm_PendingOuterTupleSlot = MakeTupleTableSlot();

		InitSlotForEddy(stemstate->cstate.cs_ResultTupleSlot);
		InitSlotForEddy(stemstate->sm_HashTupleSlot);
	}
	else
	{
		ExecInitResultTupleSlot(estate, &stemstate->cstate);

		stemstate->sm_HashTupleSlot = ExecInitExtraTupleSlot(estate);
		stemstate->sm_PendingOuterTupleSlot = ExecInitExtraTupleSlot(estate);

	}

#else							/* DISABLE_CQ_SUPPORT */

	ExecInitResultTupleSlot(estate, &stemstate->cstate);

	stemstate->sm_HashTupleSlot = ExecInitExtraTupleSlot(estate);
	stemstate->sm_PendingOuterTupleSlot = ExecInitExtraTupleSlot(estate);
#endif

	stemstate->sm_HashTupleSlot->ttc_isIntermediateTuple = true;

	/*
	 * As opposed to hash nodes, we must create the hashtable right here.
	 */
	stemstate->stemhashtable = ExecSteMHashTableCreate(node);

	/*
	 * Also create a hashtable for pending tuples
	 */
	stemstate->pendingProbeTuples = ExecSteMHashTableCreate(node);

	/* Random Sanity Check */
	Assert(stemstate->stemhashtable->totalbuckets == stemstate->pendingProbeTuples->totalbuckets);

	/*
	 * Try to abstract the functionality away so that it is easier to
	 * change later
	 */
	init_partitioning_information(stemstate);

	stemstate->num_inserted_tuples_per_bucket = (int *) palloc(sizeof(int) * stemstate->stemhashtable->totalbuckets);
	for (i = 0; i < stemstate->stemhashtable->totalbuckets; i++)
		stemstate->num_inserted_tuples_per_bucket[i] = 0;

	/*
	 * If we do have an innernode (note that we are calling this inner
	 * node, whereas the Hash node calls it outer.
	 */
	if (node->innerNode)
		ExecInitNode(node->innerNode, estate, (Plan *) node);

	/*
	 * Statistics -- currently we allow maintaining stats for various
	 * bitsets for either insertion, probing or returning.
	 *
	 * Initialize the structure, and set the correct bitsets.
	 */
	init_statistics(&node->stats);

	node->stats.bitsets_to_track_for_inserted[0] = 1 << (node->innerhashkey_var->varnoold - 1);	/* usual problems with
																										 * varnoold */
	node->stats.bitsets_to_track_for_probed[0] = 1 << (node->outerhashkey_var->varnoold - 1);
	node->stats.bitsets_to_track_for_returned[0] = node->stats.bitsets_to_track_for_probed[0] | node->stats.bitsets_to_track_for_inserted[0];

	node->stats.num_bitsets_to_track_for_inserted = 1;
	node->stats.num_bitsets_to_track_for_probed = 1;
	node->stats.num_bitsets_to_track_for_returned = 1;

	return TRUE;
}

int
ExecCountSlotsSteM(SteM * node)
{
	if (node->innerNode)
	{
		return ExecCountSlotsNode((Plan *) node->innerNode) + HASH_NSLOTS
			+ 1					/* for the temporary slot sm_HashTupleSlot */
			+ 1					/* for the temporary slot
				sm_PendingOuterTupleSlot */ ;
	}
	else
	{
		return HASH_NSLOTS
			+ 1					/* for the temporary slot sm_HashTupleSlot */
			+ 1					/* for the temporary slot
				sm_PendingOuterTupleSlot */ ;
	}
}

/* ---------------------------------------------------------------
 *		ExecEndSteM
 *
 *		clean up routine for SteM node
 * ----------------------------------------------------------------
 */
void
ExecEndSteM(SteM * node)
{
	SteMState  *stemstate = NULL;

	/*
	 * get info from the SteM state
	 */
	stemstate = node->stemstate;

#ifdef AMOLDEBUGON
	printf("stems counters : inserted = %d, probed = %d, returned = %d\n",
		   stemstate->sm_tuplesInserted,
		   stemstate->sm_tuplesProbed,
		   stemstate->sm_tuplesReturned);

	print_statistics(&node->stats);
#endif
	
	/* print_pending_tuples_information(stemstate); */

	/*
	 * free projection info.  no need to free result type info because
	 * that came from the outer plan...
	 */
	ExecFreeProjectionInfo(&stemstate->cstate);
	ExecFreeExprContext(&stemstate->cstate);
	ExecClearTuple(stemstate->cstate.cs_ResultTupleSlot);
	ExecClearTuple(stemstate->sm_HashTupleSlot);
	ExecFreeSlotIfOwned(stemstate->cstate.cs_ResultTupleSlot);
	ExecFreeSlotIfOwned(stemstate->sm_HashTupleSlot);

	/* tcqoc */
	ExecSteMHashTableDestroy(stemstate->stemhashtable);
	ExecSteMHashTableDestroy(stemstate->pendingProbeTuples);
	/* <added by Shawn> */
	pfree(stemstate->num_pending_probes_per_bucket);
	pfree(stemstate->partitioning.partition_numbers);
	pfree(stemstate->num_inserted_tuples_per_bucket);
	/* </added by Shawn> */
	pfree(node->stemstate);
	node->stemstate = stemstate = NULL;


	if (node->innerNode)
		ExecEndNode((Plan *) node->innerNode, (Plan *) node);
}


/* ----------------------------------------------------------------
 *		ExecSteMHashTableCreate
 *
 *		create a hashtable in shared memory for a stem.
 * ----------------------------------------------------------------
 */
SteMHashJoinTable
ExecSteMHashTableCreate(SteM * node)
{

	SteMHashJoinTable stemhashtable;
	int			totalbuckets;
	int			nbuckets;
	int			nbatch;
	int			i;
	MemoryContext oldcxt;

	/*
	 * Since we don't really know anything about the expected size of the
	 * hash table, we will just choose arbitrarily. Postgres hashtable
	 * seems to be complete statically allocated, which might be a problem
	 * later.
	 */
	totalbuckets = 1000;
	nbuckets = 1000;
	nbatch = 1;

	/*
	 * Initialize the hash table control block.
	 *
	 * The hashtable control block is just palloc'd from the executor's
	 * per-query memory context.
	 */
	stemhashtable = (SteMHashJoinTable) palloc(sizeof(SteMHashTableData));
	stemhashtable->nbuckets = nbuckets;
	stemhashtable->totalbuckets = totalbuckets;
	stemhashtable->buckets = NULL;
	stemhashtable->nbatch = nbatch;
	stemhashtable->curbatch = 0;
	stemhashtable->innerBatchFile = NULL;
	stemhashtable->outerBatchFile = NULL;
	stemhashtable->innerBatchSize = NULL;
	stemhashtable->outerBatchSize = NULL;

	/*
	 * Get info about the datatype of the hash key.
	 */
	get_typlenbyval(exprType(node->innerhashkey),
					&stemhashtable->typLen, &stemhashtable->typByVal);

	/*
	 * See notes in executor/stem.h.
	 */
	stemhashtable->stemContext = AllocSetContextCreate(CurrentMemoryContext,
													   "SteMContext",
												ALLOCSET_DEFAULT_MINSIZE,
											   ALLOCSET_DEFAULT_INITSIZE,
											   ALLOCSET_DEFAULT_MAXSIZE);

	stemhashtable->itupleContext = AllocSetContextCreate(stemhashtable->stemContext,
										  "SteMIntermediateTupleContext",
												ALLOCSET_DEFAULT_MINSIZE,
											   ALLOCSET_DEFAULT_INITSIZE,
											   ALLOCSET_DEFAULT_MAXSIZE);

	stemhashtable->btupleContext = AllocSetContextCreate(stemhashtable->stemContext,
												  "SteMBaseTupleContext",
												ALLOCSET_DEFAULT_MINSIZE,
											   ALLOCSET_DEFAULT_INITSIZE,
											   ALLOCSET_DEFAULT_MAXSIZE);

	oldcxt = MemoryContextSwitchTo(stemhashtable->stemContext);

	stemhashtable->buckets = (SteMHashJoinTuple *) palloc(nbuckets *
											  sizeof(SteMHashJoinTuple));

	if (stemhashtable->buckets == NULL)
		elog(ERROR, "Insufficient memory for hash table.");


	for (i = 0; i < nbuckets; i++)
		stemhashtable->buckets[i] = NULL;

	MemoryContextSwitchTo(oldcxt);

	return stemhashtable;
}



/* ----------------------------------------------------------------
 *		ExecSteMHashTableDestroy
 *
 *		destroy a stem hash table
 * ----------------------------------------------------------------
 */
void
ExecSteMHashTableDestroy(SteMHashJoinTable hashtable)
{
	int			i;

	/* Make sure all the temp files are closed */
	for (i = 0; i < hashtable->nbatch; i++)
	{
		if (hashtable->innerBatchFile && hashtable->innerBatchFile[i])
			BufFileClose(hashtable->innerBatchFile[i]);
		if (hashtable->outerBatchFile && hashtable->outerBatchFile[i])
			BufFileClose(hashtable->outerBatchFile[i]);
	}

	/*
	 * Release working memory XXX not clear whether this should be the
	 * semantics
	 */
	if (hashtable->stemContext)
		MemoryContextDelete(hashtable->stemContext);

	/* And drop the control block */
	pfree(hashtable);
}

IntermediateHeapTuple ExecSteMHashTableInsertInBucket(SteM * stem,
								SteMHashJoinTable hashtable, IntermediateHeapTuple iheapTuple, int bucketno, bool whether_to_copy);


void
ExecSteMInsert(SteM * stem, ExprContext *econtext, bool whether_to_copy)
{
	IntermediateHeapTuple inserted_tuple;
	int			bucketno;

#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d %x\n", __FILE__, __LINE__, (unsigned int) stem->stemstate);
#endif

	/* Increase the count */
	stem->stemstate->sm_tuplesInserted++;

	update_stats_inserted_tuple(&stem->stats,
								convertBitArraytoInteger(((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_sources),
								1);

#ifdef ENABLE_DISK_BASED
	/* Check if repartitioning should be performed at this point */
	if (repartitioning_required(stem->stemstate))
	{
		printf("Repartitioning the hash table \n");
		print_partitioning_information(stem->stemstate);
		repartition(stem->stemstate);
		print_partitioning_information(stem->stemstate);
	}
#endif   /* ENABLE_DISK_BASED */

	/* Find the bucket number */
	bucketno = ExecSteMHashGetBucket(stem, (SteMHashJoinTable) stem->stemstate->stemhashtable, econtext, stem->innerhashkey);

	/* bookkeeping */
	stem->stemstate->num_inserted_tuples_per_bucket[bucketno]++;

	if (bucket_in_memory(stem->stemstate, bucketno))
	{
		inserted_tuple = ExecSteMHashTableInsertInBucket(stem, stem->stemstate->stemhashtable,
														 (IntermediateHeapTuple) econtext->ecxt_scantuple->val, bucketno, whether_to_copy);
		if (get_partition_file(stem->stemstate, bucketno))
		{
			/*
			 * In that case, we should also append this tuple to the
			 * corresponding file
			 */
			Assert(!"Not implemented yet");
			Assert(econtext->ecxt_scantuple->ttc_isIntermediateTuple);
			ExecSteMSaveSteMHashTupleGivenIntermediateTuple((IntermediateHeapTuple) econtext->ecxt_scantuple->val,
				bucketno, get_partition_file(stem->stemstate, bucketno));

			/** XXX Since this is a new tuple, the timestamp must be higher than anything already written ? */
			Assert(stem->stemstate->partitioning.partitions
				   [get_partition_number(stem->stemstate, bucketno)].last_timestamp_written
				   < ((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_timestamp);
			stem->stemstate->partitioning.partitions
				[get_partition_number(stem->stemstate, bucketno)].last_timestamp_written
				= ((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_timestamp;
		}

		/** XXX : The code following probably does not make sense if the tuple is sent to
		 * disk */

		/* If we're creating the first tuple in this STeM, remember it. */
		if (NULL == stem->stemstate->oldest_tuple)
		{
			stem->stemstate->oldest_tuple = inserted_tuple;

			/*
			 * Note that this code assumes that no tuples ever disappear
			 * from the SteM!
			 */
		}

		stem->stemstate->newest_tuple = inserted_tuple;
	}
	else
	{
		/*
		 * printf("Bucket no. %d, partition no. = %d\n", bucketno,
		 * get_partition_number(stem->stemstate, bucketno));
		 */
		inserted_tuple = NULL;
		Assert(econtext->ecxt_scantuple->ttc_isIntermediateTuple);
		Assert(get_partition_file(stem->stemstate, bucketno));
		ExecSteMSaveSteMHashTupleGivenIntermediateTuple((IntermediateHeapTuple) econtext->ecxt_scantuple->val, bucketno,
						  get_partition_file(stem->stemstate, bucketno));

		/** Since this is a new tuple, the timestamp must be higher than anything already written ? */
		Assert(stem->stemstate->partitioning.partitions
			   [get_partition_number(stem->stemstate, bucketno)].last_timestamp_written
			   < ((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_timestamp);
		stem->stemstate->partitioning.partitions
			[get_partition_number(stem->stemstate, bucketno)].last_timestamp_written
			= ((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_timestamp;
	}
}

/* ----------------------------------------------------------------
 *		ExecHashTableInsert
 *
 *		insert a tuple into the hash table depending on the hash value
 *		it may just go to a tmp file for other batches
 *
 *		Returns a pointer to the tuple inserted.
 *
 *		NOTE/FIXME: The ExecSteMFetchPartition/RemovePartition also manipulate
 *		the SteMHashTable
 *		directly. If any bookkeeping is done in these functions, should be done
 *		there as well. Preferrably, change the interfaces sometime.
 * ----------------------------------------------------------------
 */
IntermediateHeapTuple
ExecSteMHashTableInsert(SteM * stem, SteMHashJoinTable hashtable,
						ExprContext *econtext,
						Node *hashkey,
						bool whether_to_copy)
{
	int			bucketno = ExecSteMHashGetBucket(stem, (SteMHashJoinTable) hashtable, econtext, hashkey);
	TupleTableSlot *slot = econtext->ecxt_scantuple;

	return ExecSteMHashTableInsertInBucket(stem, hashtable, (IntermediateHeapTuple) slot->val, bucketno, whether_to_copy);
}


IntermediateHeapTuple
ExecSteMHashTableInsertInBucket(SteM * stem, SteMHashJoinTable hashtable,
	IntermediateHeapTuple iheapTuple, int bucketno, bool whether_to_copy)
{
	/*
	 * put the tuple in hash table
	 */
	SteMHashJoinTuple stemHashTuple;
	int			hashTupleSize;

	hashTupleSize = MAXALIGN(sizeof(SteMHashJoinTupleData));

	stemHashTuple = (SteMHashJoinTuple) MemoryContextAlloc(hashtable->itupleContext, hashTupleSize);

	stemHashTuple->bucket_no = bucketno;

	if (stemHashTuple == NULL)
		elog(ERROR, "Insufficient memory for hash table.");

	memcpy((char *) &stemHashTuple->htup,
		   (char *) iheapTuple,
		   sizeof(stemHashTuple->htup));

	stemHashTuple->htup.t_datamcxt = hashtable->itupleContext;

#ifndef DISABLE_CQ_SUPPORT
	BitArrayClearAll(stemHashTuple->htup.t_ready, TCQMAXOPERATORS);
	BitArrayClearAll(stemHashTuple->htup.t_done, TCQMAXOPERATORS);
	if ((! UseOperatorPrecedence) && (! UseTupleLineage)) 
	{
		BitArrayClearAll(stemHashTuple->htup.t_queriesCompleted, TCQMAXQUERIES);
	}
#endif

	stemHashTuple->next = hashtable->buckets[bucketno];

	hashtable->buckets[bucketno] = stemHashTuple;

	/*
	 * Copy the base tuples into our memory. Need to switch the pointers
	 * in the process.
	 */
	if (whether_to_copy)
		copy_base_tuples_into_context(&stemHashTuple->htup, hashtable->btupleContext);


	/*
	 * If the promotion mode is set to true, then we also might have to
	 * take care of some pointers.
	 */
	if (stem->stemstate->sm_promotion_mode == true)
	{
		/*
		 * We shouldn't be inserting into any other bucket than the
		 * current bucket no.
		 */
		Assert(bucketno == stem->stemstate->sm_current_promoted_tuple_bucket_no);

		if (stem->stemstate->sm_tuple_before_current_promoted_tuple == NULL)
			stem->stemstate->sm_tuple_before_current_promoted_tuple = hashtable->buckets[bucketno];
	}

	return &(stemHashTuple->htup);
}

/* ----------------------------------------------------------------
 *		ExecSteMDeleteFirstTupleBucket
 *
 *		Delete the first element from the given hash bucket.
 *
 *		We only allow this currently as deleting an arbitary stemHashTuple is not very easy.
 * ----------------------------------------------------------------
 */
void
ExecSteMHashTableDeleteFirstTupleBucket(SteMHashJoinTable hashtable, int bucketNo)
{
	int			i;

	SteMHashJoinTuple stemHashTuple = hashtable->buckets[bucketNo];

	IntermediateHeapTuple iht = &stemHashTuple->htup;

	/* first delete the base tuples from the btuplecontext */
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iht->t_base_tuples[i] != NULL)
			pfree(iht->t_base_tuples[i]);
	}

	hashtable->buckets[bucketNo] = stemHashTuple->next;

	pfree(stemHashTuple);
}

/*
 *	Delete a given SteMHashJoinTuple  -- probably duplicates the above function.
 *
 *	This only deletes, you take care of "next" pointers.
 */
static void
delete_stemhashjointuple(SteMHashJoinTuple shj_tuple)
{
	/* free the base tuples */
	int			i;
	IntermediateHeapTuple iht = &shj_tuple->htup;

	/* first delete the base tuples from the btuplecontext */
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iht->t_base_tuples[i] != NULL)
			pfree(iht->t_base_tuples[i]);
	}

	pfree(shj_tuple);
}


/* ----------------------------------------------------------------
 *		ExecHashGetBucket
 *
 *		Get the hash value for a tuple
 * ----------------------------------------------------------------
 */
int
ExecSteMHashGetBucket(SteM * stem,
                      SteMHashJoinTable hashtable,
					  ExprContext *econtext,
					  Node *hashkey)
{
	int			bucketno;
	Datum		keyval;
	bool		isNull;
	MemoryContext oldContext;

	/*
	 * We reset the eval context each time to reclaim any memory leaked in
	 * the hashkey expression or hashFunc itself.
     *
	 */

	oldContext = MemoryContextSwitchTo(econtext->ecxt_per_tuple_memory);

	/*
	 * Get the join attribute value of the tuple
	 */
	keyval = ExecEvalExpr(hashkey, econtext, &isNull, NULL);

    /*
     * Update the domain sizes information 
     */
/*
    update_domain_size(stem->domain_sizes_information, 
            (stem->outerhashkey == hashkey) ? stem->outerhashkey_index : stem->innerhashkey_index,
            (int) keyval);

*/

	/*
	 * Compute the hash function
	 */
	if (isNull)
		bucketno = 0;
	else
		bucketno = hashFunc(keyval, (int) hashtable->typLen, hashtable->typByVal) % hashtable->totalbuckets;

	MemoryContextSwitchTo(oldContext);

	return bucketno;
}

/* ----------------------------------------------------------------
 *		ExecScanSteMHashBucket
 *
 *		scan a stem hash bucket of matches
 * ----------------------------------------------------------------
 */
IntermediateHeapTuple
ExecScanSteMHashBucket(SteMState * stemstate, List *stemclauses, ExprContext *econtext)
{
	SteMHashJoinTable stemhashtable = stemstate->stemhashtable;
	SteMHashJoinTuple stemHashTuple = stemstate->sm_CurTuple;

	if (stemHashTuple == NULL)
		stemHashTuple = stemhashtable->buckets[stemstate->sm_CurBucketNo];
	else
		stemHashTuple = stemHashTuple->next;

	while (stemHashTuple != NULL)
	{
		IntermediateHeapTuple heapTuple = &stemHashTuple->htup;
		TupleTableSlot *inntuple;

		/* insert hashtable's tuple into exec slot so ExecQual sees it */
		inntuple = ExecStoreTuple((HeapTuple) heapTuple,		/* tuple to store */
								  stemstate->sm_HashTupleSlot,	/* slot */
								  InvalidBuffer,
								  false);		/* do not pfree this tuple */

		econtext->ecxt_innertuple = inntuple;

		econtext->ecxt_outertuple = stemstate->sm_OuterTupleSlot;

		/* reset temp memory each time to avoid leaks from qual expression */
		ResetExprContext(econtext);

		if (ExecQual(stemclauses, econtext, false))
		{
			stemstate->sm_CurTuple = stemHashTuple;
			return heapTuple;
		}

		stemHashTuple = stemHashTuple->next;

		if (stemHashTuple == NULL)
			stemstate->sm_NeedNewOuter = true;
	}

	/*
	 * no match
	 */
	return NULL;
}


/* ----------------------------------------------------------------
 *		ExecHashTableReset
 *
 *		reset hash table header for new batch
 *
 *		ntuples is the number of tuples in the inner relation's batch
 *		(which we currently don't actually use...)
 * ----------------------------------------------------------------
 */
void
ExecSteMHashTableReset(SteMHashJoinTable hashtable, long ntuples)
{
	Assert(!"Shouldn't come here since we have only one batch.");
}

TupleTableSlot *
ExecSteM(SteM * stem)
{
	if (stem->stemstate->sm_promotion_mode == true)
	{
		Assert(stem->stemstate->sm_NeedNewOuter);		/* sanity check */
		return ExecSteMPromoteGetNext(stem);
	}
	else
		return ExecSteMProbeGetNext(stem);
}


TupleTableSlot *
ExecSteMProbeGetNext(SteM * stem)
{
	SteMState  *stemstate = stem->stemstate;
	ExprContext *econtext = stemstate->cstate.cs_ExprContext;

	IntermediateHeapTuple curTuple,
				returnval;
	TupleTableSlot *result;

	if (stemstate->sm_NeedNewOuter)
		return NULL;
	else
	{
		/*
		 * OK, scan the selected hash table for matches
		 */
		for (;;)
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: IN GETNEXT %s %d %d\n", __FILE__, __LINE__, stemstate->sm_CurBucketNo);
			debugitup_debug((IntermediateHeapTuple) stemstate->sm_OuterTupleSlot->val);
			printSteMHashJoinTable_debug(stem->stemstate->stemhashtable);
#endif

			curTuple = ExecScanSteMHashBucket(stemstate, stem->stemclauses, econtext);
			if (curTuple == NULL)
				break;

			/* Time stamp checking for duplicate elimination. */

			/*
			 * printf("%f %f\n", curTuple->t_timestamp,
			 * ((IntermediateHeapTuple)
			 * stemstate->sm_OuterTupleSlot->val)->t_timestamp);
			 */

			/* TBD TBD TBD : Amol - I'm commenting this so that tcq
			 * functionality continues to work !
			 */
			
/* 			if (curTuple->t_timestamp > ((IntermediateHeapTuple) stemstate->sm_OuterTupleSlot->val)->t_timestamp) */
/* 				continue; */

			/*
			 * We've got a match. At this point, we should check for
			 * non-hashed quals as Hashjoin does. We will skip this check
			 * for the moment being, and just create an output tuple.
			 */
#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: found a match %s %d\n", __FILE__, __LINE__);
			debugitup_debug(curTuple);
#endif

			/*
			 * debugitup((IntermediateHeapTuple)
			 * stemstate->sm_OuterTupleSlot->val);
			 */

			/*
			 * Use the temp intermediate heap tuple to return the result
			 * in.
			 */
			returnval = &stemstate->sm_tempiHeapTupleData;
			copyIntermediateHeapTuple(returnval, (IntermediateHeapTuple) stemstate->sm_OuterTupleSlot->val);
			AddBaseTuples(returnval, curTuple);

#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: %s %d %s\n", __FILE__, __LINE__, "printing the tuple returned");
			debugitup_debug(returnval);
#endif

			/*
			 * Right now we will just use the sm_HashTupleSlot which is
			 * temporary anyway. Later should figure out how to use
			 * ExecProject for this purpose.
			 */

			result = ExecStoreTuple((HeapTuple) returnval,		/* tuple to store */
									stemstate->sm_HashTupleSlot,		/* slot */
									InvalidBuffer,
									false);		/* do not pfree this tuple */

			stemstate->sm_tuplesReturned++;


			update_stats_returned_tuple(&stem->stats,
										convertBitArraytoInteger(((IntermediateHeapTuple) returnval)->t_sources),
										1);

			return result;
		}
	}

	/*
	 * Current outer tuple has run out of matches
	 */
	stemstate->sm_NeedNewOuter = true;

	return NULL;
}

/** XXX Caution: Some code duplicated below. */
void
ExecSteMSetNewOuter(SteM * stem, ExprContext *econtext)
{
#ifdef AMOLDEBUGON
	elog(DEBUG1,"ExecSteMSetNewOuter ..");
#endif

	SteMState  *stemstate = stem->stemstate;

	stemstate->sm_tuplesProbed++;

	update_stats_probed_tuple(&stem->stats,
							  convertBitArraytoInteger(((IntermediateHeapTuple) econtext->ecxt_scantuple->val)->t_sources),
							  1);

	/*
	 * This SteM better be expecting a new outer
	 */
	Assert(stemstate->sm_NeedNewOuter);

	stemstate->sm_CurBucketNo = ExecSteMHashGetBucket(stem, stemstate->stemhashtable, econtext, stem->outerhashkey);
	stemstate->sm_CurTuple = NULL;

	if (!bucket_in_memory(stemstate, stemstate->sm_CurBucketNo))
	{
		/*
		 * insert into the pendingprobetuples instead, and set
		 * sm_NeedNewOuter to true
		 */
#ifdef AMOLDEBUGON
	elog(DEBUG1,"ExecSteMSetOuterTupleNext ..(in  the !bucket_in_memory arm)");
#endif

		ExecSteMHashTableInsertInBucket(stem, stem->stemstate->pendingProbeTuples,
										(IntermediateHeapTuple) econtext->ecxt_scantuple->val,
										stemstate->sm_CurBucketNo, true /* copy */ );
		stemstate->num_pending_probes_per_bucket[stemstate->sm_CurBucketNo]++;
		stemstate->num_total_pending_probes++;
	}
	else
	{
#ifdef AMOLDEBUGON
	elog(DEBUG1,"FSteMSetOuterTupleNext ..(in  the bucket_in_memory arm)");
#endif
		stemstate->sm_NeedNewOuter = false;
		stemstate->sm_OuterTupleSlot = econtext->ecxt_scantuple;
	}

#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d %d\n", __FILE__, __LINE__, stemstate->sm_CurBucketNo);
	debugitup_debug((IntermediateHeapTuple) stemstate->sm_OuterTupleSlot->val);
	printSteMHashJoinTable_debug(stem->stemstate->stemhashtable);
#endif
	/**
	elog(DEBUG1, "Setting this new outer");
	debugitup_debug((IntermediateHeapTuple) stemstate->sm_OuterTupleSlot->val);
	elog(DEBUG1, "for this hash table");
	printSteMHashJoinTable_debug(stem->stemstate->stemhashtable);
	**/
}


bool
ExecSteMHasPendingProbes(SteM * stem)
{
	return stem->stemstate->num_total_pending_probes != 0;
}

/**
 *	Set a new outer from the pending probe tuples. First, check if there are any pending probe
 *	tuples for the partition in memory. Otherwise, scan the partitions sequentially, and bring
 *	some (currently, sequentially first) partition with pending probe tuples into memory.
 *
 *	Set the new outer tuple using this pending probe tuple.
 */
void
ExecSteMSetNewOuterFromPendingProbes(SteM * stem)
{
	int			i;
	Partition  *partition = &stem->stemstate->partitioning.partitions[stem->stemstate->current_partition_in_memory];
	TupleTableSlot *slot;
	SteMHashJoinTuple shj;
	int			bucket_to_be_used = -1;

	Assert(stem->stemstate->sm_NeedNewOuter);

	for (i = partition->range_start; i <= partition->range_end; i++)
	{
		if (stem->stemstate->num_pending_probes_per_bucket[i] != 0)
		{
			/* There is a tuple there -- use it. */
			bucket_to_be_used = i;
			break;
		}
	}

	/** If no pending probes found for the partition currently in memory, find any bucket
	 *	with pending probes, and bring the corresponding partition into memory. */
	if (bucket_to_be_used == -1)
	{
		for (i = 0; i < stem->stemstate->stemhashtable->totalbuckets; i++)
		{
			if (stem->stemstate->num_pending_probes_per_bucket[i] != 0)
			{
				bucket_to_be_used = i;

				/* printSteMHashJoinTable(stem->stemstate->stemhashtable); */
				/* printBucket(stem->stemstate->stemhashtable, 306); */

				ExecSteMRemoveOldAndFetchNewPartition(stem, get_partition_number(stem->stemstate, i));

				/* printf("After switching partitions\n"); */
				/* printBucket(stem->stemstate->stemhashtable, 306); */
				/* printSteMHashJoinTable(stem->stemstate->stemhashtable); */
				break;
			}
		}
	}

	/** Eddy calling us was an indication that there were some pending probes. */
	Assert(bucket_to_be_used != -1);

	shj = stem->stemstate->pendingProbeTuples->buckets[bucket_to_be_used];
	Assert(shj != NULL);

	/** Update that hashtable etc */
	stem->stemstate->pendingProbeTuples->buckets[bucket_to_be_used] = shj->next;
	stem->stemstate->num_pending_probes_per_bucket[bucket_to_be_used]--;
	stem->stemstate->num_total_pending_probes--;

	Assert(stem->stemstate->sm_PendingOuterTupleSlot != NULL);

	/** We must find a tupleslot to use here, because that is what the rest of the code expects */
	slot = ExecStoreTuple((HeapTuple) &shj->htup,		/* tuple to store */
						  stem->stemstate->sm_PendingOuterTupleSlot,	/* slot */
						  InvalidBuffer,
						  false);		/* do not pfree this tuple -- XXX
										 * what does the rest of the code
										 * do with this flag ? */

	slot->ttc_isIntermediateTuple = true;

	stem->stemstate->sm_NeedNewOuter = false;
	stem->stemstate->sm_OuterTupleSlot = slot;

	stem->stemstate->sm_CurBucketNo = bucket_to_be_used;
	stem->stemstate->sm_CurTuple = NULL;

	/** XXX: Memory Leak. Can't delete the shj tuple. */
}


void
SteMPrintData(SteM * stem)
{
	SteMState  *stemstate = stem->stemstate;

	printf("SteM's counters : inserted = %d, probed = %d, returned = %d\n",
		   stemstate->sm_tuplesInserted,
		   stemstate->sm_tuplesProbed,
		   stemstate->sm_tuplesReturned);
	print_pending_tuples_information(stem->stemstate);

	/* printSteMHashJoinTable(stemstate->stemhashtable);  */
}

void
SteMPrintData_Debug_with_level(SteM * stem, int level)
{
	SteMState  *stemstate = stem->stemstate;

	elog(level, "	SteM's counters : inserted = %d, probed = %d, returned = %d",
		 stemstate->sm_tuplesInserted,
		 stemstate->sm_tuplesProbed,
		 stemstate->sm_tuplesReturned);

	/* printSteMHashJoinTable_debug(stemstate->stemhashtable);	*/
}

void
SteMPrintData_Debug(SteM * stem)
{
	SteMPrintData_Debug_with_level(stem, DEBUG1);
}

/**
 *	Helper function for below
 */
static SteMHashJoinTuple
SteMDemoteBucket(SteMHashJoinTuple bucket_start, int sourceNumber)
{
	int	num_tuples_in_bucket = 0;
	SteMHashJoinTuple current = bucket_start;
	SteMHashJoinTuple new_bucket_start, prev;
	SteMHJTupleHeap *heap;

	/** We need to know how many tuples there are in the bucket. */
	while (current)
	{
		num_tuples_in_bucket++;
		current = current->next;
	}

	heap = InitSteMHJTupleHeap(num_tuples_in_bucket);

	current = bucket_start;
	while (current)
	{
		/* first demote */
		int			j;
		IntermediateHeapTuple iht = (IntermediateHeapTuple) & current->htup;

		for (j = 0; j < __MAX_RELATIONS__; j++)
		{
			if ((j != sourceNumber) && (iht->t_base_tuples[j]))
			{
				/* Delete the tuple */
				pfree(iht->t_base_tuples[j]);
				iht->t_base_tuples[j] = NULL;
				BitArrayClearBit(iht->t_sources, j);
			}
		}

		/* insert into the heap */
		SteMHJTupleHeapInsert(heap, current);
		current = current->next;
	}

	/** That done, reconstruct the bucket by pulling tuples off the heap */
	prev = new_bucket_start = SteMHJTupleHeapRemoveSmallest(heap);

	while ((prev->next = SteMHJTupleHeapRemoveSmallest(heap)))
	{
		/* only add this tuple if not equal to the previous tuple */
		if (IHTCompare(&prev->htup, &prev->next->htup) != 0)
			prev = prev->next;
		else
		{
			/** XXX - Delete the tuple */
		}
	}

	return new_bucket_start;
}

/**
 *	Demote all tuples in the SteM to the base tuple level.
 */
void
ExecSteMDemoteAllTuples(SteM * stem)
{
	int			i;
	int			sourceNumber = stem->innerhashkey_var->varno - 1;
	SteMHashJoinTable shjt;

	/** We don't know what will happen if the SteM has a pending probe tuple */
	Assert(stem->stemstate->sm_NeedNewOuter == true);

	shjt = stem->stemstate->stemhashtable;

	for (i = 0; i < shjt->nbuckets; i++)
	{
		if (shjt->buckets[i] != NULL)
			shjt->buckets[i] = SteMDemoteBucket(shjt->buckets[i], sourceNumber);
	}
}

/** We are in promotion mode -- the get next will use the current_promoted to figure out
 *	which tuple to promote next. Promotion essentially means sending the tuple back to teh
 *	Eddy which will then route it elsewhere. */
TupleTableSlot *
ExecSteMPromoteGetNext(SteM * stem)
{
	SteMState  *stemstate = stem->stemstate;
	SteMHashJoinTable shjt = stem->stemstate->stemhashtable;

	TupleTableSlot *result;

	SteMHashJoinTuple old_current_promoted = stemstate->sm_current_promoted_tuple;
	SteMHashJoinTuple new_current_promoted = NULL;
	SteMHashJoinTuple before_current_promoted = stemstate->sm_tuple_before_current_promoted_tuple;
	int			current_bucket_no = stemstate->sm_current_promoted_tuple_bucket_no;

	if (old_current_promoted == NULL)
	{
		/* FIRST CALL -- Dont do anything. */
		Assert(current_bucket_no == -1);		/* sanity check */
	}
	else
	{
		if (before_current_promoted == NULL)
		{
			/*
			 * we don't really have a before tuple; this tuple was the
			 * first tuple in the bucket.
			 */
			/* this can happen if the promoted tuple never came back. */
			Assert(shjt->buckets[current_bucket_no] == old_current_promoted);
			shjt->buckets[current_bucket_no] = new_current_promoted = old_current_promoted->next;
		}
		else
		{
			Assert(before_current_promoted->next == old_current_promoted);
			before_current_promoted->next = new_current_promoted = old_current_promoted->next;
		}

		/* DELETE OLD_CURRENT_PROMOTED XXX */
	}

	/*
	 * Now, look for a new tuple to be promoted. Start with
	 * new_current_promoted. If that is non-null, we are done.
	 */
	if (new_current_promoted == NULL)
	{
		current_bucket_no++;
		while (current_bucket_no < shjt->nbuckets)
		{
			if (shjt->buckets[current_bucket_no] != NULL)
			{
				/** We are done. This is the next tuple to be promoted.
				 *	We could also do a test here to find out if this tuple should be promoted
				 *	or not. */

				new_current_promoted = shjt->buckets[current_bucket_no];

				stemstate->sm_tuple_before_current_promoted_tuple = NULL;

				stemstate->sm_current_promoted_tuple_bucket_no = current_bucket_no;

				break;
			}
			current_bucket_no++;
		}
	}

	stemstate->sm_current_promoted_tuple = new_current_promoted;

	if (new_current_promoted == NULL)
	{
		/* we are done */
		stemstate->sm_promotion_mode = false;
		return NULL;
	}
	else
	{
		IntermediateHeapTuple returnval = &new_current_promoted->htup;

		/* printf("Promoting this tuple :\n");
		debugitup(returnval); */

		result = ExecStoreTuple((HeapTuple) returnval,	/* tuple to store */
								stemstate->sm_HashTupleSlot,	/* slot */
								InvalidBuffer,
								false); /* do not pfree this tuple */
		return result;
	}
}

/**
 * Given an inner node, create the hash table by pulling all data out of it.
 * This borrows some of teh code from the Eddy.
 */
void
ExecSteMPopulateHashTable(SteM * stem)
{
	Node	   *innerNode = (Node *) stem->innerNode;
	TupleTableSlot *srcTupleSlot = 0;
	bool		isSeqScan = false;
	int			sourceID = -1;
	IntermediateHeapTupleData ihtd;
	IntermediateHeapTuple iht = &ihtd;
	ExprContext *econtext = stem->stemstate->cstate.cs_ExprContext;

	Assert(stem->stemstate->stemhashtable != NULL);

	if (IsA(innerNode, SeqScan))
	{
		isSeqScan = true;
		sourceID = ((SeqScan *) innerNode)->scanrelid - 1;
	}

	while (true)
	{
		srcTupleSlot = ExecProcNode((Plan *) innerNode, (Plan *) stem);

		if (TupIsNull(srcTupleSlot))
			break;

		if (isSeqScan)
		{
			/*
			 * In that case, the tuple has not been converted into the
			 * intermediate tuple format. Do so.
			 */
			initFromBaseTuple(iht, srcTupleSlot->val, srcTupleSlot->ttc_tupleDescriptor, sourceID);

			/* XXX we seem to have lost the original val here. XXX need to */
			/* free that in this */
			/* case, as soon as the tuple is inserted into some stem. */

			srcTupleSlot->val = (HeapTuple) iht;
			srcTupleSlot->ttc_isIntermediateTuple = true;
			srcTupleSlot->ttc_shouldFree = false;
		}

		/* debugitup(srcTupleSlot->val); */

		/** Insert. */
		econtext->ecxt_scantuple = srcTupleSlot;

		ExecSteMInsert(stem, econtext, true || isSeqScan);
	}
}

/** Currently we only allow comparing intermediate heap tuples that consist of exactly one
 *	base table tuple. XXX No error check here though, so be careful. */
int
IHTCompare(IntermediateHeapTuple iht1, IntermediateHeapTuple iht2)
{
	int			i;

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iht1->t_base_tuples[i])
		{

			Assert(iht2->t_base_tuples[i]);

			if (iht1->t_base_tuples[i]->t_len != iht2->t_base_tuples[i]->t_len)
				return (iht1->t_base_tuples[i]->t_len < iht2->t_base_tuples[i]->t_len) ? 1 : -1;
			else
				return memcmp(iht1->t_base_tuples[i]->t_data, iht2->t_base_tuples[i]->t_data, iht1->t_base_tuples[i]->t_len);
		}
	}
	Assert(0);
	return false;
}

/** The heap sort only really requires a smallerthan function, not a full-fledged compare. */
bool
SteMHJTupleSmallerThan(SteMHashJoinTuple shjt1, SteMHashJoinTuple shjt2)
{
	return IHTCompare(&shjt1->htup, &shjt2->htup) < 0;
}


/** Init a heap of intermediate heap tuples. Note, the number of entries must be known in advance. */
SteMHJTupleHeap *
InitSteMHJTupleHeap(int num_max)
{
	SteMHJTupleHeap *out = (SteMHJTupleHeap *) palloc(sizeof(SteMHJTupleHeap));

	out->shjt_tuples = (SteMHashJoinTuple *) palloc(sizeof(SteMHashJoinTuple) * (num_max + 1)); /* we won't use the
																								 * first entry */
	out->last_position = 0;
	out->max_tuples = num_max;
	return out;
}

/** Destroy the heap. */
void
DestroySteMHJTupleHeap(SteMHJTupleHeap * heap)
{
	pfree(heap->shjt_tuples);
	pfree(heap);
}

/**
 *	The usual heap insert algorithm. Insert at the end, start moving up till it hits something
 *	smaller than itself.
 */
void
SteMHJTupleHeapInsert(SteMHJTupleHeap * heap, SteMHashJoinTuple shjt)
{
	int			current;

	/* add the element to the end of the array */
	++heap->last_position;
	Assert(1 <= heap->last_position && heap->last_position <= heap->max_tuples);
	heap->shjt_tuples[heap->last_position] = shjt;

	current = heap->last_position;

	while (current != 1)
	{
		/* compare; if smaller move up */
		if (SteMHJTupleSmallerThan(heap->shjt_tuples[current], heap->shjt_tuples[current / 2]))
		{
			/* swap */
			SteMHashJoinTuple temp = heap->shjt_tuples[current];

			Assert(1 <= current && current <= heap->max_tuples);
			heap->shjt_tuples[current] = heap->shjt_tuples[current / 2];
			Assert(1 <= current / 2 && current / 2 <= heap->max_tuples);
			heap->shjt_tuples[current / 2] = temp;
			current /= 2;
		}
		else
			break;
	}
	return;
}

/*
static bool
heap_is_valid(SteMHJTupleHeap *heap, int sourceNumber)
{
	int i;

	for(i = 1; i < heap->last_position; i++)
	{
		if(heap->shjt_tuples[i]->htup.t_base_tuples[sourceNumber] == NULL)
			return false;
	}

	return true;
}
*/

/**
 *	Again, the standard stuff. Delete hte first entry. Move the last entry to the beginning,
 *	and move it down till hits something larger. */
SteMHashJoinTuple
SteMHJTupleHeapRemoveSmallest(SteMHJTupleHeap * heap)
{
	int			current;

	SteMHashJoinTuple out = heap->shjt_tuples[1];

	if (heap->last_position == 0)
		return NULL;

	/* move the last to the beginning */
	heap->shjt_tuples[1] = heap->shjt_tuples[heap->last_position--];

	/* move down */
	current = 1;
	do
	{
		int			swap_position;

		if (2 * current > heap->last_position)
			break;
		if (2 * current == heap->last_position)
			swap_position = 2 * current;
		else
		{
			if (SteMHJTupleSmallerThan(heap->shjt_tuples[2 * current], heap->shjt_tuples[2 * current + 1]))
				swap_position = 2 * current;
			else
				swap_position = 2 * current + 1;
		}


		if (SteMHJTupleSmallerThan(heap->shjt_tuples[swap_position], heap->shjt_tuples[current]))
		{
			/* swap with that one. */
			SteMHashJoinTuple temp = heap->shjt_tuples[current];

			Assert(1 <= current && current <= heap->max_tuples);
			heap->shjt_tuples[current] = heap->shjt_tuples[swap_position];
			Assert(1 <= swap_position && swap_position <= heap->max_tuples);
			heap->shjt_tuples[swap_position] = temp;
			current = swap_position;
		}
		else
			break;
	} while (1);

	return out;
}

void
printSteMHashJoinTable_debug(SteMHashJoinTable shjt)
{
	int			i;
	SteMHashJoinTuple current;

	elog(DEBUG1, "+++++++++++++++++++++ Printing SteM Hash Join Table BEGIN ++++++++++++++++++++++\n");

	for (i = 0; i < shjt->nbuckets; i++)
	{
		current = shjt->buckets[i];
		if (current)
			elog(DEBUG1, "Bucket No. %d\n", i);
		while (current)
		{
			debugitup_debug((IntermediateHeapTuple) & current->htup);

			current = current->next;
		}
	}

	elog(DEBUG1, "--------------------- Printing SteM Hash Join Table END ----------------------\n");
}

static void
printBucket(SteMHashJoinTable shjt, int bucket_no, TupleDesc *base_tuple_descs)
{
	SteMHashJoinTuple current = shjt->buckets[bucket_no];

	if (current)
		printf("Bucket No. %d\n", bucket_no);
	while (current)
	{
		debugitup_base_tuples_descs((IntermediateHeapTuple) & current->htup, base_tuple_descs);

		current = current->next;
	}
}

void
printSteMHashJoinTable(SteMHashJoinTable shjt, TupleDesc *base_tuple_descs)
{
	int			i;

	printf("BEGIN PRINTING STEM HASH JOIN TABLE\n");

	for (i = 0; i < shjt->nbuckets; i++)
		printBucket(shjt, i, base_tuple_descs);

	printf("DONE PRINTING STEM HASH JOIN TABLE\n");
}


/* FUNCTION SteMGetOldestTuple
 * ARGUMENTS: <node> points to a STeM node.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Returns a pointer to the oldest tuple in the SteM, or NULL
 * of the SteM contains no tuples.
 */
IntermediateHeapTuple *
SteMGetOldestTuple(SteM * node)
{
	return &(node->stemstate->oldest_tuple);
}


/* FUNCTION SteMGetNewestTuple
 * ARGUMENTS: <node> points to a STeM node.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Returns a pointer to the newest tuple in the SteM, or NULL
 * of the SteM contains no tuples.
 */
IntermediateHeapTuple *
SteMGetNewestTuple(SteM * node)
{
	return &(node->stemstate->newest_tuple);
}

/* FUNCTION SteMGetContainsRelation
 * ARGUMENTS: <cur> points to an Eddy, <node> points to a SteM node,
 *		and <windowrteidx> points to the range table entry for the relation
 *		referenced by the window.
 * PRECONDITIONS: <node> is initialized, and <windowrteidx> corresponds to a valid
 *		relation.  (IMPORTANT) <cur> points to an eddy that has already had its
 *		rangetable member updated by adjust_scan_and_rt().
 * POSTCONDITIONS: Returns <true> if <node> will contain tuples from <rel>.
 */
bool
SteMGetContainsRelation(Eddy * cur, SteM * node, int windowrteidx)
{
	int			source_id = -1;


	Assert(NULL != cur);
	Assert(NULL != node);

/*	aging stuff is not working right now... returning false from this function
	effectivly turns it off. */

	/*
	 * if(1) return false;
	 */

	source_id = STEM_SOURCE(node);

	return equal(rt_fetch(source_id + 1, cur->rangetable),
				 rt_fetch(windowrteidx, cur->rangetable));


}

/* ----------------------------------------------------------------
 *		ExecSteMSaveSteMHashTuple
 *
 *		save a stemhashjoin tuple to a tmp file.
 *
 * ----------------------------------------------------------------
 */
void
ExecSteMSaveSteMHashTuple(SteMHashJoinTuple stemHashTuple,
						  BufFile *file)
{
	int			i;
	size_t		written;

	written = BufFileWrite(file, (void *) stemHashTuple, sizeof(SteMHashJoinTupleData));
	if (written != sizeof(SteMHashJoinTupleData))
		elog(ERROR, "Write to hashjoin temp file failed");

	/* Now, write the baseheap tuples */
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (stemHashTuple->htup.t_base_tuples[i] != NULL)
			ExecHashJoinSaveTuple(stemHashTuple->htup.t_base_tuples[i], file);
	}
}

/* ----------------------------------------------------------------
 *		ExecSteMSaveSteMHashTupleGivenIntermediateTuple
 *
 *		save a stemhashjoin tuple to a tmp file, but we are given a pointer
 *		to the intermediate tuple, as well as the bucket no, but not the
 *		SteMHashJoinTuple
 *
 * ----------------------------------------------------------------
 */
void
ExecSteMSaveSteMHashTupleGivenIntermediateTuple(IntermediateHeapTuple htup,
											 int bucketno, BufFile *file)
{
	SteMHashJoinTupleData stemHashTuple;

	stemHashTuple.bucket_no = bucketno;
	memcpy((char *) &stemHashTuple.htup,
		   (char *) htup,
		   sizeof(IntermediateHeapTupleData));

	ExecSteMSaveSteMHashTuple(&stemHashTuple, file);
}

/* ----------------------------------------------------------------
 *		ExecSteMGetSavedSteMHashTuple
 *
 *		read the next stem hash join tuple from a tmp file
 *
 *		Slightly different from ExecHashJoinGetSavedTuple
 *		returns a stemhashtuple tuple instead of a tupletableslot
 * ----------------------------------------------------------------
 */

SteMHashJoinTuple
ExecSteMGetSavedSteMHashTuple(BufFile *file,
						  MemoryContext iContext, MemoryContext bContext)
{
	SteMHashJoinTuple stemTuple;
	size_t		nread;
	MemoryContext oldcxt = MemoryContextSwitchTo(iContext);
	int			i;

	stemTuple = palloc(sizeof(SteMHashJoinTupleData));
	stemTuple->htup.t_datamcxt = iContext;

	nread = BufFileRead(file, (void *) stemTuple, sizeof(SteMHashJoinTupleData));

	if (nread == 0)
	{
		pfree(stemTuple);
		stemTuple = NULL;		/* end of file */
	}
	else
	{
		if (nread != sizeof(SteMHashJoinTupleData))
			elog(ERROR, "Read from hashjoin temp file failed");

		/* Read each of the base tuples */
		for (i = 0; i < __MAX_RELATIONS__; i++)
		{
			if (stemTuple->htup.t_base_tuples[i] != NULL)
				stemTuple->htup.t_base_tuples[i] = ExecHashJoinGetSavedBaseTuple(file, bContext);
		}
	}

	MemoryContextSwitchTo(oldcxt);

	return stemTuple;
}


/* ----------------------------------------------------------------
 *		ExecHashJoinSaveIntermediateTuple
 *
 *		save a intermediate tuple to a tmp file.
 *
 *		Along the lines of ExecHashJoinSaveTuple.
 *
 * ----------------------------------------------------------------
 */
void
ExecHashJoinSaveIntermediateTuple(IntermediateHeapTuple iheapTuple,
								  BufFile *file)
{
	int			i;
	size_t		written;

	Assert(0);

	written = BufFileWrite(file, (void *) iheapTuple, sizeof(IntermediateHeapTupleData));
	if (written != sizeof(IntermediateHeapTupleData))
		elog(ERROR, "Write to hashjoin temp file failed");

	/* Now, write the base heap tuples */
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iheapTuple->t_base_tuples[i] != NULL)
			ExecHashJoinSaveTuple(iheapTuple->t_base_tuples[i], file);
	}
}

/* ----------------------------------------------------------------
 *		ExecHashJoinGetSavedInteremediateTuple
 *
 *		read the next interemediate tuple from a tmp file
 *
 *		Slightly different from ExecHashJoinGetSavedTuple
 *		returns an intermediate tuple instead of a tupletableslot
 * ----------------------------------------------------------------
 */

IntermediateHeapTuple
ExecHashJoinGetSavedIntermediateTuple(BufFile *file,
						  MemoryContext iContext, MemoryContext bContext)
{
	IntermediateHeapTuple iheapTuple;
	size_t		nread;
	MemoryContext oldcxt = MemoryContextSwitchTo(iContext);
	int			i;

	Assert(0);

	iheapTuple = palloc(sizeof(IntermediateHeapTupleData));
	iheapTuple->t_datamcxt = iContext;

	nread = BufFileRead(file, (void *) iheapTuple, sizeof(IntermediateHeapTupleData));

	if (nread == 0)
		return NULL;			/* end of file */

	if (nread != sizeof(IntermediateHeapTupleData))
		elog(ERROR, "Read from hashjoin temp file failed");

	/* Read each of the base tuples */
	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (iheapTuple->t_base_tuples[i] != NULL)
			iheapTuple->t_base_tuples[i] = ExecHashJoinGetSavedBaseTuple(file, bContext);
	}

	MemoryContextSwitchTo(oldcxt);

	return iheapTuple;
}

/* ----------------------------------------------------------------
 *		ExecHashJoinGetSavedBaseTuple
 *
 *		read the next tuple from a tmp file
 *
 *		Based on, but slightly different from equivalent function
 *		in nodeHashjoin.c
 * ----------------------------------------------------------------
 */

static HeapTuple
ExecHashJoinGetSavedBaseTuple(BufFile *file, MemoryContext bContext)
{
	MemoryContext oldcxt = MemoryContextSwitchTo(bContext);

	HeapTupleData htup;
	size_t		nread;
	HeapTuple	heapTuple;

	nread = BufFileRead(file, (void *) &htup, sizeof(HeapTupleData));
	if (nread == 0)
		return NULL;			/* end of file */
	if (nread != sizeof(HeapTupleData))
		elog(ERROR, "Read from hashjoin temp file failed");
	heapTuple = palloc(HEAPTUPLESIZE + htup.t_len);
	memcpy((char *) heapTuple, (char *) &htup, sizeof(HeapTupleData));
	heapTuple->t_datamcxt = CurrentMemoryContext;
	heapTuple->t_data = (HeapTupleHeader)
		((char *) heapTuple + HEAPTUPLESIZE);
	nread = BufFileRead(file, (void *) heapTuple->t_data, htup.t_len);
	if (nread != (size_t) htup.t_len)
		elog(ERROR, "Read from hashjoin temp file failed");

	MemoryContextSwitchTo(oldcxt);
	return heapTuple;
}

/** Move partition to disk file. */
void
ExecSteMRemovePartition(SteMState * stemstate, int partition_no)
{
	int			i;
	int			last_timestamp_written = 0;
	Partition  *partition = &stemstate->partitioning.partitions[partition_no];

	/** It is possible that there is no corresponding partition file for this partition */
	if (partition->file == NULL)
	{
		partition->file = BufFileCreateTemp();
		partition->last_timestamp_written = 0;
	}

	/** All *NEW* tuples must be written, but since we are scanning in bucket order,
	 *	and not timestamp order, we have to do some funny things */
	last_timestamp_written = partition->last_timestamp_written;

	for (i = partition->range_start; i <= partition->range_end; i++)
	{
		BufFile    *file = get_partition_file(stemstate, i);
		SteMHashJoinTuple shj_tuple = stemstate->stemhashtable->buckets[i];

		while (shj_tuple != NULL)
		{
			if (last_timestamp_written < shj_tuple->htup.t_timestamp)
				ExecSteMSaveSteMHashTuple(shj_tuple, file);

			if (partition->last_timestamp_written < shj_tuple->htup.t_timestamp)
				partition->last_timestamp_written = shj_tuple->htup.t_timestamp;

			stemstate->stemhashtable->buckets[i] = shj_tuple->next;

			delete_stemhashjointuple(shj_tuple);

			shj_tuple = stemstate->stemhashtable->buckets[i];
		}
	}
}

/** Read a partition from disk */
void
ExecSteMFetchPartition(SteM * stem, int new_partition_no)
{
	int			i;
	Partition  *partition = &stem->stemstate->partitioning.partitions[new_partition_no];
	SteMHashJoinTuple stemHashTuple;
	MemoryContext iContext = stem->stemstate->stemhashtable->itupleContext;
	MemoryContext bContext = stem->stemstate->stemhashtable->btupleContext;

	/** Random sanity check */
	for (i = partition->range_start; i <= partition->range_end; i++)
		Assert(stem->stemstate->stemhashtable->buckets[i] == NULL);

	/** Read from the file one by one and insert into the hashtable */
	if (BufFileSeek(partition->file, 0, 0L, SEEK_SET))
		elog(ERROR, "Failed to rewind hash temp file");

	while ((stemHashTuple = ExecSteMGetSavedSteMHashTuple(partition->file, iContext, bContext)) != NULL)
	{
		stemHashTuple->next = stem->stemstate->stemhashtable->buckets[stemHashTuple->bucket_no];
		stem->stemstate->stemhashtable->buckets[stemHashTuple->bucket_no] = stemHashTuple;
	}
}

/**
 *	Move the current partition to disk and bring back a new partition into
 *	memory.
 */
void
ExecSteMRemoveOldAndFetchNewPartition(SteM * stem, int new_partition_no)
{
	/* Remove the old partition */
	ExecSteMRemovePartition(stem->stemstate, stem->stemstate->current_partition_in_memory);

	stem->stemstate->current_partition_in_memory = new_partition_no;

	/* Fetch the new partition in memory */
	ExecSteMFetchPartition(stem, new_partition_no);
}

/**
 *	Partitioning information and the associated functions.
 *
 *	These are critical functions for disk based shj.
 *	Takes in the SteMState as input, and changes the partitioning information
 *	inside, and creates the required files etc.
 *	Hopefully, all this functionality can be localized here, and we can use
 *	other methods.
 *
 */
bool
repartitioning_required(SteMState * stemstate)
{
    return false;
	return (stemstate->partitioning.num_partitions < __MAX_PATITIONS_ALLOWED__) && ((stemstate->sm_tuplesInserted & 0xFFFF) == 0xFFFF);
}

/* Take the current partition in memory... cut off from the end depending upon the
 * number of partitions. Assume that all the buckets are equally filled, so we can
 * do partitioning on the buckets directly. */
void
repartition(SteMState * stemstate)
{
	int			i;


	int			in_mem = stemstate->current_partition_in_memory;

	int			partition_size = stemstate->stemhashtable->totalbuckets / __MAX_PATITIONS_ALLOWED__;

	/* printSteMHashJoinTable(stemstate->stemhashtable); */

	Partition  *partition_in_mem = &stemstate->partitioning.partitions[in_mem];
	Partition  *new_partition = &stemstate->partitioning.partitions[stemstate->partitioning.num_partitions];

	new_partition->range_end = partition_in_mem->range_end;
	new_partition->range_start = new_partition->range_end - partition_size;
	new_partition->last_timestamp_written = 0;

	partition_in_mem->range_end = new_partition->range_start - 1;


	/* Change the partition_numbers structure as well */
	for (i = new_partition->range_start; i <= new_partition->range_end; i++)
		stemstate->partitioning.partition_numbers[i] = stemstate->partitioning.num_partitions;

	/* Buf files - must create one for the new_partition */
	new_partition->file = BufFileCreateTemp();

	/*
	 * If the partition_in_mem was already being pushed to the disk, must
	 * do something about it. Basically, the tuples on disk must be
	 * repartitioned and that requires more work which is why it probably
	 * never makes sense to push to disk if possibility of repartitioning.
	 * XXX
	 */
	if (partition_in_mem->file != NULL)
		Assert(!"Not implemented yet");

	ExecSteMRemovePartition(stemstate, stemstate->partitioning.num_partitions);

	stemstate->partitioning.num_partitions++;

	/* printSteMHashJoinTable(stemstate->stemhashtable); */
}

void
init_partitioning_information(SteMState * stemstate)
{
	int			i;

	stemstate->partitioning.partition_numbers = (int *) palloc(sizeof(int) * stemstate->stemhashtable->totalbuckets);
	stemstate->num_pending_probes_per_bucket = (int *) palloc(sizeof(int) * stemstate->stemhashtable->totalbuckets);
	for (i = 0; i < stemstate->stemhashtable->totalbuckets; i++)
	{
		stemstate->partitioning.partition_numbers[i] = 0;
		stemstate->num_pending_probes_per_bucket[i] = 0;
	}
	stemstate->partitioning.num_partitions = 1;
	stemstate->partitioning.partitions[0].range_start = 0;
	stemstate->partitioning.partitions[0].range_end = stemstate->stemhashtable->totalbuckets - 1;
	stemstate->partitioning.partitions[0].file = NULL;
	stemstate->partitioning.partitions[0].last_timestamp_written = 0;
}

inline int
get_partition_number(SteMState * stemstate, int bucketno)
{
	Assert(bucketno < stemstate->stemhashtable->totalbuckets);
	return stemstate->partitioning.partition_numbers[bucketno];
}

inline bool
bucket_in_memory(SteMState * stemstate, int bucketno)
{
	return stemstate->current_partition_in_memory == get_partition_number(stemstate, bucketno);
}

inline BufFile *
get_partition_file(SteMState * stemstate, int bucketno)
{
	return stemstate->partitioning.partitions[get_partition_number(stemstate, bucketno)].file;
}

void
print_partitioning_information(SteMState * stemstate)
{
	int			i;

	printf("******* Printing partitioning information ***\n");
	printf("Number of partitions = %d, Current partition in Memory = %d\n", stemstate->partitioning.num_partitions,
		   stemstate->current_partition_in_memory);
	for (i = 0; i < stemstate->partitioning.num_partitions; i++)
		printf("Partition No. %d : Start = %d, End = %d\n", i,
			   stemstate->partitioning.partitions[i].range_start,
			   stemstate->partitioning.partitions[i].range_end);
}

void
print_pending_tuples_information(SteMState * stemstate)
{
	int			i;

	printf("Total Pending probes = %d\n", stemstate->num_total_pending_probes);
	for (i = 0; i < stemstate->stemhashtable->totalbuckets; i++)
		if (stemstate->num_pending_probes_per_bucket[i])
			printf("Bucket No. %d : %d\n", i, stemstate->num_pending_probes_per_bucket[i]);
}
bool stem_contains_tuple(SteM * stem, ExprContext *econtext)
{
	int bucketno=0;
	SteMHashJoinTable hashtable = stem->stemstate->stemhashtable;
	SteMHashJoinTuple tuple=NULL;
	IntermediateHeapTuple iht = (IntermediateHeapTuple)econtext->ecxt_scantuple->val;


	bucketno = ExecSteMHashGetBucket(stem, 
									 hashtable,
									 econtext, 
									 stem->innerhashkey);
	if (!bucket_in_memory(stem->stemstate, bucketno))
	{
		elog(ERROR, "unexpected condition");
	}
	tuple = hashtable->buckets[bucketno];
	for(;tuple != NULL; tuple=tuple->next)
		{
			if(IHTCompare(iht, &(tuple->htup)) == 0)
				return true;
		}
	return false;

}
