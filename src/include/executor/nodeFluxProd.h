/*-------------------------------------------------------------------------
 *
 * nodeFluxProd.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/executor/nodeFluxProd.h,v 1.7 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXPROD_H
#define NODEFLUXPROD_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxProd(FluxProd * node);
extern bool ExecInitFluxProd(FluxProd * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFluxProd(FluxProd * node);
extern void ExecEndFluxProd(FluxProd * node);
extern int	write_b(int fd, char *buf, int size);
extern int make_remote_conn(FluxProd * node, int conn_no,
							FluxConnInfo * fcinfo, int to_idx);

#endif   /* NODEFLUXPROD_H */
