/*-------------------------------------------------------------------------
 *
 * nodeSeqscan.c
 *	  Support routines for the stream scanner.
 *
 *	  Combines the functionality of the scanner that reads from a
 *	  queue and one that reads from on-disk relations. It is used to
 *	  handle incoming network based sources.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeDataGen.c,v 1.12 2004/06/25 01:27:35 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "executor/execdebug.h"
#include "executor/nodeDataGen.h"
#include "parser/parsetree.h"
#include "executor/execState.h"

/* For gettimeofday() */
#include <sys/time.h>
#include <time.h>

/* Gets the next tuple */
TupleTableSlot *
ExecDataGen(DataGen * node)
{
	TupleTableSlot *rslot = node->cstate->cs_ResultTupleSlot;
	HeapTuple	t;
	int i;

	if (node->seqnum < 0)
	{
		/* If I have a child, execute it ... who knows it may come in handy */
		if (outerPlan(node))
		{
			ExecProcNode(outerPlan(node), (Plan *) node);
		}

		return NULL;
	}
	
	/* If I have a child, execute it ... who knows it may come in handy */
	if (outerPlan(node))
	{
		ExecProcNode(outerPlan(node), (Plan *) node);

		if (node->seqnum < 0) 
		{
			return NULL;
		}
	}
	
	node->values[node->seqno_col] = (node->seqnum)*(node->skip) + (node->offset);

	if (node->use_ts)
	{
		if ((node->seqnum % node->ts_freq) == 0)
		{
			struct timeval tv;
			gettimeofday(&tv, NULL);
			node->values[node->ts_sec] = tv.tv_sec;
			node->values[node->ts_usec] = tv.tv_usec;
		}
		else
		{
			node->values[node->ts_sec] = 0;
			node->values[node->ts_usec] = 0;
		}
	}
	
	node->seqnum++;
	
#ifdef PRNG_H

#ifdef NOT_USED
	node->values[node->rand_col] =
		(randomMT(&(node->prng)) % node->max);

	if (node->rand_col2 > (-1)) 
	{
		node->values[node->rand_col2] =
			(randomMT(&(node->prng)) % node->max2);
	}
#endif

	for (i = 0; i < node->nrandcols; i++)
	{
		node->values[node->rand_col[i]] =
			(randomMT(&(node->prng)) % node->max[i]);
	}
	
#else

#ifdef NOT_USED
	node->values[node->rand_col] =
		(lrand48() % node->max);

	if (node->rand_col2 > (-1)) 
	{
		node->values[node->rand_col2] =
			(lrand48() % node->max2);
	}
#endif

	for (i = 0; i < node->nrandcols; i++)
	{
		node->values[node->rand_col[i]] =
			(lrand48() % node->max[i]);
	}
	
#endif


/* #define DEBUG_VALUES */

#ifdef DEBUG_VALUES
	if (node->debug)
	{
		elog(DEBUG1, "DGgen: S:%ld, V: %ld", node->values[node->seqno_col],
			 node->values[node->rand_col]);
	}
#endif
	
	t = heap_formtuple(node->tupDesc, node->values, node->nulls);

	ExecStoreTuple(t, rslot, InvalidBuffer, true);

	
	return rslot;
}

/* Need slots for both the sequential scan and the queue scanner */

bool
ExecInitDataGen(DataGen * node, EState *estate, Plan *parent)
{
	CommonState *cs = makeNode(CommonState);

	node->plan.state = estate;
	node->cstate = cs;

	node->debug = false;

#define DATAGEN_NSLOTS 1
	ExecInitResultTupleSlot(estate, cs);
	ExecAssignResultType(cs, node->tupDesc, true);

	/* Assume the values and null array is already allocated, and filled. */
	/* TBD TBD TBD - Mehul check the following two lines */
#ifdef PRNG_H
	node->prng.left = -1;
	seedMT(&(node->prng), 4375U); 
#else
	srand48(4375U);
#endif

	/* XXX Sometimes needed for some of my experiments and testing. */
	if (outerPlan(node)) 
	{
		ExecInitNode(outerPlan(node), estate, (Plan *) node);
	}
	
	return TRUE;
}

int
ExecCountSlotsDataGen(DataGen * node)
{
	return ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node)) + DATAGEN_NSLOTS;
}

void
ExecEndDataGen(DataGen * node)
{
	
}

bool
ExecGetStateInitDataGen(DataGen * node)
{
	elog(DEBUG1, "DataGen: get state init");
	
	node->parentDone = false;

	/* Need to freeze the node */
	node->tmp_seqnum = node->seqnum;
	node->seqnum = (-1);
	
	/* Nothing to do, just init state of my parent */
	return ExecGetStateInit(parentPlan(node));
}

