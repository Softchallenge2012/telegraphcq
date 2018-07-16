/*-------------------------------------------------------------------------
 *
 * triageq.c
 *
 * AUTHOR: Fred Reiss
 * DESCRIPTION: The Triage Queue -- a priority queue of tuples that summarizes
 *      less-important elements when it fills up.  Used for load-shedding in
 *      TelegraphCQ.
 *
 *      The API of the triage queue is intentionally similar to that of the
 *      TelegraphCQ result queue (See resultq.c).  The main difference is that
 *      triage queues don't use serializers, as they don't need to pass
 *      anything between processes.  
 *
 *      NOTE: Triage queues don't currently do any locking, so make sure you
 *      avoid having different threads of control service the two ends of the
 *      queue!
 *
 * Copyright (c) 2004,2005, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/triageq.c,v 1.21 2005/07/27 20:04:12 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "telegraphcq/triageq.h"

#include "catalog/pg_type.h"
    /* For TIMESTAMPOID constant. */

#include "executor/executor.h"
    /* For ExecEvalExpr() */

#include "miscadmin.h"
    /* For load_shedding_disable_kept_summaries global */

#include "nodes/makefuncs.h"
    /* For makeConst() */

#include "nodes/nodeFuncs.h"
    /* For replace_opid() */

#include "parser/parse_node.h"
    /* For make_op() */

#include "telegraphcq/histogram.h"
    /* For get_timestamp_col() */

#include "telegraphcq/window.h"
    /* For functions that do window math. */

#include "telegraphcq/punct.h"
    /* For form_punct_tup() */

#include "telegraphcq/wrapch.h"
    /* For wch_tmpcontext */

/*******************************************************************************
 * DEBUGGING
 ******************************************************************************/

/* Uncomment the following line to debug the window expressions that the Triage
 * Queue uses to determine when to emit summaries. */
/*#define DEBUG_TRIAGE_TIMING*/

#ifdef DEBUG_TRIAGE_TIMING

#include <execinfo.h>
    /* For backtrace() */

static char * ts_cstr(Timestamp ts);
/* Convenience debugging function to get the string representation of a
 * timestamp. Leaks memory! */
static char * ts_cstr(Timestamp ts) 
{
    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out, TimestampGetDatum(ts)));
}

static char * interval_cstr(Interval i);
static char * interval_cstr(Interval i) 
{
    return DatumGetCString(DirectFunctionCall1(interval_out,
                        IntervalPGetDatum(&i)));
}

/* Check to put at the beginning of every function that takes a timestamp, to
 * verify that the Triage Queue sees timestamps come in in the proper order. */
static void CHECK_TS_ORDER(TriageQueue *q, Timestamp ts) {
    void *stack[100];
    size_t		stack_size;

    /* Generate a trace of who called functions that take a timestamp, so we
     * can figure out where the discrepancy came from. */
    stack_size = backtrace(stack, 100);
    elog(LOG, "Triage Queue passed ts %s from %p", ts_cstr(ts), stack[2]);

    if (!TIMESTAMP_IS_NOBEGIN(q->__DEBUG_most_recent_ts)) {
        if (ts_less_than(ts, q->__DEBUG_most_recent_ts)) {
            elog(ERROR, "Triage Queue %p received timestamps out of order\n"
                    "    (%s < %s)",
                    q, ts_cstr(ts), ts_cstr(q->__DEBUG_most_recent_ts));
        }
    }
    q->__DEBUG_most_recent_ts = ts;
}
#else
#define CHECK_TS_ORDER(q,ts)
#endif /* DEBUG_TRIAGE_TIMING */


/*******************************************************************************
 * INTERNAL CONSTANTS
 ******************************************************************************/

/* The following is initialized on first call to triage_create() using the
 * indicated constant */
/*
#define SUM_TIME_STR "0.0001 sec" 
static Interval *SUM_TIME_PER_TUP = NULL;
*/
    /* How long it takes to summarize a tuple. */

#define FULL_TIME_STR "0.001 sec" 
static Interval *FULL_TIME_PER_TUP = NULL;

#define SHADOW_TIME_STR "0.01 sec" 
static Interval *SHADOW_TIME = NULL;
    /* Time to push a summary through the shadow query. */

static Interval *ZERO_SECONDS = NULL;

#ifdef HAVE_INT64_TIMESTAMP
static Timestamp EPSILON = INT64CONST(1);
#else
static Timestamp EPSILON = 1.0e-6;
#endif

#define AGG_TIME_STR "0.0002 sec" 
static Interval *AGG_TIME_PER_TUP = NULL;

/*#define MAX_TUP_RATE (5000.0 - 1000.0)*/
    /* The maximum number of tuples per second that the system can meet its
     * delay constraint for. 
     * This should satisfy the equation:
     * max summarization rate = max data rate + max full QP rate
     * *e.g.*
     * max_data_rate = max summarization rate - max full QP rate
     */
            
/*******************************************************************************
 * PROTOTYPES OF INTERNAL FUNCTIONS
 ******************************************************************************/


bool is_initialized(TriageQueue *q);

bool is_full(TriageQueue *q);

bool is_empty(TriageQueue *q);

int next_index(TriageQueue *q, int prev);

int prev_index(TriageQueue *q, int next);

int index_diff(TriageQueue *q, int first, int last);

int num_tups_sent_from_cur_epoch(TriageQueue *q);

int num_tups_from_cur_epoch(TriageQueue *q);

SummaryTuples* get_synopsis(TriageQueue *q, StreamSynopsisGen *gen, 
        generic_stream_state **state_ptr_ptr);
        

Var *create_timestamp_accessor(TupleDesc desc);

Expr *create_window_expr(Const *windowsize);

Const *get_ts_ptr(Expr *windowexpr, bool get_newer);

void copy_timestamp_val(TupleDesc desc, HeapTuple tup, Const *dest); 

void synopsis_insertion_delay(void);

int drop_rest_of_summary_window(TriageQueue *q);

void drop_oldest(TriageQueue *q);

char * now_cstr(void);

