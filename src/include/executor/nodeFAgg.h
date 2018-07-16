/*-------------------------------------------------------------------------
 *
 * nodeFAgg.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFAgg.h,v 1.3 2005/02/09 00:16:47 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFAGG_H
#define NODEFAGG_H

#include "nodes/plannodes.h"

extern bool ProcessInitFAgg(FAgg * node, EState *estate, Plan *parent);
extern bool ProcessEndFAgg(FAgg * node, Plan *parent);
extern void ProcessNextFAgg(FAgg * node, Plan *parent);
extern bool ProcessRemoveQueryFAgg(FAgg * fagg, int queryid, Plan *parent);
extern void ProcessPrintStatsFAgg(FAgg * node, Plan *parent);

extern TupleTableSlot * ExecFAgg(FAgg *node);
extern bool ExecInitFAgg(FAgg *node, EState *estate, Plan *parent);
extern void ExecReScanFAgg(FAgg *node, ExprContext *exprCtxt, Plan *parent);
extern void ExecEndFAgg(FAgg *node);
extern int ExecCountSlotsFAgg(FAgg * node);

extern void FAggAddWindowExprs(FAgg *node, List *windowExprs);
extern void FAggSetTupleDesc(FAgg *node, TupleDesc td);


#endif   /* NODEFAGG_H */
