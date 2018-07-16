
/*-------------------------------------------------------------------------
 *
 * wrapch.c
 *
 *	  Wrapper clearing house. A separate process that services active
 *	  connections.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *           $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/wrapch.c,v 1.95 2006/02/06 01:19:46 phred Exp $
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"

#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sched.h>

#include "miscadmin.h"
#include "nodes/plannodes.h"
#include "tcop/dest.h"
#include "rqueue/resultq.h"
#include "rqueue/controlq.h"
#include "rqueue/rqdest.h"
#include "rqueue/planq.h"
#include "storage/ipc.h"
#include "utils/inval.h"
#include "utils/memutils.h"
#include "storage/spin.h"
#include "tcop/tcopprot.h"
#include "tcop/pquery.h"
#include "telegraphcq/punct.h"
#include "telegraphcq/wrapch.h"
#include "executor/executor.h"
#include "telegraphcq/telegraphcq.h"
#include "parser/parser.h"
#include "parser/parse_node.h"
#include "parser/parse_func.h"
#include "parser/parse_expr.h"
#include "parser/parse_wrapper.h"
#include "catalog/namespace.h"
#include "utils/fcache.h"
#include "fmgr.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/relcache.h"
#include "utils/ps_status.h"
#include "libpq/pqsignal.h"
#include "access/printtup.h"
#include "access/xact.h"
#include "utils/catcache.h"

#include "nodes/memnodes.h"

#include <stdio.h>				/* These are included for the networking */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include "access/heapam.h"
#include "telegraphcq/tcq_usleep.h"

#include "telegraphcq/shedding.h"
	/* Synopsis data structures. */

#include "catalog/pg_type.h"
	/* Type OID constants. */

#include "nodes/makefuncs.h"
	/* For makeConst() function. */

#include "parser/parse_type.h"
	/* For LookupTypeName() function. */

#include "telegraphcq/window.h" /* For timestamp arithmetic */


#define TRIAGE_DRAIN_DELAY 10000
								/* How many microseconds we wait between
								 * calls to serviceTriageQueue() when trying
								 * to drain a triage queue so that we can
								 * remove it. */

#define MAXPOLLTIMEOUT 100
								/* Maximum timeout in milliseconds for
								 * poll().  We have this maximum because our
								 * call to poll() currently ignores the
								 * wrapper clearinghouse control queue. */


#define FLUSHINTERVAL 100000
								/* how often archived stream buffers should
								 * be flushed, in microseconds. */
#define BUFFERCHECKINTERVAL 500	/* how often wrappers should be checked for
								 * buffered data (in poll wakeup itervals) */

/*
#define NUM_MHIST_BUCKETS 3
#define NUM_TUPS_TO_BUFFER 5
*/
								/* Parameters for building MHISTs on streams
								 * of tuples. */

#define TRIAGEQLEN 1000
	/* Length of the lossy priority queue that controls dropping of tuples.
	 * Most of the time, these queues will not fill up, as the delay
	 * constraint will kick in first. */


/* Return codes for AIORead */
#define AIOREAD_OK 0
#define AIOREAD_NO_DATA 1
#define AIOREAD_METADATA_CHANGED 2

/* Uncomment to have the main loop periodically print out how many tuples it 
 * processes. */

/* Convenience debugging function to get the string representation of a
 * timestamp. Leaks memory! */
static char * ts_cstr(Timestamp ts) {
    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out, TimestampGetDatum(ts)));
}

/*#define WRAPCH_PRINT_STATUS*/

/*******************************************************************************
 * GLOBALS
 ******************************************************************************/

/* Tuple descriptor for a stream of count summaries; used for the "null"
 * summary option. */
TupleDesc g_dummy_summary_desc = NULL;

/* Constant "null" timeval. */
struct timeval WRAPCH_NOT_A_TIMEVAL = {0, 0};

bool g_disableNagle = true;

/* connection data structures */

aioconn **conn_list;			/* Connection list (array) */
struct pollfd *pfd_list;		/* Array of pollfd's passed into poll */
int next_free = 0;

/* port information */

int acceptPort = DEF_WRAPPER_PORT;
int acceptfd = -1;
char *telegraphcqwchport = NULL;	/* declaration of WCH command line option 
									 */
Oid SafeGetStreamOid(List * streamname);

#ifdef WRAPCH_PRINT_STATUS

/* A count of how many tuples we've processed over all wrappers. */
int64 g_total_tuples_processed = 0LL;
#endif /* WRAPCH_PRINT_STATUS */

#define ARGV_SIZE	64

#define WRAPCH_DEBUG 1

MemoryContext oldcontext;

MemoryContext wch_tmpcontext = NULL;


StreamSynopsisGen wcs_synopsis;
	/* Factory for create wavelet-based histograms. Initialized in
	 * initSynopsesFactories(). */

StreamSynopsisGen mhist_synopsis;
	/* Factory for create MHIST summaries. Initialized in
	 * initSynopsesFactories(). */

StreamSynopsisGen square_mhist_synopsis;
	/* Factory for create MHIST summaries with square buckets. Initialized
	 * in initSynopsesFactories(). */

StreamSynopsisGen sample_synopsis;
    /* Factory that creates reservoir samples.  Initialized in
     * initSynopsesFactories(). */

StreamSynopsisGen count_synopsis;
	/* Stub factory that doesn't create any summaries. Initialized in 
	 * initSynopsesFactories(). */

Interval *g_synopsis_interval = NULL;
	/* An INTERVAL constant containing the normal interval between creations
	 * of signal/noise synopses. */

Interval *g_delay_constraint_interval = NULL;
	/* An INTERVAL containing the maximum amount of time that the wrapper
	 * clearinghouse is allowed to buffer a tuple before sending it to the
	 * query engine. */


int g_num_hist_buckets = -1;
	/* Mirrors the load_shedding_num_hist_buckets parameter in
	 * postgresql.conf. Determines how many buckets will be in the
	 * histograms we build as synopses. */

int g_synopsis_buf_size = -1;
	/* Mirrors the load_shedding_synopsis_buffer_size parameter in
	 * postgresql.conf. Determines how many tuples we buffer up before
	 * starting the job of synopsizing them. */

int g_square_bucket_width = -1;
	/* Mirrors the load_shedding_square_bucket_width parameter in
	 * postgresql.conf. Determines the length of a single side of a square
	 * histogram bucket. */

int g_sample_size = -1;
	/* Mirrors the load_shedding_sample_size parameter in
	 * postgresql.conf. */

int g_synopsis_delay = -1;
	 /* Mirrors the load_shedding_synopsis_delay parameter in
	  * postgresql.conf. Determines how much of an artificial delay we add
	  * to the process of inserting into a synopsis.  This delay simulates
	  * slower synopsis data structures. */

int g_synopsis_op_delay = -1;
	 /* Mirrors the load_shedding_synopsis_op_delay parameter in
	  * postgresql.conf. This is the delay added to */

Timestamp g_triage_summary_window_start = -DBL_MAX /*DT_NOBEGIN*/;
	/* All the triage queues base the start of their summary windows off this
	 * timestamp, so as to stay in sync with each other. Triage queues
	 * receive a pointer to this global. This timestamp is set on the first
	 * one-second boundary BEFORE the first tuple sent to the wrapper
	 * clearinghouse, to ensure that every tuple is in a window. */



bool synopses_initialized = false;
	/* Set to true in initSynopsesFactories */

/* new wrapper negotiation, setup, and cleanup */

static void freeWrapperstate(wrapperstate * w);
void initializeWrapperstateCommon(wrapperstate * w);
static bool readStreamName(aioconn * a, int sockfd);
static bool getWrapper(aioconn * a);
static TupleDesc lookupStream(aioconn * a);
static bool setupUnarchivedStream(aioconn * a);
static bool setupArchivedStream(aioconn * a);
static bool cleanupUnarchivedStream(aioconn * a);
static bool cleanupArchivedStream(aioconn * a);
static bool endUnarchivedStreamScan(int qidx);
void loadWrapperAttributes(aioconn * a);
static bool
setupSynopsisStreams(List * streamname, unarchivedStreamInfo * uinfo);
static void send_end_and_close_q(Queue * q, int index);

/* Main event loop. */
static void wrapch_main_loop(void);

/* adding new connections to the system */
static void doAccept(int acceptfd);
static int acceptSetup(void);
static int AIORead(struct timeval *now, bool allowed_to_sleep);

/* connection management functions */
static aioconn *createConn(void);
static int addConnToList(aioconn * conn);
static int addPushConnToList(int sock);
static int addPullConnToList(aioconn * conn);
static void deleteConn(aioconn * a);
static void deleteConnByIndex(int idx);
static void removeConnFromListByIndex(int idx);

/* pull wrapper setup  */
static void ProcessStreamCommand(Controlqentry * cqe);
static aioconn *SetupPullWrapper(Oid wrapperoid, Oid mapping,
								 Oid streamwrapper);
void WCHHandleErrorMessageForArchivedStream(int streamoid, char *msg);
void *wrapper_oid_append_stream_attr_info(Oid streamoid, Oid wrapperoid,
										  void *info);


/* connection processing functions */

/*static int get_next_timeout(int prev_timeout, bool did_work);*/
static bool processTuple(aioconn * a);
static bool processTupleArchived(aioconn * a);
static bool processTupleUnarchivedDrop(aioconn * a);
static void sendLossSummaries(aioconn * conn);
static bool serviceTriageQueue(aioconn * conn, Timestamp now);
static bool serviceTriageQueues(aioconn ** connlist, int ncons, int maxdeq);
static bool allTriageQueuesEmpty(aioconn ** conlist, int ncons);
static bool processConnection(aioconn *, int);
static void flush_stream(aioconn * a);
static bool processWCHControlQ(void);
void WCHHandleErrorMessage(int con, char *msg);
static void doShutdown(void);
static void setupGlobalConfig(void);
static void initSynopsesFactories(void);
bool enqueue_heaptuple(Queue * q, HeapTuple tup, bool blocking);
bool enqueue_endofstream(Queue * q, bool blocking);
static long setupEventMasks(struct timeval *now);
static bool checkForConnectionErrors(void);
static bool checkForNewConnections(void);

/* Load shedding. */
#if 0
void *drop_initf_stub(void *arg, const TupleDesc desc);
void drop_nextf_stub(void *arg, HeapTuple tup);
void *drop_donef_stub(void *arg);
#endif
Timestamp get_current_timestamp(wrapperstate *w);
bool enqueue_tuple_direct(aioconn * a);

/* from tcop/postgres.c */
extern CommandDest whereToSendOutput;

int
WrapCHMain(int argc, char *argv[])
{
	char usrbuf[ARGV_SIZE];
	int status;
	pid_t pid = getpid();

    /* Initialize our temporary memory context. */
    wch_tmpcontext = AllocSetContextCreate(CurrentMemoryContext,
                            "WCH Temp Context",
                            ALLOCSET_DEFAULT_MINSIZE,
                            ALLOCSET_DEFAULT_INITSIZE,
							ALLOCSET_DEFAULT_MAXSIZE);


	setupGlobalConfig();

	whereToSendOutput = Debug;
	StrNCpy(usrbuf, GetConfigOption("telegraphcquser"), ARGV_SIZE);

	elog(DEBUG1, "WrapCH: Calling postgress main");
	status = PostgresMain(argc, argv, usrbuf);
	elog(DEBUG1, "WrapCH: Done postgress main");

	/* Set the process name so we can debug it later */
	set_ps_display("wrapch");

	PG_SETMASK(&UnBlockSig);	/* Unblock signals */

	MemoryContextSwitchTo(TopMemoryContext);	/* Top context */

	conn_list = (aioconn **) palloc(sizeof(aioconn *) * MAX_CONN);
	memset(conn_list, 0, (sizeof(aioconn *) * MAX_CONN));
	pfd_list = (struct pollfd *) palloc(sizeof(struct pollfd) * MAX_CONN);
	memset(pfd_list, 0, (sizeof(struct pollfd) * MAX_CONN));
	acceptfd = acceptSetup();
	if (acceptfd != -1)
		addPushConnToList(acceptfd);
	else
		doShutdown();


	*tcqwrapchinitialized = TRUE;

	elog(LOG, "Wrapper Clearinghouse running in process %d.", pid);

	wrapch_main_loop();
	elog(FATAL,
		 "wrapch_main_loop() returned in the wrapper claringhouse... this shouldn't happen");


	return status;
}