#ifndef _USE_PRNG_MT
bool
ExecGetStateNextDataGen(DataGen * node, char * buf, int bsize)
{
	elog(ERROR,"DataGen: cannot transfer the state "
		 " because I cannot copy the state of the pseudo-random number generator");
	
	return FALSE;
}
#else
bool
ExecGetStateNextDataGen(DataGen * node, char * buf, int bsize)
{
	int off_d;
	int32 curr_pos = 0;

	if (!node->parentDone)
	{
		if (!ExecGetStateNext(parentPlan(node), buf, bsize))
			return FALSE;
				
		node->parentDone = true;
		return FALSE;
	}

	elog(DEBUG1, "DataGen: get state next");

    /* Seq num */
	memcpy(buf + curr_pos, &(node->tmp_seqnum), sizeof(int));
	curr_pos += sizeof(int);

	elog(DEBUG1, "DataGen: transferring at seqnum: %d", node->tmp_seqnum);

	memcpy(buf + curr_pos, &(node->offset), sizeof(int));
	curr_pos += sizeof(int);

	elog(DEBUG1, "DataGen: offset: %d", node->offset);
	
	memcpy(buf + curr_pos, &(node->skip), sizeof(int));
	curr_pos += sizeof(int);
							 
	if ((curr_pos + sizeof(prng_mt) + sizeof(int)) >= bsize)
	{
		elog(ERROR, "DataGen: need more space in the buffer to extract!");
	}
	
	/* PRNG */
	memcpy(buf + curr_pos, &(node->prng), sizeof(prng_mt));
	curr_pos += sizeof(prng_mt);

	off_d = (node->prng.next - node->prng.state);

	elog(DEBUG1, "DataGen: prng pointer offset %d", off_d);
	
	memcpy(buf + curr_pos, &(off_d), sizeof(int));
	curr_pos += sizeof(int);
		   
	return TRUE;
}
#endif

bool
ExecGetStateCloseDataGen(DataGen * node)
{
	/* Restore the sequence number */
	node->seqnum = node->tmp_seqnum;

	node->debug = true;

	return ExecGetStateClose(parentPlan(node));
}

bool
ExecInstallStateInitDataGen(DataGen * node)
{
	elog(DEBUG1, "DataGen: install state init");

	node->parentDone = false;

	return ExecInstallStateInit(parentPlan(node));
}

#ifndef _USE_PRNG_MT
bool
ExecInstallStateNextDataGen(DataGen * node, char * buf, int bsize)
{
	elog(ERROR,"Sorry, cannot install the state of data generator"
		 " because I cannot install the state of the pseudo-random number generator.");
	return TRUE;
}
#else
bool
ExecInstallStateNextDataGen(DataGen * node, char * buf, int bsize)
{
	int off_d;
	
	int32 curr_pos = 0;

	if (!node->parentDone)
	{
		if (!ExecInstallStateNext(parentPlan(node), buf, bsize))
			return FALSE;

		node->parentDone = TRUE;
		return FALSE;
	}

	elog(DEBUG1, "DataGen: install state next");

	/* Seq num */
	memcpy(&(node->tmp_seqnum),  buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);

	elog(DEBUG1, "DataGen: starting seqnum at: %d", node->tmp_seqnum);

	memcpy(&(node->offset), buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);

	elog(DEBUG1, "DataGen: offset: %d", node->offset);
	
	memcpy(&(node->skip), buf + curr_pos,  sizeof(int));
	curr_pos += sizeof(int);

	if ((curr_pos + sizeof(prng_mt)) >= bsize)
	{
		elog(ERROR, "DataGen: need more space in the buffer to install!");
	}

	memcpy(&(node->prng), buf + curr_pos, sizeof(prng_mt));
	curr_pos += sizeof(prng_mt);

	memcpy(&(off_d), buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);
	
	elog(DEBUG1, "DataGen: prng pointer offset %d", off_d);

	node->prng.next = node->prng.state + off_d;
		
	return TRUE;
}
#endif

bool
ExecInstallStateCloseDataGen(DataGen * node)
{
	ExecInstallStateClose(parentPlan(node));

	elog(DEBUG1, "DataGen: install state finished");
	
	node->seqnum = node->tmp_seqnum;

	node->debug = true;

	return TRUE;
}

bool
ExecResetStateDataGen(DataGen * node)
{
	return TRUE;
}
