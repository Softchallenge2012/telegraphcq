
/*-------------------------------------------------------------------------
 *
 * nodeScanSock.c
 *	  Support routines for reading tuples directly off a socket, bypassing
 *	  the Wrapper Clearinghouse.
 *
 * TODO:
 *  --> Catalog entry to tell what port to listen on for a given stream.
 *  --> Have the backend accept connections for streams that aren't being
 *      scanned and throw away the data.
 *  --> Archived stream support.
 *  --> Feedback on # tuples processed
 *  --> Punctuation tuples
 *  --> Blocking operation
 *  --> End-of-stream?
 *  --> Query removal...
 *
 * Portions Copyright (c) 2005, Regents of the University of California
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeScanSock.c,v 1.3 2006/02/14 22:34:39 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *		ExecScanSock			Reads the next tuple, if any.
 *		ExecInitScanSock		
 *		ExecEndScanSock		    Closes up any open sockets and throws away 
 *		                        unused tuples.
 *
 */
#include "postgres.h"

#include "executor/executor.h"
#include "executor/nodeScanSock.h"
#include "executor/nodeEddy.h"
#include "parser/parsetree.h"
#include "rqueue/rqdest.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "utils/builtins.h"
#include "access/printtup.h"
            /* For debugtup_lev() */
#include "funcapi.h"
#include "telegraphcq/histogram.h"
#include "telegraphcq/strmsource.h"
#include "telegraphcq/window.h"
#include "access/heapam.h"
#include "telegraphcq/punct.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

/*
 * CONSTANTS
 */

#define REPORT_INTERVAL_USEC 10000

/* Special string that denotes a user-generated puntuation tuple. */
#define PUNCT_STR "\\PUNCT"

static int PUNCT_STR_LEN = -1;
    /* Initialized in ExecInitScanSock() */


/* 
 * PROTOTYPES OF INTERNAL FUNCTIONS 
 */
HeapTuple getNextTup(ScanSock *node);
HeapTuple strToTuple(ScanSock *node, char *str);
void sendProgressReport(ScanSock *node);
HeapTuple getCurPunct(ScanSock *node); 

/*
 * External functions
 */

/* FUNCTION ExecScanSock
 * PRECONDITIONS: <node> is already initialized, and we're listening for
 *      connections on our socket.
 * POSTCONDITIONS: Accepts connections and/or receives tuples.  Returns 
 *      the first tuple that satisfies the quals, or NULL if no such tuple is
 *      available.
 */
TupleTableSlot *
ExecScanSock(ScanSock * node)
{
	ScanSockState *scansockstate;
	EState	   *estate;
	ExprContext *econtext;
	List	   *qual;
	TupleTableSlot *slot;
	bool		dontProject;
	HeapTuple	t = NULL;

    /*
     * Most of this code is based on ExecScanQueue(). 
     */

	/*
	 * Fetch data from node
	 */
	estate = node->plan.state;
	scansockstate = node->scansockstate;
	econtext = scansockstate->cstate.cs_ExprContext;
	qual = node->plan.qual;
	dontProject = node->dontProject;
	slot = scansockstate->sss_ScanTupleSlot;

	/*
	 * Reset per-tuple memory context to free any expression evaluation
	 * storage allocated in the previous tuple cycle.  Note this can't
	 * happen until we're done projecting out tuples from a scan tuple.
	 */
	if (!dontProject)
		ResetExprContext(econtext);

	/*
	 * get a tuple from the access method loop until we obtain a tuple
	 * which passes the qualification.
	 */
	for (;;)
	{
		CHECK_FOR_INTERRUPTS();

        t = getNextTup(node);
        if (NULL != t) 
		{
            /*elog(LOG, "ExecScanSock: Got a tuple.");*/
        }

        if (t == NULL || t->t_len == 0) {
            (void) ExecClearTuple(slot);
        } else {
            (void) ExecStoreTuple(t, slot, InvalidBuffer, true);
		}

		if (TupIsNull(slot))
		{
            /* If you get here, you've got a NULL tuple. */
            /*elog(LOG, "ExecScanSock: NULL tuple.");*/

			if (dontProject)
				return NULL;
			return ExecStoreTuple(NULL,
								  scansockstate->cstate.cs_ProjInfo->pi_slot,
								  InvalidBuffer,
								  true);
		}
         

        /*
         * Now we need to evaluate any expressions that were pushed down into
         * the scan.
         */

		/*
		 * place the current tuple into the expr context
		 */
		econtext->ecxt_scantuple = slot;

		/*
		 * check that the current tuple satisfies the qual-clause
		 *
		 * check for non-nil qual here to avoid a function call to ExecQual()
		 * when the qual is nil ... saves only a few cycles, but they add
		 * up ...
		 */
		if (!qual || ExecQual(qual, econtext, false))
		{
			TupleTableSlot *resultSlot;

			/*
			 * Found a satisfactory scan tuple.
			 *
			 * Form a projection tuple, store it in the result tuple slot and
			 * return it --- unless we find we can project no tuples from
			 * this scan tuple, in which case continue scan.
			 */
			if (!dontProject)
			{
				ExprDoneCond isDone;

				resultSlot = ExecProject(scansockstate->cstate.cs_ProjInfo,
										 &isDone);
				return resultSlot;
			}
			else
				resultSlot = slot;
                elog(DEBUG1,"ScanSock: Fetched a tuple ... \n\n");
                debugtup_lev(resultSlot->val,resultSlot->ttc_tupleDescriptor,NULL,DEBUG1);

            

			return resultSlot;
		} else {

            /*
             * Tuple fails qual, so free per-tuple memory and try again.
             */
            ResetExprContext(econtext);
            return NULL;
        }
	}
}

