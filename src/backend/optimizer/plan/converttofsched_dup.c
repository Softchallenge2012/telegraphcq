/*
 * converttofsched_dup.c
 *
 *	  Routines to convert a plan into one run by the scheduler.
 *	  Eventually will break the plan into segments at the Flux points.
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/converttofsched_dup.c,v 1.31 2005/12/14 20:49:23 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include <assert.h>				/* @deddySK */

#include "access/heapam.h"		/* @dfluxMS */
#include "access/xlog.h"
#include "catalog/pg_class.h"	/* @dtcqpmMS */
#include "commands/async.h"
#include "commands/trigger.h"
#include "commands/variable.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/print.h"
#include "optimizer/clauses.h"
#include "optimizer/converttofsched.h"	/* @dfluxMS */
#include "optimizer/cost.h"
#include "optimizer/planner.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "parser/parsetree.h"	/* @dwchMS	 */
#include "rewrite/rewriteHandler.h"
#include "rqueue/ingresq.h"		/* @dtcqpmMS */
#include "rqueue/resultq.h"		/* @dfluxMS  */
#include "storage/proc.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "telegraphcq/fluxroutines.h"	/* @dfluxMS */
#include "telegraphcq/wrapch.h" /* @dtcqpmMS */
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "utils/relcache.h"		/* @dwchMS	 */
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

#include "pgstat.h"

#include "telegraphcq/fluxplans.h" /* For generating static plans */

#include "executor/nodeFluxAcceptor.h" /* For the SEG PROD HACK */

/* XXX Comparison function used in binary search */
int fdflow_cmp(const void *a, const void *b)
{
	/* a is the key, type (char *)  */
	/* b is the element, type (fdflow_t *) */
	return strcmp(((char *) a),
				  ((fdflow_t *) b)->name);
}

Plan	   *pg_convert_to_fsched_dup(Plan *plan, Query *querytree);

/* Uses the FT version of Flux */
static void insert_flux_into_plan_dup(Plan *plan, FSched * f);

/* Uses the FT version of Flux with dual plans */
static void insert_flux_into_plan_dup_better(Plan *plan, FSched * f, int mpair);

/* Connects to ingress operator */
static void insert_flux_into_plan_dup_all(Plan *plan, FSched * f);

/* Connects to ingress operator with dual plans */
static void insert_flux_into_plan_dup_better_all(Plan *plan, FSched * f, int mpair);

/* Routines that create plans */
static Plan *create_segcons_plan(FSched *f, Plan *plan, Query *querytree);
static Plan *create_pair_flux(FSched *f, Plan *plan, Query *querytree);
static Plan *create_two_level(FSched *f, Plan *plan, Query *querytree);
static StateMover *create_state_mover(FSched * f);
static FluxAcceptor *create_acceptor(FSched *f);

static Plan *create_two_groups_w_flux(FSched *f, Plan *plan, Query *querytree);
static Plan *create_source_plan_all(FSched * f, Plan *plan, Query *querytree);
static Plan *create_source_click(FSched * f, Plan *plan, Query *querytree);
static Plan *create_source_web_log(FSched * f, Plan *plan, Query *querytree);

static Plan *create_source_and_egress_click(FSched * f, Plan *plan, Query *querytree);

static Plan *create_source_and_egress_lat(FSched * f, Plan *plan, Query *querytree);

static Plan *create_three_groups_w_flux(FSched *f, Plan *plan, Query *querytree);

static Plan *create_two_groups_w_flux_and_egress(FSched *f, Plan *plan,
												 Query *querytree);

static Plan *create_two_groups_w_flux_and_egress_lat(FSched *f, Plan *plan,
													 Query *querytree);

static Plan *create_two_groups_w_flux_and_egress_lat_double(FSched *f, Plan *plan,
															Query *querytree);

static Plan *create_two_groups_w_flux_and_egress_lat_single(FSched *f, Plan *plan,
															Query *querytree);

/* XXX This array must be sorted */
fdflow_t fdflow_strs[] =
{
	{"DOUBLE_SRC_EGRESS_LAT_CLICK", DOUBLE_SRC_EGRESS_LAT_CLICK, create_source_and_egress_lat, 5, false},
	{"DOUBLE_TWO_GROUPS_EGRESS", DOUBLE_TWO_GROUPS_EGRESS, create_two_groups_w_flux_and_egress_lat_double, false, false},
	{"FP_PAIR", FP_PAIR, NULL, false, false},
	{"FP_PAIR_FLUX", FP_PAIR_FLUX, create_pair_flux, false, false},
	{"FP_PAIR_FLUX_STDBY", FP_PAIR_FLUX_STDBY, create_pair_flux, true, false},
	{"FP_PAIR_NO_ACKS", FP_PAIR_NO_ACKS, NULL, false, false},
	{"FP_PAIR_SCONS", FP_PAIR_SCONS, create_segcons_plan, false, false},
	{"FP_PAIR_SCONS_STDBY", FP_PAIR_SCONS_STDBY, create_segcons_plan, true, false},
	{"FP_PAIR_SOURCE_ALL", FP_PAIR_SOURCE_ALL, NULL, false, false},
	{"FP_PAIR_SRC", FP_PAIR_SRC, create_source_plan_all, 2, false},
	{"FP_PAIR_SRC_AND_EGRESS_CLICK", FP_PAIR_SRC_AND_EGRESS_CLICK, create_source_and_egress_click, 1, false},
	{"FP_PAIR_SRC_CLICK", FP_PAIR_SRC_CLICK, create_source_click, 1, false},
	{"FP_PAIR_SRC_EGRESS_LAT_CLICK", FP_PAIR_SRC_EGRESS_LAT_CLICK, create_source_and_egress_lat, 4, false},
	{"FP_PAIR_SRC_FGROUPS", FP_PAIR_SRC_CLICK, create_source_click, 3, false},
	{"FP_PAIR_SRC_TGROUPS", FP_PAIR_SRC_CLICK, create_source_click, 2, false},
	{"FP_PAIR_SRC_WEBLOG", FP_PAIR_SRC_WEBLOG, create_source_web_log, 0, false},
	{"FP_SINGLE", FP_SINGLE, NULL, false, false},
	{"FP_SINGLE_SOURCE", FP_SINGLE_SOURCE, NULL, false, false},
	{"FP_SINGLE_SRC", FP_SINGLE_SRC, create_source_plan_all, 0, false},
	{"FP_THREE_GROUPS", FP_THREE_GROUPS, create_three_groups_w_flux, false, false},
	{"FP_THREE_GROUPS_STDBY", FP_TWO_GROUPS_STDBY, create_three_groups_w_flux, true, false},
	{"FP_TWO_GROUPS", FP_TWO_GROUPS, create_two_groups_w_flux, false, false},
	{"FP_TWO_GROUPS_EGRESS", FP_TWO_GROUPS_EGRESS, create_two_groups_w_flux_and_egress, false, false},
	{"FP_TWO_GROUPS_EGRESS_LAT", FP_TWO_GROUPS_EGRESS_LAT, create_two_groups_w_flux_and_egress_lat, false, false},
	{"FP_TWO_GROUPS_EGRESS_LAT_STDBY", FP_TWO_GROUPS_EGRESS_LAT_STDBY, create_two_groups_w_flux_and_egress_lat, true, false},
	{"FP_TWO_GROUPS_EGRESS_STDBY", FP_TWO_GROUPS_EGRESS_STDBY, create_two_groups_w_flux_and_egress, true, false},
	{"FP_TWO_GROUPS_STDBY", FP_TWO_GROUPS_STDBY, create_two_groups_w_flux, true, false},
	{"FP_TWO_LEVEL", FP_TWO_LEVEL, create_two_level, false, false},
	{"FP_TWO_LEVEL_STDBY", FP_TWO_LEVEL_STDBY, create_two_level, true, false},
	{"SINGLE_SRC_EGRESS_LAT_CLICK", SINGLE_SRC_EGRESS_LAT_CLICK, create_source_and_egress_lat, 6, false},
	{"SINGLE_TWO_GROUPS_EGRESS", SINGLE_TWO_GROUPS_EGRESS, create_two_groups_w_flux_and_egress_lat_single, false, false},
};


#define INSERT_END_SEGS(fs, node) \
		insert_end_plan_item(&(fs->segs), &(fs->numSegs), \
							 &(fs->nsegs_ent), (Plan *) node)
#define INSERT_END_FLUXEN(fs, node) \
		insert_end_plan_item(&(fs->fluxen), &(fs->nfluxen), \
							 &(fs->nfluxen_ent), (Plan *) node)

/* Just expands the static array, returns the new array, destroys old. */
static Plan **
double_array(Plan **p_array, int *tot_size)
{
	int			alloc_size = ((*tot_size) == 0) ? 10 : ((*tot_size) * 2);
	Plan	  **n_array = palloc(alloc_size * sizeof(Plan *));

	memcpy(n_array, p_array, (*tot_size) * sizeof(Plan *));

	if (p_array != NULL)
		pfree(p_array);

	(*tot_size) = alloc_size;

	return n_array;
}

static int
insert_end_plan_item(Plan ***pp_array, int *size, int *tot_size, Plan *item)
{
	if (*size == *tot_size)
		(*pp_array) = double_array(*pp_array, tot_size);
	(*pp_array)[*size] = item;
	return (*size)++;
}

