
/*
 * $RCSfile: histogram.h,v $
 * $Revision: 1.27 $
 * $Date: 2005/12/14 20:49:24 $
*/

/*******************************************************************************
 * FILENAME: histogram.h
 * AUTHOR: Fred Reiss <phred@cs.berkeley.edu>
 * DESCRIPTION: Data types and macros for histogram support.
 ******************************************************************************/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/* Stuff necessary for compiling from outside TCQ. */
#include <postgres.h>
#include <access/htup.h>

/* How many dimensions we allow for a histogram.  Controls header size. */
#define MHIST_MAX_DIMS 8

/* How many column names we leave room for in the MHIST header. */
#define MHIST_MAX_COL_NAMES 8

/* How many characters we set aside to store a column name. */
#define MHIST_COL_NAME_LEN 32

#define MHIST_NOT_A_DIMID -1


/*******************************************************************************
 * MHIST HISTOGRAMS
 ******************************************************************************/

/* Information about the original columns used to construct a dimension of an
 * MHIST. */
typedef struct mhist_dim_name {
    int dim_no;

    char name[MHIST_COL_NAME_LEN];
} mhist_dim_name;

/* Header for the structure that holds an MHIST. */
typedef struct mhist_hdr
{
	int32 length;
	/* How many bytes (including this field) long the histogram data
	 * structure is. */

	int numDims;

    int numDimNames;
	mhist_dim_name dimNames[MHIST_MAX_COL_NAMES];
        /* Human-readable names for the dimensions; may contain more than one
         * name for a dimension if it was created through an equijoin. */

	int numBucketsUsed;
    int numBucketsAllocated;
        /* The MHIST may have space for more buckets than it contains. */

    /* Flags */
    bool isFullFGHist;
        /* Set to TRUE if this histogram is a fixed-grid histogram with all its
         * buckets intact.  We can take advantage of this to speed adding 
         * tuples to the histogram. */
} mhist_hdr;

#define MHIST_HDRSZ (sizeof(mhist_hdr))

/* Size of the name buffer in the header.*/
#define MHIST_NAMESZ (sizeof(mhist_dim_name) * MHIST_MAX_COL_NAMES)

/* Information about a single dimension in an mhist. */
typedef struct mhist_dim_info {
    int min_val;
    int max_val;
        /* Minimum and maximum values along this dimension. */ 
        /* By convention, the bucket contains values x, such that
         *      min_val <= x <= max_val.  */

       /* Does this bucket accept values below the minimum or maximum? */
    bool accept_below_min;
    bool accept_above_max;
 
    int num_vals;
        /* Number of distinct values between these points. */
} mhist_dim_info;

/* The format of an mhist bucket. */
typedef struct mhist_bucket {
    double count;
        /* How many points in this bucket? */

    mhist_dim_info diminfo[1];
        /* Size of this array depends on the number of dimensions in the
         * histogram. */
} mhist_bucket;

typedef struct mhist
{
    mhist_hdr hdr;

    mhist_bucket buckets[1];
        /* Actual size of this array may vary. */
} mhist;

/* This struct acts as a placeholder for mhist_to_table() between calls. */
typedef struct mhist_to_table_state {
    int current_bucket;
    int num_buckets;
} mhist_to_table_state;

typedef struct square_mhist_stream_state {
    mhist *hist;
    int bucket_widths[256];
        /* How many values between bucket boundaries along any dimension. */
} square_mhist_stream_state;

/*******************************************************************************
 * MACROS
 *
 * For dealing with MHISTs.
 ******************************************************************************/

/* Macros for fetching header fields. */
#define MHIST_NUM_BUCKETS_USED(hist) ((hist)->hdr.numBucketsUsed)
#define MHIST_NUM_BUCKETS_ALLOC(hist) ((hist)->hdr.numBucketsAllocated)
#define MHIST_NUM_DIMS(hist)    ((hist)->hdr.numDims)
#define MHIST_IS_FGHIST(hist)           ((hist)->hdr.isFullFGHist)
#define MHIST_SET_IS_FGHIST(hist,val)   ((hist)->hdr.isFullFGHist = (val))

#define MHIST_NUM_DIM_NAMES(hist) ((hist)->hdr.numDimNames)
#define MHIST_SET_NUM_DIM_NAMES(hist,val) ((hist)->hdr.numDimNames = (val))
#define MHIST_DIM_NAME_INFO(hist,index) ((hist)->hdr.dimNames[(index)])
#define MHIST_DIM_NAME(hist,index) ((hist)->hdr.dimNames[(index)].name)
#define MHIST_NAME_DIM(hist,index) ((hist)->hdr.dimNames[(index)].dim_no)
#define MHIST_SET_NAME_DIM(hist,index,val) \
    ((hist)->hdr.dimNames[(index)].dim_no = (val))