/* FUNCTION ExecInitScanSock
 * PRECONDITIONS: <node> has been initialized by the planner and
 *      convertPlanToEddy().
 * POSTCONDITIONS: Prepares the runtime state of the node, if it hasn't been
 *      initialized already.
 */
bool
ExecInitScanSock(ScanSock * node, EState *estate, Plan *parent)
{


	ScanSockState *scansockstate;
	bool		useNewSlots = ExecUseNewSlots((Plan *) node, parent);

    /* Largely copied from ExecInitScanQueue() */

	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);

    elog(LOG, "ExecInitScanSock");

    /* This static constant can only be initialized inside a function :( */
    PUNCT_STR_LEN = strlen(PUNCT_STR);

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * Create new runtime state object.
	 */
	scansockstate = makeNode(ScanSockState);

    /* Default initialization */
    scansockstate->generating_timestamps = false;
    scansockstate->sock = NULL;
	node->scansockstate = scansockstate;
    scansockstate->nTupsRead = 0;
    scansockstate->nTupsReported = 0;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &scansockstate->cstate);

#define SCANSOCK_NSLOTS 2

	/*
	 * tuple table initialization
	 */
	if (useNewSlots)
	{
		scansockstate->sss_ScanTupleSlot = MakeTupleTableSlot();
		InitSlotForEddy(scansockstate->sss_ScanTupleSlot);
	}
	else
	{
		scansockstate->sss_ScanTupleSlot =
			ExecAllocTableSlot(estate->es_tupleTable);
	}


	ExecSetSlotDescriptor(scansockstate->sss_ScanTupleSlot,
						  node->tupDesc, false);

	if (node->dontProject)
	{

		scansockstate->cstate.cs_ResultTupleSlot =
			scansockstate->sss_ScanTupleSlot;

	}
	else
	{

		if (useNewSlots)
		{
			ExecInitNewResultTupleSlot(estate, &scansockstate->cstate);
			InitSlotForEddy(scansockstate->cstate.cs_ResultTupleSlot);
		}
		else
			ExecInitResultTupleSlot(estate, &scansockstate->cstate);

		/*
		 * initialize tuple type
		 */
		ExecAssignResultTypeFromTL((Plan *) node, &scansockstate->cstate);
		ExecAssignProjectionInfo((Plan *) node, &scansockstate->cstate);

	}

    scansockstate->end_of_stream = false;

    /*
     * Get information for building tuples from strings. 
     */
    scansockstate->tdesc = 
        scansockstate->sss_ScanTupleSlot->ttc_tupleDescriptor;
    scansockstate->inputMappings = 
        TupleDescGetAttInMetadata(scansockstate->tdesc);

	return TRUE;
}


/*
 * Copied verbatim from ExecCountSlotsScanQueue()
 */
int
ExecCountSlotsScanSock(ScanSock * node)
{
	return ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node)) +
		SCANSOCK_NSLOTS;
}

/* FUNCTION ExecEndScanSock
 * PRECONDITIONS: <node> has been initialized by ExecInitScanSock()
 * POSTCONDITIONS: Frees allocated resources and closes any open sockets.
 */
