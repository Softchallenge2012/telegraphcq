/*-------------------------------------------------------------------------
 *
 * nodeSegProd.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/executor/nodeSegProd.h,v 1.2 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESEGPROD_H
#define NODESEGPROD_H

#include "nodes/plannodes.h"

extern TupleTableSlot * ExecSegProd(SegProd * snode);
extern bool ExecInitSegProd(SegProd * node, EState *estate, Plan *parent);
extern int ExecCountSlotsSegProd(SegProd * node);
extern void ExecEndSegProd(SegProd * node);

extern bool ExecGetStateInitSegProd(SegProd * node);
extern bool ExecGetStateNextSegProd(SegProd * node, char *buf, int bsize);
extern bool ExecGetStateCloseSegProd(SegProd * node);
extern bool ExecInstallStateInitSegProd(SegProd * node);
extern bool ExecInstallStateNextSegProd(SegProd * node, char *buf, int bsize);
extern bool ExecInstallStateCloseSegProd(SegProd * node);
extern bool ExecResetStateSegProd(SegProd * node);

extern void init_conn_stat_SegProd(SegProd * node);

extern bool ready_to_copy_SegProd(SegProd *node);
extern void do_prod_sync_SegProd(SegProd *node);
extern void do_take_over_SegProd(SegProd *node);
extern void received_frunning_SegProd(FluxProd_FT *node, int part_no, bool mask);

#endif   /* NODESEGPROD_H */
