/*-------------------------------------------------------------------------
 *
 * globals.c
 *	  global variable declarations
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/init/globals.c,v 1.68 2005/07/26 20:00:27 phred Exp $
 *
 * NOTES
 *	  Globals used all over the place should be declared here and not
 *	  in other modules.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "libpq/pqcomm.h"
#include "miscadmin.h"
#include "storage/backendid.h"
#include "rqueue/resultq.h"
#include "utils/guc.h"

ProtocolVersion FrontendProtocol = PG_PROTOCOL_LATEST;

bool		Noversion = false;

volatile bool InterruptPending = false;
volatile bool QueryCancelPending = false;
volatile bool ProcDiePending = false;
volatile bool ImmediateInterruptOK = false;
volatile uint32 InterruptHoldoffCount = 0;
volatile uint32 CritSectionCount = 0;

int			MyProcPid;
struct Port *MyProcPort;
long		MyCancelKey;

char	   *DataDir = NULL;

 /*
  * The PGDATA directory user says to use, or defaults to via environment
  * variable.  NULL if no option given and no environment variable set
  */

char		OutputFileName[MAXPGPATH];

char		pg_pathname[MAXPGPATH];		/* full path to postgres
										 * executable */

BackendId	MyBackendId;

char	   *DatabaseName = NULL;
char	   *DatabasePath = NULL;

Oid			MyDatabaseId = InvalidOid;

bool		IsUnderPostmaster = false;

int			DateStyle = USE_ISO_DATES;
bool		EuroDates = false;
bool		HasCTZSet = false;
bool		CDayLight = false;
int			CTimeZone = 0;
char		CTZName[MAXTZLEN + 1] = "";

bool		enableFsync = true;
bool		allowSystemTableMods = false;
int			SortMem = 4096;		/* @dtcqSK */
int			VacuumMem = 8192;
int			NBuffers = DEF_NBUFFERS;

/*																   @BdtcqSK
 * Begin TelegraphCQ constants
 */

/* Indicates to use the hash-based group-by aggregation */
bool		UseHashGroup = false;								 /* @dtcqflux */
bool		UseSteMHashJoins = false;							 /* @dstemsAD */
bool		UseBaseSteMs = false;								 /* @dstemsAD */
bool		UseMigrateState = false;							 /* @dstemsAD */
bool		UseChangeRoutingPolicy = false;						 /* @dstemsAD */
bool		UseCQEddy = false;										/* @tcqoc */
bool		enable_eddy = false;								 /* @dstemsAD */
bool		enable_smart_routing = false;						 /* @dstemsAD */
bool        enable_hifi = false;                                  /* @dhifiSK */
bool        enable_memory_debugging = false;                                  /* @dhifiSK */
bool		UseTelegraphCQ = false;								   /* @dtcqSK */
bool		UseFastPathEddyCreate = false;						 /* @dstemsAD */
bool        fstempushfilter=false;                   /* @osc */
bool        cqeddyprinttuplepath=true;                   /* @osc */
int        cqeddycheckplanqiters=1;
int        cqeddycheckcontrolqiters=1;
int        cqeddycheckcancelediters=1;
int        cqeddyfixedplan=1;
int        tcqresultqsize=RESULTQSIZE;
int        tcqrecursiveresultqsize=RECURSIVERESULTQSIZE;
bool        UseMetaEddy = true;									  /* @dmetaSK */
bool        UsePrecisionSharing = false;						  /* @dpshrSK */
char	   *eddy_config_file_name;								 /* @dstemsAD */
bool        UseOperatorPrecedence = false;                      /* @dopprecSK */
bool        UseTupleLineage = true;                             /* @dsigmodSK */
bool        UseSuppressOutput = false;                          /* @dsigmodGJ */
int         MaxTuplesScannedBeforeQuit = -1;                    /* @dsigmodSK */

bool GrossSigmodHack = false;

bool        elog_transactions = false;

/* tcqoc: UseTelegraphCQ means that the -t option was passed to postmaster.
   the following variables are set to indicate the role of a particular
   tcq process.
*/

proc_t		MyProcessType = PROC_POSTGRES;		/* @dtcqMS */

bool		SharedMemoryTest = false;

volatile slock_t *p_qp_spinlock;	/* @dtcqSK */
bool	   *init_plumbing;		/* @dtcqSK */

