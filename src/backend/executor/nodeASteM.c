/*-------------------------------------------------------------------------
 *
 * nodeASteM.c
 *	  New SteM operator that implements aging through windows.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "catalog/pg_type.h"
#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeASteM.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeSteM.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "parser/parse_node.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "optimizer/clauses.h"
#include "optimizer/var.h"

#include "optimizer/eddy.h"
#include "parser/parsetree.h"

/* Function to simplify selecting a substem. */
SteM ** ASTEM_SUBSTEM_PTR_PTR(ASteM * node, int index);
SteM ** ASTEM_SUBSTEM_PTR_PTR(ASteM * node, int index) 
{
	Assert(index >= 0);
	Assert(index < ASTEM_NUM_SUBSTEMS);
	return &((node)->substems[index].stem);
}

#define ASTEM_SUBSTEM_PTR(node, index) \
	 (*(ASTEM_SUBSTEM_PTR_PTR((node),(index))))


/* Prototypes for internal functions. */
bool ASteMIsInitialized(ASteM * node);
void ASteMReplaceWindow(Const * new_window_size,
						Const ** old_window_size_ptr);
void ASteMCreateEmptyWindowExprs(ASteM * node, Const * dummy_window_size);
void ASteMGCSubstems(ASteM * node, ExprContext * next_insert_tup);
void ASteMInsert(ASteM * node, ExprContext * econtext);
void ASteMMakeSpaceForInsert(ASteM * node, ExprContext * nexttuple);
bool ASteMSubSteMHasExpired(ASteM * node, int index,
							ExprContext * newest_tup);
bool ASteMTupleFitsInSubSteM(ASteM * node, int index, ExprContext * tup);
bool ASteMEvalWindowExpr(ASteM * node, IntermediateHeapTuple * older_tuple,
						 IntermediateHeapTuple * newer_tuple,
						 Expr * window_expr);
void ASteMCreateNewSubstem(ASteM * node, int index);

/* bool ASteMNewWindowIsLarger(ASteM * node, Const * new_window_size); */
static bool ASteMHasSubSteMs(ASteM *node);



/* ----------------------------------------------------------------
 *		ExecInitASteM
 * ----------------------------------------------------------------
 */
bool
ExecInitASteM(ASteM *node, EState *estate, Plan *parent)
{
	int i, source_id;
	ASteMState *astemstate;
	
	Assert(node != NULL);

	/* init & assign the ASteMState */
	astemstate = makeNode(ASteMState);
	node->astemstate = astemstate;

	/* let the plan know about the estate */
	node->plan.state = estate;	
	
	/* probe variable initialization */
	astemstate->haveprobe = false;

	/* get the ExprContext into the ASteM's commonstate */
	ExecAssignExprContext(estate, &astemstate->cstate);

	/* prepare result/probe tuple slots */
	ExecInitResultTupleSlot(estate, &astemstate->cstate);
	astemstate->cstate.cs_OuterTupleSlot = ExecInitExtraTupleSlot(estate);
	
	/* Prepare the SteM that we will copy to create the SteMs in
	 * node->substems[]. */
	node->protostem = makeNode(SteM);

	node->protostem->innerhashkey = node->innerhashkey;
	node->protostem->outerhashkey = node->outerhashkey;
	node->protostem->stemclauses = node->stemclauses;
	node->protostem->joinquals = ((List*)0);
	/* don't set the innerNode because otherwise strmscan gets initialized repeatedly */
	
	ExecInitNode((Plan *)(node->protostem), estate, (Plan *) node);
	ExecInitNode(outerPlan(node), estate, (Plan *)node);
		
	/* ASTEM_NUM_SUBSTEMS is defined in plannodes.h. */
	for (i = 0; i < ASTEM_NUM_SUBSTEMS; i++) {
		ASTEM_SUBSTEM_PTR(node, i) = NULL;
	}

	/* init rest of astemstate (book-keeping and window stuff) */
	astemstate->insert_ss_index = ASTEM_NOT_AN_INDEX;
	astemstate->probe_ss_index = ASTEM_NOT_AN_INDEX;
	astemstate->first_ss_index = ASTEM_NOT_AN_INDEX;

	astemstate->ts_accessor = NULL;
	astemstate->same_stem_expr = NULL;
	astemstate->window_expr_args = NULL;
	astemstate->ts_comp_expr = NULL;
	
	astemstate->window_expr_is_valid = false;

	astemstate->initialized = true;

	/* need to get a cstate out of the estate from above! */
	ASteMMakeSpaceForInsert(node,
							astemstate->cstate.cs_ExprContext);
	
	/* add windowing for the astem now that the node has been initialized */
	if (node->wc != NULL)
	{
		WindowClause *wc = node->wc;
		List *wl;
		WindowExpr *we;
				
		/* get source id for left stem */
		if (IsA(outerPlan(node), SeqScan) ||
			IsA(outerPlan(node), StrmScan)) 
		{
			source_id = sourceGetVarNo(outerPlan(node));
		}
		else if (IsA(outerPlan(node), SymHashJoin))
		{
			/*
			 * for now, do dumb windowing for stems in middle of plan: window on any source in the stem.
			 * not most efficient, but guarantees correct results
			 */
			source_id = sourceGetVarNo(outerPlan(innerPlan(outerPlan(node))));
		}
		else 
		{
			nodeDisplay(node);
			elog(ERROR, "asteminit: Don't know how to handle windows for this ASteM");
		}

		/* scan through windowExprs looking for matches, and add correct windowing to stems */			
		for (wl = (wc->windowExprs); wl != ((List*)0); wl = wl->next) 
		{
			we = lfirst(wl);
			
			if (equal(rt_fetch(source_id, estate->es_range_table),
					  rt_fetch(we->rtref->rtindex, estate->es_range_table)))
			{
				SetTimestampAccessorASteM(node, we->tsvar);
				AddWindowASteM(node, we->rangeint);
				break;
			}
		}			
	}
	
	/* all done, return TRUE */
	return TRUE;
}

