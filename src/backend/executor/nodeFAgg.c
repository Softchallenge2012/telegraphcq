/*-------------------------------------------------------------------------
 *
 * nodeFAgg.c Fjord style operator to handle aggregates operation
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Original implementation by Owen Cooper <owenc@cs.berkeley.edu>.  
 *
 * New implementation by Fred Reiss <phred@cs.berkeley.edu>.
 *
 * "Slices" aggregation functions by Sailesh Krishnamurthy
 * <sailesh@cs.berkeley.edu>
 *
 * HOW IT WORKS
 * ------------
 *
 * This operator performs windowed aggregates after the join operations in a
 * single SPJ (select-project-join) query block.  The intermediate tuples that
 * enter the operator consist of multiple base stream tuples glued together.
 * Each base tuple in an intermediate tuple has a separate timestamp.  Every
 * SPJ block has a window clause consisting of a RANGE BY, a SLIDE BY, and a
 * START AT.  These three parameters specify a sliding, hopping, or jumping
 * window.
 *
 * Our current aggregate window semantics are as follows: 
 *
 *  -- There is a global "current time", defined as the lowest maximum 
 *      timestamp the join processor has consumed across all the streams 
 *      we are joining.  Note that current time can only advance when a tuple
 *      enters the query processor; we do not currently advance based on the
 *      system clock.
 *      
 *  -- For each base stream, the window that is closest to the current time
 *      without going beyond it is considered the "current window" for that
 *      stream.
 *
 *  -- A base tuple is considered "live" if it is inside the current window for
 *      its stream.
 *
 *  -- A tuple is considered to be inside a window if the tuple's timestamp is
 *      >= the start of the window and strictly < the end of the window.
 *
 *  -- An intermediate tuple is considered "live" only if all of its component
 *      base tuples are live.
 *
 *  -- As the current time advances, the current windows of one or more streams
 *      will change.  Each of these changes constitutes a "slide event".
 *      If two windows slide at exactly the same time, it is considered to be
 *      one slide event.
 *
 *  -- Every time that a slide event occurs, this operator outputs the value
 *      for the aggregate as applied to all intermediate tuples that are
 *      currently live.
 *
 *  -- After outputting the aggregate value, the operator updates the set of
 *      live tuples to correspond to the new set of current windows.
 *
 *
 *
 *
 * DATA STRUCTURES
 * ---------------
 *
 * This operator maintains the following data structures:
 *
 *  -- A collection of WindowState objects (implemented in
 *      src/backend/telegraphcq/window.c) to track the status of each base
 *      stream's window.  In particular, these objects keep track of RANGE BY,
 *      SLIDE BY and START AT for each window, as well as tracking the maximum
 *      timestamp we have seen from the tuples of each stream.
 *
 *  -- A set of Timestamps:
 *      a) current_time tracks the current time as inferred from the tuples this
 *          operator has seen.
 *      b) last_slide_time tracks the most recent slide event for which this
 *          operator has output aggregate values and updated current windows.
 *      c) next_slide_time tracks the time of the next slide event after
*           last_slide_time.
*       d) latest_slide_time tracks the latest point in time at which a
*           currently-active window will come to an end.
 *
 *  -- A doubly linked list of live intermediate tuples.  These are the tuples
 *      are are live as of (last_slide_time + epsilon).
 *
 *  -- A linked list of tuple activation times (that is, points in time at
 *      which one or more intermediate tuples will become live).
 *      Each element in this list contains a list of the inactive tuples that
 *      will become active at the indicated time.
 *      This list is kept in sorted order by time.
 *
 *  -- A linked list of tuple deactivation times (that is, points in time at
 *       which one or more live tuples will become dead).
 *       Each element of this list contains a list of pointers into the list of
 *       live tuples.
 *      This list is kept in sorted order by time.
 *
 *  -- An aggregation tree of operators for performing aggregation on buffers
 *      of tuples.
 *
 *
 * ALGORITHM
 * ---------
 *
 * This operator works by iteratively maintaining the correctness of the above
 * data structures.
 *
 * ProcessNextFAgg(intermediate tuple T) operates as follows:
 *
 * 1) Use the WindowStates to calculate when T will become live and when it
 *      will become dead.
 *
 * 2) File the tuple in the appropriate place as of last_slide_time:
 *    a) If the tuple is already dead, discard it and issue a warning.
 *    b) If the tuple is live, add it to the set of live windows and to the
 *      appropriate deactivation group.
 *    c) If the tuple is not yet live, add it to the appropriate activation and
 *      deactivation groups.
 *
 * 3) Update current_time if the timestamps in T cause it to change.  
 * 
 * 4) If current_time > next_slide, node is ready to output aggregate 
 *      results.  Advance the windows by removing any tuples that 
 *      die at next_slide_time and adding any tuples that become live at 
 *      next_slide_time.
 *      Update last_slide_time, latest_slide_time and next_slide_time.  
 *
 * 5) Execute the aggregation tree on the current set of live tuples.
 *    NOTE: ExecFAgg() is used as a callback to pass the tuples up to the
 *    aggregation tree.
 *      
 *    Subsequent calls to ProcessNextFAgg() will return the elements of the 
 *    aggregate results (there may be multiple tuples due to GROUP BY).
 *    NOTE: These aggregate results are produced by pulling them from the
 *    aggregation tree, which in turn pulls its input tuples from the 
 *    ExecFAgg() callback!
 *
 * 6) When all the aggregate results have been consumed, output a punctuation 
 *      tuple (see Punctuation.html in the docs) to indicate that the 
 *      aggregation is complete.
 *
 *
 * ASSUMPTIONS
 * -----------
 * 
 * The correctness of the code in this file depends on the following
 * assumptions.  If any of these are violated, this code will break.
 *
 * a) The timestamps of the streams are synchronized with each other.  
 *
 * b) The join processor consumes tuples from each stream in timestamp order 
 *    NOTE: We do NOT assume that tuples are consumed in timestamp order 
 *    ACROSS streams; only WITHIN a single stream.  
 *
 * c) When the join processor consumes a given tuple, it outputs all 
 *    join results involving that tuple and the ones before it before 
 *    consuming another tuple.
 *
 * d) The system clock is irrelevant; the current time is defined by the
 *    timestamps on the query processor's input tuples.
 * 
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION $Header:
 * /db/cvsroot/postgres/src/backend/executor/nodeFAgg.c,v 1.5 2004/08/26
 * 01:40:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/printtup.h"
#include "catalog/pg_type.h"    /* For TIMESTAMPOID */
#include "executor/execdebug.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFAgg.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "telegraphcq/punct.h"
#include "telegraphcq/window.h"
#include "utils/timestamp.h"

/* Enable the following to get debugging output: */
/*#define DEBUG_FAGG*/

/* 
 * PROTOTYPES OF INTERNAL FUNCTIONS
 */
static void add_new_tuple_range_slide(FAgg *, IntermediateHeapTuple, Plan *);
static void add_new_tuple_slices(FAgg *, IntermediateHeapTuple, Plan *);
static void FAggScheduleAggPlan(FAgg *, Plan *parent);
TupleList *make_tuple_list_node(HeapTuple tup);
ActivationGroup *get_group_for_ts(FAgg *node, bool activation, 
        Timestamp thetime);
void add_to_activation_group(FAgg *node, TupleList *elem, 
        Timestamp birth_time);
