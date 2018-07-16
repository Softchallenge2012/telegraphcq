/*
 *	PostgreSQL type definitions for built-in histogram data types.
 *
 *	$Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/histogram.c,v 1.40 2005/07/27 20:04:12 phred Exp $ 
 */

#define FRED_VLDB_HACKS

#ifdef FRED_VLDB_HACKS
/*#define FRED_MHIST_FK_HACK*/
#endif /* FRED_VLDB_HACKS */

/* Uncomment the following line to enable time-consuming sanity checks on
 * MHISTs. */
/*#define ENABLE_COMPLEX_MHIST_CHECKS*/

#include <ctype.h>
#include <sys/time.h> /* yang */

    /* For toupper(3) */

#include "postgres.h"

#include "access/hash.h"
#include "catalog/namespace.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "lib/stringinfo.h"
#include "utils/builtins.h"
#include "utils/elog.h"
#include "utils/relcache.h"

#include "utils/lsyscache.h"
    /* For get_typlenbyval() function. */


#include "telegraphcq/shedding.h"
#include "telegraphcq/histogram.h"
    

#define ENABLE_STREAM_STATE_CHECKS


/*******************************************************************************
 * INTERNAL DATA STRUCTURES
 ******************************************************************************/

/* Used for passing maxdiff information back. 
 *
 * In particular, get_maxdiffs() returns a pointer to an array of these, one per
 * bucket. */
typedef struct bucket_maxdiff_info {
    int abs;
        /* The absolute difference between two adjacent tuples when sorted
         * along a particular dimension. */


    int dimid;
        /* The dimension along which this difference occurs. */

    double norm;
        /* Normalized to the min. and max. values along this dimension. */

    bool empty_space_upper;
    bool empty_space_lower;
        /* True if there is empty space at the edge of the bucket. */

} bucket_maxdiff_info;


/*******************************************************************************
 * PROTOTYPES FOR INTERNAL FUNCTIONS 
 ******************************************************************************/

static mhist * real_mhist_in(char *relname, 
		char **cols, 
		int ncols, 
		int num_buckets);

static void mhist_set_nameinfo(mhist *hist, char *relname, 
                                char **cols, int ncols);

static HeapTuple *get_table_as_array(char *relname, char **cols, int ncols,
        int *ntuples, TupleDesc *descdest);

static bool split_a_bucket(HeapTuple *giant_tup_array, int ntuples, 
        TupleDesc desc,
        int *bucket_offsets, int *bucket_lengths, mhist *hist, 
        mhist_dim_info *ranges, int nbuckets_done);

static void sort_tuples_along_dim(HeapTuple *tuples, int length, int dimid,
        TupleDesc desc);

static void load_mhist(char *relname, char **cols, mhist *hist);

static mhist * create_mhist_copy(const mhist *orig);
static mhist * create_bigger_mhist(mhist *hist);

mhist * create_proj_mhist(const mhist * original, int dim);

void srf_firstcall_junk(FuncCallContext *funcctx, void *state, int max_calls,
        char *typename);

void srf_cleanup(FuncCallContext *funcctx);

mhist_dim_info * get_ranges(HeapTuple *giant_tup_array, int ntuples, 
        TupleDesc desc, int ndims);

bucket_maxdiff_info *get_maxdiffs(
        mhist *hist,
        HeapTuple *giant_tup_array, 
        TupleDesc desc,
        const int *bucket_offsets,
        const int *bucket_lengths, 
        int nbuckets_done, 
        const mhist_dim_info *ranges);

int get_mhist_bucket_for_tuple(mhist *hist, int *tuple, size_t nfields);

mhist * real_mhist_project(mhist *inhist, int32 proj_dim);

void project_mhist_bucket(mhist_bucket *src, mhist_bucket *dest, 
        int ndims, int proj_dim);


mhist *merge_overlapping_mhist_buckets(mhist *hist);

mhist *really_merge_overlapping_mhist_buckets(mhist *hist, bool *found_overlap,
        bool verbose);

mhist *merge_two_overlapping_buckets(mhist *hist, 
        int left_bucketid, int right_bucketid, bool verbose);

void cut_top_off_mhist_bucket(mhist_bucket *bucket_ptr, int dimid, int new_max);

void cut_bottom_off_mhist_bucket(mhist_bucket *bucket_ptr, int dimid, 
        int new_min);

bool potential_overlap(mhist_bucket *left_bucket, mhist_bucket *right_bucket,
        int ndims);

double mhist_total_count(mhist *hist);

char *mhist_to_str(mhist *hist);
 
mhist * mhist_cross_product(const mhist *hist1, const mhist *hist2);

void mhist_equality_select_in_place(mhist *hist, int dim1, int dim2);

void check_mhist_integrity(const mhist *hist, const char *id);

void get_mhist_split_point(
        mhist *hist,
        HeapTuple *giant_tup_array, TupleDesc desc, 
        const int *bucket_offsets,
        const int *bucket_lengths, 
        int nbuckets_done, 
        const mhist_dim_info *ranges,
        int *split_bucketid, int *split_dimid, int *split_offset);

bool no_buckets_to_split(HeapTuple *giant_tup_array,
        const int *bucket_offsets, 
        const int *bucket_lengths, 
        int nbuckets_done,
        TupleDesc desc);

void mhist_stream_load_buckets(mhist_stream_state *state);

mhist *fghist_equijoin(mhist *hist1, mhist *hist2, 
        int col1, int col2);

/* THE FOLLOWING FUNCTIONS ARE NOW EXPORTED because they are called by the
 * shared synopsis code in shedding.c 
void mhist_create_buckets(mhist *hist, HeapTuple *tuparr, int ntups, 
        TupleDesc desc);

void tdesc_to_mhist_diminfo(mhist *hist, TupleDesc desc);

mhist * create_reg_mhist(HeapTuple *tups, int ntups, 
        TupleDesc desc, int *bucket_widths);

void remove_empty_mhist_buckets(mhist *hist);
******/
void synopsis_op_delay(void);

int create_reg_buckets(mhist *hist, mhist_dim_info *ranges, int num_dims,
        int cur_dim, int *bucket_widths, int bucket_num, int *itr);

int remove_duplicate_names(mhist_dim_name *names, int num_names);

int get_dimid_for_name(mhist_dim_name *names, int num_names, 
        const char *name);

int get_random(HeapTuple*, int, int, int, int, double);

void create_prob_array(double*, double, int);

int skip_random(double*, int);

void print_array(HeapTuple*, int);

int fghist_stepsize_along_dimension(mhist *fghist, int dimid);

mhist_dim_info fghist_range_along_dimension(mhist *fghist, int dimid);

int fghist_num_bins_along_dimension(mhist *fghist, int dimid);

int fghist_binid_along_dimension(mhist *fghist, int dimid, int val);

int fghist_binid_along_dimension_safe(mhist *fghist, int dimid, int val);

int mhist_approx_empty_bkts(mhist *hist);
 

/* FUNCTION synopsis_insertion_delay
 * POSTCONDITIONS: Spins for the amount of time indicated by g_synopsis_delay.
 */
extern int load_shedding_synopsis_op_delay; /* In postgresql.conf */
void synopsis_op_delay(void) {
    int i, j;
    for (i = 0; i < load_shedding_synopsis_op_delay; i++) {
        for (j = 0; j < 10000; j++);
    }
}


/*******************************************************************************
 * EXTERNAL FUNCTIONS
 ******************************************************************************/

/* FUNCTION create_mhist
 * ARGUMENTS: <num_buckets> is the number of buckets for a new histogram, 
 *		and <num_dims> is the number of dimensions of the histogram.
 * POSTCONDITIONS: Creates a new, empty histogram data structure and fills in
 *		the header fields.
 */
mhist * create_mhist(int num_buckets, int num_dims) {
    mhist *result = NULL;
    int32 hist_len = MHIST_SIZE(num_buckets, num_dims);

    if (hist_len < MHIST_HDRSZ) {
        elog(ERROR, "Invalid MHIST length of %d.", hist_len);
    }

    result = (mhist*)palloc(hist_len);

    /*
    elog(LOG, "create_mhist(): Created a structure of size %d at %p.",
            hist_len, result);
    elog(LOG, "create_mhist():     Last byte at %p.", 
            ((char*)result + hist_len) );
    */

    /* Zero out the struct so as not to screw up Postgres hash functions. */
    memset(result, 0x0, hist_len);

    result->hdr.length = hist_len;
    result->hdr.numDims = num_dims;
	result->hdr.numBucketsUsed = 0;
    result->hdr.numBucketsAllocated = num_buckets;

    MHIST_SET_IS_FGHIST(result,false);

    return result;
}

/* FUNCTION mhist_load_buckets()
 * ARGUMENTS: <giant_tup_array> is an array containing a bunch of tuples.
 *      <ntuples> is the number of tuples in the array.
 *      <hist> is an MHIST.
 * PRECONDITIONS: <hist> has not been loaded yet, but its bucket boundaries
 *      have already been determined.
 * POSTCONDITIONS: Fills up the buckets array of <hist> with information about
 *      the indicated buckets.
 */
void mhist_load_buckets(HeapTuple *giant_tup_array, TupleDesc desc,
        int ntuples, mhist *hist) 
{
	/* PROF_ENTER_FUNCTION("mhist_load_buckets"); */
    int     tupleid = -1;
    int     colvals[MHIST_MAX_DIMS];

    /* SPECIAL CASE: No tuples to load. */
    if (0 == ntuples) { 
        return;
    }
    /* END SPECIAL CASE */

    for (tupleid = 0; tupleid < ntuples; tupleid++) {
        int dimid;
        int bucketid;
        mhist_bucket *target_bucket_ptr;
        for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
            colvals[dimid] =
                get_int_field_val(giant_tup_array[tupleid], desc, 
                    dimid+1 /* IMPORTANT: Attribute numbers are 1-based! */,
                    true);
        }

        /* Figure out what bucket this tuple should go into. */
        bucketid = get_mhist_bucket_for_tuple(hist, colvals, 
                                                    MHIST_NUM_DIMS(hist));

        target_bucket_ptr = MHIST_BUCKET_PTR(hist, bucketid);

        /* Increment the counters. */
        /* TODO: Calculate the number of unique values along each dimension! */
        target_bucket_ptr->count += 1.0;
    }

	/* PROF_EXIT_FUNCTION("mhist_load_buckets"); */
}



/*
 * MHist constructor.  Takes a single string argument, in the following format:
 *
 *	(<Relation name>),(<col1>,col2>,...,<coln>),(<numbuckets>)
 *     
 * Where:
 *		<Relation name> is the name of the relation to be histogrammed.
 *		<col1>,col2>,...,<coln> are the columns of the relation to be
 *				included in the histogram.
 *		<numbuckets> is the number of buckets in the histogram.
 */
Datum
mhist_in(PG_FUNCTION_ARGS)
{
	/* PROF_ENTER_FUNCTION("mhist_in"); */

	char		*str = PG_GETARG_CSTRING(0);
	char		*str_copy = NULL;
	char		*relname_str = NULL, *cols_str = NULL, *num_buckets_str = NULL;
	char		**cols_arr = NULL;
	int			ncols = -1, colno = -1;
	char		*ptr = NULL;
    mhist       *result = NULL;

    /* TODO: Sould the parsing code below be moved to a single function? */

    /* Parse a copy of the string, strtok() style. */
	str_copy = palloc(1 + strlen(str));
	strcpy(str_copy, str);

	/* First the relation name, "(name)". */
	if (str_copy[0] != '(') goto parse_error;
	relname_str = ptr = str_copy + 1;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Relation string is '%s'.", relname_str);*/

	/* Skip the comma between the parens. */
	if (*ptr != ',') goto parse_error; ++ptr;

	/* Then parse the column names, "(col1,col2,...,coln)". */
	if (*ptr != '(') goto parse_error; ++ptr;
	cols_str = ptr;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Cols string is '%s'.", cols_str);*/

	/* Skip another comma, and parse the number of buckets. */
	if (*ptr != ',') goto parse_error; ++ptr;
	if (*ptr != '(') goto parse_error; ++ptr;
	num_buckets_str = ptr;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Num. buckets string is '%s'.", num_buckets_str);*/

	/* TODO: We currently ignore any junk at the end of the string.  Should we
	 * read through it and complain about non-whitespace characters? */

	/* Generate an array of column names. */
	/* First we need to count the number of columns. */
	ncols = 1;
	ptr = cols_str;
	if (ptr == '\0') goto parse_error;
	while (*ptr != '\0') {
		if (',' == *ptr)  ++ncols;
		++ptr;
	}

	Assert(ncols <= MHIST_MAX_DIMS);

	
	/* Then we generate the array, again by parsing strtok()-style. */
	cols_arr = palloc(ncols * sizeof(char*));
	ptr = cols_str;
	for (colno = 0; colno < ncols - 1; colno++) {
		cols_arr[colno] = ptr;
		while (*ptr != ',') ++ptr;
		*ptr = '\0'; ++ptr;
	}
	cols_arr[ncols-1] = ptr;

	/* Generate the histogram object. */
	result = real_mhist_in(relname_str, cols_arr, ncols, 
            atoi(num_buckets_str));

	/* Clean up. */
    /*elog(LOG, "mhist_in(): Cleaning up.");*/
	pfree(str_copy);
	pfree(cols_arr);

	/* PROF_EXIT_FUNCTION("mhist_in"); */

	/* Now we're done. */
	PG_RETURN_MHIST_P(result);

	/* Error handlers. */
parse_error:
	/* elog(ERROR) should clean up everything we palloc()'d. */
	elog(ERROR, "mhist_in(): Error parsing string '%s'.", str);

	/* PROF_EXIT_FUNCTION("mhist_in"); */

	/* UNREACHABLE, but the compiler might not know that. */
	PG_RETURN_MHIST_P(NULL);
}

/*
 * Constructor that builds an MHIST with square buckets.  Takes a single 
 * string argument, in the following format:
 *
 *(<Relation name>),(<col1>,col2>,...,<coln>),(<width1>,<width2>,...,<widthn>)
 *     
 * Where:
 *		<Relation name> is the name of the relation to be histogrammed.
 *		<col1>,col2>,...,<coln> are the columns of the relation to be
 *				included in the histogram.
 *		<bucketwidth> is the width of a single bucket along each dimension.
 */
Datum
square_mhist_in(PG_FUNCTION_ARGS)
{
	/* PROF_ENTER_FUNCTION("square_mhist_in"); */

	char		*str = PG_GETARG_CSTRING(0);
	char		*str_copy = NULL;
	char		*relname_str = NULL, *cols_str = NULL, 
                    *bucketwidth_str = NULL;
	char		**cols_arr = NULL;
    int         *bktdims_arr = NULL;
	int			ncols = -1, colno = -1;
	char		*ptr = NULL;
    mhist       *result = NULL;
    HeapTuple   *giant_tup_array = NULL;
    TupleDesc   desc = NULL;
    int         ntuples = -1;

    /* TODO: Should the parsing code below be moved to a single function? */

    /* Parse a copy of the string, strtok() style. */
	str_copy = palloc(1 + strlen(str));
	strcpy(str_copy, str);

	/* First the relation name, "(name)". */
	if (str_copy[0] != '(') goto parse_error;
	relname_str = ptr = str_copy + 1;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Relation string is '%s'.", relname_str);*/

	/* Skip the comma between the parens. */
	if (*ptr != ',') goto parse_error; ++ptr;

	/* Then parse the column names, "(col1,col2,...,coln)". */
	if (*ptr != '(') goto parse_error; ++ptr;
	cols_str = ptr;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Cols string is '%s'.", cols_str);*/

	/* Skip another comma, and parse the number of buckets. */
	if (*ptr != ',') goto parse_error; ++ptr;
	if (*ptr != '(') goto parse_error; ++ptr;
	bucketwidth_str = ptr;
	while (*ptr != ')') {
		if ('\0' == *ptr) goto parse_error; ++ptr;
	}
	*ptr = '\0'; ++ptr;

	/*elog(LOG, "mhist_in(): Num. buckets string is '%s'.", bucketwidth_str);*/

	/* TODO: We currently ignore any junk at the end of the string.  Should we
	 * read through it and complain about non-whitespace characters? */

	/* Generate an array of column names. */
	/* First we need to count the number of columns. */
	ncols = 1;
	ptr = cols_str;
	if (ptr == '\0') goto parse_error;
	while (*ptr != '\0') {
		if (',' == *ptr)  ++ncols;
		++ptr;
	}

	Assert(ncols <= MHIST_MAX_DIMS);

	
	/* Then we generate the array, again by parsing strtok()-style. */
	cols_arr = palloc(ncols * sizeof(char*));
	ptr = cols_str;
	for (colno = 0; colno < ncols - 1; colno++) {
		cols_arr[colno] = ptr;
		while (*ptr != ',') ++ptr;
		*ptr = '\0'; ++ptr;
	}
	cols_arr[ncols-1] = ptr;

    /* Do the same for the bucket dimensions */
    bktdims_arr = palloc(ncols * sizeof(int));
	ptr = bucketwidth_str;
	for (colno = 0; colno < ncols - 1; colno++) {
		bktdims_arr[colno] = atoi(ptr);
		while (*ptr != ',') ++ptr;
		*ptr = '\0'; ++ptr;
	}
	bktdims_arr[ncols-1] = atoi(ptr);


    /* Read in the tuples for this histogram. */
    giant_tup_array = get_table_as_array(relname_str, cols_arr, ncols,
                                        &ntuples, &desc);

	/* Generate the histogram object. */
    result = create_reg_mhist(giant_tup_array, ntuples, desc, bktdims_arr);

    /* create_reg_mhist() doesn't know about the relation names, because
     * they're not in the tuple descriptor.  So we overwrite the dimension name
     * information. */
    mhist_set_nameinfo(result, relname_str, cols_arr, ncols);

    /* Fill its buckets. */
    mhist_load_buckets(giant_tup_array, desc, ntuples, result);

	/* Clean up. */
    /*elog(LOG, "mhist_in(): Cleaning up.");*/
	pfree(str_copy);
	pfree(cols_arr);

	/* PROF_EXIT_FUNCTION("square_mhist_in"); */

    /* Remove buckets that didn't get anything in them. */
    /* Or don't, because we currently don't have a fast way of processing
     * sparse fghists. 
    remove_empty_mhist_buckets(result);
    */

	/* Now we're done. */
	PG_RETURN_MHIST_P(result);

	/* Error handlers. */
parse_error:
	/* elog(ERROR) should clean up everything we palloc()'d. */
	elog(ERROR, "square_mhist_in(): Error parsing string '%s'.", str);

	/* PROF_EXIT_FUNCTION("square_mhist_in"); */

	/* UNREACHABLE, but the compiler might not know that. */
	PG_RETURN_MHIST_P(NULL);
}


/*
 * Function that creates a 2-D mhist from dimensions stored directly in a
 * table.  Arguments are:
 *
 * <relation name>,<dim1name>,<dim2name>
 *
 * Where:
 *      <relation name> is the name of the source relation
 *      <dim1name> and <dim2name> are the names you want to give the dimensions
 *          in the MHIST.
 *
 * The format of the source relation:
 *      dim1min integer,
 *      dim1max integer,
 *      dim2min integer,
 *      dim2max integer,
 *      count integer
 *
 * Note that the columns need to have these names!
 */
Datum
mhist_from_table(PG_FUNCTION_ARGS)
{
    /* Arguments */
    char *relname = PG_GETARG_CSTRING(0);
    char *dim1name = PG_GETARG_CSTRING(1);
    char *dim2name = PG_GETARG_CSTRING(2);

    /* Constants */
    char *MHIST_FROM_TABLE_COLNAMES[] = {
        "dim1min","dim1max","dim2min","dim2max","count"
    };
    char **COLNAMES = MHIST_FROM_TABLE_COLNAMES;
    const int NCOLS = 5;

    /* Variables */
    HeapTuple   *tups;
    int         ntups = -1;
    TupleDesc   desc = NULL;
    mhist       *result = NULL;
    int         i = -1;


    /* Grab the tuples */
    tups = get_table_as_array(relname, COLNAMES, NCOLS, &ntups, &desc);

    
    /* Create the MHIST */
    result = create_mhist(ntups, 2 /* num dims */);

    /* Set the dimension names */
    MHIST_SET_NUM_DIM_NAMES(result, 2);
    MHIST_SET_NAME_DIM(result,0,0);
    strncpy(MHIST_DIM_NAME(result,0), dim1name, MHIST_COL_NAME_LEN);
    MHIST_SET_NAME_DIM(result,1,1);
    strncpy(MHIST_DIM_NAME(result,1), dim2name, MHIST_COL_NAME_LEN);

    /* Use the tuples to make the buckets. */ 
    for (i = 0; i < ntups; i++) {
        int dim1min, dim1max, dim2min, dim2max, count;
        mhist_dim_info *di_ptr;
        mhist_bucket *bkt_ptr;

        dim1min = get_int_field_val(tups[i], desc, 1, false);
        dim1max = get_int_field_val(tups[i], desc, 2, false);
        dim2min = get_int_field_val(tups[i], desc, 3, false);
        dim2max = get_int_field_val(tups[i], desc, 4, false);
        count   = get_int_field_val(tups[i], desc, 5, false);

        /* Set ranges */
        bkt_ptr = MHIST_BUCKET_PTR(result, i);

        di_ptr = MHIST_BUCKET_DIMINFO_PTR(bkt_ptr, 0);
        di_ptr->min_val = dim1min;
        di_ptr->max_val = dim1max;

        di_ptr = MHIST_BUCKET_DIMINFO_PTR(bkt_ptr, 1);
        di_ptr->min_val = dim2min;
        di_ptr->max_val = dim2max;

        /* Set count */
        bkt_ptr->count = (double)count;

        MHIST_SET_NUM_BUCKETS_USED(result, i+1);
    }

    PG_RETURN_MHIST_P(result);
}