/* FUNCTION setupGlobalConfig
 * POSTCONDITIONS: Reads in values that were set in postgresql.conf.
 */
static void
setupGlobalConfig(void)
{
	const char *acceptPortStr = GetConfigOption("telegraphcqwchport");


	if (acceptPortStr && strlen(acceptPortStr) > 0)
		acceptPort = atoi(acceptPortStr);
	else
		acceptPort = DEF_WRAPPER_PORT;

	elog(LOG, "Wrapper clearinghouse will listen on port %d", acceptPort);

	/* 
	 * Interval between emitting load-shedding summaries? 
	 *
	 * Take the string in the postgresql.conf value of
	 * "load_shedding_synopsis_interval", and convert to Interval.
	 */
	g_synopsis_interval =
		DatumGetIntervalP(DirectFunctionCall3(interval_in,
											  CStringGetDatum
											  (load_shedding_synopsis_interval),
											  ObjectIdGetDatum(InvalidOid),
											  Int32GetDatum(-1)));

	/* We construct the deadline interval by a similar method. This time we
	 * don't have to box it inside a Const. */
	g_delay_constraint_interval =
		DatumGetIntervalP(DirectFunctionCall3(interval_in,
											  CStringGetDatum
											  (load_shedding_delay_constraint),
											  ObjectIdGetDatum(InvalidOid),
											  Int32GetDatum(-1)));

    elog(LOG, "Wrapper clearinghouse using delay constraint %s",
            DatumGetCString(DirectFunctionCall1(interval_out,
                    IntervalPGetDatum(g_delay_constraint_interval))));


	/* Copy the synopsis parameters from process-global config space. */
	g_num_hist_buckets = load_shedding_num_hist_buckets;
	g_synopsis_buf_size = load_shedding_synopsis_buffer_size;
	g_square_bucket_width = load_shedding_square_bucket_width;
	g_sample_size = load_shedding_sample_size;
	g_synopsis_delay = load_shedding_synopsis_delay;

	if (g_synopsis_buf_size < g_num_hist_buckets) {
        /* We can't use elog(WARNING) here, because PostmasterMain hasn't been
         * called yet. */
		elog(LOG,
			 "wrapch(): Number of buffered tuples < num histogram buckets.");
		elog(LOG,
			 "wrapch(): Your histograms will have fewer buckets than you want.");
	}


    /* Create the (global) tuple descriptor for null summaries. */
}


/* FUNCTION initSynopsesFactories
 * POSTCONDITIONS: Sets up the global factory objects that produce new synopsis
 *      data structures for dropped tuples in streams of the unarchived
 *      drop-and-summarize variety.
 */
static void
initSynopsesFactories(void)
{
	MemoryContext oldcxt = NULL;

	Assert(g_num_hist_buckets > 0);
	Assert(g_synopsis_buf_size > 0);

	/* Data structures allocated here are permanent, so allocate them in a
	 * memory context that won't go away when the current connection does. */
	oldcxt = MemoryContextSwitchTo(TopMemoryContext);
	{							/* BEGIN TOP MEMORY CONTEXT */

		/* First the factory for MHISTs. */
		/*mhist_synopsis.sumtype = SUMT_MHIST;*/
		memset(&(mhist_synopsis.protostate), 0x0, 
                sizeof(mhist_synopsis.protostate));
		mhist_synopsis.protostate.tb.cap = g_synopsis_buf_size;
		mhist_synopsis.protostate.tb.own_tuples = true;
		mhist_synopsis.protostate.ds_state.mhs.num_buckets = 
            g_num_hist_buckets;
		mhist_synopsis.protostate.stype = ST_MHIST;

		/* Then the one for mhists with square buckets. */
		/*square_mhist_synopsis.sumtype = SUMT_MHIST;*/
		memset(&(square_mhist_synopsis.protostate), 0x0, 
                sizeof(square_mhist_synopsis.protostate));
		square_mhist_synopsis.protostate.tb.cap = g_synopsis_buf_size;
		square_mhist_synopsis.protostate.tb.own_tuples = true;
		{
			int i;
			for (i = 0; i < 256; i++) {
				square_mhist_synopsis.protostate.ds_state.shs.bucket_widths[i] 
                    = g_square_bucket_width;
			}
		}
		square_mhist_synopsis.protostate.stype = ST_SQUARE_MHIST;


		/* Then the factory for wavelet coefficient synopses. */
		/*wcs_synopsis.sumtype = SUMT_WAVELET;*/
		memset(&(wcs_synopsis.protostate), 0x0, 
                sizeof(wcs_synopsis.protostate));
		wcs_synopsis.protostate.tb.cap = g_synopsis_buf_size;
		wcs_synopsis.protostate.tb.own_tuples = true;
		wcs_synopsis.protostate.ds_state.ws.maxCount = 256;
		wcs_synopsis.protostate.ds_state.ws.res = 4;
		wcs_synopsis.protostate.ds_state.ws.dimLo = NULL;
		wcs_synopsis.protostate.ds_state.ws.dimHi = NULL;
            /* NOTE: These are overwritten later on with the proper values from
             * the catalog. */

		wcs_synopsis.protostate.stype = ST_WAVELET;

        /* Then the factory that creates samples */
		memset(&(sample_synopsis.protostate), 0x0, 
                sizeof(sample_synopsis.protostate));
		sample_synopsis.protostate.tb.cap = g_synopsis_buf_size;
		sample_synopsis.protostate.tb.own_tuples = false;
        sample_synopsis.protostate.ds_state.ss.max_sample_size = g_sample_size;
        sample_synopsis.protostate.stype = ST_SAMPLE;
            /* Will be initialized depending on the input tuples. */


		/* Then the factory that just keeps counts. */
		/*count_synopsis.sumtype = SUMT_COUNTS;*/
		memset(&(count_synopsis.protostate), 0x0, 
                sizeof(count_synopsis.protostate));
		count_synopsis.protostate.ds_state.cs.count = 0;
		count_synopsis.protostate.stype = ST_COUNT;
		count_synopsis.protostate.tb.cap = g_synopsis_buf_size;
		count_synopsis.protostate.tb.own_tuples = true;


        /* We also initialize the global dummy summary descriptor here,
         * because:
         * 1) This function call is guaranteed to come before any use of
         *      g_dummy_summary_desc
         * 2) We can't do it in setupGlobalConfig(), because the SysCache
         *      hasn't been set up at that point.
         */
        g_dummy_summary_desc = CreateTemplateTupleDesc(4, true);
        TupleDescInitEntry(g_dummy_summary_desc, 
                            1	/* attribute numbers are 1-based! */ ,
                           "num_dropped", INT4OID, -1,	
                           0 /* TODO: Verify this value! */ ,
                           false);

        /* TEMPORARY: TelegraphCQ has trouble joining on timestamps, so we
         * create an auxiliary integer column to join the summary streams
         * together. */
        TupleDescInitEntry(g_dummy_summary_desc, 
                            2 /* attribute numbers are 1-based! */ ,
                           "window_num", INT4OID, -1,	
                           0 /* TODO: Verify this value! */ ,
                           false);


        TupleDescInitEntry(g_dummy_summary_desc, 
                            3 /* attribute numbers are 1-based! */ ,
                           "oldest", TIMESTAMPOID, -1,
                           0 /* TODO: Verify this value! */ ,
                           false);


        TupleDescInitEntry(g_dummy_summary_desc, 
                            4 /* attribute numbers are 1-based! */ ,
                           "newest", TIMESTAMPOID, -1,
                           0 /* TODO: Verify this value! */ ,
                           false);

	}							/* END TOP MEMORY CONTEXT */
	MemoryContextSwitchTo(oldcxt);

	synopses_initialized = true;

}

/* The main loop of the wrapper clearinghouse. */
static void
wrapch_main_loop(void)
{
	int inum = 0;
	int check_control_q_iter = 500;
	struct timeval last_buffer_time;
	struct timeval last_flush_time;
	bool dequeued_nothing_last_time = false;

#ifdef WRAPCH_PRINT_STATUS
	int64 total_elapsed = -1;
	int last_tupcount = -1;
	int this_tupcount = -1;
	struct timeval last_time;
	struct timeval this_time;

	/* Establish a baseline. */
	gettimeofday(&last_time, NULL);
	last_tupcount = g_total_tuples_processed;
#endif /* WRAPCH_PRINT_STATUS */


	/* Establish baseline timings. */
	gettimeofday(&last_buffer_time, NULL);
	gettimeofday(&last_flush_time, NULL);

	MemoryContextSwitchTo(TopMemoryContext);



	while (true) {
		int ret = -1;
		struct timeval current_time;
		int64 flush_elapsed = -1;
		int i = -1;
		bool firsttime = false;

        MemoryContextReset(wch_tmpcontext);

		gettimeofday(&current_time, NULL);

		/* Check the control queue for new connections. */
		processWCHControlQ();

		/* INNER LOOP: Suck as much data as possible off of incoming
		 * connections, periodically flushing archived stream buffers. */
		firsttime = true;
		do {
			bool allowed_to_sleep;
			inum++;
			/* We don't allow AIORead() to go to sleep inside poll() if we
			 * think there's work to be done servicing triage queues. */
			allowed_to_sleep = (firsttime && dequeued_nothing_last_time)
				|| allTriageQueuesEmpty(conn_list, next_free);

/*                || (SUMMARIZE_ONLY == g_current_load_shedding_type);*/

			ret = AIORead(&current_time, allowed_to_sleep);
			/* AIORead will update current_time. */

			/* Flush archived stream buffers every once in a while. */
			flush_elapsed = tv_diff(&last_flush_time, &current_time);
			if (flush_elapsed > FLUSHINTERVAL) {
				gettimeofday(&last_flush_time, NULL);

				for (i = 0; i < next_free; i++) {
					flush_stream(conn_list[i]);
				}
			}

			firsttime = false;
			if ((inum % check_control_q_iter) == 0) {
				/* must periodically check the control q for pull wrappers */
				ret = AIOREAD_METADATA_CHANGED;
			}

			/* If there is a background process creating tuples for us, then
			 * give it time to make more tuples. */
			sched_yield();

		} while (AIOREAD_OK == ret);

		/* FOR DEBUGGING. */
#ifdef WRAPCH_PRINT_STATUS
		this_tupcount = g_total_tuples_processed;
		gettimeofday(&this_time, NULL);
		total_elapsed = tv_diff(&last_time, &this_time);

		if (this_tupcount - last_tupcount > 0) {
			elog(LOG,
				 "wrapch_main_loop(): %d tuples in %lld usec. (%lld usec/tup)",
				 this_tupcount - last_tupcount,
				 total_elapsed,
				 (this_tupcount - last_tupcount) ?
				 total_elapsed / (this_tupcount - last_tupcount)
				 : 0LL);

			/* We only advance the counters when we have output something. */
			memcpy(&last_time, &this_time, sizeof(last_time));
			last_tupcount = this_tupcount;
		}
#endif /* WRAPCH_PRINT_STATUS */

		/* OUTER LOOP: Handle metadata changes, and send unarchived stream
		 * data to the query engine if there is time. */

		/* SPECIAL CASE: Connection added or removed. */
		if (AIOREAD_METADATA_CHANGED == ret) {
			/* Need to refresh our connection information. */
			continue;
		}
		/* END SPECIAL CASE */

		dequeued_nothing_last_time =
			serviceTriageQueues(conn_list, next_free, 1);
	}
}


/* Sets up the poll(2) masks for each of the current connections. 
 *
 * In particular, doesn't bother looking for data from connections with
 * sleeping wrappers.
 *
 * Also zeros out the received events masks.
 *
 * <now> argument should be set to the current wall-clock time.
 * 
 * Returns the minimum sleep time, in milliseconds, or MAXPOLLTIMEOUT,
 * whichever is sooner.
 */
