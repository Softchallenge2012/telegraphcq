/*-------------------------------------------------------------------------
 *
 * nodeFSteM.c
 *	  New SteM operator that respects Fjord interface
 *
 * This operator uses the new Fjord "ProcessNode" interface as opposed to the
 * standard Iterator interface.
 *
 * The current version of FSteM is basically a wrapper around nodeSteM.
 * Developers should try to keep low-level code inside nodeSteM instead of
 * adding it to this file.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFSteM.c,v 1.51 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/pg_type.h"
#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFSteM.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeSteM.h"
#include "executor/nodeEddy.h"
#include "optimizer/eddy.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "parser/parse_node.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "optimizer/clauses.h"
#include "optimizer/var.h"
#include "telegraphcq/punct.h"
#include "telegraphcq/triageq.h"

/* #define AMOLDEBUGON  1 */

/* #define TCQDEBUGON 1 */

/* #define DEBUG_FSTEM 1 */

bool can_be_applied(Node * expr, IntermediateHeapTuple iht);
bool FSteMSetOuterTupleNext(FSteM * node, ExprContext * econtext);
char * getVarInformation(Eddy *node, Var *var);

/* Function to simplify selecting a substem. */

SteM ** FSTEM_SUBSTEM_PTR_PTR(FSteM * node, int index);
SteM ** FSTEM_SUBSTEM_PTR_PTR(FSteM * node, int index) 
{
	Assert(index >= 0);
	Assert(index < FSTEM_NUM_SUBSTEMS);
	return &((node)->substems[index].stem);
}

#define FSTEM_SUBSTEM_PTR(node, index) \
	 (*(FSTEM_SUBSTEM_PTR_PTR((node),(index))))


/* Macro for iterating over the active subSTeMs. */
/*#define FSTEM_FOREACH_SUBSTEM(node, cur_index) \*/
	/*for (cur_index = (node)->first_ss_index; \*/
		/*cur_index != FSTEM_NEXT_SS_INDEX((node)->insert_ss_index); \*/
		/*cur_index = FSTEM_NEXT_SS_INDEX(cur_index)) */


/* Prototypes for internal functions. */
bool FSteMIsInitialized(FSteM * node);
bool FSteMProcessInputTuple(FSteM * node, Plan *parent);
bool FSteMGenerateOutputTuple(FSteM * node, Plan *parent);
void FSteMReplaceWindow(Const * new_window_size,
						Const ** old_window_size_ptr);
void FSteMCreateEmptyWindowExprs(FSteM * node, Const * dummy_window_size);
void FSteMGCSubstems(FSteM * node, ExprContext * next_insert_tup);
void FSteMInsert(FSteM * node, ExprContext * econtext);
void FSteMMakeSpaceForInsert(FSteM * node, ExprContext * nexttuple);
TupleTableSlot *FSteMGetNextOutput(FSteM * node);
void FSteMSetNewOuter(FSteM * node, ExprContext * newcontext);
bool FSteMSubSteMHasExpired(FSteM * node, int index,
							ExprContext * newest_tup);
bool FSteMTupleFitsInSubSteM(FSteM * node, int index, ExprContext * tup);
bool FSteMEvalWindowExpr(FSteM * node, IntermediateHeapTuple * older_tuple,
						 IntermediateHeapTuple * newer_tuple,
						 Expr * window_expr);
void FSteMCreateNewSubstem(FSteM * node, int index);
bool FSteMNewWindowIsLarger(FSteM * node, Const * new_window_size);
void FSteMPrintData_debug(Eddy *eddy, FSteM * node, int stem_no);

static bool FSteMHasSubSteMs(FSteM *node);

/* ----------------------------------------------------------------
 *		ProcessInitFSteM
 * ----------------------------------------------------------------
 */
bool
ProcessInitFSteM(FSteM * node, EState * estate, Plan * parent)
{
	int i;

	int source;
	Eddy *eddy = (Eddy*)parent;
	

	Assert(node != NULL);
	Assert(IsA(eddy,Eddy));
		   
	FjordInitialize(&(node->fjord), estate);
	/* Common Fjord initialization */

	node->haveprobe = false;
	node->probehasresults=false;
	node->doneprobe = false;

	/* Prepare the SteM that we will copy to create the SteMs in
	 * node->substems[]. */
	ExecInitSteM(node->protostem, estate, (Plan *) node);
	
	/* FSTEM_NUM_SUBSTEMS is defined in plannodes.h. */
	for (i = 0; i < FSTEM_NUM_SUBSTEMS; i++) {
		FSTEM_SUBSTEM_PTR(node, i) = NULL;
	}

	node->unqueued_output = NULL;
		
	node->insert_ss_index = FSTEM_NOT_AN_INDEX;
	node->probe_ss_index = FSTEM_NOT_AN_INDEX;
	node->first_ss_index = FSTEM_NOT_AN_INDEX;

	node->max_work_units = FSTEM_MAX_WORK_UNITS;

	node->window_expr_args = NULL;
	node->ts_comp_expr = NULL;


	if (! (UseStaticPlan && (StaticEddy != NULL))) 
	{
		node->ts_accessor = NULL;
		node->same_stem_expr = NULL;
		node->window_expr_is_valid = false;
	}

	source=STEM_SOURCE(((SteM*)node->protostem));
	node->source=(ScanModule*)eddy->sources[source];
	node->sourcehasbindings=node->source->sourcehasvarbindings;	
	node->initialized = true;

	node->last_build_ts = 0;
	node->build_td = eddy->base_tuple_descs[source];
	node->punct = NULL;
	node->punctjoin = false;
	node->punctprobe = false;

	node->total_probed    = 0; 
	node->total_inserted  = 0;
	node->total_returned  = 0;

	node->fstem_num_built 			  = 0;
	node->fstem_num_input_for_probes  = 0;
	node->fstem_num_early_eliminated  = 0;
	node->fstem_num_probe_results 	  = 0;
	node->fstem_num_probe_bouncebacks = 0;
	node->fstem_num_substems_ended 	  = 0;
	
	FSteMMakeSpaceForInsert(node,node->fjord.scanstate->cstate.cs_ExprContext);

	return TRUE;
}

bool
ProcessEndFSteM(FSteM * node, Plan * parent)
{
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	int i;
	int sourceID;
	/*bool deqsucc;*/
	
    FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	sourceID = STEM_SOURCE(node->protostem);

	Assert(node != NULL);
	Assert(true == node->initialized);

	for (i = 0; i < FSTEM_NUM_SUBSTEMS; i++) {
		if (NULL != FSTEM_SUBSTEM_PTR(node, i)) {
			ExecEndSteM(FSTEM_SUBSTEM_PTR(node, i));
			pfree(FSTEM_SUBSTEM_PTR(node, i));
			FSTEM_SUBSTEM_PTR(node, i) = NULL;

		}
	}

	/* TODO: Destroy the operator trees attached to this FSteM. */

	ExecEndSteM(node->protostem);
	pfree(node->protostem);

	if (node->ts_accessor) {
		pfree(node->ts_accessor);
		node->ts_accessor = NULL;
	}

	if (node->same_stem_expr) {
		pfree(node->same_stem_expr);
		node->same_stem_expr = NULL;
	}

	if (node->stem_expired_expr) {
		pfree(node->stem_expired_expr);
		node->stem_expired_expr = NULL;
	}

	if (node->window_expr_args) {
		MemoryContextDelete(node->window_expr_args->ecxt_per_tuple_memory);
		pfree(node->window_expr_args);
		node->window_expr_args = NULL;
	}

	FjordEnd(&node->fjord,eddy);
	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(FSteM));

	/*
	 *  Clear the sourceStems bit 
	 */ 
	BitArrayClearBit(eddy->eddystate->sourceStems[sourceID],operator);
	
	return TRUE;
}

