/*-------------------------------------------------------------------------
 *
 * nodeSymHashJoin.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESYMHASHJOIN_H
#define NODESYMHASHJOIN_H

#include "nodes/plannodes.h"
#include "optimizer/eddy.h"

extern TupleTableSlot *ExecSymHashJoin(SymHashJoin *node);
extern bool ExecInitSymHashJoin(SymHashJoin *node, EState *estate, Plan *parent);
extern void ExecEndSymHashJoin(SymHashJoin *node);

extern TupleTableSlot * ExecSymHashJoinGetProbeResult(SymHashJoin *node);
void SymHashJoinReverseRoles(SymHashJoin *node, bool *isExhausted);
void ExecSymHashJoinSetNewProbe(SymHashJoin *node, TupleTableSlot *probe);

int ExecCountSlotsSymHashJoin(SymHashJoin *node);

#endif   /* NODESYMHASHJOIN_H */
