/*-------------------------------------------------------------------------
 *
 * fluxplans.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/fluxplans.h,v 1.9 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef FLUXPLANS_H
#define FLUXPLANS_H

#include "nodes/plannodes.h"

/* Flux dataflows created by me. */
typedef enum fdflow_e
{
	FP_SINGLE = 0,
	FP_PAIR,
	FP_PAIR_NO_ACKS,
	FP_SINGLE_SOURCE,
	FP_PAIR_SOURCE_ALL,
	FP_PAIR_SCONS,
	FP_PAIR_SCONS_STDBY,
	FP_PAIR_FLUX,
	FP_PAIR_FLUX_STDBY,
	FP_SINGLE_SRC,
	FP_PAIR_SRC,
	FP_TWO_LEVEL,
	FP_TWO_LEVEL_STDBY,
	FP_TWO_GROUPS,
	FP_TWO_GROUPS_STDBY,
	FP_PAIR_SRC_CLICK,
	FP_THREE_GROUPS,
	FP_THREE_GROUPS_STDBY,
	FP_PAIR_SRC_TGROUPS,
	FP_PAIR_SRC_FGROUPS,
	FP_PAIR_SRC_WEBLOG,
	FP_TWO_GROUPS_EGRESS,
	FP_TWO_GROUPS_EGRESS_STDBY,
	FP_PAIR_SRC_AND_EGRESS_CLICK,
	FP_PAIR_SRC_EGRESS_LAT_CLICK,
	FP_TWO_GROUPS_EGRESS_LAT,
	FP_TWO_GROUPS_EGRESS_LAT_STDBY,
	DOUBLE_SRC_EGRESS_LAT_CLICK,
	DOUBLE_TWO_GROUPS_EGRESS,
	SINGLE_SRC_EGRESS_LAT_CLICK,
	SINGLE_TWO_GROUPS_EGRESS,
	FP_TOTAL_PLANS
} fdflow_e;

typedef struct fdflow_t
{
	char 		*name;
	fdflow_e 	 id;
	Plan * (*create_fplan_p) (FSched *f, Plan *plan, Query *querytree);
	bool         isStandby;
	bool         pushPlan;
} fdflow_t;

extern fdflow_t fdflow_strs[];
extern int fdflow_cmp(const void *, const void *);

#define InitFSchedPlan(f) \
        do \
        { \
	        create_state_mover(f); \
        } while (0)

#define EndFSchedPlan(f, topseg) \
        do \
        { \
            f->topSeg = topseg; \
            create_acceptor(f); \
        } while (0)

#endif /* FLUXPLANS_H */