bool
ExecEndASteM(ASteM * node)
{
	int i;
	int sourceID;
	ASteMState *astemstate = node->astemstate;
	    
	sourceID = STEM_SOURCE(node->protostem);

	Assert(node != NULL);
	Assert(true == astemstate->initialized);

	for (i = 0; i < ASTEM_NUM_SUBSTEMS; i++) {
		if (NULL != ASTEM_SUBSTEM_PTR(node, i)) {
			ExecEndSteM(ASTEM_SUBSTEM_PTR(node, i));
			pfree(ASTEM_SUBSTEM_PTR(node, i));
			ASTEM_SUBSTEM_PTR(node, i) = NULL;

		}
	}

	/* TODO: Destroy the operator trees attached to this ASteM. */

	ExecEndSteM(node->protostem);
	ExecEndNode(outerPlan(node), (Plan *)node);
	
	pfree(node->protostem);

	if (astemstate->ts_accessor) {
		pfree(astemstate->ts_accessor);
		astemstate->ts_accessor = NULL;
	}

	if (astemstate->same_stem_expr) {
		pfree(astemstate->same_stem_expr);
		astemstate->same_stem_expr = NULL;
	}

	if (astemstate->stem_expired_expr) {
		pfree(astemstate->stem_expired_expr);
		astemstate->stem_expired_expr = NULL;
	}

	if (astemstate->window_expr_args) {
		MemoryContextDelete(astemstate->window_expr_args->ecxt_per_tuple_memory);
		pfree(astemstate->window_expr_args);
		astemstate->window_expr_args = NULL;
	}

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(ASteM));

	return TRUE;
}

int
ExecCountSlotsASteM(ASteM * node)
{
	/*
	 * 1 - cs_ResultTupleSlot (latest result)
	 * 1 - cs_OuterTupleSlot (current probe)
	 */
	return ExecCountSlotsNode((Plan *)(outerPlan(node))) 
        + 3 
        + ((ASTEM_NUM_SUBSTEMS + 1) 
                * ExecCountSlotsNode((Plan*)(node->protostem)));
}

TupleTableSlot *
ExecASteM(ASteM * node)
{
	ASteMState *astemstate = node->astemstate;
	if (astemstate->haveprobe) 
	{
		/* we already have a probe, so return the next output tuple */
		return ExecASteMGetNextOutputTuple(node);
	}
	else 
	{
		/* we don't have a probe, so get a tuple, build it, and return it */
		return ExecASteMBuildTupleAuto(node);
	}
	
}

TupleTableSlot *
ExecASteMBuildTupleAuto(ASteM *node) 
{
	ASteMState *astemstate;
	TupleTableSlot *probe;
	
	Assert(node != NULL);

	astemstate = node->astemstate;
	probe = astemstate->cstate.cs_OuterTupleSlot;
	
	Assert(!astemstate->haveprobe);

	probe = ExecProcNode(outerPlan(node), (Plan*)node);
	
	return ExecASteMBuildTuple(node, probe);
}

TupleTableSlot *
ExecASteMBuildTuple(ASteM *node, TupleTableSlot *buildTuple) 
{
	ASteMState *astemstate = node->astemstate;
	ExprContext *econtext = astemstate->cstate.cs_ExprContext;


	if (TupIsNull(buildTuple)) 
	{
		return NULL;
	}
	else 
	{
		econtext->ecxt_scantuple = buildTuple;

		/* now do the real work... */
		ASteMInsert(node, econtext);

		return buildTuple;
	}

	Assert(0);
	return NULL;	
}

