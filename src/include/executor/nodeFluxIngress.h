/*-------------------------------------------------------------------------
 *
 * nodeFluxIngress.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/executor/nodeFluxIngress.h,v 1.3 2004/03/24 08:11:29 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXINGRESS_H
#define NODEFLUXINGRESS_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxIngress(FluxIngress * node);
extern bool ExecInitFluxIngress(FluxIngress * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxIngress(FluxIngress * node);
extern void ExecEndFluxIngress(FluxIngress * node);

#endif   /* NODEFLUXINGRESS_H */
