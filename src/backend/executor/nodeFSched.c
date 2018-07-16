/*-------------------------------------------------------------------------
 *
 * nodeFSched.c
 *	  Fjord scheduler.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * DESCRIPTION
 *	  This is supposed to do static scheduling of dataflow segments.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFSched.c,v 1.25 2004/07/02 21:56:28 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFSched.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include "telegraphcq/wrapch.h"
#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>				/* These are included for the networking */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "rqueue/controlq.h"	/* To talk to the front-end */
#include "rqueue/planq.h"
#include "rqueue/rqdest.h"
#include "miscadmin.h"

#define TIMER_THREAD

#ifdef TIMER_THREAD
#include <pthread.h>  /* Sorry, I need a timer thread */
#endif

#include "executor/instrument.h"
#include "telegraphcq/fluxroutines.h"

void		init_fsched_acceptfd(void);
static void processControlQ(FSched * node);
void        *timer_thread(void *atts);

/* Initializes a socket for accepting new connections for the Flux node */
void
init_fsched_acceptfd()
{

	int			enable;
	struct sockaddr_in serv_addr;

	enable = 1;

	if ((fsched_acceptfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		elog(ERROR, "Could not open socket for accepting Flux connections");
		return;
	}

	/* So I don't have to wait until that connection times out */
	if (setsockopt(fsched_acceptfd, SOL_SOCKET, SO_REUSEADDR,
				   (char *) &enable, sizeof(int)) < 0)
	{
		fsched_acceptfd = -1;
		elog(ERROR, "Could not reuse the address for that socket");
		return;
	}

	/* Bind our local address so clients can connect to us */
	memset((void *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(fluxPort);

	if (bind(fsched_acceptfd, (struct sockaddr *) & serv_addr,
			 sizeof(serv_addr)) < 0)
	{
		elog(ERROR, "Could not bind to socket for receiving Flux connections");
		proc_exit(1);			/* I really want to die */
		fsched_acceptfd = -1;	/* Indicate error */
		return;
	}

	listen(fsched_acceptfd, 32);

	/* TBD TBD TBD: Mehul - check this */
	setSocketOptions(fsched_acceptfd);

	elog(DEBUG1, "FSched: listening on port: %d", fluxPort);
	return;
}

/*
 *	Stolen from nodeEddy.
 *
 *	processControlQ
 *
 *	This routine checks the Eddy's Control Queue for any messages from a TCQ-FE
 *	process. Accordingly, the action corresponding to any message is
 *	performed. At this time, the following messages are supported:
 *
 *	   Query Removal
 *
 */
static void
processControlQ(FSched * node)
{

	Controlqentry *cqe = NULL;

	/*
	 * Check for messages in the Control Queue
	 */
	if (dequeue(*p_control_queue,
				(char **) &cqe,
				false,
				&controlqentry_serializer))
	{
		elog(DEBUG1, "FSched: got controlQ message for query id %d ", cqe->data.queryid);

		if (node->qno == cqe->data.queryid) 
		{
			node->kill_fsched = true;
		}
		else
		{
			elog(DEBUG1, "FSched got a request to remove query %d,"
				 " but my qid is: %d", cqe->data.queryid, node->qno);
		}

		pfree(cqe);
	}
}

static void
process_stats_fc(FluxCons_FT * node)
{
	struct timeval end_tv;
	struct timeval dur_tv;

	double		duration1, duration2,
				bw1, bw2;
	uint32		tuplen = 40;	/* For now */
/* 	long		total_count; */

	struct Instrumentation *instr = node->instrument;

	double      avg_resp;

	if (!instr) return;
	
/* 	if (instr->firsttuple == 0) */
/* 		return; */

/* 	total_count = (long) (instr->firsttuple); */

/* 	if (total_count < ((FluxProd *) node)->sched->ntups) return; */

	gettimeofday(&end_tv, NULL);

	timersub(&end_tv, &(instr->starttime), &dur_tv);

	duration1 = TV_IN_MSEC_DOUBLE(dur_tv);
	
/* 	elog(DEBUG1, "%d: FCons stats BEGIN ----", node->m_role); */
/* 	elog(DEBUG1, " %d tups ", ((FluxProd *) node)->sched->ntups * 10); */
/* 	elog(DEBUG1, " %ld total_count", total_count); */
/* 	elog(DEBUG1, " round %.0f ", instr->nloops); */
/* 	elog(DEBUG1, " %.0f tuples sent - primary. ", instr->firsttuple); */
/* 	elog(DEBUG1, " %.0f tuples sent - secondary. ", instr->tuplecount); */
/* 	elog(DEBUG1, " Duration %f ms ", duration); */
/* 	elog(DEBUG1, " tuplen %d ", tuplen); */
	bw1 = ((double) (instr->firsttuple * tuplen) / (double) duration1) * 1.0e3;
/* 	elog(DEBUG1, " Primary BW %f MB/sec \n", bw / ((double) 1024 * 1024)); */
/* 	bw = ((double) (instr->tuplecount * tuplen) / (double) duration) * 1.0e3; */
/* 	elog(DEBUG1, " Secondary BW %f MB/sec \n", bw / ((double) 1024 * 1024)); */
/* 	elog(DEBUG1, " starttime sec: %ld usec: %ld", instr->starttime.tv_sec, */
/* 		 instr->starttime.tv_usec); */
/* 	elog(DEBUG1, " endtime sec: %ld usec: %ld", end_tv.tv_sec, */
/* 		 end_tv.tv_usec); */
/* 	elog(DEBUG1, " dur_time sec: %ld usec: %ld", dur_tv.tv_sec, */
/* 		 dur_tv.tv_usec); */

	bw2 = ((double) (instr->firsttuple) / (double) duration1) * 1.0e3;
/* 	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024)); */
	
/* 	bw = ((double) (instr->tuplecount) / (double) duration) * 1.0e3; */
/* 	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024)); */
	
	timersub(&end_tv, &(instr->counter), &dur_tv);

	duration2 = TV_IN_MSEC_DOUBLE(dur_tv);
	
	instr->tdur = duration2;

/* 	elog(DEBUG1, " TOTAL_DUR sec: %f ms ", duration); */
	
	instr->nloops++;
	instr->firsttuple = 0;
	instr->tuplecount = 0;

	gettimeofday(&instr->starttime, NULL);

/* 	elog(DEBUG1, " new_start sec: %ld usec: %ld", instr->starttime.tv_sec, */
/* 		 instr->starttime.tv_usec); */

/* 	elog(DEBUG1, "%d: FCons stats END ----", node->m_role); */


	avg_resp = 0;
	
	if (IsA(node, FluxEgress) && ((FluxEgress *) node)->use_ts)
	{
		avg_resp = instr->total_resp_time / instr->resp_tuples;
		instr->total_resp_time = 0;
		instr->resp_tuples = 0;
	}

	elog(DEBUG1, "%d - %d: FCons, P-MB/sec: %f P-KTup/sec: %f, avg_resp: %f, span: %f ms, dur: %f ms ms \n",
		 node->m_role,
		 node->fcons.flux_idx,
		 (bw1 / ((double) 1024 * 1024)), (bw2 /((double) 1000)),
		 avg_resp,
		 duration1, duration2);

}

static bool timer_die;

static void
process_stats_fp(FluxProd_FT * node)
{
	struct timeval end_tv;
	struct timeval dur_tv;

	double		duration1, duration2,
				bw1, bw2, bw3, bw4;
	uint32		tuplen = 40;	/* For now */
/* 	long		total_count; */

	struct Instrumentation *instr = node->instrument;

	double local_firsttuple;
	double local_tuplecount;

	TupleTableSlot *scanslot = NULL;
	
	if (!instr) return;

	if (node->fpstate_ft)
	{
		scanslot = node->fpstate_ft->csstate.css_ScanTupleSlot;
		if (!TupIsNull(scanslot))
		{
			instr->m_tuplen = scanslot->val->t_len;
		}
	}

	tuplen = instr->m_tuplen;

	local_firsttuple = instr->firsttuple;
	local_tuplecount = instr->tuplecount;
	
/* 	if (instr->firsttuple == 0) */
/* 		return; */

/* 	total_count = (long) (instr->firsttuple); */

/* 	if (total_count < ((FluxProd *) node)->sched->ntups) return; */

	gettimeofday(&end_tv, NULL);

	timersub(&end_tv, &(instr->starttime), &dur_tv);

	duration1 = ((double) dur_tv.tv_sec * 1.0e3 +
				 (double) dur_tv.tv_usec * 1.0e-3);

/* 	elog(DEBUG1, "%d: FCons stats BEGIN ----", node->m_role); */
/* 	elog(DEBUG1, " %d tups ", ((FluxProd *) node)->sched->ntups * 10); */
/* 	elog(DEBUG1, " %ld total_count", total_count); */
/* 	elog(DEBUG1, " round %.0f ", instr->nloops); */
/* 	elog(DEBUG1, " %.0f tuples sent - primary. ", instr->firsttuple); */
/* 	elog(DEBUG1, " %.0f tuples sent - secondary. ", instr->tuplecount); */
/* 	elog(DEBUG1, " Duration %f ms ", duration); */
/* 	elog(DEBUG1, " tuplen %d ", tuplen); */
	bw1 = ((double) (local_firsttuple * tuplen) / (double) duration1) * 1.0e3;
/* 	elog(DEBUG1, " Primary BW %f MB/sec \n", bw / ((double) 1024 * 1024)); */
	bw2 = ((double) (local_tuplecount * tuplen) / (double) duration1) * 1.0e3;
/* 	elog(DEBUG1, " Secondary BW %f MB/sec \n", bw / ((double) 1024 * 1024)); */
/* 	elog(DEBUG1, " starttime sec: %ld usec: %ld", instr->starttime.tv_sec, */
/* 		 instr->starttime.tv_usec); */
/* 	elog(DEBUG1, " endtime sec: %ld usec: %ld", end_tv.tv_sec, */
/* 		 end_tv.tv_usec); */
/* 	elog(DEBUG1, " dur_time sec: %ld usec: %ld", dur_tv.tv_sec, */
/* 		 dur_tv.tv_usec); */

	bw3 = ((double) (local_firsttuple) / (double) duration1) * 1.0e3;
	bw4 = ((double) (local_tuplecount) / (double) duration1) * 1.0e3;

/* 	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024)); */
	
/* 	bw = ((double) (instr->tuplecount) / (double) duration) * 1.0e3; */
/* 	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024)); */
	
	timersub(&end_tv, &(instr->counter), &dur_tv);

	duration2 = ((double) dur_tv.tv_sec * 1.0e3 +
				(double) dur_tv.tv_usec * 1.0e-3);

/* 	elog(DEBUG1, " TOTAL_DUR sec: %f ms ", duration); */

	instr->tdur = duration2;

	instr->nloops++;
	instr->firsttuple = 0;
	instr->tuplecount = 0;

	gettimeofday(&instr->starttime, NULL);

/* 	elog(DEBUG1, " new_start sec: %ld usec: %ld", instr->starttime.tv_sec, */
/* 		 instr->starttime.tv_usec); */

/* 	elog(DEBUG1, "%d: FCons stats END ----", node->m_role); */

	elog(DEBUG1, "%d - %d: FProd, P-MB/sec: %f, P-KTup/sec: %f, "
		 "S-MB/sec: %f, S-KTup/sec: %f, T-MB/sec: %f, T-KTup/sec: %f, span: %f ms, dur: %f ms \n",
		 node->m_role,
		 node->fprod.flux_idx,
		 (bw1 / ((double) 1024 * 1024)), (bw3 /((double) 1000)),
		 (bw2 / ((double) 1024 * 1024)), (bw4 / ((double) 1000)),
		 ((bw1 + bw2) / ((double) 1024 * 1024)), ((bw3 + bw4) / ((double) 1000)),
		 duration1, duration2);

	if (IsA(node, FluxIngress))
	{
		elog(DEBUG1, "%d - tsb entries, %d - dropped", ((FluxProd_FT *) node)->fpstate_ft->tsb->size, ((FluxProd_FT *) node)->instrument->dropped);
		node->instrument->dropped = 0;
	}
	
	
	if (outerPlan(node) && (nodeTag(outerPlan(node)) == T_HashGroup))
	{
		if ((((HashAgg *) outerPlan(node))->aggstate) &&
			(((HashAgg *) outerPlan(node))->aggstate->hashtable))
		{
			elog(DEBUG1, "%d - hg nentries",
				 ((HashAgg *) outerPlan(node))->aggstate->hashtable->nentries);
		}
	}
	
/* #define KILL_MACHINE */
	
#ifdef KILL_MACHINE
	/* After 10 seconds, kill the third machine */
	if ((instr->tdur >= ((20.0) * 1e03)) && ((telegraph_nodeid) == 3))
	{
		timer_die = true;
	}
#endif

}

void *
timer_thread(void *atts)
{
	FSched *node = (FSched *) atts;
	Plan *runNode;
	int i;
	struct timespec bs1;
	struct timespec bs2;
	
// #define FINE_GRAN
#ifdef FINE_GRAN
	bs1.tv_sec = 0;
	bs1.tv_nsec = 25e7;
#else
	bs1.tv_sec = 1;
	bs1.tv_nsec = 0;
#endif	

	timer_die = false;
	
	while(!(node->kill_fsched))
	{
		for (i = 0; i < node->nfluxen; i++)
		{
			runNode = (node->fluxen)[i];
			if (IsA(runNode, FluxProd_FT) ||
				IsA(runNode, FluxIngress) ||
				IsA(runNode, SegProd))
			{
				process_stats_fp((FluxProd_FT *) runNode);
			}
			else if (IsA(runNode, FluxCons_FT) ||
					 IsA(runNode, SegCons) ||
					 IsA(runNode, FluxEgress))
			{
				process_stats_fc((FluxCons_FT *) runNode);
			}
		}

		if (timer_die)
		{
			elog(FATAL, "Dying, because I was told to.");
		}
		
		nanosleep(&bs1, &bs2);
	}

#ifdef TIMER_THREAD
	pthread_exit(0);
#endif
	
	return NULL;
}

/* ------------------------------------------
 *	 ExecFSched
 *
 *		Does round-robin scheduling for now
 * ------------------------------------------
 */


#ifdef TIMER_THREAD
static pthread_t timer;
#endif

TupleTableSlot *
ExecFSched(FSched * node)
{
	TupleTableSlot *result;
	Plan	   *runNode;

	int			i,
				tupsofar,
				round;
	struct timeval start_tv,
				end_tv,
				dur_tv;
	struct timezone tz;
	double		duration;
	double		bw;
	uint32		tuplen;			/* TBD: mashah - uninitialized automatic
								 * variables */


	
#ifdef TIMER_THREAD
	elog(DEBUG1, "nodeFSched: Creating a timer thread");

	pthread_create(&timer, NULL, timer_thread, (void *) node);
#endif

	tupsofar = 0;
	round = 0;

	result = NULL;

	gettimeofday(&start_tv, &tz);

	while ((result == NULL) && !(node->kill_fsched))
	{

		for (i = 0; i < node->numSegs; i++)
		{

			runNode = (node->segs)[i];
			if (runNode == (node->topSeg))
				result = ExecProcNode(runNode, (Plan *) node);
			else
				ExecProcNode(runNode, (Plan *) node);

		}

		/* Count results from the top-level node, don't pass it on */
		if (!TupIsNull(result))
		{
			tupsofar++;

			/* Output statistics every once in a while */
			if ((tupsofar % (node->ntups)) == 0)
/* 			if (0) */
			{

				gettimeofday(&end_tv, &tz);

				/* Output timing information */
#if __linux__
				timersub(&end_tv, &start_tv, &dur_tv);
#endif

				duration = ((double) dur_tv.tv_sec * 1.0e3 +
							(double) dur_tv.tv_usec * 1.0e-3);

				if (result->val != NULL)
					tuplen = result->val->t_len;
				else
					tuplen = -1;

				elog(DEBUG1, " round %d \n", round);
				elog(DEBUG1, " %d tuples processed. \n", node->ntups);
				elog(DEBUG1, " Duration %f ms \n", duration);
				elog(DEBUG1, " tuplen %d \n", tuplen);

				bw = ((double) (node->ntups * tuplen) / (double) duration) * 1.0e3;

				elog(DEBUG1, " BW %f MB/sec \n", bw / ((double) 1024 * 1024));

				round++;

				gettimeofday(&start_tv, &tz);

			}

		}

		result = NULL;			/* Never want to exit at the momemt */

		processControlQ(node);

/* #define DBG_GPROF */
		
#ifdef DBG_GPROF
		if (tupsofar >= 5000000)
		{
			elog(FATAL, "Cannot continue, must die for gprof stuff.");
		}
#endif
	}

	if (node->kill_fsched)
		return NULL;

	return result;				/* Every once in a while the top node
								 * returns a result */

}

static void
restore_working_directory(int code, char *pathname)
{
	if (pathname != NULL)
		chdir(pathname);
}	/* @EdstemsAD */

static void
fsched_proc_exit(int code, FSched * node)
{
	node->kill_fsched = true;
	elog(DEBUG1, "Set it to TRUE! Changing directories.");
#ifdef TIMER_THREAD
	pthread_join(timer, NULL);
#endif
/* 	pthread_kill_other_threads_np(); */
	restore_working_directory(code, "/home/mashah/research/pgdb/");
}

/* -----------------
 * ExecInitFSched
 *
 * -----------------
 */
bool
ExecInitFSched(FSched * node, EState *estate, Plan *parent)
{
	int			i;
	bool		rbool;

	on_proc_exit(fsched_proc_exit, (Datum) node);

	node->kill_fsched = false;
	
	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	node->ntups = report_freq;	/* I dunno how often! */

/* 	node->ntups = 10000; */

	node->ntups = 100000;

#define FSCHED_NSLOTS 0

	/* Note numSegs is setup beforehand */
	for (i = 0; i < node->numSegs; i++)
	{
		rbool = ExecInitNode((node->segs)[i], estate, (Plan *) node);
		if (!rbool)
			return rbool;		/* If we did not initialize return false */
	}

	return true;
}

int
ExecCountSlotsFSched(FSched * node)
{
	int			i,
				total = 0;

	for (i = 0; i < node->numSegs; i++)
		total += ExecCountSlotsNode((node->segs)[i]);

	elog(DEBUG1, "Need %d slots", total + FSCHED_NSLOTS);

	return total + FSCHED_NSLOTS;
}

/* ------------------------
 *		ExecEndFSched(node)
 *
 * -----------------------
 */
void
ExecEndFSched(FSched * node)
{
	int			i;

	/* Rotate through all the segments and clean them up */
	for (i = 0; i < node->numSegs; i++)
		ExecEndNode((node->segs)[i], (Plan *) node);

	failed_node = FALSE;

	/* Free the resultq */
	FreeQueue(node->qno);

	elog(DEBUG1, "FSched finished for qno: %d", node->qno);

	return;
}

void
set_failed_machine(FSched * node, int machine)
{
	int			i;
	Plan	   *tnode;

	elog(DEBUG1, "FSched: setting machine to failed %d", machine);

	if (failed_node)
	{

		if (which_failed == machine)
		{
			elog(DEBUG1, "Detected another failure of same machine %d",
				 machine);

			TraceDump();

			return;				/* Already know this failed */
		}

		elog(DEBUG1, "Error, detected two failures. Previous machine %d, this "
			 "machine %d", which_failed, machine);

		sleep(2000);

		elog(ERROR, "Waited 2000 seconds ... failing.");
	}

	node->n_recover = 0;

	for (i = 0; i < node->nfluxen; i++)
	{

		tnode = node->fluxen[i];

		switch (nodeTag(tnode))
		{

			case T_SegProd:
			case T_FluxProd_FT:
				if ((((FluxProd_FT *) tnode)->mode != FLUX_NORMAL) &&
					(((FluxProd_FT *) tnode)->mode != STANDBY_NORMAL))
				{
					elog(ERROR, "FSCHED: WRONG MODE");
				}
				
				((FluxProd_FT *) tnode)->mode = FLUX_FAIL_OVER;
				break;

			case T_FluxEgress:
			case T_FluxCons_FT:
				if ((((FluxCons_FT *) tnode)->mode != FLUX_NORMAL) &&
					(((FluxCons_FT *) tnode)->mode != STANDBY_NORMAL))
				{
					elog(ERROR, "FSCHED: WRONG MODE");
				}

				((FluxCons_FT *) tnode)->mode = FLUX_FAIL_OVER;
				break;

			case T_FluxIngress:
				((FluxProd_FT *) tnode)->mode = FLUX_FAIL_OVER;
				break;

			case T_SegCons:
				/* XXX FluxIngress will tell when to do statemovement */
/* 				((FluxCons_FT *) tnode)->mode = FLUX_FAIL_OVER; */
				break;

			case T_StateMover:
				node->n_recover++;		/* State mover recovers by itself */
				break;

			default:
				elog(ERROR, "Cannot set the node to fail-over mode, tag: %d",
					 nodeTag(tnode));
				break;

		}

	}

	failed_node = true;
	which_failed = machine;

}

void
set_recovered_fluxen(FSched * node, int f_idx)
{

	node->n_recover++;

	if (node->n_recover == node->nfluxen)
		failed_node = false;

}