bool
ASteMHasProbe(ASteM *node)
{
	return (ASteMIsInitialized(node) && (node->astemstate->haveprobe));
}

bool
ExecASteMSetNewProbe(ASteM *node, ExprContext *newcontext) 
{
	int current_index;
	ASteMState *astemstate = node->astemstate;
	
	Assert(!astemstate->haveprobe);
		
	if (! ASteMHasSubSteMs(node))
		return false;

	/* first, set all the hashkey and stemclauses for all the subSteMs */
	current_index = astemstate->first_ss_index;
	do
	{
		SteM *curstem = ASTEM_SUBSTEM_PTR(node, current_index);
		current_index = ASTEM_NEXT_SS_INDEX(current_index);
		
		curstem->outerhashkey = node->outerhashkey;
		curstem->stemclauses = node->stemclauses;

		ExecSteMSetNewOuter(curstem, newcontext);
	} while (current_index != ASTEM_NEXT_SS_INDEX(astemstate->insert_ss_index));

	/* Reset the state that keeps track of where we are in the current probe. */
	astemstate->probe_ss_index = ASTEM_NOT_AN_INDEX;

	astemstate->haveprobe = true;
		
	return true;
}

TupleTableSlot *
ExecASteMGetNextOutputTuple(ASteM *node) 
{
	TupleTableSlot *result;
	ASteMState *astemstate = node->astemstate;
			
	Assert(node != NULL);
	
	if (!ASteMHasSubSteMs(node)) {
		astemstate->haveprobe = false;
		return NULL;
	}
	
	if (ASTEM_NOT_AN_INDEX == astemstate->probe_ss_index) 
	{
		astemstate->probe_ss_index = astemstate->first_ss_index;
	}

	result = ExecSteM(ASTEM_SUBSTEM_PTR(node, astemstate->probe_ss_index));

	if (TupIsNull(result)) {

		/* We've exhausted this subSteM, so move onto the next one, if any. */
		if (astemstate->probe_ss_index != astemstate->insert_ss_index) {
			/* Recall that node->insert_ss_index is the index of the most
			 * recent subSteM. */
			astemstate->probe_ss_index = ASTEM_NEXT_SS_INDEX(astemstate->probe_ss_index);

			/* Recurse to check the next substem. */
			return ExecASteMGetNextOutputTuple(node);
		} else {
			/* We've run out of subSteMs to check. */
			astemstate->probe_ss_index = ASTEM_NOT_AN_INDEX;
			astemstate->haveprobe = false;
		}
	}

	return result;
}

/*
 * ASTEM-SPECIFIC INTERFACE FUNCTIONS
 */

/* FUNCTION SetTimestampAccessorASteM
 * ARGUMENTS: <node> is an ASteM, <ts_accessor> is a Var node that fetches the
 *		timestamp field from tuples with the signature of this ASteM.
 * PRECONDITIONS: <node> is initialized, and <ts_accessor> points to an object
 *		that fetches the appropriate field.
 * POSTCONDITIONS: Makes a copy of the accessor for this ASteM's use, if the
 *		ASteM does not already have one.  This function is itempotent.
 */
void
SetTimestampAccessorASteM(ASteM * node, Var * ts_accessor)
{
	ASteMState *astemstate = node->astemstate;
	
	Assert(NULL != node);
	Assert(NULL != ts_accessor);
	Assert(true == astemstate->initialized);


	if (NULL != astemstate->ts_accessor) {
		/* Already got one! */
		return;
	} else {
		/* We make a deep copy, since the object pointed to by <ts_accessor>
		 * will probably disappear very soon. */
		astemstate->ts_accessor = copyObject(ts_accessor);
	}

}


/* FUNCTION AddWindowASteM
 * ARGUMENTS: <node> is an ASteM, and <window_size> is a constant that defines
 *		the window size, given as a string that encodes a relative time 
 *		interval (reltime).
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Increases the window size of <node> so that this interval
 *		encompasses <window_size>.
 */