static long
setupEventMasks(struct timeval *now)
{
	int i = -1;
	int nfds = next_free;

	long min_sleep_time = MAXPOLLTIMEOUT;

	for (i = 0; i < nfds; i++) {
		aioconn *conn = conn_list[i];

		if (conn->wrapperstate.asleep) {

            Assert(WRAPCH_IS_A_TIME(&(conn->wrapperstate.wakeup_time)));

			/* 
			 * elog(LOG, "setupEventMasks(): Connection %d needs to sleep %d 
			 * msec", i, (int)(sleep_time_usec / 1000)); */
			/* Don't bother polling if the wrapper is asleep. */
			pfd_list[i].events = 0x0;

		} else {
			/* Connection is not sleeping. */
			pfd_list[i].events = POLLIN | POLLHUP | POLLERR | POLLNVAL;
		}

        if (WRAPCH_IS_A_TIME(&(conn->wrapperstate.wakeup_time))) {
			int64 sleep_time_usec =
				tv_diff(now, &(conn->wrapperstate.wakeup_time));
			min_sleep_time = Min(sleep_time_usec / 1000, min_sleep_time);
        }


		/* For paranoia's sake, reset the received events mask. */
		pfd_list[i].revents = 0x0;
	}

	return Max(min_sleep_time, 0L);
}


/* Invokes wrappers on all sockets with available data.  
 *
 * Arguments:
 *      <now> is a pointer to the current wall-clock time.
 *      <allowed_to_sleep> is TRUE if AIORead is allowed to pass a non-zero
 *              timeout to poll() (Because there's no other work to be done).
 *
 * Updates the wall-clock time after the poll() call.
 *
 * Returns AIOREAD_OK if it found readable sockets,
 *      AIOREAD_NO_DATA if it found no readable sockets at all,
 *      AIOREAD_METADATA_CHANGED if something happened that requires handling
 *              of the control queue.
 *
 * TODO: Divide this function into several smaller ones.
 */
static int
AIORead(struct timeval *now, bool allowed_to_sleep)
{

	int intret = 0;
	int i = 0;
	int nfds = 0;
	bool got_data = false;
	long poll_timeout = -1;

	nfds = next_free;

	poll_timeout = setupEventMasks(now);

	/* elog(LOG, "AIORead: poll() timeout is %ld.", poll_timeout); */

	if (!allowed_to_sleep) {
		poll_timeout = 0;
	}

	/* Check to see if any open sockets have data. */
	intret = poll(pfd_list, nfds, poll_timeout);
	if (intret < 0) {
		if (errno != EINTR)
			elog(ERROR, "Error polling for events on incoming sockets");
		/* Unreachable. */
		else {
			elog(DEBUG1, "Received signal");
			elog(DEBUG1, "nfds: %d", nfds);
			return false;		/* We just were interrupted */
		}
	}

	/* Check for errors... */
	if (checkForConnectionErrors()) {
		return AIOREAD_METADATA_CHANGED;
	}

	/* Check for new connections... */
	if (checkForNewConnections()) {
		return AIOREAD_METADATA_CHANGED;
	}

	/* Run all the awake wrappers with readable sockets. */
	for (i = 0; i < nfds; i++) {
		if (pfd_list[i].revents & POLLIN ||
			conn_list[i]->wrapperstate.hasBufferedData
			|| pfd_list[i].fd == 0
			|| conn_list[i]->wrapperstate.mode != MODE_NEXT) {
			bool removed = false;


			if (pfd_list[i].revents & POLLIN ||
				conn_list[i]->wrapperstate.mode == MODE_RESCAN ||
				conn_list[i]->wrapperstate.hasBufferedData) {
				if (pfd_list[i].revents & POLLIN)
					got_data = true;
				conn_list[i]->wrapperstate.connectionReady = true;
			} else {
				conn_list[i]->wrapperstate.connectionReady = false;
			}


			if (pfd_list[i].fd != acceptfd) {
				/* Run each wrapper until it goes to sleep or stops spitting
				 * out tuples. */
				int orig_tupleCounter = -1;
				do {
					orig_tupleCounter =
						conn_list[i]->wrapperstate.tupleCounter;

					removed = processConnection(conn_list[i], i);
					if (removed) {
						return AIOREAD_METADATA_CHANGED;
					}

				} while (conn_list[i]->wrapperstate.tupleCounter
						 > orig_tupleCounter
						 && !(conn_list[i]->wrapperstate.asleep)
						 && conn_list[i]->wrapperstate.mode == MODE_NEXT);
			}
		}
	}


	/* Wake up any wrappers whose sleep time has elapsed. We do this last so 
	 * as to quickly wake up anyone who set a timeout in the past. */
	gettimeofday(now, NULL);

	for (i = 0; i < nfds; i++) {
		aioconn *conn = conn_list[i];

        /*
		if (conn->wrapperstate.asleep
			&& tv_diff(now, &(conn->wrapperstate.wakeup_time)) <= 0LL) {
            */
        if (WRAPCH_IS_A_TIME(&(conn->wrapperstate.wakeup_time))
			&& tv_diff(now, &(conn->wrapperstate.wakeup_time)) <= 0LL) 
        {
			bool removed = false;

			/* Timeout has expired. */
			conn->wrapperstate.asleep = false;
            conn->wrapperstate.wakeup_time = WRAPCH_NOT_A_TIMEVAL;
            conn->wrapperstate.timeoutExpired = true;

			/* Wrapper gets a call to its next() function when its timeout
			 * expires. */
			removed = processConnection(conn, i);
			if (removed) {
				return AIOREAD_METADATA_CHANGED;
			}

            /* Reset the timeout flag, now that we've made our callback. */
            conn->wrapperstate.timeoutExpired = false;

			/* We consider a timeout expiring to be "getting data", since the 
			 * wrapper is likely to have spat out a tuple. */
			got_data = true;
		}
	}

	if (got_data) {
		return AIOREAD_OK;
	} else {
		return AIOREAD_NO_DATA;
	}
}


/* Takes its inputs from global variables (yuck!).
 * Assumes you've called poll() recently and that the connection metadata
 * hasn't changed since that call to poll().
 * Returns TRUE if it found an error. */