void add_to_deactivation_group(FAgg *node, TupleList *elem, 
        Timestamp death_time); 
void add_to_live_list(FAgg *node, TupleList *elem);
void remove_from_live_list(FAgg *node, TupleList *elem);
void advance_window(FAgg *node);
void fix_tdesc_timestampcol(TupleDesc desc);
void destroy_tuple_list_node(TupleList *node);

void reset_agg_tree(FAgg *node, Plan *parent);
void mark_agg_tree(FAgg *node);

#ifdef DEBUG_FAGG
static char * ts_cstr(Timestamp ts);
#endif /* DEBUG_FAGG */

/* 
 * PUBLIC FUNCTIONS
 */


bool
ProcessInitFAgg(FAgg * node, EState * es, Plan * parent)
{
	Plan 		*bottom = NULL;
	FAggState 	*faggstate;

	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FjordInitialize(&(node->fjord), es);	/* Common initialization */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */
	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);


	/* 
	 * assign the node's execution state
	 */
	node->aggPlan->state = es;

    /* We use the temporary memory context to do timestamp arithmetic. */
	fjord->usesTempContext = true;

	/* we need to place this node at the bottom of the agg plan. it is both a 
	 * Fjord type node and a Postgres style node */

	for (bottom = node->aggPlan;
		 outerPlan(bottom) != NULL; bottom = outerPlan(bottom)) ;
	Assert(bottom && outerPlan(bottom) == NULL);

	outerPlan(bottom) = (Plan *) node;

    /* Initialize local data structures. */
	node->total_entered = node->total_exited 
        = node->total_aggregated_this_window = 0;

    node->input_tdesc = node->output_tdesc = NULL;

    node->current_time = DT_NOBEGIN;

    node->live_tuples_head = NULL;
    node->live_tuples_tail = NULL;
    node->live_tuples_cur = NULL;

    node->activation_groups = NULL;
    node->deactivation_groups = NULL;

	node->slicedAgg = FALSE;
	
	/* initialize nodeAgg here */
	ExecInitNode(node->aggPlan, es, (Plan *) node);
    mark_agg_tree(node);

	faggstate = makeNode(FAggState);
	faggstate->faggContext = AllocSetContextCreate(CurrentMemoryContext,
												   "faggContext",
												   ALLOCSET_DEFAULT_MINSIZE,
												   ALLOCSET_DEFAULT_INITSIZE,
												   ALLOCSET_DEFAULT_MAXSIZE);
	faggstate->buffContext = AllocSetContextCreate(CurrentMemoryContext,
												   "buffContext",
												   ALLOCSET_DEFAULT_MINSIZE,
												   ALLOCSET_DEFAULT_INITSIZE,
												   ALLOCSET_DEFAULT_MAXSIZE);
	faggstate->puncContext = AllocSetContextCreate(CurrentMemoryContext,
												   "puncContext",
												   ALLOCSET_DEFAULT_MINSIZE,
												   ALLOCSET_DEFAULT_INITSIZE,
												   ALLOCSET_DEFAULT_MAXSIZE);
    faggstate->state = FAS_NO_WINDOW_EXPR;
	faggstate->pending = NIL;
	faggstate->swstate = NULL;

	node->faggstate = faggstate;

	oiht = NULL;
	
	return TRUE;
}

bool
ProcessEndFAgg(FAgg * node, Plan * parent)
{
	int i;

	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	for (i = 0; i < length(node->window_states); i++) {
		freeWindowState(lfirst(node->window_states));
		node->window_states = lnext(node->window_states);
	}

    /* Free the TupleDesc constraint created by fix_tdesc_timestampcol(),
     * if applicable. */
    if (NULL != node->output_tdesc)
    {
        FreeTupleDesc(node->output_tdesc);
        node->output_tdesc = NULL;
            /*
        if (NULL != node->output_tdesc->constr) 
        {
            pfree(node->output_tdesc->constr);
            node->output_tdesc->constr = NULL;
        }
        pfree(node->output_tdesc);
        */
    }

	MemoryContextDelete(node->faggstate->faggContext);
	MemoryContextDelete(node->faggstate->buffContext);

	FjordEnd(&node->fjord, eddy);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(FAgg));

	return TRUE;
}

/* ----------------------------------------------------------------
 *		ProcessNextFAggRangeSlide
 *
 *	See note at the top of this file for a detailed explanation.
 * ----------------------------------------------------------------
 */
static void
ProcessNextFAggRangeSlide(FAgg * node, Plan * parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	bool deqsucc;
	MemoryContext mcxttmp = NULL;


    FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */

    Assert(FAS_READING == node->faggstate->state 
            || FAS_RUNNING_AGG == node->faggstate->state
            || FAS_FINISHING_AGG == node->faggstate->state
            || FAS_PUNCTUATING == node->faggstate->state);

	/* 
	 *  Fetch next intermediate heap tuple from input queue.
	 *
	 *  NOTE:
	 *
	 *       Here we just manipulate pointers and modify the actual physical
	 *       Input IHT as necessary and then send on a pointer to the same IHT
	 */
	FJORD_DEQ_INP_PTR(deqsucc, node, &piiht);
	if (deqsucc) {
#ifdef DEBUG_FAGG
        elog(NOTICE, "FAgg: got input tuple");
        debugitup_lev(iiht, NOTICE);
		if (ihtIsPunct(iiht)) 
		{
			elog(LOG, "FAgg: got punct input tuple");
		}
#endif /* DEBUG_FAGG */

        /* The following takes care of steps 1-3 from the top of this file */
        add_new_tuple_range_slide(node, iiht, parent);
    } 
    
    /*
     * 4) If current_time > next_slide, node is ready to output aggregate 
     *      results.  Advance the windows by removing any tuples that 
     *      die at next_slide_time and adding any tuples that become live at 
     *      next_slide_time.
     *      Update last_slide_time, latest_slide_time and next_slide_time.  
     *
     * 5) Execute the aggregation tree on the current set of live tuples.
     *    NOTE: ExecFAgg() is used as a callback to pass the tuples up to the
     *    aggregation tree.
     *      
     *    Subsequent calls to ProcessNextFAgg() will return the elements of the 
     *    aggregate results (there may be multiple tuples due to GROUP BY).
     *    NOTE: These aggregate results are produced by pulling them from the
     *    aggregation tree, which in turn pulls its input tuples from the 
     *    ExecFAgg() callback!
     *
     */
    switch(node->faggstate->state) {

        case FAS_READING:
            /* We're reading tuples and NOT computing an aggregate value */

            mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
            { /* Begin temporary memory context. */
                if (ts_geq(node->current_time, node->next_slide_time)) {
                    /* Time to slide, and therefore time to output aggregate
                     * values. */
#ifdef DEBUG_FAGG
                    elog(LOG, "FAgg: Sliding (%s >= %s)",
                            ts_cstr(node->current_time), 
                            ts_cstr(node->next_slide_time));
#endif /* DEBUG_FAGG */
                    /* Set up the result of the wtime(*) aggregate */
                    node->wtime = node->next_slide_time;

                    /* Move our window forward and update the set of live
                     * tuples. */
                    advance_window(node);

                    node->faggstate->state = FAS_RUNNING_AGG;

                    /* Set up the iterator to the "live tuples" list so that we
                     * can run through the list sending tuples to the
                     * aggregation tree. */
                    node->live_tuples_cur = node->live_tuples_head;
                }
            } /* End temporary memory context. */
            MemoryContextSwitchTo(mcxttmp);

            if (node->faggstate->state != FAS_RUNNING_AGG) break;
                /* NOTE: We flow through to the next case on state transition 
                 * to FAS_RUNNING_AGG */

        case FAS_RUNNING_AGG:
        case FAS_FINISHING_AGG:

			FAggScheduleAggPlan(node,parent);
            break;

        case FAS_PUNCTUATING:
                /* We just sent the punctuation tuple.  Now we wrap up the
                 * aggregation and start reading input tuples again. */
                reset_agg_tree(node, parent);
                node->faggstate->state = FAS_READING;

            break;

        default:
            elog(FATAL, "FAgg: Invalid node state");
    }
         
}

