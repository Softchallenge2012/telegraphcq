/*-------------------------------------------------------------------------
 *
 * nodeWindow.h
 *	  prototypes for nodeWindow.c
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeWindow.h,v 1.5 2005/02/09 00:16:47 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEWINDOW_H
#define NODEWINDOW_H

/*@BwndaggWH */
#include "nodes/plannodes.h"
#include "nodes/execnodes.h"

extern TupleTableSlot *ExecWindow(Window *node);
extern bool ExecInitWindow(Window *node, EState *estate, Plan *parent);
extern int	ExecCountSlotsWindow(Window *node);
extern void ExecEndWindow(Window *node);
extern void ExecReScanWindow(Window *node, ExprContext *exprCtxt, Plan *parent);

extern void freeTupleList(TupleList * p);

/*@EwndaggWH */

#endif   /* NODEWINDOW_H */
