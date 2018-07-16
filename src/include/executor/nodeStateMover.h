/*-------------------------------------------------------------------------
 *
 * nodeStateMover.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeStateMover.h,v 1.5 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESTATEMOVER_H
#define NODESTATEMOVER_H

#include "nodes/plannodes.h"

#define REQ_QSIZE	100
#define STATE_QSIZE 20

extern TupleTableSlot *ExecStateMover(StateMover * node);
extern bool ExecInitStateMover(StateMover * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsStateMover(StateMover * node);
extern void ExecEndStateMover(StateMover * node);
extern void ExecStateMoverAddConn(StateMover * node,
								  int sockfd, struct sockaddr_in *addr,
								  int node_id, int sidx);

#endif   /* NODESTATEMOVER_H */