int window_no(Timestamp epochstart, Interval *epochlen, Timestamp ts);

Timestamp simple_window_end(Timestamp epochstart, Interval *epochlen, 
        int epochnum);

bool simple_ts_past_window(Timestamp ts, Timestamp epochstart,
        Interval *epochlen, int epochnum);


/*******************************************************************************
 * EXTERNAL FUNCTIONS
 ******************************************************************************/

/* FUNCTION triage_create
 * ARGUMENTS: <qlen> is a queue length, in elements.  
 *      <syn_gen> is an object that will generate synopses 
 *      <input_desc> describes the tuples we'll be sending through this queue. 
 *      <summary_desc> describes the summary tuples we'll return.
 *      <epochstart_p> points to the timestamp at which the first window for 
 *          generating summaries starts.  This timestamp may not be set yet,
 *          but it MUST be set before the first call to triage_enqueue().
 *      <windowsize> is the interval at which to generate summaries
 *      <time_constr> is how late we are allowed to deliver a tuple.
 * PRECONDITIONS: You're in the memory context you want this queue to occupy.
 * POSTCONDITIONS: Creates and returns a new triage queue.  Will never attempt
 *      to free <gen>.  Doesn't free <desc> either, but makes a copy for its
 *      own use.
 */
TriageQueue *triage_create(int qlen, StreamSynopsisGen *gen, 
        const TupleDesc input_desc, const TupleDesc summary_desc,
        Timestamp *epochstart_p, const Interval *windowsize, 
        const Interval *time_constr)
{
    TriageQueue *ret = NULL;
    int         buf_len = -1;

    Assert(qlen > 0);
    Assert(gen != NULL);

    /* This constant can't be initialized statically, so we set it here. */
    if (NULL == FULL_TIME_PER_TUP) {
        /* We need to allocate this constant in a persistent memory context so
         * it doesn't go away when this connection closes. */
        MemoryContext origcxt = MemoryContextSwitchTo(TopMemoryContext);
        { /* Begin top level memory context. */
            /*
            SUM_TIME_PER_TUP = 
                DatumGetIntervalP(
                    DirectFunctionCall3(interval_in,
                        CStringGetDatum(SUM_TIME_STR),
                        ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));
                        */

            FULL_TIME_PER_TUP = 
                DatumGetIntervalP(
                    DirectFunctionCall3(interval_in,
                        CStringGetDatum(FULL_TIME_STR),
                        ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));

            SHADOW_TIME = 
                DatumGetIntervalP(
                    DirectFunctionCall3(interval_in,
                        CStringGetDatum(SHADOW_TIME_STR),
                        ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));

            AGG_TIME_PER_TUP = 
                DatumGetIntervalP(
                    DirectFunctionCall3(interval_in,
                        CStringGetDatum(AGG_TIME_STR),
                        ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));

            ZERO_SECONDS = 
                DatumGetIntervalP(
                    DirectFunctionCall3(interval_in,
                        CStringGetDatum("0 seconds"),
                        ObjectIdGetDatum(InvalidOid), Int32GetDatum(-1)));

        } /* End top level memory context. */
        MemoryContextSwitchTo(origcxt);
    }

    ret = palloc(sizeof(TriageQueue)); 
    ret->qlen = qlen;
    ret->nelem = 0;
    ret->next_enqueue_ix = ret->next_dequeue_ix = 0;

    /* Allocate the circular buffer. */

    buf_len = qlen * (sizeof *(ret->buf));
    ret->buf = palloc(buf_len);
    memset(ret->buf, 0x0, buf_len);

    /* Initialize the synopses. */
    ret->syn_gen = gen;

    /* We don't own the TupleDesc pointer. */
    ret->input_desc = CreateTupleDescCopyConstr(input_desc);
    ret->summary_desc = CreateTupleDescCopyConstr(summary_desc);

    ret->drop_sum_state = getSummarizer(gen, ret->input_desc, 
            ret->summary_desc);
    ret->keep_sum_state = getSummarizer(gen, ret->input_desc, 
            ret->summary_desc);

    /* Start our accounting for when to spit out summaries. */
    ret->epochstart_p = epochstart_p;
        /* Timestamps are passed by value. */

    ret->epochlen = *windowsize;
        /* Intervals are passed by reference. */

    ret->epochnum = 1;
        /* Summary windows are 1-based, and we start outputting summaries
         * immediately, even if we don't get any tuples. */
#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "triage_create(): Initializing epochnum for %p to %d",
            ret, ret->epochnum);
#endif /* DEBUG_TRIAGE_TIMING */

    ret->next_epoch_ix = TQ_NOT_AN_INDEX;

    ret->time_cstr_p = (Interval*)time_constr;

    ret->nsent_this_epoch = 0;

    ret->state = TQS_NO_TS;

#ifdef DEBUG_TRIAGE_TIMING
    TIMESTAMP_NOBEGIN(ret->__DEBUG_most_recent_ts);
#endif /* DEBUG_TRIAGE_TIMING */
    return ret;
}


/* FUNCTION triage_destroy
 * ARGUMENTS: <q> is an allocated queue.
 * POSTCONDITIONS: Destroys the queue and frees all its contents.
 */
void triage_destroy(TriageQueue *q) {
	void *drop_tmp_ptr;
	void *keep_tmp_ptr;

    Assert(is_initialized(q));

    /* Empty out the queue first. */
    while ( ! is_empty(q)) {
        HeapTuple ht = triage_dequeue(q);
        heap_freetuple(ht);
    }


    /* <drop_sum_state> and <keep_sum_state> don't hold the synopses
     * themselves, just the status of building them.  So we use the API to
     * clear them out. */
    drop_tmp_ptr = generic_stream_done(q->drop_sum_state);
    keep_tmp_ptr = generic_stream_done(q->keep_sum_state);

    /* SPECIAL CASE: The sample summary has a special destructor function. */
    if (ST_SAMPLE == q->drop_sum_state->stype) {
        sample_destroy((reservoir_sample*)drop_tmp_ptr);    
        sample_destroy((reservoir_sample*)keep_tmp_ptr);    
    }
    /* END SPECIAL CASE */
    else {
        pfree(drop_tmp_ptr);
        pfree(keep_tmp_ptr);
    }

    /* q->syn_gen doesn't belong to us, so don't free it. */

    /* q->time_cstr_p also doesn't belong to us. */

    /* Now it's safe to wipe out the memory. */
    memset(q->buf, 0x0, q->qlen * sizeof(HeapTuple));
    pfree(q->buf);

    memset(q, 0x0, sizeof(TriageQueue));
    pfree(q);
}