/* This creates the duplicated plan */
static void
insert_flux_into_plan_dup_better(Plan *plan, FSched * f, int mpair)
{
	hash_cols  *hcols;
	FluxCons   *fcons_p,
			   *fcons_s;
	FluxProd   *fprod_p,
			   *fprod_s;
	FluxCons_FT *fcons_ft_p,
			   *fcons_ft_s;
	FluxProd_FT *fprod_ft_p,
			   *fprod_ft_s;
	StrmScan   *ascan_p,
			   *ascan_s;
	Queue	   *local_q_pp,
			   *local_q_ps,
			   *local_q_sp,
			   *local_q_ss;
	Queue	   *local_r_pp,
			   *local_r_ps,
			   *local_r_sp,
			   *local_r_ss;
	HashGroup  *hashgrp_p,
			   *hashgrp_s;
	FluxAcceptor *faccept;

	f->kill_fsched = false;
	f->numSegs = 5;
	f->nfluxen = f->numSegs - 1;

	f->segs = (Plan **) palloc(sizeof(Plan *) * (f->numSegs));
	f->fluxen = (Plan **) palloc(sizeof(Plan *) * (f->nfluxen));

	/* Right now, there is just a hash-group and a scan */

	/* Creating the primary nodes */
	fcons_ft_p = makeNode(FluxCons_FT);
	fprod_ft_p = makeNode(FluxProd_FT);
	fcons_p = (FluxCons *) fcons_ft_p;
	fprod_p = (FluxProd *) fprod_ft_p;
	ascan_p = (StrmScan *) outerPlan(plan);
	hashgrp_p = (HashGroup *) plan;

	/* Look out for the error condition */
	if (!IsA(ascan_p, StrmScan) ||
		!IsA(hashgrp_p, HashGroup))
	{
		elog(DEBUG1, "Cannot convert this to FSched plan yet");
		f->numSegs = 0;
		return;
	}

	/* Creating the secondary nodes */
	fcons_ft_s = makeNode(FluxCons_FT);
	fprod_ft_s = makeNode(FluxProd_FT);
	fcons_s = (FluxCons *) fcons_ft_s;
	fprod_s = (FluxProd *) fprod_ft_s;
	ascan_s = (StrmScan *) copyObject(ascan_p);
	hashgrp_s = (HashGroup *) copyObject(hashgrp_p);

	if (mpair == 1)
	{

		fcons_ft_p->isPair = true;
		fprod_ft_p->isPair = true;
		fcons_ft_s->isPair = true;
		fprod_ft_s->isPair = true;

	}
	else
	{

		fcons_ft_p->isPair = false;
		fprod_ft_p->isPair = false;
		fcons_ft_s->isPair = false;
		fprod_ft_s->isPair = false;

	}

	/* Set the scheduler */
	fcons_ft_p->fcons.sched = fcons_ft_s->fcons.sched = f;

	faccept = makeNode(FluxAcceptor);
	f->faccept = faccept;

	fcons_p->plan.targetlist = ascan_p->scansock.plan.targetlist;
	fprod_p->plan.targetlist = ascan_p->scansock.plan.targetlist;

	fcons_s->plan.targetlist = ascan_s->scansock.plan.targetlist;
	fprod_s->plan.targetlist = ascan_s->scansock.plan.targetlist;

	fcons_p->tupDesc = CreateTupleDescCopy((ascan_p->scansock).tupDesc);
	fcons_s->tupDesc = CreateTupleDescCopy((ascan_s->scansock).tupDesc);

	outerPlan(hashgrp_p) = (Plan *) fcons_p;
	outerPlan(hashgrp_s) = (Plan *) fcons_s;
	parentPlan(fcons_p) = (Plan *) hashgrp_p;
	parentPlan(fcons_s) = (Plan *) hashgrp_s;

	outerPlan(fprod_p) = (Plan *) ascan_p;
	outerPlan(fprod_s) = (Plan *) ascan_s;
	parentPlan(ascan_p) = (Plan *) fprod_p;
	parentPlan(ascan_s) = (Plan *) fprod_s;

	/*
	 * Four types of local queues: Producer-Consumer pp: primary-primary.
	 * (forward) ps: primary-secondary.  (acks) sp: secondary-primary.
	 * (acks) ss: secondary-secodary. (forward)
	 */

	make_queue(&local_q_pp, *p_no_lock, flux_queue_size);
	make_queue(&local_q_ps, *p_no_lock, flux_queue_size);
	make_queue(&local_q_sp, *p_no_lock, flux_queue_size);
	make_queue(&local_q_ss, *p_no_lock, flux_queue_size);
	make_queue(&local_r_pp, *p_no_lock, flux_queue_size);
	make_queue(&local_r_ps, *p_no_lock, flux_queue_size);
	make_queue(&local_r_sp, *p_no_lock, flux_queue_size);
	make_queue(&local_r_ss, *p_no_lock, flux_queue_size);

	fprod_p->local_q = local_q_pp;
	fcons_p->local_q = local_q_pp;

	fprod_s->local_q = local_q_ss;
	fcons_s->local_q = local_q_ss;

	fprod_ft_p->local_r = local_r_pp;
	fcons_ft_p->local_r = local_r_pp;

	fprod_ft_s->local_r = local_r_ss;
	fcons_ft_s->local_r = local_r_ss;

	fprod_ft_p->local_r_s = local_r_ps;
	fcons_ft_s->local_r_s = local_r_ps;

	fprod_ft_p->local_q_s = local_q_ps;
	fcons_ft_s->local_q_s = local_q_ps;

	fprod_ft_s->local_r_s = local_r_sp;
	fcons_ft_p->local_r_s = local_r_sp;

	fprod_ft_s->local_q_s = local_q_sp;
	fcons_ft_p->local_q_s = local_q_sp;

	/* Wow, that connection setup was incredibly confusing! */

	fcons_p->all_done = false;
	fprod_p->all_done = false;

	fcons_s->all_done = false;
	fprod_s->all_done = false;

	fcons_p->nProd = total_machines;
	fprod_p->nCons = total_machines;

	init_map_c2m(&(fcons_p->map_c2m), fcons_p->nProd);
	init_map_c2m(&(fprod_p->map_c2m), fprod_p->nCons);
	init_map_c2m(&(fcons_p->map_m2c), fcons_p->nProd);
	init_map_c2m(&(fprod_p->map_m2c), fprod_p->nCons);

	fcons_s->nProd = total_machines;
	fprod_s->nCons = total_machines;

	init_map_c2m(&(fcons_s->map_c2m), fcons_s->nProd);
	init_map_c2m(&(fprod_s->map_c2m), fprod_s->nCons);
	init_map_c2m(&(fcons_s->map_m2c), fcons_s->nProd);
	init_map_c2m(&(fprod_s->map_m2c), fprod_s->nCons);

	fprod_p->route_fun_init = hash_route_init;
	fprod_p->route_fun = hash_route;
	fprod_p->rstate = palloc(sizeof(hash_cols));

	fprod_s->route_fun_init = hash_route_init;
	fprod_s->route_fun = hash_route;
	fprod_s->rstate = palloc(sizeof(hash_cols));

	hcols = (hash_cols *) fprod_p->rstate;
	hcols->numCols = ((HashGroup *) hashgrp_p)->agg.numCols;
	hcols->keyColIdx = ((HashGroup *) hashgrp_p)->agg.grpColIdx;

	hcols = (hash_cols *) fprod_s->rstate;
	hcols->numCols = ((HashGroup *) hashgrp_s)->agg.numCols;
	hcols->keyColIdx = ((HashGroup *) hashgrp_s)->agg.grpColIdx;

	(f->topSeg) = (Plan *) hashgrp_p;	/* I should have numerous top
										 * segments */

	/* Two dataflow segments, the top level and the bottom level */
	(f->segs)[0] = (Plan *) hashgrp_p;
	(f->segs)[1] = (Plan *) fprod_p;
	(f->segs)[2] = (Plan *) hashgrp_s;
	(f->segs)[3] = (Plan *) fprod_s;
	(f->segs)[4] = (Plan *) faccept;

	fcons_p->node_id = telegraph_nodeid;
	fprod_p->node_id = telegraph_nodeid;
	fcons_s->node_id = telegraph_nodeid;
	fprod_s->node_id = telegraph_nodeid;

	ascan_p->part_id = telegraph_nodeid;
	ascan_s->part_id = ROTATE_L(telegraph_nodeid, total_machines);

	((FluxCons_FT *) fcons_p)->m_role = F_PRIMARY;
	((FluxProd_FT *) fprod_p)->m_role = F_PRIMARY;
	((FluxCons_FT *) fcons_s)->m_role = F_SECONDARY;
	((FluxProd_FT *) fprod_s)->m_role = F_SECONDARY;

	(f->fluxen)[0] = (Plan *) fcons_p;
	fcons_p->flux_idx = 0;
	fcons_p->fprod_idx = 1;
	fcons_ft_p->fprod_idx_s = 3;

	(f->fluxen)[1] = (Plan *) fprod_p;
	fprod_p->flux_idx = 1;
	fprod_p->fcons_idx = 0;
	fprod_ft_p->fcons_idx_s = 2;

	(f->fluxen)[2] = (Plan *) fcons_s;
	fcons_s->flux_idx = 2;
	fcons_s->fprod_idx = 3;
	fcons_ft_s->fprod_idx_s = 1;

	(f->fluxen)[3] = (Plan *) fprod_s;
	fprod_s->flux_idx = 3;
	fprod_s->fcons_idx = 2;
	fprod_ft_s->fcons_idx_s = 0;

}

static void
insert_flux_into_plan_dup_better_all(Plan *plan, FSched * f, int mpair)
{

	int			i;
	Plan	  **o_fluxen;
	SegCons    *scons_p,
			   *scons_s;
	FluxProd_FT *fprod_p,
			   *fprod_s;
	StrmScan   *ascan;

	/* Create the main plan, and add the SegCons at the ends */
	insert_flux_into_plan_dup_better(plan, f, mpair);

	f->numSegs = 5;
	f->nfluxen = 6;

	o_fluxen = f->fluxen;
	f->fluxen = (Plan **) palloc(sizeof(Plan *) * (f->nfluxen));

	/* Copy the first four back in */
	for (i = 0; i < 4; i++)
		(f->fluxen)[i] = o_fluxen[i];

	scons_p = (SegCons *) makeNode(SegCons);
	scons_s = (SegCons *) makeNode(SegCons);

	((FluxCons *) scons_p)->all_done = false;
	((FluxCons *) scons_s)->all_done = false;

	((FluxCons *) scons_p)->node_id = telegraph_nodeid;
	((FluxCons *) scons_s)->node_id = telegraph_nodeid;

	((FluxCons_FT *) scons_p)->m_role = F_PRIMARY;
	((FluxCons_FT *) scons_s)->m_role = F_SECONDARY;

	((FluxCons_FT *) scons_p)->isPair = true;
	((FluxCons_FT *) scons_s)->isPair = true;

	((FluxCons *) scons_p)->sched = f;
	((FluxCons *) scons_s)->sched = f;

	fprod_p = (FluxProd_FT *) (f->fluxen)[1];
	fprod_s = (FluxProd_FT *) (f->fluxen)[3];

	ascan = (StrmScan *) outerPlan(fprod_p);

	((FluxCons *) scons_p)->plan.targetlist = ascan->scansock.plan.targetlist;
	((FluxCons *) scons_s)->plan.targetlist = ascan->scansock.plan.targetlist;

	((FluxCons *) scons_p)->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);
	((FluxCons *) scons_s)->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);

	/* Reset these guys to have the segcons as producers */
	outerPlan(fprod_p) = (Plan *) scons_p;
	parentPlan(scons_p) = (Plan *) fprod_p;

	outerPlan(fprod_s) = (Plan *) scons_s;
	parentPlan(scons_s) = (Plan *) fprod_s;

	((FluxCons *) scons_p)->nProd = 1;	/* Only the ingress is the
										 * producer */
	((FluxCons *) scons_s)->nProd = 1;

	((FluxCons *) scons_p)->map_c2m = palloc(sizeof(int) *
										  ((FluxCons *) scons_p)->nProd);
	((FluxCons *) scons_s)->map_c2m = palloc(sizeof(int) *
										  ((FluxCons *) scons_s)->nProd);

	/*
	  XXX These are wrong, but I won't fix them right now.
	  It doesn't matter because, seg cons does not really use these.
	*/
	
	((FluxCons *) scons_p)->map_c2m[0] = total_machines;
	((FluxCons *) scons_s)->map_c2m[0] = total_machines;

	init_map_c2m(&(((FluxCons *) scons_p)->map_m2c), total_machines + 2);
	init_map_c2m(&(((FluxCons *) scons_p)->map_m2c), total_machines + 2);

	for (i = 0; i < total_machines; i++)
	{
		((FluxCons *) scons_p)->map_m2c[i] = -1;
		((FluxCons *) scons_s)->map_m2c[i] = -1;
	}

	((FluxCons *) scons_p)->map_m2c[total_machines] = 0;
	((FluxCons *) scons_s)->map_m2c[total_machines] = 0;

	/* Don't have a reverse connection to spare machine */
	((FluxCons *) scons_p)->map_m2c[total_machines + 1] = -1;
	((FluxCons *) scons_s)->map_m2c[total_machines + 1] = -1;

	(f->fluxen)[4] = (Plan *) scons_p;
	((FluxCons *) scons_p)->flux_idx = 4;
	((FluxCons *) scons_p)->fprod_idx = 0;		/* FluxIngress operator */
	((FluxCons_FT *) scons_p)->fprod_idx_s = 0; /* FluxIngress operator */

	(f->fluxen)[5] = (Plan *) scons_s;
	((FluxCons *) scons_s)->flux_idx = 5;
	((FluxCons *) scons_s)->fprod_idx = 0;		/* FluxIngress operator */
	((FluxCons_FT *) scons_s)->fprod_idx_s = 0; /* FluxIngress operator */

}