#define MHIST_SET_NUM_BUCKETS_USED(hist,num) \
    ((hist)->hdr.numBucketsUsed = (num))

/* Macro that calculates the length of a single bucket. */
#define MHIST_BUCKET_LEN(numdims) \
    (sizeof(double) + (numdims) * sizeof(mhist_dim_info))

/* Macro that describes the true length of a histogram struct. */
#define MHIST_SIZE(numbuckets,numdims) \
    (MHIST_HDRSZ + ((numbuckets) * MHIST_BUCKET_LEN(numdims)))

/* Macro for computing offsets into an mhist's buckets array. */
/* Turned into a function so that we can call it from the debugger. */
extern mhist_bucket * MHIST_BUCKET_PTR(const mhist *hist, int bucketnum);
/*
#define MHIST_BUCKET_PTR(hist,bucketnum) \
    (mhist_bucket*)(((char*)(hist)->buckets) + \
        ((bucketnum) * (MHIST_BUCKET_LEN(MHIST_NUM_DIMS(hist)))))
        */

#define MHIST_BUCKET_DIMINFO_PTR(bucket_ptr,dimid) \
    (&((bucket_ptr)->diminfo[(dimid)]))

#define MHIST_DIMINFO_PTR(hist,bucketnum,dimid) \
    (&((MHIST_BUCKET_PTR(hist,bucketnum))->diminfo[(dimid)]))


/*******************************************************************************
 * EXPORTED FUNCTIONS 
 *
 * Defined in histogram.c
 ******************************************************************************/

/* 
 * Prototypes for built-in functions in histogram.c.
 */
extern Datum mhist_in(PG_FUNCTION_ARGS);
extern Datum square_mhist_in(PG_FUNCTION_ARGS);
extern Datum mhist_from_table(PG_FUNCTION_ARGS);
extern Datum mhist_out(PG_FUNCTION_ARGS);
extern Datum mhist_to_table(PG_FUNCTION_ARGS);
extern Datum mhist_equijoin(PG_FUNCTION_ARGS);
extern Datum mhist_crossprod(PG_FUNCTION_ARGS);
extern Datum mhist_eq_selection(PG_FUNCTION_ARGS);
extern Datum mhist_range_selection(PG_FUNCTION_ARGS);
extern Datum mhist_union(PG_FUNCTION_ARGS);
extern Datum mhist_slice(PG_FUNCTION_ARGS);
extern Datum mhist_project(PG_FUNCTION_ARGS);
extern Datum mhist_project_many(PG_FUNCTION_ARGS);

extern Datum get_cached_mhist(PG_FUNCTION_ARGS);

/*
 * Protoypes for other exported functions.
 */
extern bool mhist_can_summarize_tuple(TupleDesc desc); 

extern int get_timestamp_col(TupleDesc desc);

extern int get_num_non_timestamp_attrs(TupleDesc desc);

extern int get_attr_id_skipping_ts(int id, TupleDesc desc);

extern int64 get_int_field_val(HeapTuple tup, TupleDesc desc, int fnum,
        bool skip_ts);

extern char *mhist_to_str(mhist *hist);

extern bool add_bucket_to_mhist(mhist* hist, int* minvals, int* maxvals, double count);

extern int get_timestamp_col(TupleDesc desc);

extern mhist * create_mhist(int num_buckets, int num_dims);
extern void mhist_load_buckets(HeapTuple *giant_tup_array, TupleDesc desc,
        int ntuples, mhist *hist);

/* Functions used by shedding.c (common routines for load shedding) */
extern void tdesc_to_mhist_diminfo(mhist *hist, TupleDesc desc);
extern void mhist_create_buckets(mhist *hist, HeapTuple *tuparr, int ntups, 
        TupleDesc desc);
extern mhist * create_reg_mhist(HeapTuple *tups, int ntups, 
        TupleDesc desc, int *bucket_widths);
extern void remove_empty_mhist_buckets(mhist *hist);




/*
 * fmgr interface macros
 *
 * We need to detoast here because these histograms could be larger than a
 * page (See http://www.postgresql.org/docs/7.4/interactive/xtypes.html).
 */
#define DatumGetMhistP(X)	((mhist *) DatumGetPointer(X))
#define MhistPGetDatum(X)	PointerGetDatum(X)
#define PG_GETARG_MHIST_P(n)	DatumGetMhistP(PG_DETOAST_DATUM(PG_GETARG_DATUM(n)))
#define PG_RETURN_MHIST_P(x)    return MhistPGetDatum(x)




#endif /* HISTOGRAM_H */
