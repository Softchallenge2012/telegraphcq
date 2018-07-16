/*-------------------------------------------------------------------------
 * NOTE:
 * if you are tempted to edit this file, please be aware that it is currently
 * NOT USED. 
 *
 * nodeWindow.c
 *	  Routines to handle window nodes (used for aggregate queries with a window clause).
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * DESCRIPTION
 *
 *	  The Window node is designed for handling aggregate queries with a window
 *	  clause.  Its outer plan must deliver tuples such that tuples from the same
 *	  window are consecutive. That way, we just need to compare adjacent tuples
 *	  to locate window boundaries.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeWindow.c,v 1.12 2004/07/15 20:39:55 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeWindow.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"

/* ---------------------------------------
 *	 ExecWindow -
 *
 *		Window boundaries are marked with NULL TupleTableSlot.
 *		End of all windows is indicated by setting
 *		WindowState.wnd_done to TRUE.
 * ------------------------------------------
 */
TupleTableSlot *
ExecWindow(Window *node)
{
	WindowState *wndstate;
	HeapTuple	tuple;
	TupleTableSlot *outerslot;
	WindowBuffer *wndBuf;

	/*
	 * get state info from node
	 */
	wndstate = node->wndstate;
	if (wndstate->wnd_done)
		return NULL;

	wndBuf = wndstate->wnd_buffer;
	tuple = fetchFromWindowBuffer(wndBuf);
	if (tuple == NULL)
	{
		if (wndstate->input_done)
		{
			advanceWindowBuffer(wndBuf);
			if (wndBuf->tupleListHead == NULL)
				wndstate->wnd_done = TRUE;
			return NULL;
		}
		else
		{
			outerslot = ExecProcNode(outerPlan(node), (Plan *) node);
			if (TupIsNull(outerslot))
			{
				wndstate->input_done = TRUE;
				advanceWindowBuffer(wndBuf);
				return NULL;
			}
			tuple = outerslot->val;

			if (addTupleToWindowBuffer(wndBuf, tuple))
			{
				advanceWindowBuffer(wndBuf);
				return NULL;
			}
		}
		tuple = fetchFromWindowBuffer(wndBuf);
	}
	ExecStoreTuple(tuple,
				   wndstate->csstate.css_ScanTupleSlot,
				   InvalidBuffer, FALSE);

	return wndstate->csstate.css_ScanTupleSlot;
}

/* -----------------
 * ExecInitWindow
 *
 *	Creates the run-time information for the Window node produced by the
 *	planner and initializes its outer subtree
 * -----------------
 */
