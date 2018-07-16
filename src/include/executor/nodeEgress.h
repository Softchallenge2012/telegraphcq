/*-------------------------------------------------------------------------
 *
 * nodeEgress.h
 *
 *
 *
 * Portions Copyright (c) 2004, Regents of the University of California
 *
 * $Id: nodeEgress.h,v 1.1 2004/03/08 19:51:21 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEEGRESS_H
#define NODEEGRESS_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxCons_FT(FluxCons_FT * node);

extern bool ExecInitFluxCons_FT(FluxCons_FT * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxCons_FT(FluxCons_FT * node);
extern void ExecEndFluxCons_FT(FluxCons_FT * node);
extern void ExecFluxCons_FTAddConn(FluxCons_FT * node, int sockfd,
								   int node_id, int fprod_idx);

#endif   /* NODEEGRESS_H */