/* FUNCTION triage_size
 * POSTCONDITIONS: Returns the number of tuples in a queue.
 */
int triage_size(TriageQueue *q) {
    Assert(is_initialized(q));

    return (q->nelem);
}

/* FUNCTION triage_enqueue
 * ARGUMENTS: <q> points to a triage queue.  <tup> points to a tuple to enqueue.
 * PRECONDITIONS: <q> has been initialized with triage_create().
 *      <tup> is not NULL.
 *      The summary window starting time (e.g. *q->epochstart_p) has been set.
 * POSTCONDITIONS: Attempts to insert the tuple into the queue, performing
 *      triage if it runs out of space.  In the event that this tuple gets 
 *      dropped, summarizes it.
 */
void triage_enqueue(TriageQueue *q, HeapTuple tup) {

    /* Check arguments. */
    Assert(tup != NULL);
    Assert(is_initialized(q));
    CHECK_TS_ORDER(q,get_timestamp_val(q->input_desc,tup));

    /* Make sure that the caller set the start of the summary windows before
     * calling us. */
    Assert(DT_NOBEGIN != *(q->epochstart_p));

    /* PULL wrappers use tuples with all NULLs in non-binding columns to 
       indicate sucessful delivery of results for a lookup.  This breaks the 
       following code.
    */

#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "triage_enqueue(): Enqueueing tuple with ts %s",
            ts_cstr(get_timestamp_val(q->input_desc, tup)));
#endif /* DEBUG_TRIAGE_TIMING */


    /* SPECIAL CASE: This is the first tuple enqueued. */
    if (q->state == TQS_NO_TS) {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "triage_enqueue(): First tuple has ts %s",
                ts_cstr(get_timestamp_val(q->input_desc, tup)));
#endif /* DEBUG_TRIAGE_TIMING */

        /* Advance the summary window to the current epoch. */
        q->epochnum = window_no(*(q->epochstart_p), &(q->epochlen), 
                get_timestamp_val(q->input_desc, tup));

#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "    Set epochnum to %d.", q->epochnum);
#endif /* DEBUG_TRIAGE_TIMING */

        q->state = TQS_HAVE_TS;
    }
    /* END SPECIAL CASE */


    /* SPECIAL CASE: Ran out of space in the queue.
     * This usually happens because the query processor is not pulling any
     * tuples from the stream or the delay tolerance was set really high. */
    if (is_full(q)) {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "triage_enqueue(): Queue is full.  Dropping oldest tuple.");
#endif /* DEBUG_TRIAGE_TIMING */

        /* Drop the oldest tuple in the queue, e.g. the first one, in the hopes
         * that we will be able to deliver the younger tuples on time.
         */
        drop_oldest(q); 

        /* Now there's room to insert into the queue. */
        triage_enqueue(q, tup);
        return;
    }
    /* END SPECIAL CASE */

    /* If we get here, there's space in the queue for this tuple. */
    /*
    elog(LOG, "triage_enqueue(): Inserting at index %d.",
            q->next_enqueue_ix);
            */

    /* Check to see whether this tuple is the first tuple from the next
     * summary window. */
    if (TQ_NOT_AN_INDEX == q->next_epoch_ix) {
        /* Every tuple currently in the queue is from the current window;
         * check to see whether this tuple is too. */
        if (simple_ts_past_window(get_timestamp_val(q->input_desc, tup),
                    *(q->epochstart_p), &(q->epochlen), q->epochnum)) {
            q->next_epoch_ix = q->next_enqueue_ix;
#ifdef DEBUG_TRIAGE_TIMING
            elog(LOG, "enqueue(): Setting next_epoch_ix to %d",
                    q->next_epoch_ix);
            elog(LOG, "    Tuple ts = %s; window end = %s",
                    ts_cstr(get_timestamp_val(q->input_desc, tup)),
                    ts_cstr(simple_window_end(*(q->epochstart_p), 
                            &(q->epochlen),
                            q->epochnum)));
#endif /* DEBUG_TRIAGE_TIMING */

        } else {
            if (window_no(*(q->epochstart_p), &(q->epochlen), 
                get_timestamp_val(q->input_desc, tup)) != q->epochnum) 
            {
                elog(ERROR, "Got tuple for window %d during window %d",
                    window_no(*(q->epochstart_p), &(q->epochlen), 
                        get_timestamp_val(q->input_desc, tup)),
                        q->epochnum);
            }
        }
    }
    
    q->buf[q->next_enqueue_ix] = tup;
    q->next_enqueue_ix = next_index(q, q->next_enqueue_ix);
    q->nelem++;

    /* Drop tuples that we won't have time to process. */
    /*elog(LOG, "Cleanup of expired from ENQUEUE");*/
    triage_cleanup_expired_tuples(q, get_timestamp_val(q->input_desc, tup));
}

TupleDesc triage_get_tup_desc(TriageQueue *q) {
    Assert(is_initialized(q));
    return q->input_desc;
}


#if 0
/* FUNCTION triage_just_summarize
 * ARGUMENTS: <q> points to a triage queue.  <tup> points to a tuple.
 * PRECONDITIONS: <q> has been initialized with triage_create().
 *      <tup> is not NULL.
 * POSTCONDITIONS: Adds the tuple to the "kept" summary without bothering to
 *      insert it into the queue.
 */