bool
ProcessRemoveQueryFSteM(FSteM * node, int32 queryid, Plan * parent)
{
	List *iter1, *iter2 = NIL, *iter3 = NIL;
	List *fstemclauses_list, *interested_queries;


	Assert(node != NULL);
	Assert(true == node->initialized);

	/** We should also remove the outerhashkeys, and fstemclauses corresponding
	 * to this query */

	foreach(iter1, node->fstemouterhashkeys) 
	{
		List *fstemclauses_list =
		((FSteMOuterHashkey *) lfirst(iter1))->fstemclauses_list;

		foreach(iter2, fstemclauses_list) 
		{
			List *interested_queries =
			((FSteMClauses *) lfirst(iter2))->interested_queries;

			foreach(iter3, interested_queries) 
			{
				if (lfirsti(iter3) == queryid) 
				{
					goto foundmatch;
				}
			}
		}
	}

	return false;
	

	foundmatch:
#ifdef AMOLDEBUGON
	elog(DEBUG1, "Found the stem clauses with the query id");
	elog(DEBUG1,
		 nodeToString(((FSteMOuterHashkey *) lfirst(iter1))->outerhashkey));
#endif

	fstemclauses_list = ((FSteMOuterHashkey *)lfirst(iter1))->fstemclauses_list;
	interested_queries = ((FSteMClauses *) lfirst(iter2))->interested_queries;

#ifdef AMOLDEBUGON
	elog(DEBUG1, "interested_queries_length = %d, fstemclauses length = %d",
		 length(interested_queries), length(fstemclauses_list));
#endif
	
	if (length(interested_queries) == 1) {
		if (length(fstemclauses_list) == 1) {
			/* remove the fstemouterhashkey element completely */
			if (node->currentouterhashkey == lfirst(iter1)) {
				node->currentouterhashkey = lnext(iter1);
				node->currentfstemclauses =
				((FSteMOuterHashkey *) lnext(iter1))->fstemclauses_list;
			}
			pfree(lfirst(iter1));	/* broken ... */
			node->fstemouterhashkeys = lremove(lfirst(iter1), node->fstemouterhashkeys);
#ifdef AMOLDEBUGON
			elog(DEBUG1, "removing the outerhashkey structure completely");
#endif
		} else {
			/* only remove the fstemclauses from the list */
			if (node->currentfstemclauses == iter2) {
				node->currentfstemclauses = lnext(iter2);
			}
			pfree(lfirst(iter2));	/* broken... */
			fstemclauses_list = lremove(lfirst(iter2), fstemclauses_list);
			((FSteMOuterHashkey *) lfirst(iter1))->fstemclauses_list =
			fstemclauses_list;
#ifdef AMOLDEBUGON
			elog(DEBUG1, "removing the fstemclauses");
#endif
		}
	} else {
		/* only delete the query id from the list of interested queries */
		Assert(lfirsti(iter3) == queryid);
		interested_queries = lremovei(lfirsti(iter3), interested_queries);
		((FSteMClauses *) lfirst(iter2))->interested_queries =
		interested_queries;
#ifdef AMOLDEBUGON
		elog(DEBUG1, "removing just the query id %d",length(interested_queries));
#endif
	}

#ifdef AMOLDEBUGON
	elog(DEBUG1, "Done with ProcessRemoveQueryFSteM.\n");
#endif

	return FjordRemoveQuery((Fjord *)node, queryid);
}

bool
can_be_applied(Node * expr, IntermediateHeapTuple iht)
{

	/** expr's var must not be set to null in iht */
	List *vars = pull_var_clause(expr, false);
	List *iter;
	bool ret = true;


	foreach(iter, vars) {
		int index = ((Var *) lfirst(iter))->varno - 1;
		if (iht->t_base_tuples[index] == NULL) {
			ret = false;
			break;

		}

	}
	freeList(vars);
	return ret;
}

/** This function, which iterates over the outerhashkeys and fstemclauses, 
 * is being written as a recursive function for the moment being. */
bool
FSteMSetOuterTupleNext(FSteM * node, ExprContext * econtext)
{
	Node *outerhashkey;
	List *stemclauses;

#ifdef AMOLDEBUGON
	elog(DEBUG1,"FSteMSetOuterTupleNext ..");
#endif
	
	if (node->currentouterhashkey == NIL) 
	{
		/* this means that this is the first invocation */
		node->currentouterhashkey = node->fstemouterhashkeys;
		node->currentfstemclauses =
			((FSteMOuterHashkey *) lfirst(node->currentouterhashkey))->
			fstemclauses_list;
	}
	else 
	{
		/*
		 * We are actually iterating inside either the outerhashkeys or the
		 * fstemclauses within it
		 */

		node->currentfstemclauses = lnext(node->currentfstemclauses);

		if (node->currentfstemclauses == NIL) 
		{
			node->currentouterhashkey = lnext(node->currentouterhashkey);

			if (node->currentouterhashkey == NIL) 
			{
				return false;	/* We are _really_ done with this tuple. */
			}
			else 
			{
				node->currentfstemclauses =
					((FSteMOuterHashkey *)
					 lfirst(node->currentouterhashkey))->fstemclauses_list;

				/* THIS CAN NOT BE NIL */
				Assert(node->currentfstemclauses != NIL);
			}
		}
	}

	outerhashkey =
		((FSteMOuterHashkey *) lfirst(node->currentouterhashkey))->
		outerhashkey;

	stemclauses = 
		((FSteMClauses *) lfirst(node->currentfstemclauses))->
	    stemclauses;

	/*
	 * Check if the currentouterhashkey can actually be applied to this outer
	 * tuple
	 */
	
	if (can_be_applied
		(outerhashkey,
		 (IntermediateHeapTuple) econtext->ecxt_scantuple->val)) {

		/*
		 * WARNING !! This is not good. We are setting variables inside the
		 * SteM. This shouldn't create any problems right now, but it might
		 * later.
		 */

		int current_index;

		if (FSteMHasSubSteMs(node)) 
		{
			/* Set the hashkey and stemclauses for all the subSteMs.
			 * Recall the the range of subSteMs that is currently being 
			 * used runs from (node->first_ss_index) to 
			 * (node->insert_ss_index), inclusive. */
			current_index = node->first_ss_index;
			do
			{
				SteM *curstem = FSTEM_SUBSTEM_PTR(node, current_index);
				current_index = FSTEM_NEXT_SS_INDEX(current_index);

				curstem->outerhashkey = outerhashkey;
				curstem->stemclauses = stemclauses;
			} while (current_index != 
					FSTEM_NEXT_SS_INDEX(node->insert_ss_index));
		}

		/* 
		 * elog(DEBUG, "USING THIS SET OF KEYS FOR THIS OUTER TUPLE");
		 * elog(DEBUG, nodeToString(node->stem->outerhashkey)); elog(DEBUG,
		 * nodeToString(node->stem->stemclauses)); */

		FSteMSetNewOuter(node, econtext);

		return true;
	} else {
		/* iterate/recurse */
		return FSteMSetOuterTupleNext(node, econtext);
	}
}

/* -----------------------------------------------------------------------------
 *		ProcessNextFSteM
 * -----------------------------------------------------------------------------
 */
void
ProcessNextFSteM(FSteM * node, Plan * parent)
{
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	bool 		no_inputs 	  = true;
	bool        results_exist = false;
	bool 		enqsucc 	  = false;
	int 		i;

    FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */

#ifdef AMOLDEBUGON
	elog(DEBUG1,"------------------------------------------------------------");
	elog(DEBUG1,"Entering ProcessNextFSteM [%d]",fjord->operator);
	elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d], node->probehasresults: [%d]",
		  node->doneprobe, node->haveprobe,node->probehasresults);
#endif
	
	/* Loop until we exhaust our input queue or fill our output queue or 
	 * process the highest allowed number of tuples. */
	for (i = 0; i < node->max_work_units; i++) 
	{
		/*
		 * The last enqueue operation may have failed; do we have an output
		 * tuple left over from a failed enqueue?
		 */
		if (NULL != node->unqueued_output) 
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1,"ProcessNextFSteM: unqueued_output present");
#endif
			FJORD_ENQ_OUT_PTR(enqsucc, node, &(node->unqueued_output));

			if (enqsucc) 
			{
				node->unqueued_output = NULL;
                node->fstem_num_tuples_returned++;
			}
			else 
			{
				/* Output queue is full; can't do any more work. */
#ifdef AMOLDEBUGON
				elog(DEBUG1, "ProcessNextFSteM(): Output queue is full (1).");
				elog(DEBUG1,"Exiting ProcessNextFSteM");
				elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d], node->probehasresults: [%d]",
					  node->doneprobe, node->haveprobe,node->probehasresults);
				elog(DEBUG1,"------------------------------------------------------------");
#endif
				return;
			}
		}
		if (! node->haveprobe)								/* No probe yet ? */
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1,"ProcessNextFSteM: Not currently probing");
#endif
			/* Not currently probing, so check the input queue. */
			no_inputs = FSteMProcessInputTuple(node, parent);

			if (no_inputs)
			{
#ifdef AMOLDEBUGON
				elog(DEBUG1, "ProcessNextFSteM(): No inputs.");
				elog(DEBUG1,"Exiting ProcessNextFSteM");
				elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d], node->probehasresults: [%d]",
					  node->doneprobe, node->haveprobe,node->probehasresults);
				elog(DEBUG1,"------------------------------------------------------------");
#endif
				/* Input queue is empty. */
				return;
			}
		}
#ifdef AMOLDEBUGON
		elog(DEBUG1,"ProcessNextFSteM: Currently probing .. call genout ..");
