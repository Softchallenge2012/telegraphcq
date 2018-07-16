/*-------------------------------------------------------------------------
 *
 * nodeFluxProd_FT.h
 *
 *
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/executor/nodeFluxProd_FT.h,v 1.13 2004/07/01 23:50:13 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFLUXPRODFT_H
#define NODEFLUXPRODFT_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFluxProd_FT(FluxProd_FT * node);
extern TupleTableSlot *ExecFluxProd_FT_old(FluxProd_FT * node);

extern bool ExecInitFluxProd_FT(FluxProd_FT * node, EState *estate, Plan *parent);

extern int	ExecCountSlotsFluxProd_FT(FluxProd_FT * node);

extern void ExecEndFluxProd_FT(FluxProd_FT * node);

extern bool process_acks_FluxProd_FT(FluxProdState_FT *fpstate,
									 FluxConnInfo *fcinfo,
									 int part_id,
									 int mask);

extern void make_connections_FluxProd_FT(FluxProd_FT * node);


extern bool ExecReadyForTakeOverFluxProd_FT(FluxProd_FT * node);

extern bool getNextTuple_FluxProd_FT(Plan *node, TupleTableSlot *rslot);

extern bool ready_to_copy_FluxProd_FT(FluxProd_FT *node);

extern bool ExecGetStateInitFluxProd_FT(FluxProd_FT * node);
extern bool ExecGetStateNextFluxProd_FT(FluxProd_FT * node,
										char *buf, int bsize);
extern bool ExecGetStateCloseFluxProd_FT(FluxProd_FT * node);

extern bool ExecInstallStateInitFluxProd_FT(FluxProd_FT * node);
extern bool ExecInstallStateNextFluxProd_FT(FluxProd_FT * node,
											char *page, int bsize);
extern bool ExecInstallStateCloseFluxProd_FT(FluxProd_FT * node);

extern bool ExecResetStateFluxProd_FT(FluxProd_FT * node);

/* Functions for serializing the TSB */

extern bool serTSBMetaDataG(FProdSMCB *smcb,
							TSB *tsb,
							TupleTableSlot *scanslot,
							int * fmasks,
							bool * copy_p_head,
							bool * copy_s_head,
							char * buf,
							int bsize,
							char * debug_name,
							FluxRole m_role);

extern bool serTSBTuples(FProdSMCB *smcb, TSB * tsb, int * fmasks,
						 char * buf, int bsize,
						 char * debug_name, FluxRole m_role);

extern bool deserTSBMetaDataG(FProdSMCB * smcb, TSB * tsb, int * fmasks,
							  TupleTableSlot *scanslot, bool flip_mask,
							  char * buf, int bsize,
							  char * debug_name, FluxRole m_role);

extern bool deserTSBTuples(FProdSMCB * smcb, TSB * tsb, bool flip_mask,
						   char * buf, int bsize,
						   char * debug_name, FluxRole m_role);

extern void adjustTSBPointers(FProdSMCB * smcb, TSB * tsb, int * fmasks,
							  char * debug_name, FluxRole m_role);

struct FProdSMCB
{
	bool copied_md;

	int *check_counts; /* For debugging purposes */
	int *counts; /* So we know when we are done */

	/* Sequence pointers */
	int *prod_ack;
	int *prim_ack;
	int *sec_ack;
	int *p_head;
	int *s_head;
	int *lents;

	int curr_part;
	TSB_Entry *cursor;
};

#endif   /* NODEFLUXPRODFT_H */
