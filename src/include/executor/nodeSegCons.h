/*-------------------------------------------------------------------------
 *
 * nodeSegCons.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeSegCons.h,v 1.3 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESEGCONS_H
#define NODESEGCONS_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecSegCons(SegCons * node);
extern bool ExecInitSegCons(SegCons * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsSegCons(SegCons * node);
extern void ExecEndSegCons(SegCons * node);
extern bool getResultFromConn(Plan *node, FluxConnInfo * fcinfo,
							  result * p_res, int conn_idx);

#endif   /* NODESEGCONS_H */