/* ----------------------------------------------------------------
 *	FUNCTION   ProcessNextFAggSlices
 *
 *	See note at the top of this file for a detailed explanation.
 * ----------------------------------------------------------------
 */
static void
ProcessNextFAggSlices(FAgg * node, Plan * parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	bool 		 deqsucc   = false;
	FAggState 	*faggstate = node->faggstate;

    FJORD_AUTOVAR_DEFNS;		/* Common Fjord auto definitions */

    Assert(FAS_BEFORE_START == node->faggstate->state
		   || FAS_READING == node->faggstate->state 
		   || FAS_RUNNING_AGG == node->faggstate->state
		   || FAS_FINISHING_AGG == node->faggstate->state
		   || FAS_PUNCTUATING == node->faggstate->state);

#ifdef DEBUG_FAGG
	elog(LOG, "FAgg: pending list (%x), state (%d) ",
		 (uint) faggstate->pending, faggstate->state);
#endif
	
	/* 
	 *  Fetch next intermediate heap tuple from input queue
	 *
	 *  NOTE:
	 *
	 *       Here we just manipulate pointers and modify the actual physical
	 *       Input IHT as necessary and then send on a pointer to the same IHT
	 */
	FJORD_DEQ_INP_PTR(deqsucc, node, &piiht);
	if (deqsucc)
	{
#ifdef DEBUG_FAGG
		elog(NOTICE, "FAgg: got input tuple");
		debugitup_lev(iiht, NOTICE);
		if (ihtIsPunct(iiht)) 
		{
			elog(LOG, "FAgg: got punct input tuple");
		}
#endif /* DEBUG_FAGG */
		add_new_tuple_slices(node, iiht, parent);
	}
	
	switch (faggstate->state) 
	{
		case FAS_BEFORE_START:
		case FAS_READING:
			if ((!deqsucc) && (NIL != faggstate->pending))
			{
				add_new_tuple_slices(node, NULL, parent);
			}
			if (FAS_RUNNING_AGG != faggstate->state) break;
			
			/* NOTE: Flowing down to next case statement */
			
		case FAS_RUNNING_AGG:
		case FAS_FINISHING_AGG:
			
			FAggScheduleAggPlan(node,parent);
            break;

        case FAS_PUNCTUATING:
			/* We just sent the punctuation tuple.  Now we wrap up the
			 * aggregation and start reading input tuples again. */
            reset_agg_tree(node, parent);
			node->faggstate->state = FAS_READING;
			MemoryContextReset(node->faggstate->buffContext);
			node->live_tuples_head = NULL;
			node->live_tuples_tail = NULL;
			node->live_tuples_cur = NULL;

            break;

        default:
            elog(FATAL, "FAgg: Invalid node state");
    }
}

/* ----------------------------------------------------------------
 *	 ProcessNextFAgg
 *
 * Dispatches to either Slices version or Range-Slide version
 * ----------------------------------------------------------------
 */
void
ProcessNextFAgg(FAgg * node, Plan * parent)
{
	if (node->slicedAgg) 
	{
		ProcessNextFAggSlices(node,parent);
	}
	else 
	{
		ProcessNextFAggRangeSlide(node,parent);
	}
}

bool
ProcessRemoveQueryFAgg(FAgg * node, int queryid, Plan * parent)
{
	/* no special state here, so dispatch to the fjord version directly. */
	// **** need to add stuff to remove WindowState/WindowBuffer
	// elog(FATAL,"remove windowState/windowBuffer");
	return FjordRemoveQuery(&(node->fjord), queryid);
}

static void
ProcessPrintStatsFAgg_with_level(FAgg * node, Plan * parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	char *type;
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */


	type = (node->slicedAgg) ? "sliced" : "range-slide";
	
	elog(level, " FAgg (%s) ",
		 getNodeInformation(eddy, (Node *) node->aggPlan));

	elog(level, "FAgg(%s): Tuples entered: %d Tuples exited: %d",
		 type, node->total_entered, node->total_exited);
}

void
ProcessPrintStatsFAgg(FAgg * node, Plan * parent)
{
	ProcessPrintStatsFAgg_with_level(node, parent, INFO);
}


int
ExecCountSlotsFAgg(FAgg * node)
{
	return 0;					/* slots are dynamically allocated for fjord
								 * nodes */
}

/* ----------------------------------------------------------------
 *	ExecFAgg
 *
 *
 *	This is a callback used by our aggregation tree to fetch the
 *	tuples in the current window.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecFAgg(FAgg * node)
{
	TupleTableSlot *resultSlot =
		node->fjord.scanstate->cstate.cs_ResultTupleSlot;
	HeapTuple result;

    Assert(FAS_RUNNING_AGG == node->faggstate->state);

    /* Grab the next tuple. */
    if (NULL == node->live_tuples_cur) {
        /* We've read everything in the current window. */
#ifdef DEBUG_FAGG
        elog(NOTICE, "ExecFAgg done delivering tuples");
#endif /* DEBUG_FAGG */
        node->faggstate->state = FAS_FINISHING_AGG;
        return NULL;
    }

    /* TODO: Do we need to make a copy here?  The tuple pointed to by
     * live_tuples_cur->tuple is guaranteed not to be deleted until the last
     * aggregation result has been received! */
    Assert(NULL != node->live_tuples_cur->tuple);
	result = heap_copytuple(node->live_tuples_cur->tuple); 
    node->live_tuples_cur = node->live_tuples_cur->next;

#ifdef DEBUG_FAGG
    elog(NOTICE, "ExecFAgg delivering tuple :");
    debugtup_lev(result, node->input_tdesc, NULL, NOTICE);
#endif /* DEBUG_FAGG */

    ExecStoreTuple(result, resultSlot, InvalidBuffer, true);

    return resultSlot;

}

/* 
 * Auxiliary functions for the ExecFagg callback.
 */

bool
ExecInitFAgg(FAgg * node, EState * estate, Plan * parent)
{
	/* this is a leak, but is intentionally a leak becase the FE plan leaks
	 * tuple table slots like there is no tomorrow.  OC FIX this */
	Fjord *fjord = (Fjord *) & (node->fjord);

	node->fjord.scanstate->cstate.cs_ResultTupleSlot = MakeTupleTableSlot();
	InitSlotForEddy(fjord->scanstate->cstate.cs_ResultTupleSlot);
	ExecAssignResultTypeFromTL((Plan *) node,
							   &((node)->fjord.scanstate->cstate));
	ExecAssignProjectionInfo((Plan *) node,
							 &((node)->fjord.scanstate->cstate));
	return TRUE;
}

