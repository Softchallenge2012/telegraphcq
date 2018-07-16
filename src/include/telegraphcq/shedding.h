/*
 * $RCSfile: shedding.h,v $
 * $Revision: 1.4 $
 * $Date: 2005/05/04 02:28:40 $
 */

#include "telegraphcq/histogram.h"
#include "telegraphcq/wavelet.h"

#ifndef SHEDDING_H
#define SHEDDING_H

/* A struct containing all the information necessary to construct a tuple,
 * except for a TupleDesciptor. */
typedef struct Prototuple {
    Datum *fields;
        /* An array of values for the fields of the tuple. */
    char *nulls;

    bool isPunct;

    int nfields_used;
    int nfields_alloc;
        /* Length of the two arrays and how much of that length is for the
         * actual tuple, as opposed to extra space (for additional columns) */

} Prototuple;

extern Prototuple *createPrototuple(int nfields, int nextra,
        const Datum *fields, const char *nulls, bool isPunct);
extern Prototuple *heaptupleToPrototuple(HeapTuple tup, TupleDesc desc, 
        int nextra);
extern HeapTuple heaptupleFromPrototuple(Prototuple *pt, TupleDesc desc);
Timestamp timestampFromPrototuple(Prototuple *pt, TupleDesc desc);
extern void destroyPrototuple(Prototuple *pt);


/* A generic buffer of tuples for building a synopsis. */
typedef struct TupBuf {
    HeapTuple *buf;
        /* A buffer of tuples for determining bucket boundaries.  Note that
         * HeapTuple is typedef'd to a pointer. */

    int count;

    /* 
     * The following two members are read-only constants.
     */
    int cap;
        /* How many tuples in *buf, and how many the buffer can hold,
         * respectively. */

    bool own_tuples;
        /* TRUE if the tuple buffer is responsible for freeing the tuples. */
} TupBuf;

/* Substructures of generic_stream_state: */
typedef struct mhist_stream_state {
    mhist *hist;
    int num_buckets;
        /* How many buckets the histogram will have; used when the histogram
         * isn't allocated yet. */
} mhist_stream_state;

/* Struct to hold a reservoir sample */
typedef struct reservoir_sample {

    int max_sample_size;
        /* Maximum number of tuples that can be in the sample */
    
    int cur_sample_size;
        /* The true current size of our sample */

    int tot_size;
        /* The total number of tuples represented by the sample.  Used to
         * compute the __samplefrac field of sample tuples. */

    HeapTuple *reservoir;
        /* Pointer to an array of tuples of length sample_size */

    int cur_sample_itr;
        /* Our current position in the cur_sample array when we are reading
         * the tuples out of the sample, or -1 otherwise. */


} reservoir_sample;

/* Struct to hold the status of the factory that is producing reservoir 
 * samples for Data Triage. */
typedef struct sample_stream_state {
    reservoir_sample *sample;

    int max_sample_size;
        /* Maximum number of tuples that can be in the sample */

    int sampmult_att_0based;
        /* 0-based index of the attribute containing the __samplefrac field in
         * our output tuples. */
} sample_stream_state;


typedef struct count_stream_state {
    int count;
} count_stream_state;

/* The state of building an MHIST from a stream of tuples. */
typedef enum {
    MH_INIT = 0,
        /* Haven't allocated the histogram yet, because we don't know how many
         * columns to use. */
    MH_LOADING_BOUNDS,
        /* Loading the set of tuples that will be used to determine bucket
         * boundaries. */
    MH_LOADING_COUNTS,
        /* Done determining bucket boundaries; scanning through tuples to
         * compute bucket counts. */ 
    MH_COMPLETE
        /* Histogram complete. */
} mhist_status;

/* The types of summary we allow. */
typedef enum {
    ST_MHIST,
    ST_SQUARE_MHIST,
    ST_WAVELET,
    ST_SAMPLE,
    ST_COUNT
} summary_type;


/* A generic container for a summary. */
typedef struct generic_summary {
    summary_type stype;

    union {
        mhist               *m;     /* MHIST */
        mhist               *f;     /* FGHist */
        reservoir_sample    *s;     /* Sample */
        WCS                 *w;     /* Wavelet-based histogram */
        int                 c;     /* Count; stored unboxed! */
    } summary;
} generic_summary;

/* Information that we need to build any synopsis. */
typedef struct generic_stream_state {
    mhist_status state;
        /* What state is this histogram in? */

    TupBuf tb;
        /* A buffer of tuples for determining bucket boundaries. */


    TupleDesc input_desc;
        /* Descriptor for the tuples we will stick into the histogram. */

    TupleDesc output_desc;
        /* Descriptor for the tuples we will generate. */

    summary_type stype;

    union {
        mhist_stream_state mhs;    
        square_mhist_stream_state shs;
        wcs_stream_state ws;
        sample_stream_state ss;
        count_stream_state cs;
    } ds_state;
    /* State specific to the current data structure. */
} generic_stream_state;

/* Functions for creating summary factories with StreamSynopsisGen. */
extern generic_stream_state *generic_stream_init(generic_stream_state * proto, 
        TupleDesc input_desc, TupleDesc output_desc);
extern void generic_stream_next(generic_stream_state *sp, HeapTuple tup);
extern generic_summary *generic_stream_done(generic_stream_state *sp);


extern void sample_destroy(reservoir_sample *samp);


/*******************************************************************************
 * PROFILING
 ******************************************************************************/
extern int prof_enter_function(char *);
extern int prof_exit_function(char *);

extern Datum test_sampling_speed(PG_FUNCTION_ARGS);

#ifndef MHIST_PROFILING

#define PROF_ENTER_FUNCTION(name) 
#define PROF_EXIT_FUNCTION(name) 

#else /* ! MHIST_PROFILING */

#define PROF_ENTER_FUNCTION(name) prof_enter_function(name)
#define PROF_EXIT_FUNCTION(name) prof_exit_function(name)


#endif /* MHIST_PROFILING */

#endif /* SHEDDING_H */