static void
insert_flux_into_plan_dup_all(Plan *plan, FSched * f)
{
	hash_cols  *hcols;
	FluxCons   *fcons;
	FluxProd   *fprod;
	FluxCons_FT *fcons_ft;		/* Implictly level 1 */
	FluxProd_FT *fprod_ft;		/* Implictly level 1 */
	StrmScan   *ascan;
	Queue	   *local_q;
	FluxAcceptor *faccept;

	FluxCons   *fcons_0;

	f->kill_fsched = false;
	f->numSegs = 3;
	f->nfluxen = 3;

	f->segs = (Plan **) palloc(sizeof(Plan *) * (f->numSegs));
	f->fluxen = (Plan **) palloc(sizeof(Plan *) * (f->nfluxen));

	/* Right now, there is just a hash-group and a scan */
	fcons_ft = makeNode(FluxCons_FT);
	fprod_ft = makeNode(FluxProd_FT);

	fcons_0 = makeNode(FluxCons);

	fcons_ft->isPair = false;
	fprod_ft->isPair = false;

	fcons_ft->m_role = F_PRIMARY;
	fprod_ft->m_role = F_PRIMARY;

	fcons = (FluxCons *) fcons_ft;
	fprod = (FluxProd *) fprod_ft;

	ascan = (StrmScan *) outerPlan(plan);

	ascan->part_id = telegraph_nodeid;

	/* Look out for the error condition */
	if (!IsA(ascan, StrmScan) ||
		!IsA(plan, HashGroup))
	{
		elog(DEBUG1, "Cannot convert this to FSched plan yet");
		f->numSegs = 0;
		return;
	}

	/*
	 * We are going to abort the stream scan, and replace it with a flux
	 * consumer
	 */

	fcons_ft->fcons.sched = fcons_0->sched = f;

	faccept = makeNode(FluxAcceptor);
	f->faccept = faccept;

	fcons->plan.targetlist = ascan->scansock.plan.targetlist;
	fprod->plan.targetlist = ascan->scansock.plan.targetlist;

	fcons->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);
	fcons_0->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);

	outerPlan(plan) = (Plan *) fcons;
	parentPlan(fcons) = (Plan *) plan;

/*	outerPlan(fprod)  = (Plan *) ascan; */
	outerPlan(fprod) = (Plan *) fcons_0;
	parentPlan(fcons_0) = (Plan *) fprod;

	make_queue(&local_q, *p_no_lock, flux_queue_size);

	fcons->local_q = local_q;
	fprod->local_q = local_q;

	fcons->all_done = false;
	fprod->all_done = false;
	fcons_0->all_done = false;

	fcons->nProd = total_machines;
	fprod->nCons = total_machines;

	fcons_0->nProd = 1;			/* Only one producer here! */

	init_map_c2m(&(fcons->map_c2m), fcons->nProd);
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);
	init_map_c2m(&(fcons->map_m2c), fcons->nProd);
	init_map_c2m(&(fprod->map_m2c), fprod->nCons);

	fcons_0->map_c2m = palloc(sizeof(int) * fcons_0->nProd);

	fcons_0->map_c2m[0] = total_machines;		/* The source lives on the
												 * next available machine. */

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	hcols = (hash_cols *) fprod->rstate;
	hcols->numCols = ((HashGroup *) plan)->agg.numCols;
	hcols->keyColIdx = ((HashGroup *) plan)->agg.grpColIdx;

	/* We need to set the state for the hashing function */

	/* Two dataflow segments, the top level and the bottom level */
	(f->topSeg) = plan;
	(f->segs)[0] = (Plan *) plan;
	(f->segs)[1] = (Plan *) fprod;
	(f->segs)[2] = (Plan *) faccept;

	f->fluxen[0] = (Plan *) fcons;
	f->fluxen[1] = (Plan *) fprod;
	f->fluxen[2] = (Plan *) fcons_0;

	fcons->node_id = telegraph_nodeid;
	fcons->fprod_idx = 1;
	fcons->flux_idx = 0;
	fprod->node_id = telegraph_nodeid;
	fprod->fcons_idx = 0;
	fprod->flux_idx = 1;

	fcons_0->node_id = telegraph_nodeid;
	fcons_0->fprod_idx = 0;		/* The plan below is just a source. */
	fcons_0->flux_idx = 2;
}

/* This just creates a single site plan */
static void
insert_flux_into_plan_dup(Plan *plan, FSched * f)
{
	hash_cols  *hcols;
	FluxCons   *fcons;
	FluxProd   *fprod;
	FluxCons_FT *fcons_ft;
	FluxProd_FT *fprod_ft;
	StrmScan   *ascan;
	Queue	   *local_q;
	FluxAcceptor *faccept;

	f->kill_fsched = false;
	f->numSegs = 3;
	f->nfluxen = f->numSegs - 1;

	f->segs = (Plan **) palloc(sizeof(Plan *) * (f->numSegs));
	f->fluxen = (Plan **) palloc(sizeof(Plan *) * (f->nfluxen));

	/* Right now, there is just a hash-group and a scan */
	fcons_ft = makeNode(FluxCons_FT);
	fprod_ft = makeNode(FluxProd_FT);

	fcons_ft->isPair = false;
	fprod_ft->isPair = false;

	fcons_ft->m_role = F_PRIMARY;
	fprod_ft->m_role = F_PRIMARY;

	fcons = (FluxCons *) fcons_ft;
	fprod = (FluxProd *) fprod_ft;

	ascan = (StrmScan *) outerPlan(plan);

	ascan->part_id = telegraph_nodeid;

	/* Look out for the error condition */
	if (!IsA(ascan, StrmScan) ||
		!IsA(plan, HashGroup))
	{
		elog(DEBUG1, "Cannot convert this to FSched plan yet");
		f->numSegs = 0;
		return;
	}

	fcons->sched = f;

	faccept = makeNode(FluxAcceptor);
	f->faccept = faccept;

	fcons->plan.targetlist = ascan->scansock.plan.targetlist;
	fprod->plan.targetlist = ascan->scansock.plan.targetlist;

	fcons->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);

	outerPlan(plan) = (Plan *) fcons;
	parentPlan(fcons) = (Plan *) plan;

	outerPlan(fprod) = (Plan *) ascan;
	parentPlan(ascan) = (Plan *) fprod;

	make_queue(&local_q, *p_no_lock, flux_queue_size);

	fcons->local_q = local_q;
	fprod->local_q = local_q;

	fcons->all_done = false;
	fprod->all_done = false;

	fcons->nProd = total_machines;
	fprod->nCons = total_machines;

	init_map_c2m(&(fcons->map_c2m), fcons->nProd);
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);
	init_map_c2m(&(fcons->map_m2c), fcons->nProd);
	init_map_c2m(&(fprod->map_m2c), fprod->nCons);

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	hcols = (hash_cols *) fprod->rstate;
	hcols->numCols = ((HashGroup *) plan)->agg.numCols;
	hcols->keyColIdx = ((HashGroup *) plan)->agg.grpColIdx;

	/* We need to set the state for the hashing function */

	/* Two dataflow segments, the top level and the bottom level */
	(f->topSeg) = plan;
	(f->segs)[0] = (Plan *) plan;
	(f->segs)[1] = (Plan *) fprod;
	(f->segs)[2] = (Plan *) faccept;

	f->fluxen[0] = (Plan *) fcons;
	f->fluxen[1] = (Plan *) fprod;

	fcons->node_id = telegraph_nodeid;
	fcons->fprod_idx = 1;
	fcons->flux_idx = 0;
	fprod->node_id = telegraph_nodeid;
	fprod->fcons_idx = 0;
	fprod->flux_idx = 1;

}

/* This creates a specific data generator for me */
static DataGen *
create_datagen(StrmScan *sscan, int mskip, int moffset, bool isStandby)
{
	DataGen *dg;

	dg = makeNode(DataGen);
	dg->plan.targetlist = copyObject(((Plan *) sscan)->targetlist);	/* Target lists should now
																	 * match. */
	dg->ncols = 3;
	dg->values = palloc(sizeof(Datum) * dg->ncols);
	/* dg->nulls = palloc(sizeof(char) * dg->ncols); */
	
	dg->nrandcols = 1;
	dg->rand_col = palloc(sizeof(Datum) * dg->nrandcols);
	dg->max = palloc(sizeof(Datum) * dg->nrandcols);
	
	dg->nulls = "   ";			/* No null columns */
	dg->seqno_col = 0;
	dg->rand_col[0] = 1;
	dg->max[0] = NUM_HASH_GROUPS;
	dg->values[2] = 1974U;		/* Some constant I picked */
	
	dg->tupDesc = CreateTupleDescCopy((sscan->scansock).tupDesc);
	dg->seqnum = isStandby ? (-1) : 0;
	
	dg->offset = moffset;
	dg->skip   = mskip;
	
	return dg;
}

static DataGen *
create_datagen_cstream(StrmScan *sscan, int mskip, int moffset, bool isStandby)
{
	DataGen *dg;

	dg = makeNode(DataGen);
	dg->plan.targetlist = copyObject(((Plan *) sscan)->targetlist);	/* Target lists should now
																	 * match. */

	dg->ncols = 4;
	dg->values = palloc(sizeof(Datum) * dg->ncols);

	dg->nrandcols = 2;
	dg->rand_col = palloc(sizeof(Datum) * dg->nrandcols);
	dg->max = palloc(sizeof(Datum) * dg->nrandcols);

	dg->nulls = "   n";
	dg->seqno_col = 0;

	dg->rand_col[0] = 1;
	dg->rand_col[1] = 2;
	
	dg->max[0] = NUM_HASH_GROUPS/*  * 10 */ / 10;
	dg->max[1] = 10;
	
	dg->tupDesc = CreateTupleDescCopy((sscan->scansock).tupDesc);
	dg->seqnum = isStandby ? (-1) : 0;

	dg->offset = moffset;
	dg->skip = mskip;

	return dg;
}

static DataGen *
create_datagen_ts(StrmScan *sscan, int mskip, int moffset, bool isStandby)
{
	DataGen *dg;
	VarChar *vc;
	char *m_str = "Random Large String. 	dg->seqno_col = 0; 	dg->rand_col[0] = 3;";
	
	dg = makeNode(DataGen);
	dg->plan.targetlist = copyObject(((Plan *) sscan)->targetlist);	/* Target lists should now
																	 * match. */

	dg->ncols = 6;
	dg->values = palloc(sizeof(Datum) * dg->ncols);
	
	dg->nrandcols = 2;
	dg->rand_col = palloc(sizeof(Datum) * dg->nrandcols);
	dg->max = palloc(sizeof(Datum) * dg->nrandcols);

#define BIG_TUPS
	
#ifdef BIG_TUPS
	dg->nulls = "      ";

	vc = palloc(sizeof(VarChar) + strlen(m_str));
	memcpy(vc->vl_dat, m_str, strlen(m_str));
	vc->vl_len = strlen(m_str);
	
	dg->values[5] = (Datum) vc;
#else
	dg->nulls = "     n";
#endif
	
	dg->seqno_col = 0;

	dg->rand_col[0] = 3;
	dg->rand_col[1] = 4;
	
	dg->max[0] = NUM_HASH_GROUPS /* / 2 */ /*  * 10 */ /* / 10 */;
	dg->max[1] = 10;
	
	dg->tupDesc = CreateTupleDescCopy((sscan->scansock).tupDesc);
	dg->seqnum = isStandby ? (-1) : 0;

	dg->offset = moffset;
	dg->skip = mskip;

	/* Columns for time stamp */
	dg->use_ts = true;
	dg->ts_sec = 1;
	dg->ts_usec = 2;
	dg->ts_freq = 100;

	return dg;
}