void
ExecReScanFAgg(FAgg * node, ExprContext * exprCtxt, Plan * parent)
{
	Assert(outerPlan(node) == NULL);
	Assert(innerPlan(node) == NULL);
}

void
ExecEndFAgg(FAgg * node)
{
}

/* FUNCTION FAggAddWindowExprs
 * ARGUMENTS: <windowExprs> is a list of WindowExpr objects
 * PRECONDITIONS: <node> is initialized (by ProcessInitFAgg()) and has not 
 *      had any window expressions added to it.
 * POSTCONDITIONS: Completes initialization by adding the window expressions
 *      and setting an initial window boundary.
 */
void FAggAddWindowExprs(FAgg *node, List *windowExprs) 
{
    List *ptr = NULL;

    Assert(NULL != node);
    Assert(FAS_NO_WINDOW_EXPR == node->faggstate->state);
    Assert(NULL == node->window_states);
    Assert(NULL != windowExprs);

	node->slicedAgg = FALSE;

#ifdef DEBUG_FAGG
	elog(LOG,"In FAggAddWindowExprs ... ");
#endif

    /* Add the window expressions... */
    foreach (ptr, windowExprs) {
        WindowExpr *we = (WindowExpr *)lfirst(ptr);
        WindowState *ws = createWindowState(we);

        if (NULL != node->input_tdesc) {
            windowStateSetTupleDesc(ws, node->input_tdesc);            
        }

		if (we->slices != NIL) 
		{
			node->slicedAgg = TRUE;
		}
		
        node->window_states = lcons((void*)ws, node->window_states);
    }

	if (node->slicedAgg) 
	{
		WindowState *ws = (WindowState *) lfirst(node->window_states);
		
		Assert(length(node->window_states) == 1);

		node->faggstate->state = FAS_BEFORE_START;
		node->faggstate->swstate = createSlicedWindowState(ws);
		node->next_slide_time = node->faggstate->swstate->next_edge;		
	}
	else 
	{
		/* Now we can compute the time of the first change. */
		TIMESTAMP_NOEND(node->next_slide_time);
		TIMESTAMP_NOEND(node->last_slide_time);
		TIMESTAMP_NOBEGIN(node->latest_slide_time);
		
		foreach (ptr, node->window_states) {
			WindowState *ws = (WindowState *)lfirst(ptr);
			
			node->next_slide_time = ts_min(node->next_slide_time,
										   windowStateGetFirstChange(ws));
			node->latest_slide_time = ts_max(node->next_slide_time,
											 windowStateGetFirstChange(ws));
			node->last_slide_time = ts_min(node->last_slide_time, ws->startAt);
			
			node->wtime = node->next_slide_time;
		}
#ifdef DEBUG_FAGG
		elog(LOG,"next: %s, last: %s, latest: %s \n",
			 ts_cstr(node->next_slide_time), ts_cstr(node->last_slide_time),
			 ts_cstr(node->latest_slide_time));
#endif	
		
		Assert(!TIMESTAMP_IS_NOEND(node->next_slide_time));
		Assert(!TIMESTAMP_IS_NOEND(node->last_slide_time));
		Assert(!TIMESTAMP_IS_NOBEGIN(node->latest_slide_time));
		/* We're ready to receive tuples now. */
		node->faggstate->state = FAS_READING;
	}
}


/* FUNCTION FAggSetTupleDesc
 * ARGUMENTS: <td> is a tuple descriptor for the tuples that this FAgg node
 *      will receive.
 * PRECONDITIONS: The node has not previously received any tuples.
 * POSTCONDITIONS: Updates the tuple descriptor for this node and for the
 * WindowState objects underneath it.
 * 
 * Makes copies of the tuple descriptor.
 */
void FAggSetTupleDesc(FAgg *node, TupleDesc td) {
    List *ptr = NULL;

    Assert(NULL == node->input_tdesc);

    node->input_tdesc = CreateTupleDescCopyConstr(td);

    foreach (ptr, node->window_states) {
        WindowState *ws = (WindowState *)lfirst(ptr);

        windowStateSetTupleDesc(ws, td);
    }
}


/*
 * INTERNAL FUNCTIONS
 */

/*
 * Note that this is the Range-Slide version of this function
 *
 *
 * Update our internal data structures to take into account a new tuple we've
 * just received:
 *
 * 1) Use the WindowStates to calculate when T will become live and when it
 *      will become dead.
 *
 * 2) File the tuple in the appropriate place as of last_slide_time:
 *    a) If the tuple is already dead, discard it and issue a warning.
 *    b) If the tuple is live, add it to the set of live windows and to the
 *      appropriate deactivation group.
 *    c) If the tuple is not yet live, add it to the appropriate activation and
 *      deactivation groups.
 *
 * 3) Update current_time if the timestamps in T cause it to change.  
 */
