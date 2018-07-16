/*-------------------------------------------------------------------------
 *
 * CONVERTTOFSCHED_H
 *	  prototypes for routing functions.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: converttofsched.h,v 1.7 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef CONVERTTOFSCHED_H
#define CONVERTTOFSCHED_H

#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"
#include "executor/aggHash.h"

typedef unsigned long int ub4;	/* unsigned 4-byte quantities */
typedef unsigned char ub1;		/* unsigned 1-byte quantities */

/* Some routing functions you might want to use */
extern void hash_route_init(FluxProd * fp);
extern int	hash_route(FluxProd * fp, TupleTableSlot *slot);
extern void setup_hash_route(Plan * cons, Plan * prod, FluxProd * fp);

extern void round_robin_route_init(FluxProd * fp);
extern int	round_robin_route(FluxProd * fp, TupleTableSlot *slot);

extern int  fixed_mod_route(FluxProd *fp, TupleTableSlot *slot);

extern void  init_map_c2m(int **c2m, int size);

#ifdef NOT_USED
/* Hash function I grabbed from the web. */
extern ub4	hash(ub1 * k, ub4 length, ub4 initval);
#endif

extern uint32 hashFunc_multiple(Datum key, int typLen, bool byVal, uint32 prev_hash);

/* Used to calculate the hash of the grouping cols: <hcols> */
extern ub4	calc_composite_key_hash(hash_cols * hcols, TupleTableSlot *slot, ub4 seed);

#endif   /* CONVERTTOFSCHED_H */