/*
 *	MHIST-to-string conversion function (for output).
 *
 *	It's hard to convert a histogram to a one-line string, so this function
 *	just prints a note to the effect that this is an MHIST.
 */
Datum
mhist_out(PG_FUNCTION_ARGS)
{
	mhist      *hist = PG_GETARG_MHIST_P(0);
	char	   *result = mhist_to_str(hist);

	PG_RETURN_CSTRING(result);
}

/*
 * Function to convert an mhist into relational representation.  This is a
 * set-returning function (See section 9.5.6.2 of the Programmer's Reference),
 * so it needs to save its state across multiple calls.  Yuck.
 *
 * Based on hist_to_table().
 *
 * In order to call this function, you need to define the following SQL
 * datatypes:
 *
 * CREATE_TYPE __mhist1 AS (bucketid, min0, max0, count);
 * CREATE_TYPE __mhist2 AS (bucketid, min0, max0, min1, max1, count);
 * CREATE_TYPE __mhist3 AS (bucketid, min0, max0, min1, max1, min2, max2, 
 *                          count);
 * CREATE_TYPE __mhist4 AS (bucketid, min0, max0, min1, max1, min2, max2,
 *                          min3, max3, count);
 * [Add additional types here if you change the value of HISTOGRAM_MAX_DIMS.]
 */
Datum
mhist_to_table(PG_FUNCTION_ARGS)
{
	mhist			        *hist = PG_GETARG_MHIST_P(0);
	/* PROF_ENTER_FUNCTION("mhist_to_table"); */

	FuncCallContext		    *funcctx = NULL; 
	mhist_to_table_state    *state = NULL;
/*	TupleDesc			    tupdesc;*/
	char				    typename[128];

    check_mhist_integrity(hist, "mhist_to_table()");
		
	if (SRF_IS_FIRSTCALL()) { 
        MemoryContext		    oldcontext; 

        funcctx = SRF_FIRSTCALL_INIT(); 

        /*elog(LOG, "mhist_to_table(): Starting first-call setup.");*/

		
   		/* Create the data structure that will remember the status of this
		 * function. */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx); 
		state = palloc(sizeof(*state));
        MemoryContextSwitchTo(oldcontext);

        state->current_bucket = 0;
        state->num_buckets = MHIST_NUM_BUCKETS_USED(hist);

        /* Generate the name of the type that corresponds to the histogram's
		 * number of dimensions. */
		snprintf(typename, sizeof(typename), "__mhist%d", hist->hdr.numDims);

        /* Postgres SRF crap has been moved to this function: */
	    srf_firstcall_junk(funcctx, state, 
                MHIST_NUM_BUCKETS_USED(hist), typename);

        /*elog(LOG, "mhist_to_table(): Done with first-call setup.");*/
	} 
	
	/* Stuff that gets done every time the function is called: */
	funcctx = SRF_PERCALL_SETUP(); 
	
	state = (mhist_to_table_state*)(funcctx->user_fctx);
		
	/* Are we done returning tuples? */
	if (state->current_bucket < state->num_buckets) {
		char		**values;
        int         numvalues = -1;
		HeapTuple	tuple;
		Datum		result;
		int			valid = -1;

        /*
        elog(LOG, "mhist_to_table(): Generating tuple for bucket %d (%d total).",
                state->current_bucket, state->num_buckets);
                */

		/* Not done returning tuples -- construct the next one. */
		/* For the sake of simplicity, we convert the field values into strings
		 * and then back into values. */

        numvalues = 2 * hist->hdr.numDims + 2; 
			/* Number of items in row == 2 + (2 x number of dimensions:
             *   1 column for the bucketID (not strictly necessary)
             *   1 column for the bucket's count
             *   2 columns per dimension to hold min and max values.
             */

        /*elog(LOG, "mhist_to_table():     This tuple has %d fields.",
                numvalues);*/

		/* Build up the array of string values. */
		values = (char**)palloc(sizeof(char*) * (numvalues));

		for (valid = 0; valid < numvalues; valid++) {
			values[valid] = (char*)palloc(32 * sizeof(char));
		}

		/* Fill in the array. */
		for (valid = 0; valid < numvalues; valid++) {
            if (0 == valid) {
                /* First entry is the bucketid. */
                snprintf(values[valid], 32, "%d", state->current_bucket);

            } else if (numvalues - 1 == valid) {
                /* Last entry is the bucket count. */
                mhist_bucket *cur_bucket = 
                    MHIST_BUCKET_PTR(hist, state->current_bucket);
                /*
                snprintf(values[valid], 32, "%d", 
                        (int)(cur_bucket->count + 0.5));
                        */
                    /* Round up if necessary */
                snprintf(values[valid], 32, "%f", 
                        (cur_bucket->count));
                 
            } else {
                /* valid > 0 and < (numvalues-1) */
                /* Entries between first and last are the bounds of the
                 * rectangle that comprises this bucket.  These entries
                 * alternate between the lower and upper bound. */
                int dimid = (valid - 1) / 2;
                bool is_min = (valid % 2 == 1);
                    /* Is this a lower or upper bound? */
                mhist_bucket *cur_bucket = 
                    MHIST_BUCKET_PTR(hist, state->current_bucket);
                snprintf(values[valid], 32, "%d", 
                        is_min ?
                        cur_bucket->diminfo[dimid].min_val
                        : cur_bucket->diminfo[dimid].max_val);
            }

            /*elog(LOG, "mhist_to_table():     Value %d is '%s'.",
                    valid, values[valid]);*/

		}

        /*elog(LOG, "mhist_to_table():     Done generating values.");*/

		/* Construct the tuple for the strings. */
		tuple = BuildTupleFromCStrings(funcctx->attinmeta, values);

        /*elog(LOG, "mhist_to_table():     Done building tuple.");*/

		/* Make the tuple into a datum. */ 
		result = TupleGetDatum(funcctx->slot, tuple);

		/* Clean up. */
		for (valid = 0; valid < hist->hdr.numDims; valid++) {
            /*elog(LOG, "mhist_to_table(): Freeing field %d of a tuple.",
                    valid);*/
			pfree(values[valid]);
		}
        /*elog(LOG, "mhist_to_table(): Freeing holder for tuple %d.",
                state->current_bucket);*/
		pfree(values);

        /*elog(LOG, "mhist_to_table(): Done generating tuple for bucket %d.",
                state->current_bucket);*/

        ++(state->current_bucket);

		/* PROF_EXIT_FUNCTION("mhist_to_table"); */

		SRF_RETURN_NEXT(funcctx, result); 
	} else { 
        /*elog(LOG, "mhist_to_table(): Cleaning up.");*/

		/* here we are done returning items, and just need to clean up: */ 
        srf_cleanup(funcctx);
		
		/* PROF_EXIT_FUNCTION("mhist_to_table"); */

		SRF_RETURN_DONE(funcctx); 
	}
}


/*
 * Function that removes a dimension of an MHIST, merging all bins along the
 * dimension.
 *
 * Usage: mhist_project(hist, dim)
 *
 * Where:
 *			<hist> is a histogram created with mhist_in().
 *			<dim> is the name of the dimension to eliminate.
 *
 * This function creates and returns a new MHIST.
 */
Datum
mhist_project(PG_FUNCTION_ARGS) {
	/* PROF_ENTER_FUNCTION("mhist_project"); */

	mhist		    *inhist = PG_GETARG_MHIST_P(0);	
	char			*proj_dim_name = PG_GETARG_CSTRING(1);

    mhist           *result = NULL;

    /* Figure out which index to project. */
    int             proj_dim = get_dimid_for_name(inhist->hdr.dimNames,
                                                    inhist->hdr.numDimNames,
                                                    proj_dim_name);

    synopsis_op_delay();

    if (MHIST_NOT_A_DIMID == proj_dim) {
        elog(ERROR, "mhist_project(): Dimension '%s' of '%s' not found.",
                proj_dim_name, mhist_to_str(inhist));
    }
    
    result = real_mhist_project(inhist, proj_dim);

	/* PROF_EXIT_FUNCTION("mhist_project"); */

    PG_RETURN_MHIST_P(result); 
}

/*
 * Function that removes all dimensions of an MHIST except for the dimensions
 * corresponding to the specified columns.
 *
 * Usage: mhist_project_many(hist, dimlist)
 *
 * Where:
 *			<hist> is a histogram created with mhist_in().
 *			<dimlist> is a string containing the names of the dimensions to 
 *			        keep, separated by spaces or commas.
 *
 * This function creates and returns a new MHIST.
 */
Datum
mhist_project_many(PG_FUNCTION_ARGS) {
	mhist		    *inhist = PG_GETARG_MHIST_P(0);	
	char			*dimlist = PG_GETARG_CSTRING(1);
    int             i = -1;
    int             num_cols_kept = -1;
    int             dimid = -1;
    char            dimlist_copy[512];
    char            *cols_kept[MHIST_MAX_DIMS];
    mhist           *curhist = NULL;
    bool            dims_to_keep[MHIST_MAX_DIMS];
    bool            first_copy = false;
    int             num_to_keep = -1;

	/* PROF_ENTER_FUNCTION("mhist_project"); */

    synopsis_op_delay();

    check_mhist_integrity(inhist, "mhist_project_many()");

    /* Parse the list of column names. */
    Assert(dimlist != NULL);

    /* In one pass, we make an uppercase copy, count the number of columns, 
     * and make an array of pointers to their names. */
    num_cols_kept = 1;
    cols_kept[0] = dimlist_copy;
    for (i = 0; dimlist[i] != '\0' && i < sizeof(dimlist_copy); i++) {
        if (' ' == dimlist[i] || ',' == dimlist[i]) {
            /* Names are separated by a single space. */
            dimlist_copy[i] = '\0';
            cols_kept[num_cols_kept] = dimlist_copy + (i+1);
            num_cols_kept++;
            Assert(num_cols_kept <= MHIST_MAX_DIMS);
        } else {
            dimlist_copy[i] = toupper(dimlist[i]);
        }
    }
    dimlist_copy[i] = '\0';

    /*
    elog(LOG, "mhist_project_many(): Keeping the following cols:");
    for (i = 0;  i < num_cols_kept; i++) {
        elog(LOG, "    '%s'", cols_kept[i]);
    }
    */

    /* Now we go through each column and see if it is one of the columns we
     * want to keep; if it is not, we will project it out. */
    curhist = inhist;
    for (dimid = 0; dimid < MHIST_NUM_DIMS(curhist); dimid++) {
        int     colid = -1;

        dims_to_keep[dimid] = false;
            /* Start out assuming we're going to throw away this column. */

        for (colid = 0; colid < num_cols_kept; colid++) {
            int nameid = -1;
            /*
            elog(LOG, "Checking %d names.", MHIST_NUM_DIM_NAMES(curhist));
            */
            for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(curhist); nameid++) 
            {
                /*
                elog(LOG, "Comparing '%s' with '%s'",
                        cols_kept[colid], MHIST_DIM_NAME(curhist, nameid));
                        */
                if (MHIST_NAME_DIM(curhist, nameid) == dimid
                        && 0 == strncasecmp(cols_kept[colid], 
                                            MHIST_DIM_NAME(curhist, nameid), 
                                            MHIST_COL_NAME_LEN)) 
                {
                    dims_to_keep[dimid] = true;
                        /* Want to keep this column. */
                    /*elog(LOG, "    Found a match!");*/
                }
            }
        }
    }

    /* Make sure we got the right number of columns. */
    num_to_keep = 0;
    for (dimid = 0; dimid < MHIST_NUM_DIMS(curhist); dimid++) {
        if (dims_to_keep[dimid]) {
            num_to_keep++;
        }
    }

    if (num_to_keep != num_cols_kept) {
        elog(ERROR, "Only found %d of %d columns '%s'.",
                num_to_keep, num_cols_kept, dimlist);
    }

    /* For now, we project dimensions out one by one, since that code is
     * already written.  We do the dimensions in reverse order since projection
     * causes the dimensions above the one projected to be renumbered. */
    /* TODO: Perform all the projections in one step. */
    first_copy = true;
    for (dimid = MHIST_NUM_DIMS(curhist) - 1; dimid >= 0; dimid--) {
        if (false == dims_to_keep[dimid]) {
            mhist *newhist = real_mhist_project(curhist, dimid);

            /* Don't want to pfree() the function argument, inhist. */
            if (first_copy) {
                first_copy = false;
            } else {
                pfree(curhist);
            }
            curhist = newhist; 
        }
    }

	/* PROF_EXIT_FUNCTION("mhist_project"); */

    /*
    elog(LOG, "mhist_project_many(): Histogram initially of size %d",
            curhist->hdr.length);
            */

    /* This function is usually called at the top of the tree of function calls
     * that the Data Triage rewriter generates, so we want to make sure the
     * histogram returned is as small as possible. */
    remove_empty_mhist_buckets(curhist);

    /*
    elog(LOG, "mhist_project_many(): Returning %s", mhist_to_str(curhist));
    elog(LOG, "   Size of histogram is %d", curhist->hdr.length);
    */

	PG_RETURN_MHIST_P(curhist); 
}



/*
 * Function that gets the cached summary of a table. 
 *
 * The cache is stored in an other table, with the name __tablename_kept or
 * __tablename_dropped.  Right now, this function does not populate the cache;
 * it only reads it.
 *
 * Argument:  <cachename> is the name of the cache table, e.g.
 *                  __tablename_kept or __tablename_dropped
 */
Datum get_cached_mhist(PG_FUNCTION_ARGS) {
	char		*cachename = PG_GETARG_CSTRING(0);

    mhist       *result = NULL;

    int         attrno = 1;
        /* We always fetch the first attribute. */

    bool        isnull = true;

    RangeVar   *relvar;
	Relation	rel;
	List	   *relname_list;

    Snapshot    snapshot;

    TupleDesc   cachedesc;

    HeapTuple   tup;
    HeapScanDesc scan;

	/* Open relation and get the tuple descriptor for it.  Code copied from
     * RelationNameGetTupleDesc().
     *
     * We leave the relation open so we can do a scan.
     */
	relname_list = stringToQualifiedNameList(cachename, 
            "RelationNameGetTupleDesc");
	relvar = makeRangeVarFromNameList(relname_list);
	rel = relation_openrv(relvar, AccessShareLock);
	cachedesc = RelationGetDescr(rel);

    /* Fetch the first column of the first tuple. */
    if (cachedesc->attrs[attrno - 1]->atttypid != MHISTOID) {
        elog(ERROR, "First column type ID %d doesn't match MHIST type %d.",
                cachedesc->attrs[0]->atttypid, MHISTOID);
    }

    /* We have to create our own "query snapshot", because we may be running
     * in the back end.  This is most unkosher.  Oh, well. */
    snapshot = GetSnapshotData(false);

    scan = heap_beginscan(rel, snapshot, 0, (ScanKey)NULL);
        /* TODO: Figure out what the "ScanKey" argument does */

    tup = heap_getnext(scan, ForwardScanDirection);

    result = DatumGetMhistP(heap_getattr(tup, attrno, cachedesc, &isnull));

    if (isnull) {
        elog(ERROR, "Got a null tuple instead of a histogram");
    }

    /* We're done with the scan. */
    heap_endscan(scan);
	relation_close(rel, AccessShareLock);

    /* TODO: Clean up!  We'll be called from inside continuous queries. */

    PG_RETURN_MHIST_P(result);
}

/*
 * Function that adds the corresponding buckets of two mhists.
 *
 * Usage: mhist_union(hist1, hist2)
 *
 * Where:
 *			<hist1> and <hist2> are MHISTs created with mhist_in().
 *
 * The dimension names for the output histogram will be taken from the 
 * union of the two histograms.
 */
Datum
mhist_union(PG_FUNCTION_ARGS) {
	/* PROF_ENTER_FUNCTION("mhist_union"); */

mhist		    *hist1 = PG_GETARG_MHIST_P(0);	
	mhist		    *hist2 = PG_GETARG_MHIST_P(1);	
	mhist		    *result = NULL;
	int				num_buckets = -1;
	int				bucketid = -1;
    int             nameid = -1;
    int             num_names = -1;
    mhist_dim_name  new_names[2 * MHIST_MAX_COL_NAMES];

    synopsis_op_delay();

	
	/* Verify the integrity of the inputs. */
    check_mhist_integrity(hist1, "mhist_union() hist1");
    check_mhist_integrity(hist2, "mhist_union() hist2");
	if (MHIST_NUM_DIMS(hist1) != MHIST_NUM_DIMS(hist2)) {
		elog(ERROR, 
		"mhist_union(): Histograms have different numbers of dimensions.");	
	}

    /*
    elog(LOG, "mhist_union():     Left arg is (%s).",
            mhist_to_str(hist1));
    elog(LOG, "mhist_union():     Right arg is (%s).",
            mhist_to_str(hist2));
            */

    /* Start out with the buckets of the original histograms. */
	num_buckets = MHIST_NUM_BUCKETS_USED(hist1) 
        + MHIST_NUM_BUCKETS_USED(hist2);

    /* Allocate a new histogram containing all the buckets from the input
     * histograms. */
    result = create_mhist(num_buckets, MHIST_NUM_DIMS(hist1)); 

    /* Copy over the dimension names, and remove duplicates. */
    for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist1); nameid++) {
        new_names[nameid] = MHIST_DIM_NAME_INFO(hist1, nameid);
    }

    for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist2); nameid++) {
        new_names[nameid + MHIST_NUM_DIM_NAMES(hist1)] = 
            MHIST_DIM_NAME_INFO(hist2, nameid);
    }

    num_names = remove_duplicate_names(new_names, 
            MHIST_NUM_DIM_NAMES(hist1) + MHIST_NUM_DIM_NAMES(hist2));

    if (num_names > MHIST_MAX_COL_NAMES) {
        elog(ERROR, "mhist_union(): Too many dimension names.");
    }

    memcpy(result->hdr.dimNames, new_names, num_names * sizeof(*new_names));

    MHIST_SET_NUM_DIM_NAMES(result, num_names);


    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist1); bucketid++) {
        memcpy(MHIST_BUCKET_PTR(result, bucketid), 
                MHIST_BUCKET_PTR(hist1, bucketid),
                MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist1)));
    }

    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist2); bucketid++) {
        memcpy(MHIST_BUCKET_PTR(result, 
                    bucketid + MHIST_NUM_BUCKETS_USED(hist1)),
                MHIST_BUCKET_PTR(hist2, bucketid),
                MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist1)));
    }

    MHIST_SET_NUM_BUCKETS_USED(result, num_buckets);

    /*check_mhist_integrity(result, "mhist_union() before merge");*/

    /* Fix overlapping buckets. */
    result = merge_overlapping_mhist_buckets(result);

	/* PROF_EXIT_FUNCTION("mhist_union"); */
    /*check_mhist_integrity(result, "mhist_union() result");*/

	PG_RETURN_MHIST_P(result);
}

/*
 * Function that removes a dimension of am MHIST, keeping only those bins
 * for which the bin along the indicated dimension was a particular number.
 *
 * Usage: mhist_slice(hist, dim, val)
 *
 * Where:
 *			<hist> is a histogram created with hist_in().
 *			<dim> is the name of the dimension to eliminate.
 *			<val> is the value to keep along that dimension.
 *
 * This function creates and returns a new MHIST.
 */
Datum
mhist_slice(PG_FUNCTION_ARGS) {
	/* PROF_ENTER_FUNCTION("mhist_slice"); */

	mhist		    *hist = PG_GETARG_MHIST_P(0);	
	char			*dim_name = PG_GETARG_CSTRING(1);
	int32			val = PG_GETARG_INT32(2);
	mhist		    *result = NULL;
    int             num_result_buckets = -1;
    int             bucketid = -1;

	int32			dim = -1;
	
	/* Verify the integrity of the inputs. */
    check_mhist_integrity(hist, "mhist_slice() argument");

    dim = get_dimid_for_name(hist->hdr.dimNames, 
            MHIST_NUM_DIM_NAMES(hist), dim_name);

    if (MHIST_NOT_A_DIMID == dim) {
        elog(ERROR, "mhist_slice(): Dimension '%s' of '%s' not found.",
                dim_name, mhist_to_str(hist));
    }

    /* TODO: Right now, we accept any value.  Is that okay? */

	/* Create the empty output histogram. */
	result = create_proj_mhist(hist, dim);

	/* Project the buckets and adjust their counts.  While we're at it, remove
     * buckets that don't overlap with the indicated slice. */
    num_result_buckets = 0;
    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist); bucketid++) 
	{
        int min_val = MHIST_DIMINFO_PTR(hist, bucketid, dim)->min_val;
        int max_val = MHIST_DIMINFO_PTR(hist, bucketid, dim)->max_val;

        /* Should we copy this bucket over? */
        if (val >= min_val && val <= max_val) {
            mhist_bucket *bkt_ptr = NULL;

            project_mhist_bucket(
                MHIST_BUCKET_PTR(hist, bucketid),
                MHIST_BUCKET_PTR(result, num_result_buckets),
                MHIST_NUM_DIMS(hist), dim);

            /* Adjust the count of the bucket. */
            bkt_ptr = MHIST_BUCKET_PTR(result, num_result_buckets);

            /* We're taking a slice that is one unit wide. */
            bkt_ptr->count /= (max_val - min_val + 1.0);

            num_result_buckets++;
        }
	}

    Assert(MHIST_NUM_BUCKETS_ALLOC(result) >= num_result_buckets);

    MHIST_SET_NUM_BUCKETS_USED(result, num_result_buckets);

	/* PROF_EXIT_FUNCTION("mhist_slice"); */

	PG_RETURN_MHIST_P(result);
}