void triage_just_summarize(TriageQueue *q, HeapTuple tup) {
    /* Check arguments. */
    Assert(tup != NULL);
    Assert(is_initialized(q));

    /* Manage our stored timestamps. */

    /* SPECIAL CASE: Haven't set the oldest timestamp. */
    if (q->state == TQS_NO_TS) {
        copy_timestamp_val(q->input_desc, tup, get_ts_ptr(q->windowexpr, false)); 

        q->state = TQS_HAVE_TS;
    }
    /* END SPECIAL CASE */

    copy_timestamp_val(q->input_desc, tup, get_ts_ptr(q->windowexpr, true)); 

    /* Don't need to make a copy of the tuple. */
    synopsis_insertion_delay();
    q->syn_gen->nextf(q->keep_sum_state, tup);
}
#endif /* 0 */

/* FUNCTION triage_dequeue
 * ARGUMENTS: <q> points to a triage queue.
 * PRECONDITIONS: The queue is initialized with triage_create().
 * POSTCONDITIONS: If there is anything in the queue, dequeues and returns the
 *      first element.  Otherwise, returns NULL.
 */
HeapTuple triage_dequeue(TriageQueue *q) {
    HeapTuple   ret;

    /* Check arguments. */
    Assert(is_initialized(q));

    /* SPECIAL CASE: Empty queue. */
    if (is_empty(q)) {
        /*
        elog(LOG, "triage_dequeue(): Queue at %p is empty (nelem is %d)",
                q, q->nelem);
                */

        return NULL;
    }

    /* END SPECIAL CASE */

    ret = q->buf[q->next_dequeue_ix];
  
#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "triage_dequeue(): Dequeueing tuple with ts %s from index %d.",
            ts_cstr(get_timestamp_val(q->input_desc, ret)),
            q->next_dequeue_ix);
#endif /* DEBUG_TRIAGE_TIMING */

    /* The tuple we dequeue had better be from the current summary window! */
    if (window_no(*(q->epochstart_p), &(q->epochlen), 
            get_timestamp_val(q->input_desc, ret)) != q->epochnum)
    {
        elog(ERROR, "Tried to dequeue a tuple from the wrong summary window\n"
                "    (current summary window is %d; tuple's is %d)",
                q->epochnum, window_no(*(q->epochstart_p), &(q->epochlen), 
            get_timestamp_val(q->input_desc, ret)));
    }


    q->next_dequeue_ix = next_index(q, q->next_dequeue_ix);
    q->nelem--;

    if (!load_shedding_disable_kept_summaries) {
        HeapTuple copy = heap_copytuple(ret);
        /*
        elog(LOG, "Inserting tuple copy %p into kept summary.",
                copy);
                */
        /* Make a copy of this tuple and send it to the summary. */
        synopsis_insertion_delay();
        generic_stream_next(q->keep_sum_state, copy);
    }

    q->nsent_this_epoch++;

    return ret;
}

/* FUNCTION triage_peek
 * ARGUMENTS: <q> points to a triage queue.
 * PRECONDITIONS: The queue is initialized with triage_create().
 * POSTCONDITIONS: If there is anything in the queue, returns the first 
 *      element, WITHOUT dequeueing it.  Otherwise, returns NULL.
 */
HeapTuple triage_peek(TriageQueue *q) {
    HeapTuple   ret;

    /* Check arguments. */
    Assert(is_initialized(q));

    /* SPECIAL CASE: Empty queue. */
    if (is_empty(q)) {
        /*
        elog(LOG, "triage_peek(): Queue at %p is empty (nelem is %d)",
                q, q->nelem);
                */
        return NULL;
    }

    /* END SPECIAL CASE */

    /*
    elog(LOG, "triage_peek(): Peeking at index %d.",
            q->next_dequeue_ix);
            */

    ret = q->buf[q->next_dequeue_ix];

    return ret;
}

/* FUNCTION triage_summaries_ready
 * POSTCONDITIONS: Returns <true> if the kept/dropped summaries for the queue
 *      contain all information about the current summary window.
 */
bool triage_summaries_ready(TriageQueue *q, Timestamp now) {
    Assert(is_initialized(q));

    CHECK_TS_ORDER(q,now);

    /* SPECIAL CASE: The global summary window start time is currently not set.
     */
    if (DT_NOBEGIN == *(q->epochstart_p)) {
        return false;
    }
    /* END SPECIAL CASE */

    /* The summaries are ready if any of the following are true:
     *  a) The first tuple in the queue is from a different summary window.
     *  b) The queue is empty, and the current time has passed the end of the
     *  summary window.
     */
    if (q->next_dequeue_ix == q->next_epoch_ix) {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, 
            "(%d/%s) Summaries done: next_dequeue = next_epoch",
            q->epochnum, now_cstr());
#endif /* DEBUG_TRIAGE_TIMING */
        return true;
    } else if (q->nelem == 0 
                && window_no(*(q->epochstart_p), &(q->epochlen), now) > q->epochnum)
    {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, 
            "(%d/%s) Summaries done: queue empty and time ran out",
            q->epochnum, now_cstr());
        elog(LOG, "    -> now = %s, epoch ends at %s", ts_cstr(now), 
            ts_cstr(simple_window_end(*(q->epochstart_p), &(q->epochlen), 
                    q->epochnum)));
#endif /* DEBUG_TRIAGE_TIMING */

        return true;
    } else {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "(%d/%s) Summaries NOT done (now is epoch %d)\n"
                  "    (current epoch ends at %s)", 
                q->epochnum, now_cstr(),
                window_no(*(q->epochstart_p), &(q->epochlen), now),
                ts_cstr(simple_window_end(*(q->epochstart_p),
                        &(q->epochlen), q->epochnum)));
#endif /* DEBUG_TRIAGE_TIMING */
        return false;
    }
}



/* FUNCTION triage_get_summaries
 * ARGUMENTS: <kept_p> and <dropped_p> point to the locations where the summary
 *      tuples will be stored.
 * PRECONDITIONS: The summaries are ready (e.g. triage_get_summaries_ready()
 *      returns true).
 * POSTCONDITIONS: Returns the current set of summaries (as tuples), then
 *      starts a new summary.
 */
