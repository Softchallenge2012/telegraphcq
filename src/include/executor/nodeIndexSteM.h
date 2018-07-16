/*-------------------------------------------------------------------------
 *
 * nodeSteM.h
 *
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 * $Id: nodeIndexSteM.h,v 1.2 2003/07/16 09:42:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef NODEINDEXSTEM_H
#define NODEINDEXSTEM_H

#include "nodes/plannodes.h"

TupleTableSlot *ExecIndexSteM(IndexSteM * node);

void		ExecIndexSteMSetNewOuter(IndexSteM * istem, ExprContext *econtext);

bool		ExecInitIndexSteM(IndexSteM * node, EState *estate, Plan *parent);

int			ExecCountSlotsIndexSteM(IndexSteM * node);

void		ExecEndIndexSteM(IndexSteM * node);

#endif   /* NODEINDEXSTEM_H */