/*
 * Function that performs the approximate equijoin of two relations using
 * MHISTs.
 *
 * Usage: mhist_equijoin(hist1, col1, hist2, col2)
 *
 * Where:
 *			<hist1> and <hist2> are MHISTs created with mhist_in().
 *			<col1> and <col2> are the names of columns for the MHISTs.
 */
Datum
mhist_equijoin(PG_FUNCTION_ARGS)
{
	/* PROF_ENTER_FUNCTION("mhist_equijoin"); */

	/* Function arguments. */
    mhist		    *hist1 = PG_GETARG_MHIST_P(0);	
    char*			colname1 = PG_GETARG_CSTRING(1);
    mhist		    *hist2 = PG_GETARG_MHIST_P(2);	
    char*			colname2 = PG_GETARG_CSTRING(3);	

    /* Locals */
    mhist           *xp = NULL;
    mhist		    *result = NULL;
    int             xp_dim1 = -1;
    int             xp_dim2 = -1;
    int				col1 = -1;
    int				col2 = -1;
    int             nameid = -1;

    synopsis_op_delay();

    /* Check validity of arguments. */
    check_mhist_integrity(hist1, "mhist_equijoin() hist1");
    check_mhist_integrity(hist2, "mhist_equijoin() hist2");

    col1 = get_dimid_for_name(hist1->hdr.dimNames, 
            MHIST_MAX_COL_NAMES, colname1);
    col2 = get_dimid_for_name(hist2->hdr.dimNames, 
            MHIST_MAX_COL_NAMES, colname2);

    if (MHIST_NOT_A_DIMID == col1) {
        elog(ERROR, "mhist_equijoin(): Dimension '%s' of '%s' not found.", 
                colname1, mhist_to_str(hist1));
    }
    if (MHIST_NOT_A_DIMID == col2) {
        elog(ERROR, "mhist_equijoin(): Dimension '%s' of '%s' not found.", 
                colname2, mhist_to_str(hist2));
    }

    /* Strip out empty buckets, if there are a lot of them. */
    if (mhist_approx_empty_bkts(hist1) > MHIST_NUM_BUCKETS_USED(hist1) / 2)
        remove_empty_mhist_buckets(hist1);

    if (mhist_approx_empty_bkts(hist2) > MHIST_NUM_BUCKETS_USED(hist2) / 2)
        remove_empty_mhist_buckets(hist2);

    /* First, compute the cross-product of the two histograms.  Using the
     * cross-product isn't terribly space-efficient, but it makes 
     * correctness easier. 
     *
     * TODO: Compute the join directly.
     */

    /* SPECIAL CASE: Fixed-grid histograms -- we can do the join more
     * efficiently for these guys. */
    if (MHIST_IS_FGHIST(hist1) && MHIST_IS_FGHIST(hist2)) {
        PG_RETURN_MHIST_P(fghist_equijoin(hist1, hist2, col1, col2));

        /* Unreachable */
    }
    /* END SPECIAL CASE */

    xp = mhist_cross_product(hist1, hist2);	
    /*check_mhist_integrity(xp, "mhist_equijoin() xp");*/


    /*
    elog(LOG, "mhist_equijoin(): Starting.");
    elog(LOG, "mhist_equijoin():     Left arg is (%s).",
            mhist_to_str(hist1));
    elog(LOG, "mhist_equijoin():     Right arg is (%s).",
            mhist_to_str(hist2));
    elog(LOG, "mhist_equijoin():     Cross product is (%s).",
            mhist_to_str(xp));
            */

    /* Map our dimensions into the dimensions of the cross-product histogram.
     */
    xp_dim1 = col1;
    xp_dim2 = MHIST_NUM_DIMS(hist1) + col2;

    /* Next, perform the selection. */
    mhist_equality_select_in_place(xp, xp_dim1, xp_dim2);;

    /*check_mhist_integrity(xp, "mhist_equijoin() done selection");*/

    /* Eliminate zero-size buckets. */
    remove_empty_mhist_buckets(xp);

    /*check_mhist_integrity(xp, "mhist_equijoin() removed empty");*/

    /* Finally, project the resulting histogram so we have only one copy of the
     * join column. */
    result = real_mhist_project(xp, xp_dim2);
    /*check_mhist_integrity(xp, "mhist_equijoin() result");*/

    /* The projection removed the names of the second column in the equijoin,
     * but we want the names transferred to the first column.  Fix up the names
     * array. */
    for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist2); nameid++) {
        if (MHIST_NAME_DIM(hist2, nameid) == col2) {
            int new_id = MHIST_NUM_DIM_NAMES(result);
            if (new_id >= MHIST_MAX_COL_NAMES) {
                elog(ERROR, 
                    "Too many column names (%d >= %d) creating equijoin.",
                    new_id, MHIST_MAX_COL_NAMES);
            }
            result->hdr.dimNames[new_id] = hist2->hdr.dimNames[nameid];
            MHIST_SET_NAME_DIM(result, new_id, xp_dim1);
            MHIST_SET_NUM_DIM_NAMES(result, MHIST_NUM_DIM_NAMES(result) + 1);
        }
    }


    pfree(xp);

	/* PROF_EXIT_FUNCTION("mhist_equijoin"); */

    /* TODO: Should we free hist1 and hist2? */
    PG_RETURN_MHIST_P(result);
}

/*
 * Function that performs the approximate cross-product of two relations using
 * MHISTs.
 *
 * Usage: mhist_crossprod(hist1, hist2)
 *
 * Where:
 *			<hist1> and <hist2> are MHISTs created with mhist_in().
 */
Datum
mhist_crossprod(PG_FUNCTION_ARGS)
{
	/* PROF_ENTER_FUNCTION("mhist_crossprod"); */

    /* Function arguments. */
    mhist		    *hist1 = PG_GETARG_MHIST_P(0);	
    mhist		    *hist2 = PG_GETARG_MHIST_P(1);	

    /* Locals */
    mhist           *xp = NULL;

    /* Check validity of arguments. */
    check_mhist_integrity(hist1, "mhist_crossprod() hist1");
    check_mhist_integrity(hist2, "mhist_crossprod() hist2");

    xp = mhist_cross_product(hist1, hist2);	

	/* PROF_EXIT_FUNCTION("mhist_crossprod"); */

    /* TODO: Should we free hist1 and hist2? */
    PG_RETURN_MHIST_P(xp);
}

/*
 * Function that performs the approximate equality selection on an MHIST.
 *
 * Usage: mhist_eq_selection(hist, col1, col2)
 *
 * Where:
 *			<hist> is an MHIST created with mhist_in().
 *			<col1> and <col2> are the names of columns stored inside <hist>.
 */
Datum
mhist_eq_selection(PG_FUNCTION_ARGS)
{
	/* PROF_ENTER_FUNCTION("mhist_eq_selection"); */

	/* Function arguments. */
    mhist		    *hist = PG_GETARG_MHIST_P(0);	
    char*			colname1 = PG_GETARG_CSTRING(1);
    char*			colname2 = PG_GETARG_CSTRING(2);

    /* Locals */
    mhist           *result = NULL;
    int             dim1 = -1, dim2 = -1;

    /* Check validity of arguments. */
    check_mhist_integrity(hist, "mhist_eq_selection()");

    dim1 = get_dimid_for_name(hist->hdr.dimNames, 
            MHIST_MAX_COL_NAMES, colname1);
    dim2 = get_dimid_for_name(hist->hdr.dimNames, 
            MHIST_MAX_COL_NAMES, colname2);

    if (MHIST_NOT_A_DIMID == dim1) {
        elog(ERROR, "mhist_eq_selection(): Dimension '%s' of '%s' not found.", 
                colname1, mhist_to_str(hist));
    }
    if (MHIST_NOT_A_DIMID == dim2) {
        elog(ERROR, "mhist_eq_selection(): Dimension '%s' of '%s' not found.", 
                colname2, mhist_to_str(hist));
    }

    result = create_mhist_copy(hist);  

    mhist_equality_select_in_place(result, dim1, dim2);

    /* Eliminate zero-size buckets. */
    remove_empty_mhist_buckets(result);

	/* PROF_EXIT_FUNCTION("mhist_eq_selection"); */

	PG_RETURN_MHIST_P(result);
}

/*
 * Function that performs the approximate 1-d range selection on an MHIST.
 *
 * Usage: mhist_range_selection(hist, col, min, max)
 *
 * Where:
 *			<hist> is an MHIST created with mhist_in().
 *			<col> is the name of the column to do the selection on.
 *			<min> and <max> define the range of values to select.
 *			    In particular, we select min <= val <= max
 */
Datum
mhist_range_selection(PG_FUNCTION_ARGS)
{
	/* Function arguments. */
    mhist		    *hist = PG_GETARG_MHIST_P(0);	
    char*			colname = PG_GETARG_CSTRING(1);
    int32           min = PG_GETARG_INT32(2);             
    int32           max = PG_GETARG_INT32(3);             

    /* Locals */
    mhist           *result = NULL;
    int             dim = -1;
    int             bucketid = -1;

    /* Check validity of arguments. */
    check_mhist_integrity(hist, "mhist_range_selection()");

    dim = get_dimid_for_name(hist->hdr.dimNames, 
            MHIST_MAX_COL_NAMES, colname);

    if (MHIST_NOT_A_DIMID == dim) {
        elog(ERROR, 
                "mhist_range_selection(): Dimension '%s' of '%s' not found.", 
                colname, mhist_to_str(hist));
    }

    if (max < min) {
        elog(ERROR,
                "mhist_range_selection(): Min value %d > max value %d",
                min, max);
    }

    /* TODO: Instead of creating the copy, do everything in one pass. */
    result = create_mhist_copy(hist);  

    /* Adjust bucket counts, assuming that points are uniformly distributed
     * within each bucket. */
    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(result); bucketid++) {
        mhist_bucket *bkt = MHIST_BUCKET_PTR(result, bucketid);
        int max_val = MHIST_BUCKET_DIMINFO_PTR(bkt, dim)->max_val;
        int min_val = MHIST_BUCKET_DIMINFO_PTR(bkt, dim)->min_val;
        int range = max_val - min_val + 1;
        int overlap = -1;
        double overlap_frac = -1.0;
        if ((min_val < min && max_val < min)
            || (min_val > min && min_val > max)) {
            /* Ranges don't overlap. */
            overlap = 0;
        } else {
            overlap = Min(max_val, max) - Max(min_val, min) + 1;
        }

        overlap_frac = (double)overlap / (double)range; 

        bkt->count *= overlap_frac;
    }

    /* Eliminate zero-size buckets. */
    remove_empty_mhist_buckets(result);

	PG_RETURN_MHIST_P(result);
}

/* FUNCTION mhist_can_summarize_tuple
 * ARGUMENTS: <desc> describes a type of tuple.
 * POSTCONDITIONS: Returns TRUE if the current implementation of MHISTs can
 *      summarize the indicated type of tuple.
 */
bool mhist_can_summarize_tuple(TupleDesc desc) {
    int i = -1;
    int ncols = get_num_non_timestamp_attrs(desc);

	/* PROF_ENTER_FUNCTION("mhist_can_summarize_tuple"); */

    for (i = 0; i < ncols; i++) {
        
        /* Skip the timestamp, if any. */
        int attrid = get_attr_id_skipping_ts(i + 1, desc);


        /* Is this attribute an integer? */ 
        switch (desc->attrs[attrid - 1]->atttypid) {
            case INT2OID:
            case INT4OID:
            case INT8OID:
            case CHAROID:
            case BYTEAOID:
                /* We can insert values from this index into the MHIST. */
                break;

            default:
                elog(LOG, "Can't build an MHIST on columns of type '%s'",
                        format_type_with_typemod(desc->attrs[i]->atttypid,
                            desc->attrs[i]->atttypmod));
				/* PROF_EXIT_FUNCTION("mhist_can_summarize_tuple"); */
                return false;
        }
    }

	/* PROF_EXIT_FUNCTION("mhist_can_summarize_tuple"); */

    /* Haven't found any indication that we can't summarize this tuple... */
    return true;
}

/* FUNCTION get_timestamp_col
 * ARGUMENTS: <desc> is a tuple descriptor.
 * PRECONDITIONS: <desc> represents a stream.
 * POSTCONDITIONS: Returns the attrid of the timestamp attribute for the
 *      stream. 
 */
int get_timestamp_col(TupleDesc desc) 
{
    Assert(desc != NULL);

    if (NULL == desc->constr) {
        /* No constraints, so no timestamp. */
        return TCQTSATTRNOP;
    }

    /* Someone decided to make ts_attrnum 0-based, rather than 1-based.  Rather
     * than correct that mistake, we add 1 to convert to the actual attribute
     * number. */
    /* TODO: Fix the original problem. */
    return 1 + (desc->constr->ts_attrnum);
}

/* FUNCTION get_num_non_timestamp_attrs
 * ARGUMENTS: <desc> points to a tuple descriptor.
 * POSTCONDITIONS: Returns the number of attributes of <desc> that are not
 *      timestamps.
 */
int get_num_non_timestamp_attrs(TupleDesc desc) 
{
    int ts_col = -1;
    Assert(desc != NULL);

    ts_col = get_timestamp_col(desc);

    if (ts_col != TCQTSATTRNOP) {
        /* Have a timestamp column. */
        return desc->natts - 1;
    } else {
        /* No timestamp. */
        return desc->natts;
    }

}

/* FUNCTION get_attr_id_skipping_ts
 * ARGUMENTS: <id> is an attribute number and <desc> is a tuple descriptor.
 * PRECONDITIONS: <id> is >= 1 and <= the number of non-TCQTIME attributes in
 *      <desc>.
 * POSTCONDITIONS: Returns the attribute number of the <id>th attribute that is
 *      not a timestamp.
 */
int get_attr_id_skipping_ts(int id, TupleDesc desc) 
{
    int ts_col = -1;
    Assert(desc != NULL);
    Assert(id >= 1 && id <= get_num_non_timestamp_attrs(desc));

    ts_col = get_timestamp_col(desc);

    if (ts_col != TCQTSATTRNOP && id >= ts_col) {
        return id + 1;
    } else {
        return id;
    }
}

/* FUNCTION get_int_field_val
 * ARGUMENTS: <tup> is a tuple described by <desc>, and <fnum> is a field
 *      number for <tup>.  <skip_ts> is true if you wish to skip the timestamp
 *      field of the tuple.
 * PRECONDITIONS: <fnum> is 1-based, and the indicated field is not NULL.
 * POSTCONDITIONS: Fetches the indicated field of the tuple, converts it into
 *      32-bit integer, and returns it.
 */
int64 get_int_field_val(HeapTuple tup, TupleDesc desc, int fnum, bool skip_ts) {
    Datum d;
    int64 val = -1;
    bool is_null = false;
    Oid			typoid;
    int16 typlen = -1;
    bool typbyval = false;

    Assert(tup != NULL);
    Assert(desc != NULL);

    /* We skip over the timestamp column, if desired. */
    if (skip_ts) {
        fnum = get_attr_id_skipping_ts(fnum, desc);
    }
                
    /* Fetch the value of the tuple along this dimension. */
    /* TODO: Should we be using fastgetattr()? */
    d = heap_getattr(tup, fnum, desc, &is_null);

    Assert(!is_null);

    /* There are three kinds of integer, each of which has a different
     * method of extracting from a Datum.  Figure out which one to use
     * here. */
    typoid = desc->attrs[fnum - 1]->atttypid;

    get_typlenbyval(typoid, &typlen, &typbyval);

    /*elog(LOG, "get_int_field_val(): Length of integer is %d.", typlen);*/

    switch(typlen) {
        case 1:
            /* 8-bit integer... */
            val = DatumGetUInt8(d);
            break;

        case 2:
            val = DatumGetInt16(d);
            break;

        case 4:
            val = DatumGetInt32(d);
            break;

        case 8:
            val = DatumGetInt64(d);
            break;

        default:
            elog(ERROR, "get_int_field_val(): Got invalid field length %d.",
                    typlen);
            break;
    }

    return val;
}


/* The following used to be a macro; we made it a function so that it can be
 * called from the debugger. */
extern mhist_bucket * MHIST_BUCKET_PTR(const mhist *hist, int bucketnum) {
    return
        (mhist_bucket*)(((char*)(hist)->buckets) + \
            ((bucketnum) * (MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)))));
}



/*******************************************************************************
 * INTERNAL FUNCTIONS 
 ******************************************************************************/

/* FUNCTION create_mhist_copy
 * ARGUMENTS: <hist> is an MHIST.
 * PRECONDITIONS: <hist> is properly initialized.
 * POSTCONDITIONS: Creates a copy of <hist>.
 */
static mhist * create_mhist_copy(const mhist *orig) {
    mhist *result = NULL;
    int len = MHIST_SIZE(MHIST_NUM_BUCKETS_ALLOC(orig), MHIST_NUM_DIMS(orig));

    result = palloc(len);

    /* Since there are no pointers stored in the MHIST currently, we can just
     * do a byte-by-byte copy. */
    memcpy(result, orig, len);

    check_mhist_integrity(result, "create_mhist_copy()");

    return result;
}

/* FUNCTION create_bigger_mhist
 * ARGUMENTS: <hist> is an MHIST.
 * PRECONDITIONS: <hist>'s numBucketsUsed header field is set correctly.
 * POSTCONDITIONS: Creates a copy of <hist> with some empty space for new
 *      buckets.
 */
static mhist *create_bigger_mhist(mhist *hist) {
    /* Double the size of the buckets array. */
    mhist       *ret = create_mhist(2 * MHIST_NUM_BUCKETS_ALLOC(hist), 
            MHIST_NUM_DIMS(hist));

    /* Copy information about dimension names. */
    memcpy(ret->hdr.dimNames, hist->hdr.dimNames, MHIST_NAMESZ);
    MHIST_SET_NUM_DIM_NAMES(ret, MHIST_NUM_DIM_NAMES(hist));

    /* Set the correct number of buckets used. */
    MHIST_SET_NUM_BUCKETS_USED(ret, MHIST_NUM_BUCKETS_USED(hist));

    /* Copy the buckets en masse. */
    memcpy(ret->buckets, hist->buckets, 
            MHIST_NUM_BUCKETS_USED(hist) 
            * MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)));

    return ret; 
}


/* FUNCTION real_mhist_in
 * ARGUMENTS: <relname> is the name of the relation on which to build a
 *		histogram.  <cols> and <ncols> represent which columns of the relation
 *		to build on, and <num_buckets> is the number of buckets to use.
 * PRECONDITIONS: All strings are non-NULL, and the number of buckets is
 *      greater than zero.
 * POSTCONDITIONS: Attempts to create an mhist on the indicated relation.
 *		Calls elog() on failure.
 */
static mhist * real_mhist_in(char *relname, 
		char **cols, 
		int ncols, 
		int num_buckets)
{
	/* PROF_ENTER_FUNCTION("real_mhist_in"); */
	mhist	    *result = NULL;

	/* Create the histogram data structure. */
	result = create_mhist(num_buckets, ncols);

    /* Fill in information about the different dimensions. */
    mhist_set_nameinfo(result, relname, cols, ncols);	


	/* Load the histogram. */
	load_mhist(relname, cols, result);

	/* Pass our newly-minted histogram back up the stack. */
	/* PROF_EXIT_FUNCTION("real_mhist_in"); */
	return result;
}

/* FUNCTION mhist_set_nameinfo
 * ARGUMENTS: <relname> is a relation name, and <cols> is an array of column
 * names.  <hist> is an MHIST.
 * PRECONDITIONS: The histogram is to be built on the indicated columns of the
 *      indicated relation, with dimensions in the same order as <cols>.
 * POSTCONDITIONS: Fills in the dimension name information of the histogram.
 */
static void mhist_set_nameinfo(mhist *hist, char *relname, 
                                char **cols, int ncols) {
    int         dimid = -1;

	for (dimid = 0; dimid < ncols; dimid++) {
        MHIST_SET_NAME_DIM(hist, dimid, dimid);
        snprintf(MHIST_DIM_NAME(hist, dimid), MHIST_COL_NAME_LEN, 
                "%s.%s", relname, cols[dimid]);
            /* We're assuming that the entire histogram was zeroed out;
             * otherwise, we would have to pad the rest of the name field with
             * zeros to avoid screwing up postgres hashing. */
	}

    MHIST_SET_NUM_DIM_NAMES(hist, ncols);
}



/* FUNCTION get_table_as_array
 * ARGUMENTS: <relname>, <cols>, and <ncols> are as with real_mhist_in().
 *      <ntuples> points to a location where we will put the number of tuples
 *      read.  <descdest> points to where we will put a descriptor for the 
 *      tuples in the array we return.
 * POSTCONDITIONS: Creates an array of HeapTuples of the appropriate size 
 *      and loads it with the contents of the indicated columns.  Returns 
 *      the array, and sticks a tuple descriptor into  *descdest.
 */
