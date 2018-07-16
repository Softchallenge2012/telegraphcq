/*-------------------------------------------------------------------------
 *
 * nodeSteMHashjoin.h
 *
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeSteMHashjoin.h,v 1.2 2003/07/16 09:42:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESTEMHASHJOIN_H
#define NODESTEMHASHJOIN_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecSteMHashJoin(SteMHashJoin * node);
extern bool ExecInitSteMHashJoin(SteMHashJoin * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsSteMHashJoin(SteMHashJoin * node);
extern void ExecEndSteMHashJoin(SteMHashJoin * node);

#endif   /* NODEHASHJOIN_H */
