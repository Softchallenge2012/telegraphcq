/*-------------------------------------------------------------------------
 *
 * instrument.h
 *	  definitions for run-time statistics collection
 *
 *
 * Copyright (c) 2001, PostgreSQL Global Development Group
 *
 * $Id: instrument.h,v 1.7 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <sys/time.h>


typedef struct Instrumentation
{
	/* Info about current plan cycle: */
	bool		running;		/* TRUE if we've completed first tuple */
	struct timeval starttime;	/* Start time of current iteration of node */
	struct timeval counter;		/* Accumulates runtime for this node */
	double		firsttuple;		/* Time for first tuple of this cycle */
	double		tuplecount;		/* Tuples so far this cycle */
	/* Accumulated statistics across all completed cycles: */
	double		startup;		/* Total startup time (in seconds) */
	double		total;			/* Total total time (in seconds) */
	double		ntuples;		/* Total tuples produced */
	double		nloops;			/* # of run cycles for this node */
	double      tdur;

	/* @BfluxMS Extra stats for Flux */

	int 				pages_moved;
	struct timeval 		move_start;

  
	bool                use_ts;
	int                 ts_sec_col;
	int                 ts_usec_col;
	double              total_resp_time;
	int                 resp_tuples;

	int                 dropped;

/* 	double              m_tuplen; */
	int                 m_tuplen;

	/* @EfluxMS */
	
} Instrumentation;

extern Instrumentation *InstrAlloc(void);
extern void InstrStartNode(Instrumentation *instr);
extern void InstrStopNode(Instrumentation *instr, bool returnedTuple);
extern void InstrEndLoop(Instrumentation *instr);

#endif   /* INSTRUMENT_H */
