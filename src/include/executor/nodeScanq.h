/*-------------------------------------------------------------------------
 *
 * nodeScanq.h
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeScanq.h,v 1.3 2003/03/20 02:28:30 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESCANQ_H
#define NODESCANQ_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecScanQueue(ScanQueue * node);
extern bool ExecInitScanQueue(ScanQueue * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsScanQueue(ScanQueue * node);
extern void ExecEndScanQueue(ScanQueue * node);

#endif   /* NODESCANQ_H */