void
ExecEndScanSock(ScanSock * node)
{
	ScanSockState *scansockstate;

	/*
	 * get information from node
	 */
	scansockstate = node->scansockstate;

	/*
	 * Free the projection info and the scan attribute info
	 */
	ExecFreeProjectionInfo(&scansockstate->cstate);
	ExecFreeExprContext(&scansockstate->cstate);

	/*
	 * clean out the tuple table
	 */
    /* TODO: Should the following two lines be uncommented for some cases? */
	/*ExecClearTuple(scansockstate->cstate.cs_ResultTupleSlot);*/
	/*ExecClearTuple(scansockstate->sss_ScanTupleSlot);*/
	/*ExecFreeSlotIfOwned(scansockstate->cstate.cs_ResultTupleSlot);*/
	if (scansockstate->cstate.cs_ResultTupleSlot !=
		scansockstate->sss_ScanTupleSlot)
		ExecFreeSlotIfOwned(scansockstate->sss_ScanTupleSlot);

    /* Send our open socket, if any, back to the Eddy's socket pool. */
    if (scansockstate->sock && scansockstate->eddy 
            && scansockstate->sock->fd > 0) {
        scansockstate->eddy->unscannedSocks = 
            lcons(scansockstate->sock, scansockstate->eddy->unscannedSocks);
    }
}

SockState *ScanSockGetSocket(ScanSock *node) {
    return node->scansockstate->sock;
}

/* The third argument allows us to give up control of our socket to the Eddy's
 * socket pool when we're deleted. */
void ScanSockSetSocket(ScanSock *node, SockState *sock, Eddy *e) {
    node->scansockstate->sock = sock;
    node->scansockstate->eddy = e;
}





/*
 * INTERNAL FUNCTIONS
 */

HeapTuple getNextTup(ScanSock *node) 
{
    ScanSockState *sss = node->scansockstate;
    char *nextLine;

    
    if (NULL == sss->sock) {
        /* no network --> no data */
        return NULL;
    }
   
    /* Attempt to "top off" our read buffer */
    fillReadBuf(sss->sock);


    /* Get the next line. */
    nextLine = getNextLine(sss->sock);

    if (NULL == nextLine) {
        /* No data */

        /* Check to see if the socket has been closed by fillReadBuf().
         * If it has, we can now throw away the socket data, since we've
         * read all the tuples. */
        if (-1 == sss->sock->fd) {
            elog(LOG, "ScanSock %p has processed all socket data",
                    node);
            pfree(sss->sock);
            sss->sock = NULL; 
        }

        if (sss->generating_timestamps) {
            /* TODO: Check to see if we should generate a punctuation tuple. */
        }

        return NULL;
    }

    /*elog(LOG, "getNextTup: Got line '%s'", nextLine); */

    /* Keep the sender informed of our progress. */
    sss->nTupsRead++;
    sendProgressReport(node);

    /* SPECIAL CASE: The sender has requested a punctuation tuple */
    if (0 == strncmp(nextLine, PUNCT_STR, PUNCT_STR_LEN)) {

        if (',' == nextLine[PUNCT_STR_LEN]) {
            /* The user sent us a comma, followed by the timestamp */
            char *ts_str = nextLine + PUNCT_STR_LEN;
            Timestamp ts = DatumGetTimestamp(
                    DirectFunctionCall3(timestamp_in, 
                        CStringGetDatum(ts_str),
                        ObjectIdGetDatum(InvalidOid),
                        Int32GetDatum(-1)));

            Assert(!sss->generating_timestamps);

            return form_punct_tup(sss->tdesc, ts);

        } else {
            Assert(sss->generating_timestamps);
    
            return getCurPunct(node);
        }
    }
    /* END SPECIAL CASE */

    
    /* Now we can do the conversion. */
    return strToTuple(node, nextLine);
}

/*
 * Give the sender an update on how many tuples we've processed, if enough time
 * has passed.
 */
void sendProgressReport(ScanSock *node) 
{
    ScanSockState *sss = node->scansockstate;

    char buf[256];

    int diff = sss->nTupsRead - sss->nTupsReported;
    bool ret;
    int len;
    int64 usec_elapsed;
    struct timeval now;

    Assert(diff >= 0);

    /* Don't need to send a message if there's no progress. */
    if (0 == diff) return;

    /* Don't need to send a message if not enough time has elapsed. */
    gettimeofday(&now, NULL);
    usec_elapsed = tv_diff(&(sss->lastReportTime), &now);

    if (usec_elapsed < REPORT_INTERVAL_USEC) return;

    len = snprintf(buf, sizeof(buf), "%d tuples consumed\n", sss->nTupsRead);

    ret = lossyWrite(sss->sock, buf, len);

    /* The write may fail if the sender is ignoring our messages. */
    if (false == ret) {
        sss->nTupsReported = sss->nTupsRead;
        sss->lastReportTime = now;
            /* struct assignment */
    }
}

