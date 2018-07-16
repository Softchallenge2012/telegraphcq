/*-------------------------------------------------------------------------
 *
 * nodeFluxEgress.h
 *
 *
 *
 * Portions Copyright (c) 2004, Regents of the University of California
 *
 * $Id: nodeFluxEgress.h,v 1.1 2004/03/08 19:51:21 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXEGRESS_H
#define NODEFLUXEGRESS_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxEgress(FluxEgress * node);
extern bool ExecInitFluxEgress(FluxEgress * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxEgress(FluxEgress * node);
extern void ExecEndFluxEgress(FluxEgress * node);

#endif   /* NODEEGRESS_H */