void
AddWindowASteM(ASteM * node, Const * window_size)
{
	Expr *rhs;
	Const **window_size_ptr_ptr;
	ASteMState *astemstate = node->astemstate;
	
	Assert(NULL != node);
	Assert(NULL != window_size);
	Assert(true == astemstate->initialized);

	/* Make sure that we have the accessor that lets us reach the timestamp
	 * field of the type of tuple that we handle with this SteM. */
	if (NULL == astemstate->ts_accessor) {
		elog(ERROR, "(internal error) ASteM timestamp accessor not set");
		return;
	}

	/* If necessary, create the tree of operators that tells us whether two
	 * tuples should go in the same subSteM. */
	if (NULL == astemstate->same_stem_expr) {
		ASteMCreateEmptyWindowExprs(node, window_size);
	}

	/* skipping if statement, assume new window overrules */
	/* Check to see whether we need to change the window size. */
	/* if (ASteMNewWindowIsLarger(node, window_size)) */
	/*{ */
		elog(DEBUG1, "ASteM: Putting a new window size into place."); 

		/* 
		 * Updating node->same_stem_expr...
		 */

		/* rhs here is the "window_size / <num. substems - 1>" part of the
		 * expression" */
		rhs =
			(Expr *)
			DatumGetPointer(lfirst(lnext(astemstate->same_stem_expr->args)));
		window_size_ptr_ptr = (Const **)(&(rhs->args->elem.ptr_value));

		/*ASteMReplaceWindow(window_size, (Const **) (&lfirst(rhs->args)));*/
		ASteMReplaceWindow(window_size, window_size_ptr_ptr);

		/* 
		 * Updating node->stem_expired_expr...
		 */

		window_size_ptr_ptr = 
			(Const**)
			(&((lnext(astemstate->stem_expired_expr->args))->elem.ptr_value));
		/*
		rhs =
			(Expr *)
			DatumGetPointer(lfirst(lnext(node->stem_expired_expr->args)));*/
		ASteMReplaceWindow(window_size, window_size_ptr_ptr);
		/* } */

	/* The window expression should be valid now. */
	astemstate->window_expr_is_valid = true;

}

/* FUNCTION ClearWindowASteM
 * ARGUMENTS: <node> is an ASteM
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Removes the arguments for any window expression attached to 
 *		<node>.  Does NOT touch the timestamp accessor.
 */
void
ClearWindowASteM(ASteM * node)
{
	ASteMState *astemstate = node->astemstate;
	
	Assert(ASteMIsInitialized(node));

	astemstate->window_expr_is_valid = false;
}

/* FUNCTION GetRepresentativeSTeMASteM
 * ARGUMENTS: <node> points to an ASteM.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Returns a SteM that has the same properties (e.g. signatures
 * of tuples stored) as the SteMs that this ASteM uses to store tuples.
 */
SteM *
GetRepresentativeSteMASteM(ASteM * node)
{
	Assert(ASteMIsInitialized(node));

	return node->protostem;
}

/*
 * INTERNAL FUNCTIONS
 */

/* FUNCTION ASteMIsInitialized
 * ARGUMENTS: <node> is a pointer to an ASteM node.
 * POSTCONDITIONS: Returns TRUE if the node is initialized.
 */
bool ASteMIsInitialized(ASteM * node) 
{
	return (node != NULL && true == node->astemstate->initialized);
}

/* FUNCTION ASteMReplaceWindow
 * ARGUMENTS: <new_window_size> is a new window size, and <old_window_size_ptr>
 *		points to where an existing window size is stored.
 * PRECONDITIONS: <old_window_size_ptr> either is NULL or points to 
 *		something that can be removed using pfree().
 * POSTCONDITIONS: Removes the old window size and puts a copy of the new 
 *		one in its place.
 */
void
ASteMReplaceWindow(Const * new_window_size, Const ** old_window_size_ptr)
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

/* FUNCTION ASteMCreateWindowExprs
 * ARGUMENTS: <node> is an ASteM node, and <dummy_window_size> is a Const that
 * has the same type as the window sizes you're going to pass to the ASteM
 * later on.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Creates window expressions with a window size of zero
 *		and attaches these expressions to the <same_stem_expr> and
 *		<stem_expired_expr> members of <node>.  
 */