static DataGen *
create_datagen_tgroup(StrmScan *sscan, int mskip, int moffset, bool isStandby)
{
	DataGen *dg;

	dg = makeNode(DataGen);
	dg->plan.targetlist = copyObject(((Plan *) sscan)->targetlist);	/* Target lists should now
																	 * match. */
	dg->ncols = 5;
	dg->values = palloc(sizeof(Datum) * dg->ncols);

	dg->nrandcols = 3;
	dg->rand_col = palloc(sizeof(Datum) * dg->nrandcols);
	dg->max = palloc(sizeof(Datum) * dg->nrandcols);
	
	dg->nulls = "    n";
	dg->seqno_col = 0;

	dg->rand_col[0] = 1;
	dg->rand_col[1] = 2;
	dg->rand_col[2] = 3;
	
	dg->max[0] = NUM_HASH_GROUPS / 100;
	dg->max[1] = 10;
	dg->max[2] = 10;
	
	dg->tupDesc = CreateTupleDescCopy((sscan->scansock).tupDesc);
	dg->seqnum = isStandby ? (-1) : 0;

	dg->offset = moffset;
	dg->skip = mskip;

	return dg;
}

/* Four levels of group-by */

static DataGen *
create_datagen_fgroup(StrmScan *sscan, int mskip, int moffset, bool isStandby)
{
	DataGen *dg;

	dg = makeNode(DataGen);
	dg->plan.targetlist = copyObject(((Plan *) sscan)->targetlist);	/* Target lists should now
																	 * match. */
	dg->ncols = 6;
	dg->values = palloc(sizeof(Datum) * dg->ncols);

	dg->nrandcols = 4;
	dg->rand_col = palloc(sizeof(Datum) * dg->nrandcols);
	dg->max = palloc(sizeof(Datum) * dg->nrandcols);
	
	dg->nulls = "     n";
	dg->seqno_col = 0;

	dg->rand_col[0] = 1;
	dg->rand_col[1] = 2;
	dg->rand_col[2] = 3;
	dg->rand_col[3] = 4;
	
	dg->max[0] = NUM_HASH_GROUPS / 100;
	dg->max[1] = 10;
	dg->max[2] = 10;
	dg->max[3] = 10;
	
	dg->tupDesc = CreateTupleDescCopy((sscan->scansock).tupDesc);
	dg->seqnum = isStandby ? (-1) : 0;

	dg->offset = moffset;
	dg->skip = mskip;

	return dg;
}

static void
connect_plans(Plan *cons, Plan *prod)
{
	outerPlan(cons) = prod;
	parentPlan(prod) = cons;
}

static TupleDesc
get_tupledesc(Plan *outer)
{
	/* Warning ugliness below.  Argh. I wish tuple descriptors were calculated
	 * earlier. We need the tuple descriptor set because flux cons needs it. */

	switch(nodeTag(outer))
	{
		case (T_StrmScan):
			if (((ScanSock *) outer)->dontProject)
			{
				return CreateTupleDescCopy(((ScanSock *) outer)->tupDesc);
			}
			else
			{
				/*
				 * XXX I picked hasoid to be false here. I don't know what else to do. I
				 * cannot call other routines because the common state is yet
				 * uninitialized.
				 */
				elog(DEBUG1, "Copying tuple descriptor from targetlist! Dangerous.");
				return ExecTypeFromTL(((Plan *) outer)->targetlist, false);
/* 				elog(DEBUG1, "Tupdesc has %d atts", fp->fprod.tupDesc->natts); */
			}
			break;

		case (T_DataGen):
			elog(DEBUG1, "Copying tuple descriptor from datagen");
			return CreateTupleDescCopy(((DataGen *) outer)->tupDesc);
/* 			elog(DEBUG1, "Tupdesc has %d atts", fp->fprod.tupDesc->natts); */
			break;

		case (T_HashGroup):
			elog(DEBUG1, "Copying tuple descriptor from hashgroup");
			return ExecTypeFromTL(outer->targetlist, false);
/* 			elog(DEBUG1, "Tupdesc has %d atts", fp->fprod.tupDesc->natts); */
			break;

		default:
			elog(DEBUG1, "Copying tuple descriptor from targetlist! Dangerous.");
			return ExecTypeFromTL(((Plan *) outer)->targetlist, false);
/* 			elog(DEBUG1, "Tupdesc has %d atts", fp->fprod.tupDesc->natts); */
			break;
	}
}

/* Installs the newly created flux prod into the scheduler */
static FluxProd_FT *
create_fprod_ft(FSched *f, Plan *outer, FluxRole frole, bool isPair, bool isStandby)
{
	FluxProd_FT *fp;
	
	fp = (FluxProd_FT *) makeNode(FluxProd_FT);

	fp->fprod.sched = f;
	fp->fprod.flux_idx = INSERT_END_FLUXEN(f, fp);

	fp->fprod.all_done = false;

	fp->isPair = isPair;

	((Plan *) fp)->pushPlan = fdataflow->pushPlan;
	
	fp->fprod.plan.targetlist = copyObject(outer->targetlist);

	fp->fprod.tupDesc = get_tupledesc(outer);
	
	fp->fprod.node_id = telegraph_nodeid;
	fp->m_role = frole;
	fp->mode = isStandby ? STANDBY_NORMAL : FLUX_NORMAL;
	fp->isStandby = isStandby;

	fp->sync = false;
	
	elog(DEBUG1, "Creating prod, isStandby: %d, mode: %d", isStandby, fp->mode);
	
	return fp;
}

/* Installs the newly created flux cons into the scheduler */
static FluxCons_FT *
create_fcons_ft(FSched *f, FluxRole frole, bool isPair, bool isStandby)
{
	FluxCons_FT *fc;

	fc = (FluxCons_FT *) makeNode(FluxCons_FT);
	
	fc->fcons.sched = f;
	fc->fcons.flux_idx = INSERT_END_FLUXEN(f, fc);

	fc->fcons.all_done = false;
	
	fc->isPair = isPair;
	
	fc->fcons.node_id = telegraph_nodeid;
	fc->m_role = frole;
	fc->mode = isStandby ? STANDBY_NORMAL : FLUX_NORMAL;
	fc->isStandby = isStandby;

	((Plan *) fc)->pushPlan = fdataflow->pushPlan;
	
	elog(DEBUG1, "Creating cons, isStandby: %d, mode: %d", isStandby, fc->mode);
	
	return fc;
}

/* XXX total_prod and total_cons must be the same at the moment. */
static void
connect_flux_ft(FluxProd_FT *fp_p, FluxProd_FT *fp_s,
				FluxCons_FT *fc_p, FluxCons_FT *fc_s,
				int total_prod, int total_cons, bool hasStandby)
{
	/* q is the out for prod and in for cons.
	   r is the in for prod and out for cons.
	   ab --- a is the producer, b is the consumer.
	   e.g. local_q_sp is the out for secondary prod and in for primary cons */

	/* XXX Danger. Mind your p's and s's in this function */
	Queue	   *local_q_pp,
			   *local_q_ps,
			   *local_q_sp,
			   *local_q_ss;
	Queue	   *local_r_pp,
			   *local_r_ps,
			   *local_r_sp,
			   *local_r_ss;

	int nmach, i;

	Assert(total_prod == total_cons);
	
	/* Assign types to the flux cons */
	((Plan *) fc_p)->targetlist = copyObject(((Plan *) fp_p)->targetlist);
	fc_p->fcons.tupDesc = CreateTupleDescCopy(fp_p->fprod.tupDesc);
	((Plan *) fc_s)->targetlist = copyObject(((Plan *) fp_s)->targetlist);
	fc_s->fcons.tupDesc = CreateTupleDescCopy(fp_s->fprod.tupDesc);

	fc_p->fcons.nProd = total_prod;
	fc_s->fcons.nProd = total_prod;
	fp_p->fprod.nCons = total_cons;
	fp_s->fprod.nCons = total_cons;
	
	/* Connect primaries. */
	fc_p->fcons.fprod_idx = fp_p->fprod.flux_idx;
	fp_p->fprod.fcons_idx = fc_p->fcons.flux_idx;
	
	fc_s->fcons.fprod_idx = fp_s->fprod.flux_idx;
	fp_s->fprod.fcons_idx = fc_s->fcons.flux_idx;

	/* Connect secondaries. */
	fc_p->fprod_idx_s = fp_s->fprod.flux_idx;
	fp_s->fcons_idx_s = fc_p->fcons.flux_idx;

	fc_s->fprod_idx_s = fp_p->fprod.flux_idx;
	fp_p->fcons_idx_s = fc_s->fcons.flux_idx;

	/* Create and initialize the connection maps */
	init_map_c2m(&(fc_p->fcons.map_c2m), total_prod);
	init_map_c2m(&(fc_s->fcons.map_c2m), total_prod);
	fc_p->spare_m = hasStandby ? total_prod : UNKNOWN_MACHINE;
	fc_s->spare_m = hasStandby ? total_prod : UNKNOWN_MACHINE;

	/* Has one more if there is a standby */
	nmach = hasStandby ? (total_prod + 1) : total_prod;
	init_map_c2m(&(fc_p->fcons.map_m2c), nmach);
	init_map_c2m(&(fc_s->fcons.map_m2c), nmach);
	if (hasStandby) 
	{
		elog(DEBUG1, "Has stanby --- setting spare -- %d", total_prod);
		fc_p->fcons.map_m2c[total_prod] = SPARE_CONN_IDX;
		fc_s->fcons.map_m2c[total_prod] = SPARE_CONN_IDX;

#define DEBUG_MAP
		
#ifdef DEBUG_MAP
		elog(DEBUG1, "cons: SADDR-P: %p", fc_p->fcons.map_m2c);
		elog(DEBUG1, "cons: SADDR-S: %p", fc_s->fcons.map_m2c);
		for (i= 0; i < total_prod + 1; i++)
		{
			elog(DEBUG1, "cons - P: map: %d -> %d", i, fc_p->fcons.map_m2c[i]);
			elog(DEBUG1, "cons - S: map: %d -> %d", i, fc_s->fcons.map_m2c[i]);
		}
#endif
		
	}

	/* Now do it for the producer */
	init_map_c2m(&(fp_p->fprod.map_c2m), total_cons);
	init_map_c2m(&(fp_s->fprod.map_c2m), total_cons);
	fp_p->spare_m = hasStandby ? total_cons : UNKNOWN_MACHINE;
	fp_s->spare_m = hasStandby ? total_cons : UNKNOWN_MACHINE;

	nmach = hasStandby ? (total_cons + 1) : total_cons;
	init_map_c2m(&(fp_p->fprod.map_m2c), nmach);
	init_map_c2m(&(fp_s->fprod.map_m2c), nmach);
	if (hasStandby) 
	{
		fp_p->fprod.map_m2c[total_cons] = SPARE_CONN_IDX;
		fp_s->fprod.map_m2c[total_cons] = SPARE_CONN_IDX;

#ifdef DEBUG_MAP
		elog(DEBUG1, "prod: SADDR-P: %p", fp_p->fprod.map_m2c);
		elog(DEBUG1, "prod: SADDR-S: %p", fp_s->fprod.map_m2c);
		for (i= 0; i < total_cons + 1; i++)
		{
			elog(DEBUG1, "prod - P: map: %d -> %d", i, fp_p->fprod.map_m2c[i]);
			elog(DEBUG1, "prod - S: map: %d -> %d", i, fp_s->fprod.map_m2c[i]);
		}
#endif

	}

	/* Create and initialize the local queues */
	make_queue(&local_q_pp, *p_no_lock, flux_queue_size);
	make_queue(&local_q_ps, *p_no_lock, flux_queue_size);
	make_queue(&local_q_sp, *p_no_lock, flux_queue_size);
	make_queue(&local_q_ss, *p_no_lock, flux_queue_size);
	make_queue(&local_r_pp, *p_no_lock, flux_queue_size);
	make_queue(&local_r_ps, *p_no_lock, flux_queue_size);
	make_queue(&local_r_sp, *p_no_lock, flux_queue_size);
	make_queue(&local_r_ss, *p_no_lock, flux_queue_size);

	fp_p->fprod.local_q = local_q_pp;
	fc_p->fcons.local_q = local_q_pp;

	fp_s->fprod.local_q = local_q_ss;
	fc_s->fcons.local_q = local_q_ss;

	fp_p->local_r = local_r_pp;
	fc_p->local_r = local_r_pp;

	fp_s->local_r = local_r_ss;
	fc_s->local_r = local_r_ss;

	fp_p->local_r_s = local_r_ps;
	fc_s->local_r_s = local_r_ps;

	fp_p->local_q_s = local_q_ps;
	fc_s->local_q_s = local_q_ps;

	fp_s->local_r_s = local_r_sp;
	fc_p->local_r_s = local_r_sp;

	fp_s->local_q_s = local_q_sp;
	fc_p->local_q_s = local_q_sp;

#ifdef DEBUG_MAP
	for (i= 0; i < total_cons + 1; i++)
	{
		elog(DEBUG1, "cons - P: map: %d -> %d", i, fc_p->fcons.map_m2c[i]);
		elog(DEBUG1, "cons - S: map: %d -> %d", i, fc_s->fcons.map_m2c[i]);
		elog(DEBUG1, "prod - P: map: %d -> %d", i, fp_p->fprod.map_m2c[i]);
		elog(DEBUG1, "prod - S: map: %d -> %d", i, fp_s->fprod.map_m2c[i]);
	}
#endif
	
}