static void
add_new_tuple_range_slide(FAgg *node, IntermediateHeapTuple iiht, Plan *parent)
{
	MemoryContext       faggContext = node->faggstate->faggContext;
	MemoryContext 		mcxttmp 	= NULL;
	MemoryContext       ftmpcxt     = NULL;
    HeapTuple   		inputTuple;

	Timestamp 	 new_current_time = DT_NOEND;
	List 		*wsl;

	/* When does the current tuple become live? */
    Timestamp    birth_time 	  = DT_NOBEGIN;
    Timestamp    death_time 	  = DT_NOEND;


	/*
	 *  Note that most of this function executes in the node->fjord.tempContext
	 *  which gets reset when ProcessNextFAgg returns. As and when needed, we
	 *  switch to the longer lived faggContext that is managed by the FAgg.
	 */
	ftmpcxt = MemoryContextSwitchTo(node->fjord.tempContext);
	
    /* Make a copy, because we may buffer the tuple for longer than the 
     * memory context from which it came will last. */
    /* TODO: Is this copy really necessary? */
    mcxttmp = MemoryContextSwitchTo(faggContext);		   /* Beg agg mem cxt */
	inputTuple = EddyExecProject((Eddy *) parent, (IntermediateHeapTuple) iiht,
								 (int) lfirst(node->fjord.usedByQueries));
        
    MemoryContextSwitchTo(mcxttmp);						   /* End agg mem cxt */

    node->total_entered++;
    /*
     * 1) Use the WindowStates to calculate when T will become live and when it
     *      will become dead.
     */
    /* Our timestamp operations will allocate temporary Timestamp objects, so
     * we need to operate in a separate memory context to avoid leaking memory.
     */
	foreach(wsl, node->window_states)
	{
		WindowState *ws = (WindowState *) lfirst(wsl);
		Timestamp cur_birth_time, cur_death_time;

		/* The tuple becomes live when it is live for every base stream, so the
		 * start of the live time is equal to the maximum start time of the base
		 * streams and the end of the live time is equal to the minimum end
		 * time. */
		windowStateGetLifetime(ws, inputTuple,&cur_birth_time, &cur_death_time);

		birth_time = ts_max(birth_time, cur_birth_time);
		death_time = ts_min(death_time, cur_death_time);
	} 
#ifdef DEBUG_FAGG
    elog(LOG, "Birth at %s, death at %s",
                ts_cstr(birth_time), ts_cstr(death_time));
    elog(LOG, "    Last slide was %s; next is %s",
                ts_cstr(node->last_slide_time), ts_cstr(node->next_slide_time));
#endif /* DEBUG_FAGG */

    /* 2) File the tuple in the appropriate place as of last_slide_time:
     *    a) If the tuple is already dead, discard it and issue a warning.
     *    b) If the tuple is live, add it to the set of live windows and to the
     *      appropriate deactivation group.
     *    c) If the tuple is not yet live, add it to the appropriate 
     *      activation and deactivation groups.
     */
	if (TIMESTAMP_IS_NOEND(birth_time) || ts_geq(birth_time, death_time)) 
	{
		/* SPECIAL CASE: The tuple will never become alive, period. */
#ifdef DEBUG_FAGG
		elog(LOG, "FAgg: Got a windowless tuple.");
#endif /* DEBUG_FAGG */
		/* END SPECIAL CASE. */
	}
	else if (ts_leq(death_time, node->last_slide_time)
			 && !TIMESTAMP_IS_NOBEGIN(node->last_slide_time)) 
	{
		/* SPECIAL CASE: The tuple will never become alive, because its 
		 * window has already passed. */
#ifdef DEBUG_FAGG
		elog(NOTICE, "FAgg: Got a tuple whose window has passed.");
		elog(NOTICE, "FAgg:     %s < %s",
			 ts_cstr(death_time), ts_cstr(node->last_slide_time));
#endif /* DEBUG_FAGG */
		/* END SPECIAL CASE. */
	}
	else
	{
		/* COMMON CASE: The tuple will be live at some future slide time */

        /* NOTE: We used to add the tuple directly to the live tuples list if
         * possible, but that could screw up timestamp ordering.  So now we put
         * every tuple into an activation group.
         *
         * TODO: Figure out whether putting every tuple into an activation 
         * group represents a big performance hit.
         */
		TupleList *elem;
		
#ifdef DEBUG_FAGG
		elog(NOTICE, "FAgg: Adding tuple to activation group (%s)",
			 ts_cstr(death_time));
		elog(NOTICE, "FAgg: Adding tuple to deactivation group (%s)",
			 ts_cstr(death_time));
#endif /* DEBUG_FAGG */
		mcxttmp = MemoryContextSwitchTo(faggContext);
		elem = make_tuple_list_node(inputTuple);			
		add_to_activation_group(node, elem, birth_time);
		add_to_deactivation_group(node, elem, death_time);
		MemoryContextSwitchTo(mcxttmp);
	}

	/* 3) Update current_time if the timestamps in T cause it to change.  
     *      If current_time > next_slide_time, node is ready to output
     *      aggregate results.
     */
	foreach(wsl, node->window_states) 
	{
		WindowState *ws = (WindowState *) lfirst(wsl);
		
		windowStateUpdateMaxTS(ws, inputTuple);
		
		new_current_time = ts_min(new_current_time, windowStateGetMaxTS(ws));
	} 
	Assert(!TIMESTAMP_IS_NOEND(new_current_time));
	
	node->current_time = ts_max(node->current_time, new_current_time);
    MemoryContextSwitchTo(ftmpcxt);
    /* All our invariants should be restored now. */ 
}

/*
 * Note that this is the slices version of this function
 *
 *
 * Update our internal data structures to take into account a new tuple we've
 * just received:
 *
 * 1) If the tuple timestamp > next edge
 *       Mark the tuple "outstanding"
 *    else
 *       Add tuple to live list
 */
static void
add_new_tuple_slices(FAgg *node, IntermediateHeapTuple iiht, Plan *parent)
{
	MemoryContext        buffContext = node->faggstate->buffContext;
	MemoryContext        faggContext = node->faggstate->faggContext;
	MemoryContext        itupContext;
	MemoryContext 		 mcxttmp 	 = NULL;
    HeapTuple   		 inputTuple  = NULL;
	HeapTuple 			 ht 		 = NULL;
	bool                 inaggcxt    = false;
	FAggState           *faggstate 	 = node->faggstate;
	Timestamp 			 ts, pastEdge;
	WindowState 		*ws  		 = (WindowState *)(lfirst(node->window_states));
	SlicedWindowState 	*sws 		 = faggstate->swstate;
	TupleList 			*elem;

#ifdef DEBUG_FAGG
	elog(LOG,"add_new_tuple_slices: iiht: (%x), pending: (%x) ",
		 (uint) iiht, (uint) faggstate->pending);
#endif

	if (NULL != iiht)
	{
		/*
		 *  If there are _ANY_ pending tuples this tuple will have to be
		 *  appended to the pending list. So let it it be allocated in the
		 *  faggContext in the first place.
		 */ 
		itupContext = (NIL != faggstate->pending) ? faggContext : buffContext;
		
		mcxttmp = MemoryContextSwitchTo(itupContext);  /* Beg app mem context */
		inputTuple = EddyExecProject((Eddy *) parent,
									 (IntermediateHeapTuple) iiht,
									 (int) lfirst(node->fjord.usedByQueries));
		MemoryContextSwitchTo(mcxttmp);				   /* End app mem context */
#ifdef DEBUG_FAGG
		elog(LOG,"add_new_tuple_slices: (fc:%x), (bc:%x), (ic:%x)",
			 (uint) faggContext, (uint) buffContext, (uint) itupContext);
		elog(LOG,"add_new_tuple_slices: (after EEP) inputTuple: (%x)",
			 (uint) inputTuple);
#endif
		
		node->total_entered++;
		
		if (0 == (node->total_entered % 1000)) 
		{
			char *type = node->slicedAgg ? "sliced" : "agg";

			elog(LOG,"FAgg node (%s) has seen (%d) tuples ... ",
				 type,node->total_entered);
		}
	}
	else 
	{
		Assert(NIL != faggstate->pending);
	}
	if (NIL != faggstate->pending)
	{
		List 	*rest;
		List 	*first;

		first = faggstate->pending;
		rest  = lnext(first);

		if (NULL == inputTuple) 
		{
			faggstate->pending = rest;
		}
		else 
		{
			faggstate->pending = lappend(rest,inputTuple);
		}

		inputTuple = (HeapTuple) lfirst(first);
		Assert(inputTuple);
		pfree(first);
		
		inaggcxt = true;
	}
		
	/*
	 * 0) Compute the timestamp of the tuple
	 */
    /*
     * Begin temporary memory context.
	 *
	 *   Our timestamp operations will allocate temporary Timestamp objects, so
	 *   we need to operate in a separate memory context to avoid leaking
	 *   memory.
     */
	mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
#ifdef DEBUG_FAGG
	elog(LOG,"add_new_tuple_slices: inputTuple: (%x)", (uint) inputTuple);
#endif
	ts = windowStateGetTS(ws,inputTuple);
#ifdef DEBUG_FAGG
	elog(LOG,"add_new_tuple_slices: Tuple timestamp: (%s)", ts_cstr(ts));
#endif
	/*
	 * 1) Check if tuple timestamp > START AT
	 */
	if (FAS_BEFORE_START == faggstate->state)
	{
		if (! ts_geq(ts,ws->startAt))
		{
#ifdef DEBUG_FAGG
			elog(LOG,"add_new_tuple_slices: Still before START AT");
#endif		
			heap_freetuple(inputTuple);
		}
		else 
		{
#ifdef DEBUG_FAGG
			elog(LOG,"add_new_tuple_slices: Gone past START AT");
#endif		
			faggstate->state = FAS_READING;
		}
	}
    MemoryContextSwitchTo(mcxttmp);			  /* End temporary memory context */

	if (FAS_BEFORE_START == faggstate->state) 
	{
		Assert (NIL != faggstate->pending);
		return;
	}
    /*
     * 2) If the input tuple timestamp is greater than the next slice edge,
     * append it to the "pending" list and switch to FAS_RUNNING. Otherwise
     * append the tuple to the live list and continue in FAS_READING mode.
     */
	mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
	pastEdge = ts_geq(ts,sws->next_edge);
    MemoryContextSwitchTo(mcxttmp);			  /* End temporary memory context */
	if (pastEdge) 
	{
#ifdef DEBUG_FAGG
		elog(LOG,"ts: (%s), next_edge: (%s) ",
			 ts_cstr(ts), ts_cstr(sws->next_edge));
#endif
		node->wtime = sws->next_edge;
		/* Set up the iterator to the "live tuples" list so that we can run
		 * through the list sending tuples to the aggregation tree. */
		node->live_tuples_cur = node->live_tuples_head;
		faggstate->state = FAS_RUNNING_AGG;
		mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
		SlicedWindowAdvance(sws);
		MemoryContextSwitchTo(mcxttmp);		  
		node->next_slide_time = sws->next_edge;
#ifdef DEBUG_FAGG
			 elog(LOG,"New next_edge: (%s) ", ts_cstr(sws->next_edge));
#endif

		mcxttmp = MemoryContextSwitchTo(faggContext);	   /* Beg agg mem cxt */
		if (inaggcxt)			/* inputTuple was previously in pending list */
		{
#ifdef DEBUG_FAGG
			elog(LOG,"aggcontext: (%d) ", inaggcxt);
#endif
			/*
			 *  Here we have a pending tuple that has to go back to the pending
			 *  list. Ergo, there was at least one more slice edge before this
			 *  pending tuple.
			 */
			faggstate->pending = lcons(inputTuple,faggstate->pending);
		}
		else 
		{
#ifdef DEBUG_FAGG
			elog(LOG,"Appending to pending list");
#endif
			ht = heap_copytuple(inputTuple);
			faggstate->pending = lappend(faggstate->pending,ht);
			heap_freetuple(inputTuple);
		}
		MemoryContextSwitchTo(mcxttmp);					   /* End agg mem cxt */
	}
	else 
	{
		mcxttmp = MemoryContextSwitchTo(buffContext);  /* Beg buf mem context */
		if (inaggcxt)			 /* inputTuple was previously in pending list */
		{
			ht = inputTuple;
			inputTuple = heap_copytuple(ht);
			heap_freetuple(ht);
		}
		elem = make_tuple_list_node(inputTuple);
#ifdef DEBUG_FAGG
		elog(NOTICE, "FAgg: Adding tuple to live list.");
#endif /* DEBUG_FAGG */
		add_to_live_list(node, elem);
		MemoryContextSwitchTo(mcxttmp);				   /* End buf mem context */
    } 
	
    /* All our invariants should be restored now. */ 
}