void triage_get_summaries(TriageQueue *q, 
        SummaryTuples **kept_p, SummaryTuples **dropped_p) 
{
    Assert(is_initialized(q));

    Assert(q->next_dequeue_ix == q->next_epoch_ix || q->nelem == 0);

#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "    (%d) Advancing summaries", q->epochnum);
#endif /* DEBUG_TRIAGE_TIMING */

    /* Fetch the summaries and create fresh ones. */
    *kept_p = get_synopsis(q, q->syn_gen, &(q->keep_sum_state));

    *dropped_p = get_synopsis(q, q->syn_gen, &(q->drop_sum_state));

    /* Move our internal state to the next window. */
    q->epochnum++;
#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "        epochnum set to %d", q->epochnum);
#endif /* DEBUG_TRIAGE_TIMING */


    /* Advance the next_epoch pointer to the beginning of the new next epoch.
     */
    if (q->nelem == 0) {
        q->next_epoch_ix = TQ_NOT_AN_INDEX;
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "    Set next index to TQ_NOT_AN_INDEX (a)");
#endif /* DEBUG_TRIAGE_TIMING */
    } else {
        Assert(q->next_epoch_ix == q->next_dequeue_ix);

        while (q->next_epoch_ix != q->next_enqueue_ix
                && !simple_ts_past_window(
                        get_timestamp_val(q->input_desc, q->buf[q->next_epoch_ix]),
                        *(q->epochstart_p),
                        &(q->epochlen),
                        q->epochnum))
        {
            q->next_epoch_ix = next_index(q, q->next_epoch_ix);
        }

        if (q->next_epoch_ix == q->next_enqueue_ix) {
            /* We checked every tuple in the queue, and they're all from the
             * new current window. */
            q->next_epoch_ix = TQ_NOT_AN_INDEX;
#ifdef DEBUG_TRIAGE_TIMING
            elog(LOG, "    Set next index to TQ_NOT_AN_INDEX (b)");
#endif /* DEBUG_TRIAGE_TIMING */
        } else {
#ifdef DEBUG_TRIAGE_TIMING
            elog(LOG, "    Set next index to %d",
                    q->next_epoch_ix);
#endif /* DEBUG_TRIAGE_TIMING */
        }
    }
#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "        nelem = %d, enq = %d, deq = %d",
                    q->nelem, q->next_enqueue_ix, 
                    q->next_dequeue_ix);
#endif /* DEBUG_TRIAGE_TIMING */

    /* Reset the counter of number of tuples sent to the backend. */
    q->nsent_this_epoch = 0;

}

/* FUNCTION triage_get_sum_type
 * POSTCONDITIONS: Returns the type of summary this queue generates on calls to
 *      triage_get_*_synopsis().
 */
summary_type triage_get_sum_type(TriageQueue *q) {
    Assert(is_initialized(q));

    return getSummaryType(q->syn_gen);
}

/* FUNCTION triage_cleanup_expired_tuples
 * ARGUMENTS: <now> is a timestamp representing the current time, and
 *      <time_constraint> is an interval specifying how late a tuple is
 *      allowed to arrive for its window.
 * POSTCONDITIONS: Summarizes any tuples that are about to expire.  Enqueues
 *      the summaries if necessary.
 */
void triage_cleanup_expired_tuples(TriageQueue *q, Timestamp now)
{
    MemoryContext origcxt = NULL;

    Assert(is_initialized(q));
    CHECK_TS_ORDER(q,now);
    
    /* SPECIAL CASE: Empty queue. */
    if (TQS_NO_TS == q->state || triage_peek(q) == NULL) {
        return;
    }
    /* END SPECIAL CASE */

    
    origcxt = MemoryContextSwitchTo(wch_tmpcontext);
    { /* Begin temporary memory context */
        Timestamp win_end;
        Interval agg_time;
        /*Interval time_left;*/
        /*int ntups_could_arrive;*/
        int tup_cap; 
        int overflow;

        /* We're allowed to have thw following number of tuples in the queue 
         * at once:
         *
         * (W + D - Cshadow - t - agg_time) / Cfull - 1
         *
         *
         * Where:
         *      - W is the window size
         *      - D is the delay constaint
         *      - Cshadow is the per-summary cost of the shadow query
         *      - t is the current offset into the window
         *      - agg_time is the time we're 
         *
         * This is equivalent to:
         *       (D - Cshadow + (window_end - now) - agg_time) / Cfull - 1
         */
        win_end = simple_window_end(*(q->epochstart_p), &(q->epochlen), 
                                    q->epochnum);

        agg_time = i_mul(*AGG_TIME_PER_TUP, num_tups_sent_from_cur_epoch(q));

#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "At %s, summary window (%d) ends at %s",
                ts_cstr(now), q->epochnum, ts_cstr(win_end));
#endif /* DEBUG_TRIAGE_TIMING */

#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "    Delay constraint is %s", 
                interval_cstr(*(q->time_cstr_p)));
#endif /* DEBUG_TRIAGE_TIMING */


        /* Here we evaluate:
         * divide(
         *      subtract(
         *          add(subtract(D,Cshadow),subtract(window_end,now)),
         *          agg_time
         *      ,Cfull) - 1
         */
        tup_cap = (int)
            i_div_i(
                i_mi_i(
                    i_pl_i(
                        i_mi_i(*(q->time_cstr_p),*SHADOW_TIME),
                        ts_mi(win_end, now)
                    ), 
                    agg_time
                ),
                *FULL_TIME_PER_TUP
            ) - 1;