static HeapTuple *get_table_as_array(char *relname, char **cols, int ncols,
        int *ntuples, TupleDesc *descdest) {
	/* PROF_ENTER_FUNCTION("get_table_as_array"); */

	StringInfo	sql = makeStringInfo();
	int			dimid = -1, return_code = -1, proc = -1;
	int			tupnum = -1;

    HeapTuple   *ret = NULL;
        /* Will be returned. */

    MemoryContext main_context;
    MemoryContext oldcontext;

    Assert(ntuples != NULL);


    /* First, we snarf all the data out of the table.  At some point in the
     * future, we will probably do some sampling here, but for now, we just
     * snarf.
     *
     * In order to get at the tuples, we create a query, namely:
     *
     *      SELECT col1, col2, ... , coln FROM relation;
     */
	appendStringInfo(sql, "SELECT ");

	for (dimid = 0; dimid < ncols; dimid++) {

		appendStringInfo(sql, "%s.%s", relname, cols[dimid]);
        if (dimid < ncols - 1) {
            appendStringInfo(sql, ", ");
        }
	}
	appendStringInfo(sql, " FROM %s" , relname);

	elog(LOG, "get_table_as_array(): Query is '%s'.", sql->data);

    /* The Server Programming Interface is going to switch memory contexts
     * behind our back, so we need to remember what the original context was.
     */
    main_context = CurrentMemoryContext;

	/* Connect to the Server Programming Interface (see Postgresql Programmer's
	 * Reference, Chapter 17). */
	if ((return_code = SPI_connect()) < 0) {
		elog(ERROR, "get_table_as_array(): SPI_connect returned %d.", 
                return_code);
	}

	/* Execute the query. */
	SetQuerySnapshot();		/* Don't know why we need to do this. */
	return_code = SPI_exec(sql->data, 0);
		/* The 0 here for number of tuples to retrieve apparently means to get
		 * as many tuples as possible. */
	proc = SPI_processed;

	elog(LOG, "get_table_as_array(): Query generated %d tuples.", proc);

    /* Make a copy of the tuple descriptor.  See note above about memory
     * contexts... */
    oldcontext = MemoryContextSwitchTo(main_context);
    *descdest = CreateTupleDescCopy(SPI_tuptable->tupdesc); 
    MemoryContextSwitchTo(oldcontext);


    /* SPECIAL CASE: Empty result. */
    if (0 == proc) {
        *ntuples = 0;
        
        /* The caller should free the returned pointer unconditionally, so we
         * need to allocate _something_. */
        /* See note above about memory contexts. */
        oldcontext = MemoryContextSwitchTo(main_context);
        ret = palloc(1);
        MemoryContextSwitchTo(oldcontext);

        goto cleanup;
    }
    /* END SPECIAL CASE */

	/* Fetch the result tuples and put them into a giant array. */
    *ntuples = proc;

    /* See note above about memory contexts. */
    oldcontext = MemoryContextSwitchTo(main_context);
    ret = palloc(proc * sizeof(HeapTuple));
    MemoryContextSwitchTo(oldcontext);

	for (tupnum = 0; tupnum < proc; tupnum++) {
        HeapTuple   current_tup;

        /* See note above about memory contexts. */
        oldcontext = MemoryContextSwitchTo(main_context);
        current_tup = heap_copytuple(SPI_tuptable->vals[tupnum]);
        MemoryContextSwitchTo(oldcontext);

        ret[tupnum] = current_tup;
	}


cleanup:
	
	/* Disconnect from the Server Programming Interface. */
	if ((return_code = SPI_finish()) != SPI_OK_FINISH) {
		elog(ERROR, "get_table_as_array(): SPI_finish() returned %d.", 
                return_code);
	}

	/* PROF_EXIT_FUNCTION("get_table_as_array"); */

    return ret;
}

/* FUNCTION split_a_bucket()
 * ARGUMENTS: <giant_tup_array> is an array containing <ntuples> tuples,
 *      currently divided into (contiguous) buckets at locations given by
 *      <bucket_offsets> and <bucket_lengths>.
 * PRECONDITIONS: <giant_tup_array> has been properly divided up.
 * POSTCONDITIONS: If possible, splits one of the buckets and redistributes 
 *      tuples accordingly.  Updates the <bucket_offsets> and <bucket_lengths> 
 *      arrays.  Returns TRUE if it managed to split a bucket, FALSE 
 *      otherwise.
 */
static bool split_a_bucket(
        HeapTuple *giant_tup_array, int ntuples, 
        TupleDesc desc,
        int *bucket_offsets, int *bucket_lengths, mhist *hist,
        mhist_dim_info *ranges, int nbuckets_done) {

    /* PROF_ENTER_FUNCTION("split_a_bucket"); */

    int     split_bucketid = -1;
    int     split_dimid = -1;
    int     split_offset = -1;

    int     offset = -1, length = -1;
    HeapTuple     *bucketstart = NULL;

    mhist_bucket *maxdiff_bucket_ptr = NULL;
    mhist_bucket *new_bucket_ptr = MHIST_BUCKET_PTR(hist, nbuckets_done);
        /* The actual bucket (currently empty) */

    mhist_dim_info *maxdiff_di = NULL;
    mhist_dim_info *newbucket_di = NULL;

    int64   split_val = -1;

    int     ndims = (NULL == desc) ? 0 : get_num_non_timestamp_attrs(desc);

    Assert(0.0 == new_bucket_ptr->count);


    /* SPECIAL CASE: Can't perform any splits. */
    if (no_buckets_to_split(giant_tup_array, bucket_offsets, bucket_lengths,
                nbuckets_done, desc)) {
        return false;
    }
    /* END SPECIAL CASE */


    /* Figure out where to split. */
    get_mhist_split_point(hist, giant_tup_array, desc, bucket_offsets,
            bucket_lengths, nbuckets_done, ranges, 
            &split_bucketid, &split_dimid, &split_offset);


    /* Now we split the chosen bucket along the chosen dimension. */
    maxdiff_bucket_ptr = MHIST_BUCKET_PTR(hist, split_bucketid);
    offset = bucket_offsets[split_bucketid];
    length = bucket_lengths[split_bucketid];
    bucketstart = giant_tup_array + offset;


    /* First we sort along the dimension. */
    sort_tuples_along_dim(bucketstart, length, split_dimid, desc);



    /* The sorting has already partitioned the tuples; we just need to
     * make note of the new bucket boundary! */
    bucket_offsets[nbuckets_done] = 
            bucket_offsets[split_bucketid] + split_offset + 1;


    bucket_lengths[nbuckets_done] = 
            bucket_lengths[split_bucketid] - split_offset - 1;

    bucket_lengths[split_bucketid] = split_offset + 1;

            
    /* Adjust the bucket boundaries in the histogram data structure. */

    /* We start out by making a copy of the bucket we just split. */
    memcpy(new_bucket_ptr, maxdiff_bucket_ptr, MHIST_BUCKET_LEN(ndims));

    /* The we adjust the boundaries of the copies so that they split
     * the space along the maxdiff line. */ 
    maxdiff_di = &(maxdiff_bucket_ptr->diminfo[split_dimid]);
    newbucket_di = &(new_bucket_ptr->diminfo[split_dimid]);

    /* SPECIAL CASE: The bucket is to be split below the lowest value to
     * create an empty bucket and a full bucket. */
    if (-1 == split_offset) {
        split_val = 
                get_int_field_val(*bucketstart, desc, split_dimid + 1, true)
                    - 1;
    }
    /* END SPECIAL CASE */
    else {
        split_val = get_int_field_val(bucketstart[split_offset], desc, 
                                    split_dimid + 1, true);
    }
    
           
            
    /* By convention, the bucket contains values x, such that
     *      min_val <= x <= max_val.
     */
    maxdiff_di->max_val = split_val;
    newbucket_di->min_val = split_val + 1;

    Assert(maxdiff_di->max_val >= maxdiff_di->min_val);
    Assert(newbucket_di->max_val >= newbucket_di->min_val);



    /* We've created a hard boundary, so these two buckets should no
     * longer accept outliers in the direction of the boundary. */
    maxdiff_di->accept_above_max = false;
    newbucket_di->accept_below_min = false;

    /* PROF_EXIT_FUNCTION("split_a_bucket"); */

    return true;
}

/* Comparison function for passing to qsort.  The globals before it are used to
 * pass parameters to it on a "side channel". */
static int global_cmpf_dimid = -1;
static TupleDesc global_cmpf_desc = NULL;
static int cmpf(const void * left, const void * right) {
    HeapTuple *left_ptr = (HeapTuple*)left; 
    HeapTuple *right_ptr = (HeapTuple*)right; 
    int left_val = get_int_field_val(*left_ptr, global_cmpf_desc,
            global_cmpf_dimid + 1, true);
    int right_val = get_int_field_val(*right_ptr, global_cmpf_desc,
            global_cmpf_dimid + 1, true);

    /* We return a number less than zero if left < right */   
    if (left_val < right_val) {
        return -1;
    } else if (left_val == right_val) {
        return 0;
    } else {
        return 1;
    }
}

/* FUNCTION sort_tuples_along_dim()
 * ARGUMENTS: <tuples> points to an array of <length> tuples, each of <ndims>
 *      dimensions.  <dimid> is a dimension < <ndims>.
 * PRECONDITIONS: The specified range stays within the actual bounds of the
 *      array.
 * POSTCONDITIONS: Sorts the range of tuples along dimension <dimid>.
 */
static void sort_tuples_along_dim(HeapTuple *tuples, int length, int dimid,
        TupleDesc desc) {

    /* PROF_ENTER_FUNCTION("sort_tuples_along_dim"); */

    /* Tell our comparison function which dimension to pay attention to. */
    global_cmpf_dimid = dimid;
    global_cmpf_desc = desc;

    /* Then we can let qsort do the heavy lifting... */
    qsort(tuples, length, sizeof(*tuples), &cmpf);

    /* PROF_EXIT_FUNCTION("sort_tuples_along_dim"); */
}



/* FUNCTION load_mhist
 * ARGUMENTS: <relname> and <cols> are as with real_mhist_in(), and
 *		<hist> is an appropriately-sized and initialized MHIST data
 *		structure.
 * PRECONDITIONS: The histogram is initialized and all buckets zeroed out.
 * POSTCONDITIONS: Loads the histogram from the indicated table.  Calls elog()
 *		on error.
 */
static void load_mhist(char *relname, char **cols, mhist *hist) 
{
	/* PROF_ENTER_FUNCTION("load_mhist"); */
    HeapTuple   *giant_tup_array = NULL;
    int         ncols = hist->hdr.numDims;
    int         ntuples = -1;
    TupleDesc   desc = NULL;

    elog(LOG, "load_mhist(): Fetching tuples.");

    /* We start out by fetching all the tuples into an array. */
    /* TODO: Collect a sample instead. */
    giant_tup_array = get_table_as_array(relname, cols, ncols, &ntuples, 
            &desc);

    /* Then we bucketize them to form a histogram. */
    mhist_create_buckets(hist, giant_tup_array, ntuples, desc);

    /* Now we've finished creating buckets, so we can calculate their counts
     * and numbers of unique values. */
    mhist_load_buckets(giant_tup_array, desc, ntuples, hist);

    elog(LOG, "load_mhist(): Cleaning up.");

    /* TODO: Shoud we be freeing the individual tuples? */
    /*elog(LOG, "load_mhist(): Freeing giant tuple array.");*/

    pfree(giant_tup_array);

	/* PROF_EXIT_FUNCTION("load_mhist"); */
}

/* FUNCTION create_proj_mhist
 * ARGUMENTS: <original> is an MHIST, and <dim> is a dimension to be
 *		projected out of <original>.
 * PRECONDITIONS: <original> is fully initialized, and <dim> is a valid
 *		dimension.
 * POSTCONDITIONS: Creates and returns a new (empty) MHIST with all the 
 *		dimensions of <original> except for dimension <dim>.  This empty mhist
 *		has enough space to store all the buckets in <original>, but contains
 *		no data.
 */
mhist * create_proj_mhist(const mhist * original, int dim) {
	mhist * result = NULL;
	int result_num_dims = -1;
	int result_nameid = -1;
	int nameid = -1;

	Assert(original != NULL);
	Assert(dim >= 0 && dim < MHIST_NUM_DIMS(original));

	/* Create the output histogram.  Give it enough space to hold the buckets
     * of the input histogram. */
	result_num_dims = MHIST_NUM_DIMS(original) - 1;

	result = create_mhist(MHIST_NUM_BUCKETS_USED(original), result_num_dims);


	/* Copy the dimension names for the dimensions we're keeping. */
	result_nameid = 0;
	for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(original); nameid++)
    {
        if (MHIST_NAME_DIM(original, nameid) != dim) {
            elog(DEBUG1, "create_proj_mhist(): Copying name '%s'",
                MHIST_DIM_NAME(original, nameid));

            MHIST_DIM_NAME_INFO(result, result_nameid)
                = MHIST_DIM_NAME_INFO(original, nameid);
            /*
			result->hdr.dimNames[result_nameid] = 
                    original->hdr.dimNames[nameid];
                    */

            /* Dimensions above the one we're removing need to be moved down by
             * one. */
            if (MHIST_NAME_DIM(original, nameid) > dim) {
                mhist_dim_name *name = 
                    &(MHIST_DIM_NAME_INFO(result, result_nameid));
                name->dim_no--;
            }

			++result_nameid;
		} else {
            elog(DEBUG1, "create_proj_mhist(): NOT copying name '%s'",
                MHIST_DIM_NAME(original, nameid));
        }
	}

    MHIST_SET_NUM_DIM_NAMES(result, result_nameid);

    /* Make a note of the fact that this histogram is empty. */
    MHIST_SET_NUM_BUCKETS_USED(result, 0); 



	return result;
}


/* FUNCTION srf_firstcall_junk
 * ARGUMENTS: <state> is an object that will hold the state of your function
 *      across invocations, and <max_calls> is the number of times the function
 *      will be called.  <tupdesc> describes the type of tuples that your
 *      function will return.
 * PRECONDITIONS: You are calling this from the first invocation of a
 *      set-returning function.  <funcctx> was allocated by the
 *      SRF_FIRSTCALL_INIT() macro. You've already set up *state.
 * POSTCONDITIONS: Does the obligatory setup stuff that you always for an SRF.
 */
void srf_firstcall_junk(FuncCallContext *funcctx, void *state, int max_calls,
        char *typename) {
	MemoryContext		    oldcontext; 
	TupleTableSlot		    *slot = NULL;
	AttInMetadata		    *attinmeta = NULL;
	TupleDesc			    tupdesc;

    /* Switch to a memory context appropriate for multiple function calls.
     */
    oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx); 


    /* Fill in the "official" function context object. */
    funcctx->max_calls = max_calls;
    funcctx->user_fctx = (void*)(state);
        
    /* Generate attribute metadata needed later to produce tuples from 
     * raw C strings */ 
    tupdesc = RelationNameGetTupleDesc(typename);	
    attinmeta = TupleDescGetAttInMetadata(tupdesc); 
    funcctx->attinmeta = attinmeta;

    /* allocate a slot for a tuple with this tupdesc */ 
    slot = TupleDescGetSlot(tupdesc);
    funcctx->slot = slot; 

    MemoryContextSwitchTo(oldcontext); 

}

/* FUNCTION srf_cleanup
 * ARGUMENTS: <funcctx> is a used function context from a set-returning
 *      function.
 * PRECONDITIONS: You're done with the function's using the indicated context.
 * POSTCONDITIONS: Cleans up the context.  You should call the
 *      SRF_RETURN_DONE() macro after calling this function.
 */
void srf_cleanup(FuncCallContext *funcctx) {
    MemoryContext oldcontext 
        = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx); 
    /*elog(LOG, "srf_cleanup(): Freeing user function context.");*/
    pfree(funcctx->user_fctx);
    funcctx->user_fctx = NULL;
    /*elog(LOG, "srf_cleanup(): Switching back to old memory context.");*/
    MemoryContextSwitchTo(oldcontext); 
}

/* FUNCTION get_ranges
 * ARGUMENTS: <giant_tup_array> is an array of tuples, each of which has
 *      <ndims> fields.
 * PRECONDITIONS: Returns an array of <mhist_dim_info> with the min_val and
 *      max_val fields filled in.
 * POSTCONDITIONS: The returned array must be freed by the caller. 
 */
mhist_dim_info * get_ranges(HeapTuple *giant_tup_array, int ntuples, 
        TupleDesc desc, int ndims) 
{
    int dimid = -1;
    int tupid = -1;
    mhist_dim_info *ret = NULL;
        /* Freed by caller. */

    ret = palloc(ndims * sizeof(*ret));

    memset(ret, 0x0, (ndims * sizeof(*ret)));

#ifdef DEMO_HACK
    /* TEMPORARY DEMO HACK!!! */
    if (1 == ndims)
    {
        for (dimid = 0; dimid < ndims; dimid++) {
            ret[dimid].min_val = 0;
            ret[dimid].max_val = 10511;
        }
        return ret;
    } 
    /* END HACK */
#endif 

    /* SPECIAL CASE: No tuples. */
    if (0 == ntuples) {
        for (dimid = 0; dimid < ndims; dimid++) {
            ret[dimid].min_val = ret[dimid].max_val = 0;
        }
        return ret;
    }
    /* END SPECIAL CASE */

    Assert(desc != NULL);

    
    /* Initialize the data structure. */
    for (dimid = 0; dimid < ndims; dimid++) {
        ret[dimid].min_val = ret[dimid].max_val = 
            get_int_field_val(giant_tup_array[0], desc, dimid+1, true);
    }

    elog(DEBUG1, "get_ranges(): Calculating ranges.");
    for (dimid = 0; dimid < ndims; dimid++) {
        for (tupid = 0; tupid < ntuples; tupid++) {
            int val = get_int_field_val(giant_tup_array[tupid], desc, 
                     dimid+1 /* Attribute IDs are 1-based... */, true);

            if (val > ret[dimid].max_val) {
                ret[dimid].max_val = val;
            }
            if (val < ret[dimid].min_val) {
                ret[dimid].min_val = val;
            }
        }

    }

    return ret;
}


/* FUNCTION get_maxdiffs
 * ARGUMENTS: <giant_tup_array> is an array of tuples,
 *      <desc> is a descriptor for the tuples in <giant_tup_array>,
 *      <bucket_offsets> and <bucket_lengths> describe the current
 *      bucketization of the array, 
 *      <nbuckets_done> is the number of buckets we've created thus far,
 *      <ranges> describes the maximum and minimum values along each dimension
 *      of the tuples. 
 * PRECONDITIONS: <giant_tup_array> is divided into buckets as described in the
 *      <bucket_offsets> and <bucket_lengths> arrays.
 * POSTCONDITIONS: Finds the maximum difference between any two adjacent tuples
 *      along any dimension, for each bucket.  Returns an array of structs, one
 *      per bucket, containing that information.
 *
 *      If there is empty space at the edge of a bucket, chooses the edge of
 *      that empty space as the split point.
 */
bucket_maxdiff_info *get_maxdiffs(
        mhist *hist,
        HeapTuple *giant_tup_array, 
        TupleDesc desc, 
        const int *bucket_offsets,
        const int *bucket_lengths, 
        int nbuckets_done, 
        const mhist_dim_info *ranges) {
    int bucketid = -1;
    int dimid = -1;
    int tupid = -1; 
    int ndims = get_num_non_timestamp_attrs(desc);

    /* Create the result array. */
    bucket_maxdiff_info *result = palloc(nbuckets_done 
            * sizeof(bucket_maxdiff_info));

    /*
     * To find the max. difference, we sort each bucket along each dimension
     * and check the differences between adjacent tuples.
     */
    for (bucketid = 0; bucketid < nbuckets_done; bucketid++) {
        /* Initialize our counters. */
        bucket_maxdiff_info *current = &(result[bucketid]);
        current->abs = -1;
        current->dimid = -1;
        current->norm = -1.0;
        current->empty_space_upper = false;
        current->empty_space_lower = false;

        for (dimid = 0; 
                dimid < ndims 
                    && !(current->empty_space_lower 
                            || current->empty_space_upper); 
                dimid++) 
        {
            int offset = bucket_offsets[bucketid];
            int length = bucket_lengths[bucketid];
            HeapTuple *bucketstart = giant_tup_array + offset;

            mhist_dim_info *di = MHIST_DIMINFO_PTR(hist, bucketid, dimid);

            sort_tuples_along_dim(bucketstart, length, dimid, desc);

            /* SPECIAL CASE: If there is empty space at the edge of the bucket
             * along this dimension, we want to split along this dimension. */
            if (length > 0 
                    && di->min_val < get_int_field_val(bucketstart[0],
                                            desc, dimid+1, true))
            { 
                current->dimid = dimid;
                current->empty_space_lower = true;
            } else if (length > 0 
                    && di->max_val > get_int_field_val(bucketstart[length-1],
                                            desc, dimid+1, true))
            {
                current->dimid = dimid;
                current->empty_space_upper = true;
            } 
            /* END SPECIAL CASE */
            else {
                for (tupid = 0; tupid < (length - 1); tupid++) {
                    int this_val = get_int_field_val(bucketstart[tupid], desc,
                                                    dimid+1, true);
                    int next_val = get_int_field_val(bucketstart[tupid+1], desc,
                                                    dimid+1, true);
                    int diff = next_val - this_val;
                    double norm_diff = (double)diff / (double)
                        (ranges[dimid].max_val - ranges[dimid].min_val);

                    if ( (norm_diff > current->norm) || (current->abs == -1)) {
                        current->abs = diff;
                        current->norm = norm_diff;
                        current->dimid = dimid;
                    }
                }
            }
        }
    }

    return result;
}

