/*-------------------------------------------------------------------------
 *
 * nodeFluxAcceptor.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeFluxAcceptor.h,v 1.5 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXACCEPTOR_H
#define NODEFLUXACCEPTOR_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxAcceptor(FluxAcceptor * node);
extern bool ExecInitFluxAcceptor(FluxAcceptor * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxAcceptor(FluxAcceptor * node);
extern void ExecEndFluxAcceptor(FluxAcceptor * node);

extern void ExecInsertConnectionFluxAcceptor(FluxAcceptor * node, int sockfd,
								 int node_id, int to_idx, int from_idx);

#define PRIM_SEG_PROD (-74)
#define SEC_SEG_PROD (-76)

#endif   /* NODEFLUXACCEPTOR_H */