#endif
		/* Currently probing; produce the next tuple of output. */
		do 
		{
			results_exist = FSteMGenerateOutputTuple(node, parent);
#ifdef AMOLDEBUGON
			if (results_exist) 
			{
				elog(DEBUG1,"ProcessNextFSteM(): Results exist - not yet produced");
				elog(DEBUG1,"Must call GenerateOutputTuple again");
			}
#endif
		}
		while (results_exist);


/* 		if (full_output) */
/* 		{ */
/* #ifdef AMOLDEBUGON */
/* 			elog(DEBUG1, "ProcessNextFSteM(): Output queue is full (2)."); */
/* 			elog(DEBUG1,"Exiting ProcessNextFSteM"); */
/* 			elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d], node->probehasresults: [%d]", */
/* 				  node->doneprobe, node->haveprobe,node->probehasresults); */
/* 			elog(DEBUG1,"------------------------------------------------------------"); */
/* #endif */ /* Output queue is full */
/* 			return; */
/* 		} */
	}
#ifdef AMOLDEBUGON
	elog(DEBUG1,"Exiting ProcessNextFSteM");
	elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d], node->probehasresults: [%d]",
		  node->doneprobe, node->haveprobe,node->probehasresults);
	elog(DEBUG1,"------------------------------------------------------------");
#endif
}



/* FUNCTION FSteMProcessInputTuple
 * ARGUMENTS: <node> is an FSteM.
 * PRECONDITIONS: <node> is initialized and is not in the middle of a probe.
 * POSTCONDITIONS: Attempts to read a tuple from the input queue.  If it finds
 * a tuple, does the appropriate action (build/probe) with it and returns
 * <FALSE>; otherwise, returns <TRUE>.
 */
bool FSteMProcessInputTuple(FSteM * node, Plan *parent) 
{
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */

	bool 		 deqsucc = false;
	bool 		 enqsucc = false;
	bits8 		*source  = node->protostem->sourceBuiltOn;
	int          sourceID = STEM_SOURCE(node->protostem);

    FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */

	Assert(NULL != node);
	Assert(!node->haveprobe);

	/* elog(LOG,"FSteMProcessInputTuple()"); */

	/* 
	 * Fetch next tuple from input queue, if we can.
	 */
	FJORD_DEQ_INP_PTR(deqsucc,node,&piiht);
	if(! deqsucc)								   /* Better get a tuple! */
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1,"FSteMProcessInputTuple: Nothing in queue !!!");
#endif
		fjord->doneInput = true;
		/* Didn't find anything in the queue. */
		return TRUE;
		/*elog(ERROR, "FSteM dequeue did not get a tuple when expected");*/
	}
	
	input->val = (HeapTuple) iiht;
	input->ttc_isIntermediateTuple = TRUE;
	ResetExprContext(econtext);
	econtext->ecxt_scantuple = input;

#ifdef AMOLDEBUGON
		elog(DEBUG1,"FSteMProcessInputTuple: input tuple");
		debugitup_debug(iiht);
#endif

	/* 
	 * If the tuple corresponds to the source this stem is built on ...
	 */
	if (BitArrayEqual(source, iiht->t_sources, TCQMAXSOURCES)) 
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1,"FSteMProcessInputTuple: Build");
#endif
		/* BUILD */

#ifdef AMOLDEBUGON
		elog(DEBUG1, "Building a tuple in the stem");
#endif
		/* Effectively there are no inputs in this case */
		FSteMInsert(node, econtext);
		node->fstem_num_built++;

		if (node->window_expr_is_valid) 
		{
			node->last_build_ts = get_timestamp_val_IHT(iiht,sourceID);
		}
		
		if (UseOperatorPrecedence || UseStaticPlan) //if (UseOperatorPrecedence) 
		{
			/*
			 *  In this case, we have to bounce the build tuple back ! 
			 */
#ifdef AMOLDEBUGON
			elog(DEBUG1, "Bounce back the build ");
#endif
			FJORD_ENQ_OUT_PTR(enqsucc,node,&iiht);	 /* Bounce back the probe */
			fjord->doneInput = true;
		}
		return true;
	}
	else 
	{
		/* This tuple doesn't correspond to the source this STeM was built
		 * on, so prepare to probe. */

#ifdef AMOLDEBUGON
		elog(DEBUG1,"FSteMProcessInputTuple: Probe");
#endif
		node->fstem_num_input_for_probes++;

		if (ihtIsPunct(iiht))		 /* Mark probe as puncuation if necessary */
		{
			elog(DEBUG1, "***** FStem saw a punctuation tuple!");
			node->punctjoin = true;
		}
		
		if (UseTupleLineage && UsePrecisionSharing) 
		{
			/*
			 * But first, check if we really have to do anything ...
			 */
			bits8 check[TCQMAXQUERIES];

			BitArrayOr(fjord->interestedQueries,
					   iiht->t_queriesCompleted,
					   check,
					   TCQMAXQUERIES);

			if (! BitArrayIsAnyCleared(check,TCQMAXQUERIES)) 
			{
				/*
				 * Hasta la vista, baby !
				 */
#ifdef AMOLDEBUGON
				elog(DEBUG1,"Early out elimination of probe .. precision sharing");
				elog(DEBUG1,"Hey Einstein, wait up .. we still have to bounce back the probe !");
				elog(DEBUG1,"(Not for a static plan though !)");
#endif

				node->fstem_num_early_eliminated++;
				if (! UseStaticPlan) 
				{
					FJORD_ENQ_OUT_PTR(enqsucc,node,&iiht);/* Bounce back the probe */

					if (enqsucc)
					{
						node->fstem_num_tuples_returned++;
						return true;
					}
					else
					{
						/* Enqueue failed; save it for later. */
#ifdef AMOLDEBUGON
						elog (DEBUG1, "Hmm .. shouldn't happen .. enqueue failed ");
#endif
						node->unqueued_output = iiht;
						return false;
					}
				}
				return true;
			}
		}
		
		node->currentouterhashkey = NIL;	/* not really needed. */
		node->currentfstemclauses = NIL;

		/** This function will set the outer tuple and the
		 * stem->outerhashkey etc correctly */

		if ((! node->punctjoin) &&
			(FSteMSetOuterTupleNext(node, econtext) == false))
		{
#ifdef AMOLDEBUGON
			elog(ERROR, "FSteMSetOuterTupleNext failed");
#endif
		}
		
		/* FSteMSetNewOuter(node, econtext); */
		node->haveprobe = TRUE;
		node->doneprobe = FALSE;
		node->probehasresults=FALSE;
	}

	return FALSE;

}

/* FUNCTION FSteMGenerateOutputTuple
 * ARGUMENTS: <node> is an FSteM
 * PRECONDITIONS: <node> is initialized and is currently probing. 
 * POSTCONDITIONS: Generates and enqueues the next tuple in the probe 
 * (possibly the bounce-back tuple), and returns <FALSE>.  Returns <TRUE> 
 * and sets node->unqueued_output if it runs out space in the output queue.
 */
