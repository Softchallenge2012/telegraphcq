/*-------------------------------------------------------------------------
 *
 * nodeScanModule.h
 *
 * This operator uses the new eddy "Fjord" interface as opposed to the
 * standard Iterator interface
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeScanModule.h,v 1.12 2005/06/13 02:59:29 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESCANMODULE_H
#define NODESCANMODULE_H

#include "nodes/plannodes.h"
#include "executor/intermediateTuple.h"



extern bool ProcessInitScanModule(ScanModule * node, EState *estate, Plan *parent);
extern void ProcessNextScanModule(ScanModule * node, Plan *parent);
extern bool ProcessEndScanModule(ScanModule * node, Plan *parent);
extern bool ProcessRemoveQueryScanModule(ScanModule * node, int32 queryid, Plan *parent);
extern void ProcessPrintStatsScanModule(ScanModule * node, Plan *parent);

extern void InsertRecursiveResults(ScanModule * node, HeapTuple inputTuple);
extern void ScanModuleDeliverProbeTuple(ScanModule * node,
							IntermediateHeapTuple iht);
extern bool IsScanModuleDone(ScanModule * s, bool isCancel);
extern void getNextTupleIntoSlot(ScanModule * s);
extern Timestamp lookAheadNextTupleTimestamp(ScanModule * node);
/* returns true if this is an end probe, or work needs to be done
   on behalf of this tuple.  false otherwise.
*/
extern bool updatePendingProbes(Plan *p, HeapTuple tuple,
					bool isDone);
extern Oid	getRealSourceOid(Plan *node);

#endif   /* NODESCANMODULE_H */
