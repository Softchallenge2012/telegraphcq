/*-------------------------------------------------------------------------
 *
 * nodeScanq.h
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeStrmScan.h,v 1.8 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESTRMSCAN_H
#define NODESTRMSCAN_H

#include "nodes/plannodes.h"



extern Oid GetRelOidStrmScan(StrmScan *node);
extern Index GetRTIndexStrmScan(StrmScan *node); 
extern CommonScanState *GetScanStateStrmScan(StrmScan *node);

extern void load_bindings_for_stream(Oid streamoid, StrmScan * strmscan);
extern TupleTableSlot *ExecStrmScan(StrmScan * node);
extern TupleTableSlot *ExecStrmScanBlocking(StrmScan * node);
extern bool ExecInitStrmScan(StrmScan * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsStrmScan(StrmScan * node);
extern void ExecEndStrmScan(StrmScan * node);
extern void ExecReScanStrmScan(StrmScan * node, ExprContext *exprCtxt,
				   Plan *parent);
extern ProjectionInfo *createProbeProjectionInfo(List *targetlist);
extern bool strmscan_has_var_bindings(StrmScan * s);
extern void StrmScanInitBindingInfo(StrmScan * node);
#endif   /* NODESTRMSCAN_H */