/*
 * Generate a punctuation tuple for the current time
 */
HeapTuple getCurPunct(ScanSock *node) 
{
    ScanSockState *sss = node->scansockstate;
    struct timeval tp;
    char templ[100];
    char tsbuf[100];
    Timestamp ts;

    /* We generate a timestamp string to be converted below. */
    /* Copied from timeofday() in nabstime.c */
    gettimeofday(&tp, NULL);
    strftime(templ, sizeof(templ), "%a %b %d %H:%M:%S.%%06d %Y %Z",
             localtime((time_t *) &tp.tv_sec));
    snprintf(tsbuf, sizeof(tsbuf), templ, tp.tv_usec);

    /*elog(LOG, "Generating timestamp from '%s'", tsbuf);*/

    ts = DatumGetTimestamp(DirectFunctionCall3(timestamp_in, 
                        CStringGetDatum(tsbuf),
                        ObjectIdGetDatum(InvalidOid),
                        Int32GetDatum(-1)));
 
    return form_punct_tup(sss->tdesc, ts);
}

/*
 * Convert a comma-delimited string to a HeapTuple.
 *
 * Modifies its second argument, strtok()-style...
 */
HeapTuple strToTuple(ScanSock *node, char *str) 
{
    ScanSockState *sss = node->scansockstate;
    MemoryContext perTupleCxt = sss->cstate.cs_ExprContext->ecxt_per_tuple_memory;
    MemoryContext origCxt;
    Datum *datums;
    char **strs;
    char *nulls;
        /* String with ' ' for non-null and 'n' for null column */
    int nattrs = sss->tdesc->natts;
    int nstrs = 0;
    int i;
    char tsbuf[100];
        /* Buffer for creating at timestamp string. */
    HeapTuple ret = NULL;

    /* We're about to do a bunch of allocations to create this tuple, so switch
     * to the per-tuple memory context. */
    origCxt = MemoryContextSwitchTo(perTupleCxt);

    /* Code based on csv wrapper... */
    datums = palloc(nattrs * sizeof(Datum));
    strs = palloc(nattrs * sizeof(Datum));
    nulls = palloc(nattrs + 1);

    nulls[nattrs] = '\0';

    /* Tokenize the string. */
    nstrs = 1;
    strs[0] = str;
    for (i = 0; str[i] != '\0'; i++) {
        if (',' == str[i]) {
            nstrs++;

            if (nstrs > nattrs) {
                elog(ERROR, "Read too many columns.");
            }

            str[i] = '\0';
            strs[nstrs-1] = str + (i + 1);
        }
    }

    /* Check to see if we should be generating timestamps. */
    if (nstrs == nattrs - 1) {
        sss->generating_timestamps = true;
    }
    
    if (sss->generating_timestamps) {
        struct timeval tp;
        char		templ[100];
        int         ts_ix = get_timestamp_col(sss->tdesc) - 1;
        char        *tmp;

        if (nstrs == nattrs) {
            elog(ERROR, 
                "Received an explicit timestamp while generating timestamps");
        } 

        /* We generate a timestamp string to be converted below. */
        /* Copied from timeofday() in nabstime.c */
        gettimeofday(&tp, NULL);
        strftime(templ, sizeof(templ), "%a %b %d %H:%M:%S.%%06d %Y %Z",
                 localtime((time_t *) &tp.tv_sec));
        snprintf(tsbuf, sizeof(tsbuf), templ, tp.tv_usec);

        /* Put the timestamp in the appropriate place, moving everything
         * after that over by one column. */
        tmp = strs[ts_ix];
        strs[ts_ix] = tsbuf;
        for (i = ts_ix + 1; i < nattrs; i++) {
            char *ptr = strs[i];
            strs[i] = tmp;
            tmp = ptr;
        }
        nstrs++;
    }

    if (nstrs != nattrs) {
        elog(ERROR, "Read wrong number of fields (%d instead of %d)",
                nstrs, nattrs);
    }
    
    /* Convert the strings to binary values of the appropriate types. */
    for (i = 0; i < nattrs; i++) {
        bool wasNull;
        datums[i] = DatumFromTypeInstance(sss->inputMappings, 
                i + 1, /* Postgres column numbers are 1-based... */
                strs[i], perTupleCxt, &wasNull);

        nulls[i] = wasNull ? 'n' : ' ';
    }

    /* Yes, we allocate this tuple in the per-tuple context.  It will be copied
     * into the module's output slot before the context is reset. */
    ret = heap_formtuple(sss->tdesc, datums, nulls);

    MemoryContextSwitchTo(origCxt);

    return ret;
}


