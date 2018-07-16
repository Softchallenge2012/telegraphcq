// TODO trim headers
#include <ctype.h>
#include <sys/time.h>

    /* For toupper(3) */

#include "postgres.h"

#include "access/hash.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "lib/stringinfo.h"
#include "utils/builtins.h"
#include "utils/elog.h"
#include "utils/relcache.h"

#include "utils/lsyscache.h"
    /* For get_typlenbyval() function. */

#include "telegraphcq/punct.h"

#include "telegraphcq/wavelet.h"
#include "telegraphcq/histogram.h"
#include "telegraphcq/shedding.h"
#include "telegraphcq/window.h"

bool stream_state_is_valid(generic_stream_state *state);

void tupbuf_init(TupBuf *tbp);

void tupbuf_clear(TupBuf *tbp);

void tupbuf_destroy(TupBuf *tbp);

bool tupbuf_insert(TupBuf *tbp, HeapTuple tup);

void generic_create_ds(generic_stream_state *state);

void generic_load_ds(generic_stream_state *state);

reservoir_sample * sample_create(int max_size);

void sample_load(HeapTuple *tups, int tup_count, reservoir_sample *samp);

int sampmult_att(TupleDesc desc);

void sample_prepare_for_output(reservoir_sample *samp, 
        TupleDesc input_desc, TupleDesc output_desc, int sampmult_attr);

double really_test_sampling_speed(int sample_size, int numtups);

/*******************************************************************************
 * PROFILING
 ******************************************************************************/
int prof_enter_function(char *str) {
    struct timeval  tv;
    gettimeofday(&tv, 0);
    elog(LOG, "YANG START %s %ld.%06ld", str, (long int)tv.tv_sec, 
            (long int)tv.tv_usec);
    return 0;
}

int prof_exit_function(char *str) {
    struct timeval  tv;
    gettimeofday(&tv, 0);
    elog(LOG, "YANG END %s %ld.%06ld", str, (long int)tv.tv_sec, 
            (long int)tv.tv_usec);
    return 0;
}

/* FUNCTION test_sampling_speed
 * ARGUMENTS: <sample_size> determines how large a sample to run.
 * POSTCONDITIONS: Calibrates itself and runs a benchmark, then returns the 
 * number of sec. per tuple as a double.
 *
 * NOTE: To use this function, add the following to your psql input file:
 * 
 * create or replace function test_sampling_speed(integer)
 * returns float8
 * as 'test_sampling_speed'
 * language 'internal' volatile;
 *
 * To run it, do:
 * select test_sampling_speed(sample_size);
 *
 */
Datum test_sampling_speed(PG_FUNCTION_ARGS) {
    double result;
    int sample_size = PG_GETARG_INT32(0);
    int numtups = 1000;
    double runtime_sec = 0.0;

    /* First figure out the number of tuples to run.  We want the benchmark to
     * run for at least a second to get good timings. */
    do {
        numtups *= 2;
        runtime_sec = really_test_sampling_speed(sample_size, numtups);
        /*elog(NOTICE, "%d tuples takes %f seconds", numtups, runtime_sec);*/
    } while (runtime_sec < 1.0 && (double)numtups < 1e7);

    /* Take our last runtime and compute the number of seconds per tuple. */
    result = runtime_sec / numtups;

    PG_RETURN_FLOAT8(result);
}



/* FUNCTION stream_state_is_valid
 * POSTCONDITIONS: Runs some sanity checks and returns TRUE if the state object
 *      passed to us is valid.
 */             