/* Duplicates the plan, and inserts flux after */
static void
insert_flux_after(FSched *f, Plan *plan_p, Plan *plan_s)
{
	FluxProd_FT *fp_p, *fp_s;
	FluxCons_FT *fc_p, *fc_s;
	Plan *outer_p, *outer_s;

	outer_p = outerPlan(plan_p);
	outer_s = outerPlan(plan_s);

	fp_p = create_fprod_ft(f, outer_p, F_PRIMARY, true, fdataflow->isStandby);
	fp_s = create_fprod_ft(f, outer_s, F_SECONDARY, true, fdataflow->isStandby);

	/* Set the partitioning, or routing function */
	setup_hash_route(plan_p, outer_p, (FluxProd *) fp_p);
	setup_hash_route(plan_s, outer_s, (FluxProd *) fp_s);

	connect_plans((Plan *) fp_p, (Plan *) outer_p);
	connect_plans((Plan *) fp_s, (Plan *) outer_s);

	fc_p = create_fcons_ft(f, F_PRIMARY, true, fdataflow->isStandby);
	fc_s = create_fcons_ft(f, F_SECONDARY, true, fdataflow->isStandby);

	/* This version always has a standby */
	connect_flux_ft(fp_p, fp_s, fc_p, fc_s,
					total_machines, total_machines, true);

	connect_plans(plan_p, (Plan *) fc_p);
	connect_plans(plan_s, (Plan *) fc_s);

	if (fdataflow->pushPlan)
	{
		INSERT_END_SEGS(f, fc_p);
		INSERT_END_SEGS(f, fc_s);
	}
	else
	{
		INSERT_END_SEGS(f, fp_p);
		INSERT_END_SEGS(f, fp_s);
	}
}

typedef struct PlanPair
{
	Plan *top_p;
	Plan *top_s;
	Plan *bottom_p; /* XXX temporary, usually there are many leaves */
	Plan *bottom_s;
} PlanPair;

/* Constructor for the plan pair */
static PlanPair *
new_plan_pair(Plan *tp, Plan *ts, Plan *bp, Plan *bs)
{
	PlanPair *rplan = palloc(sizeof(PlanPair));
	rplan->top_p = tp;
	rplan->top_s = ts;
	rplan->bottom_p = bp;
	rplan->bottom_s = bs;

	return rplan;
}

/* Destructor for plan pair */
static void
destroy_plan_pair(PlanPair *pp)
{
	pfree(pp);
}

/* Returns the top level plan */
static PlanPair *
create_pair_flux_sub(FSched *f, Plan *plan)
{
	StrmScan  	*sscan;
	HashGroup 	*hg_p, *hg_s;
	DataGen   	*dg_p, *dg_s;

	if (!IsA(plan, HashGroup))
	{
		elog(DEBUG1, "Sorry, expecting hash group-by.");
		return NULL;
	}

	sscan = (StrmScan *) outerPlan(plan);

	if (!IsA(sscan, StrmScan))
	{
		elog(DEBUG1, "Sorry, expecting stream scan.");
		return NULL;
	}

	/* The target list already has been copied into f */
	
	hg_p = (HashGroup *) plan;
	hg_s = copyObject(hg_p); /* This creates an extra stream scan ... oh well. */
	
	dg_p = create_datagen(sscan,
						  total_machines,
						  telegraph_nodeid,
						  fdataflow->isStandby);
	dg_s = create_datagen(sscan,
						  total_machines,
						  ROTATE_L(telegraph_nodeid, total_machines),
						  fdataflow->isStandby);

	/* Reconnect these plans */
	connect_plans((Plan *) hg_p, (Plan *) dg_p);
	connect_plans((Plan *) hg_s, (Plan *) dg_s);

	/* Insert flux after the group-by */
	insert_flux_after(f, (Plan *) hg_p, (Plan *) hg_s);

	INSERT_END_SEGS(f, hg_p);
	INSERT_END_SEGS(f, hg_s);

	return new_plan_pair((Plan *) hg_p, (Plan *) hg_s, (Plan *) dg_p, (Plan *) dg_s);
}

static Plan *
create_pair_flux(FSched * f, Plan *plan, Query *querytree)
{
	PlanPair *fx;

	plan = pg_convert_to_hash_group(plan, querytree);
		
	InitFSchedPlan(f);
	fx = create_pair_flux_sub(f, plan);
	EndFSchedPlan(f, fx->top_p);
	destroy_plan_pair(fx); /* Done with it */
	return (Plan *) f;
}


