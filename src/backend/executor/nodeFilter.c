/*-------------------------------------------------------------------------
 *
 * nodeFilter.c
 *	  Routines to handle Filter operation
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFilter.c,v 1.13 2005/05/10 21:46:48 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/nodeFilter.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "telegraphcq/punct.h"

bool
ProcessInitFilter(Filter * node, EState *estate, Plan *parent)
{
	CommonScanState *scanstate;

	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create new CommonScanState for node
	 */
	scanstate = makeNode(CommonScanState);
	node->scanstate = scanstate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &scanstate->cstate);

#define FILTER_NSLOTS 0

	return TRUE;
}

bool
ProcessEndFilter(Filter * node, Plan *parent)
{
	ExecFreeExprContext(&node->scanstate->cstate);
	node->scanstate = NULL;
	return TRUE;
}

/* ----------------------------------------------------------------
 *		ProcessNextFilter
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ProcessNextFilter(Filter * node, TupleTableSlot *slot)
{
	CommonScanState *scanstate;
	EState	   *estate;
	ExprContext *econtext;
	Node	   *qual;
	Datum		expr_value;
	bool		isNull;
	bool		result;

	/*
	 * Fetch data from node
	 */
	estate = node->plan.state;
	scanstate = node->scanstate;
	econtext = scanstate->cstate.cs_ExprContext;
	qual = node->filterqual;

	/*
	 * place the current tuple into the expr context
	 */
	econtext->ecxt_scantuple = slot;

    /* SPECIAL CASE: We pass punctuation tuples through. */
    if (slot != NULL && slotContainsPunct(slot)) { 
        return slot; 
    }
    /* END SPECIAL CASE */

	/*
	 * check that the current tuple satisfies the qual-clause
	 */
	expr_value = ExecEvalExpr(qual, econtext, &isNull, NULL);

	result = (!isNull) && (DatumGetBool(expr_value));
	return (result ? slot : NULL);
}


/* ----------------------------------------------------------------
 *		ExecEvaluateFilter
 * ----------------------------------------------------------------
 */
bool
ExecEvaluateFilter(Filter *node, ExprContext *econtext)
{
    bool     isNull;
    Datum    expr_value;

    expr_value = ExecEvalExprSwitchContext(node->filterqual, econtext, &isNull, NULL);

	return (!isNull) && (DatumGetBool(expr_value));
}