bool
FSteMGenerateOutputTuple(FSteM * node, Plan *parent) 
{
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */

	bool 		 enqsucc = false;
	EddyState 	*eddystate;
	bits8 		 scratch[BASIZE(TCQMAXOPERATORS)];
	int 		 i;

	bool 		tuple_is_null 		  = false;
	bool 		 remaining_join_quals = false;
	List 		*iter;
	int 		 queryid;
	int          sourceID 			  = STEM_SOURCE(node->protostem);
	
		
    FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */

	eddystate = eddy->eddystate;

	Assert(node != NULL);
	Assert(node->haveprobe);
	Assert(NULL == node->unqueued_output);

#ifdef AMOLDEBUGON
	elog (DEBUG1,"node->doneprobe: [%d], node->haveprobe: [%d]",node->doneprobe, node->haveprobe);
#endif
	
	if (node->doneprobe) {	/* Last tuple was the bounce back */
		node->doneprobe = false;	/* All set, again */
		node->haveprobe = false;	/* Get ready for next tuple */
		return FALSE;				/* No output */
	}

	econtext->ecxt_scantuple = input;
	iiht = (IntermediateHeapTuple) input->val;

	elog(DEBUG1,"FSteMGenerateOutputTuple: (npj:%d) (npp:%d)",
		 node->punctjoin,node->punctprobe);
	if (node->punctjoin) 
	{
		/*
		 *  1) If there was a previously alloc'd punctuation, free it first
		 */
		if (NULL != node->punct) 
		{
			heap_freetuple(node->punct);
			node->punct = NULL;
		}
		/*
		 *  2) We create a build punct tuple using the last build tuple seen
		 */
		if (node->window_expr_is_valid) 
		{
			node->punct = form_punct_tup(node->build_td, node->last_build_ts);
		}
		else 
		{
			node->punct = form_punct_tup_no_ts(node->build_td);
		}
		/*
		 *  3) We generate a new join tuple with punctuations and return it
		 */
		copyIntermediateHeapTuple(oiht,iiht);
		AddBaseTuple(oiht,node->punct,node->build_td,sourceID,0);
#ifdef DEBUG_FSTEM
		elog(DEBUG1,"Printing oiht ... after adding punct tuple");
#endif
		debugitup_debug(oiht);
	}
	else if (node->punctprobe) 
	{
		tuple_is_null = true;
		node->punctprobe = false;
	}
	else 
	{
		do 
		{
			output = FSteMGetNextOutput(node);
			tuple_is_null = TupIsNull(output);
		
		} while (tuple_is_null &&
				 (remaining_join_quals = FSteMSetOuterTupleNext(node, econtext)));
	}

    if (tuple_is_null) 
	{
		
#ifdef AMOLDEBUGON
		elog(DEBUG1, "AMOLCODE: Really done with this outer tuple");
#endif
		
		node->doneprobe = true;	/* No more join tuples */

		/* since this a probe tuple, it should not be output */
		/* for any query on which this stem is built.  iterate */
		/* over the queriesUsedBy list and set the completed bits */
		if (! UseTupleLineage) 
		{
			node->haveprobe = false;
			fjord->doneInput = true;
			return false;
		}
		
		foreach(iter, fjord->usedByQueries)
		{
			queryid = lfirsti(iter);
			BitArraySetBit(iiht->t_queriesCompleted, queryid);
		}
		if(!node->probehasresults)
		{
			if(node->sourcehasbindings && node->source)
			{
#ifdef AMOLDEBUGON
				elog(DEBUG1, "%d", nodeTag(node->source));
#endif
				Assert(IsA(node->source, ScanModule));
				ScanModuleDeliverProbeTuple(node->source,
											(IntermediateHeapTuple)input->val);
				
				
			}
		}

		if (BitArrayIsAnyCleared(iiht->t_queriesCompleted, TCQMAXQUERIES)) 
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1, "Bounce back the probe ");
			debugitup_debug(iiht);
#endif 

			FJORD_ENQ_OUT_PTR(enqsucc,node,&iiht);	 /* Bounce back the probe */

			node->fstem_num_probe_bouncebacks++;
		
			if (enqsucc)
			{
				node->haveprobe = false;
				node->fstem_num_tuples_returned++;
				return FALSE;
			}
			else
			{
				/* Enqueue failed; save it for later. */
#ifdef AMOLDEBUGON
				elog (DEBUG1, "Hmm .. shouldn't happen .. enqueue failed ");
#endif
				node->unqueued_output = iiht;
				return TRUE;
			}
		}
		else 
		{
			node->haveprobe = false;
#ifdef AMOLDEBUGON
			elog(DEBUG1,"The probe is done with all queries .. hasta la vista");
#endif
			return FALSE;
		}
	}
	else 
	{
		
		BitArray		 otherstems;
		List 			*iter2;
		bool 			 found;
		
		node->probehasresults=TRUE;


		if (node->punctjoin) 
		{
			node->punctjoin = false;
			node->punctprobe = true;
		}
		else 
		{
			copyIntermediateHeapTuple(oiht,(IntermediateHeapTuple) output->val);
		
			if (UseTupleLineage) 
			{
				foreach(iter, fjord->usedByQueries)
				{
					queryid = lfirsti(iter);
					found = false;
#ifdef AMOLDEBUGON
					elog(DEBUG1,"\tTesting query %d, found=%d ",queryid,found);
#endif
					if (NULL == node->currentfstemclauses) 
					{
						elog(NOTICE, "Warning: No current fstem clauses...");
					}
					else 
					{
						List 		 *cfclist = node->currentfstemclauses;
						FSteMClauses *clauses = (FSteMClauses *)lfirst(cfclist);
						
						foreach(iter2, clauses->interested_queries)
						{
#ifdef AMOLDEBUGON
							elog(DEBUG1,"\t\tTesting lfirsti(iter2): %d ",
								 lfirsti(iter2));
#endif
							
							if (queryid == lfirsti(iter2)) 
							{
								found = true;
							}
						}
					}
					
					if (! found) 
					{
#ifdef AMOLDEBUGON
						elog(DEBUG1,"\tSetting QC bit %d ",queryid);
#endif
						BitArraySetBit(oiht->t_queriesCompleted, queryid);
					}
				}
			}
		}
		
		
#ifdef AMOLDEBUGON
		elog(DEBUG1,"Got a result from the stem\n");
		debugitup_debug(oiht);
		elog(DEBUG1,"oiht->QC is:");
		BitArrayPrintDebug(oiht->t_queriesCompleted, 8);
#endif
		if ((! UseOperatorPrecedence) && UseTupleLineage)
		{
			
			/*
			 * Fetch the build tuple's current ready bits from the Eddy.
			 *
			 * Note that it's not possible to fetch this from the SteM since the
			 * tuples built in the stem have had their done bits scrubbed clean
			 */
			BitArrayOrSelf(oiht->t_ready,
						   eddystate->operators[STEM_SOURCE(node->protostem)],
						   TCQMAXOPERATORS);
			/*
			 * We now want to make sure that we _don't_ go to any of the stems of
			 * any of the sources of the probe tuple
			 *
			 * Note that one option would be to use the PostJoinOperators here. That
			 * would however be oppressive as the pjolist data structure will then
			 * have entries for all source signatures.
			 */
			for (i = 0; i < TCQMAXSOURCES; i++) 
			{
				if (BitArrayBitIsSet(iiht->t_sources, i)) 
				{
					BitArrayClone(scratch, eddystate->sourceStems[i],
								  TCQMAXOPERATORS);
					BitArrayNotSelf(scratch, TCQMAXOPERATORS);
					BitArrayAndSelf(oiht->t_ready, scratch, TCQMAXOPERATORS);
				}
			}
			/*
			 * Now we need to augment the output tuple with ready bits for all Post
			 * join operators.
			 * 
			 * Fetch bitmaps from PostJoinOperators.
			 */
			setReadyFromPJOList(eddy, oiht);
		
			/* 
			 *  Note that we shouldn't come back here !! 
			 */
			BitArrayClearBit(oiht->t_ready, operator);
		
			/*
			 * Finally we need to make sure we set the done bits of all _other_
			 * stems built on this same source - we oughtn't to visit 'em !
			 */
			otherstems = eddystate->sourceStems[STEM_SOURCE(node->protostem)];
			for (i = 0; i < TCQMAXOPERATORS; i++) 
			{
				if (BitArrayBitIsSet(otherstems, i)) 
				{
					BitArraySetBit(oiht->t_done, i);
				
					/*
					 * To truly make sure that these tuples don't get probed into
					 * other stems built on the same relation (the boucing of the
					 * probe tuple will make sure correct results are produced)
					 * we need to clear the ready bits in addition to setting
					 * the done bits, otherwise the eddy will send it to the other
					 * module anyway if there is another legitamite (filter, etc)
					 * module that the tuple needs to get sent to.
					 *
					 * -garrettj
					 */				
					BitArrayClearBit(oiht->t_ready, i);
				}
			}


			/* 
			* And just when we thought we were all done, we need to make sure that 
			* this composite tuple does not go to ANY event operator, as they do not
			* like tuples that are multi-source.
			*
			*     - TCQ SR
			*/
			
			BitArrayClone(scratch, eddystate->eventOperators, TCQMAXOPERATORS);
			BitArrayNotSelf(scratch, TCQMAXOPERATORS);
			BitArrayAndSelf(oiht->t_ready, scratch, TCQMAXOPERATORS);

		}
		
		/*
		 *  All done .. phew .. we can output the tuple
		 */
		if ((! UseTupleLineage) ||
			BitArrayIsAnyCleared(oiht->t_queriesCompleted, TCQMAXQUERIES)) 
		{
			FJORD_ENQ_OUT_PTR(enqsucc,node,&oiht);
			node->fstem_num_probe_results++;
			if (enqsucc)
			{
				node->fstem_num_tuples_returned++;
				fjord->doneInput = false;
				return FALSE;
			}
			else 
			{
				elog(ERROR,"Enqueue failed should never, ever, happen ..");
				/* Enqueue failed; save it for later. */ 
				node->unqueued_output = iiht;
				return TRUE;
			}
		}
		else 
		{
			return TRUE;
		}
	}
}


/*
 * FSTEM-SPECIFIC INTERFACE FUNCTIONS
 */