#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "    Effective queue size of %d tuples" tup_cap);
#endif /* DEBUG_TRIAGE_TIMING */

        overflow = num_tups_from_cur_epoch(q) - tup_cap;

        /* Double the number of tuples we drop, to compensate for the delay in
         * this function getting called. */
        overflow *= 2;

        if (overflow > num_tups_from_cur_epoch(q)) {
            /* We can't triage tuples from subsequent summary windows until
             * we've sent the summaries for this window. */
            overflow = num_tups_from_cur_epoch(q);
        }

        /* If we have more tuples than we can process, get rid of the excess.
         */
        if (overflow > 0) {
            int ndropped;

#ifdef DEBUG_TRIAGE_TIMING
            elog(LOG, "    Triaging %d expired tuples at %s", overflow,
                    ts_cstr(now));
#endif /* DEBUG_TRIAGE_TIMING */

            /* drop_oldest() may want to allocate long-lived data 
             * structures, so switch to a long-lived context. */
            MemoryContextSwitchTo(origcxt);
            for (ndropped = 0; ndropped < overflow; ndropped++) { 
                /* SPECIAL CASE: Never drop a punctuation tuple. */
                if (HeapTupleIsPunct(triage_peek(q))) {
                    break;
                }
                /* END SPECIAL CASE */
                drop_oldest(q);
            }
            
            /* SPECIAL CASE: If we just blew away the entire window,
             * and we've got something from a later window,
             * put a punctuation tuple at the front. */
            if (0 == num_tups_from_cur_epoch(q) && q->nelem > 0) {

                q->next_dequeue_ix = prev_index(q, q->next_dequeue_ix);

                /* It's safe to put a tuple here, because we wouldn't have
                 * dropped any if the current epoch was empty. */ 
                q->buf[q->next_dequeue_ix]
                    = form_punct_tup_no_leaks(q->input_desc,
                        win_end - EPSILON, wch_tmpcontext);
                        /* HACK ALERT: We subtract a tiny, tiny bit from the
                         * timestamp to keep it in the current window.
                         */
                q->nelem++; 
            }
            /* END SPECIAL CASE */

            MemoryContextSwitchTo(wch_tmpcontext);

        } else {
#ifdef DEBUG_TRIAGE_TIMING
            elog(LOG, "    Not triaging; %d tuples in queue,"
                    " time for %d more tuples", 
                    num_tups_from_cur_epoch(q),
                    (-overflow));
#endif /* DEBUG_TRIAGE_TIMING */
        }
    } /* End temporary memory context. */
    MemoryContextSwitchTo(origcxt);
}

Timestamp get_timestamp_val(TupleDesc desc, HeapTuple tup)
{
    bool is_null;
    Datum ret;

    /* Make sure this tuple has a timestamp column... */
    Assert(tc_has_timestamp(desc->constr));
    
    ret = heap_getattr(tup, get_timestamp_col(desc), desc, &is_null);
    Assert(false == is_null);

    return DatumGetTimestamp(ret);
}

Timestamp get_timestamp_val_IHT(IntermediateHeapTuple iht, int sourceid) 
{
	HeapTuple ht;

	Assert(iht != NULL);
    ht = iht->t_base_tuples[sourceid];
    Assert(NULL != ht);

	return get_timestamp_val(iht->t_base_tuple_descs[sourceid], ht);
}

/* FUNCTION getSummarizer
 * ARGUMENTS: <gen> is a factory object for summary generators.
 *      <input_desc> describes the tuples that the new object will summarize
 *      <output_desc> describes the tuples that the object will generate.
 * PRECONDITIONS: <gen> has been properly initialized.
 * POSTCONDITIONS: Returns a new summarizer object, the type of which depends
 * on the parameters inside <gen>.
 * Caller is responsible for freeing the summarizer object.
 */
generic_stream_state *getSummarizer(StreamSynopsisGen *gen, 
        TupleDesc input_desc, TupleDesc output_desc) {
    generic_stream_state *ret;
    Assert(NULL != gen);
    
    /* Turn the prototype into an active summarizer. */
    ret = generic_stream_init(&(gen->protostate), input_desc, output_desc);

    return ret;
}

/* FUNCTION getSummaryType
 * ARGUMENTS: <gen> is a factory object for summary generators.
 * PRECONDITIONS: <gen>->proto has been properly initialized.
 * POSTCONDITIONS: Returns the type of summary producer that <gen> will
 * produce.
 */
summary_type getSummaryType(StreamSynopsisGen *gen) {
    Assert(NULL != gen);

    return gen->protostate.stype;
}

/* FUNCTION summaryTuplesGetNext
 * ARGUMENTS: <st> is an object holding the information necessary to
 * build the tuples of a summary for a window.
 * PRECONDITIONS: You have not previously called SummaryTupleGetNext() on 
 *      this SummaryTuples object.
 * POSTCONDITIONS: Returns the next tuple in the collection, until it reaches
 * the end of the collection; then returns NULL.
 */
HeapTuple summaryTuplesGetNext(SummaryTuples *st) {
    HeapTuple ret;

    Assert (NULL != st);

    switch (st->summary->stype) {
        case    ST_MHIST:
        case    ST_SQUARE_MHIST:
        case    ST_WAVELET:
        {
            Datum values[4];
                /* Box for passing to heap_formtuple() */
            char nulls[4] = "    ";
                /* Indicates that none of the values are null. */
 
            /* These summaries produce a single tuple. */
            if (st->tuples_produced >= 1) return NULL;

            /* These guys are passed to heap_formtuple() as pointers. */
            values[0] = PointerGetDatum((void*)(st->summary->summary.m));
            values[1] = st->epochnum;
            values[2] = TimestampGetDatum(st->epochstart);
            values[3] = TimestampGetDatum(st->epochend);
            ret = heap_formtuple(st->summary_tdesc, values, nulls);
        }
            break;

        case ST_COUNT:
        {
            /* Single tuple, by value. */
            Datum values[4];
            char nulls[4] = "    ";
            if (st->tuples_produced >= 1) return NULL;
            values[0] = Int32GetDatum(st->summary->summary.c);
            values[1] = st->epochnum;
            values[2] = TimestampGetDatum(st->epochstart);
            values[3] = TimestampGetDatum(st->epochend);
            ret = heap_formtuple(st->summary_tdesc, values, nulls);
        }
            break;

        case ST_SAMPLE:
        {
            /* Multiple tuples, which are already constructed. */
            reservoir_sample *samp = st->summary->summary.s;

            Assert(samp->cur_sample_itr >= 0);

            if (samp->cur_sample_itr >= samp->cur_sample_size) {
                /* We send a single punctuation tuple after we've sent the rest
                 * of the tuples. */
                if (st->tuples_produced == samp->cur_sample_size) {
                    ret = form_punct_tup(st->summary_tdesc, st->epochend);
                } else {
                    return NULL;
                }
            } else {
                ret = samp->reservoir[samp->cur_sample_itr];
                (samp->cur_sample_itr)++;
            }
        }
            break;


        default: 
            elog(ERROR, 
                "summaryTuplesGetNext(): Don't know about summary type %d",
                    st->summary->stype);

    }

    st->tuples_produced++;
    return ret;
}