/* Goes up the plan and finds the first ancestor of type provided */
#define FFANCESTOR(p, _type_) \
  find_first_ancestor(p, T_##_type_)

static Plan *
find_first_ancestor(Plan *p, NodeTag type)
{
	while (p && (nodeTag(p) != type)) 
	{
		p = parentPlan(p);
	}

	return p;
}

static void
connect_dataseg_ends(FluxProd_FT *fp, FluxCons_FT *fc)
{
	fc->fprod_parent = fp;
	/* fp->fcons_child = fc; */
}


static PlanPair *
create_segcons_plan_sub(FSched *f, /* StrmScan */Plan *ascan)
{
	int			i,
				m_id;
	SegCons    *scons_p,
			   *scons_s;

	scons_p = (SegCons *) makeNode(SegCons);
	scons_s = (SegCons *) makeNode(SegCons);

	if (fdataflow->pushPlan)
	{
		((Plan *) scons_p)->pushPlan = true;
		((Plan *) scons_s)->pushPlan = true;
	}
	
	((FluxCons *) scons_p)->all_done = false;
	((FluxCons *) scons_s)->all_done = false;

	((FluxCons *) scons_p)->node_id = telegraph_nodeid;
	((FluxCons *) scons_s)->node_id = telegraph_nodeid;

	((FluxCons_FT *) scons_p)->m_role = F_PRIMARY;
	((FluxCons_FT *) scons_s)->m_role = F_SECONDARY;

	/* I know there is redundancy here !*/
	((FluxCons_FT *) scons_p)->isStandby = fdataflow->isStandby;
	((FluxCons_FT *) scons_s)->isStandby = fdataflow->isStandby;
	
	if (!fdataflow->isStandby)
	{
		((FluxCons_FT *) scons_p)->mode = FLUX_NORMAL;
		((FluxCons_FT *) scons_s)->mode = FLUX_NORMAL;
	}
	else
	{
		((FluxCons_FT *) scons_p)->mode = STANDBY_NORMAL;
		((FluxCons_FT *) scons_s)->mode = STANDBY_NORMAL;
	}

	((FluxCons_FT *) scons_p)->isPair = true;
	((FluxCons_FT *) scons_s)->isPair = true;

	((FluxCons *) scons_p)->sched = f;
	((FluxCons *) scons_s)->sched = f;

/* 	ascan = (StrmScan *) outerPlan(plan); */

	if (!IsA(ascan, StrmScan))
/* 	if (!IsA_Scan(ascan)) */
	{
		elog(DEBUG1, "Cannot convert this to FSched plan yet.");
		f->numSegs = 0;
		return NULL;
	}

/* 	((FluxCons *) scons_p)->plan.targetlist = ascan->scansock.plan.targetlist; */
/* 	((FluxCons *) scons_s)->plan.targetlist = ascan->scansock.plan.targetlist; */

	((Plan *) scons_p)->targetlist = ((Plan *) ascan)->targetlist;
	((Plan *) scons_s)->targetlist = ((Plan *) ascan)->targetlist;

	if (IsA(ascan, StrmScan) &&
		(((StrmScan *) ascan)->seqscan->dontProject))
	{
		((FluxCons *) scons_p)->tupDesc = CreateTupleDescCopy((((StrmScan *) ascan)->scansock).tupDesc);
		((FluxCons *) scons_s)->tupDesc = CreateTupleDescCopy((((StrmScan *) ascan)->scansock).tupDesc);
	}
	else 
	{
		((FluxCons *) scons_p)->tupDesc = ExecTypeFromTL(((Plan *) scons_p)->targetlist, false);
		((FluxCons *) scons_s)->tupDesc = ExecTypeFromTL(((Plan *) scons_s)->targetlist, false);
	}
	
	/* Reset these guys to have the segcons as producers */
	parentPlan(scons_p) = NULL;
	parentPlan(scons_s) = NULL;

	((FluxCons *) scons_p)->nProd = 1;	/* Only the ingress is the
										 * producer */
	((FluxCons *) scons_s)->nProd = 1;

	((FluxCons *) scons_p)->map_c2m = palloc(sizeof(int) *
										  ((FluxCons *) scons_p)->nProd);
	((FluxCons *) scons_s)->map_c2m = palloc(sizeof(int) *
										  ((FluxCons *) scons_s)->nProd);

	((FluxCons *) scons_p)->map_c2m[0] = total_machines + 1;
	((FluxCons *) scons_s)->map_c2m[0] = total_machines + 1;

	init_map_c2m(&(((FluxCons *) scons_p)->map_m2c), total_machines + 2);
	init_map_c2m(&(((FluxCons *) scons_s)->map_m2c), total_machines + 2);

	for (i = 0; i < (total_machines); i++)
	{
		((FluxCons *) scons_p)->map_m2c[i] = UNKNOWN_CONN_IDX;
		((FluxCons *) scons_s)->map_m2c[i] = UNKNOWN_CONN_IDX;
	}

	/* (total_machines) is the machine with the spare stuff */
	((FluxCons *) scons_p)->map_m2c[total_machines] = SPARE_CONN_IDX;
	((FluxCons *) scons_s)->map_m2c[total_machines] = SPARE_CONN_IDX;

	/* (total_machines + 1) is the machine with the source */
	((FluxCons *) scons_p)->map_m2c[total_machines + 1] = 0;
	((FluxCons *) scons_s)->map_m2c[total_machines + 1] = 0;

	/*
	 * (total_machines) is the spare machine contains no reverse
	 * connection.
	 */
	/* ((FluxCons *) scons_p)->map_m2c[total_machines] = 0; */
	/* ((FluxCons *) scons_s)->map_m2c[total_machines] = 0; */

	((FluxCons_FT *) scons_p)->spare_m = total_machines;
	((FluxCons_FT *) scons_s)->spare_m = total_machines;

	/* INSERT_END_SEGS(f, scons_p); */
	m_id = INSERT_END_FLUXEN(f, scons_p);		/* Return should be 1 */

	((FluxCons *) scons_p)->flux_idx = m_id;
	((FluxCons *) scons_p)->fprod_idx = 0;		/* FluxIngress operator */
	((FluxCons_FT *) scons_p)->fprod_idx_s = 0; /* FluxIngress operator */

	/* INSERT_END_SEGS(f, scons_s); */
	m_id = INSERT_END_FLUXEN(f, scons_s);		/* Return should be 2 */

	((FluxCons *) scons_s)->flux_idx = m_id;
	((FluxCons *) scons_s)->fprod_idx = 0;		/* FluxIngress operator */
	((FluxCons_FT *) scons_s)->fprod_idx_s = 0; /* FluxIngress operator */

	return new_plan_pair((Plan *) scons_p, (Plan *) scons_s,
						 (Plan *) scons_p, (Plan *) scons_s);
}

static Plan *
create_segcons_plan(FSched * f, Plan * plan, Query *querytree)
{
	PlanPair *sc;

	plan = pg_convert_to_hash_group(plan, querytree);
		
	InitFSchedPlan(f);
	sc = create_segcons_plan_sub(f, outerPlan(plan)); /* Outer should be a StrmScan */

	/* Make sure these guys can get scheduled */
	INSERT_END_SEGS(f, sc->top_p);
	INSERT_END_SEGS(f, sc->top_s);
	
	EndFSchedPlan(f, sc->top_p);
	destroy_plan_pair(sc);
	return (Plan *) f;
}

static Plan *
create_two_level(FSched * f, Plan *plan, Query *querytree)
{
	PlanPair *sc;
	PlanPair *fx;
	Plan *tmp;

	plan = pg_convert_to_hash_group(plan, querytree);
	
	InitFSchedPlan(f);

	/* Must be created first, so FluxIngress knows to which operator to connect */
	sc = create_segcons_plan_sub(f, outerPlan(plan)); /* Outer should be a StrmScan */
	fx = create_pair_flux_sub(f, plan);

	/* Connect the bottom plans */
	connect_plans(fx->bottom_p, sc->top_p);
	connect_plans(fx->bottom_s, sc->top_s);

	/* Find the first flux prod */
	tmp = FFANCESTOR(fx->bottom_p, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_p);

	tmp = FFANCESTOR(fx->bottom_s, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_s);
	
	EndFSchedPlan(f, fx->top_p);
	destroy_plan_pair(sc);
	destroy_plan_pair(fx);

	return (Plan *) f;
}

static Plan *
create_two_groups_w_flux(FSched *f, Plan *plan, Query *querytree)
{
	/* At this point, I should have a scan-hgrpb-hgrpt in plan (t for top, and b for bottom) */
	HashGroup *hgt_p, *hgb_p, *hgt_s, *hgb_s;
	StrmScan *sscan;
	PlanPair *sc;
	Plan *tmp;

	plan = pg_convert_all_to_hash_group(plan, querytree);

	remove_seqnums_from_group_cols(plan);
		
	hgt_p = (HashGroup *) plan;

	if (!IsA(hgt_p, HashGroup))
	{
		elog(DEBUG1, "Sorry, expecting hash group-by.");
		return NULL;
	}

	hgb_p = (HashGroup *) outerPlan(hgt_p);
	
	if (!IsA(hgb_p, HashGroup))
	{
		elog(DEBUG1, "Sorry, expecting another hash group-by.");
		return NULL;
	}

	sscan = (StrmScan *) outerPlan(hgb_p);

	if (!IsA(sscan, StrmScan))
	{
		elog(DEBUG1, "Sorry, expecting stream scan.");
		return NULL;
	}

	InitFSchedPlan(f);

	/* This must be created first so the Ingress knows the correct operator to which to connect */
	sc = create_segcons_plan_sub(f, (Plan *) sscan);

	hgt_s = copyObject(hgt_p);
	hgb_s = (HashGroup *) outerPlan(hgt_s);
	
	insert_flux_after(f, (Plan *) hgt_p, (Plan *) hgt_s);

	if (!fdataflow->pushPlan)
	{
		/* Add these guys as segments that need to be scheduled */
		INSERT_END_SEGS(f, hgt_p);
		INSERT_END_SEGS(f, hgt_s);
	}
	else
	{
		INSERT_END_SEGS(f, sc->top_p);
		INSERT_END_SEGS(f, sc->top_s);
	}
	
	/* Connect the segcons to the bottom */
	connect_plans((Plan *) hgb_p, sc->top_p);
	connect_plans((Plan *) hgb_s, sc->top_s);

	/* Find the first flux prod and connect the ends. */
	tmp = FFANCESTOR((Plan *) hgb_p, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_p);

	tmp = FFANCESTOR((Plan *) hgb_s, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_s);

	if (!fdataflow->pushPlan)
	{
		EndFSchedPlan(f, (Plan *) hgt_p);
	}
	else
	{
		EndFSchedPlan(f, outerPlan(hgt_p));
	}
	
	destroy_plan_pair(sc);

	return (Plan *) f;
}

static SegProd *
create_seg_prod(FSched *f, Plan *outer, FluxRole frole, bool isPair, bool isStandby)
{
	int i;
	SegProd *sp;

	sp = makeNode(SegProd);
	
	((FluxProd *) sp)->sched = f;
	INSERT_END_FLUXEN(f, sp);
	
	((FluxProd *) sp)->flux_idx = (frole == F_PRIMARY) ? PRIM_SEG_PROD : SEC_SEG_PROD;
		
	((FluxProd *) sp)->all_done = false;

	((FluxProd_FT *) sp)->isPair = isPair;

	((Plan *) sp)->pushPlan = fdataflow->pushPlan;

	((Plan *) sp)->targetlist = copyObject(outer->targetlist);

	((FluxProd *) sp)->tupDesc = get_tupledesc(outer);
	
	((FluxProd *) sp)->node_id = telegraph_nodeid;
	
	((FluxProd_FT *) sp)->m_role = frole;
	
	((FluxProd_FT *) sp)->mode = isStandby ? STANDBY_NORMAL : FLUX_NORMAL;
	
	((FluxProd_FT *) sp)->isStandby = isStandby;

	((FluxProd_FT *) sp)->sync = false;
	
	elog(DEBUG1, "Creating seg prod, isStandby: %d, mode: %d", isStandby, ((FluxProd_FT *) sp)->mode);

	/* Connection information */
	
	((FluxProd *) sp)->nCons = 1;

	((FluxProd *) sp)->fcons_idx = 1;   /* Index of egress on last machine */
	((FluxProd_FT *) sp)->fcons_idx_s = 1; /* Index of egress on last machine */

	init_map_c2m(&(((FluxProd *) sp)->map_c2m), ((FluxProd *) sp)->nCons);

	((FluxProd *)sp)->map_c2m[0] = total_machines + 1;

	init_map_c2m(&(((FluxProd *) sp)->map_m2c), total_machines + 2);

	for (i = 0; i < (total_machines); i++)
	{
		((FluxProd *) sp)->map_m2c[i] = UNKNOWN_CONN_IDX;
	}

	/* (total_machines) is the machine with the spare stuff */
	((FluxProd *) sp)->map_m2c[total_machines] = SPARE_CONN_IDX;

	((FluxProd_FT *) sp)->spare_m = UNKNOWN_MACHINE;

	/* (total_machines + 1) is the machine with the source */
	((FluxProd *) sp)->map_m2c[total_machines + 1] = 0;

	return sp;
}

static Plan *
create_two_groups_w_flux_and_egress_sub(FSched *f, Plan *plan, Query *querytree)
{
	/* At this point, I should have a scan-hgrpb-hgrpt in plan (t for top, and b for bottom) */
	HashGroup *hgt_p, *hgb_p, *hgt_s, *hgb_s;
	StrmScan *sscan;
	PlanPair *sc;
	Plan *tmp;

	SegProd *sp_p, *sp_s;
	
		
	hgt_p = (HashGroup *) plan;

	if (!IsA(hgt_p, HashGroup))
	{
		elog(DEBUG1, "Sorry, expecting hash group-by.");
		return NULL;
	}

	hgb_p = (HashGroup *) outerPlan(hgt_p);
	
	if (!IsA(hgb_p, HashGroup))
	{
		elog(DEBUG1, "Sorry, expecting another hash group-by.");
		return NULL;
	}

	sscan = (StrmScan *) outerPlan(hgb_p);

	if (!IsA(sscan, StrmScan))
	{
		elog(DEBUG1, "Sorry, expecting stream scan.");
		return NULL;
	}

	InitFSchedPlan(f);

	/* This must be created first so the Ingress knows the correct operator to which to connect */
	sc = create_segcons_plan_sub(f, (Plan *) sscan);

	hgt_s = copyObject(hgt_p);
	hgb_s = (HashGroup *) outerPlan(hgt_s);
	
	insert_flux_after(f, (Plan *) hgt_p, (Plan *) hgt_s);

	sp_p = create_seg_prod(f, (Plan *) hgt_p, F_PRIMARY, true, fdataflow->isStandby);
	sp_s = create_seg_prod(f, (Plan *) hgt_s, F_SECONDARY, true, fdataflow->isStandby);

	connect_plans((Plan *) sp_p, (Plan *) hgt_p);
	connect_plans((Plan *) sp_s, (Plan *) hgt_s);

/* #define SLOW_DOWN */

#ifdef SLOW_DOWN
	hgt_p->wndSpecs[0].hop_size = 20;
	hgt_s->wndSpecs[0].hop_size = 20;
#endif
	
	/* Connect ends of dataflow segments, assuming the outer plan of the hash group
	   is a flux cons */
	connect_dataseg_ends((FluxProd_FT *) sp_p, (FluxCons_FT *) outerPlan(hgt_p));
	connect_dataseg_ends((FluxProd_FT *) sp_s, (FluxCons_FT *) outerPlan(hgt_s));

	if (!fdataflow->pushPlan)
	{
		/* Add these guys as segments that need to be scheduled */
		INSERT_END_SEGS(f, sp_p);
		INSERT_END_SEGS(f, sp_s);
	}
	else
	{
		INSERT_END_SEGS(f, sc->top_p);
		INSERT_END_SEGS(f, sc->top_s);
	}
	
	/* Connect the segcons to the bottom */
	connect_plans((Plan *) hgb_p, sc->top_p);
	connect_plans((Plan *) hgb_s, sc->top_s);

	/* Find the first flux prod and connect the ends. */
	tmp = FFANCESTOR((Plan *) hgb_p, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_p);

	tmp = FFANCESTOR((Plan *) hgb_s, FluxProd_FT);
	Assert(tmp);
	connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) sc->top_s);


	if (!fdataflow->pushPlan)
	{
		EndFSchedPlan(f, (Plan *) sp_p);
	}
	else
	{
		EndFSchedPlan(f, outerPlan(hgt_p));
	}
	
	destroy_plan_pair(sc);

	return (Plan *) f;
}