/* FUNCTION SetTimestampAccessorFSteM
 * ARGUMENTS: <node> is an FSteM, <ts_accessor> is a Var node that fetches the
 *		timestamp field from tuples with the signature of this FSteM.
 * PRECONDITIONS: <node> is initialized, and <ts_accessor> points to an object
 *		that fetches the appropriate field.
 * POSTCONDITIONS: Makes a copy of the accessor for this FSteM's use, if the
 *		FSteM does not already have one.  This function is itempotent.
 */
void
SetTimestampAccessorFSteM(FSteM * node, Var * ts_accessor)
{
	Assert(NULL != node);
	Assert(NULL != ts_accessor);
	Assert(true == node->initialized);


	if (NULL != node->ts_accessor) {
		/* Already got one! */
		return;
	} else {
		/* We make a deep copy, since the object pointed to by <ts_accessor>
		 * will probably disappear very soon. */
		node->ts_accessor = copyObject(ts_accessor);
	}

}


/* FUNCTION AddWindowFSteM
 * ARGUMENTS: <node> is an FSteM, and <window_size> is a constant that defines
 *		the window size, given as a string that encodes a relative time 
 *		interval (reltime).
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Increases the window size of <node> so that this interval
 *		encompasses <window_size>.
 */
void
AddWindowFSteM(FSteM * node, Const * window_size)
{
	Expr *rhs;
	Const **window_size_ptr_ptr;

	Assert(NULL != node);
	Assert(NULL != window_size);
	Assert(true == node->initialized);

	/* Make sure that we have the accessor that lets us reach the timestamp
	 * field of the type of tuple that we handle with this SteM. */
	if (NULL == node->ts_accessor) {
		elog(ERROR, "(internal error) FSteM timestamp accessor not set");
		return;
	}

	/* If necessary, create the tree of operators that tells us whether two
	 * tuples should go in the same subSteM. */
	if (NULL == node->same_stem_expr) {
		FSteMCreateEmptyWindowExprs(node, window_size);
	}

	/* Check to see whether we need to change the window size. */
	if (FSteMNewWindowIsLarger(node, window_size)) 
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1, "FSteM: Putting a larger window size into place.");
#endif

		/* 
		 * Updating node->same_stem_expr...
		 */

		/* rhs here is the "window_size / <num. substems - 1>" part of the
		 * expression" */
		rhs =
			(Expr *)
			DatumGetPointer(lfirst(lnext(node->same_stem_expr->args)));
		window_size_ptr_ptr = (Const **)(&(rhs->args->elem.ptr_value));

		/*FSteMReplaceWindow(window_size, (Const **) (&lfirst(rhs->args)));*/
		FSteMReplaceWindow(window_size, window_size_ptr_ptr);

		/* 
		 * Updating node->stem_expired_expr...
		 */

		window_size_ptr_ptr = 
			(Const**)
			(&((lnext(node->stem_expired_expr->args))->elem.ptr_value));
		/*
		rhs =
			(Expr *)
			DatumGetPointer(lfirst(lnext(node->stem_expired_expr->args)));*/
		FSteMReplaceWindow(window_size, window_size_ptr_ptr);
	}

	/* The window expression should be valid now. */
	node->window_expr_is_valid = true;

}

/* FUNCTION ClearWindowFSteM
 * ARGUMENTS: <node> is an FSteM
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Removes the arguments for any window expression attached to 
 *		<node>.  Does NOT touch the timestamp accessor.
 */
void
ClearWindowFSteM(FSteM * node)
{
	Assert(FSteMIsInitialized(node));

	node->window_expr_is_valid = false;
}

/* FUNCTION GetRepresentativeSTeMFSteM
 * ARGUMENTS: <node> points to an FSteM.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Returns a SteM that has the same properties (e.g. signatures
 * of tuples stored) as the SteMs that this FSteM uses to store tuples.
 */
SteM *
GetRepresentativeSteMFSteM(FSteM * node)
{
	Assert(FSteMIsInitialized(node));

	return node->protostem;
}

/* FUNCTION FSteMSetMaxWorkPerIter
 * ARGUMENTS: <node> is an FSteM, and <numtuples> is the number of tuples 
 *		to process per call to ProcessNextFSteM().
 * PRECONDITIONS: <node> is initialized, and <numtuples> is greater than zero.
 * POSTCONDITIONS: Sets the number of tuples to process per call.
 */
void FSteMSetMaxWorkPerIter(FSteM * node, int numtuples) 
{
	Assert(FSteMIsInitialized(node));
	Assert(numtuples > 0);
	
	node->max_work_units = numtuples;
}


/*
 * INTERNAL FUNCTIONS
 */

/* FUNCTION FSteMIsInitialized
 * ARGUMENTS: <node> is a pointer to an FSteM node.
 * POSTCONDITIONS: Returns TRUE if the node is initialized.
 */
bool FSteMIsInitialized(FSteM * node) 
{
	return (node != NULL && true == node->initialized);
}

/* FUNCTION FSteMReplaceWindow
 * ARGUMENTS: <new_window_size> is a new window size, and <old_window_size_ptr>
 *		points to where an existing window size is stored.
 * PRECONDITIONS: <old_window_size_ptr> either is NULL or points to 
 *		something that can be removed using pfree().
 * POSTCONDITIONS: Removes the old window size and puts a copy of the new 
 *		one in its place.
 */
void
FSteMReplaceWindow(Const * new_window_size, Const ** old_window_size_ptr)
{
	Assert(NULL != old_window_size_ptr);

	if (NULL != *old_window_size_ptr) {

		/* The reltime is stored boxed, so we need to do a "deep"
		 * pfree(). */
		/* TODO: Find the Postgres library function for freeing Consts. */
		Assert(false == (*old_window_size_ptr)->constbyval);
		Assert(false == (*old_window_size_ptr)->constisnull);
		/* Sanity checks. */

		pfree(DatumGetPointer((*old_window_size_ptr)->constvalue));

		pfree((*old_window_size_ptr));
	}

	/* Now we can finally put the new value in place. */
	*old_window_size_ptr = copyObject(new_window_size);
}

/* FUNCTION FSteMCreateWindowExprs
 * ARGUMENTS: <node> is an FSteM node, and <dummy_window_size> is a Const that
 * has the same type as the window sizes you're going to pass to the FSteM
 * later on.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Creates window expressions with a window size of zero
 *		and attaches these expressions to the <same_stem_expr> and
 *		<stem_expired_expr> members of <node>.  
 */
void
FSteMCreateEmptyWindowExprs(FSteM * node, Const * dummy_window_size)
{
	Var *first_ts_acc = NULL, *second_ts_acc = NULL;
	Expr *lhs = NULL, *rhs = NULL;
	Const *window_divisor = NULL;
	Expr *new_lhs = NULL;
	Const *new_rhs = NULL;
	Const *window_size_placeholder_1 = NULL;

	/* Types of variables in our expressions. */
	Oid INTEGER_TYPE = INT4OID /*, RELTIME_TYPE = TINTERVALOID */;

	Assert(NULL == node->same_stem_expr);
	/* Prevent memory leaks. */

	/* The first window expression takes as input two timestamps (which we'll 
	 * call t1 and t2) and the time interval <window_size>, and returns 
	 * whether:
	 *      (t2 - t1) < (window_size / <number of substems - 1>)
	 */

	/* First, use node->ts_accessor to create accessors for the two
	 * timestamps.  */
	Assert(NULL != node->ts_accessor);
	first_ts_acc = (Var *) copyObject(node->ts_accessor);
	first_ts_acc->varno = FSTEM_OLDER_TUPLE_VARNO;
	second_ts_acc = (Var *) copyObject(node->ts_accessor);
	second_ts_acc->varno = FSTEM_NEWER_TUPLE_VARNO;

	/* Now we can create the left hand side of the equation, e.g.  t2 - t1 */
	lhs = make_op(makeList1(makeString("-")), 
			(Node *) second_ts_acc, (Node *) first_ts_acc);

	replace_opid((Oper *) (lhs)->oper);
	/* Not sure what this call to replace_opid() does. */

	/* Create an integer constant for <number of substems - 1>. */
	window_divisor = makeConst(INTEGER_TYPE, sizeof(int), 
			(Datum) (FSTEM_NUM_SUBSTEMS - 1), 
							false	/* Not null */ ,
							true /* Passed by value */ ,
							false /* Not a set. */ ,
							false /* Not a cast? */ );

	/* Now we can create the right hand side of the equation, e.g.
	 * window_size / <number of substems - 1> */
	/* Since we don't yet have the window size in hand, we use a placholder
	 * string. */
	window_size_placeholder_1 = copyObject(dummy_window_size);

	rhs =
		make_op(makeList1(makeString("/")), (Node *) window_size_placeholder_1,
				(Node *) window_divisor);
	replace_opid((Oper *) (rhs)->oper);

	/* Put it all together and we get the window expression. */
	node->same_stem_expr = make_op(makeList1(makeString("<")), 
			(Node *) lhs, (Node *) rhs);
	replace_opid((Oper *) ((Expr *) node->same_stem_expr)->oper);

	/* Done with <same_stem_expr>. */


	/* The second window expression is a simpler version of the first: (t2 -
	 * t1) > window_size NOTE: ">" instead of "<"... */
	/* Reuse the left hand side of the first expression. */
	new_lhs = copyObject(lhs);
	new_rhs = copyObject(dummy_window_size);

	node->stem_expired_expr =
		make_op(makeList1(makeString(">")), (Node *) new_lhs, (Node *) new_rhs);
	replace_opid((Oper *) ((Expr *) node->stem_expired_expr)->oper);

	/* Done with <stem_expired_expr>. */

	/* No window size yet! */
	node->window_expr_is_valid = false;
}