/* FUNCTION get_mhist_bucket_for_tuple
 * ARGUMENTS: <hist> is an MHIST data structure, and <tuple> is a tuple to be
 *      put into the histogram.  <desc> describes how to get at the attributes
 *      of <tuple>.
 * PRECONDITIONS: <hist> has its bucket boundaries properly initialized, and
 *      <tuple> is of the appropriate length.
 * POSTCONDITIONS: Returns the index of the bucket in which the tuple belongs.
 */
int get_mhist_bucket_for_tuple(mhist *hist, int *tuple, size_t nfields) {
    int bucketid = -1;
    int proper_bucketid = -1;

    /* SPECIAL CASE: Fixed-grid histogram; we can compute the bucket index
     * directly. */
    if (MHIST_IS_FGHIST(hist)) {
        /* Buckets are arranged in an n-dimensional dense matrix; first all the
         * buckets for the first bin of the first dimension, etc. */
        /* We find the range of buckets that match on the first dimension, then
         * recurse on the remaining dimensions. */
        int start_ix = 0;
        int nbuckets = MHIST_NUM_BUCKETS_USED(hist);
        int range_size = nbuckets;
        int dimid = -1;

        for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {

            /* Get our value along this dimension. */
            int val = tuple[dimid];

            /* Use the size of the first bucket to compute how many bins
             * we divide this dimension into */
            /*
            mhist_dim_info *first_di = MHIST_DIMINFO_PTR(hist, 0, dimid);
            mhist_dim_info *last_di = 
                MHIST_DIMINFO_PTR(hist, nbuckets-1, dimid);
                */


            /*
            int num_bins = (last_di->max_val - first_di->min_val + 1)
                            / (first_di->max_val - first_di->min_val + 1);
                            */
            int num_bins = fghist_num_bins_along_dimension(hist, dimid);
                /* Plus 1 is because of our funky integer bucket boundaries. */
            int bin_size = range_size / num_bins;

            /* Calculate the bin number for this tuple. */
            /*
            int bin_no = (val - first_di->min_val) 
                / (first_di->max_val - first_di->min_val + 1);
                */
            int bin_no = fghist_binid_along_dimension(hist, dimid, val);

            /* bin_no may overflow if the value is outside the range of buckets
             * in the histogram. */
            if (bin_no < 0) bin_no = 0;
            if (bin_no >= num_bins) bin_no = num_bins - 1;

            /* Adjust our range accordingly. */
            start_ix = start_ix + (bin_size * bin_no);
            range_size = bin_size;

            Assert(start_ix < nbuckets);
        }

        Assert(1 == range_size);
        
        return start_ix;
    }
    /* END SPECIAL CASE */

    /* We do linear search for now. */
    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist); bucketid++) {
        bool fits_this_bucket = true;
            /* We'll assume the tuple fits in this bucket until it's proven
             * otherwise. */
        int dimid = -1;

        /* Check to see whether the tuple is out of range for this bucket along
         * any dimension. */
        for (dimid = 0; 
                dimid < MHIST_NUM_DIMS(hist) && (true == fits_this_bucket); 
                dimid++) {
            mhist_dim_info *cur_di = MHIST_DIMINFO_PTR(hist, bucketid, dimid);
            int val = tuple[dimid];

            /* By convention, the bucket contains values x, such that
             *      min_val <= x <= max_val.
             */
            bool is_out_of_range = 
                (val > cur_di->max_val && (false == cur_di->accept_above_max))
                ||
                (val < cur_di->min_val && (false == cur_di->accept_below_min));

            if (is_out_of_range) {
                fits_this_bucket = false;
            }
        }

        if (true == fits_this_bucket) {
            /* A tuple shouldn't fit into more than one bucket! */
            Assert(-1 == proper_bucketid);
            proper_bucketid = bucketid;
        }
    }

    return proper_bucketid;
}

/* The actual implementation of mhist_project().  Separated out so that we can
 * use it as a subroutine for other functions. */
mhist * real_mhist_project(mhist *inhist, int32 proj_dim) {
	mhist		    *outhist = NULL;
    int             bucketid = -1;

    /*
    mhist_bucket    *in_bkt_ptr = NULL;
    mhist_bucket    *out_bkt_ptr = NULL;
    */
	
	/* Verify the integrity of the inputs. */
    check_mhist_integrity(inhist, "mhist_project() input");

	if (proj_dim < 0 || proj_dim >= MHIST_NUM_DIMS(inhist)) {
		elog(ERROR,
				"mhist_project(): Invalid dimension ID %d.", proj_dim);
	}

    /* First, we create a new histogram of with the same number of buckets as
     * the old one but one less dimension; we will increase the size of this 
     * histogram as needed. */
    outhist = create_proj_mhist(inhist, proj_dim);


    /* Project all the buckets of the input histogram along the dimension we're
     * cutting out, and copy them to the new histogram. */
    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(inhist); bucketid++) {
        project_mhist_bucket(
                MHIST_BUCKET_PTR(inhist, bucketid),
                MHIST_BUCKET_PTR(outhist, bucketid),
                MHIST_NUM_DIMS(inhist), proj_dim);

        MHIST_SET_NUM_BUCKETS_USED(outhist, MHIST_NUM_BUCKETS_USED(outhist) + 1);
    }


    elog(DEBUG1, "mhist_project(): Before merge: %d buckets.",
            MHIST_NUM_BUCKETS_USED(outhist));


    /* Iteratively merge the buckets of the project histogram until none of
     * them overlap. */
    outhist = merge_overlapping_mhist_buckets(outhist);

    elog(DEBUG1, "mhist_project(): After merge: %d buckets.",
            MHIST_NUM_BUCKETS_USED(outhist));

    return outhist;
}


/* FUNCTION project_mhist_bucket
 * ARGUMENTS: <src> is a bucket with <ndims> dimensions, and <dest> is a bucket
 *      with <ndims-1> dimensions.  <proj_dim> is a dimension along which to
 *      project.
 * PRECONDITIONS: <proj_dim> < <ndims>.
 * POSTCONDITIONS: Copies <src> to <dest>, leaving out dimension <proj_dim>
 */
void project_mhist_bucket(mhist_bucket *src, mhist_bucket *dest, 
        int ndims, int proj_dim)
{
    int dimid = -1;
    Assert(ndims > 0);
    Assert(proj_dim >= 0 && proj_dim < ndims);

    /* First copy over the bounds. */
    for (dimid = 0; dimid < ndims; dimid++) {
        if (dimid < proj_dim) {
            dest->diminfo[dimid] = src->diminfo[dimid];
        } else if (dimid > proj_dim) {
            /* Dimensions over proj_dim are offset by one in the destination.
             */
            dest->diminfo[dimid-1] = src->diminfo[dimid];
        }
    }
    
    /* The copy over the contents. */
    dest->count = src->count;
}

/* Comparison function for passing to qsort.  
 *
 * This particular function is used for sorting the buckets of an MHIST along 
 * all dimensions, in order.
 *
 * Parameters:
 *      global_mhist_1dcmpf_ndims tells the function which dimension to 
 *              sort the buckets on.
 *
 * Arguments should be pointers to histogram buckets.
 */
static int global_mhist_bucket_cmpf_ndims = -1;
static int mhist_bucket_cmpf(const void * left, const void * right) {
    mhist_bucket *left_bucket = (mhist_bucket*)left;
    mhist_bucket *right_bucket = (mhist_bucket*)right;
    int dimid = -1;

    for (dimid = 0; dimid < global_mhist_bucket_cmpf_ndims; dimid++) {

        mhist_dim_info *left_di = 
            MHIST_BUCKET_DIMINFO_PTR(left_bucket, dimid);
        mhist_dim_info *right_di = 
            MHIST_BUCKET_DIMINFO_PTR(right_bucket, dimid);

        /* We return a number less than zero if left < right */   
        /* Compare along the lower edge first, and use the upper edge to break
         * ties. */
        if (left_di->min_val < right_di->min_val) {
            return -1;
        } else if (left_di->min_val > right_di->min_val) {
            return 1;
        } else {
            /* min_val's are equal, so compare the max_val's. */
            if (left_di->max_val < right_di->max_val) {
                return -1;
            } else if (left_di->max_val > right_di->max_val) {
                return 1;
            } else {
                /* Tie on both lower and upper values -- go to the next 
                 * dimension. */
            }
        }
    }

    /* The buckets matched along every dimension. */
    return 0;
}


/* FUNCTION remove_empty_mhist_buckets
 * ARGUMENTS: <hist> is an MHIST
 * PRECONDITIONS: <hist> is properly initialized.
 * POSTCONDITIONS: Removes any empty buckets from <hist> (e.g. count == 0),
 *      and truncates the histogram accordingly.
 */
void remove_empty_mhist_buckets(mhist *hist) {
    int         dest_bucketid = -1;
    int         src_bucketid = -1;
    int         num_buckets_removed = -1;
    
    check_mhist_integrity(hist, "remove_empty_mhist_buckets()");

    /* Copy buckets from src_bucketid to dest_bucketid */
    num_buckets_removed = 0;
    dest_bucketid = 0;
    for (src_bucketid = 0; src_bucketid < MHIST_NUM_BUCKETS_USED(hist); 
            src_bucketid++) {
        mhist_bucket *src_bkt_ptr = MHIST_BUCKET_PTR(hist, src_bucketid);
        mhist_bucket *dest_bkt_ptr = MHIST_BUCKET_PTR(hist, dest_bucketid);

        
        if (0.0 != src_bkt_ptr->count) {
            /* Non-empty bucket; copy it over. */
            memcpy(dest_bkt_ptr, src_bkt_ptr, 
                    MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)));

            dest_bucketid++;
             
        } else {
            /* Empty bucket.  Don't bother copying it. */
            num_buckets_removed++; 
        }
    }


    /* Update the number of buckets used. */
    MHIST_SET_NUM_BUCKETS_USED(hist, 
            MHIST_NUM_BUCKETS_USED(hist) - num_buckets_removed);

    /* Zero out the newly-empty buckets. */
    memset(MHIST_BUCKET_PTR(hist, MHIST_NUM_BUCKETS_USED(hist)),
            0x0, 
            (MHIST_NUM_BUCKETS_ALLOC(hist) - MHIST_NUM_BUCKETS_USED(hist)) 
                * MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)));

    /* Truncate the histogram.  Note that this doesn't actually pfree() the
     * extra memory the histogram was using up. */
    hist->hdr.numBucketsAllocated = MHIST_NUM_BUCKETS_USED(hist);
    hist->hdr.length = MHIST_SIZE(MHIST_NUM_BUCKETS_USED(hist),
            MHIST_NUM_DIMS(hist));

    /* Now that we've removed buckets, this is no longer a full FGHist */
    MHIST_SET_IS_FGHIST(hist,false);
}


/* FUNCTION merge_overlapping_mhist_buckets
 * ARGUMENTS: <hist> is an MHIST with possibly-overlapping buckets.
 *      <found_overlap> points to a local Boolean variable.
 * PRECONDITIONS: <hist> is properly initialized; in particular, its
 *      numBucketsUsed and numBucketsAllocated header fields are correctly set.
 * POSTCONDITIONS: Finds and merges overlapping buckets in <hist>, and returns
 *      a pointer to a new histogram with those overlapping buckets replaced.  
 *      Resizes the histogram as needed, freeing old copies.
 */
mhist *merge_overlapping_mhist_buckets(mhist *hist) {
    bool found_overlap = false;
    int num_merges = 0;
    const int MAX_NUM_MERGES = 1000;
    const int WORRYSOME_NUM_MERGES = 10;

    do {
        num_merges++;
        
        /* Start printing debug messages when we get into large numbers of
         * merges. */
        if (num_merges > WORRYSOME_NUM_MERGES) {
            elog(DEBUG1, 
                    "merge_overlapping_mhist_buckets(): Before merge %d: %s", 
                    num_merges, mhist_to_str(hist));
            hist = really_merge_overlapping_mhist_buckets(hist, 
                    &found_overlap, true); 
        } else {
            hist = really_merge_overlapping_mhist_buckets(hist, 
                    &found_overlap, false); 
        }

    } while (found_overlap && num_merges < MAX_NUM_MERGES);
        /* The condition on num_merges is to prevent runaways. */

    if (num_merges >= MAX_NUM_MERGES) {
        elog(ERROR, 
                "merge_overlapping_mhist_buckets(): Merged too many times.");
    }

    return hist;
}

mhist *really_merge_overlapping_mhist_buckets(mhist *hist, bool *found_overlap,
    bool verbose) 
{
    int             left_bucketid = -1;
    int             right_bucketid = -1;
    mhist_bucket    *left_bucket = NULL;
    mhist_bucket    *right_bucket = NULL;
    int             orig_num_buckets = MHIST_NUM_BUCKETS_USED(hist); 
    int             num_dims = MHIST_NUM_DIMS(hist);
    
    *found_overlap = false;

    /* Sort along all dimensions to find buckets that could possibly overlap.
     */
    global_mhist_bucket_cmpf_ndims = num_dims;
        /* Defined above. */
    qsort(hist->buckets, orig_num_buckets, MHIST_BUCKET_LEN(num_dims), 
            &mhist_bucket_cmpf);

    /* Scan through the array, finding pairs of overlapping buckets. 
     * 
     * When we find a pair of buckets that overlap, merge the buckets.  The
     * merging process creates new buckets, which we stick onto the end of the
     * buckets array, resizing it as necessary. */
    for (left_bucketid = 0; left_bucketid < orig_num_buckets - 1
            && left_bucketid < MHIST_NUM_BUCKETS_USED(hist) - 1; 
            left_bucketid++) 
    {
        left_bucket = MHIST_BUCKET_PTR(hist, left_bucketid);

        /* Starting from the current "leftmost" bucket, check the buckets after
         * it that could possibly overlap with it. */
        right_bucketid = left_bucketid + 1;
        right_bucket = MHIST_BUCKET_PTR(hist, right_bucketid);

        /* Could left_bucket and right_bucket overlap?  Take advantage of the
         * buckets being sorted to prune the number of righthand buckets we
         * check. */
        while (right_bucketid < orig_num_buckets
                && right_bucketid < MHIST_NUM_BUCKETS_USED(hist)
                && potential_overlap(left_bucket, right_bucket, num_dims))
        {
            bool buckets_overlap = true;
            int dimid = -1;

            /* Check to see whether the buckets really overlap. */
            buckets_overlap = true;
            for (dimid = 0; dimid < num_dims && buckets_overlap; dimid++) {
                mhist_dim_info  *left_di = 
                    MHIST_BUCKET_DIMINFO_PTR(left_bucket, dimid);
                mhist_dim_info  *right_di = 
                    MHIST_BUCKET_DIMINFO_PTR(right_bucket, dimid);

                /* Note the use of ">" and "<" instead of ">=" and "<=" here.
                 * The current convention is to use integer bucket boundaries,
                 * with bucket contents being _inclusive_ of the boundaries. */
                if (right_di->min_val > left_di->max_val
                        || right_di->max_val < left_di->min_val) {
                    buckets_overlap = false;
                }
            }

            if (buckets_overlap) {
                *found_overlap = true;

                /* Generate debugging output, if requested. */
                if (verbose) {
                    int dimid = -1;
                    elog(LOG, "m_o_h_b(): Merging buckets %d and %d.", 
                            left_bucketid, right_bucketid);

                    elog(LOG, "m_o_h_b():     Bucket %d:", left_bucketid);

                    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
                        mhist_dim_info  *left_di = 
                            MHIST_BUCKET_DIMINFO_PTR(left_bucket, dimid);
                        elog(LOG, "m_o_h_b():         Dim %d: %d to %d",
                                dimid, left_di->min_val, left_di->max_val);
                         
                    }

                    elog(LOG, "m_o_h_b():     Bucket %d:", right_bucketid);

                    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
                        mhist_dim_info  *right_di = 
                            MHIST_BUCKET_DIMINFO_PTR(right_bucket, dimid);
                        elog(LOG, "m_o_h_b():         Dim %d: %d to %d",
                                dimid, right_di->min_val, right_di->max_val);
                         
                    }
                }

                /* The buckets overlap on every dimension, so we need to merge
                 * them. */
                hist = merge_two_overlapping_buckets(hist, left_bucketid,
                        right_bucketid, verbose); 
                    /* merge_two_overlapping_buckets() may resize hist and
                     * return a larger copy. */
            } 
            
            /* Update our pointers into hist, in case the histogram was 
             * resized. */
            left_bucket = MHIST_BUCKET_PTR(hist, left_bucketid);


            /* Move onto the next bucket. */
            right_bucketid++;
            right_bucket = MHIST_BUCKET_PTR(hist, right_bucketid);
        }
    }

    return hist;
}

/* FUNCTION merge_two_overlapping_buckets
 * ARGUMENTS: <hist> is an MHIST, and <left_bucketid> and <right_bucketid> are
 *      buckets in the histogram.
 * PRECONDITIONS: <hist>'s numBucketsUsed parameter is set correctly.
 *      <left_bucketid> is less than <right_bucketid>, and the lefthand bucket
 *      is to the left of the righthand bucket along dimension 0.  Furthermore,
 *      the lefthand bucket overlaps with the righthand bucket along all
 *      dimensions.
 * POSTCONDITIONS: Merges the two buckets in place, while keeping the "old"
 *      part of the array of buckets sorted along dimension 0.  Adds new
 *      buckets to the end of the array, resizing the histogram as needed.
 *      Returns a pointer to the (possibly resized) histogram; frees old copies
 *      of the histogram as needed.
 */
mhist *merge_two_overlapping_buckets(mhist *hist, 
        int left_bucketid, int right_bucketid, bool verbose) {
    int             ndims = MHIST_NUM_DIMS(hist);
    int             dimid = -1;
    mhist_bucket    *left_bucket = MHIST_BUCKET_PTR(hist, left_bucketid);
    mhist_bucket    *right_bucket = 
        MHIST_BUCKET_PTR(hist, right_bucketid);
    mhist_bucket    *last_bucket =
        MHIST_BUCKET_PTR(hist, MHIST_NUM_BUCKETS_USED(hist) - 1);
    mhist_bucket    *new_bucket = NULL;
    bool            buckets_overlap_completely = false;
    int             merge_dimid = -1;

    /* Do the buckets overlap exactly along every dimension? */
    buckets_overlap_completely = true;
    for (dimid = 0; dimid < ndims && buckets_overlap_completely; dimid++) {
        mhist_dim_info  *left_di = 
                    MHIST_BUCKET_DIMINFO_PTR(left_bucket, dimid);
        mhist_dim_info  *right_di = 
                    MHIST_BUCKET_DIMINFO_PTR(right_bucket, dimid);
        if (left_di->min_val != right_di->min_val
                || left_di->max_val != right_di->max_val) {
            buckets_overlap_completely = false;
            merge_dimid = dimid;
                /* We will merge the buckets along this dimension. */
        }
    }

    if (buckets_overlap_completely) {

        /* If the buckets overlap completely, merge them and copy someone else 
         * into the place of the right-hand bucket. */
        left_bucket->count += right_bucket->count; 
        memcpy(right_bucket, last_bucket, MHIST_BUCKET_LEN(ndims));
        memset(last_bucket, 0x0, MHIST_BUCKET_LEN(ndims));

        if (verbose) {
            elog(DEBUG1, 
                "merge_two_overlapping_buckets(): Buckets overlap completely.");
            elog(DEBUG1, 
                "merge_two_overlapping_buckets(): Shrank size from %d to %d.",
                MHIST_NUM_BUCKETS_USED(hist), MHIST_NUM_BUCKETS_USED(hist) - 1);
        } 

        MHIST_SET_NUM_BUCKETS_USED(hist, MHIST_NUM_BUCKETS_USED(hist) - 1);
    } else {
        mhist_dim_info  *left_di = NULL;
        mhist_dim_info  *right_di = NULL;
        mhist_bucket *bucket_to_split = NULL;
        int val_to_split_at = -1;
       

        /* If the buckets don't overlap completely, take the lowest dimension 
         * along which the buckets partially overlap and split them along 
         * that dimension. */


        /* Resize the buckets array, if necessary, to make space for the new
         * bucket we're going to add. */
        if (MHIST_NUM_BUCKETS_USED(hist) == MHIST_NUM_BUCKETS_ALLOC(hist)) {
            mhist *newhist = create_bigger_mhist(hist);
            pfree(hist);
            hist = newhist;
        }

        /* Refresh our pointers, in case the data structure was wiped out. */
        left_bucket = MHIST_BUCKET_PTR(hist, left_bucketid);
        right_bucket = MHIST_BUCKET_PTR(hist, right_bucketid);
 
        left_di = MHIST_BUCKET_DIMINFO_PTR(left_bucket, merge_dimid);
        right_di = MHIST_BUCKET_DIMINFO_PTR(right_bucket, merge_dimid);
            /* merge_dimid was set in the previous for loop. */


        if (verbose) {
            elog(LOG, 
                "merge_two_overlapping_buckets(): "
                "Buckets don't completely overlap.");
            elog(LOG, 
                "merge_two_overlapping_buckets(): Merge dimension is %d",
                merge_dimid);
            elog(LOG, 
                "merge_two_overlapping_buckets():     Left: %d to %d",
                left_di->min_val, left_di->max_val);
            elog(LOG, 
                "merge_two_overlapping_buckets():     Right: %d to %d",
                right_di->min_val, right_di->max_val);
        } 
 
        /* First figure out which bucket to split. */
        if (left_di->min_val < right_di->min_val) {
            
            /* Split the left bucket along the min. value of the right bucket.
             */
            bucket_to_split = left_bucket;
            val_to_split_at = right_di->min_val - 1;
        } else if (left_di->min_val > right_di->min_val) {

            /* Split the right bucket along the min. value of the left bucket.
             */
            bucket_to_split = right_bucket;
            val_to_split_at = left_di->min_val - 1;

        } else {
            /* The lower bounds of the buckets line up, so cut the top off of
             * whichever bucket has the higher maximum value. */
            if (left_di->max_val > right_di->max_val) {
                bucket_to_split = left_bucket;
                val_to_split_at = right_di->max_val;
            } else if (left_di->max_val < right_di->max_val) {
                bucket_to_split = right_bucket;
                val_to_split_at = left_di->max_val;
            } else {
                elog(ERROR, "Something has gone horribly wrong.");
            }
        }


        /* Actually perform the split. */
        new_bucket = MHIST_BUCKET_PTR(hist, MHIST_NUM_BUCKETS_USED(hist));
        memcpy(new_bucket, bucket_to_split, 
                MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)));
        cut_top_off_mhist_bucket(bucket_to_split, merge_dimid, 
            val_to_split_at);

        /* Bottom of range is >=, so add 1 */
        cut_bottom_off_mhist_bucket(new_bucket, merge_dimid, 
                val_to_split_at + 1); 

        MHIST_SET_NUM_BUCKETS_USED(hist, 1 + MHIST_NUM_BUCKETS_USED(hist));
    }

    return hist;
}