static Plan *
create_two_groups_w_flux_and_egress(FSched *f, Plan *plan, Query *querytree)
{
	plan = pg_convert_all_to_hash_group(plan, querytree);
	
	remove_seqnums_from_group_cols(plan);

	return create_two_groups_w_flux_and_egress_sub(f, plan, querytree);
}

static Plan *
create_two_groups_w_flux_and_egress_lat(FSched *f, Plan *plan, Query *querytree)
{
	plan = pg_convert_all_to_hash_group(plan, querytree);
	
	remove_seqnums_ts_from_group_cols(plan);

	return create_two_groups_w_flux_and_egress_sub(f, plan, querytree);
}

static Plan *
create_two_groups_w_flux_and_egress_lat_double(FSched *f, Plan *plan, Query *querytree)
{
	int i; Plan *tmp;
	
	plan = pg_convert_all_to_hash_group(plan, querytree);
	
	remove_seqnums_ts_from_group_cols(plan);

	plan = create_two_groups_w_flux_and_egress_sub(f, plan, querytree);

	/* Make them all pair less */
	for (i = 0; i < f->nfluxen; i++)
	{
		tmp = f->fluxen[i];
		if (IsA(tmp,FluxProd_FT))
		{
			((FluxProd_FT *) tmp)->isPair = false;
		}
		else if (IsA(tmp,FluxCons_FT) ||
				 IsA(tmp, SegCons))
		{
			((FluxCons_FT *) tmp)->isPair = false;
		}
	}

	return (Plan *) f;
}

static Plan *
create_two_groups_w_flux_and_egress_lat_single(FSched *f, Plan *plan, Query *querytree)
{
	int i; Plan *tmp; int j;
	
	plan = pg_convert_all_to_hash_group(plan, querytree);
	
	remove_seqnums_ts_from_group_cols(plan);

	plan = create_two_groups_w_flux_and_egress_sub(f, plan, querytree);

	/* Secondaries don't need connections */
	for (i = 0; i < f->nfluxen; i++)
	{
		tmp = f->fluxen[i];

		if (IsA(tmp, FluxProd_FT) ||
			IsA(tmp, SegProd))
		{
			if (((FluxProd_FT *) tmp)->m_role == F_SECONDARY)
			{
				((FluxProd *) tmp)->all_done = true;
			}

			((FluxProd_FT *) tmp)->isPair = false;
		}
		
		if (IsA(tmp, FluxCons_FT) ||
			IsA(tmp, SegCons))
		{
			if (((FluxCons_FT *) tmp)->m_role == F_SECONDARY)
			{
				((FluxCons *) tmp)->all_done = true;
			}

			((FluxCons_FT *) tmp)->isPair = false;
		}
	}

	for (i = 0, j = 0; i < f->numSegs; i++)
	{
		tmp = f->segs[i];

		/* Remove it if its a secondary */
		if (IsA(tmp, FluxProd_FT) ||
			IsA(tmp, SegProd))
		{
			if (((FluxProd_FT *) tmp)->m_role == F_SECONDARY)
			{
				continue;
			}
		}
		
		if (IsA(tmp, FluxCons_FT) ||
			IsA(tmp, SegCons))
		{
			if (((FluxCons_FT *) tmp)->m_role == F_SECONDARY)
			{
				continue;
			}
		}

		/* Move it up, cause we need to keep it */
		f->segs[j] = f->segs[i];
		j++;
	}

	f->numSegs = j;

	return (Plan *) f;
}

static Plan *
create_three_groups_w_flux(FSched *f, Plan *plan, Query *querytree)
{
	/* At this point, I should have a scan-hgrpb-hgrpt in plan (t for top, and b for bottom) */
	HashGroup *hgt_p, *hgt_s;
/* 	StrmScan *sscan; */
	Plan *sscan;
	PlanPair *sc;
	Plan *tmp, *pcursor_p, *pcursor_s, *pcons_p, *pcons_s;

	plan = pg_convert_all_to_hash_group(plan, querytree);

	remove_seqnums_from_group_cols(plan);

	connect_parents(plan);
	
	pcursor_p = plan;

	pcursor_s = copyObject(plan);

	InitFSchedPlan(f);

	f->ntups = 25000;

	/* Find the bottom of the plan */
/* 	while (pcursor_p && !(IsA(pcursor_p, StrmScan))) */
	while (pcursor_p && !(IsA_Scan(pcursor_p)))
	{
		pcursor_p = outerPlan(pcursor_p);
		pcursor_s = outerPlan(pcursor_s);
	}

	sscan = pcursor_p;

	/* This must be created first so the Ingress knows the correct operator to which to connect */
	sc = create_segcons_plan_sub(f, sscan);

	/* Move up a level and connect to the segcons */
	pcursor_p = parentPlan(pcursor_p);
	pcursor_s = parentPlan(pcursor_s);

	pcons_p = sc->top_p;
	pcons_s = sc->top_s;
	
	/* Connect the segcons to the bottom */
	connect_plans((Plan *) pcursor_p, pcons_p);
	connect_plans((Plan *) pcursor_s, pcons_s);

	/* pcursor_s should be identical */
	while (parentPlan(pcursor_p))
	{
		hgt_p = (HashGroup *) parentPlan(pcursor_p);
	
		if (!IsA(hgt_p, HashGroup))
		{
			elog(DEBUG1, "Sorry, expecting hash group-by ... sleeping");
			sleep(1000);
			elog(ERROR, "Waited too long.");
			return NULL;
		}

		hgt_s = (HashGroup *) parentPlan(pcursor_s);
		
		if (!IsA(hgt_s, HashGroup))
		{
			elog(DEBUG1, "Sorry, expecting hash group-by ... sleeping");
			sleep(1000);
			elog(ERROR, "Waited too long.");
			return NULL;
		}

		insert_flux_after(f, (Plan *) hgt_p, (Plan *) hgt_s);

		tmp = FFANCESTOR((Plan *) pcons_p, FluxProd_FT);
		Assert(tmp);
		connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) pcons_p);

		tmp = FFANCESTOR((Plan *) pcons_s, FluxProd_FT);
		Assert(tmp);
		connect_dataseg_ends((FluxProd_FT *) tmp, (FluxCons_FT *) pcons_s);

		/* Gets the FluxCons below */
		pcons_p = outerPlan(hgt_p);
		pcons_s = outerPlan(hgt_s);

		pcursor_p = (Plan *) hgt_p;
		pcursor_s = (Plan *) hgt_s;
	}

	if (!fdataflow->pushPlan)
	{
		/* Add these guys as segments that need to be scheduled */
		INSERT_END_SEGS(f, pcursor_p);
		INSERT_END_SEGS(f, pcursor_s);
	}
	else
	{
		INSERT_END_SEGS(f, sc->top_p);
		INSERT_END_SEGS(f, sc->top_s);
	}

	if (!fdataflow->pushPlan)
	{
		EndFSchedPlan(f, (Plan *) pcursor_p);
	}
	else
	{
		EndFSchedPlan(f, outerPlan(pcursor_p));
	}
	
	destroy_plan_pair(sc);

	return (Plan *) f;
}

/* Initialize an empty scheduler */
static FSched *
init_fsched()
{
	FSched	   *f;

	f = makeNode(FSched);
	f->numSegs = 0;
	f->segs = NULL;
	f->nsegs_ent = 0;
	f->topSeg = NULL;
	f->nfluxen = 0;
	f->fluxen = NULL;
	f->nfluxen_ent = 0;
	f->kill_fsched = false;
	f->ddeclust = total_machines; /* For now this is how we'll do it */

	return f;
}

static StateMover *
create_state_mover(FSched * f)
{
	StateMover *sm;

	sm = makeNode(StateMover);
	sm->n_smovers = total_machines + 1; /* One more for the standby */
	sm->fprod.node_id = telegraph_nodeid;

	/* Initialize with identity */
	init_map_c2m(&(((FluxProd *) sm)->map_c2m), sm->n_smovers);
	init_map_c2m(&(((FluxProd *) sm)->map_m2c), sm->n_smovers);

	INSERT_END_SEGS(f, sm);
	INSERT_END_FLUXEN(f, sm);

	/* Ensure the state mover is where it belongs. */
	Assert(f->fluxen[STATE_MOVER_IDX] == (Plan *) sm);

	return sm;
}

static FluxAcceptor *
create_acceptor(FSched * f)
{
	FluxAcceptor *faccept = makeNode(FluxAcceptor);

	f->faccept = faccept;

	INSERT_END_SEGS(f, faccept);

	return faccept;
}

static Plan *
create_main_plan(Plan *plan, Query *querytree)
{
	Plan	   *result;
	FSched	   *f;

	f = init_fsched();

	f->plan.targetlist = copyObject(plan->targetlist);

	switch (is_pair)
	{

		case FP_SINGLE:
			elog(DEBUG1, "Creating non-mirror plan");
			
			/* First convert the group-by aggs into hash-based versions. */
			result = pg_convert_to_hash_group(plan, querytree);

			/* Make sure it is our plan that we want to convert */
			if (!IsA(result, HashGroup))
				return result;

			insert_flux_into_plan_dup(result, f);
			break;

		case FP_PAIR:
			elog(DEBUG1, "Creating mirrored plan");
			
			/* First convert the group-by aggs into hash-based versions. */
			result = pg_convert_to_hash_group(plan, querytree);

			/* Make sure it is our plan that we want to convert */
			if (!IsA(result, HashGroup))
				return result;

			insert_flux_into_plan_dup_better(result, f, is_pair);
			break;

		case FP_PAIR_NO_ACKS:
			elog(DEBUG1, "Creating mirrored w/o acks plan");

			/* First convert the group-by aggs into hash-based versions. */
			result = pg_convert_to_hash_group(plan, querytree);

			/* Make sure it is our plan that we want to convert */
			if (!IsA(result, HashGroup))
				return result;

			insert_flux_into_plan_dup_better(result, f, is_pair);
			break;

		case FP_SINGLE_SOURCE:
			elog(DEBUG1, "Creating non-mirror plan, with SegCons");

			/* First convert the group-by aggs into hash-based versions. */
			result = pg_convert_to_hash_group(plan, querytree);

			/* Make sure it is our plan that we want to convert */
			if (!IsA(result, HashGroup))
				return result;

			insert_flux_into_plan_dup_all(result, f);
			break;

		case FP_PAIR_SOURCE_ALL:
			elog(DEBUG1, "Creating mirror plan, with SegCons");

			/* First convert the group-by aggs into hash-based versions. */
			result = pg_convert_to_hash_group(plan, querytree);

			/* Make sure it is our plan that we want to convert */
			if (!IsA(result, HashGroup))
				return result;

			insert_flux_into_plan_dup_better_all(result, f, is_pair);
			break;

			/* Sorry for screwing up the order in which we send in
			   parameters */
		default:
			elog(DEBUG1, "Calling the function to create a plan: %s.", fdataflow->name);
			(*fdataflow->create_fplan_p) (f, plan, querytree);
			break;
	}

	querytree->isContinuousQuery = true;

	return (Plan *) f;
}