/* FUNCTION FSteMGCSubstems
 * ARGUMENTS: <node> is an FSteM, <next_insert_tup> is the tuple for the next
 *		insert.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Garbage-collects SteMs whose tuples have expired, and
 * updates internal data structures accordingly.
 */
void
FSteMGCSubstems(FSteM * node, ExprContext * next_insert_tup)
{
	int new_first_index;
	SteM *st;


	/* SPECIAL CASE: If we don't know the window size, assume an infinite
	 * window. */
	if (!(node->window_expr_is_valid)) {
		return;
	}


	/* SubSteMs should expire in order, so start with the oldest and move
	 * forward in time. */
	while (FSTEM_NOT_AN_INDEX != node->first_ss_index
		   && true == FSteMSubSteMHasExpired(node, node->first_ss_index,
											 next_insert_tup)) {

		st = FSTEM_SUBSTEM_PTR(node, node->first_ss_index);

		node->total_probed   += st->stemstate->sm_tuplesProbed;
		node->total_inserted += st->stemstate->sm_tuplesInserted;
		node->total_returned += st->stemstate->sm_tuplesReturned;

		
		
		/* The subSteM at node->first_ss_index has expired, so wipe it out. */
		ExecEndSteM(st);
		node->fstem_num_substems_ended++;
		
#ifdef TCQDEBUGON
			elog(DEBUG1, "TCQDEBUG: %s %d removed a substem\n",
				 __FILE__, __LINE__);
#endif
		
		FSTEM_SUBSTEM_PTR(node, node->first_ss_index) = NULL;
		/* node->substems[node->first_ss_index].firsttime = FSTEM_NOT_A_TIME; */
		/* node->substems[node->first_ss_index].lasttime = FSTEM_NOT_A_TIME; */

		/* insert_ss_index is the last index. */
		if (node->insert_ss_index == node->first_ss_index) {
			/* We just removed the last subSteM. */
#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: %s %d removed the last substem\n",
				 __FILE__, __LINE__);
#endif
			node->insert_ss_index = FSTEM_NOT_AN_INDEX;
			node->probe_ss_index = FSTEM_NOT_AN_INDEX;
			node->first_ss_index = FSTEM_NOT_AN_INDEX;
			node->haveprobe = false;
			node->doneprobe = false;
		} else {
			/* We still have one or more active subSteMs.  Move on to the
			 * next subSteM in chronological order.  Don't forget that
			 * substems[] wraps around! */
			if (node->first_ss_index == node->insert_ss_index) {
				/* We only have one active subSteM. */
				new_first_index = FSTEM_NOT_AN_INDEX;
			} else {
				new_first_index = FSTEM_NEXT_SS_INDEX(node->first_ss_index);
			}

			/* Pull the probe index forward, if necessary. */
			if (node->probe_ss_index == node->first_ss_index) {
				node->probe_ss_index = new_first_index;
			}

			node->first_ss_index = new_first_index;
		}
	}

}

/* FUNCTION FSteMInsert
 * ARGUMENTS: <node> is an FSteM, <econtext> contains a tuple.
 * PRECONDITIONS: <node> is initialized and <econtext> is valid.  <currenttime>
 *		is >= the highest timestamp of any tuple inserted thus far. 
 * POSTCONDITIONS: Inserts the tuple into the latest subSteM, making space if
 *		necessary.
 */
void
FSteMInsert(FSteM * node, ExprContext * econtext)
{

	/* We shouldn't be inserting if we're in the middle of a probe. */
	Assert(false == node->haveprobe);

	/* Garbage-collect expired subSteMs. */
#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d\n", __FILE__, __LINE__);
#endif
	FSteMGCSubstems(node, econtext);


	FSteMMakeSpaceForInsert(node, econtext);


#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d\n", __FILE__, __LINE__);
#endif

	ExecSteMInsert(FSTEM_SUBSTEM_PTR(node, node->insert_ss_index), econtext, true /* whether_to_copy */);

    node->fstem_num_tuples_inserted++;

	/* Don't forget to update those timestamp values! */
	/* On second thought, forget it. */
	/* node->substems[node->insert_ss_index].lasttime = currenttime; */
}

/* FUNCTION FSteMMakeSpaceForInsert
 * ARGUMENTS: <node> is an FSteM, <nexttuple> points to an expression context
 *		containing the next tuple to insert, and hence its timestamp. 
 * PRECONDITIONS: The FSteM is initialized.
 * POSTCONDITIONS: Makes sure that there is space to perform a probe, and
 * updates insert_ss_index if necessary.
 */
void
FSteMMakeSpaceForInsert(FSteM * node, ExprContext * nexttuple)
{
	int new_substem_index;

	/* Is there an active subSteM to insert into? */
	if (FSTEM_NOT_AN_INDEX == node->insert_ss_index) {
		/* No subSteMs exist, so create one. */
		FSteMCreateNewSubstem(node, 0);
#ifdef AMOLDEBUGON
        /*
		elog(DEBUG1, "AMOLCODE: %s %d %x %d creating a new SteM\n", __FILE__,
			 __LINE__, node, node->insert_ss_index);
             */
#endif

		node->insert_ss_index = node->first_ss_index = 0;
#ifdef AMOLDEBUGON
        /*
		elog(DEBUG1, "AMOLCODE: %s %d %x %d creating a new SteM\n", __FILE__,
			 __LINE__, node, node->insert_ss_index);
             */
#endif
	} else {
		/* There is at least one existing subSteM.  Is this subSteM valid for
		 * the time inverval containing the next tuple? */
		if (!FSteMTupleFitsInSubSteM(node, node->insert_ss_index, nexttuple)) {
			/* The current active subSteM is no longer accepting tuples, so
			 * create a new subSteM, if possible. */

#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: %s %d creating a new SteM\n", __FILE__,
				 __LINE__);
#endif

			new_substem_index = FSTEM_NEXT_SS_INDEX(node->insert_ss_index);

			/* If there isn't any space to create a new subSteM (due to an
			 * increase in the window size), we need to continue using the
			 * current subSteM. */
			if (new_substem_index != node->first_ss_index) {

				FSteMCreateNewSubstem(node, new_substem_index);

				node->insert_ss_index = new_substem_index;
			}

		} else {
			/* If we get here, there should be a subSteM that will accept the
			 * next tuple.  It shouldn't be necessary to do anything in this
			 * case. */

			/* For sanity's sake, check to make sure we aren't getting
			 * out-of-order tuples. */
			/* Assert(currenttime >=
			 * node->substems[node->insert_ss_index].lasttime); */
		}
	}
}


/* FUNCTION FSteMGetNextOutput
 * ARGUMENTS: <node> is an FSteM.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: If <node> is in the middle of producing output tuples and
 * has an available tuple in one of its SteMs, returns the next available
 * output tuple; otherwise, returns NULL.
 */
TupleTableSlot *
FSteMGetNextOutput(FSteM * node)
{
	TupleTableSlot *probe_result;
	/* int new_substem_index; */

	/* AFAIK, ExecSteM returns tuples until it runs out of results for the
	 * current probe, and then returns NULL.  We go through the substems one
	 * by one, using node->probe_ss_index to remember the current location. */

	if (!FSteMHasSubSteMs(node)) {
		return NULL;
	}

	/* If we get here, there must be at least one active subSteM. */

	if (FSTEM_NOT_AN_INDEX == node->probe_ss_index) 
	{
		/* We're starting a totally new probe.  Are there any subSteMs to
		 * probe? */
/*		if (FSTEM_NOT_AN_INDEX == node->first_ss_index) {
			return NULL;
		} else {*/
			node->probe_ss_index = node->first_ss_index;
/*		} */
	}

	probe_result = ExecSteM(FSTEM_SUBSTEM_PTR(node, node->probe_ss_index));

	if (NULL == probe_result) {

		/* We've exhausted this subSteM, so move onto the next one, if any. */
		if (node->probe_ss_index != node->insert_ss_index) {
			/* Recall that node->insert_ss_index is the index of the most
			 * recent subSteM. */
			node->probe_ss_index = FSTEM_NEXT_SS_INDEX(node->probe_ss_index);

			/* Recurse to check the next substem. */
			return FSteMGetNextOutput(node);
		} else {
			/* We've run out of subSteMs to check. */
			node->probe_ss_index = FSTEM_NOT_AN_INDEX;
		}
	} else {

#ifdef AMOLDEBUGON
		elog(DEBUG1, "AMOLCODE: %s %d probe result not null-- printing\n",
			 __FILE__, __LINE__);
		debugitup_debug((IntermediateHeapTuple) probe_result->val);
#endif
	}

	return probe_result;
}