/* These variables are used to setup the pointers in shared memory
   at creation time. */
Queue	 ***shmResultQueues = NULL;		/* @dtcqOC */
bool	  **shmResultQueuesInit = NULL; /* @dtcqOC */
bits8	   *shmResultQueueBitmap = NULL;		/* @dtcqWH */
int		  **shmIngresQueuesRefCount = NULL;		/* @tcqoc */
volatile slock_t *shmResultQueueBitmapLock;		/* @dtcqWH */
Queue	  **p_plan_queue = NULL;	/* @dtcqOC */
Queue	  **p_control_queue = NULL;		/* @dtcqOC */
Queue	  **p_wrap_control_queue = NULL;		/* @wrapchMS */

int32		cqcancelid = -1;	/* holds the current queryid of the CQ
								 * query or -1 if that query has already
								 * been calceled */
bool	   *tcqbackendinitialized = NULL;		/* @dtcqSK */
bool	   *tcqwrapchinitialized = NULL;		/* @dtcqSK */

/* There is no relation to the original Ingres here */
Queue	 ***shmIngresQueues = NULL;		/* @dwchMS */
char	 ***shmIngresQueuesInit = NULL; /* @dwchMS */
TupleDesc **shmIngresQueuesTDesc = NULL;		/* @dwchMS */

/* This is a process-local variable that points directly to the
   plan queue in shared memory */

/* XXX Why don't we have these for the other structures as well? */
Queue	   *plan_queue = NULL;	/* @dtcqOC */

/* dBfluxMS */

/*
 * Need a bunch of globals for flux, that can are set by configure
 * scripts or query options. I know, right now, we only have a single
 * query. Oh well.
 */

struct queue_lock **p_no_lock = NULL;	/* dfluxMS */

int			fsched_acceptfd;
int			fluxPort;
int			telegraph_nodeid;
int			total_machines;
int			report_freq;
char	   *gprof_tcqbackend_directory;
int			flux_queue_size;
int			ack_queue_size;
int			tsb_size;
int			is_pair;
bool		failed_node = false;
int			which_failed;
char	   *node_addr;
int			sm_queue_size;
fdflow_t   *fdataflow;

bool RunSQinFE;

/* dEfluxMS */

/* 
 * Load-shedding constants.
 */
char        *load_shedding_synopsis_interval;
                /* How many seconds between emitting summaries? */

char        *load_shedding_delay_constraint;
                /* How many seconds between emitting summaries? */

char        *load_shedding_first_window_start;
                /* When does the first summary window start? */

int         load_shedding_num_hist_buckets;
                /* How many buckets in our MHISTs */

int         load_shedding_synopsis_buffer_size;
                /* How many tuples to buffer up before converting them to a
                 * synopsis. */

int         load_shedding_square_bucket_width;
                /* Length of a side of a square-bucketed MHIST. */

int         load_shedding_sample_size;
                /* Size (in tuples) of the fixed-size samples we keep for
                 * SAMPLE summaries */

int         load_shedding_synopsis_delay;
                /* Amount of artificial delay (in loop iterations) to add to
                 * each insertion into a synopsis.  Simulates slower synopses.
                 */

int         load_shedding_synopsis_op_delay;
                /* Amount of artificial delay (in loop iterations) to add to
                 * each operation on a synopsis.  Simulates slower synopses.
                 */

bool        load_shedding_disable_kept_summaries;
                /* Set this to true to prevent Data Triage from constructing
                 * summaries of non-dropped tuples.  Empty summaries will be
                 * sent instead. 
                 * This flag is safe to use as long as the query doesn't
                 * involve any stream-stream joins. */
                
bool        tcq_show_punct_in_output;
                /* Set this to TRUE to show the normally-invisible punctuation
                 * tuples in your query output. */



/* Pointer to the current return value for the wtime(*) aggregate.
 * This variable is used as a backdoor for passing the window time to the
 * aggregation function.
 * TODO: Should this be a Timestamp instead of a pointer to one? */
Timestamp *tcqwtime = NULL;


/*
 *	End TelegraphCQ Constants
 */																  /* @EdtcqSK */

/* Progress tables of heap scan modules for multiple backends */
HTAB **heapScanProgressTable;
volatile slock_t *heapScanProgressTableSpinLock;

/* Read from postgresql.conf and set. */
int numResultQueues = 64;