/* FUNCTION summaryTuplesDestroy
 * ARGUMENTS: <st> is an object that holds the information for constructing
 *      summary tuples.
 * POSTCONDITIONS: Frees the object and any unneeded stuff that it points to.
 */
void summaryTuplesDestroy(SummaryTuples *st) {
    switch (st->summary->stype) {
        case    ST_MHIST:
            pfree(st->summary->summary.m);
            break;

        case    ST_SQUARE_MHIST:
            pfree(st->summary->summary.f);
            break;

        case    ST_WAVELET:
            pfree(st->summary->summary.w);
            break;
            
        case ST_COUNT:
            /* Nothing to free */
            break;

        case ST_SAMPLE:
            sample_destroy(st->summary->summary.s);
            break;


        default: 
            elog(ERROR, 
                "summaryTuplesDestroy(): Don't know about summary type %d",
                    st->summary->stype);

    }

    pfree(st->summary);
    st->summary = NULL;

    /* Now that we've freed everything the struct points to, we can get rid of
     * the struct. */
    pfree(st);
}

/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/

/* Do some sanity checks on a queue. */
bool is_initialized(TriageQueue *q) {
    if (q == NULL) {
        return false;
    }

    if (q->state == TQS_NOT_INITIALIZED) {
        return false;
    }

    if (q->buf == NULL) {
        return false;
    }


    /* Passed the checks. */
    return true;
}

bool is_full(TriageQueue *q) {
    return (q->nelem == q->qlen);
}


bool is_empty(TriageQueue *q) {
    return (q->nelem == 0);
}

/* Calculate the next index in the circular buffer. */
int next_index(TriageQueue *q, int prev) {
    return ((prev + 1) % (q->qlen));
}

int prev_index(TriageQueue *q, int next) {
    return ((next - 1) % (q->qlen));
}
/* Calculate the number of queue entries between <first> and <last>, inclusive
 * of <first> but not <last>.
 * */
int index_diff(TriageQueue *q, int first, int last) {
    if (last >= first) {
        return last - first;
    }
    
    /* If we get here, then the queue wraps around between last and first. */
    return (q->qlen - first) + last; 
}

/* Returns the number of tuples from the current epoch that we have sent to the
 * backend. */
int num_tups_sent_from_cur_epoch(TriageQueue *q) {
    return q->nsent_this_epoch;
}

/* Returns number of tuples from the current epoch that are in the queue. */
int num_tups_from_cur_epoch(TriageQueue *q) {
    if (TQ_NOT_AN_INDEX == q->next_epoch_ix) {
        /* Every tuple is from the current epoch. */
        return q->nelem;
    } else {
        return index_diff(q, q->next_dequeue_ix, q->next_epoch_ix);
    }
}

/* Function that returns all the data you need to construct a summary
 * for a section of a stream. */
/* stream_td here is the descriptor for the _input_ stream, not the synopsis
 * tuples! */
SummaryTuples* get_synopsis( TriageQueue *q,
        StreamSynopsisGen *gen, generic_stream_state **state_ptr_ptr) {
    SummaryTuples   *ret = palloc(sizeof(*ret));
        /* Freed by caller in summaryTuplesDestroy() */

    ret->tuples_produced = 0;
    ret->epochnum = q->epochnum;
    ret->epochstart = simple_window_end(*(q->epochstart_p), &(q->epochlen), 
                            q->epochnum - 1);
    ret->epochend = simple_window_end(*(q->epochstart_p), &(q->epochlen), 
                            q->epochnum);

    /* This call should make copies of the timestamps. */
    ret->summary_tdesc = (*state_ptr_ptr)->output_desc;

    ret->summary = generic_stream_done(*state_ptr_ptr);
        /* The call to generic_stream_done() also frees the summarizer*/

    /* Start a new synopsis going. */
    *state_ptr_ptr = getSummarizer(gen, q->input_desc, q->summary_desc);

    return ret;

}

/* Accessors for the timestamp expression. */
/* <get_newer> is TRUE to fetch the "newer" timestamp and FALSE to fetch the
 * "older" timestamp. */
Const *get_ts_ptr(Expr *windowexpr, bool get_newer) 
{
    Expr *lhs = NULL;

    /* The expression is in the format:
     *  (newer_timestamp - older_timestamp) > window_size 
     *
     * Get the left-hand side of the expression.
     */
    lhs = (Expr*)lfirst(windowexpr->args);

    /* Then fetch a pointer to the appropriate Const. */
    if (get_newer) {
        return (Const*)(lfirst(lhs->args)); 
    } else {
        return (Const*)(lsecond(lhs->args));
    }
}


void copy_timestamp_val(TupleDesc desc, HeapTuple tup, Const *dest) 
{
    Datum   ts;
    /*char    *debug_str;*/

    ts = get_timestamp_val(desc, tup);
    
    /* ts here is actually a pointer to a temporary buffer.  Make a copy of the
     * data in that buffer. */
    memcpy((void*)(dest->constvalue), (void*)(ts), sizeof(Timestamp));

}

/* Function to create a window expression.
 *
 * The expression evaluates whether:
 *  (newer_timestamp - older_timestamp) > window_size 
 *
 * <windowsize> is a constant of type "timestamp".
 */