bool
ExecInitWindow(Window *node, EState *estate, Plan *parent)
{
	WindowState *wndstate;
	Plan	   *outerPlan;
	Oid			funcOid;

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create state structure
	 */
	wndstate = makeNode(WindowState);
	node->wndstate = wndstate;
	wndstate->wnd_done = FALSE;
	wndstate->input_done = FALSE;

	/*
	 * create expression context
	 */
	ExecAssignExprContext(estate, &wndstate->csstate.cstate);

#define WINDOW_NSLOTS 2

	/*
	 * tuple table initialization
	 */
	ExecInitScanTupleSlot(estate, &wndstate->csstate);
	ExecInitResultTupleSlot(estate, &wndstate->csstate.cstate);

	/*
	 * initializes child nodes
	 */
	outerPlan = outerPlan(node);
	ExecInitNode(outerPlan, estate, (Plan *) node);

	/*
	 * initialize tuple type.
	 */
	ExecAssignScanTypeFromOuterPlan((Plan *) node, &wndstate->csstate);

	/*
	 * Initialize tuple type for both result and scan. This node does no
	 * projection
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &wndstate->csstate.cstate);
	ExecAssignProjectionInfo((Plan *) node, &wndstate->csstate.cstate);


	funcOid = compatible_oper_funcid(makeList1(makeString("-")),
									 TIMESTAMPOID, TIMESTAMPOID, true);
	Assert(OidIsValid(funcOid));
	fmgr_info(funcOid, &wndstate->tsMinus);
	funcOid = compatible_oper_funcid(makeList1(makeString("=")),
									 TIMESTAMPOID, TIMESTAMPOID, true);
	Assert(OidIsValid(funcOid));
	fmgr_info(funcOid, &wndstate->tsEqual);
	funcOid = compatible_oper_funcid(makeList1(makeString("<")),
									 TIMESTAMPOID, TIMESTAMPOID, true);
	Assert(OidIsValid(funcOid));
	fmgr_info(funcOid, &wndstate->tsLt);
	funcOid = compatible_oper_funcid(makeList1(makeString("<")),
									 INTERVALOID, INTERVALOID, true);
	Assert(OidIsValid(funcOid));
	fmgr_info(funcOid, &wndstate->intervalLt);

	wndstate->wnd_buffer = createWindowBuffer(node->wndClause, wndstate);
	return TRUE;
}

int
ExecCountSlotsWindow(Window *node)
{
	return ExecCountSlotsNode(outerPlan(node)) + WINDOW_NSLOTS;
}

/* ------------------------
 *		ExecEndWindow(node)
 *
 * -----------------------
 */
void
ExecEndWindow(Window *node)
{
	WindowState *wndstate;
	Plan	   *outerPlan;

	wndstate = node->wndstate;

	ExecFreeProjectionInfo(&wndstate->csstate.cstate);
	ExecFreeExprContext(&wndstate->csstate.cstate);

	outerPlan = outerPlan(node);
	ExecEndNode(outerPlan, (Plan *) node);

	/* clean up tuple table */
	ExecClearTuple(wndstate->csstate.css_ScanTupleSlot);
	freeWindowBuffer(wndstate->wnd_buffer);
}

void
ExecReScanWindow(Window *node, ExprContext *exprCtxt, Plan *parent)
{
	WindowState *wndstate = node->wndstate;

	wndstate->wnd_done = FALSE;
	wndstate->input_done = FALSE;
	/* recreate WindowBuffer */
	freeWindowBuffer(wndstate->wnd_buffer);
	wndstate->wnd_buffer = createWindowBuffer(node->wndClause, wndstate);

	if (((Plan *) node)->lefttree &&
		((Plan *) node)->lefttree->chgParam == NULL)
		ExecReScan(((Plan *) node)->lefttree, exprCtxt, (Plan *) node);
}

/* create an empty WindowBuffer */
WindowBuffer *
createWindowBuffer(WindowClause * wndClause, WindowState * wndState)
{
	WindowBuffer *wndBuf = (WindowBuffer *) palloc(sizeof(WindowBuffer));

	wndBuf->tupleListHead = NULL;
	wndBuf->tupleListCurrent = NULL;
	wndBuf->tupleListTail = NULL;
	wndBuf->wndClause = wndClause;
	wndBuf->tsMinus = &wndState->tsMinus;
	wndBuf->tsEqual = &wndState->tsEqual;
	wndBuf->intervalLt = &wndState->intervalLt;
	wndBuf->tsLt = &wndState->tsLt;
	wndBuf->tupdesc = ExecGetScanType(&wndState->csstate);
	return wndBuf;
}

void
freeTupleList(TupleList * p)
{
	TupleList  *prev = NULL;

	while (p)
	{
		if (prev != NULL)
		{
			heap_freetuple(prev->tuple);
			pfree(prev);
			prev = NULL;
		}
		prev = p;
		p = p->next;
	}
	if (prev != NULL)
	{
		heap_freetuple(prev->tuple);
		pfree(prev);
		prev = NULL;
	}
}

void
freeWindowBuffer(WindowBuffer * wndBuf)
{
	if (wndBuf == NULL)
		return;
	freeTupleList(wndBuf->tupleListHead);
	pfree(wndBuf);
}

/* add a new tuple to WindowBuffer */
/* returns true if a window boundary is detected, false otherwise */
bool
addTupleToWindowBuffer(WindowBuffer * wndBuf, HeapTuple tuple)
{
	List	   *wl;
	TupleList  *newEntry = (TupleList *) palloc(sizeof(TupleList));
	TupleList  *oldEntry = NULL;
	bool		isNull = false;
	bool		windowEnd = FALSE;
	bool		skipTuple = FALSE;

	newEntry->tuple = heap_copytuple(tuple);
	newEntry->next = NULL;
	foreach(wl, wndBuf->wndClause->windowExprs)
	{
		WindowExpr *we = (WindowExpr *) lfirst(wl);
		Var		   *tsvar = we->tsvar;
		Datum		ts = heap_getattr(newEntry->tuple, tsvar->varattno,
									  wndBuf->tupdesc, &isNull);

		Assert(!isNull);
		if (wl == wndBuf->wndClause->windowExprs ||
		  DatumGetBool(FunctionCall2(wndBuf->tsLt, newEntry->maxTs, ts)))
			newEntry->maxTs = ts;
	}

	oldEntry = wndBuf->tupleListHead;
	if (oldEntry == NULL ||
		DatumGetBool(FunctionCall2(wndBuf->tsEqual, oldEntry->maxTs, newEntry->maxTs)))
		windowEnd = FALSE;
	else
	{
		bool		tupleOutOfOrder = FALSE;

		if (DatumGetBool(FunctionCall2(wndBuf->tsLt, newEntry->maxTs, oldEntry->maxTs)))
			tupleOutOfOrder = TRUE;
		foreach(wl, wndBuf->wndClause->windowExprs)
		{
			WindowExpr *we = (WindowExpr *) lfirst(wl);
			Var		   *tsvar = we->tsvar;
			Datum		ts = heap_getattr(oldEntry->tuple, tsvar->varattno,
										  wndBuf->tupdesc, &isNull);
			Datum		tsDelta;

			Assert(!isNull);
			tsDelta = FunctionCall2(wndBuf->tsMinus, newEntry->maxTs, ts);
			if (!DatumGetBool(FunctionCall2(wndBuf->intervalLt, tsDelta, we->rangeint->constvalue)))
			{
				if (tupleOutOfOrder)
					skipTuple = TRUE;
				else
					windowEnd = TRUE;
				break;
			}
		}
	}
	if (skipTuple)
	{
		heap_freetuple(newEntry->tuple);
		pfree(newEntry);
	}
	else
	{
		if (wndBuf->tupleListHead == NULL)
		{
			wndBuf->tupleListHead = wndBuf->tupleListCurrent =
				wndBuf->tupleListTail = newEntry;
		}
		else
		{
			wndBuf->tupleListTail->next = newEntry;
			wndBuf->tupleListTail = newEntry;
			if (wndBuf->tupleListCurrent == NULL)
				wndBuf->tupleListCurrent = newEntry;
		}
	}

	return windowEnd;
}

/* delete tuples that have fallen out of window */
/* move cursor to beginning of new window */
void
advanceWindowBuffer(WindowBuffer * wndBuf)
{
	TupleList  *prev = NULL,
			   *cur;

	for (cur = wndBuf->tupleListHead; cur != NULL; cur = cur->next)
	{
		if (prev != NULL &&
			!DatumGetBool(FunctionCall2(wndBuf->tsEqual,
										prev->maxTs, cur->maxTs)))
			break;
		if (prev != NULL)
		{
			heap_freetuple(prev->tuple);
			pfree(prev);
		}
		prev = cur;
	}

	if (prev != NULL)
	{
		heap_freetuple(prev->tuple);
		pfree(prev);
	}

	wndBuf->tupleListHead = cur;
	wndBuf->tupleListCurrent = cur;
}

/* fetch a tuple from WindowBuffer */
/* advance cursor */
/* returns NULL if WindowBuffer is empty */
HeapTuple
fetchFromWindowBuffer(WindowBuffer * wndBuf)
{
	HeapTuple	tuple;

	if (wndBuf->tupleListCurrent == NULL)
		return NULL;
	tuple = wndBuf->tupleListCurrent->tuple;
	wndBuf->tupleListCurrent = wndBuf->tupleListCurrent->next;
	return tuple;
}