/* Create a new tuple list node.  Make sure that you are in the proper memory
 * context before calling! */
TupleList *make_tuple_list_node(HeapTuple tup) {
    TupleList *ret = palloc(sizeof(*ret));

#ifdef DEBUG_FAGG
    elog(LOG, "FAgg:    Allocating TupleList entry %p", ret);
#endif /* DEBUG_FAGG */

    ret->tuple = tup;
    ret->next = NULL;
    ret->prev = NULL;

    return ret;
}

/* Functions for maintaining our particular tuple lists.  Make sure that these
 * are called from a long-lived memory context. */
ActivationGroup *get_group_for_ts(FAgg *node, bool activation, 
        Timestamp thetime) 
{
    MemoryContext mcxttmp;

    ActivationGroup *ltgroup = NULL;
        /* Latest group with timestamp less than ours. */
    ActivationGroup *geqgroup = NULL;
        /* Next group with a timestamp >= ours. */

    ActivationGroup *ourgroup = NULL;
        /* The group we're going to put this tuple into. */

    /* Find the proper place in the list of groups. */
    mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
    { /* Begin temporary memory context. */
        geqgroup = activation ? 
            node->activation_groups : node->deactivation_groups;

        while (NULL != geqgroup &&
                ts_greater_than(thetime, geqgroup->activation_time))
        {
            ltgroup = geqgroup;
            geqgroup = geqgroup->next;
        }

        if (NULL != geqgroup && 
                ts_equals(thetime, geqgroup->activation_time)) {
            /* The tuple goes into geqgroup. */
            ourgroup = geqgroup;
        } 
    } /* End temporary memory context. */
    MemoryContextSwitchTo(mcxttmp);

    if (NULL == ourgroup) {
        /* If we get here, then we need to make a new group for this tuple. */
#ifdef DEBUG_FAGG
        elog(LOG, "FAgg: Making a new %s group for %s",
                activation ? "activation" : "deactivation",
                ts_cstr(thetime));
#endif /* DEBUG_FAGG */

        ourgroup = palloc(sizeof(*ourgroup)); 
            /* Freed in advance_window(). */
        memset(ourgroup, 0x0, sizeof(*ourgroup));
            /* This memset() zeroes out the pointers in the union. */

        ourgroup->activation_time = thetime;
        ourgroup->next = geqgroup;

        ourgroup->tups_head = ourgroup->tups_tail = NULL;
            /* Also clears out the pointers for deactivation groups. */

        if (NULL == ltgroup) {
            if (activation) {
                node->activation_groups = ourgroup;
            } else {
                node->deactivation_groups = ourgroup;
            }
        } else {
            ltgroup->next = ourgroup;
        }
    }

    return ourgroup;
}

void add_to_activation_group(FAgg *node, TupleList *elem, Timestamp birth_time) 
{
    ActivationGroup *ourgroup = get_group_for_ts(node, true, birth_time);


    /* Add the tuple to the appropriate group.  We only need a singly-linked 
     * list, so we don't bother with the back pointers. */
    elem->next = NULL;
    if (ourgroup->tups_head == NULL) {
        ourgroup->tups_head = elem;
    } else {
        Assert(NULL != ourgroup->tups_tail);
        ourgroup->tups_tail->next = elem;
    }
    ourgroup->tups_tail = elem;

}

void add_to_deactivation_group(FAgg *node, TupleList *elem, 
        Timestamp death_time) 
{
    /* Similar to add_to_activation_group(), except that we build a list of
     * pointers into the live tuples list instead of a list of tuples. */
    ActivationGroup *ourgroup = get_group_for_ts(node, false, death_time);

    ourgroup->tups = lcons((void*)elem, ourgroup->tups);
}