/* FUNCTION FSteMSetNewOuter
 * ARGUMENTS: <node> is an FSteM, <newcontext> contains a tuple.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Sets the outer tuple that we use for probing the subSteMs.
 */
void
FSteMSetNewOuter(FSteM * node, ExprContext * newcontext)
{
	int current_index;

#ifdef AMOLDEBUGON
	elog(DEBUG1,"FSteMSetNewOuter ..");
#endif

	/* SPECIAL CASE: Don't need to do anything if there are no substems. */
	if (! FSteMHasSubSteMs(node)) return;

	/* We should be done with the last probe before we start a new one... */
	Assert(FSTEM_NOT_AN_INDEX == node->probe_ss_index);

	/* Set the outer tuple for all the subSteMs. */
	/* Recall the the range of subSteMs that is currently being used runs 
	 * from (node->first_ss_index) to (node->insert_ss_index), inclusive.  */
	current_index = node->first_ss_index;
	do
	{
		SteM *curstem = FSTEM_SUBSTEM_PTR(node, current_index);
		current_index = FSTEM_NEXT_SS_INDEX(current_index);

		ExecSteMSetNewOuter(curstem, newcontext);

	} while (current_index != FSTEM_NEXT_SS_INDEX(node->insert_ss_index));

	/* Reset the state that keeps track of where we are in the current probe. */
	node->probe_ss_index = FSTEM_NOT_AN_INDEX;

    node->fstem_num_tuples_probed++;
}

/* FUNCTION FSteMSubSteMHasExpired
 * ARGUMENTS: <node> is an FSteM node, <index> is an index into node->substems,
 *		<newest_tup> holds a tuple that we're about to insert.
 * PRECONDITIONS: <node> is initialized, and the indicated index contains an
 *		active subSteM.
 * POSTCONDITIONS: Returns <true> if the timestamp of the indicated tuple is
 *		within the window of time that we're putting in the indicated subSteM.
 */
bool
FSteMSubSteMHasExpired(FSteM * node, int index, ExprContext * newest_tup)
{
	IntermediateHeapTuple *older_tuple;
	IntermediateHeapTuple *newer_tuple;
	bool ret;
	/* Will be returned. */


	Assert(NULL != FSTEM_SUBSTEM_PTR(node, index));

	Assert(node->window_expr_is_valid);

	/* node->stem_expired_expr is an expression Node that, given two tuples
	 * encoded as INNER and OUTER (renamed here to FSTEM_OLDER_TUPLE_VARNO
	 * and FSTEM_NEWER_TUPLE_VARNO) arguments, returns TRUE if the two tuples 
	 * have timestamps that are more than <window_size> units of time apart. */

	/* We return <true> if the _newest_ tuple in the substem has expired. */

	/* First, we need to construct an expression context that contains the
	 * two tuples in question. */
	older_tuple = SteMGetNewestTuple(FSTEM_SUBSTEM_PTR(node, index));

	if (NULL == (*older_tuple)) {
		/* SubSteM is empty. */
		return false;
	}

	/*
	elog(DEBUG1, "FSteM: Older tuple is %p, which points to %p.",
			older_tuple, *older_tuple);
			*/

	/* Assert(tup->ecxt_scantuple->ttc_isIntermediateTuple); */
	newer_tuple =
		(IntermediateHeapTuple *) & (newest_tup->ecxt_scantuple->val);


	/* TODO: Make sure that this is the correct slot to use. */
	ret = FSteMEvalWindowExpr(node, older_tuple, newer_tuple,
							  node->stem_expired_expr);

	if (true == ret) {
#ifdef AMOLDEBUGON
		elog(DEBUG1, "FSteM(%p): Subtem %d has expired.", node, index);
#endif
	} else {
/*		elog(DEBUG1, "FSteM: Subtem %d has NOT expired.", index); */
	}

	return ret;

}

/* FUNCTION FSteMTupleFitsInSubSteM
 * ARGUMENTS: <node> is an FSteM node, <index> is an index into node->substems,
 *		<tup> holds a tuple that we're about to insert.
 * PRECONDITIONS: <node> is initialized, and the indicated index contains an
 *		active subSteM.
 * POSTCONDITIONS: Returns <true> if the timestamp of the indicated tuple is
 *		within the window of time that we're putting in the indicated subSteM.
 */
bool
FSteMTupleFitsInSubSteM(FSteM * node, int index, ExprContext * tup)
{
	IntermediateHeapTuple *older_tuple;
	IntermediateHeapTuple *newer_tuple;

	Assert(NULL != FSTEM_SUBSTEM_PTR(node, index));

	/* SPECIAL CASE: If there is no window expression, assume an infinite
	 * window. */
	if (!node->window_expr_is_valid) {
		return true;
	}

	/* node->same_stem_expr is an expression Node that, given two tuples
	 * encoded as INNER and OUTER (renamed here to FSTEM_OLDER_TUPLE_VARNO
	 * and FSTEM_NEWER_TUPLE_VARNO) arguments, returns TRUE if the two
	 * tuples should be put into the same subSteM.  */

	/* First, we need to construct an expression context that contains the
	 * two tuples in question. */
	older_tuple = SteMGetOldestTuple(FSTEM_SUBSTEM_PTR(node, index));

	if (NULL == (*older_tuple)) {
		/* SubSteM is empty. */
		return true;	
	}

	/* Assert(tup->ecxt_scantuple->ttc_isIntermediateTuple); */
	newer_tuple = (IntermediateHeapTuple *) & (tup->ecxt_scantuple->val);
	/* TODO: Make sure that this is the correct slot to use. */

	return FSteMEvalWindowExpr(node, older_tuple, newer_tuple,
							   node->same_stem_expr);
}

/* FUNCTION FSteMEvalWindowExpr
 * ARGUMENTS: <node> is an FSteM node, <older_tuple> and <newer_tuple> are 
 *		tuples with timestamp fields that <node->ts_accessor> can decode,
 *		and <window_expr> is a window expression.
 * PRECONDITIONS: The timestamp on <older_tuple> is older than the timestamp on
 *		<newer_tuple> (currently not checked!).
 * POSTCONDITIONS: Allocates node->window_expr_args if necessary and evaluates
 * the window expression.
 */
bool
FSteMEvalWindowExpr(FSteM * node, IntermediateHeapTuple * older_tuple,
					IntermediateHeapTuple * newer_tuple, Expr * window_expr)
{
	CommonState *scanstate;
	Datum expr_result;
	bool expr_is_null;
	ExprDoneCond expr_is_done;


	Assert(NULL != window_expr);

	/* Allocate the data structure if necessary. */
	if (NULL == node->window_expr_args) {

		/* We need to create a separate memory context for evaluating this
		 * expression.  In order to create this memory context, we need to
		 * create a new CommonState Node. */
		scanstate = makeNode(CommonState);

		ExecAssignExprContext(node->fjord.plan.state, scanstate);

		/* Now we can grab the ExprState out of the CommonScanState we just
		 * created. */
		node->window_expr_args = scanstate->cs_ExprContext;

		/* This expression context needs some TupleTableSlots to hold its
		 * arguments. */
		node->window_expr_args->ecxt_innertuple = MakeTupleTableSlot();
		node->window_expr_args->ecxt_outertuple = MakeTupleTableSlot();

		node->window_expr_args->ecxt_innertuple->ttc_isIntermediateTuple
			= true;
		node->window_expr_args->ecxt_outertuple->ttc_isIntermediateTuple
			= true;

	}
	ResetExprContext(node->window_expr_args);

	/* Then fill it in... */
	node->window_expr_args->ecxt_innertuple->val =
		*((HeapTuple *) older_tuple);
	node->window_expr_args->ecxt_outertuple->val =
		*((HeapTuple *) newer_tuple);


	/* Now we've got the arguments marshalled up, so we can fire the
	 * expression. */
	expr_result = ExecEvalExprSwitchContext((Node *) (window_expr),
											node->window_expr_args,
											&expr_is_null, &expr_is_done);


#ifdef AMOLDEBUGON
    /*
	elog(DEBUG1, "AMOLCODE: %s %d expr_result = %d\n", __FILE__, __LINE__,
		 expr_result);
         */
#endif


	return (DatumGetBool(expr_result));

}