static bool
checkForConnectionErrors(void)
{
	int i = -1;
	int nfds = next_free;

	for (i = 0; i < nfds; i++) {
		if (pfd_list[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
			/* This doesn't ever seem to happen under Linux; instead, read()
			 * just returns zero... */
			elog(NOTICE, "WrapCH: Error on socket %d, deleting it",
				 pfd_list[i].fd);
			deleteConnByIndex(i);
			return true;

		}
	}

	return false;
}

/* Takes its inputs from global variables (yuck!).
 * Assumes you've called poll() recently and that the connection metadata
 * hasn't changed since that call to poll().
 * Returns TRUE if it found a new connection. */
static bool
checkForNewConnections(void)
{
	int i = -1;
	int nfds = next_free;
	for (i = 0; i < nfds; i++) {
		if (pfd_list[i].fd == acceptfd && pfd_list[i].revents & POLLIN) {
			elog(DEBUG1, " connection request pending .. calling doAccept");

			doAccept(pfd_list[i].fd);

			return true;
		}
	}

	return false;
}







bool
processConnection(aioconn * conn, int idx)
{
	MemoryContext oldContext;
	FunctionCachePtr fcache = NULL;
	bool shouldClose = false;
	bool shouldRead = true;
	bool boolret = false;
	bool removed = false;

	Datum wrapperret;

	Assert(conn->wrapper != NULL);

	/* we want non-shedding behavior for pull streams */
	/* 
	 * if(conn->isPullWrapper && (ST_UNARCHIVED_DROP == conn->streamtype)) {
	 * while(serviceTriageQueue(conn) == false); } */

	if (conn->wrapperstate.error_message &&
		conn->wrapperstate.mode != MODE_DONE) {
		return false;
	}

	if (conn->tuple != NULL) {
		boolret = processTuple(conn);
		if (boolret)
			Assert(conn->tuple == NULL);
		else {
			shouldRead = false;
		}
	}


	if (shouldRead) {
		/* ret = processInput(conn); */
		switch (conn->wrapperstate.mode) {
			case MODE_INIT:
				fcache = conn->wrapper->init;
				break;
			case MODE_NEXT:
				fcache = conn->wrapper->next;
				break;
			case MODE_DONE:
				fcache = conn->wrapper->done;
				break;
			case MODE_END:
				fcache = NULL;
				break;
			case MODE_RESCAN:
				fcache = conn->wrapper->rescan;
				break;
			default:
				elog(DEBUG1, "invalid function mode");
		}

		if (fcache == NULL) {
			switch (conn->wrapperstate.mode) {
				case MODE_INIT:
					conn->wrapperstate.mode = MODE_NEXT;
					break;
				case MODE_RESCAN:
				case MODE_NEXT:
					conn->wrapperstate.mode = MODE_DONE;
					break;
				case MODE_DONE:
					conn->wrapperstate.mode = MODE_END;
					break;
				case MODE_END:
					break;
				default:
					elog(ERROR, "invalid function mode");

			}

		} 

        /* 
         * elog(DEBUG1, "about to call function with oid %d",
         * fcache->func.fn_oid);
         */
        /* Here's where we call the wrapper's state transition function. */
        oldContext = MemoryContextSwitchTo(conn->perConnectionContext);
        wrapperret = DirectFunctionCall1(fcache->func.fn_addr,
                                         PointerGetDatum(&
                                                         (conn->
                                                          wrapperstate)));
        MemoryContextSwitchTo(oldContext);
        if (DatumGetBool(wrapperret) == false) {
            elog(DEBUG1, "wrapper function returned fatal error");
            if (!conn->wrapperstate.error_message)
                conn->wrapperstate.error_message =
                    "Unspecified WCH error";


            WCHHandleErrorMessage(idx, conn->wrapperstate.error_message);
            if (conn->streamtype == ST_ARCHIVED) {
                if (conn->wrapperstate.hasBufferedData)
                    conn->wrapperstate.mode = MODE_NEXT;
                else {
                    conn->wrapperstate.doneWithProbe = true;
                    conn->wrapperstate.mode = MODE_RESCAN;
                }
            }

        } else if (conn->wrapperstate.fd != pfd_list[idx].fd) {
            setSocketOptions(conn->wrapperstate.fd);
            pfd_list[idx].fd = conn->wrapperstate.fd;
        }

        /* Create the next tuple, if possible. */
        if (conn->wrapperstate.hasTuple) {
            TupleDesc desc = conn->wrapperstate.aim->tupdesc;
            int ts_attr = desc->constr->ts_attrnum;
            Timestamp ts = DatumGetTimestamp(
                        conn->wrapperstate.returnTuple[ts_attr]);
            
            if (ts_less_than(ts, conn->wrapperstate.latestTs)) {
                elog(LOG, "WCH: Out-of-order timestamp on stream '%s'\n"
                       "(%s followed by %s)",
                        NameListToString(conn->streamname),
                        ts_cstr(conn->wrapperstate.latestTs),
                        ts_cstr(ts));
            } else {
                conn->wrapperstate.latestTs = ts; 
            }

            
            /* SPECIAL CASE: Wrapper wants to send punctuation. */
            if (conn->wrapperstate.returnTupleIsPunct) {
                /*elog(LOG, "WCH creating a punctuation tuple.");*/

                conn->tuple = form_punct_tup_no_leaks(desc, ts, 
                                                    wch_tmpcontext);
            } /* END SPECIAL CASE */
            else {
                conn->tuple =
                    heap_formtuple(desc,
                                   conn->wrapperstate.returnTuple,
                                   conn->wrapperstate.returnTupleIsNull);
            }
            MemoryContextReset(conn->wrapperstate.perTupleContext);
            conn->wrapperstate.hasTuple = false;
            conn->wrapperstate.returnTupleIsPunct = false;
        }

        if (conn->wrapperstate.doneWithProbe) {
            if (conn->wrapperstate.probetuple) {
                conn->tuple = conn->wrapperstate.probetuple;
                conn->wrapperstate.probetuple = NULL;
                processTuple(conn);
            }
            conn->wrapperstate.doneWithProbe = false;
            if (conn->wrapperstate.mode != MODE_DONE)
                conn->wrapperstate.mode = MODE_RESCAN;
        }
        

        /* If the connection has a tuple, try to send it to the backend.  
         * Note that it may take multiple tries to send the tuple. */
        if (NULL != conn->tuple) {
            bool ret = processTuple(conn);
            if (ret) {
                /* Enqueue succeeded */
                conn->wrapperstate.tupleCounter++;
#ifdef WRAPCH_PRINT_STATUS
                g_total_tuples_processed++;
#endif /* WRAPCH_PRINT_STATUS */
            }
        }


		if (conn->wrapperstate.mode == MODE_END) {
			elog(DEBUG1, "wrapper function declared that it is finished");
			shouldClose = true;
		}


		if (shouldClose) {
			elog(NOTICE, "Socked closed %d, deleting it", pfd_list[idx].fd);
			deleteConnByIndex(idx);
			shouldClose = false;
			removed = true;

		}
	}

	return removed;
}

static bool
endUnarchivedStreamScan(int qidx)
{
	bool changed = false;

	int i;
	int qusers = 0;
	aioconn *dconn;

	for (i = 0; i < next_free; i++) {
		dconn = conn_list[i];
		if (dconn->uinfo == NULL)
			continue;
		if (dconn->uinfo->idx == qidx)
			qusers++;
	}

	/* 
	 * if this is the last outstanding scan over this queue, close down
	 * the WCH connections feeding that queue.
	 *
	 * OR NOT...  It turns out we want to keep these queues alive until the
	 * wrapper stops sending data.
	 *
	 * TODO: Make sure we shouldn't be removing the queue for PULL wrappers.
	 */
#if 0
	if (qusers + 1 == getIngresQueueRefCount(qidx)) {
		for (i = 0; i < next_free;) {
			dconn = conn_list[i];
			if (dconn->uinfo == NULL) {
				i++;
				continue;
			}
			if (dconn->uinfo->idx == qidx) {
				/* 
				 * replaces deleted conection with last in list and
				 * decrements next_free
				 */
				elog(LOG, "endUnarchivedStreamScan(): Deleting connection.");
				deleteConnByIndex(i);
				changed = true;

			} else
				i++;
		}
	}
#endif /* 0 */

	/* Decrement the refcount on the queue. */
	freeIngresQ(qidx);

	elog(LOG, "endUnarchivedStreamScan(): Done.");
	return changed;

}

static bool
processWCHControlQ(void)
{
	/* Use a union to get around C99 strict aliasing rules. */
	union
	{
		Controlqentry *cqe;
		char *cqe_char;
	} u;
	bool connlist_changed = false;

	u.cqe = NULL;

	if (dequeue(*p_wrap_control_queue, (char **) &(u.cqe_char), false,
				&controlqentry_serializer)) {


		/* 
		 * Free ingresQ, it assumes everything inside is kosher otherwise
		 * we'll get memory leaks
		 */

		switch ((u.cqe)->command) {

			case INGRESQ_REMOVE:
				elog(DEBUG1, "WrapCH: Removing IngresQ for idx: %d ",
					 (u.cqe)->data.ingresqid);
				connlist_changed =
					endUnarchivedStreamScan((u.cqe)->data.ingresqid);
				break;
			case STREAM_STARTUP:
			case STREAM_SHUTDOWN:
			case STREAM_PROBE:
				ProcessStreamCommand(u.cqe);
				connlist_changed = true;
				break;
			case SHUTDOWN:
				doShutdown();
				break;

			default:
				elog(DEBUG1, "WrapCH: Unknown control code: ");
				break;

		}

		pfree(u.cqe);
	}
	return connlist_changed;
}

void
doShutdown()
{
	int i = 0;

	elog(LOG, "Wrapper clearinghouse shutting down...");

	for (i = 0; i < next_free; i++) {
		elog(LOG, "    Closing wch connection %d.", i);
		deleteConn(conn_list[i]);
	}

    /* Clear out the temporary context to avoid confusing our leak detector */
    MemoryContextReset(wch_tmpcontext);
    MemoryContextDelete(wch_tmpcontext);
    wch_tmpcontext = NULL;

    /* Ditto with the catalog cache. */
    ResetCatalogCaches();

	proc_exit(0);
}

static bool
readBytes(int sockfd, void *buf, int length)
{
	bool done = FALSE;
	int bread = 0;
	int i = 0;
	/* int maxretries=2000000; */

	while (!done) {
		bread = read(sockfd, buf + i, length - i);

		/* 
		 * elog(DEBUG5, "read returns %d", bread);
		 */
		if (bread == 0)			/* EOF on socket */
			return FALSE;
		else if (bread < 0) {
			/* XXX XXX XXX */

			/* We need a cross-platform solution to timeouts when the WCH
			 * client dies */
			if (errno != EAGAIN)
				/* if (errno != EAGAIN || maxretries-- < 0) */
			{
				elog(DEBUG1,
					 "Couldn't read from the socket, aborting connection");

				return FALSE;
			}
		} else
			i += bread;
		if (i == length) {
			done = true;		/* We read to the last character */
		}
	}
	return TRUE;
}

bool
readStreamName(aioconn * a, int sockfd)
{
	char *buf = NULL;
	char *streamnamebuf = NULL;

	bool status = false;
	#define LENGTH 5
	char lengthStr[LENGTH];
	uint32 length;
	int i;

	/* Read in the name of the source */
	status = false;

#ifdef WRAPCH_DEBUG
	elog(DEBUG1, "Reading wrapper information");
#endif
	memset(lengthStr, 0, sizeof(LENGTH));

	/* Read string length */
	status = readBytes(sockfd, (void *) (lengthStr), (size_t)(LENGTH - 1));
	length = atoi(lengthStr);

	if (status) {
		buf = palloc(length + 2);
		a->streamname = NIL;
		streamnamebuf = (char *) palloc(length + 2);
		a->wrappername = (char *) palloc(length + 2);
		*a->wrappername = '\0';
		*streamnamebuf = '\0';
		status = readBytes(sockfd, (void *) buf, length);

		/* Make sure the string is NULL-terminated.  This also lets us accept
		 * init strings that end with carriage return instead of NULL. */
		buf[length - 1] = '\0';
		if (status) {
			if (sscanf(buf, "%[^,],%s", a->wrappername, streamnamebuf) < 1)
				status = false;
			if (*a->wrappername == '\0') {
				pfree(a->wrappername);
				a->wrappername = NULL;
			}

			/* Convert to lowercase */
			for (i = 0; streamnamebuf[i] != '\0'; i++) {
				streamnamebuf[i] = tolower(streamnamebuf[i]);
			}


			if (*streamnamebuf != '\0') {
				a->streamname = StringToNameList(streamnamebuf);
				if (a->streamname == NULL)
					return false;


			} else {
				a->streamname = NIL;

			}

			pfree(streamnamebuf);


		}

	}

	return status;
}

static void
initializeWrapperstate(wrapperstate * w, int fd,
					   TupleDesc d, Queue * pending_probes,
					   MemoryContext tupleContext)
{
	initializeWrapperstateCommon(w);

	/* get a wrapper ready for use */
    TIMESTAMP_NOBEGIN(w->latestTs);
	w->hasTuple = false;
	w->fd = fd;
	w->tdesc = d;
	w->mode = MODE_INIT;
	w->udfstate = NULL;
	w->returnTuple = palloc(w->tdesc->natts * sizeof(Datum));
	w->returnTupleIsNull = palloc(w->tdesc->natts * sizeof(char));
    w->returnTupleIsPunct = FALSE;
	w->returnTupleLength = w->tdesc->natts;
	w->hasBufferedData = FALSE;
    w->tsState = DONT_KNOW;
	w->connectionReady = FALSE;
    w->timeoutExpired = FALSE;
	w->probetuple = NULL;
	w->aim = TupleDescGetAttInMetadata(d);
	w->aom = TupleDescGetAttOutMetadata(d);
	w->pending_probes = pending_probes;
	w->doneWithProbe = false;
	w->mapping_info = NULL;
	w->error_message = NULL;
	w->perTupleContext = tupleContext;
}

static void
freeWrapperstate(struct wrapperstate *w)
{
	w->hasTuple = FALSE;
	if (w->udfstate) {
		/* this really should be NULL (freed by DONE) because this is */
		/* an opaque pointer */
		elog(NOTICE, "User defined wrapper did not free its funcstate.");
	}
	if (w->returnTuple) {
		pfree(w->returnTuple);
		w->returnTuple = NULL;
	}
	if (w->returnTupleIsNull) {
		pfree(w->returnTupleIsNull);
		w->returnTupleIsNull = NULL;
	}
}

static bool
getWrapper(aioconn * a)
{
	Oid wrapperoid = InvalidOid;
	Oid streamoid = InvalidOid;

	wrapperoid = find_wrapper_oid(a->wrappername);
	if (a->streamname != NIL) {
		if (length(a->streamname) != 2) {
			elog(NOTICE, "stream names must be specified as schema.stream");

		} else
			streamoid = SafeGetStreamOid(a->streamname);
	}


	if (a->streamname != NULL && streamoid == InvalidOid) {
		elog(NOTICE, "stream '%s' does not exist in the catalogs",
			 NameListToString(a->streamname));
		return false;
	}

	if (a->wrappername != NULL && wrapperoid == InvalidOid) {
		elog(NOTICE, "wrapper '%s' does not exist in the catalogs",
			 a->wrappername);
		return false;
	}


	if (wrapperoid && streamoid) {

		if (is_valid_wrapper_usage(wrapperoid, streamoid)) {
			a->wrapper = load_wrapper_by_oid(wrapperoid);
			a->streamoid = streamoid;
		} else {
			elog(NOTICE, "WCH: wrapper '%s' is not mapped to stream '%s'",
				 a->wrappername, NameListToString(a->streamname));
			return false;
		}
	} else if (wrapperoid != InvalidOid && streamoid == InvalidOid) {
		Oid *stream_oids = NULL;
		int stream_oids_size = 0;

		get_streams_for_wrapper(wrapperoid,
								&stream_oids, NULL, &stream_oids_size);
		if (stream_oids_size <= 0) {
			elog(NOTICE, "wrapper '%s' is not mapped to any stream",
				 a->wrappername);
			return false;
		} else if (stream_oids_size > 1) {
			elog(NOTICE,
				 "the wrapper clearinghouse currently does not support mapping a wrapper to multiple streams");
			return false;
		} else {
			a->wrapper = load_wrapper_by_oid(wrapperoid);
			a->streamname = get_stream_name_by_oid(stream_oids[0]);
			if (a->streamname == NIL) {
				elog(NOTICE, "lookup of stream name failed");
				return false;
			}
			a->streamoid = stream_oids[0];

		}

	}

	else {
		elog(NOTICE, "error in getWrapper");
		return false;

	}

	loadWrapperAttributes(a);
	load_wrapper_functions(a->wrapper);

	return true;
}


static int
acceptSetup(void)
{

	/* Accepts on a port and adds the connection to the list */
	int acceptfd;
	int opt_true = 1;

	struct sockaddr_in serv_addr;

	/* set_ps_display("aioaccept"); */
	MemoryContextSwitchTo(TopMemoryContext);

	/* Create the socket on which we accept new connections */
	if ((acceptfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

		elog(NOTICE, "the wrapper clearinghouse could not open a socket");
		return -1;
	}
	/* set socket options */

	if (setsockopt(acceptfd, SOL_SOCKET, SO_REUSEADDR, (void *) &opt_true,
				   sizeof(int)) < 0)
		elog(NOTICE, "setting SO_REUSEADDR failed");


	/* Bind our local address so clients can connect to us */
	memset((void *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(acceptPort);

	if (bind(acceptfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {

		elog(NOTICE,
			 "The wrapper clearinghouse could not bind to socket for receiving connections");
		elog(NOTICE, strerror(errno));
		return -1;
	}


	listen(acceptfd, 5);		/* Allow 5 backlog of accept connections */

	/* 
	 * Make the socket non-blocking so read commands don't stall the
	 * reader thread
	 */

	if (fcntl(acceptfd, F_SETFL, O_NONBLOCK) < 0) {
		elog(DEBUG1, "Couldn't set to NONBLOCKING I/O");
		perror("enable NBIO");
	}
	return acceptfd;
}


static void
doAccept(int acceptfd)
{
	bool ret = false;
	TupleDesc tdesc = NULL;
	int newconnid = -1;
	aioconn *newconn = NULL;
	struct sockaddr_in cli_addr;
	socklen_t cli_len;
	int sockfd;
	MemoryContext oldcxt = NULL;

	cli_len = sizeof(cli_addr);
	if ((sockfd = accept(acceptfd, (struct sockaddr *) &cli_addr,
						 &cli_len)) < 0) {

		elog(NOTICE, "Could not accept for connections.");
		elog(NOTICE, strerror(errno));
		return;
	}

#ifdef WRAPCH_DEBUG
	elog(DEBUG1, "AIOAccept adding a connection");
#endif

	newconnid = addPushConnToList(sockfd);
	if (newconnid == -1)
		return;

	newconn = conn_list[newconnid];
	oldcxt = MemoryContextSwitchTo(newconn->perConnectionContext);
	if (readStreamName(newconn, sockfd) == false) {
		elog(NOTICE, "Failed to read stream name.  Closing connection.");
		MemoryContextSwitchTo(oldcxt);
		deleteConnByIndex(newconnid);
		return;
	}
	elog(LOG, "got stream name '%s', wrappername '%s'",
		 (newconn->streamname ?
		  NameListToString(newconn->streamname) : "null"),
		 (newconn->wrappername ? newconn->wrappername : "null"));

	/* 
	 * normally this would be done at a transaction boundary to make sure
	 * that the backend's relcache is properly updated.  Since we operate
	 * in a long running transaction, we'll do cache invalidation at
	 * accept time.
	 */
	AcceptInvalidationMessages();


	elog(DEBUG1, "about to load wrapper information ");
	if (getWrapper(newconn) == false) {
		elog(NOTICE,
			 " couldn't load a wrapper for connection string: wrappername=%s, streamname=%s",
			 (newconn->wrappername ? newconn->wrappername : ""),
			 (newconn->
			  streamname ? NameListToString(newconn->streamname) : ""));

		MemoryContextSwitchTo(oldcxt);
		deleteConnByIndex(newconnid);
		return;
	} else {
		elog(DEBUG1,
			 " loaded wrapper '%s' for connection string: wrappername=%s, streamname=%s",
			 (newconn->wrapper->name),
			 (newconn->wrappername ? newconn->wrappername : ""),
			 (newconn->
			  streamname ? NameListToString(newconn->streamname) : ""));
	}

	elog(DEBUG1, "about to lookup stream informtion");
	tdesc = lookupStream(newconn);
	if (tdesc == NULL) {
		elog(NOTICE, "stream not found... deleting connection");
		deleteConnByIndex(newconnid);
		MemoryContextSwitchTo(oldcxt);
		return;
	}

	elog(DEBUG1, "got tuple descriptor for stream '%s', streamtype is %s ",
		 NameListToString(newconn->streamname),
		 (newconn->streamtype == ST_ARCHIVED ? "archived" : "unarchived"));
	initializeWrapperstate(&(newconn->wrapperstate),
						   sockfd,
						   tdesc, newconn->pending_probes,
						   newconn->perTupleContext);

	newconn->wrapper->info =
		wrapper_oid_append_stream_attr_info(newconn->streamoid,
											newconn->wrapper->wrapperoid,
											newconn->wrapper->info);
	newconn->wrapperstate.wrapper_info = newconn->wrapper->info;

	elog(DEBUG1, "Done looking up stream informtion");

	if (newconn->streamtype == ST_ARCHIVED ||
		newconn->streamtype == ST_UNARCHIVED_SPILL) {
		ret = setupArchivedStream(newconn);
	} else {
		ret = setupUnarchivedStream(newconn);
	}

	if (ret == false) {
		elog(NOTICE, "AIOAccept: Failed to set up stream.");
		deleteConnByIndex(newconnid);
		MemoryContextSwitchTo(oldcxt);
		return;
	}

	elog(DEBUG1, "AIOAccept done adding a connection");

#ifdef WRAPCH_DEBUG
	elog(DEBUG1, "AIOAccept done adding a connection");
#endif
	MemoryContextSwitchTo(oldcxt);

}


static void
flush_stream(aioconn * a)
{
	if (a->ainfo == NULL)
		return;
	a->ainfo->forceflush = true;
	processTuple(a);
}

/* Handle the next tuple in the stream.  Returns false if it fails to enqueue
 * the tuple (e.g. you need to try again later) */
static bool
processTuple(aioconn * a)
{
	bool is_null = false;
	Datum ts = (Datum) NULL;;

	switch (a->streamtype) {
		case ST_ARCHIVED:
		case ST_UNARCHIVED_SPILL:
			/* We'll use processTupleArchived for UNARCHIVED_SPILL as well */
			return processTupleArchived(a);
			break;

		case ST_UNARCHIVED_DROP:
			{
				ts = heap_getattr(a->tuple,
								  get_timestamp_col(a->wrapperstate.tdesc),
								  a->wrapperstate.tdesc, &is_null);
				if (ts == (Datum) NULL) {	/* this is only true for
											 * doneWithProbe */
					return enqueue_tuple_direct(a);
				} else
					return processTupleUnarchivedDrop(a);
				break;
			}

		default:
			elog(ERROR, "Invalid stream type.");

			break;
	}

	/* Unreachable. */
	return false;
}

static bool
processTupleArchived(aioconn * a)
{
	if (a->ainfo->tail == (ARCHIVED_TUPARRAYSIZE) || a->ainfo->forceflush) {
		int count = 0;
		Relation r = relation_open(a->ainfo->reloid, NoLock);
		if (r != NULL) {
			for (; a->ainfo->head < a->ainfo->tail; a->ainfo->head++) {
				simple_heap_insert(r, a->ainfo->tupArray[a->ainfo->head]);
				heap_freetuple(a->ainfo->tupArray[a->ainfo->head]);
				a->ainfo->tupArray[a->ainfo->head] = NULL;
				count++;
			}
			relation_close(r, NoLock);
		}


		if (a->ainfo->head == ARCHIVED_TUPARRAYSIZE) {
			Assert(a->ainfo->tail == ARCHIVED_TUPARRAYSIZE);
			a->ainfo->head = a->ainfo->tail = 0;
		}
	}

	if (a->tuple) {
		a->ainfo->tupArray[a->ainfo->tail++] = a->tuple;
		a->tuple = NULL;
	}

	return true;
}


static bool
processTupleUnarchivedDrop(aioconn * a)
{
    Timestamp tup_ts; 
    MemoryContext origcxt;

	Assert(ST_UNARCHIVED_DROP == a->streamtype);


    origcxt = MemoryContextSwitchTo(wch_tmpcontext);
    tup_ts = 
        get_timestamp_val(a->wrapperstate.tdesc, a->tuple);
    MemoryContextSwitchTo(origcxt);

    /*
	elog(LOG, "Wrapper CH got a tuple for unarchived stream '%s'",
	        NameListToString(a->streamname)); 
            */

	/* SPECIAL CASE: We currently don't support load-shedding for pull
	 * streams. Instead, we bypass the triage queue and send tuples directly 
	 * to the engine. 
     * We do the same thing if the load-shedding behavior for the stream is
     * OVERLOAD_BLOCK */
	if (a->isPullWrapper || OVERLOAD_BLOCK == a->uinfo->summarytype) {
		return enqueue_tuple_direct(a);
	}
	/* END SPECIAL CASE */

	/* Set the global summary window start if this is the very first tuple
	 * from any stream. */
	if (DT_NOBEGIN == g_triage_summary_window_start) {

		/* elog(NOTICE, "Using current timestamp for START AT"); */
		/* Use the parse time so that every stream will get the same START AT 
		 */
        /* Now this parameter may be set in the postgresql.conf file. */
        if (0 == strcmp("none", load_shedding_first_window_start)) {
            /* Round the timestamp down to the previous full summary epoch, 
             * with the first epoch starting at UNIX time = 0 */
            g_triage_summary_window_start
                = round_timestamp(tup_ts, *g_synopsis_interval);
        } else {
             g_triage_summary_window_start = DatumGetTimestamp(
                    DirectFunctionCall3(timestamp_in,
                           CStringGetDatum(load_shedding_first_window_start),
                           ObjectIdGetDatum(InvalidOid),
                           Int32GetDatum(-1)));
        }

        elog(LOG, "Triage summary window start at '%s'",
                ts_cstr(g_triage_summary_window_start));
        elog(LOG, "    (user string was '%s')", 
                load_shedding_first_window_start);
	}

    /* We want sending summaries to preempt insertion into the triage queue. */
    /* NOTE: We use tup_ts here instead of get_current_timestamp() because the
     * triage queue _MUST_ be fed timestamps in order! */
	if (triage_summaries_ready(a->uinfo->triageq, tup_ts)) {
		sendLossSummaries(a);
	}

	triage_enqueue(a->uinfo->triageq, a->tuple);


	/* The triage queue now "owns" the tuple. */
	a->tuple = NULL;

	return true;
}

/* FUNCTION serviceTriageQueue
 * ARGUMENTS: <conn> is an object representing an open connection.
 * PRECONDITIONS: <conn> has a triage queue associated with it.  Any summaries
 *      of kept and/or triaged tuples that the triage queue has generated have
 *      been sent off to the query engine.
 * POSTCONDITIONS: Pulls a single tuple off the queue and either drops it (if
 *      it expired) or tries to enqueue it.
 *      Returns TRUE if it was unable to send any tuples, FALSE otherwise.
 */
static bool
serviceTriageQueue(aioconn * conn, Timestamp now)
{
	bool couldnt_send_anything = true;
	bool ret = false;
	HeapTuple next_tup;

	Assert(conn != NULL);

	/* Only streams of type ST_UNARCHIVED_DROP have triage queues. */
	Assert(ST_UNARCHIVED_DROP == conn->streamtype);

    /*
	elog(LOG, "Top of serviceTriageQueue() for stream '%s'.",
	    NameListToString(conn->streamname)); 
        */

	/* We don't want to send the query engine stale tuples, so drop any that
	 * have expired. */
	/* elog(LOG, "Cleanup of expired from DEQUEUE"); */
	triage_cleanup_expired_tuples(conn->uinfo->triageq, now);

	/* If the triage queue has finished summarizing the triaged tuples from
	 * the most recent summary window, send the summaries to the query
	 * engine. */
	if (triage_summaries_ready(conn->uinfo->triageq, now)) {
        elog(LOG, "Sending loss summaries");
		sendLossSummaries(conn);
	}

	/* Now we're free to process the next tuple in the triage queue. */
	next_tup = triage_peek(conn->uinfo->triageq);

	if (NULL != next_tup) {

        /*
        elog(LOG, "Wrapper CH enqueueing a tuple for stream '%s'.",
             NameListToString(conn->streamname)); 
             */

		/* Our enqueueing operation is nonblocking and may fail! Attempt to
		 * send the tuple to the query engine. */
		ret = enqueue_heaptuple(conn->uinfo->inQ, next_tup, false);

		if (ret) {
			/* Enqueue succeeded. */
            /*elog(LOG, "    Enqueue succeeded!");*/
			couldnt_send_anything = false;

			triage_dequeue(conn->uinfo->triageq);
			heap_freetuple(next_tup);
		}
	} else {
		/* elog(LOG, " serviceTriageQueue(): Got no tuples."); */
	}

	return couldnt_send_anything;
}



/* FUNCTION sendLossSummaries
 * ARGUMENTS: <conn> is a connection.
 * PRECONDITIONS: The Triage Queue for <conn> is constructing summaries of kept
 *      and/or triaged tuples.
 *      The Triage Queue has seen all the tuples from the current summary
 *      window, and triage_summaries_ready() returns TRUE
 * POSTCONDITIONS: Collectes the summaries from the Triage Queue and sends 
 *      the summaries off to the query engine.  Blocks until it manages to 
 *      do so.
 */
static void
sendLossSummaries(aioconn * conn)
{
    SummaryTuples *keep;
    SummaryTuples *drop;
    Relation r; 


	Assert(conn != NULL);

    /* Fetch the current summaries. */
    triage_get_summaries(conn->uinfo->triageq, &keep, &drop);
        /* *keep and *drop will be freed below by the calls to
         * summaryTuplesDestroy() */

    /* SPECIAL CASE: We aren't sending any summaries for this stream. */
    if (OVERLOAD_DROP == conn->uinfo->summarytype)
    {
        summaryTuplesDestroy(keep);
        summaryTuplesDestroy(drop);
        return; 
    }
    /* END SPECIAL CASE */


    Assert(NULL != keep && NULL != drop);

    /* Send the tuples in the summaries off to the query engine. */
    r = relation_open(conn->uinfo->keep_sum_oid, NoLock);
    for (;;) {
        HeapTuple tup = summaryTuplesGetNext(keep); 
        if (NULL == tup) break;
        simple_heap_insert(r, tup);
        heap_freetuple(tup);
    }
    relation_close(r, NoLock);

    r = relation_open(conn->uinfo->drop_sum_oid, NoLock);
    for (;;) {
        HeapTuple tup = summaryTuplesGetNext(drop); 
        if (NULL == tup) break;
        simple_heap_insert(r, tup);
        heap_freetuple(tup);
    }
    relation_close(r, NoLock);

    /* Clean up the tuple sets, now that we've sent all the tuples. */
    summaryTuplesDestroy(keep);
    summaryTuplesDestroy(drop);
}


/* FUNCTION serviceTriageQueues()
 * ARGUMENTS: <connlist> contains information about the open wrappers,
 *      and <maxdeq> is the maximum number of 
 * PRECONDITIONS: The wrapper clearinghouse has finished starting up and is
 *      running zero or more wrappers.
 * POSTCONDITIONS: Pulls tuples off of the lossy triage queues and sends them
 *      off to the query engine until the engine runs out of space or we've
 *      dequeued <maxdeq> elements from a given queue.
 *      Returns TRUE if it was unable to send any tuples, FALSE otherwise.
 */
static bool
serviceTriageQueues(aioconn ** connlist, int ncons, int maxdeq)
{
	bool couldnt_send_anything_at_all = true;
	bool keepgoing = false;
	int i = -1;
	int deq = -1;

	Assert(connlist != NULL);
	Assert(ncons >= 0);
	Assert(maxdeq > 0);


	for (i = 0; i < ncons; i++) {
		/* Only streams of type ST_UNARCHIVED_DROP have triage queues. */
		if (ST_UNARCHIVED_DROP == connlist[i]->streamtype
                && NULL != connlist[i]->uinfo->triageq) {
            Timestamp now = get_current_timestamp(&(connlist[i]->wrapperstate));

            /*
            elog(LOG, "serviceTriageQueues(): Checking connection %d", i);
            */

			/* Keep popping tuples off until we reach the limit or can't take
			 * anything off the queue. */
			for (deq = 0, keepgoing = true; deq < maxdeq && keepgoing; deq++) {
				if (false == serviceTriageQueue(connlist[i], now)) {
					couldnt_send_anything_at_all = false;
				} else {
					keepgoing = false;
				}
			}

		}
	}

	return couldnt_send_anything_at_all;
}


/* Returns TRUE if there are not tuples sitting in triage queues. */
static bool
allTriageQueuesEmpty(aioconn ** connlist, int ncons)
{
	int i = -1;

	for (i = 0; i < ncons; i++) {
		/* Only streams of type ST_UNARCHIVED_DROP have triage queues. */
		if (ST_UNARCHIVED_DROP == connlist[i]->streamtype
                && NULL != connlist[i]->uinfo->triageq) {
			if (triage_size(connlist[i]->uinfo->triageq) > 0) {
				return false;
			}
		}
	}

	return true;
}


static TupleDesc
lookupStream(aioconn * a)
{
	Relation r = NULL;
	TupleDesc tdesc = NULL;


	if (a == NULL || a->streamname == NIL)
		return NULL;

	/* 
	 * the wrapper clearinghouse must grab a lock on the relation to make
	 * sure that it does not disappear out from under us.  This lock will
	 * be released when the stream is closed by the clearinghouse
	 */


	r = relation_open(a->streamoid, AccessShareLock);


	if (r == NULL) {
		elog(DEBUG1, "wrapch: relation %s not found",
			 NameListToString(a->streamname));
		return NULL;
	} else if (!IsAStream(r->rd_rel->relkind))
	{
		elog(DEBUG1, "wrapch: relation %s found, but not a stream",
			 NameListToString(a->streamname));
		relation_close(r, AccessShareLock);
		return NULL;
	} else {
		tdesc = CreateTupleDescCopyConstr(r->rd_att);
		relation_close(r, AccessShareLock);

		if (r->rd_rel->relkind == RELKIND_UNARCHIVEDSTREAM_SPILL) {
			a->streamtype = ST_UNARCHIVED_SPILL;
			a->needsUnlock = TRUE;
		} else if (r->rd_rel->relkind == RELKIND_UNARCHIVEDSTREAM_DROP) {
			a->streamtype = ST_UNARCHIVED_DROP;
			a->needsUnlock = FALSE;
		} else {
			a->streamtype = ST_ARCHIVED;
			a->needsUnlock = TRUE;
		}
	}
	return tdesc;
}

unarchivedStreamInfo *
setupUnarchivedStreamInfo(List * streamname, StreamType type, Oid streamoid)
{
	unarchivedStreamInfo *uinfo = palloc(sizeof(unarchivedStreamInfo));

	memset(uinfo, 0, sizeof(unarchivedStreamInfo));
	uinfo->idx = allocIngresQ(DEF_INGRES_QSIZE, NameListToString(streamname));
	uinfo->inQ = getIngresQ(uinfo->idx);

	/* Set up the lossy priority queue if this stream needs to drop tuples. */
	if (type == ST_UNARCHIVED_DROP) {
		Relation r;
		TupleDesc input_desc = NULL;
		TupleDesc summary_desc = NULL;
		/* bool use_mhist = false; */
		TupleDesc ptr = NULL;
		StreamSynopsisGen *syn_gen = NULL;

		if (false == synopses_initialized) {
			initSynopsesFactories();
		}

		r = relation_open(streamoid, NoLock);
		ptr = RelationGetDescr(r);
		input_desc = CreateTupleDescCopyConstr(ptr);
		relation_close(r, NoLock);

        uinfo->summarytype = r->rd_rel->reloverloadtype;

		switch (uinfo->summarytype) {
			case OVERLOAD_BLOCK:
			case OVERLOAD_DROP:
				syn_gen = &count_synopsis;
                    /* We use this summary scheme as a no-op. */
				break;
			case OVERLOAD_COUNTS:
				syn_gen = &count_synopsis;
				break;
			case OVERLOAD_REGHIST:
				syn_gen = &square_mhist_synopsis;
				break;
			case OVERLOAD_MHIST:
				syn_gen = &mhist_synopsis;
				break;
			case OVERLOAD_WAVELET:
				{
                    /* Read in the wavelet parameters from the relation's
                     * catalog entry. */
                    /* TODO: Move this code into generic_stream_init() -- use
                     * the input tuple descriptor to determine the relation */
					int dimCount = r->rd_rel->relnatts - 1;	
                        /* Don't include the timestamp column. */
					int *params = &r->rd_rel->reloverloadparam3;
					int i;
					wcs_stream_state *ws;
					syn_gen = &wcs_synopsis;
					ws = &(syn_gen->protostate.ds_state.ws);
					ws->maxCount = r->rd_rel->reloverloadparam1;
					ws->res = r->rd_rel->reloverloadparam2;
					ws->dimLo = (int *) palloc(dimCount * sizeof(int));
					ws->dimHi = (int *) palloc(dimCount * sizeof(int));
					for (i = 0; i < dimCount; i++) {
						ws->dimLo[i] = params[i];
					}
					params += i;
					for (i = 0; i < dimCount; i++) {
						ws->dimHi[i] = params[i];
					}
				}
				break;
            case OVERLOAD_SAMPLE:
                syn_gen = &sample_synopsis;
                break;
			default:
				Assert(FALSE);
				break;
		}

		/* Connect up the synopsis streams for this stream. */
        if (OVERLOAD_DROP == uinfo->summarytype) {
            /* No summaries, but we still need a tuple descriptor to produce
             * the counts that we throw away. */
            summary_desc = g_dummy_summary_desc;
        } else if (OVERLOAD_BLOCK == uinfo->summarytype) {
            summary_desc = NULL;
        } else {
            setupSynopsisStreams(streamname, uinfo);
            summary_desc = RelationGetDescr(uinfo->drop_sum_rel);
        }

		/* Syncronize the triage summaries to the first WCH connection. */
		/* Now we sync with the first tuple, so that the windows line up with
		 * the query windows. if (DT_NOBEGIN ==
		 * g_triage_summary_window_start) { g_triage_summary_window_start =
		 * get_current_timestamp(); } */
        
        /* Note that we also create a triage queue for OVERLOAD_DROP, even
         * though no summaries are sent. */
        if (OVERLOAD_BLOCK != uinfo->summarytype) {
            uinfo->triageq = triage_create(TRIAGEQLEN, syn_gen, input_desc,
                                        summary_desc,
									   &g_triage_summary_window_start,
									   g_synopsis_interval,
									   g_delay_constraint_interval);
        }

		/* Clean up after ourselves. */
		FreeTupleDesc(input_desc);

	} else {
		uinfo->triageq = NULL;
        uinfo->drop_sum_oid = InvalidOid;
        uinfo->keep_sum_oid = InvalidOid;
        uinfo->drop_sum_rel = NULL;
        uinfo->keep_sum_rel = NULL;
        uinfo->summarytype = OVERLOAD_BLOCK;
	}

	return uinfo;
}

static bool
setupUnarchivedStream(aioconn * a)
{
	if (a->streamtype == ST_ARCHIVED) {
		elog(ERROR,
			 "Someone passed an archived stream to setupUnarchivedStream().");
		return false;
	}

	elog(DEBUG1, "Setting up unarchived stream '%s'.",
		 (a->streamname ? NameListToString(a->streamname) : "null"));

	a->uinfo = setupUnarchivedStreamInfo(a->streamname, a->streamtype,
										 a->streamoid);

	return true;
}

/* Sets up the streams that summarize dropped and kept tuples for a stream of
 * type ST_UNARCHIVED_DROP */

/* Also adds the appropriate queues to *uinfo */
static bool
setupSynopsisStreams(List * streamname, unarchivedStreamInfo * uinfo)
{
	/* MHIST summaries: */
	List *dropped_name = NULL;
	List *kept_name = NULL;

	/* Do a deep copy, since all the strings are stored in Value structs. */
	dropped_name = copyObject(streamname);
	kept_name = copyObject(streamname);

	/* Is this a qualified name? */
	if (NULL != lnext(streamname)) {
		strVal(lsecond(dropped_name)) = palloc(256);
		strVal(lsecond(kept_name)) = palloc(256);
		SET_DROPPED_STREAM_NAME(strVal(lsecond(dropped_name)), 256,
								strVal(lsecond(streamname)));
		SET_KEPT_STREAM_NAME(strVal(lsecond(kept_name)), 256,
							 strVal(lsecond(streamname)));
	} else {
		strVal(lfirst(dropped_name)) = palloc(256);
		strVal(lfirst(kept_name)) = palloc(256);
		SET_DROPPED_STREAM_NAME(strVal(lfirst(dropped_name)), 256,
								strVal(lfirst(streamname)));
		SET_KEPT_STREAM_NAME(strVal(lfirst(kept_name)), 256,
							 strVal(lfirst(streamname)));
	}

    /* Get the relation IDs for the (archived) summary streams */
    uinfo->drop_sum_oid = SafeGetStreamOid(dropped_name);
    uinfo->keep_sum_oid = SafeGetStreamOid(kept_name);

    /* We need to open the relations here (and close them in
     * cleanupUnarchivedStream()) so that the tuples get written to disk. */
    uinfo->drop_sum_rel = relation_open(uinfo->drop_sum_oid, NoLock);
    uinfo->keep_sum_rel = relation_open(uinfo->keep_sum_oid, NoLock);

    Assert(RELKIND_ARCHIVEDSTREAM == uinfo->drop_sum_rel->rd_rel->relkind);
    Assert(RELKIND_ARCHIVEDSTREAM == uinfo->keep_sum_rel->rd_rel->relkind);
	return true;
}

static void
send_end_and_close_q(Queue * q, int index)
{
	bool ret;

	Assert(NULL != q);

	ret = enqueue_endofstream(q, true /* Blocking! */ );

	if (!ret) {
		elog(WARNING, "WrapCH: Error enqueuing end-of-stream tuple.");
	}

	freeIngresQ(index);
}

static bool
cleanupUnarchivedStream(aioconn * a)
{
	/* OC: need to revisit */
	if (a->uinfo == NULL) {
        elog(LOG, "uinfo NULL; returning TRUE");
		return true;
	}
	if (a->uinfo->triageq != NULL) {
		/* If this stream has a triage queue, empty out the queue. */
		while (triage_size(a->uinfo->triageq) > 0) {
			/* Since we've already got a function to empty out _all_ the
			 * queues, just call that one. */
			serviceTriageQueue(a, get_current_timestamp(&(a->wrapperstate)));

			/* Give the query engine some time to suck up the tuples. */
			tcq_usleep(TRIAGE_DRAIN_DELAY);
		}

		/* Generate one last summary. */
		sendLossSummaries(a);

		/* _Now_ we're done with the triage queue. */
		triage_destroy(a->uinfo->triageq);
        a->uinfo->triageq = NULL;
	}

    /* Now we need to call relation_close() on the summary streams so that
     * they get written out to disk. */
    if (a->uinfo->summarytype != OVERLOAD_BLOCK 
            && a->uinfo->summarytype != OVERLOAD_DROP) {
        relation_close(a->uinfo->drop_sum_rel, NoLock);
        relation_close(a->uinfo->keep_sum_rel, NoLock);
    }
    a->uinfo->drop_sum_rel = NULL;
    a->uinfo->keep_sum_rel = NULL;

	/* Send an end-of-stream tuple to indicate that the wrapper is shutting
	 * down. */
	if (a->uinfo->inQ) {
		send_end_and_close_q(a->uinfo->inQ, a->uinfo->idx);
	} else {
		elog(WARNING, "WrapCH: Didn't find a queue when deleting stream.");
	}

	pfree(a->uinfo);
	a->uinfo = NULL;

	return true;
}

static bool
setupArchivedStream(aioconn * a)
{
	ResetUsage();
	if (a->streamtype != ST_ARCHIVED && a->streamtype != ST_UNARCHIVED_SPILL) {
		elog(ERROR,
			 "Someone passed an unarchived drop stream to setupArchivedStream().");
		return false;
	}

	a->ainfo = palloc(sizeof(archivedStreamInfo));
	a->ainfo->reloid = a->streamoid;
	a->ainfo->head = 0;
	a->ainfo->tail = 0;
	a->ainfo->forceflush = false;
	a->ainfo->r = relation_open(a->ainfo->reloid, NoLock);
	return a->ainfo->reloid != InvalidOid;

}

static bool
cleanupArchivedStream(aioconn * a)
{
	if (a->ainfo == NULL)
		return true;
	if (a->ainfo->reloid == InvalidOid)
		return false;
	a->ainfo->forceflush = true;
	processTuple(a);

	relation_close(a->ainfo->r, NoLock);

	pfree(a->ainfo);
	a->ainfo = NULL;
	ShowUsage("WrapperClearingHouse Stats: ");
	return true;
}

void
AbortWrapCH(void)
{
	elog(DEBUG1, "WrapCH: Don't yet know how to abort wrapper processing");
}

/* @EdtcqpmMS */
static void
deleteConnByIndex(int idx)
{
	elog(LOG, "WCH deleting connection %d.", idx);
	deleteConn(conn_list[idx]);
	removeConnFromListByIndex(idx);
	elog(LOG, "WCH done deleting connection %d.", idx);
}

static void
deleteConn(aioconn * a)
{
	Relation rel;

	if (a->wrapperstate.fd > 0)
		close(a->wrapperstate.fd);
	delete_wrapper(&(a->wrapper));

	if (a->streamtype == ST_ARCHIVED || a->streamtype == ST_UNARCHIVED_SPILL) {
		cleanupArchivedStream(a);
	} else {
		cleanupUnarchivedStream(a);
	}

	/* release the lock on the relation */
	if (a->needsUnlock) {
		rel = relation_open(a->streamoid, NoLock);
		if (rel)
			relation_close(rel, AccessShareLock);
	}
	freeWrapperstate(&(a->wrapperstate));
	elog(LOG, "Done calling freeWrapperState()");

	MemoryContextDelete(a->perTupleContext);
	MemoryContextDelete(a->perConnectionContext);

    free_queue(&(a->pending_probes));

	pfree(a);


}



static void
removeConnFromListByIndex(int idx)
{

	/* Index is out of range */
	if ((idx >= next_free) || (idx < 0))
		return;


	/* Replace the hole we created from the one at the bottom */
	/* This works even if the bottom most entry is deleted */
	conn_list[idx] = NULL;
	memset(&pfd_list[idx], 0, sizeof(struct pollfd));
	conn_list[idx] = conn_list[next_free - 1];
	pfd_list[idx] = pfd_list[next_free - 1];
	conn_list[next_free - 1] = NULL;
	memset(&pfd_list[next_free - 1], 0, sizeof(struct pollfd));
	next_free--;
	elog(DEBUG1, "Deleted connection: %d", idx);

}

void
ProcessStreamCommand(Controlqentry * cqe)
{
	int i = 0;
	Oid *wrappers = NULL;
	Oid *mapping = NULL;
	int wrappers_size = 0;
	aioconn *pending;
	List *toBeRemoved = NIL;
	List *iter = NIL;
	Oid streamoid = cqe->data.w.streamoid;
	Oid wrapperoid = InvalidOid;

	if (cqe->command == STREAM_PROBE) {
		wrapperoid = cqe->probeinfo.wrapperoid;
		for (i = 1; i < next_free; i++) {
			if (conn_list[i]->wrapper->wrapperoid == wrapperoid &&
				conn_list[i]->streamoid == streamoid) {
				if (enqueue(conn_list[i]->pending_probes,
							(char *) cqe->probeinfo.probetuple,
							false, &tuple_serializer) == false) {
					elog(NOTICE, "enqueue to pending probes queue failed");
				}
				if (conn_list[i]->wrapperstate.mapping_info)
					pfree(conn_list[i]->wrapperstate.mapping_info);
				conn_list[i]->wrapperstate.mapping_info =
					cqe->probeinfo.mapping;
				break;
			}
		}
	} else {
		get_wrappers_for_stream(streamoid, &wrappers, &mapping,
								&wrappers_size);
		for (i = 0; i < wrappers_size; i++) {
			if (wrappers[i] == (Oid) NULL)
				continue;
			switch (cqe->command) {
				case STREAM_STARTUP:

					pending =
						SetupPullWrapper(wrappers[i], mapping[i], streamoid);
					if (pending)
						addPullConnToList(pending);

					break;
				case STREAM_SHUTDOWN:

					for (i = 0; i < next_free; i++) {
						if (conn_list[i]->streamoid &&
							is_pull_wrapper(conn_list[i]->wrapper) &&
							(conn_list[i]->streamoid == streamoid))
							toBeRemoved = lconsi(i, toBeRemoved);
					}

					foreach(iter, toBeRemoved) {
						conn_list[lfirsti(iter)]->wrapperstate.mode =
							MODE_DONE;
					}
					break;
				default:
					elog(FATAL, "invalid command");
			}
		}
		if (wrappers != NULL)
			pfree(wrappers);


	}

}

void
setSocketOptions(int sockfd)
{
	int enable = 1;


	/* Disable Nagle and make non-blocking */
	if (g_disableNagle) {
		if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY,
					   (char *) &enable, sizeof(int)) < 0) {
			elog(DEBUG1, "Couldn't set TCP_NODELAY");
			perror("disable nagle");
		}
	}

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
		elog(DEBUG1, "Couldn't set to NONBLOCKING I/O");
		perror("enable NBIO");
	}
}



static int
addPushConnToList(int sockfd)
{
	aioconn *nconn;

	nconn = createConn();
	setSocketOptions(sockfd);
	nconn->wrapperstate.fd = sockfd;
	return addConnToList(nconn);
}






void
initializeWrapperstateCommon(wrapperstate * w)
{
	/* Zeroing out the struct takes care of most of our initialization for
	 * us. */
	memset(w, 0x0, sizeof(*w));

	w->fd = -1;

	/* TODO: Add additional initialization for anything else that shouldn't
	 * start with a value of 0x0. */

}

aioconn *
SetupPullWrapper(Oid wrapperoid, Oid mappingoid, Oid streamoid)
{
	bool ret = true;
	TupleDesc tdesc = NULL;
	aioconn *a = NULL;
	wrapper *w = load_wrapper_by_oid(wrapperoid);
	w->info = append_attr_info(mappingoid, w->info);

	if (w && is_pull_wrapper(w))
		load_wrapper_functions(w);
	else {
		if (w)
			delete_wrapper(&w);
		return NULL;
	}
	a = createConn();
	a->wrapper = w;
	a->wrappername = pstrdup(a->wrapper->name);
	a->streamname = get_stream_name_by_oid(streamoid);
	a->streamoid = streamoid;
	a->wrapperstate.wrapper_info = w->info;
	a->isPullWrapper = TRUE;

	/* the connection has been established by the init method, but the */
	/* stream information has not yet been initialized... do that now. */

	tdesc = lookupStream(a);
	if (tdesc == NULL) {
		elog(DEBUG1, "stream not found... deleting connection");
		deleteConn(a);
		return false;
	}
	elog(DEBUG1, "got tuple descriptor for stream '%s', streamtype is %s ",
		 NameListToString(a->streamname),
		 (a->streamtype == ST_ARCHIVED ? "archived" : "unarchived"));
	initializeWrapperstate(&(a->wrapperstate),
						   a->wrapperstate.fd,
						   tdesc, a->pending_probes, a->perTupleContext);

	if (a->streamtype == ST_ARCHIVED || a->streamtype == ST_UNARCHIVED_SPILL) {
		ret = setupArchivedStream(a);
	} else {
		ret = setupUnarchivedStream(a);
	}

	loadWrapperAttributes(a);
	a->wrapperstate.wrapper_info = w->info;
	return a;
}



int
establishConnection(void *wrapper_info)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *entry;
	int port = -1;

	key_value_entry *hostnamedata = get_entry(HOSTNAME_KEY, wrapper_info);
	key_value_entry *portdata = get_entry(PORT_KEY, wrapper_info);

	if (!hostnamedata || !portdata)
		return -2;

	port = atoi(portdata->value);

	entry = gethostbyname(hostnamedata->value);
	if (entry == NULL) {
		elog(DEBUG1, "wrapper connection failure");
		return -2;
	}

	elog(DEBUG1, "HostIP: %s %s %d\n", hostnamedata->value,
		 inet_ntoa(*(struct in_addr *) (entry->h_addr)), entry->h_length);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	/* serv_addr.sin_addr.s_addr = inet_addr(entry->h_addr); */
	serv_addr.sin_addr = *(struct in_addr *) (entry->h_addr);
	serv_addr.sin_port = htons(port);

	/* open a TCP socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		elog(DEBUG1, "source: cannot open socket\n");
		return -2;
	}


	/* connect to well known TCQ wrapper */
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <
		0) {
		elog(DEBUG1, " cannot connect to pull source wrapper");
		return -2;
	}


	setSocketOptions(sockfd);
	return sockfd;

}

void
loadWrapperAttributes(aioconn * a)
{

	key_value_entry *e = NULL;


	e = get_entry("BATCHSIZE", a->wrapper->info);
	if (e)
		a->batchsize = atoi(e->value);
}



static aioconn *
createConn()
{
	struct queue_lock *ql;

	aioconn *ret = palloc(sizeof(aioconn));

	memset(ret, 0, sizeof(aioconn));
	memset(&ret->wrapperstate, 0, sizeof(struct wrapperstate));
	initializeWrapperstateCommon(&ret->wrapperstate);
	ret->wrapper = NULL;
	ret->needsUnlock = FALSE;
	ret->batchsize = 1;
	make_lock_nolock(&ql);
	make_queue(&ret->pending_probes, ql, WCHPROBEQSIZE);

	ret->tuple = NULL;
	ret->perTupleContext = AllocSetContextCreate(TopMemoryContext,
												 "per tuple context",
												 ALLOCSET_DEFAULT_MINSIZE,
												 ALLOCSET_DEFAULT_INITSIZE,
												 ALLOCSET_DEFAULT_MAXSIZE);
	ret->perConnectionContext = AllocSetContextCreate(TopMemoryContext,
													  "per connection context",
													  ALLOCSET_DEFAULT_MINSIZE,
													  ALLOCSET_DEFAULT_INITSIZE,
													  ALLOCSET_DEFAULT_MAXSIZE);

	return ret;
}




int
addConnToList(aioconn * conn)
{
	if (next_free > MAX_CONN) {
		elog(NOTICE,
			 " wrapper clearinghouse has exceeded the maximum number of connections (%d)",
			 MAX_CONN);

		return -1;
	}

	pfd_list[next_free].fd = conn->wrapperstate.fd;
	pfd_list[next_free].events = POLLIN | POLLHUP | POLLERR | POLLNVAL;
	pfd_list[next_free].revents = 0;	/* zero it out */
	conn_list[next_free] = conn;
	next_free++;

	return next_free - 1;

}


static int
addPullConnToList(aioconn * conn)
{

	return addConnToList(conn);

}







/* -----	functions below the line are exported to wrapper writers ---- */
Oid
SafeGetStreamOid(List * streamname)

{
	RangeVar *rvar;
	Oid streamoid = InvalidOid;
	sigjmp_buf *old_jump_buf;
	sigjmp_buf sigjmp_restart;

	old_jump_buf = elog_jump_buf;
	elog_jump_buf = &sigjmp_restart;
	if (sigsetjmp(sigjmp_restart, 1) == 0) {
		rvar = makeRangeVarFromNameList(streamname);
		streamoid = RangeVarGetRelid(rvar, true);
	} else {
		streamoid = InvalidOid;

	}
	elog_jump_buf = old_jump_buf;
	InError = false;
	return streamoid;
}


Datum
DatumFromTypeInstance(AttInMetadata * aim,
					  int colno,
					  char *typeInstance, MemoryContext ctx, bool * wasNull)
{
	Datum ret = PointerGetDatum(NULL);
	MemoryContext oldctx;
	sigjmp_buf *old_jump_buf;
	sigjmp_buf sigjmp_restart;

	Assert(wasNull != NULL);
	*wasNull = TRUE;
	oldctx = MemoryContextSwitchTo(ctx);

	old_jump_buf = elog_jump_buf;
	elog_jump_buf = &sigjmp_restart;
	if (sigsetjmp(sigjmp_restart, 1) == 0) {
		PGFunction func = aim->attinfuncs[colno - 1].fn_addr;
		FunctionCallInfoData fcinfo;
		Datum result;
		MemSet(&fcinfo, 0, sizeof(fcinfo));
		fcinfo.nargs = 3;
		fcinfo.arg[0] = CStringGetDatum(typeInstance);
		fcinfo.arg[1] = ObjectIdGetDatum(aim->attelems[colno - 1]);
		fcinfo.arg[2] = Int32GetDatum(aim->atttypmods[colno - 1]);
		result = (*func) (&fcinfo);
		if (fcinfo.isnull)
			*wasNull = TRUE;
		else
			*wasNull = FALSE;

		return result;
	} else {
		ret = PointerGetDatum(NULL);
		*wasNull = TRUE;
		elog_jump_buf = old_jump_buf;
		InError = false;
		MemoryContextSwitchTo(oldctx);
		return ret;
	}
	return (Datum) NULL;
}

void
WCHHandleErrorMessage(int con, char *msg)
{
	if (conn_list[con]->uinfo)
		TCQHandleErrorMessageForQuery(conn_list[con]->uinfo->idx, msg);
	else {
		WCHHandleErrorMessageForArchivedStream(conn_list[con]->streamoid,
											   msg);
	}

}

void
WCHHandleErrorMessageForArchivedStream(int streamoid, char *msg)
{
	Controlqentry c;
	MemoryContext old = MemoryContextSwitchTo(TopSharedMemoryContext);
	c.command = WCH_ERROR;
	c.data.w.streamoid = streamoid;
	c.error = pstrdup(msg);
	MemoryContextSwitchTo(old);
	if (enqueue
		(*p_control_queue, (char *) &c, false,
		 &controlqentry_serializer) == false) {
		elog(NOTICE, "control queue is full.... this shouldn't happen");
	}
}

char *
CStringFromTuple(AttOutMetadata * aom,
				 HeapTuple tup, int attno, bool * isNull, MemoryContext ctx)
{
	Datum ret = PointerGetDatum(NULL);
	Datum attr = PointerGetDatum(NULL);
	MemoryContext oldctx = MemoryContextSwitchTo(ctx);

	bool in = false;
	sigjmp_buf *old_jump_buf;
	sigjmp_buf sigjmp_restart;


	if (aom == NULL || attno < 1 || attno > aom->tupdesc->natts)
		elog(ERROR, "invalid argument");

	old_jump_buf = elog_jump_buf;
	elog_jump_buf = &sigjmp_restart;
	if (sigsetjmp(sigjmp_restart, 1) == 0) {
		attr = heap_getattr(tup, attno, aom->tupdesc, &in);
		*isNull = in;
		if (*isNull == false) {
			ret =
				DirectFunctionCall2(aom->attoutfuncs[attno - 1].fn_addr, attr,
									aom->attelems[attno - 1]);

		}

	} else
		ret = PointerGetDatum(NULL);
	elog_jump_buf = old_jump_buf;
	InError = false;
	MemoryContextSwitchTo(oldctx);
	return DatumGetCString(ret);

}

bool
hasNewProbeTuple(wrapperstate * s)
{
	bool ret = false;

	if (s->probetuple)
		ret = true;
	else
		ret = dequeue(s->pending_probes,
					  (char **) &s->probetuple, false, &tuple_serializer);
	return ret;
}

bool
isBindingColumn(wrapperstate * s, int colno)
{
	if (s->mapping_info)
		return s->mapping_info[colno - 1] >= 0;
	return false;
}

int
getNumBindings(wrapperstate * s)
{
	int i = 0;
	int count = 0;

	for (i = 0; i < s->tdesc->natts; i++) {
		if (s->mapping_info[i] > 0)
			count++;
	}

	return count;
}

char *
getBindingValueByNumber(wrapperstate * s, int bindingno)
{
	int i = 1;
	int count = 0;

	for (i = 1; i <= s->tdesc->natts; i++) {
		if (s->mapping_info[i - 1] >= 0)
			count++;
		if (count == bindingno)
			return getBindingValueByStreamCol(s, i);
	}
	return NULL;
}

char *
getBindingValueByStreamCol(wrapperstate * s, int streamcol)
{
	bool isNull = false;

	if (s->mapping_info == NULL ||
		streamcol > s->tdesc->natts || s->mapping_info[streamcol - 1] <= 0)
		return NULL;
	return CStringFromTuple(s->aom,
							s->probetuple,
							streamcol, &isNull, s->perTupleContext);
}


/*
 * Convenience functions for dealing with ingress queues.
 *
 * Returns TRUE if the enqueue succeeded.
 */
bool
enqueue_heaptuple(Queue * q, HeapTuple tup, bool blocking)
{
	result r;

	Assert(q != NULL);

	/* Need to box up the tuple inside a "result" struct. */
	r.type = RESULT_HEAPTUPLE;
	r.data.tuple = tup;

	return enqueue(q, (char *) (&r), blocking, &result_serializer);
}

bool
enqueue_endofstream(Queue * q, bool blocking)
{
	result eos_result;
	eos_result.type = RESULT_DONE;
	eos_result.data.info = 0x0;
	return enqueue(q, (char *) (&eos_result), blocking, &result_serializer);
}



/*******************************************************************************
 * The following stub functions implement the drop-only (as opposed to
 * drop-and-summarize) load-shedding strategy.
 ******************************************************************************/
#if 0
void *
drop_initf_stub(void *arg, const TupleDesc desc)
{
	CountSynopsis *c = palloc(sizeof(*c));
	/* CountSynopsis is typedef'd to int32. */

	*c = 0;

	/* Tuple descriptor is ignored. */

	return (void *) c;
}

void
drop_nextf_stub(void *arg, HeapTuple tup)
{
	CountSynopsis *c = (CountSynopsis *) (arg);

	(*c)++;

	/* We're responsible for freeing the tuples passed to us. */
	heap_freetuple(tup);
}

void *
drop_donef_stub(void *arg)
{
	CountSynopsis *c = (CountSynopsis *) (arg);

	CountSynopsis c_val = *c;

	pfree(c);

	return (void *) (c_val);
}
#endif
/* End stub functions. */





void *
wrapper_oid_append_stream_attr_info(Oid streamoid, Oid wrapperoid, void *info)
{
	Oid mappingoid = get_wrapper_stream_mappingoid(wrapperoid, streamoid);
	Assert(mappingoid != InvalidOid);
	info = append_attr_info(mappingoid, info);
	return info;
}

/* Fetch the current time in PostgreSQL format.  Argument is a wrapperstate; if
 * the wrapper is generating timestamps, returns the equivalent of
 * gettimeofday(); otherwise, returns the most recent timestamp the wrapper has
 * seen.
 */
Timestamp
get_current_timestamp(wrapperstate *w)
{
    MemoryContext origcxt;
	Timestamp ret;

    /* SPECIAL CASE: The wrapper is receiving premade timestamps (or doesn't
     * know whether it should be generating timestamps), so we shouldn't 
     * generate realtime ones. */
    if (GENERATING != w->tsState) {
        return w->latestTs;
    }
    /* END SPECIAL CASE */

    /* The following calls may leak memory, so switch to our temporary memory
     * context. */
    origcxt = MemoryContextSwitchTo(wch_tmpcontext);
	ret = DatumGetTimestamp(DirectFunctionCall1(text_timestamp,
												 DirectFunctionCall1
												 (timeofday,
												  PointerGetDatum(NULL))));
    MemoryContextSwitchTo(origcxt);
    return ret;
}


/* Attempt to send an aioconn object's current tuple directly to TCQ using the
 * queue mechanism.  Bypasses triage queues entirely.  Returns true and clears
 * the tuple field of <a> if successful (enqueue is nonblocking and may fail). 
 */
bool
enqueue_tuple_direct(aioconn * a)
{
	bool ret = false;
	ret = enqueue_heaptuple(a->uinfo->inQ, a->tuple, false);
	if (ret) {
        /*elog(LOG, "Enqueue succeeded");*/
		a->tuple = NULL;
    } else {
        /*elog(LOG, "Enqueue failed");*/
    }
	return ret;
}


