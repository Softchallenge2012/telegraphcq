/*-------------------------------------------------------------------------
 *
 * nodeFluxCons_FT.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeFluxCons_FT.h,v 1.10 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXCONSFT_H
#define NODEFLUXCONSFT_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxCons_FT(FluxCons_FT * node);
extern TupleTableSlot *ExecFluxCons_FT_old(FluxCons_FT * node);

extern bool ExecInitFluxCons_FT(FluxCons_FT * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxCons_FT(FluxCons_FT * node);
extern void ExecEndFluxCons_FT(FluxCons_FT * node);

extern void ExecFluxCons_FTAddConn(FluxCons_FT * node,
								   int sockfd, struct sockaddr_in *addr,
								   int node_id, int fprod_idx);

extern void ExecReScanFluxCons_FT(FluxCons_FT * node,
								  ExprContext *exprCtxt, Plan *parent);

extern void print_connections(FluxCons_FT * node);

extern bool sendAckToConn_FluxCons_FT(FluxCons_FT * node,
									  FluxConnInfo * fcinfo,
									  HeapTuple p_htup);

extern bool sendIntAckToConn_FluxCons_FT(FluxCons_FT * node,
										 FluxConnInfo * fcinfo,
										 int seqnum, bool flush);

extern bool process_acks_FluxCons_FT(FluxCons_FT * node,
									 FluxConnInfo * fcinfo);

extern bool ExecGetStateInitFluxCons_FT(FluxCons_FT * node);
extern bool ExecGetStateNextFluxCons_FT(FluxCons_FT * node,
										char *buf, int bsize);

extern bool ExecGetStateCloseFluxCons_FT(FluxCons_FT * node);

extern bool ExecInstallStateInitFluxCons_FT(FluxCons_FT * node);

extern bool ExecInstallStateNextFluxCons_FT(FluxCons_FT * node,
											char *page, int bsize);

extern bool ExecInstallStateCloseFluxCons_FT(FluxCons_FT * node);

extern bool ExecResetStateFluxCons_FT(FluxCons_FT * node);

typedef enum FOverStage /* Modes during fail-over */
{
	UNKNOWNFOS = (-1),
	ADJUST_CONN = 0,
	REVERSE_SCONN,   /* Wait for the connections to reverse */
	WAIT_FPRODSYNC,  /* Wait for failed producers to recover */
	SEND_PAUSE,      /* Send pause messages */
	WAIT_ACKS,       /* Wait for pause acks */
	SIGNAL_SMOVER,
	WAIT_SMOVER,
	WAIT_TUP_ACKS    /* Wait until tuple acks are sent */
} FOverStage;

typedef enum TOverStage /* Modes during take-over */
{
	INIT_SM = 0,
	MOVE_STATE,
	SEND_SYNC
} TOverStage;

#endif   /* NODEFLUXCONSFT_H */