/* FUNCTION cut_top_off_mhist_bucket
 * ARGUMENTS: <bucket_ptr> points to an MHIST bucket, <dimid> is a dimension of
 *      the bucket, and <new_max> is a new max. value for the bucket along 
 *      that dimension.
 * POSTCONDITIONS: Removes the indicated fraction of the bucket.
 */
void cut_top_off_mhist_bucket(mhist_bucket *bucket_ptr, int dimid, int new_max)
{
    mhist_dim_info *di = MHIST_BUCKET_DIMINFO_PTR(bucket_ptr, dimid);
    double remaining_fraction = -1.0;
    int num_vals_before = -1;
    int num_vals_after = -1;

    if (new_max < di->min_val || new_max > di->max_val) {
        elog(ERROR, "cut_top_off_mhist_bucket(): Invalid new max value %d.",
                new_max);
    }


    /* SPECIAL CASE: Not cutting anything off. */
    if (new_max == di->max_val) {

        /* Don't need to do anything in this case. */
        return;
    }
    /* END SPECIAL CASE */

    /* Normal case. */
    num_vals_before = (di->max_val - di->min_val) + 1;
    num_vals_after = (new_max - di->min_val) + 1;
    remaining_fraction = 
            (double)(num_vals_after) / (double)(num_vals_before);

    di->max_val = new_max;
    di->accept_above_max = false;
    bucket_ptr->count *= remaining_fraction;
}

void cut_bottom_off_mhist_bucket(mhist_bucket *bucket_ptr, int dimid, 
        int new_min)
{
    mhist_dim_info *di = MHIST_BUCKET_DIMINFO_PTR(bucket_ptr, dimid);
    double remaining_fraction = -1.0;
    int num_vals_before = -1;
    int num_vals_after = -1;

    if (new_min < di->min_val || new_min > di->max_val) {
        elog(ERROR, 
            "cut_bottom_off_mhist_bucket(): Invalid new min value %d "
            "(Range %d to %d).",
                new_min, di->min_val, di->max_val);
    }

    /* SPECIAL CASE: Not cutting anything off. */
    if (new_min == di->min_val) {

        /* Don't need to do anything in this case. */
        return;
    }
    /* END SPECIAL CASE */


    num_vals_before = (di->max_val - di->min_val) + 1;
    num_vals_after = (di->max_val - new_min) + 1;
    remaining_fraction = 
            (double)(num_vals_after) / (double)(num_vals_before);

    di->min_val = new_min;
    di->accept_below_min = false;
    bucket_ptr->count *= remaining_fraction;
}


/* FUNCTION potential_overlap
 * ARGUMENTS: <left_bucket> and <right_bucket> are buckets in an MHIST.
 * PRECONDITIONS: The MHIST's buckets are sorted along all dimensions, and
 *      <left_bucket> comes earlier in that sort than <right_bucket> does.
 * POSTCONDITIONS: Returns TRUE if <right_bucket> is inside the range of 
 *      buckets that could possibly overlap with <left_bucket>.
 */
bool potential_overlap(mhist_bucket *left_bucket, mhist_bucket *right_bucket,
        int ndims) {
    int dimid = -1;
    for (dimid = 0; dimid < ndims; dimid++) {
        mhist_dim_info *left_di = 
            MHIST_BUCKET_DIMINFO_PTR(left_bucket, dimid);
        mhist_dim_info *right_di = 
            MHIST_BUCKET_DIMINFO_PTR(right_bucket, dimid);
        if (right_di->min_val <= left_di->max_val) {
            return true;
        }
    }
    return false;
}

/* FUNCTION mhist_total_count
 * ARGUMENTS: <hist> is an MHIST.
 * PRECONDITIONS: <hist> is properly initialized.
 * POSTCONDITIONS: Returns the sum of all the bucket counts in the histogram.
 */
double mhist_total_count(mhist *hist) {
    double total = 0.0;
    int bucket = -1;

    for (bucket = 0; bucket < MHIST_NUM_BUCKETS_USED(hist); bucket++) 
    {
        mhist_bucket *b_ptr = MHIST_BUCKET_PTR(hist, bucket);
 
        total += b_ptr->count;
    }

    return total;
}

/* FUNCTION mhist_to_str
 * POSTCONDITIONS: Returns a human-readable string representation of the
 * indicated MHIST.
 */
char *mhist_to_str(mhist *hist) {
	char	   *result = NULL;
	char		buf[256];
	int			dimid = -1;

    check_mhist_integrity(hist, "mhist_to_str()");

	result = (char *) palloc(256);

    snprintf(result, 256, "MHIST with %d buckets on %1.2f tuples of (",
            MHIST_NUM_BUCKETS_USED(hist), mhist_total_count(hist));

    /*
    elog(LOG, "mhist_to_str(): %d-dimensional histogram.", 
            MHIST_NUM_DIMS(hist));
            */
    
	for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
        /* A dimension may have multiple names. */
        int nameid = -1;
        int num_names = 0;

        /* First, count how many names there are. */
        for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist); nameid++) 
        {
            if (MHIST_NAME_DIM(hist, nameid) == dimid) {
                num_names++;
            }
        }

        if (0 == num_names) {
            elog(ERROR, 
            "mhist_to_str(): Couldn't find name for dimension %d.", dimid);
        }

        /* Then generate the name string. */
        for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist); nameid++) 
        {
            if (MHIST_NAME_DIM(hist, nameid) == dimid) {
                if (num_names > 1) {                
                    snprintf(buf, sizeof(buf), "%s/", 
                            MHIST_DIM_NAME(hist, nameid));
                } else {
                    snprintf(buf, sizeof(buf), "%s", 
                            MHIST_DIM_NAME(hist, nameid));
                }
                strncat(result, buf, 256);
                num_names--;
            }
        }

		if (dimid != hist->hdr.numDims - 1) {
			strncat(result, ", ", 256);
		} else {
			strncat(result, ")", 256);
		}
	}

    return result;
}

/* FUNCTION mhist_cross_product
 * ARGUMENTS: <hist1> and <hist2> point to MHISTs.
 * PRECONDITIONS: Arguments are initialized and loaded.
 * POSTCONDITIONS: Creates a new MHIST and fills it with the cross product of
 *      the input histograms.
 */
mhist *mhist_cross_product(const mhist *hist1, const mhist *hist2) {
    mhist *         result = NULL;
    int             hist1_bucketid = -1;
    int             hist2_bucketid = -1;
    int             result_bucketid = -1;
    int             hist1_nameid = -1;
    int             hist2_nameid = -1;
    int             new_num_dims = -1;

    check_mhist_integrity(hist1, "mhist_cross_product() hist1");
    check_mhist_integrity(hist2, "mhist_cross_product() hist2");

    new_num_dims = MHIST_NUM_DIMS(hist1) + MHIST_NUM_DIMS(hist2);
    if (new_num_dims > MHIST_MAX_DIMS) {
        elog(ERROR, 
                "mhist_cross_product(): (%s) X (%s) would have %d dimensions; "
                "max is %d.",
                mhist_to_str((mhist*)hist1), mhist_to_str((mhist*)hist2), 
                new_num_dims, MHIST_MAX_DIMS
            );
    }

    /* We create a new bucket for each pair of input buckets. */
    result = create_mhist(
            MHIST_NUM_BUCKETS_USED(hist1) * MHIST_NUM_BUCKETS_USED(hist2),
            new_num_dims);

    /* Copy over the names of the dimensions. */
    for (hist1_nameid = 0; hist1_nameid < MHIST_NUM_DIM_NAMES(hist1);
            hist1_nameid++) 
    {
        result->hdr.dimNames[hist1_nameid] = 
            hist1->hdr.dimNames[hist1_nameid];
    }
    for (hist2_nameid = 0; hist2_nameid < MHIST_NUM_DIM_NAMES(hist2);
            hist2_nameid++) 
    {
        int result_nameid = hist2_nameid + MHIST_NUM_DIM_NAMES(hist1);
        if (result_nameid >= MHIST_MAX_COL_NAMES) {
            elog(ERROR, 
                "Too many column names (%d >= %d) creating cross product.",
                result_nameid, MHIST_MAX_COL_NAMES);
        }

        result->hdr.dimNames[result_nameid] = 
            hist2->hdr.dimNames[hist2_nameid];

        /* Need to renumber the dimensions of the second input histogram. */
        MHIST_SET_NAME_DIM(result, result_nameid, 
                MHIST_NUM_DIMS(hist1) + MHIST_NAME_DIM(result, result_nameid));
    }

    MHIST_SET_NUM_DIM_NAMES(result, 
                            MHIST_NUM_DIM_NAMES(hist1) + 
                            MHIST_NUM_DIM_NAMES(hist2));

    /* Now we do the buckets. */
    result_bucketid = 0;
    for (hist1_bucketid = 0; hist1_bucketid < MHIST_NUM_BUCKETS_USED(hist1);
            hist1_bucketid++) {
        mhist_bucket *hist1_bkt_ptr = MHIST_BUCKET_PTR(hist1, hist1_bucketid);

        for (hist2_bucketid = 0; 
                hist2_bucketid < MHIST_NUM_BUCKETS_USED(hist2);
                hist2_bucketid++) {
            mhist_bucket *hist2_bkt_ptr = MHIST_BUCKET_PTR(hist2, 
                    hist2_bucketid);

            mhist_bucket *result_bkt_ptr = MHIST_BUCKET_PTR(result, 
                    result_bucketid);
            int             hist1_dimid = -1, hist2_dimid = -1, 
                            result_dimid = -1;
             
            /* Dimension information for the result bucket is the
             * contcatentation of the two source buckets' dimension
             * information. */
            result_dimid = 0;
            for (hist1_dimid = 0; hist1_dimid < MHIST_NUM_DIMS(hist1);
                    hist1_dimid++) {
                mhist_dim_info *hist1_di_ptr = 
                    MHIST_BUCKET_DIMINFO_PTR(hist1_bkt_ptr, hist1_dimid);
                
                mhist_dim_info *result_di_ptr = 
                    MHIST_BUCKET_DIMINFO_PTR(result_bkt_ptr, result_dimid);

                *result_di_ptr = *hist1_di_ptr;
                
                result_dimid++;
            }

            /* Note we don't reinitialize result_dimid here.  The values of
             * this variable keep incrementing as we fill in the rest of the
             * result's dimensions array. */
            for (hist2_dimid = 0; hist2_dimid < MHIST_NUM_DIMS(hist2);
                    hist2_dimid++) {
                mhist_dim_info *hist2_di_ptr = 
                    MHIST_BUCKET_DIMINFO_PTR(hist2_bkt_ptr, hist2_dimid);
                
                mhist_dim_info *result_di_ptr = 
                    MHIST_BUCKET_DIMINFO_PTR(result_bkt_ptr, result_dimid);

                *result_di_ptr = *hist2_di_ptr;
                
                result_dimid++;
            }

#ifdef FRED_MHIST_FK_HACK
            /* MAJOR HACK ALERT:
             * We force FK-join calculations here. */
            /* hist1 is the key relation; hist2 is the foreign-key relation.
             * We don't compensate for distributions along the key table... */
            result_bkt_ptr->count = hist2_bkt_ptr->count;
#else
            /* The number of tuples in the result bucket is equal to the
             * product of the numbers of tuples in the source buckets. */
            result_bkt_ptr->count = 
                hist1_bkt_ptr->count * hist2_bkt_ptr->count;
#endif

            /* We're done filling in this bucket. */
            result_bucketid++;
        }
    }

    MHIST_SET_NUM_BUCKETS_USED(result, result_bucketid);

    return result;
}

/* FUNCTION mhist_equality_select_in_place
 * ARGUMENTS: <hist> is an mhist, and <dim1> and <dim2> are the indices of two
 *      columns stored in the mhist.
 * PRECONDITIONS: The indicated columns are in range.
 * POSTCONDITIONS: Performs an approximate equality selection between the 
 *      two columns by shrinking the count of each bucket.
 *      May leave empty buckets.  Quite likely to do so, actually.
 */
void mhist_equality_select_in_place(mhist *hist, int dim1, int dim2) {
   int          bucketid = -1;

   for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist); bucketid++) {
        mhist_bucket *bkt = MHIST_BUCKET_PTR(hist, bucketid);
        int dim1_max = MHIST_BUCKET_DIMINFO_PTR(bkt, dim1)->max_val;
        int dim1_min = MHIST_BUCKET_DIMINFO_PTR(bkt, dim1)->min_val;
        int dim1_range = dim1_max - dim1_min + 1;
        int dim2_max = MHIST_BUCKET_DIMINFO_PTR(bkt, dim2)->max_val;
        int dim2_min = MHIST_BUCKET_DIMINFO_PTR(bkt, dim2)->min_val;
        int dim2_range = dim2_max - dim2_min + 1;
        double prob_val_fits_bucket = -1.0;
        double prob_choose_second_dim = -1.0;
        double prob_choose_same_val = -1.0;

        /* First, we compute the amount of overlap between the two ranges. */
        int overlap = -1;

        if ((dim1_min < dim2_min && dim1_max < dim2_min)
                ||
                (dim1_min > dim2_min && dim1_min > dim2_max)) {
            /* Ranges don't overlap. */
            overlap = 0;
        } else {
            overlap = Min(dim1_max, dim2_max) - Max(dim1_min, dim2_min) + 1;
        }

#if 0
        /* Complicated version: */
        if (dim1_min < dim2_min) {
            if (dim1_max >= dim2_min) { 
                overlap = Min(dim1_max, dim2_max) - dim2_min + 1;
            } else { 
                overlap = 0;
            }
        } else if (dim1_min > dim2_min) {
            if (dim1_min <= dim2_max) {
                overlap = Min(dim1_max, dim2_max) - dim1_min + 1;
            } else {
                overlap = 0;
            }
        } else { /* dim1_min == dim2_min */
            overlap = Min(dim1_max, dim2_max) - dim1_min + 1; 
        }
#endif /* 0 */

#ifdef FRED_MHIST_FK_HACK
        /* We're enforcing FK-join semantics in mhist_cross_product(). */
        /* TODO: At least compensate for overlap... */
        prob_val_fits_bucket = (double)(overlap) / (double)(dim1_range);

        bkt->count *= prob_val_fits_bucket;
        bkt->count *= 0.0625;
        /*
        elog(LOG, "Dim1 range is %f; dim2 range is %f", 
                (float)dim1_range, 
                (float)dim2_range);
                */
#else
        /* We assume that tuples are randomly distributed within a bin. */

        /* Given that we chose a particular value along the first dimension,
         * what is the likelihood of choosing the same value along the second
         * dimension?
         *
         * First, we compute the probability that the corresponding
         * second-dimension value maps into this bucket.
         */
        prob_val_fits_bucket = (double)(overlap) / (double)(dim1_range);

        /* Next, we compute the probability that we choose the corresponding
         * value for this tuple, given that that value falls into this bucket. 
         */
        prob_choose_second_dim = 1.0 / (double)(dim2_range);

        /* Multiply these two probabilities to obtain the probability that we
         * choose a dim1 for which a corresponding dim2 exists AND we then
         * choose that same value for dim2, assuming that we're sampling
         * uniformly at random from the bucket. */
        prob_choose_same_val = prob_val_fits_bucket * prob_choose_second_dim;

        /* Now we can adjust the bucket's count to create the result. */
        bkt->count *= prob_choose_same_val;
#endif
    }
}

/* FUNCTION check_mhist_integrity
 * ARGUMENTS: <hist> is an MHIST.  <id> is a string that identifies the
 * position in the code where you're calling this function from.
 * POSTCONDITIONS: Does a number of sanity checks on <hist>.  
 *      Calls elog(ERROR) if it detects a problem. 
 */
void check_mhist_integrity(const mhist *hist, const char *id) {
    /* 
     * Simple checks... 
     */
    if (NULL == hist) {
        elog(ERROR, "%s: NULL MHIST pointer!", id);
    }
    if (hist->hdr.length != MHIST_SIZE(MHIST_NUM_BUCKETS_ALLOC(hist), 
                MHIST_NUM_DIMS(hist))) 
    {
        elog(ERROR, "%s: Invalid data structure length %d (Should be %d).", 
                id, hist->hdr.length, 
                MHIST_SIZE(MHIST_NUM_BUCKETS_ALLOC(hist), 
                    MHIST_NUM_DIMS(hist)) );
    }

    if (MHIST_NUM_DIMS(hist) > MHIST_MAX_DIMS) {
        elog(ERROR, "%s: Histogram has %d dimensions; max is %d.",
                id, MHIST_NUM_DIMS(hist), MHIST_MAX_DIMS);
    }

    if (MHIST_NUM_DIM_NAMES(hist) < MHIST_NUM_DIMS(hist)) {
        elog(ERROR, "%s: Histogram has %d dimensions but only %d names.",
                id, MHIST_NUM_DIMS(hist), MHIST_NUM_DIM_NAMES(hist));
    }

    if (MHIST_NUM_DIM_NAMES(hist) > MHIST_MAX_COL_NAMES) {
        elog(ERROR, "%s: Histogram has too many dimension names.", id);
    }

    /* 
     * Complex checks... 
     */

#ifdef ENABLE_COMPLEX_MHIST_CHECKS
    {
    int         bucketid = -1;
    int         dimid = -1;

    /* Make sure the empty buckets are zeroed out. */
    for (bucketid = MHIST_NUM_BUCKETS_USED(hist);
            bucketid < MHIST_NUM_BUCKETS_ALLOC(hist); bucketid++) {
        char *bkt_byte_ptr = (char*)(MHIST_BUCKET_PTR(hist, bucketid));
        int bytenum = -1;

        for (bytenum = 0; 
                bytenum < MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)); 
                bytenum++) 
        {
            if (bkt_byte_ptr[bytenum] != 0) {
                elog(ERROR, "%s: Nonzero value in byte %d of empty bucket %d.",
                        id, bytenum, bucketid);
            }
        }
    }

    /* Check for buckets with a count < 0 */
    for (bucketid = MHIST_NUM_BUCKETS_USED(hist);
            bucketid < MHIST_NUM_BUCKETS_ALLOC(hist); bucketid++) {
        mhist_bucket *bkt_ptr = MHIST_BUCKET_PTR(hist, bucketid);

        if (bkt_ptr->count < 0.0) {
            elog(ERROR, "%s: Negative count on bucket %d",
                    id, bucketid);
        }
    }

 

    /* 
     * Debugging checks... 
     */

    /* Check for corruption of the bucket boundaries. */
    for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist); bucketid++) {
        for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
            mhist_dim_info *di = MHIST_DIMINFO_PTR(hist, bucketid, dimid);

            if (di->min_val > di->max_val) {
                elog(ERROR, 
                "%s: Min val %d greater than max val %d in bucket %d, dim %d.",
                id, di->min_val, di->max_val, bucketid, dimid);
            }
            if (di->min_val < -1000000 || di->min_val > 1000000) {
                elog(ERROR, "%s: Min val %d in bucket %d, dim %d out of range.",
                        id, di->min_val, bucketid, dimid);
            }
            if (di->max_val < -1000000 || di->max_val > 1000000) {
                elog(ERROR, "%s: Max val %d in bucket %d, dim %d out of range.",
                        id, di->max_val, bucketid, dimid);
            }
        }
    }

    /* Make sure we have labels along all the dimensions. */
    /*
    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
        if (0 == strlen(hist->hdr.dims[dimid].name)) {
            elog(ERROR, "%s: Empty name for dimension %d.", id, dimid);
        }
    }
    */
    }
