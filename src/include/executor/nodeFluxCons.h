/*-------------------------------------------------------------------------
 *
 * nodeFluxCons.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeFluxCons.h,v 1.5 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXCONS_H
#define NODEFLUXCONS_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxCons(FluxCons * node);
extern bool ExecInitFluxCons(FluxCons * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxCons(FluxCons * node);
extern void ExecEndFluxCons(FluxCons * node);
extern void ExecFluxConsAddConn(FluxCons * node,
								int sockfd, struct sockaddr_in *addr,
								int node_id, int fprod_idx);

#endif   /* NODEFLUXCONS_H */