void add_to_live_list(FAgg *node, TupleList *elem) {
#ifdef DEBUG_FAGG
    List *wsl;
    elog(NOTICE, "Adding tuple at %p to live list.  Timestamps are:",
            elem);
    foreach(wsl, node->window_states) {
        WindowState *ws = (WindowState *) lfirst(wsl);
        elog(NOTICE, "     %s", ts_cstr(windowStateGetTS(ws, elem->tuple)));
    }
#endif /* DEBUG_FAGG */

    /* We always insert at the end, in the vain hope that future aggregates
     * that depend on tuple ordering will still work. */
    if (NULL == node->live_tuples_head) {
        /* First tuple added */
        node->live_tuples_head = elem;
        elem->prev = NULL;
    } else {
        node->live_tuples_tail->next = elem;
        elem->prev = node->live_tuples_tail;
    }

    node->live_tuples_tail = elem;
    elem->next = NULL;

#ifdef DEBUG_FAGG
	{
		TupleList *tl = node->live_tuples_head;

		elog(LOG,"Timestamps in live tuples list are:");
		while (tl != NULL) 
		{
			foreach(wsl, node->window_states) {
				WindowState *ws = (WindowState *) lfirst(wsl);
				elog(NOTICE, "     %s", ts_cstr(windowStateGetTS(ws, tl->tuple)));
			}
			tl = tl->next;
		}
	}
#endif
}

/* N.B. Caller is responsible for freeing <elem> */
void remove_from_live_list(FAgg *node, TupleList *elem) {
    /* elem here is a pointer to an element of the live tuple list. */
#ifdef DEBUG_FAGG
    elog(NOTICE, "Removing tuple in %p from live list.",
            elem);
#endif /* DEBUG_FAGG */

    /* First fix up the head/tail. */
    if (node->live_tuples_head == elem) {
        node->live_tuples_head = elem->next;
    }

    if (node->live_tuples_tail == elem) {
        node->live_tuples_tail = elem->prev;
    }

    /* Then remove from the doubly-linked list. */
    if (NULL != elem->prev) {
        elem->prev->next = elem->next;
    }
    if (NULL != elem->next) {
        elem->next->prev = elem->prev;
    }

    /* Note that we do not do any pfree()ing! */
}


/* Function that advances the window once, updating all internal data
 * structures accordingly. */
void advance_window(FAgg *node) {


    /* The activation/deactivation times are kept in sorted order. */
    ActivationGroup *first_activation = node->activation_groups;
    ActivationGroup *first_deactivation = node->deactivation_groups;

    Timestamp new_slide;
    Timestamp new_latest_slide;

#ifdef DEBUG_FAGG
    elog(LOG, "In advance_window() %s -> %s",
            ts_cstr(node->last_slide_time), ts_cstr(node->next_slide_time));
    elog(LOG, "   first_activation = %p, first_deactivation = %p",
            first_activation, first_deactivation);
#endif /* DEBUG_FAGG */
    
    /* The window will slide at node->next_slide_time.  See if anything is
     * going to be added or removed at that time. */

    MemoryContext mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
    { /* Begin temporary memory context. */
        if (NULL != first_activation
                && !ts_equals(first_activation->activation_time,
                    node->next_slide_time)) {
#ifdef DEBUG_FAGG
            elog(LOG,
                "First activation is later (%s != %s)",
                    ts_cstr(first_activation->activation_time),
                    ts_cstr(node->next_slide_time));
#endif /* DEBUG_FAGG */

            Assert(ts_greater_than(first_activation->activation_time,
                        node->next_slide_time));

            first_activation = NULL;
        }
        if (NULL != first_deactivation
                && !ts_equals(first_deactivation->activation_time,
                    node->next_slide_time)) {
#ifdef DEBUG_FAGG
            elog(LOG,
                "First deactivation is later (%s != %s)",
                    ts_cstr(first_deactivation->activation_time),
                    ts_cstr(node->next_slide_time));
#endif /* DEBUG_FAGG */

            Assert(ts_greater_than(first_deactivation->activation_time,
                        node->next_slide_time));

            first_deactivation = NULL;
        }
    } /* End temporary memory context. */
    MemoryContextSwitchTo(mcxttmp);

    /* Remove expired tuples. */
    if (NULL != first_deactivation) {
        List *ptr = NULL;

#ifdef DEBUG_FAGG
        elog(LOG, "Removing tuples from first deactivation group (%s)",
                ts_cstr(first_deactivation->activation_time));
#endif /* DEBUG_FAGG */

        foreach (ptr, first_deactivation->tups) {
            TupleList *elem = (TupleList*)lfirst(ptr);
            remove_from_live_list(node, elem);
            /*pfree(elem);*/
			destroy_tuple_list_node(elem);
			
        }
        freeList(first_deactivation->tups);

        node->deactivation_groups = first_deactivation->next;
        pfree(first_deactivation);
    }

    /* Add new tuples. */
    if (NULL != first_activation) {
        TupleList *ptr = NULL;
        
#ifdef DEBUG_FAGG
        elog(LOG, "Adding tuples from first activation group (%s)",
                ts_cstr(first_activation->activation_time));
#endif /* DEBUG_FAGG */

        ptr = first_activation->tups_head;
        while (ptr != NULL) {
            /* add_to_live_list() will wipe out ptr->next, so save it. */
            TupleList *next_ptr = ptr->next;

            add_to_live_list(node, ptr);
            ptr = next_ptr;
        }

        /* No need to free the tuple list, because all the nodes are reused in
         * the live list. */
        node->activation_groups = first_activation->next;
        pfree(first_activation);
    }


    /* Determine the next slide time.  This may not correspond to any of
     * our activation/deactivation groups if there are no tuples between now
     * and then. */
    new_slide = DT_NOEND;
    new_latest_slide = DT_NOBEGIN;
    mcxttmp = MemoryContextSwitchTo(node->fjord.tempContext);
    { /* Begin temporary memory context. */
        List *wsl = NULL;
        foreach(wsl, node->window_states) {
            WindowState *ws = (WindowState *) lfirst(wsl);
            Timestamp nextslide = 
                    windowStateGetNextSlide(ws, node->next_slide_time);
        
            new_slide = ts_min(new_slide, nextslide);
            new_latest_slide = ts_max(new_latest_slide, nextslide);
        }
    } /* End temporary memory context. */
    MemoryContextSwitchTo(mcxttmp);

#ifdef DEBUG_FAGG
        elog(LOG, "Next slide after %s will be at %s",
                ts_cstr(node->next_slide_time), ts_cstr(new_slide));
#endif /* DEBUG_FAGG */

    /* Make sure that the windows actually advance */
    Assert(ts_greater_than(new_slide, node->next_slide_time));

    node->last_slide_time = node->next_slide_time;
    node->next_slide_time = new_slide;
    node->latest_slide_time = new_latest_slide;

}

/* Function that infers what the "timestamp" column of our aggregate result
 * tuples should be (assuming there is one!).  Puts the result into the
 * constraints of the input TupleDesc. 
 *
 * FIXME: This function is a big-time hack.  The user ought to be forced to
 * include a wtime(*) in every aggregate query.
 */
void fix_tdesc_timestampcol(TupleDesc desc) {
    int i;

    Assert(NULL != desc);

    /* If we already have a timestamp constraint, we don't need to do anything.
     * */
    if (NULL != desc->constr) return;

    /* Create the new constraint.  */
    desc->constr = palloc(sizeof(TupleConstr));
        /* pfreed in ProcessEndFAgg */ 

    memset(desc->constr, 0x0, sizeof(*(desc->constr)));
    desc->constr->ts_attrnum = -1;
        /* Start out assuming no timestamp column. */

    /* First look for a column called "tcqtime" */
    for (i = 0; i < desc->natts; i++) {
        if (0 == strncasecmp(desc->attrs[i]->attname.data, "tcqtime", 
                NAMEDATALEN)) {
            desc->constr->ts_attrnum = i; /* ts_attrnum is 0-based! */
            return;
        }
    }

    /* Then look for a column called "wtime" */
    for (i = 0; i < desc->natts; i++) {
        if (0 == strncasecmp(desc->attrs[i]->attname.data, "wtime", 
                NAMEDATALEN)) {
            desc->constr->ts_attrnum = i; /* ts_attrnum is 0-based! */
            return;
        }
    }

    /* If the previous steps don't work, look for the rightmost attribute that
     * is a timestamp. */
    for (i = desc->natts-1; i >= 0; i--) {
        if (TIMESTAMPOID == desc->attrs[i]->atttypid) {
            desc->constr->ts_attrnum = i; /* ts_attrnum is 0-based! */
            return;
        }
    }
}