bool stream_state_is_valid(generic_stream_state *state) {
    if (NULL == state) {
        return false;
    }

#ifdef ENABLE_STREAM_STATE_CHECKS
    switch (state->stype) {
        case ST_MHIST:
        {
            mhist_stream_state *mhs = &(state->ds_state.mhs);

            if (mhs->num_buckets <= 0) return false;
            if (state->tb.cap < mhs->num_buckets) return false;

            if (NULL == mhs->hist) {
                /* TODO: Make sure that we're in a state that allows 
                 * unitialized histogram pointers. */
            } else {
                check_mhist_integrity(mhs->hist, 
                                    "stream_state_is_valid()");
            }
        }
            break;
        case ST_SQUARE_MHIST:
        {
            square_mhist_stream_state *shs = &(state->ds_state.shs);

            if (shs->bucket_width <= 0) return false;

            /* TODO: Add additional sanity checks here. */
        }
            break;

        case ST_WAVELET:
        {
            /*wcs_stream_state *ws = &(state->ds_state.ws);*/
            /* TODO: Add checks. */
        }
            break;

        case ST_SAMPLE:
        {
            /*sample_stream_state *ss = &(state->ds_state.ss);*/

            /* TODO: Add checks. */
        }
            break;

        case ST_COUNT:
        {
            if (state->ds.state.cs.count < 0) return false;
        }
            break;

        default:
            return false;
            break;
    }

#endif /* ENABLE_STREAM_STATE_CHECKS */

    return true;
}

/* Support function.  Loads the current set of tuples into the current data
 * structure. */
void generic_load_ds(generic_stream_state *state) {
    switch (state->stype) {
        case ST_MHIST:
            mhist_load_buckets(state->tb.buf, state->input_desc, 
                state->tb.count, state->ds_state.mhs.hist);
            break;
        case ST_SQUARE_MHIST:
            mhist_load_buckets(state->tb.buf, state->input_desc, 
                state->tb.count, state->ds_state.shs.hist);
            /*
            elog(LOG, "generic_load_ds(): After loading, hist is '%s'",
                    mhist_to_str(state->ds_state.shs.hist));
                    */
            break;
        case ST_WAVELET:
            {
                WCS* wcs = wcs_create_stream(state, TRUE);
                state->ds_state.ws.wcs = 
                    real_wcs_union(wcs, state->ds_state.ws.wcs);
                break;
            }

        case ST_SAMPLE:
            sample_load(state->tb.buf, state->tb.count, 
                    state->ds_state.ss.sample);
            break;

        case ST_COUNT:
            state->ds_state.cs.count += state->tb.count;
            break;

        default:
            elog(ERROR, "generic_load_ds(): Invalid summary type.");
            /* Unreachable. */
            break;
    }

    /* Don't need those tuples around any more, now that we've
     * loaded them into the buckets. */
    tupbuf_clear(&(state->tb));
}

/* FUNCTION tupbuf_init
 * ARGUMENTS: <tbp> points to a newly-allocated tuple buffer.
 * PRECONDITIONS: <tbp>->cap contains the indended capacity of the buffer.
 * POSTCONDITIONS: Prepares that buffer for insertion.  The caller is
 *      responsible for calling tupbuf_destroy() on the buffer.
 */
void tupbuf_init(TupBuf *tbp) {
    Assert(tbp != NULL);
    Assert(tbp->cap > 0);
    Assert(tbp->cap < 1e9);

    tbp->buf = palloc(tbp->cap * (sizeof(*(tbp->buf))));
        /* Freed in tupbuf_destroy(). */

    tbp->count = 0;
}

/* Removes all tuples. */
void tupbuf_clear(TupBuf *tbp) {
    int tupnum = -1;

    Assert(tbp != NULL);
    Assert(tbp->buf != NULL);

    for (tupnum = 0; tupnum < tbp->count; tupnum++) {
        if (tbp->own_tuples) {
            heap_freetuple(tbp->buf[tupnum]);
        }
        tbp->buf[tupnum] = (HeapTuple)(0xDEADFACE);
    }

    tbp->count = 0;
}

void tupbuf_destroy(TupBuf *tbp) {
    Assert(tbp != NULL);
    Assert(tbp->buf != NULL);

    tupbuf_clear(tbp);

    pfree(tbp->buf);
    memset(tbp, 0xD, sizeof(*tbp));
}