static Plan *
create_source_web_log(FSched *f, Plan *plan, Query *querytree)
{
	/* Note we don't have a acceptor here. */
	FluxProd   *fprod;
	StrmScan   *ascan;
	int			m_id;

	/* Convert to hash group */
	plan = pg_convert_to_hash_group(plan, querytree);
	remove_seqnums_from_group_cols(plan);
	
	/* Get the seq scan */
	ascan = (StrmScan *) outerPlan(plan);

	/* Otherwise we are not doing the right thing */
	Assert(IsA(ascan, StrmScan));

	/*
	 * Target lists here will be correct, only if the proper query is
	 * specified, i.e. select * from strm_name;
	 */
	f->plan.targetlist = ascan->scansock.plan.targetlist;	/* Target lists should
														 * now match. */

	fprod = (FluxProd *) makeNode(FluxIngress);
	((FluxIngress *) fprod)->no_acks = false; /* Process acks */
	((FluxProd_FT *) fprod)->isPair = true;
	((FluxProd_FT *) fprod)->fcons_idx_s = 2; /* Secondary consumer in main plan */
	((FluxProd_FT *) fprod)->spare_m = total_machines;

	fprod->plan.targetlist = f->plan.targetlist;		/* Target lists should
														 * now match. */
	
	fprod->all_done = false;
	fprod->nCons = total_machines;

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	setup_hash_route(plan, plan, fprod);
	
	fprod->node_id = telegraph_nodeid;
	
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);
	init_map_c2m(&(fprod->map_m2c), fprod->nCons);

	connect_plans((Plan *) fprod, (Plan *) ascan);
	
	/* Insert into the plan */
	INSERT_END_SEGS(f, fprod);
	m_id = INSERT_END_FLUXEN(f, fprod);

	elog(DEBUG1, "Source plan, FluxIngress id: %d", m_id);

	f->topSeg = (Plan *) fprod;

	fprod->flux_idx = m_id;
	fprod->fcons_idx = 1;		/* Index of the consumer in the main plan */

	return (Plan *) f;
}

static Plan *
create_source_click_sub(FSched * f, Plan *plan, Query *querytree)
{
	/* Note we don't have a acceptor here. */
	DataGen    *dg = NULL;
	FluxProd   *fprod = NULL;
	StrmScan   *ascan = NULL;
	int			m_id;

	/* Get the stream scan */
	ascan = (StrmScan *) outerPlan(plan);

	/* Otherwise we are not doing the right thing */
	Assert(IsA(ascan, StrmScan));

	/*
	 * Target lists here will be correct, only if the proper query is
	 * specified, i.e. select * from strm_name;
	 */
	f->plan.targetlist = ascan->scansock.plan.targetlist;	/* Target lists should
														 * now match. */

	Assert(fdataflow->isStandby);
	
	fprod = (FluxProd *) makeNode(FluxIngress);

	if (fdataflow->isStandby == 5) /* Double dataflow */
	{
		((FluxIngress *) fprod)->no_acks = true;
		((FluxProd_FT *) fprod)->isPair = true;
	}
	else if (fdataflow->isStandby == 6) /* Single dataflow */
	{
		((FluxIngress *) fprod)->no_acks = true;
		((FluxProd_FT *) fprod)->isPair = false;
	}
	else
	{
		((FluxIngress *) fprod)->no_acks = false; /* Flux dataflow */
		((FluxProd_FT *) fprod)->isPair = true;
	}
	
	((FluxProd_FT *) fprod)->fcons_idx_s = 2; /* Secondary consumer in main plan */
	((FluxProd_FT *) fprod)->spare_m = total_machines;

	fprod->plan.targetlist = f->plan.targetlist;		/* Target lists should
														 * now match. */
	
	fprod->all_done = false;
	fprod->nCons = total_machines;

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	setup_hash_route(plan, plan, fprod);
	
	fprod->node_id = telegraph_nodeid;
	
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);
	init_map_c2m(&(fprod->map_m2c), fprod->nCons);

	switch(fdataflow->isStandby)
	{
		case (1):
			/* Initialize the data generator */
			dg = create_datagen_cstream(ascan, 1, 0, FALSE);
			break;

		case (2):
			dg = create_datagen_tgroup(ascan, 1, 0, FALSE);
			break;

		case (3):
			dg = create_datagen_fgroup(ascan, 1, 0, FALSE);
			break;

		case (4):
		case (5):
		case (6):
			/* Initialize the data generator */
			dg = create_datagen_ts(ascan, 1, 0, FALSE);
			break;

		default:
			elog(DEBUG1, "No datagen plan here! Sleeping ...");
			sleep(1000);
			elog(ERROR, "Exiting ...");
			break;
	}
	
	connect_plans((Plan *) fprod, (Plan *) dg);
	
	/* Insert into the plan */
	INSERT_END_SEGS(f, fprod);
	m_id = INSERT_END_FLUXEN(f, fprod);

	elog(DEBUG1, "Source plan, FluxIngress id: %d", m_id);

	f->topSeg = (Plan *) fprod;

	fprod->flux_idx = m_id;
	fprod->fcons_idx = 1;		/* Index of the consumer in the main plan */

	return (Plan *) f;
}

static Plan *
create_source_click(FSched * f, Plan * plan, Query * querytree)
{
	/* Convert to hash group */
	plan = pg_convert_to_hash_group(plan, querytree);
	remove_seqnums_from_group_cols(plan);
	
	return create_source_click_sub(f, plan, querytree);
}

static void
create_egress_click(FSched * f, Plan * plan, Query * querytree, bool lat)
{
	int m_id;
	FluxEgress *fe;
	FluxCons_FT *fcons;
	
	fe = makeNode(FluxEgress);
	fcons = (FluxCons_FT *) fe;

	if (fdataflow->isStandby == 6) /* Single dataflow */
	{
		fcons->isPair = false;
	}
	else if (fdataflow->isStandby == 5)
	{
		fcons->isPair = true + 1;
	}
	else
	{
		fcons->isPair = true;
	}
	
	/* This really does nothing */
	if (fdataflow->pushPlan)
	{
		((Plan *) fcons)->pushPlan = true;
	}

	Assert(IsA(plan, HashGroup));

	((FluxCons *) fcons)->tupDesc = get_tupledesc(plan);
		
	((FluxCons *) fcons)->all_done = false;
	((FluxCons *) fcons)->node_id  = telegraph_nodeid;

	fcons->m_role = F_PRIMARY;

	fcons->mode = FLUX_NORMAL;

	((FluxCons *) fcons)->sched = f;

	parentPlan(fcons) = NULL;

	((FluxCons *) fcons)->nProd = total_machines;

	init_map_c2m(&(((FluxCons *) fcons)->map_c2m), total_machines);
	fcons->spare_m = total_machines;
	
	init_map_c2m(&(((FluxCons *) fcons)->map_m2c), total_machines + 1);
	((FluxCons *) fcons)->map_m2c[total_machines] = SPARE_CONN_IDX;

	m_id = INSERT_END_FLUXEN(f, fe);

	((FluxCons *) fcons)->flux_idx = m_id;
	((FluxCons *) fcons)->fprod_idx = PRIM_SEG_PROD;		/* FluxIngress operator */
	(fcons)->fprod_idx_s = SEC_SEG_PROD;

	INSERT_END_SEGS(f, fcons);

	if (lat)
	{
		fe->use_ts = true;
		fe->ts_sec_col = 2;
		fe->ts_usec_col = 3;
	}
	else
	{
		fe->use_ts = false;
	}
	
	EndFSchedPlan(f, (Plan *) fe);
}

static Plan *
create_source_and_egress_click_sub(FSched * f, Plan *plan, Query *querytree, bool lat)
{
	Plan *bottom;

	bottom = plan;

	while (outerPlan(bottom) && (!IsA(outerPlan(bottom), StrmScan)))
	{
		bottom = outerPlan(bottom);
	}

	/* Note, these two must be created in this order */
	create_source_click_sub(f, bottom, querytree);
	create_egress_click(f, plan, querytree, lat);

	return (Plan *) f;
}

static Plan *
create_source_and_egress_click(FSched * f, Plan *plan, Query *querytree)
{
	/* Convert to hash group */
	plan = pg_convert_all_to_hash_group(plan, querytree);
	remove_seqnums_from_group_cols(plan);

	return create_source_and_egress_click_sub(f, plan, querytree, false);
}

static Plan *
create_source_and_egress_lat(FSched * f, Plan *plan, Query *querytree)
{
	/* Convert to hash group */
	plan = pg_convert_all_to_hash_group(plan, querytree);
	remove_seqnums_ts_from_group_cols(plan);

	return create_source_and_egress_click_sub(f, plan, querytree, true);
}


static Plan *
create_source_plan_all(FSched * f, Plan *plan, Query *querytree)
{
	/* Note we don't have a acceptor here. */
	DataGen    *dg;
	FluxProd   *fprod = NULL;
	StrmScan   *ascan;
	int			m_id;
	hash_cols   *hcols;
	AttrNumber *attr;
		
	/* Otherwise we are not doing the right thing */
	Assert(IsA(plan, StrmScan));

	ascan = (StrmScan *) plan;

	/*
	 * Target lists here will be correct, only if the proper query is
	 * specified, i.e. select * from strm_name;
	 */
	f->plan.targetlist = ascan->scansock.plan.targetlist;	/* Target lists should
														 * now match. */

	switch (fdataflow->isStandby)
	{
		case 0:
			fprod = makeNode(FluxProd);
			break;

		case 1:
			fprod = (FluxProd *) makeNode(FluxProd_FT);
			((FluxProd_FT *) fprod)->isPair = false;
			break;

		case 2:
			fprod = (FluxProd *) makeNode(FluxIngress);
			((FluxIngress *) fprod)->no_acks = false; /* Process acks */
			((FluxProd_FT *) fprod)->isPair = true;
			((FluxProd_FT *) fprod)->fcons_idx_s = 2;
			((FluxProd_FT *) fprod)->spare_m = total_machines;
			break;
	}

	fprod->plan.targetlist = f->plan.targetlist;		/* Target lists should
														 * now match. */
	
	fprod->all_done = false;
	fprod->nCons = total_machines;
/* 	fprod->route_fun_init = round_robin_route_init; */
/* 	fprod->route_fun      = round_robin_route; */

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	attr = palloc(sizeof(AttrNumber));

	*attr = (AttrNumber) 1;
	
	hcols = (hash_cols *) fprod->rstate;
	hcols->numCols = 1;
	hcols->keyColIdx = attr;
	
/* 	fprod->route_fun      = fixed_mod_route; */

	fprod->node_id = telegraph_nodeid;
	
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);
	init_map_c2m(&(fprod->map_m2c), fprod->nCons);

	/* Initialize the data generator */
	dg = create_datagen(ascan, 1, 0, FALSE);

	connect_plans((Plan *) fprod, (Plan *) dg);
	
	/* Insert into the plan */
	INSERT_END_SEGS(f, fprod);
	m_id = INSERT_END_FLUXEN(f, fprod);

	elog(DEBUG1, "Source plan, FluxIngress id: %d", m_id);

	f->topSeg = (Plan *) fprod;

	fprod->flux_idx = m_id;
	fprod->fcons_idx = 1;		/* Index of the consumer in the main plan */

	return (Plan *) f;
}

Plan *
pg_convert_to_fsched_dup(Plan *plan, Query *querytree)
{
	Plan	   *result;

	elog(DEBUG1, "Folding plan into fsched.");
	elog(DEBUG1, "Version - source working.");
	
	/* Get the streamers in there. */
	result = pg_convert_seqscan_to_strmscan(plan, querytree, true);

	return create_main_plan(result, querytree);
}