Expr *create_window_expr(Const *windowsize) {
    Const *old_ts_const = NULL, *new_ts_const = NULL;
    Expr *lhs = NULL, *rhs = NULL;
    Expr *ret = NULL;
    Timestamp *new_ts = NULL, *old_ts = NULL;

    /* The window expression is just:
     *  (newer_timestamp - older_timestamp) > window_size 
     */

    /* First we build the left hand side of the expression, 
     * (newer_timestamp - older_timestamp) */

    /* Timestamps are passed by reference, but we get a pointer to a temporary
     * buffer when we call heap_getattr(), so we need to make a local copy of
     * the value in said buffer.  Create space for those local copies here. */
    new_ts = palloc(sizeof(*new_ts));
    old_ts = palloc(sizeof(*old_ts));

    memset(new_ts, 0x0, sizeof(*new_ts));
    memset(old_ts, 0x0, sizeof(*old_ts));

    /* This requires Const nodes for the operands... */
    new_ts_const = makeConst(TIMESTAMPOID,
                        sizeof(Timestamp),
                        (Datum)(new_ts),
                        false /* Not null */,
                        false /* Not by value */,
                        false /* Not a set-valued attribute. */,
                        false /* Not a cast. */);
    old_ts_const = makeConst(TIMESTAMPOID,
                        sizeof(Timestamp),
                        (Datum)(old_ts),
                        false /* Not null */,
                        false /* Not by value */,
                        false /* Not a set-valued attribute. */,
                        false /* Not a cast. */);

	lhs = make_op(makeList1(makeString("-")), 
			(Node *) new_ts_const, (Node *) old_ts_const);

    replace_opid((Oper *) (lhs)->oper);
	/* Not sure what this call to replace_opid() does. */

    rhs = (Expr*)windowsize;

    ret = make_op(makeList1(makeString(">")), (Node *)lhs, (Node *)rhs);
	replace_opid((Oper *) ((Expr *)ret)->oper);


    return ret;
}

/* FUNCTION synopsis_insertion_delay
 * POSTCONDITIONS: Spins for the amount of time indicated by g_synopsis_delay.
 */
extern int g_synopsis_delay; /* In wrapch.c */
void synopsis_insertion_delay() {
    int i, j;
    for (i = 0; i < g_synopsis_delay; i++) {
        for (j = 0; j < 10000; j++);
    }
}



/* FUNCTION drop_rest_of_summary_window
 * POSTCONDITIONS: Triages all the tuples remaining in the current summary
 * window.
 *
 * Returns the number of tuples triaged.
 */
int drop_rest_of_summary_window(TriageQueue *q) {
    int ix;
    int ntriaged = 0;

#ifdef DEBUG_TRIAGE_TIMING
    elog(LOG, "    Dropping -- count = %d, next = %d, next_epoch = %d",
            q->nelem, q->next_dequeue_ix, q->next_epoch_ix);
#endif /* DEBUG_TRIAGE_TIMING */

    for (ix = q->next_dequeue_ix; 
            ix != q->next_enqueue_ix && ix != q->next_epoch_ix;
            ix = q->next_dequeue_ix)
    {
#ifdef DEBUG_TRIAGE_TIMING
        elog(LOG, "        Dropping tuple at index %d", ix);
#endif /* DEBUG_TRIAGE_TIMING */
        drop_oldest(q);
        ntriaged++;
    }

    return ntriaged;
}

/* FUNCTION drop_oldest
 * PRECONDITIONS: The oldest tuple is from the same summary window as the
 * current 'drop' summary.
 * POSTCONDITIONS: Drops the oldest (e.g. first) tuple in the triage queue.  
 * Summarizes it, of course.
 */
void drop_oldest(TriageQueue *q) {
    HeapTuple oldest;

    Assert(is_initialized(q));

    oldest = triage_peek(q);

    Assert(NULL != oldest);

    /* The tuple we drop had better be going to a summary for an epoch that
     * contains the tuple's timestamp! */
    if (window_no(*(q->epochstart_p), &(q->epochlen), 
            get_timestamp_val(q->input_desc, oldest)) != q->epochnum)
    {
        elog(ERROR, "Tried to drop a tuple from the wrong summary window\n"
                "    (current summary window is %d; tuple's is %d)",
                q->epochnum, window_no(*(q->epochstart_p), &(q->epochlen), 
            get_timestamp_val(q->input_desc, oldest)));
    }


    /* Summarize the tuple */
    generic_stream_next(q->drop_sum_state, oldest);

    /* We no longer own this tuple, having passed it to the summarizer, so
     * there is no need to free the tuple. */
    q->next_dequeue_ix = next_index(q, q->next_dequeue_ix);
    q->nelem--;
}

/* Function that fetches the current timestamp as a string.  Leaks
 * memory, so it's best to only use it for debugging. */
char * now_cstr() {
    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out,
                DirectFunctionCall1(text_timestamp,
                    DirectFunctionCall1(timeofday,PointerGetDatum(NULL))))); 

}

/* Convenience functions for window math when RANGE BY == SLIDE BY.
 */
int window_no(Timestamp epochstart, Interval *epochlen, Timestamp ts) {
    MemoryContext origcxt = MemoryContextSwitchTo(wch_tmpcontext);
    int ret = last_window_no(epochstart, epochlen, epochlen, ts);
    MemoryContextSwitchTo(origcxt);
    return ret;
}

Timestamp simple_window_end(Timestamp epochstart, Interval *epochlen, 
        int epochnum)
{
    MemoryContext origcxt = MemoryContextSwitchTo(wch_tmpcontext);
    Timestamp ret = window_end(epochstart, epochlen, epochlen, epochnum);
    MemoryContextSwitchTo(origcxt);
    return ret;
}


bool simple_ts_past_window(Timestamp ts, Timestamp epochstart,
        Interval *epochlen, int epochnum) 
{
    MemoryContext origcxt = MemoryContextSwitchTo(wch_tmpcontext);
    int ret = timestamp_past_window(ts, epochstart, epochlen, epochlen, 
            epochnum);
    MemoryContextSwitchTo(origcxt);
    return ret;
}