/* Returns true if we filled up the buffer. */
bool tupbuf_insert(TupBuf *tbp, HeapTuple tup) {
    Assert(tbp->count < tbp->cap);

    /* SPECIAL CASE: Ignore punctuation tuples. */
    if (HeapTupleIsPunct(tup)) {
        heap_freetuple(tup);
        return false;
    }
    /* END SPECIAL CASE */

    tbp->buf[tbp->count] = tup;
    tbp->count++;

    if (tbp->count >= tbp->cap) {
        return true;
    } 
    return false;
}



/* Support function. */
void generic_create_ds(generic_stream_state *state) {
    switch (state->stype) {
        case ST_MHIST:
        {
            mhist_stream_state *mhs = &(state->ds_state.mhs);

            /* Figure out how many columns our tuples have. */
            int num_cols = get_num_non_timestamp_attrs(state->input_desc);


            /* Knowing how many columns we're using, we can create an empty 
             * histogram of the appropriate size. */
            mhs->hist = create_mhist(mhs->num_buckets, num_cols);

            /* Fill in information about the different dimensions. */
            tdesc_to_mhist_diminfo(mhs->hist, state->input_desc);

            mhist_create_buckets(mhs->hist, state->tb.buf, 
                    state->tb.count, state->input_desc);
        }
            break;
        case ST_SQUARE_MHIST:
        {
            square_mhist_stream_state *shs = &(state->ds_state.shs);

            shs->hist = create_reg_mhist(state->tb.buf,
                    state->tb.count, state->input_desc, shs->bucket_widths);

        }
            break;
        case ST_WAVELET:
        {
            wcs_stream_state *ws = &(state->ds_state.ws);
            ws->wcs = wcs_create_stream(state, FALSE);
        }
            break;

        case ST_SAMPLE:
        {
            sample_stream_state *ss = &(state->ds_state.ss);
            /* ss->max_sample_size should have been set for us */

            Assert(ss->max_sample_size > 0);

            ss->sample = sample_create(ss->max_sample_size);
                /* Destroyed by the wrapper clearinghouse. */
        }
            break;

        case ST_COUNT:
        {
            count_stream_state *cs = &(state->ds_state.cs);
            cs->count = 0;
        }
            break;

        default:
            elog(ERROR, "generic_create_ds(): Invalid synopsis type.");
            /* Unreachable. */
            break;
    }
}

// originally private

/* FUNCTION generic_stream_init()
 * ARGUMENTS: 
 *      <proto> points to a prototype mhist_stream_state object holding
 *      information about the number of buckets and number of tuples to buffer.
 *      <input_desc> describes the tuples we're going to get.
 *      <output_desc> describes the tuples we're going to produce.
 * PRECONDITIONS: We're in a long-lived memory context.
 * POSTCONDITIONS: Prepares to build an summary on a stream of tuples, using
 *      information encoded in <proto>.  
 *      Returns a newly-palloc'd struct containing the state of this 
 *      MHIST-building process.  
 */
generic_stream_state *generic_stream_init(generic_stream_state * proto, 
        TupleDesc input_desc, TupleDesc output_desc) 
{
	// PROF_ENTER_FUNCTION("generic_stream_init");

	generic_stream_state    *protoptr = (generic_stream_state*)proto;
    generic_stream_state    *state = NULL;

    Assert(stream_state_is_valid(protoptr));
    Assert(input_desc != NULL);
    Assert(output_desc != NULL);

    state = palloc(sizeof(*state));
        /* Freed in mhist_stream_done(). */
       
    /* Copy over the attributes from the prototype. */
    memcpy(state, protoptr, sizeof(*state));

    tupbuf_init(&(state->tb));


    state->state = MH_LOADING_BOUNDS;

    state->input_desc = input_desc;
    state->output_desc = output_desc;

    /* SPECIAL CASE: The sample summary factory needs to get tuple 
     * descriptors at stream binding time. 
     * (For other summary types, the output_desc member should be correctly set
     * in <proto>) */
    if (ST_SAMPLE == state->stype) {
        /* Figure out which column of our output tuples will contain the sample
         * fraction. */
        int sm_att = 
            sampmult_att(state->output_desc);

        if (-1 == sm_att) {
            elog(ERROR, "Couldn't find __samplemult attribute");
        } else if (sm_att != state->output_desc->natts - 1) {
            elog(ERROR, 
                "__samplemult attribute must be last attribute in tuple");
        }
        state->ds_state.ss.sampmult_att_0based = sm_att;
        
    }
    /* END SPECIAL CASE */


    Assert(stream_state_is_valid(state));

	// PROF_EXIT_FUNCTION("generic_stream_init");

	return state;
}