#endif /* ENABLE_COMPLEX_MHIST_CHECKS */

}


/* FUNCTION get_mhist_split_point
 * ARGUMENTS: <giant_tup_array> is an array of tuples, 
 *      <desc> is a descriptor for the tuples in <giant_tup_array>,
 *      <bucket_offsets> and <bucket_lengths> describe the current
 *      bucketization of the array, 
 *      <nbuckets_done> is the number of buckets we've created thus far,
 *      <ranges> describes the maximum and minimum values along each dimension
 *      of the tuples.
 *      <split_bucketid>, <split_dimid>, and <split_offset> 
 *              are output parameters that describe where to split.
 * PRECONDITIONS: The tuples are properly partitioned within the array.
 * POSTCONDITIONS: Returns which bucket to split at and at what offset.
 *      Note that the offset is the index of the element to the _left_ of
 *          the split point!
 */
void get_mhist_split_point(
        mhist *hist,
        HeapTuple *giant_tup_array, TupleDesc desc,
        const int *bucket_offsets,
        const int *bucket_lengths, 
        int nbuckets_done, 
        const mhist_dim_info *ranges,
        int *split_bucketid, int *split_dimid, int *split_offset) 
{
    /* PROF_ENTER_FUNCTION("get_mhist_split_point"); */

    bucket_maxdiff_info *maxdiffs = NULL;
    int bucketid = -1;
    int tupid = -1;
    
    double best_norm_diff = -1.0;
    int best_size = -1;

    HeapTuple *bucketstart = NULL;

    /* We currently use the MaxDiff heuristic; that is, we sort each bucket 
     * along each dimension and maximize the distance between points on either
     * side of the bucket boundary we choose.  */
    maxdiffs = get_maxdiffs(hist, giant_tup_array, desc, bucket_offsets, 
            bucket_lengths, nbuckets_done, ranges);


    /* We split along the bucket with the maximum difference between adjacent
     * values, using bucket size to break ties. */
    *split_bucketid = -1;
    *split_dimid = -1;
    for (bucketid = 0; bucketid < nbuckets_done; bucketid++) {
        /* SPECIAL CASE: If the edge of a bucket is more than one unit away
         * from the closest point in the bucket, split off a bucket with 
         * zero elements. */
        if (maxdiffs[bucketid].empty_space_lower) {
            *split_bucketid = bucketid;
            *split_dimid = maxdiffs[bucketid].dimid;
            *split_offset = -1;

            return;
        } 
        if (maxdiffs[bucketid].empty_space_upper) {
            *split_bucketid = bucketid;
            *split_dimid = maxdiffs[bucketid].dimid;
            *split_offset = bucket_lengths[bucketid] - 1;
            return; 
        }
        /* END SPECIAL CASE */

        if ((-1 == *split_bucketid) 
                || (maxdiffs[bucketid].norm > best_norm_diff) 
                || (maxdiffs[bucketid].norm == best_norm_diff 
                    && bucket_lengths[bucketid] > best_size))
        {
            *split_bucketid = bucketid;
            *split_dimid = maxdiffs[bucketid].dimid;
            best_norm_diff = maxdiffs[bucketid].norm;
            best_size = bucket_lengths[bucketid];
        }
    }


    /* We should have found a pair of tuples to split between somewhere. */
    Assert(best_norm_diff > 0.0);

    /* Figure out where to split the bucket. */
    *split_offset = -1;

    bucketstart = giant_tup_array + bucket_offsets[*split_bucketid];
    sort_tuples_along_dim(bucketstart, bucket_lengths[*split_bucketid], 
            *split_dimid, desc);


    /* Try to find a point near the middle of the bucket. */
    for (tupid = (bucket_lengths[*split_bucketid] / 2); 
            tupid < (bucket_lengths[*split_bucketid] - 1); 
            tupid++) {
        int lower_val = get_int_field_val(bucketstart[tupid], desc,
                    (*split_dimid) + 1, true);
        int higher_val = get_int_field_val(bucketstart[tupid+1], desc,
                    (*split_dimid) + 1, true);
        int diff = higher_val - lower_val;
        
        if (diff == maxdiffs[*split_bucketid].abs) {
            /**split_offset =  tupid + 1;*/
            *split_offset =  tupid;
            break;
        }
    }

    if (-1 == *split_offset) {
        /* Didn't find anything in the second half, so try the first. */
        for (tupid = 0; 
                tupid < ((bucket_lengths[*split_bucketid] / 2) ); 
                tupid++) 
        {
            int lower_val = get_int_field_val(bucketstart[tupid], desc,
                        (*split_dimid) + 1, true);
            int higher_val = get_int_field_val(bucketstart[tupid+1], desc,
                        (*split_dimid) + 1, true);
            int diff = higher_val - lower_val;
            
            if (diff == maxdiffs[*split_bucketid].abs) {
                /**split_offset =  tupid + 1;*/
                *split_offset =  tupid;
                break;
            }

        }
    }


    /* We should have found a pair of tuples with the specified difference. */
    Assert(-1 != *split_offset);

    /* Clean up. */
    pfree(maxdiffs);

    /* PROF_EXIT_FUNCTION("get_mhist_split_point"); */
}


bool no_buckets_to_split(HeapTuple *giant_tup_array,
        const int *bucket_offsets, 
        const int *bucket_lengths, 
        int nbuckets_done,
        TupleDesc desc)
{
    int         bucketid = -1;

    bool        all_buckets_tiny = false;
    bool        all_buckets_single_val = false;
    int         ndims = (desc == NULL) ? 0 
                        : get_num_non_timestamp_attrs(desc);

    HeapTuple   *bucketstart = NULL;

    /* If all the buckets are of size 0 or 1, we can't split them. */
    all_buckets_tiny = true; 
    for (bucketid = 0; bucketid < nbuckets_done; bucketid++) {

        if (bucket_lengths[bucketid] > 1) {
            all_buckets_tiny = false;
            break;
        }
    }

    if (all_buckets_tiny) {
        elog(DEBUG1, "no_buckets_to_split(): All buckets are tiny.");
        return true;
    }

    /* If every bucket contains only a single value along all dimensions, 
     * then we can't do any further splitting with the current technique. */
    all_buckets_single_val = true;
    for (bucketid = 0; bucketid < nbuckets_done; bucketid++) {
        int tupid = -1;

        bucketstart = giant_tup_array + bucket_offsets[bucketid];

        /* Just compare each tuple to the first one in this bucket. */
        for (tupid = 1; tupid < bucket_lengths[bucketid]; tupid++) {
            int dimid = -1; 

            for (dimid = 0; dimid < ndims; dimid++) {
                int first_val = get_int_field_val(bucketstart[0], desc, 
                        dimid+1, true);
                int this_val = get_int_field_val(bucketstart[tupid], desc,
                        dimid+1, true);
                if (this_val != first_val) {
                    all_buckets_single_val = false;
                    break;
                }
            }
        }
    }

    if (all_buckets_single_val) {
        elog(DEBUG1, 
            "no_buckets_to_split(): All buckets contain a single value.");
        return true; 
    }


    return false;
}



/* FUNCTION mhist_create_buckets
 * ARGUMENTS: <mhist> is an empty MHIST with zero buckets.  <tuparr> is an
 *      array of <ntups> tuples described by <desc> that give a general idea of
 *      what tuples will be inserted into <mhist>.
 * PRECONDITIONS: <mhist> has never had any buckets allocated to it.
 * POSTCONDITIONS: Uses the tuples provided to create as many buckets as
 *      possible in the histogram.  Does NOT free the tuples.
 */
void mhist_create_buckets(mhist *hist, HeapTuple *tuparr, int ntups, 
        TupleDesc desc)
{
	/* PROF_ENTER_FUNCTION("mhist_create_buckets"); */
    int         *bucket_offsets = NULL;
    int         *bucket_lengths = NULL;
    int         nbuckets = MHIST_NUM_BUCKETS_ALLOC(hist);
    int         nbuckets_done = -1;
    int         dimid = -1;
    int         ndims = -1;
    int         random_count;
    int         per_bucket = 2;
    int         max_length = 0;
    double      prob_array_margin = 0.001;
    mhist_dim_info *ranges = NULL;
    mhist_bucket *first_bucket_ptr = NULL;

    /*check_mhist_integrity(hist, "mhist_create_buckets() input");*/

    Assert(0 == MHIST_NUM_BUCKETS_USED(hist));

    /* The MHIST bucketizing algorithm is as follows:
     *
     * WHILE the number of buckets is less than the target number DO
     *      Choose a bucket to split and a dimension to split it on.
     *      Split the bucket.
     * END
     *
     * Currently, we use the MaxDiff heuristic; that is, we sort along each
     * dimension and choose the dimension with the maximum difference between
     * two adjacent points.
     */

    /*random_count = get_random(giant_tup_array, ntups, PROB); */
    /*elog(LOG, "YANG beginning random selection...."); */
    random_count = get_random(tuparr, ntups, nbuckets, per_bucket, max_length, prob_array_margin);
    ntups = random_count;
    /*elog(LOG, "YANG randomly selected %d tuples", random_count); */

    /* Allocate an array of offsets into the tuple array containing the
     * different buckets. */
    bucket_offsets = palloc(nbuckets * sizeof(int));
    bucket_lengths = palloc(nbuckets * sizeof(int));
        /* Freed at bottom of function. */

    elog(DEBUG1, 
        "mhist_create_buckets(): Calculating value ranges for %d tuples.", 
            ntups);

    /* Compute the min. and max. values along each dimension. */
    ranges = get_ranges(tuparr, ntups, desc, MHIST_NUM_DIMS(hist));

    elog(DEBUG1, "mhist_create_buckets(): Creating first bucket.");

    /* We start with one big bucket... */
    bucket_offsets[0] = 0;
    bucket_lengths[0] = ntups;

    ndims = get_num_non_timestamp_attrs(desc);

    first_bucket_ptr = MHIST_BUCKET_PTR(hist, 0);
    for (dimid = 0; dimid < ndims; dimid++) {
        mhist_dim_info *di = MHIST_DIMINFO_PTR(hist, 0, dimid);    
        di->min_val = ranges[dimid].min_val;
        di->max_val = ranges[dimid].max_val;
        di->accept_above_max = di->accept_below_min = true;
    }
    
    elog(DEBUG1, "mhist_create_buckets(): Splitting buckets.");

    /* Since there's one bucket to begin with, we iterate (nbuckets-1) times.
     */
    for (nbuckets_done = 1; nbuckets_done < nbuckets; nbuckets_done++) {
        bool managed_to_split = split_a_bucket(tuparr, ntups, 
                desc, bucket_offsets, bucket_lengths, hist, ranges, 
                nbuckets_done);

        if (managed_to_split) {
            MHIST_SET_NUM_BUCKETS_USED(hist, nbuckets_done + 1);
        } else {
            /* Ran out of buckets to split. */
            break;
        }
    }

    /* SPECIAL CASE: Didn't do any splits.  Zero out the first bucket to avoid
     * triggering debug code. */
    if (0 == MHIST_NUM_BUCKETS_USED(hist)) {
        memset(first_bucket_ptr, 0x0, MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)));
    }
    /* END SPECIAL CASE */

    /* Clean up after ourselves. */
    pfree(bucket_offsets);
    pfree(bucket_lengths);
    pfree(ranges);

    /*check_mhist_integrity(hist, "mhist_create_buckets() output");*/


	/* PROF_EXIT_FUNCTION("mhist_create_buckets"); */
}


/* FUNCTION tdesc_to_mhist_diminfo
 * ARGUMENTS: <hist> is an empty MHIST, and <desc> is a tuple descriptor.
 * PRECONDITIONS: <desc> matches the tuples you're going to insert into <hist>
 * POSTCONDITIONS: Copies the column names in <desc> into <hist>'s header
 *      fields.
 */
void tdesc_to_mhist_diminfo(mhist *hist, TupleDesc desc) {
    int dimid = -1;

    Assert(get_num_non_timestamp_attrs(desc) == MHIST_NUM_DIMS(hist));
    Assert(mhist_can_summarize_tuple(desc));

	/* Fill in information about the different dimensions. */
	for (dimid = 0; dimid < MHIST_NUM_DIMS(hist); dimid++) {
        int attrid = get_attr_id_skipping_ts(dimid + 1, desc);
        Form_pg_attribute   attr = desc->attrs[attrid-1];

        /* The OID of the relation is stored in the tuple descriptor's
         * attribute fields; different fields could come from different
         * relations, so we compute the name for each field. */
        Relation r = RelationIdGetRelation(attr->attrelid);

        /* Need to check whether this relation exists (the column might be
         * an intermediate result). */
        char * relname;
        if (NULL == r) {
            relname = "(unknown)";
        } else {
            relname = RelationGetRelationName(r);
        }

        /* TODO: Should we call RelationClose() here to decrement a reference
         * count? */

        MHIST_SET_NAME_DIM(hist, dimid, dimid);
        snprintf(MHIST_DIM_NAME(hist, dimid), MHIST_COL_NAME_LEN,
                "%s.%s", relname, (desc->attrs[attrid-1])->attname.data);
	}

    MHIST_SET_NUM_DIM_NAMES(hist, MHIST_NUM_DIMS(hist));

    /* Verify that this is a valid MHIST. */
    check_mhist_integrity(hist, "tdesc_to_mhist_diminfo()");
}


/* FUNCTION create_reg_mhist()
 * ARGUMENTS: <tups> is a set of tuples.  
 * POSTCONDITIONS: Creates an MHIST with square (e.g. hypercube) buckets, 
 *      <bucket_width> units to a side, aligned along <bucket_width>.  This
 *      MHIST will be just big enough to hold the indicated range of tuples.
 */
mhist * create_reg_mhist(HeapTuple *tups, int ntups, 
        TupleDesc desc, int *bucket_widths)
{
    int num_cols = get_num_non_timestamp_attrs(desc);
    int colno = -1;
    int tot_num_buckets = -1;
    mhist * ret = NULL;
    int *itr = NULL;
    int dimid = -1;
    int nbuckets_created = -1;


    /* Compute the range along each dimension. */
    mhist_dim_info *ranges = get_ranges(tups, ntups, desc, num_cols);


    /* Expand the ranges to align them. */
    for (colno = 0; colno < num_cols; colno++) {
        int min_val_diff = ranges[colno].min_val % bucket_widths[colno];
        int max_val_diff = ranges[colno].max_val % bucket_widths[colno];

        Assert(min_val_diff >= 0);
        Assert(max_val_diff >= 0);
            /* Not sure if this code works otherwise. */

        ranges[colno].min_val = ranges[colno].min_val - min_val_diff; 
        ranges[colno].max_val = ranges[colno].max_val + 
            (bucket_widths[colno] - max_val_diff - 1); 
        
    }

    /* We'll build a full grid of buckets, at least for starters. */
    tot_num_buckets = 1;
    for (colno = 0; colno < num_cols; colno++) {
        int bkts_for_col =
            (ranges[colno].max_val + 1 - ranges[colno].min_val) 
                / bucket_widths[colno];
            /* The +1 in the previous line is because our bucket
             * boundaries are inclusive. */

        /*
        elog(LOG, "create_reg_mhist(): %d buckets on dimension %d",
                bkts_for_col, colno);
                */
        tot_num_buckets *= bkts_for_col; 
    }

    /*
    elog(LOG, "create_reg_mhist(): Initial bucket count is %d.",
            tot_num_buckets);
            */

    ret = create_mhist(tot_num_buckets, num_cols);

    /* Set up the dimension information in the header. */
    tdesc_to_mhist_diminfo(ret, desc);

    /* Create the buckets. */
    itr = palloc(num_cols * sizeof(*itr));
    for (dimid = 0; dimid < num_cols; dimid++) {
        itr[dimid] = 0;
    }

    nbuckets_created = 
        create_reg_buckets(ret, ranges, num_cols, 0, bucket_widths, 0, itr);  

    Assert(nbuckets_created == tot_num_buckets);

    MHIST_SET_NUM_BUCKETS_USED(ret, nbuckets_created);

    pfree(itr);
    pfree(ranges);

    MHIST_SET_IS_FGHIST(ret,true);

    /*elog(LOG, "create_reg_mhist(): Hist is '%s'", mhist_to_str(ret));*/

    return ret;
}

/* Recursive subroutine of create_reg_mhist().  
 *
 * Returns number of buckets loaded.
 *
 * Arguments:
 *
 *      <itr> is an "iterator" containing our bucket number along each
 *      dimension.
 * */
int create_reg_buckets(mhist *hist, mhist_dim_info *ranges, int num_dims,
        int cur_dim, int *bucket_widths, int bucket_num, int *itr) 
{
    int dimid = -1;

    Assert(bucket_num < MHIST_NUM_BUCKETS_ALLOC(hist));

    if (cur_dim == num_dims) {
        /* BASE CASE: Generate the indicated bucket. */
        mhist_bucket *b = MHIST_BUCKET_PTR(hist, bucket_num);

        for (dimid = 0; dimid < num_dims; dimid++) {
            mhist_dim_info *di = MHIST_BUCKET_DIMINFO_PTR(b, dimid);

            di->min_val = ranges[dimid].min_val + 
                (bucket_widths[dimid] * itr[dimid]);
            di->max_val = di->min_val + bucket_widths[dimid] - 1;

            di->accept_below_min = (di->min_val == ranges[dimid].min_val) 
                ? true : false;
            di->accept_above_max = (di->max_val == ranges[dimid].max_val)
                ? true : false;

            Assert(di->max_val >= di->min_val);
        }

        return 1;

    } else {
        /* RECURSIVE CASE */
        int nsteps = (1 + ranges[cur_dim].max_val - ranges[cur_dim].min_val)
                        / bucket_widths[cur_dim];
        int orig_bucket_num = bucket_num;
        
        for (itr[cur_dim] = 0; itr[cur_dim] < nsteps; itr[cur_dim]++) {
            bucket_num += 
                create_reg_buckets(hist, ranges, num_dims, cur_dim + 1, 
                    bucket_widths, bucket_num, itr);
        }

        return (bucket_num - orig_bucket_num);
    }

}

/* FUNCTION remove_duplicate_names
 * ARGUMENTS: <names> is an array of <num_names> dimension names.
 * POSTCONDITIONS: Removes names for the same dimension that are the same.
 *      Also ensures that everything is zero-padded.
 *
 *      Returns the number of names left after duplicate elimination.
 */
int remove_duplicate_names(mhist_dim_name *names, int num_names) {
    int         nameid = -1;
    int         otherid = -1;

    int         orig_num_names = num_names;

    for (nameid = 0; nameid < num_names; nameid++) 
    {
        int cur_dim_no = names[nameid].dim_no;
        char *cur_name = names[nameid].name;

        Assert(MHIST_NOT_A_DIMID != cur_dim_no);

        /* We maintain the invariant that every entry < nameid is
         * duplicate-free.  Check to see if there's a duplicate for this entry
         * among the entries > nameid. */
        for (otherid = nameid + 1; otherid < num_names; otherid++) 
        {
            if (names[otherid].dim_no == cur_dim_no
                    && 0 == strncasecmp(names[otherid].name, cur_name, 
                                        MHIST_COL_NAME_LEN)) 
            {
/*                
        elog(LOG, 
        "Name '%s' at index %d matches with '%s' at index %d.",
        cur_name, cur_dim_no, names[otherid].name, otherid);
        */
                /* Found a duplicate -- move everything to the right one 
                 * index to the left.*/
                int i = -1;
                for (i = otherid + 1; i < num_names; i++) {
                    names[i-1] = names[i]; 
                }
                num_names--;

                /* We just moved the next name we were going to check, so we
                 * have to back up in the array. */
                otherid--;
            }
        }
    }

    /* Zero out the rest of the array. */
    memset(&(names[num_names]), 0x0, 
            (sizeof(mhist_dim_name) * (orig_num_names - num_names)));

    return num_names;

}

/* FUNCTION get_dimid_for_name
 * ARGUMENTS: <names> is an array of <num_names> dimension names.
 *      <name> is a string.
 * POSTCONDITIONS: Returns the dimid for the indicated column name, ignoring
 *      case, or MHIST_NOT_A_DIMID if it can't find the name.
 */
int get_dimid_for_name(mhist_dim_name *names, int num_names, 
        const char *name) {
    int nameid = -1;

    for (nameid = 0; nameid < num_names; nameid++) 
    {
        if (0 == strncasecmp(names[nameid].name, name, MHIST_COL_NAME_LEN)) {
            return names[nameid].dim_no;
        }
    }

    /* Didn't find anything... */
    return MHIST_NOT_A_DIMID;
}

/*******************************************************************************
 * RANDOM SELECTION
 ******************************************************************************/

/* FUNCTION get_random
 * ARGUMENTS: <array> is a large HeapTuple array of size <length> of whose
 *      elements we're selecting a subset by using random skipping with a
 *      probability of <prob>.
 * POSTCONDITIONS: Returns the number of elements that were selected, and swaps
 *      all the selected elements to the start of the array.
 */
