/*-------------------------------------------------------------------------
 *
 * triageq.h
 *
 * FILENAME: triageq.h
 * AUTHOR: Fred Reiss
 * DESCRIPTION: Function prototypes and data structures for the Triage Queue --
 *      part of TelegraphCQ's load-shedding implementation.
 *
 * Copyright (c) 2004,2005, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/triageq.h,v 1.13 2005/07/26 20:00:31 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TRIAGEQ_H
#define TRIAGEQ_H

#include "postgres.h"
    /* Basic Postgres includes. */
    

#include "access/htup.h"
#include "access/heapam.h"
    /* Heap access method and heap tuples. */
#include "executor/intermediateTuple.h"
#include "telegraphcq/shedding.h"
    /* generic_stream_state data structure */

/*******************************************************************************
 * DATA STRUCTURES
 ******************************************************************************/

/* The types of summary we currently support. */
/*
typedef enum SummaryType {
	SUMT_COUNTS,
	SUMT_REGHIST,
	SUMT_MHIST,
	SUMT_WAVELET,
    SUMT_SAMPLE
} SummaryType;
*/


/* A generic factory for objects that build the summary data structure du
 * jour. */
typedef struct StreamSynopsisGen
{
    /*SummaryType sumtype;*/
        /* What type of summary does this generator generate? */

    generic_stream_state protostate;
        /* Prototype object that holds the state of the synopsizing. 
         * This object holds any parameters that need to be passed to the
         * synopsis data structure. */

    /*TupleDesc synopsis_tuple_desc;*/
        /* A tuple descriptor that describes a tuple containing one column.
         * The value of this column is a synopsis datatype. */

    /*generic_stream_state *(*initf)(void *, const TupleDesc);*/
        /* Function that creates a new state object to represent a stream. */
        /* First argument is a pointer to <protostate>, and the second argument
         * points to a descriptor of the tuples we will be summarizing. */
        /* The function should make its own copies of the second argument. */

    /*void (*nextf)(void *, HeapTuple);*/
        /* Function that incorporates the next tuple into the synopsis. */
        /* First argument is a pointer to the state object created by <initf>, 
         * and second argument is the next tuple. */
        /* This function is responsible for freeing the tuple. */

    /*void *(*donef)(void*);*/
        /* Function that finalizes the synopsis and frees the state object. */
        /* Argument is a pointer to the state object. */
        /* Returns a pointer to the synopsis, in varlena (variable-length
         * array) format. */
} StreamSynopsisGen;

/* Object that holds the information necessary to construct the HeapTuple(s)
 * of a summary. */
typedef struct SummaryTuples
{
    int tuples_produced;
    int epochnum;
    Timestamp   epochstart;
    Timestamp   epochend;
    TupleDesc   summary_tdesc;
    
    generic_summary *summary;
     
} SummaryTuples;

/* generate-object function of the above factory */
extern generic_stream_state *getSummarizer(StreamSynopsisGen *gen,
        TupleDesc input_desc, TupleDesc output_desc);
extern summary_type getSummaryType(StreamSynopsisGen *gen);
/* TODO: Make a constructor for StreamSynopsisGen */

/* A stub data structure for drop-only load shedding. */
typedef int32 CountSynopsis;


enum TriageQueueState {
    TQS_NOT_INITIALIZED = 0,
    TQS_NO_TS,
        /* Queue is initialized, but we haven't enqueued anything. */
    TQS_HAVE_TS
        /* We've enqueued at least one tuple, so the window expression is
         * usable. */
};

#define TQ_NOT_AN_INDEX -1


typedef struct TriageQueue
{
    enum TriageQueueState    state;

    HeapTuple           *buf;
        /* Internal implementation: A circular buffer of pointers.  
         * We'll probably use something different when we get into 
         * exotic triage schemes. */

    TupleDesc           input_desc;
        /* Tuple descriptor for the tuples in <buf>.  Used for constructing
         * summaries of these tuples. */

    TupleDesc           summary_desc;
        /* Tuple descriptor for the tuples in the summary streams.  Used in
         * constructing the summaries that go into those streams. */ 

    int                 qlen;
        /* Length in number of elements. */

    int                 nelem;
        /* How many elements are currently in the queue. */

    int                 next_enqueue_ix;
    int                 next_dequeue_ix;
        /* Indexes into *buf */

    StreamSynopsisGen   *syn_gen;
        /* The current synopsis generator. */

    generic_stream_state *drop_sum_state;
    generic_stream_state *keep_sum_state;
        /* We keep two summaries: One for dropped tuples, and one for
         * non-dropped tuples.  These pointers point not to the synopses
         * themselves, but to the objects that keept track of the state of
         * creating those synopses. */

    Timestamp           *epochstart_p;
        /* The point in time at which the summary window starts.  Usually tied
         * to the time at which the first tuple entered the system. */

    Interval            epochlen;
        /* The length of each summary window. */

    int                 epochnum;
        /* The number of the current epoch.  Meaningless until the triage queue
         * has received its first tuple. */

    int                 next_epoch_ix;
        /* Index of the first tuple from the next epoch, or TQ_NOT_AN_INDEX
         * if every tuple in the triage queue is from the current epoch. */

    int                 nsent_this_epoch;
        /* The number of tuples that have been dequeued this epoch; used to
         * calculate the aggregation overhead remaining for this round. */

    Interval            *time_cstr_p;
        /* The amount by which we are allowed to be late in delivering a tuple
         * or its summary to the query processor.
         * The target of this pointer is not owned by the triage queue. */

    Timestamp           __DEBUG_most_recent_ts;
        /* The most recent timestamp this triage queue has seen.
         * ONLY used when DEBUG_TRIAGE_TIMING is enabled in triageq.c. */

} TriageQueue;


/*******************************************************************************
 * PROTOTYPES OF EXTERNAL FUNCTIONS
 *
 * See triageq.c for function comments.
 ******************************************************************************/
extern TriageQueue *triage_create(int qlen, StreamSynopsisGen *gen, 
        const TupleDesc input_desc, const TupleDesc summary_desc,
        Timestamp *epochstart_p, const Interval *windowsize, 
        const Interval *time_constr);
extern void triage_destroy(TriageQueue *q);
extern int triage_size(TriageQueue *q);
extern void triage_enqueue(TriageQueue *q, HeapTuple tup);
/*void triage_just_summarize(TriageQueue *q, HeapTuple tup);*/
extern HeapTuple triage_dequeue(TriageQueue *q);
extern HeapTuple triage_peek(TriageQueue *q);
extern bool triage_summaries_ready(TriageQueue *q, Timestamp now);
extern void triage_get_summaries(TriageQueue *q, 
        SummaryTuples **kept_p, SummaryTuples **dropped_p);
extern summary_type triage_get_sum_type(TriageQueue *q);
extern void triage_cleanup_expired_tuples(TriageQueue *q, Timestamp now);
extern TupleDesc triage_get_tup_desc(TriageQueue *q);

/* Exported utility functions */
extern Timestamp get_timestamp_val(TupleDesc desc, HeapTuple tup);
extern Timestamp get_timestamp_val_IHT(IntermediateHeapTuple iht, int sourceid);

/* Accessors for the SummaryTuples objects returned by triage_get_summaries()
 */
extern HeapTuple summaryTuplesGetNext(SummaryTuples *st);
extern void summaryTuplesDestroy(SummaryTuples *st);


#endif /* TRIAGEQ_H */