/* FUNCTION generic_stream_next()
 * ARGUMENTS: <state> is a state object created by generic_stream_init(), 
 *      and <tup> is a tuple.
 * PRECONDITIONS: You're in the same memory context you called
 *      mhist_stream_init() from, and <tup> was allocated in this context (VERY
 *      IMPORTANT) according to the tuple descriptor passed to
 *      generic_stream_init().
 * POSTCONDITIONS: Adds the indicated tuple to the synopsis under construction.
 */
void generic_stream_next(generic_stream_state *sp, HeapTuple tup) 
{
	// PROF_ENTER_FUNCTION("generic_stream_next");
	generic_stream_state *state = (generic_stream_state*)sp;

    Assert(stream_state_is_valid(state));
    Assert(tup != NULL);


    switch(state->state) {
        case MH_LOADING_BOUNDS:
            /* We're building up a buffer of tuples to pick bucket boundaries.
             */
            if (tupbuf_insert(&(state->tb), tup)) {
                /* tupbuf_insert() returns true if we filled up the buffer. */
                generic_create_ds(state);

                /* Don't forget that these initial tuples are part of the set
                 * we're synopsizing! */
                generic_load_ds(state);

                state->state = MH_LOADING_COUNTS;
            }

            break;

        case MH_LOADING_COUNTS:
            /* We've created the buckets and are populating them. */
            /* We load tuples a buffer at a time partly for symmetry and partly
             * because we're planning to make this function apply to other 
             * data structures, and those data structures might take blocks 
             * of tuples. */
            if (tupbuf_insert(&(state->tb), tup)) {
                generic_load_ds(state);
            }
            break;

        default:
            elog(ERROR, "mhist_stream_next(): Invalid state.");
            break;
    }

	// PROF_EXIT_FUNCTION("generic_stream_next");
}

/* FUNCTION generic_stream_done()
 * ARGUMENTS: <sp> is a state object 
 *      created by generic_stream_init() and possibly passed to 
 *      generic_stream_next() a couple of times.
 * PRECONDITIONS: You're in the same memory context you called
 *      generic_stream_init() from.
 * POSTCONDITIONS: Finalizes the summary, frees <sp>, and returns a new 
 *      summary containing the data passed to us.  
 *      NOTE: After calling this function, <sp> will point to nothing!
 */