int get_random(HeapTuple* array, int length, int bucket_count, int per_bucket, int max_length, double margin) {
    /* PROF_ENTER_FUNCTION("get_random"); */
    double prob = ((double) (bucket_count * per_bucket)) / (double) length;
    int min_count = (int) ceil(log(margin) / log(1.0 - prob));
    /*int max_count = length / (bucket_count * per_bucket);*/
    int prob_length = max_length == 0 || min_count < max_length ? min_count : max_length;
    /*int prob_length = min_count < max_count ? min_count : max_count; */
    /*elog(LOG, "YANG length = %d", prob_length);*/
    double prob_array[prob_length];

    /*elog(LOG, "Actual Array\n", array[0]); */
    int i = 0;
    int count = 0;

	HeapTuple temp;
	
	/* XXX mashah change. Moved these to compile, please check. */
	if (prob >= 1.0) {
        return length;
    }

	create_prob_array(prob_array, prob, prob_length);
	/* XXX mashah change. */
	
    while (TRUE) { 
        int skip = skip_random(prob_array, prob_length);
        i += skip;
        if (i >= length) {
            break;
        }
        /*elog(LOG, "skipping %d, getting (%d)\n", skip, array[i].x); */
        /*printf("skipping %d, getting (%d,%d)\n", skip, array[i].x, array[i].y); */
        
        /* swap the randomly selected element to start of array */
        temp = array[count];
        array[count] = array[i];
        array[i] = temp;
        
        count++;
    }
    
    /* PROF_EXIT_FUNCTION("get_random"); */
    return count;
}

/* FUNCTION create_prob_array
 * ARGUMENTS: <array> is the probability array of length <length> to be filled.
 *      <prob> is the probability that <array> will be based on.
 * POSTCONDITIONS: Returns the number of elements that were selected.
 */
void create_prob_array(double* array, double prob, int length) {
    /* int* array = *array_ptr; 
     * (int*) palloc(length * sizeof(int)); 
     * new int[length]; */
    double cur_prob = prob;
    int i;
    array[0] = prob;
    /*elog(LOG, "Probability Array\n", array[0]); */
    /*elog(LOG, "%f\n", array[0]); */
    for (i = 1; i < length; i++) {
        cur_prob = cur_prob * (1.0 - prob);
        array[i] = array[i - 1] + cur_prob;
        /*elog(LOG, "%f\n", array[i]); */
    }
/*    printf("RAND_MAX = %d\n", RAND_MAX); */
/*    print_array(array, length); */
    
    /*array_ptr = array; */
    /*return length; */
}

/* FUNCTION skip_random
 * ARGUMENTS: <array> is a probability array of size <length> that will be used
 *      to determine the number of elements we're skipping.
 * POSTCONDITIONS: Returns the number of elements to skip.
 */
int skip_random(double* array, int length) {
    double  r = (double) rand() / (double) RAND_MAX;
    int high = length;
    int low = -1;
    int i = -1;
    
    while (TRUE) {
        i = (high + low) / 2; /* find the midpoint */
        if (i == 0 || i == length - 1) {
            break;
        } else if (r < array[i] && r >= array[i - 1]) {
            break;
        } else if (r < array[i]) {
            high = i;
        } else {
            low = i;
        }
    }

    return i + 1;
}

/* FUNCTION add_bucket_to_mhist
 * ARGUMENTS: <hist> is an MHIST to which we want to add a bucket.
 *      <minvals> and <maxvals> each contain <ndims> values, representing
 *      the min/max values along each of the <ndims> dimensions.
 * POSTCONDITIONS: Returns false if the histogram was full, true otherwise.
 */
bool add_bucket_to_mhist(mhist* hist, int* minvals, int* maxvals, double count) {
    mhist_bucket* bucket;
    int bucketid;
    int dimid;
    int nbuckets_alloc;
    int nbuckets_used;
    int ndims;

    /* check the bucket counts */
    nbuckets_used = MHIST_NUM_BUCKETS_USED(hist);
    nbuckets_alloc = MHIST_NUM_BUCKETS_ALLOC(hist);
    assert( nbuckets_used <= nbuckets_alloc );
    if (nbuckets_used == nbuckets_alloc) {
        return false;
    }

    /* get and initialize the new bucket */
    bucketid = nbuckets_used;
    bucket = MHIST_BUCKET_PTR(hist, bucketid);
    bucket->count = count;

    /* set the min/max values along each dimension */
    ndims = MHIST_NUM_DIMS(hist);
    for (dimid = 0; dimid < ndims; dimid++) {
        mhist_dim_info *di = MHIST_DIMINFO_PTR(hist, bucketid, dimid);
        di->min_val = minvals[dimid];
        di->max_val = maxvals[dimid];
        di->accept_above_max = di->accept_below_min = false;
    }

    /* increment the count of used buckets */
    nbuckets_used++;
    MHIST_SET_NUM_BUCKETS_USED(hist, nbuckets_used);

    /* the new bucket was successfully added */
    return true;
}

/* FUNCTION fghist_equijoin
 * ARGUMENTS: <hist1> and <hist2> are the join operands, and <col1> and <col2>
 *      are columns of those histograms.
 * PRECONDITIONS: Input histograms are fixed-grid histograms.
 *      The join predicate is <col1> = <col2>
 * POSTCONDITIONS: Computes the join as quickly as possible.
 */
mhist *fghist_equijoin(mhist *hist1, mhist *hist2, 
        int col1, int col2)
{
    int new_num_dims = -1;
    int new_num_bkts = -1;
    mhist_dim_info *first_di1 = NULL;
    mhist_dim_info *first_di2 = NULL;
    mhist_dim_info *last_di1 = NULL;
    mhist_dim_info *last_di2 = NULL;
    int joindim_stepsize = -1;
    int joindim_numsteps = -1;
    int lowest_join_val = -1;
    int highest_join_val = -1;
    int dimid = -1;
    mhist *result = NULL;
    int *bucket_widths = NULL;
    mhist_dim_info *bucket_ranges = NULL;
    int bkt_dimid = -1;
    int join_dimid = -1;
    int hist1dim_to_resultdim[MHIST_MAX_DIMS];
    int hist2dim_to_resultdim[MHIST_MAX_DIMS];


    Assert(MHIST_IS_FGHIST(hist1) && MHIST_IS_FGHIST(hist2));

    /* Make sure that the two histograms line up along the indicated dimension.
     */
    Assert(fghist_stepsize_along_dimension(hist1,col1)
            == fghist_stepsize_along_dimension(hist2,col2));

    /* Compute the dimensionality of the join result. */
    new_num_dims = MHIST_NUM_DIMS(hist1) + MHIST_NUM_DIMS(hist2) - 1;

    /* The last dimension is the join dimension. */
    join_dimid = new_num_dims - 1;

    if (new_num_dims > MHIST_MAX_DIMS) {
        elog(ERROR, 
            "fghist_equijoin(): (%s) join (%s) would have %d dimensions; "
            "max is %d.",
            mhist_to_str((mhist*)hist1), mhist_to_str((mhist*)hist2), 
            new_num_dims, MHIST_MAX_DIMS
        );
    }

    /* Compute number of buckets in the join result */
    first_di1 = MHIST_DIMINFO_PTR(hist1, 0, col1); 
    first_di2 = MHIST_DIMINFO_PTR(hist2, 0, col2); 
    last_di1 = MHIST_DIMINFO_PTR(hist1, MHIST_NUM_BUCKETS_USED(hist1)-1, col1); 
    last_di2 = MHIST_DIMINFO_PTR(hist2, MHIST_NUM_BUCKETS_USED(hist2)-1, col2); 
        /* Note that first_di1 might equal last_di1 and so on. */

    lowest_join_val = max(first_di1->min_val, first_di2->min_val);
    highest_join_val = min(last_di1->max_val, last_di2->max_val);

    joindim_stepsize = fghist_stepsize_along_dimension(hist1, col1);
    joindim_numsteps = 
        (highest_join_val - lowest_join_val + 1) / joindim_stepsize;

    new_num_bkts = joindim_numsteps;
        /* We take the range that overlaps between the two histograms. */
        /* The '+1' here is because each bucket holds values x such that
         * min <= x <= max. */

    bucket_widths = palloc(sizeof(int) * new_num_dims);
    bucket_ranges = palloc(sizeof(mhist_dim_info) * new_num_dims);
        /* pfree'd down below */

    bkt_dimid = 0;

    /* Define the dimensions of the result in terms of the input dimensions. */
    /* Recall that the dimensions of result are:
     *      non-join dims of hist1,
     *      non-join dims of hist2,
     *      join dim
     */
    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist1); dimid++) {
        if (dimid < col1) {
            hist1dim_to_resultdim[dimid] = dimid;
        } else if (dimid == col1) {
            hist1dim_to_resultdim[dimid] = join_dimid;
        } else {
            hist1dim_to_resultdim[dimid] = dimid - 1;
        }
    }
    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist2); dimid++) {
        if (dimid < col2) {
            hist2dim_to_resultdim[dimid] = MHIST_NUM_DIMS(hist1) + dimid - 1;
        } else if (dimid == col2) {
            hist2dim_to_resultdim[dimid] = join_dimid;
        } else {
            hist2dim_to_resultdim[dimid] = MHIST_NUM_DIMS(hist1) + dimid - 2;
        }
    }
             

    /* Build up a list of the result's bucket dimensions. */
    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist1); dimid++) {
        if (dimid != col1) {
            new_num_bkts *= fghist_num_bins_along_dimension(hist1,dimid);
        }
        bucket_ranges[hist1dim_to_resultdim[dimid]] = 
                fghist_range_along_dimension(hist1, dimid);
        bucket_widths[hist1dim_to_resultdim[dimid]] = 
                fghist_stepsize_along_dimension(hist1, dimid);
    }
    for (dimid = 0; dimid < MHIST_NUM_DIMS(hist2); dimid++) {
        if (dimid != col2) {
            new_num_bkts *= fghist_num_bins_along_dimension(hist2,dimid);
        }
        bucket_ranges[hist2dim_to_resultdim[dimid]] = 
            fghist_range_along_dimension(hist2, dimid);
        bucket_widths[hist2dim_to_resultdim[dimid]] = 
            fghist_stepsize_along_dimension(hist2, dimid);
    }

    /* Fix up the join dimension; the two histograms may not overlap 
     * completely. */
    bucket_ranges[join_dimid].min_val = lowest_join_val;
    bucket_ranges[join_dimid].max_val = highest_join_val;
    bucket_widths[join_dimid] = joindim_stepsize;


    /* Create an empty result and initialize its dimension information. */
    {
        int nameid = -1;
        int dest_nameid = -1;
        int nbkts = -1;

        result = create_mhist(new_num_bkts, new_num_dims);

        dest_nameid = 0;
        for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist1); nameid++) {
            /* Figure out what dimension this name represents. */
            MHIST_DIM_NAME_INFO(result,dest_nameid)
                    = MHIST_DIM_NAME_INFO(hist1,nameid);
            MHIST_SET_NAME_DIM(result,dest_nameid,
                hist1dim_to_resultdim[MHIST_NAME_DIM(hist1,nameid)]); 
            dest_nameid++;
        }
        for (nameid = 0; nameid < MHIST_NUM_DIM_NAMES(hist2); nameid++) {
            dimid = MHIST_NAME_DIM(hist2,nameid);
            MHIST_DIM_NAME_INFO(result,dest_nameid)
                    = MHIST_DIM_NAME_INFO(hist2,nameid);
            MHIST_SET_NAME_DIM(result,dest_nameid,
                hist2dim_to_resultdim[MHIST_NAME_DIM(hist2,nameid)]); 
            dest_nameid++;
        }

        /* We don't calculate this from the inputs, because each one could
         * have multiple names for the join dimension. */
        MHIST_SET_NUM_DIM_NAMES(result, dest_nameid);

        /* SPECIAL CASE: Empty result */
        if (0 == new_num_bkts) {
            MHIST_SET_NUM_BUCKETS_USED(result, 0);
            goto cleanup;
        }
        /* END SPECIAL CASE */
        else
        {
            int *itr = palloc(new_num_dims * sizeof(*itr));
                /* Freed at bottom of scope */

            for (dimid = 0; dimid < new_num_dims; dimid++) {
                itr[dimid] = 0;
            }


            nbkts = create_reg_buckets(result, bucket_ranges, new_num_dims, 0,
                               bucket_widths, 0, itr);
            Assert(nbkts == new_num_bkts);
            MHIST_SET_NUM_BUCKETS_USED(result, nbkts);

     
            pfree(itr);
        }
    }


    /* Now we can do the join.  Algorithm is:
     * foreach bucket B in result do
     *      B1 <- bucket in hist1 that matches B 
     *      B2 <- bucket in hist2 that matches B
     *
     *      adjust B's count.
     *  end
     */
    {
        int bucketid = -1;
        int hist1tup[MHIST_MAX_DIMS];
        int hist2tup[MHIST_MAX_DIMS];
        mhist_bucket *hist1bkt = NULL;
        mhist_bucket *hist2bkt = NULL;
        mhist_bucket *resultbkt = NULL;

#ifdef FRED_MHIST_FK_HACK
        /* To facilitate our FK-join calculations, precompute the total
         * counts for hist1 (the relation with the key) for each row of buckets
         * along the key dimension. */
        double *key_bkt_totals = palloc(
                fghist_num_bins_along_dimension(hist1,col1) 
                * sizeof(double));
            /* Freed at bottom of scope. */
        int i;
        for (i = 0; i < joindim_numsteps; i++) {
            key_bkt_totals[i] = 0.0;
        }

        for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(hist1);
                bucketid++) 
        {
            int ix = -1;

            hist1bkt = MHIST_BUCKET_PTR(hist1, bucketid);

            /* What is this bucket's index along the join dimension? */ 
            ix = fghist_binid_along_dimension_safe(hist1, col1, 
                    MHIST_BUCKET_DIMINFO_PTR(hist1bkt,col1)->min_val);

            Assert(ix >= 0 && ix < fghist_num_bins_along_dimension(hist1,col1));
  
            key_bkt_totals[ix] += hist1bkt->count; 
        }
#endif /* FRED_MHIST_FK_HACK */

        for (bucketid = 0; bucketid < MHIST_NUM_BUCKETS_USED(result);
                bucketid++) 
        {
            /* We find the corresponding buckets in hist1 and hist2 by
             * constructing points we know to be in the buckets and using 
             * get_mhist_bucket_for_tuple() to map those points to buckets.
             */
            for (dimid = 0; dimid < MHIST_NUM_DIMS(hist1); dimid++) {
                mhist_dim_info *di
                    = MHIST_DIMINFO_PTR(result,bucketid, 
                            hist1dim_to_resultdim[dimid]);
                hist1tup[dimid] = di->min_val;
                    /* The buckets hold every x s.t. min <= x <= max */
            }
            
            for (dimid = 0; dimid < MHIST_NUM_DIMS(hist2); dimid++) {
                mhist_dim_info *di
                    = MHIST_DIMINFO_PTR(result,bucketid, 
                            hist2dim_to_resultdim[dimid]);
                hist2tup[dimid] = di->min_val;
                    /* The buckets hold every x s.t. min <= x <= max */
            }

            hist1bkt = MHIST_BUCKET_PTR(hist1,
                            get_mhist_bucket_for_tuple(hist1,hist1tup,
                            MHIST_NUM_DIMS(hist1)));
            hist2bkt = MHIST_BUCKET_PTR(hist2,
                            get_mhist_bucket_for_tuple(hist2,hist2tup,
                            MHIST_NUM_DIMS(hist2)));

            resultbkt = MHIST_BUCKET_PTR(result,bucketid); 

            /* Now we're finally ready to do the computation of the number of
             * results. */
#ifdef FRED_MHIST_FK_HACK
            /* For now, assume a foreign-key join with the key in hist1 and the
             * foreign key in hist2. */
            int ix = fghist_binid_along_dimension_safe(hist1, col1, 
                    MHIST_BUCKET_DIMINFO_PTR(hist1bkt,col1)->min_val);

            /* All the values from the hist2 bucket were sprayed out into the
             * buckets that match on this join dimension. */
            resultbkt->count = hist2bkt->count * 
                (hist1bkt->count / key_bkt_totals[ix]);
#else
            /* Calculation based on that in mhist_equality_select_in_place() */
            resultbkt->count = 
                    (double)hist1bkt->count * (double)hist2bkt->count 
                        / (double)joindim_stepsize;
#endif /* FRED_MHIST_FK_HACK */

        }

#ifdef FRED_MHIST_FK_HACK
        pfree(key_bkt_totals);
#endif /* FRED_MHIST_FK_HACK */
    }
        
cleanup:

    pfree(bucket_ranges);
    pfree(bucket_widths);

    MHIST_SET_IS_FGHIST(result,true);

    return result;
}

/* FUNCTION fghist_stepsize_along_dimension
 * POSTCONDITIONS: Computes the width of bins along the indicated dimension
 * for this fixed-grid histogram.
 */
int fghist_stepsize_along_dimension(mhist *fghist, int dimid) {
    mhist_dim_info *first_di = MHIST_DIMINFO_PTR(fghist, 0, dimid);

    return (first_di->max_val - first_di->min_val + 1);
            /* Plus 1 is because of our funky integer bucket boundaries. */
}

/* FUNCTION fghist_range_along_dimension
 * POSTCONDITIONS: Computes the total range along the indicated dimension of
 * this fixed-grid histogram.
 */
mhist_dim_info fghist_range_along_dimension(mhist *fghist, int dimid) 
{
    mhist_dim_info *first_di;
    mhist_dim_info *last_di;
    mhist_dim_info result;

     Assert(dimid >= 0 && dimid < MHIST_NUM_DIMS(fghist));
    Assert(MHIST_IS_FGHIST(fghist));

    first_di = MHIST_DIMINFO_PTR(fghist, 0, dimid);
    last_di = 
        MHIST_DIMINFO_PTR(fghist, MHIST_NUM_BUCKETS_USED(fghist)-1, dimid);
    
    result.min_val = first_di->min_val;
    result.max_val = last_di->max_val;
    result.accept_below_min = result.accept_above_max = true;
    result.num_vals = 0;

    return result;

}



/* FUNCTION fghist_num_bins_along_dimension
 * POSTCONDITIONS: Computes the number of bins along the indicated dimension
 * for this fixed-grid histogram.
 */
int fghist_num_bins_along_dimension(mhist *fghist, int dimid) {

    mhist_dim_info *first_di = MHIST_DIMINFO_PTR(fghist, 0, dimid);
    mhist_dim_info *last_di = 
        MHIST_DIMINFO_PTR(fghist, MHIST_NUM_BUCKETS_USED(fghist)-1, dimid);

    int tot_range = (last_di->max_val - first_di->min_val + 1);
            /* Plus 1 is because of our funky integer bucket boundaries. */
    int stepsize = fghist_stepsize_along_dimension(fghist,dimid);

    Assert(dimid >= 0 && dimid < MHIST_NUM_DIMS(fghist));
    Assert(MHIST_IS_FGHIST(fghist));
    
    Assert(0 == tot_range % stepsize);

    return tot_range / stepsize;

}

/* FUNCTION fghist_binid_along_dimension
 * POSTCONDITIONS: Computes the binid of the indicated value on the indicated
 * dimension of this fixed-grid histogram. 
 */
int fghist_binid_along_dimension(mhist *fghist, int dimid, int val) {

    mhist_dim_info *first_di = MHIST_DIMINFO_PTR(fghist, 0, dimid);
    mhist_dim_info *last_di = 
        MHIST_DIMINFO_PTR(fghist, MHIST_NUM_BUCKETS_USED(fghist)-1, dimid);
    int bin_no;

    Assert(dimid >= 0 && dimid < MHIST_NUM_DIMS(fghist));
    Assert(MHIST_IS_FGHIST(fghist));

    /* Handle values outside the histogram's range. */
    if (val < first_di->min_val) val = first_di->min_val;
    if (val > last_di->max_val) val = last_di->max_val;

    /* Calculate the bin number for this tuple. */
    bin_no = (val - first_di->min_val) 
                / (first_di->max_val - first_di->min_val + 1);

    return bin_no;
}

/* A version of fghist_binid_along_dimension that dies if passed a value past
 * the edge of the histogram's range.
 */
int fghist_binid_along_dimension_safe(mhist *fghist, int dimid, int val) {

    mhist_dim_info *first_di = MHIST_DIMINFO_PTR(fghist, 0, dimid);
    int bin_no;

    Assert(dimid >= 0 && dimid < MHIST_NUM_DIMS(fghist));
    Assert(MHIST_IS_FGHIST(fghist));

    /* Calculate the bin number for this tuple. */
    Assert(val >= first_di->min_val); 
    bin_no = (val - first_di->min_val) 
                / (first_di->max_val - first_di->min_val + 1);

    return bin_no;
}


/* FUNCTION mhist_approx_empty_bkts
 * POSTCONDITIONS: Returns approximately how many buckets are empty.
 */
int mhist_approx_empty_bkts(mhist *hist) {
    int count = 0;
    int bktid = -1;
    int BKT_STRIDE = 11;
    int nbkts = MHIST_NUM_BUCKETS_USED(hist);

    /* We look at only some of the buckets. */
    for (bktid = 0; bktid < nbkts; bktid += BKT_STRIDE) {
        if (0.0 == MHIST_BUCKET_PTR(hist, bktid)->count) {
            count++;
        }
    }

    /* Compsensate for sampling. */
    count *= BKT_STRIDE;
    return count;
}