/* FUNCTION FSteMCreateNewSubstem
 * ARGUMENTS: <node> is an FSteM, <index> is an index into node->substems.
 * PRECONDITIONS: <node> is initialized and <index> does not currently contain
 *		a subSteM.
 * POSTCONDITIONS: Creates and initializes a new subSteM at the indicated
 *		index.
 */
void
FSteMCreateNewSubstem(FSteM * node, int index)
{
	Assert(index >= 0 && index < FSTEM_NUM_SUBSTEMS);

	/* Verify the invariant that no subSteM exists at this index already. */
	Assert(NULL == FSTEM_SUBSTEM_PTR(node, index));

	/* We copy the (empty) SteM that was passed to us on initialization when
	 * we create a new SteM. */
	FSTEM_SUBSTEM_PTR(node, index) = copyObject(node->protostem);

	ExecInitSteM(FSTEM_SUBSTEM_PTR(node, index), ((Fjord *) node)->plan.state,
				 (Plan *) node);
}

/* FUNCTION FSteMNewWindowIsLarger
 * ARGUMENTS: <node> is an FSteM node, and <new_window_size> is a window size,
 *		encoded as a relative time interval in string format.
 * PRECONDITIONS: <node> is initialized and has a window expression (possibly
 *		with no window size).
 * POSTCONDITIONS: Returns <true> if <new_window_size> is larger than the
 *		existing window size of <node>.
 */
bool
FSteMNewWindowIsLarger(FSteM * node, Const * new_window_size)
{
	Expr *rhs = (Expr *)
		DatumGetPointer(lfirst(lnext(node->same_stem_expr->args)));
	/* rhs here is the "window_size / <num. substems - 1>" part of the window 
	 * expression" */

	Const *old_window_size = (Const *) DatumGetPointer(lfirst(rhs->args));
	Const *old_window_size_copy = NULL;
	Datum expr_result;
	bool expr_is_null;
	ExprDoneCond expr_is_done;

	/*
	if (NULL == old_window_size) {
		elog(LOG, "FSteM: Old window size is NULL; new one is '%s'.",
				(char*)(new_window_size->constvalue));
	} else {
		elog(LOG, "FSteM: Old window size is '%s'; new one is '%s'.",
			(char*)(old_window_size->constvalue),
			(char*)(new_window_size->constvalue));
	}
	*/

	/* First check to see whether <node> has a window at all. */
	if (NULL == old_window_size) {
		return true;
	} else {
		/* If we get here, <node> has an existing window, so we need to check
		 * whether this old window is a superset of the new window. */

		/* If necessary, create the expression that determines whether the
		 * old window size is large enough. */
		if (NULL == node->ts_comp_expr) {
			/* The expression we're creating is: <new_window_size> >
			 * <old_window_size> */
			old_window_size_copy = (Const *) copyObject(old_window_size);
			node->ts_comp_expr = make_op(makeList1(makeString(">")), 
					(Node *) new_window_size,
					(Node *) old_window_size_copy);
			replace_opid((Oper *) (node->ts_comp_expr)->oper);
		} else {
			/* Put the new window size into place. */
			lfirsti(node->ts_comp_expr->args)
				= PointerGetDatum(new_window_size);
		}

		/* Evaluate the expression. */
		expr_result = ExecEvalExprSwitchContext((Node *) (node->ts_comp_expr),
												node->fjord.scanstate->cstate.
												cs_ExprContext, &expr_is_null,
												&expr_is_done);


		/* We never made a copy of the new window size, so erase our pointer
		 * to it to avoid freeing it twice when destroying the FSteM. */
		lfirst(node->ts_comp_expr->args) = PointerGetDatum(NULL);
		/* TODO: Is this the correct way to do this? */

		return (DatumGetBool(expr_result));
	}
}

/** Debugging print */
static void
ProcessPrintStatsFSteM_with_level(FSteM *node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	elog(level, " FSteM (%s) ",
		 getNodeInformation(eddy,(Node *) node->protostem->innerhashkey));
	
/*     elog(level, "Total: inserted = %d, probed = %d, returned = %d", */
/* 		 node->total_inserted, node->total_probed, node->total_returned); */
	elog(level, " FSteM: [Built: %d] [Probed: %d] [EarlyOut: %d] ",
		 node->fstem_num_built, node->fstem_num_input_for_probes,
		 node->fstem_num_early_eliminated);
	elog(level, " FSteM: [ProbeBounceBack: %d] [ProbeResults: %d",
		 node->fstem_num_probe_bouncebacks, node->fstem_num_probe_results);
	elog(level," FSteM: [SubStemsEnded: %d]",node->fstem_num_substems_ended);
}

void
ProcessPrintStatsFSteM(FSteM *node, Plan *parent)
{
	ProcessPrintStatsFSteM_with_level(node, parent, INFO);
}

/* FUNCTION FSteMHasSubSteMs
 * ARGUMENTS: <node> is an FSteM node
 * PRECONDITIONS: <node> is initialized
 * POSTCONDITIONS: Returns <true> if the FSteM <node> has substems already
 */
static bool
FSteMHasSubSteMs(FSteM *node) 
{
	return (node->first_ss_index != FSTEM_NOT_AN_INDEX);
}


/* 		foreach(iter, fjord->usedByQueries) */
/*         { */
/* 			queryid = lfirsti(iter); */
/* 			found = false; */
/* #ifdef AMOLDEBUGON */
/* 			elog(DEBUG1,"Testing query %d, found=%d ",queryid,found); */
/* #endif */
/* 			foreach(iter2, node->fstemouterhashkeys) */
/* 			{ */
/* 				FSteMOuterHashkey *fohk = (FSteMOuterHashkey *)lfirst(iter2); */
/* #ifdef AMOLDEBUGON */
/* 				elog(DEBUG1,"\tIterating through FSteMOuterHashkey list"); */
/* #endif */
				
/* 				foreach (iter3, fohk->fstemclauses_list) */
/* 				{ */
/* 					FSteMClauses *fsc = (FSteMClauses *) lfirst(iter3); */

/* #ifdef AMOLDEBUGON */
/* 					elog(DEBUG1,"\t\tIterating through FSteMClauses list"); */
/* #endif */

/* 					foreach (iter4, fsc->interested_queries)  */
/* 					{ */
/* #ifdef AMOLDEBUGON */
/* 						elog(DEBUG1,"\t\t\tIterating through interested_queries list"); */
/* #endif */
/* #ifdef AMOLDEBUGON */
/* 						elog(DEBUG1,"\t\tAgainst query %d",lfirsti(iter4)); */
/* #endif */
/* 						if (queryid == lfirsti(iter4))  */
/* 						{ */
/* 							found = true; */
/* 						} */
/* 					} */
/* 				} */
/* 			} */
/* 			if (! found)  */
/* 			{ */
/* #ifdef AMOLDEBUGON */
/* 				elog(DEBUG1,"\tSetting QC bit %d ",queryid); */
/* #endif */
/* 				BitArraySetBit(oiht->t_queriesCompleted, queryid); */
/* 			} */
/* 		} */
		
/* FUNCTION: FSteMContainsTuple
 * ARGUMENT: ExprContext whoose scan tuple slot contains the input iht
 * OUTPUT:  true if the stem contains the tuple, false otherwise
 * PRECOND: stem is not in the middle of a probe
 */
bool FSteMContainsTuple(FSteM * node, IntermediateHeapTuple iht, Eddy * parent)
{
	SteM * curstem = NULL;
	int stemno=0;
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	bool foundDup=false;
	TupleTableSlot tmp;
	bool isslotlocal=false;
	FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */
	if(node->haveprobe)
		{
			elog(FATAL, "FSteMContainsTuple called in the middle of a proble");
		}
	if(econtext->ecxt_scantuple == NULL)
		{
			econtext->ecxt_scantuple=&tmp;
			isslotlocal=true;
		}
	econtext->ecxt_scantuple->val=(HeapTuple)iht;
	econtext->ecxt_scantuple->ttc_isIntermediateTuple=true;

	for(stemno=node->first_ss_index;
		stemno <= node->insert_ss_index;
		stemno++
		)
		{
			curstem = FSTEM_SUBSTEM_PTR(node, stemno);
			if(stem_contains_tuple(curstem,
								   econtext))
				{
					foundDup=true;
					break;
				}
		}


	if(isslotlocal)
		{
			econtext->ecxt_scantuple=NULL;
		}
	return foundDup;
}