generic_summary *generic_stream_done(generic_stream_state *sp) 
{
	// PROF_ENTER_FUNCTION("generic_stream_done");
    generic_summary *ret = palloc(sizeof(*ret));
        /* Will be returned; freed by caller. */

	generic_stream_state *state = (generic_stream_state*)sp;

    /* Empty out the current buffer of tuples. */
    switch(state->state) {
        case MH_LOADING_BOUNDS:

            generic_create_ds(state);

            /* Note the lack of a break statement here. */

        case MH_LOADING_COUNTS:
            generic_load_ds(state);

            break;

        default:
            elog(ERROR, "generic_stream_done(): Invalid state.");
            break;
    }

    state->state = MH_COMPLETE;

    /* Clean up. */
    tupbuf_destroy(&(state->tb));

    ret->stype = state->stype;

    switch(state->stype) {
        case ST_WAVELET:
            ret->summary.w = state->ds_state.ws.wcs;
            break;
        case ST_MHIST:
            /* Pack the MHIST to keep it as small as possible. */
            remove_empty_mhist_buckets(state->ds_state.mhs.hist);
            ret->summary.m =  state->ds_state.mhs.hist;
            break;
        case ST_SQUARE_MHIST:
            ret->summary.f = state->ds_state.shs.hist;
            break;

        case ST_SAMPLE:
            sample_prepare_for_output(state->ds_state.ss.sample,
                state->input_desc, state->output_desc, 
                state->ds_state.ss.sampmult_att_0based);
            ret->summary.s = state->ds_state.ss.sample;
            break;

        case ST_COUNT:
            ret->summary.c = state->ds_state.cs.count;
            break;

        default:
            elog(ERROR, "generic_stream_done(): Invalid synopsis type.");
            /* Unreachable. */
            break;
    }

    /* We don't own the tuple descriptor pointer, since we didn't make a copy.
     */

    /* But we do own the state pointer. */
    pfree(state);

	// PROF_EXIT_FUNCTION("generic_stream_done");

    return ret;
}

/* 
 * Functions that implement our sampling summary.
 *
 * We include them in this file because they are quite small.
 */
reservoir_sample * sample_create(int max_size) {
    reservoir_sample *ret = palloc(sizeof(*ret));
        /* Freed by sample_destroy() */
    ret->max_sample_size = max_size;
    ret->cur_sample_size = 0;
    ret->tot_size = 0;
    ret->reservoir = palloc(max_size * sizeof(*(ret->reservoir)));
        /* Freed by sample_destroy() */
    ret->cur_sample_itr = -1;

    return ret;
}

/* Prepare a completed sample for output. */
void sample_prepare_for_output(reservoir_sample *samp, 
        TupleDesc input_desc, TupleDesc output_desc, int sampmult_attr) 
{
    int i;
    float4 sampmult;


    Assert(sampmult_attr == input_desc->natts);

    /* Prepare the arguments for heap_modifytuple(): A new value and a mask
     * indicating which value is changed, as well as a Relation pointer. */
    sampmult = 
        0 == samp->cur_sample_size ? 0.0
            : (float4)samp->tot_size / (float4)(samp->cur_sample_size);
        /* The __sampmult column represents how many real tuples each tuple in
         * the sample stands for */

    for (i = 0; i < samp->cur_sample_size; i++) {
        HeapTuple newtup;
        Prototuple *copy;

        /* Disassemble the tuple.  This makes a copy.  Yuck. */
        copy = heaptupleToPrototuple(samp->reservoir[i], input_desc, 1);

        /* Add in the __sampmult column. */
        copy->fields[sampmult_attr] = Float4GetDatum(sampmult);
        copy->nulls[sampmult_attr] = ' ';

        /* Then reassemble the tuple.  This makes another copy :( */
        newtup = heaptupleFromPrototuple(copy, output_desc);
        destroyPrototuple(copy);

        /* We have to wait until after <newtup> is created to do this, because
         * <copy> might contain pointers into samp->reservoir[i]. */
        heap_freetuple(samp->reservoir[i]);

        samp->reservoir[i] = newtup;
    }
    
    /* Indicate that the sample is now in "consume" mode. */
    samp->cur_sample_itr = 0;
}

void sample_destroy(reservoir_sample *samp) {
    if (-1 == samp->cur_sample_itr) {
        samp->cur_sample_itr = 0;
    }

    /* Free any HeapTuples that haven't yet been consumed. */
    for (; samp->cur_sample_itr < samp->cur_sample_size; 
            samp->cur_sample_itr++) 
    {
        heap_freetuple(samp->reservoir[samp->cur_sample_itr]);
    }

    pfree(samp->reservoir);
    memset(samp, 0x0, sizeof(*samp));
    pfree(samp);
}