void
ASteMCreateEmptyWindowExprs(ASteM * node, Const * dummy_window_size)
{
	Var *first_ts_acc = NULL, *second_ts_acc = NULL;
	Expr *lhs = NULL, *rhs = NULL;
	Const *window_divisor = NULL;
	Expr *new_lhs = NULL;
	Const *new_rhs = NULL;
	Const *window_size_placeholder_1 = NULL;
	ASteMState *astemstate = node->astemstate;
	
	/* Types of variables in our expressions. */
	Oid INTEGER_TYPE = INT4OID /*, RELTIME_TYPE = TINTERVALOID */;

	Assert(NULL == astemstate->same_stem_expr);
	/* Prevent memory leaks. */

	/* The first window expression takes as input two timestamps (which we'll 
	 * call t1 and t2) and the time interval <window_size>, and returns 
	 * whether:
	 *      (t2 - t1) < (window_size / <number of substems - 1>)
	 */

	/* First, use node->ts_accessor to create accessors for the two
	 * timestamps.  */
	Assert(NULL != astemstate->ts_accessor);
	first_ts_acc = (Var *) copyObject(astemstate->ts_accessor);
	first_ts_acc->varno = ASTEM_OLDER_TUPLE_VARNO;
	second_ts_acc = (Var *) copyObject(astemstate->ts_accessor);
	second_ts_acc->varno = ASTEM_NEWER_TUPLE_VARNO;

	/* Now we can create the left hand side of the equation, e.g.  t2 - t1 */
	lhs = make_op(makeList1(makeString("-")), 
			(Node *) second_ts_acc, (Node *) first_ts_acc);

	replace_opid((Oper *) (lhs)->oper);
	/* Not sure what this call to replace_opid() does. */

	/* Create an integer constant for <number of substems - 1>. */
	window_divisor = makeConst(INTEGER_TYPE, sizeof(int), 
							   (Datum) (ASTEM_NUM_SUBSTEMS - 1), 
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
	astemstate->same_stem_expr = make_op(makeList1(makeString("<")), 
			(Node *) lhs, (Node *) rhs);
	replace_opid((Oper *) ((Expr *) astemstate->same_stem_expr)->oper);

	/* Done with <same_stem_expr>. */


	/* The second window expression is a simpler version of the first: (t2 -
	 * t1) > window_size NOTE: ">" instead of "<"... */
	/* Reuse the left hand side of the first expression. */
	new_lhs = copyObject(lhs);
	new_rhs = copyObject(dummy_window_size);

	astemstate->stem_expired_expr =
		make_op(makeList1(makeString(">")), (Node *) new_lhs, (Node *) new_rhs);
	replace_opid((Oper *) ((Expr *) astemstate->stem_expired_expr)->oper);

	/* Done with <stem_expired_expr>. */

	/* No window size yet! */
	astemstate->window_expr_is_valid = false;
}

/* FUNCTION ASteMGCSubstems
 * ARGUMENTS: <node> is an ASteM, <next_insert_tup> is the tuple for the next
 *		insert.
 * PRECONDITIONS: <node> is initialized.
 * POSTCONDITIONS: Garbage-collects SteMs whose tuples have expired, and
 * updates internal data structures accordingly.
 */
void
ASteMGCSubstems(ASteM * node, ExprContext * next_insert_tup)
{
	int new_first_index;
	ASteMState *astemstate = node->astemstate;
	
	/* SPECIAL CASE: If we don't know the window size, assume an infinite
	 * window. */
	if (!(astemstate->window_expr_is_valid)) {
		return;
	}


	/* SubSteMs should expire in order, so start with the oldest and move
	 * forward in time. */
	while (ASTEM_NOT_AN_INDEX != astemstate->first_ss_index
		   && true == ASteMSubSteMHasExpired(node, astemstate->first_ss_index,
											 next_insert_tup)) {

		/* The subSteM at node->first_ss_index has expired, so wipe it out. */
		ExecEndSteM(ASTEM_SUBSTEM_PTR(node, astemstate->first_ss_index));

		ASTEM_SUBSTEM_PTR(node, astemstate->first_ss_index) = NULL;
		/* node->substems[node->first_ss_index].firsttime = ASTEM_NOT_A_TIME; */
		/* node->substems[node->first_ss_index].lasttime = ASTEM_NOT_A_TIME; */

		/* insert_ss_index is the last index. */
		if (astemstate->insert_ss_index == astemstate->first_ss_index) {
			/* We just removed the last subSteM. */
#ifdef AMOLDEBUGON
			elog(DEBUG1, "AMOLCODE: %s %d removed the last substem\n",
				 __FILE__, __LINE__);
#endif
			astemstate->insert_ss_index = ASTEM_NOT_AN_INDEX;
			astemstate->probe_ss_index = ASTEM_NOT_AN_INDEX;
			astemstate->first_ss_index = ASTEM_NOT_AN_INDEX;
			astemstate->haveprobe = false;
		} else {
			/* We still have one or more active subSteMs.  Move on to the
			 * next subSteM in chronological order.  Don't forget that
			 * substems[] wraps around! */
			if (astemstate->first_ss_index == astemstate->insert_ss_index) {
				/* We only have one active subSteM. */
				new_first_index = ASTEM_NOT_AN_INDEX;
			} else {
				new_first_index = ASTEM_NEXT_SS_INDEX(astemstate->first_ss_index);
			}

			/* Pull the probe index forward, if necessary. */
			if (astemstate->probe_ss_index == astemstate->first_ss_index) {
				astemstate->probe_ss_index = new_first_index;
			}

			astemstate->first_ss_index = new_first_index;
		}
	}

}

/* FUNCTION ASteMInsert
 * ARGUMENTS: <node> is an ASteM, <econtext> contains a tuple.
 * PRECONDITIONS: <node> is initialized and <econtext> is valid.  <currenttime>
 *		is >= the highest timestamp of any tuple inserted thus far. 
 * POSTCONDITIONS: Inserts the tuple into the latest subSteM, making space if
 *		necessary.
 */
void
ASteMInsert(ASteM * node, ExprContext * econtext)
{
	ASteMState *astemstate = node->astemstate;
	
	/* We shouldn't be inserting if we're in the middle of a probe. */
	Assert(false == astemstate->haveprobe);

	/* Garbage-collect expired subSteMs. */
#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d\n", __FILE__, __LINE__);
#endif
	ASteMGCSubstems(node, econtext);


	ASteMMakeSpaceForInsert(node, econtext);


#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d\n", __FILE__, __LINE__);
#endif

	ExecSteMInsert(ASTEM_SUBSTEM_PTR(node, astemstate->insert_ss_index), econtext, true /* whether_to_copy */);

	/* Don't forget to update those timestamp values! */
	/* On second thought, forget it. */
	/* node->substems[node->insert_ss_index].lasttime = currenttime; */
}

/* FUNCTION ASteMMakeSpaceForInsert
 * ARGUMENTS: <node> is an ASteM, <nexttuple> points to an expression context
 *		containing the next tuple to insert, and hence its timestamp. 
 * PRECONDITIONS: The ASteM is initialized.
 * POSTCONDITIONS: Makes sure that there is space to perform a probe, and
 * updates insert_ss_index if necessary.
 */
void
ASteMMakeSpaceForInsert(ASteM * node, ExprContext * nexttuple)
{
	ASteMState *astemstate = node->astemstate;
	int new_substem_index;

	/* Is there an active subSteM to insert into? */
	if (ASTEM_NOT_AN_INDEX == astemstate->insert_ss_index) {
		/* No subSteMs exist, so create one. */
		ASteMCreateNewSubstem(node, 0);
		astemstate->insert_ss_index = astemstate->first_ss_index = 0;
	} else {

		/* There is at least one existing subSteM.  Is this subSteM valid for
		 * the time inverval containing the next tuple? */
		if (!ASteMTupleFitsInSubSteM(node, astemstate->insert_ss_index, nexttuple)) {
			/* The current active subSteM is no longer accepting tuples, so
			 * create a new subSteM, if possible. */

			new_substem_index = ASTEM_NEXT_SS_INDEX(astemstate->insert_ss_index);

			/* If there isn't any space to create a new subSteM (due to an
			 * increase in the window size), we need to continue using the
			 * current subSteM. */
			if (new_substem_index != astemstate->first_ss_index) {

				ASteMCreateNewSubstem(node, new_substem_index);

				astemstate->insert_ss_index = new_substem_index;
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

bool
ASteMSubSteMHasExpired(ASteM * node, int index, ExprContext * newest_tup)
{
	ASteMState *astemstate = node->astemstate;
	IntermediateHeapTuple *older_tuple;
	IntermediateHeapTuple *newer_tuple;
	bool ret;
	/* Will be returned. */


	Assert(NULL != ASTEM_SUBSTEM_PTR(node, index));

	Assert(astemstate->window_expr_is_valid);

	/* node->stem_expired_expr is an expression Node that, given two tuples
	 * encoded as INNER and OUTER (renamed here to ASTEM_OLDER_TUPLE_VARNO
	 * and ASTEM_NEWER_TUPLE_VARNO) arguments, returns TRUE if the two tuples 
	 * have timestamps that are more than <window_size> units of time apart. */

	/* We return <true> if the _newest_ tuple in the substem has expired. */

	/* First, we need to construct an expression context that contains the
	 * two tuples in question. */
	older_tuple = SteMGetNewestTuple(ASTEM_SUBSTEM_PTR(node, index));

	if (NULL == (*older_tuple)) {
		/* SubSteM is empty. */
		return false;
	}

	/*
	elog(DEBUG1, "ASteM: Older tuple is %p, which points to %p.",
			older_tuple, *older_tuple);
			*/

	/* Assert(tup->ecxt_scantuple->ttc_isIntermediateTuple); */
	newer_tuple =
		(IntermediateHeapTuple *) & (newest_tup->ecxt_scantuple->val);


	/* TODO: Make sure that this is the correct slot to use. */
	ret = ASteMEvalWindowExpr(node, older_tuple, newer_tuple,
							  astemstate->stem_expired_expr);

	if (true == ret) {
		elog(LOG, "ASteM(%p): Subtem %d has expired.", node, index);
	} else {
/*		elog(DEBUG1, "ASteM: Subtem %d has NOT expired.", index); */
	}

	return ret;

}

/* FUNCTION ASteMTupleFitsInSubSteM
 * ARGUMENTS: <node> is an ASteM node, <index> is an index into node->substems,
 *		<tup> holds a tuple that we're about to insert.
 * PRECONDITIONS: <node> is initialized, and the indicated index contains an
 *		active subSteM.
 * POSTCONDITIONS: Returns <true> if the timestamp of the indicated tuple is
 *		within the window of time that we're putting in the indicated subSteM.
 */
bool
ASteMTupleFitsInSubSteM(ASteM * node, int index, ExprContext * tup)
{
	ASteMState *astemstate = node->astemstate;
	IntermediateHeapTuple *older_tuple;
	IntermediateHeapTuple *newer_tuple;

	Assert(NULL != ASTEM_SUBSTEM_PTR(node, index));

	/* SPECIAL CASE: If there is no window expression, assume an infinite
	 * window. */
	if (!astemstate->window_expr_is_valid) {
		return true;
	}

	/* node->same_stem_expr is an expression Node that, given two tuples
	 * encoded as INNER and OUTER (renamed here to ASTEM_OLDER_TUPLE_VARNO
	 * and ASTEM_NEWER_TUPLE_VARNO) arguments, returns TRUE if the two
	 * tuples should be put into the same subSteM.  */

	/* First, we need to construct an expression context that contains the
	 * two tuples in question. */
	older_tuple = SteMGetOldestTuple(ASTEM_SUBSTEM_PTR(node, index));

	if (NULL == (*older_tuple)) {
		/* SubSteM is empty. */
		return true;	
	}

	/* Assert(tup->ecxt_scantuple->ttc_isIntermediateTuple); */
	newer_tuple = (IntermediateHeapTuple *) & (tup->ecxt_scantuple->val);
	/* TODO: Make sure that this is the correct slot to use. */

	return ASteMEvalWindowExpr(node, older_tuple, newer_tuple,
							   astemstate->same_stem_expr);
}

/* FUNCTION ASteMEvalWindowExpr
 * ARGUMENTS: <node> is an ASteM node, <older_tuple> and <newer_tuple> are 
 *		tuples with timestamp fields that <node->ts_accessor> can decode,
 *		and <window_expr> is a window expression.
 * PRECONDITIONS: The timestamp on <older_tuple> is older than the timestamp on
 *		<newer_tuple> (currently not checked!).
 * POSTCONDITIONS: Allocates node->window_expr_args if necessary and evaluates
 * the window expression.
 */
bool
ASteMEvalWindowExpr(ASteM * node, IntermediateHeapTuple * older_tuple,
					IntermediateHeapTuple * newer_tuple, Expr * window_expr)
{
	ASteMState *astemstate = node->astemstate;
	CommonState *scanstate;
	Datum expr_result;
	bool expr_is_null;
	ExprDoneCond expr_is_done;


	Assert(NULL != window_expr);

	/* Allocate the data structure if necessary. */
	if (NULL == astemstate->window_expr_args) {

		/* We need to create a separate memory context for evaluating this
		 * expression.  In order to create this memory context, we need to
		 * create a new CommonState Node. */
		scanstate = makeNode(CommonState);

		ExecAssignExprContext(node->plan.state, scanstate);

		/* Now we can grab the ExprState out of the CommonScanState we just
		 * created. */
		astemstate->window_expr_args = scanstate->cs_ExprContext;

		/* This expression context needs some TupleTableSlots to hold its
		 * arguments. */
		astemstate->window_expr_args->ecxt_innertuple = MakeTupleTableSlot();
		astemstate->window_expr_args->ecxt_outertuple = MakeTupleTableSlot();

		astemstate->window_expr_args->ecxt_innertuple->ttc_isIntermediateTuple
			= true;
		astemstate->window_expr_args->ecxt_outertuple->ttc_isIntermediateTuple
			= true;

	}
	ResetExprContext(astemstate->window_expr_args);

	/* Then fill it in... */
	astemstate->window_expr_args->ecxt_innertuple->val =
		*((HeapTuple *) older_tuple);
	astemstate->window_expr_args->ecxt_outertuple->val =
		*((HeapTuple *) newer_tuple);


	/* Now we've got the arguments marshalled up, so we can fire the
	 * expression. */
	expr_result = ExecEvalExprSwitchContext((Node *) (window_expr),
											astemstate->window_expr_args,
											&expr_is_null, &expr_is_done);


#ifdef AMOLDEBUGON
	elog(DEBUG1, "AMOLCODE: %s %d expr_result = %d\n", __FILE__, __LINE__,
		 expr_result);
#endif


	return (DatumGetBool(expr_result));

}

/* FUNCTION ASteMCreateNewSubstem
 * ARGUMENTS: <node> is an ASteM, <index> is an index into node->substems.
 * PRECONDITIONS: <node> is initialized and <index> does not currently contain
 *		a subSteM.
 * POSTCONDITIONS: Creates and initializes a new subSteM at the indicated
 *		index.
 */
void
ASteMCreateNewSubstem(ASteM * node, int index)
{
	Assert(index >= 0 && index < ASTEM_NUM_SUBSTEMS);

	/* Verify the invariant that no subSteM exists at this index already. */
	Assert(NULL == ASTEM_SUBSTEM_PTR(node, index));

	/* We copy the (empty) SteM that was passed to us on initialization when
	 * we create a new SteM. */
	ASTEM_SUBSTEM_PTR(node, index) = copyObject(node->protostem);

	ExecInitNode((Plan *)(ASTEM_SUBSTEM_PTR(node, index)), node->plan.state,
				 (Plan *) node);
}

/* FUNCTION ASteMNewWindowIsLarger
 * ARGUMENTS: <node> is an ASteM node, and <new_window_size> is a window size,
 *		encoded as a relative time interval in string format.
 * PRECONDITIONS: <node> is initialized and has a window expression (possibly
 *		with no window size).
 * POSTCONDITIONS: Returns <true> if <new_window_size> is larger than the
 *		existing window size of <node>.
 */

/* bool */
/* ASteMNewWindowIsLarger(ASteM * node, Const * new_window_size) */
/* { */
/* 	ASteMState *astemstate = node->astemstate; */
		
/* 	Expr *rhs = (Expr *) */
/* 		DatumGetPointer(lfirst(lnext(astemstate->same_stem_expr->args))); */
/* 	/\* rhs here is the "window_size / <num. substems - 1>" part of the window  */
/* 	 * expression" *\/ */

/* 	Const *old_window_size = (Const *) DatumGetPointer(lfirst(rhs->args)); */
/* 	Const *old_window_size_copy = NULL; */
/* 	Datum expr_result; */
/* 	bool expr_is_null; */
/* 	ExprDoneCond expr_is_done; */


/* 	/\* */
/* 	if (NULL == old_window_size) { */
/* 		elog(LOG, "ASteM: Old window size is NULL; new one is '%s'.", */
/* 				(char*)(new_window_size->constvalue)); */
/* 	} else { */
/* 		elog(LOG, "ASteM: Old window size is '%s'; new one is '%s'.", */
/* 			(char*)(old_window_size->constvalue), */
/* 			(char*)(new_window_size->constvalue)); */
/* 	} */
/* 	*\/ */

/* 	/\* First check to see whether <node> has a window at all. *\/ */
/* 	if (NULL == old_window_size) { */
/* 		return true; */
/* 	} else { */
/* 		/\* If we get here, <node> has an existing window, so we need to check */
/* 		 * whether this old window is a superset of the new window. *\/ */

/* 		/\* If necessary, create the expression that determines whether the */
/* 		 * old window size is large enough. *\/ */
/* 		if (NULL == astemstate->ts_comp_expr) { */
/* 			/\* The expression we're creating is: <new_window_size> > */
/* 			 * <old_window_size> *\/ */
/* 			old_window_size_copy = (Const *) copyObject(old_window_size); */
/* 			astemstate->ts_comp_expr = make_op(makeList1(makeString(">")),  */
/* 					(Node *) new_window_size, */
/* 					(Node *) old_window_size_copy); */
/* 			replace_opid((Oper *) (astemstate->ts_comp_expr)->oper); */
/* 		} else { */
/* 			/\* Put the new window size into place. *\/ */
/* 			lfirsti(astemstate->ts_comp_expr->args) */
/* 				= PointerGetDatum(new_window_size); */
/* 		} */

/* 		/\* Evaluate the expression. *\/ */
/* 		expr_result = ExecEvalExprSwitchContext((Node *) (astemstate->ts_comp_expr), */
/* 												node->plan.scanstate->cstate. */
/* 												cs_ExprContext, &expr_is_null, */
/* 												&expr_is_done); */


/* 		/\* We never made a copy of the new window size, so erase our pointer */
/* 		 * to it to avoid freeing it twice when destroying the ASteM. *\/ */
/* 		lfirst(astemstate->ts_comp_expr->args) = PointerGetDatum(NULL); */
/* 		/\* TODO: Is this the correct way to do this? *\/ */

/* 		return (DatumGetBool(expr_result)); */
/* 	} */
/* } */


/* FUNCTION ASteMHasSubSteMs
 * ARGUMENTS: <node> is an ASteM node
 * PRECONDITIONS: <node> is initialized
 * POSTCONDITIONS: Returns <true> if the ASteM <node> has substems already
 */
static bool
ASteMHasSubSteMs(ASteM *node) 
{
	return (node->astemstate->first_ss_index != ASTEM_NOT_AN_INDEX);
}