static void
FAggScheduleAggPlan(FAgg *node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;
	
	bool 				 enqsucc 	 = false;
	TupleTableSlot 		*resultTTS 	 = NULL;
	FAggState           *faggstate   = node->faggstate;
	MemoryContext        faggContext = faggstate->faggContext;	
	MemoryContext        puncContext = faggstate->puncContext;	
	MemoryContext 		 mcxttmp 	 = NULL;
    bool                 have_result = false;

	FJORD_AUTOVAR_DEFNS;
	
	
	mcxttmp = MemoryContextSwitchTo(faggContext);		   /* Beg agg mem cxt */

	/* Set up execution state for wtime() and its transition function
	 * tcq_wtime() */
	tcqwtime = &(node->wtime);
	resultTTS = ExecProcNode(node->aggPlan, parent);
	tcqwtime = NULL;		  /* calling wtime at the wrong time is an  error */
	MemoryContextSwitchTo(mcxttmp);						   /* End agg mem cxt */

    /* Some operators indicate EOF by returning a null tuple; others use a null
     * TupleTableSlot.
     *
     * TODO: Make those silly operators consistent! */
    have_result = (NULL != resultTTS && NULL != resultTTS->val);
    
	if (have_result)
	{
        /* If we get here, we received a tuple from the aggregation operator
         * tree. */
		if (NULL == node->output_tdesc) 
		{
            /* We can't get the output tuple descriptor until we receive the
             * first tuple. */
			node->output_tdesc = 
			CreateTupleDescCopyConstr(resultTTS->ttc_tupleDescriptor);
			/* pfreed in ProcessEndFAgg */
			fix_tdesc_timestampcol(node->output_tdesc);
		}

		/* Got a tuple from the aggregation tree. */
#ifdef DEBUG_FAGG
		node->total_aggregated_this_window++;
#endif /* DEBUG_FAGG */

		/* convert to agg IHT if we use tuple lineage, have to set
		 * ready/done bits so the next module visited is the output.
		 * for now we have a hack in the output operator */
		mcxttmp = MemoryContextSwitchTo(faggContext);	   /* Beg agg mem cxt */
		initFromBaseTuple(oiht, resultTTS->val,node->output_tdesc, 0);
		oiht->isAggTuple = true;
		MemoryContextSwitchTo(mcxttmp);					   /* End agg mem cxt */

#ifdef DEBUG_FAGG
		elog(NOTICE, "FAgg: produced aggregate result tuple");
		debugitup_debug(oiht);
#endif /* DEBUG_FAGG */

		/* Send the tuple to our output queue. */
		FJORD_ENQ_OUT_PTR(enqsucc, node, &oiht);
		if (!enqsucc) elog(FATAL, "IHT did not enqueue");

		node->total_exited++;

	}
	else 
	{
		Assert(FAS_FINISHING_AGG == node->faggstate->state);
		/* We should have consumed the entire window before coming
		 * to the end of the aggregate results. */

		/* 
		 * 6) When all the aggregate results have been consumed, 
		 *      output a punctuation tuple (see Punctuation.html 
		 *      in the docs) to indicate that the aggregation is 
		 *      complete.
		 */
#ifdef DEBUG_FAGG
		elog(NOTICE, 
			 "FAgg: done fetching aggregate results: got %d tuples", 
			 node->total_aggregated_this_window);
		node->total_aggregated_this_window = 0;
#endif /* DEBUG_FAGG */

		/* SPECIAL CASE: We don't have an output tuple descriptor yet, because
         * we haven't received any output tuples.  We can't construct a 
         * punctuation tuple without a tuple descriptor. */
		if (NULL == node->output_tdesc)
		{
			elog(LOG, "***** FAgg couldn't generate punctuation");

            reset_agg_tree(node, parent);
			node->faggstate->state = FAS_READING;

			return;
		}
		/* END SPECIAL CASE */

		/* Use the same hack as above to force the eddy to send this
		 * tuple to the output. 
		 * TODO: Keep the following in sync with the lines approx. one
		 * screen up.
		 */
		Assert(NULL != node->output_tdesc);
		MemoryContextReset(puncContext);
		mcxttmp = MemoryContextSwitchTo(puncContext);	  /* Beg punc mem cxt */
		if (node->output_tdesc->constr->ts_attrnum > 0) 
		{
			/* Output tuples contain a timestamp. */
			faggstate->punct = form_punct_tup(node->output_tdesc,
											  node->next_slide_time);
			/* Note that the timestamp is that of the NEXT output. The
			 * punctuation indicates that there will be no tuples from this
			 * operator UNTIL then.*/
		}
		else 
		{
			/* Output tuples do not contain a timestamp. */
			faggstate->punct = form_punct_tup_no_ts(node->output_tdesc);
		}
		MemoryContextSwitchTo(mcxttmp);					  /* End punc mem cxt */
		initFromBaseTuple(oiht, faggstate->punct, node->output_tdesc, 0);
		oiht->isAggTuple = true;

		/* Send the punctuation tuple to our output queue. */
#ifdef DEBUG_FAGG
		elog(DEBUG1,"FAgg: FAgg produced punctuation tuple");
#endif
		FJORD_ENQ_OUT_PTR(enqsucc, node, &oiht);
		if (!enqsucc) elog(FATAL, "IHT did not enqueue");

		/* Punctuations do not count for the Eddy statistics. */

		node->faggstate->state = FAS_PUNCTUATING;
	}
}

void destroy_tuple_list_node(TupleList *node) 
{
    Assert(node != NULL);
    heap_freetuple(node->tuple);
    memset(node, 0xdeadbeef, sizeof(*node));
    pfree(node); 
}

/* Reset the internal state of our aggregation tree at the end of a window. */
void reset_agg_tree(FAgg *node, Plan *parent) {
    ExecReScan(node->aggPlan, NULL, parent);
}

/* Mark the nodes of the aggregate plan as being under the control of this
 * FAgg node. */
void mark_agg_tree(FAgg *node)
{
    Plan * curnode = node->aggPlan;
    while (NULL != curnode && curnode != (Plan*)node) {
#ifdef DEBUG_FAGG
        elog(LOG, "Marking node %p as under an FAgg", curnode); 
#endif /* DEBUG_FAGG */
        curnode->insideFAgg = true;
        curnode = curnode->lefttree;
    }
}


#ifdef DEBUG_FAGG
/* Convenience debugging function to get the string representation of a
 * timestamp. Leaks memory! */
static char * ts_cstr(Timestamp ts) {

    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out, TimestampGetDatum(ts)));
}
#endif /* DEBUG_FAGG */