void sample_load(HeapTuple *tups, int tup_count, reservoir_sample *samp) {
    int i;

    /* We shouldn't be calling this function if we're currently consuming the
     * sample. */
    Assert(-1 == samp->cur_sample_itr);

    /* We use Alan Waterman's naive reservoir sampling algorithm.  There
     * are several more efficient algorithms, but the work we would save by
     * implementing one of them would be overwhelmed by the tuple
     * construction/destruction work that is currently unavoidable.
     *
     * For further reading, see
     * Jeffrey Scott Vitter's paper "Random Sampling with a Reservoir" 
     * in ACM Trans. Mathematical Software */

    for (i = 0; i < tup_count; i++) {
        HeapTuple cur_tup = tups[i];
        double randnum = ((double)rand()) / ((double)RAND_MAX);
            /* Random number from 0 to 1. */

        /* The algorithm works by maintaining the invariant that the reservoir
         * is always a random sample of the tuples we've seen so far. */

        /* The probability that the current tuple will go into the reservoir is
         * (tot_size + 1) / (cur_sample_size) */
        double prob_use_tup = ((double)(samp->tot_size) + 1.0) 
                                / (double)(samp->cur_sample_size);
        if (randnum <= prob_use_tup) {
            if (samp->cur_sample_size < samp->max_sample_size) {
                /* If the reservoir is not full, append to the end of it */
                samp->reservoir[samp->cur_sample_size] = cur_tup;
                samp->cur_sample_size++;
            } else {
                /* If the reservoir is full, choose a victim and replace the
                 * victim. */
                int victim_ix = 
                    (int)(
                        samp->cur_sample_size * ((double)rand()) 
                            / ((double)RAND_MAX));
                heap_freetuple(samp->reservoir[victim_ix]);
                samp->reservoir[victim_ix] = cur_tup;
            }
        } else {
            /* We're not using this tuple, so we can garbage-collect it. */
            heap_freetuple(cur_tup);
        }

        samp->tot_size++;
    }
}

/* Figure out which attribute of a tuple descriptor is the __samplemult
 * attribute. 
 * Returns a 0-based index! 
 * Returns -1 if no attribute is found. */
int sampmult_att(TupleDesc desc) {
    int attno;
    for (attno = 0; attno < desc->natts; attno++) {
		if (namestrcmp(&(desc->attrs[attno]->attname), "__samplemult") == 0)
        {
            return attno;
        }
    }
    return -1;
}


/*
 * The Prototuple data structure.
 */
Prototuple *createPrototuple(int nfields, int nextra,
        const Datum *fields, const char *nulls, bool isPunct) {
    Prototuple *ret;
    int i;

    Assert(fields != NULL);
    Assert(nulls != NULL);
    Assert(nextra >= 0);
    Assert(nfields >= 0);
    
    ret = palloc(sizeof(*ret));

    ret->fields = palloc(sizeof(*(ret->fields)) * (nfields + nextra));
    ret->nulls = palloc(sizeof(*(ret->nulls)) * (nfields + nextra + 1));

    for (i = 0; i < nfields; i++) {
        ret->fields[i] = fields[i];
        ret->nulls[i] = nulls[i];
    }

    ret->nulls[nfields + nextra] = '\0';

    ret->isPunct = isPunct;

    ret->nfields_used = nfields;
    ret->nfields_alloc = nfields + nextra;

    return ret;
}

Prototuple *heaptupleToPrototuple(HeapTuple tup, TupleDesc desc,
        int nextra) {
    Prototuple *ret;
    int i;
    int nfields = desc->natts;
    
    ret = palloc(sizeof(*ret));

    ret->fields = palloc(sizeof(*(ret->fields)) * (nfields + nextra));
    ret->nulls = palloc(sizeof(*(ret->nulls)) * (nfields + 1));

    for (i = 0; i < nfields; i++) {
        bool isnull;
        ret->fields[i] = heap_getattr(tup, i + 1, desc, &isnull);
            /* Note that the attribute numbers are 1-based!!! */
        ret->nulls[i] = isnull ? 'n' : ' ';
    }

    ret->nfields_used = nfields;
    ret->nfields_alloc = nfields + nextra;

    ret->nulls[nfields] = '\0';

    ret->isPunct = HeapTupleIsPunct(tup);

    return ret;
     
}

HeapTuple heaptupleFromPrototuple(Prototuple *pt, TupleDesc desc) {
    HeapTuple ret;

    Assert(pt != NULL);

    if (pt->isPunct) {
        int ts_attr = desc->constr->ts_attrnum;
        Timestamp ts = DatumGetTimestamp(pt->fields[ts_attr]);

        elog(LOG, "WCH creating a punctuation tuple.");

        ret = form_punct_tup(desc, ts);

    } else {
        
        ret = heap_formtuple(desc, pt->fields, pt->nulls);
    }

    return ret;
}

void destroyPrototuple(Prototuple *pt) {
    Assert(pt != NULL);
    Assert(pt->fields != NULL);
    Assert(pt->nulls != NULL);

    pfree(pt->fields);
    pfree(pt->nulls);

    memset(pt, 0x0, sizeof(*pt));

    pfree(pt);
}

Timestamp timestampFromPrototuple(Prototuple *pt, TupleDesc desc) {
    Datum ret;

    /* Make sure this tuple has a timestamp column... */
    Assert(tc_has_timestamp(desc->constr));
    Assert(get_timestamp_col(desc) < pt->nfields_used);
    
    ret = pt->fields[get_timestamp_col(desc)];

    return DatumGetTimestamp(ret);
}


/* FUNCTION really_test_sampling_speed
 * ARGUMENTS: <sample_size> and <numtups> are the parameters for the
 * microbenchmark run.
 * POSTCONDITIONS: Times the insertion of the indicated number of tuples into a
 * sample of the indicated size.  Returns seconds elapsed.
 */
double really_test_sampling_speed(int sample_size, int numtups) {
    struct timeval  start_tv;
    struct timeval  end_tv;
    reservoir_sample *samp;
    int i;
    Datum vals[2];
    char *nulls = "  ";
    HeapTuple *input = NULL;

    /* Create a dummy tuple descriptor for a relation with two integer columns.
     */
    TupleDesc desc = CreateTemplateTupleDesc(2, false);

    TupleDescInitEntry(desc, 1 /* attribute numbers are 1-based! */ ,
                       "a", INT4OID, -1, 0, false);
    TupleDescInitEntry(desc, 2 /* attribute numbers are 1-based! */ ,
                       "b", INT4OID, -1, 0, false);

    /* Create the set of "tuples" */
    /* HeapTuples are opaque to sampling, so values don't matter. */
    vals[0] = Int32GetDatum(42);
    vals[1] = Int32GetDatum(42);

    input = palloc(numtups * (sizeof(*input)));

    for (i = 0; i < numtups; i++) {
        input[i] = heap_formtuple(desc, vals, nulls);
    }

    /* Create the reservoir data structure. */
    samp = sample_create(sample_size);

    /* Start the clock */
    gettimeofday(&start_tv, 0);

    /* Do sampling */
    sample_load(input, numtups, samp);

    /* Stop the clock. */
    gettimeofday(&end_tv, 0);

    pfree(input);
    sample_destroy(samp);

    /*
    elog(NOTICE, "Start is %u.%06u; end is %u.%06u",
            (unsigned)start_tv.tv_sec, (unsigned)start_tv.tv_usec, 
            (unsigned)end_tv.tv_sec, (unsigned)end_tv.tv_usec);

    elog(NOTICE, "Total elapsed is %lld usec", tv_diff(&start_tv, &end_tv));
    */
            
    /* Return the seconds elapsed. */
    return (((double)(tv_diff(&start_tv, &end_tv))) / 1e6);
}

