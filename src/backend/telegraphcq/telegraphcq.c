/*-------------------------------------------------------------------------
 *
 * telegraphcq.c
 *
 *	  Entry point for new TelegraphCQ Executor
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/telegraphcq.c,v 1.52 2005/06/13 02:59:28 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "postgres.h"
#include "miscadmin.h"

#include "executor/nodeEddy.h"
#include "executor/nodeFSched.h"
#include "libpq/pqsignal.h"
#include "nodes/plannodes.h"
#include "optimizer/cost.h"
#include "rqueue/resultq.h"
#include "rqueue/rqdest.h"
#include "rqueue/planq.h"
#include "storage/ipc.h"
#include "storage/spin.h"
#include "telegraphcq/dyncatalog.h"
#include "telegraphcq/wrapch.h"
#include "telegraphcq/telegraphcq.h"
#include "tcop/dest.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "utils/timestamp.h"

void		restore_working_directory(int code, char *pathname);

char	   *telegraphcqdb;
char	   *telegraphcquser;
int32		tcqlocaltz;

#define ARGV_SIZE	64
/* from tcop/postgres.c */
extern CommandDest whereToSendOutput;

extern int	random_seed;

extern int numResultQueues;

/*
 * This function initializes global data structures in shared memory as well as
 * assigning process local heap pointers to pointers to these global data
 * structures. The reason we need the extra level of indirection is because the
 * shared memory data structures such as the query-plan/query-tree repeatedly
 * get created and destroyed in the shared memory context. So each time they may
 * be created in a different place in shared memory.
 *
 * Not all shared memory data structures keep changing. The query-plan semaphore
 * that is used to protect access to the query plan never changes. So, we will
 * not use a pointer to a pointer in that case.
 *
 */
void
InitializeTCQPlumbing()
{
	Queue	  **result_queues = NULL;
	bool	   *result_queues_init = NULL;
	Queue	  **ingres_queues = NULL;
	char	  **ingres_queues_init = NULL;
	TupleDesc  *ingres_queues_tdesc = NULL;

	MemoryContext oldContext;
	int			i = 0;

	/* add declaration for HASHCTL to create the heap scan progress table */
	HASHCTL heapProgress_info;

	elog(DEBUG1, "TelegraphCQ: InitializeTCQPlumbing()");

	if (!(*init_plumbing))
	{
		SpinLockAcquire(p_qp_spinlock);

		if (!(*init_plumbing))	/* Re-test the flag !! */

			/*
			 * Because before the acquire and after the test, it could
			 * have been initialized
			 */
		{
			struct queue_lock *ql;
			struct queue_lock *cqsl;

			/*
			 * Allocate and initialize result_queue in shared memory
			 */

			/*
			 * tcqoc: calling SHMContextinit doesn't hurt here, but since
			 * it needs to be called prior to any forks, I put a call to
			 * it in postmaster.c when the -t option is parsed.
			 */
			oldContext = CurrentMemoryContext;
			MemoryContextSwitchTo(TopSharedMemoryContext);
			*tcqbackendinitialized = FALSE;
			*tcqwrapchinitialized = FALSE;
			make_queue(p_plan_queue, NULL, PLANQSIZE);

			if (!UseHashGroup)
				make_queue(p_control_queue, NULL, CONTROLQSIZE);
			else
			{
				make_lock_sl(&cqsl);
				make_queue(p_control_queue, cqsl, CONTROLQSIZE);
			}

			/*
			 * This queue has to be non-blocking, so clearing house can
			 * continue when nothing exists
			 */
			make_lock_sl(&ql);
			make_queue(p_wrap_control_queue, ql, WRAPCONTROLQSIZE);

			*shmResultQueues = (Queue **) palloc(sizeof(Queue *) * numResultQueues);
			*shmResultQueuesInit = (bool *) palloc(sizeof(bool) * numResultQueues);
			*shmIngresQueues = (Queue **) palloc(sizeof(Queue *) * NUM_INGRES_QUEUES);
			*shmIngresQueuesInit = (char **) palloc(sizeof(char *) * NUM_INGRES_QUEUES);
			*shmIngresQueuesRefCount = (int *) palloc(sizeof(int) * NUM_INGRES_QUEUES);
			*shmIngresQueuesTDesc = (TupleDesc *) palloc(sizeof(TupleDesc) * NUM_INGRES_QUEUES);

			/* This is to we have a common no lock */
			make_lock_nolock(p_no_lock);

			MemoryContextSwitchTo(oldContext);
			elog(DEBUG1,
				 "TelegraphCQ: Created %d slots for result queues "
				 "in the shared memory context",
				 numResultQueues);
			elog(DEBUG1,
				 "TelegraphCQ: Created %d slots for ingres queues"
				 "in the shared memory context",
				 NUM_INGRES_QUEUES);

			plan_queue = *p_plan_queue;
			result_queues = *shmResultQueues;
			result_queues_init = *shmResultQueuesInit;
			ingres_queues = *shmIngresQueues;
			ingres_queues_init = *shmIngresQueuesInit;
			ingres_queues_tdesc = *shmIngresQueuesTDesc;

			for (i = 0; i < numResultQueues; i++)
			{
				result_queues[i] = NULL;
				result_queues_init[i] = false;
			}
			CreateResultQueues();

			for (i = 0; i < NUM_INGRES_QUEUES; i++)
			{
				ingres_queues[i] = NULL;
				ingres_queues_init[i] = NULL;
				ingres_queues_tdesc[i] = NULL;
				(*shmIngresQueuesRefCount)[i] = 0;
			}

			/* 
			 * Create the hash table for heap scan progress table
			 */
			MemSet(&heapProgress_info, 0, sizeof(heapProgress_info));
			heapProgress_info.keysize = sizeof(Oid);
			heapProgress_info.entrysize = sizeof(HeapProgressLookupEnt);
			heapProgress_info.hash = tag_hash;
			heapProgress_info.hcxt = TopSharedMemoryContext;
			*heapScanProgressTable = 
			  hash_create("heapScanProgressTable", 
				      32, 
				      &heapProgress_info, 
				      HASH_ELEM | HASH_FUNCTION | HASH_CONTEXT);
			if(*heapScanProgressTable == NULL)
			{   
			  elog(ERROR, "InitializeTCQPlumbing: Creation of heapScanProgressTable failed");
			}

			*init_plumbing = true;
		}
		else
		{

			plan_queue = *p_plan_queue; /* tcqoc */
			result_queues = *shmResultQueues;
			result_queues_init = *shmResultQueuesInit;
			ingres_queues = *shmIngresQueues;
			ingres_queues_init = *shmIngresQueuesInit;
			ingres_queues_tdesc = *shmIngresQueuesTDesc;

		}

		SpinLockRelease(p_qp_spinlock);
	}
	else
	{
		plan_queue = *p_plan_queue;
		result_queues = *shmResultQueues;
		result_queues_init = *shmResultQueuesInit;
		ingres_queues = *shmIngresQueues;
		ingres_queues_init = *shmIngresQueuesInit;
		ingres_queues_tdesc = *shmIngresQueuesTDesc;
	}

}


void
CleanupTCQPlumbing()
{
	Queue	  **ingres_queues = NULL;
	char	  **ingres_queues_init = NULL;
	bool		haveIngresQueues = (shmIngresQueues && shmIngresQueuesInit);
	int			i = 0;

	elog(DEBUG1, "running CleanupTCQPlumbing");



	if (haveIngresQueues)
	{
		ingres_queues = *shmIngresQueues;
		ingres_queues_init = *shmIngresQueuesInit;
	}
	else
		elog(NOTICE, "ingres queue structures not allocated in CleanupTCQPlumbing");

	if (p_plan_queue && *p_plan_queue)
	{
		free_queue(p_plan_queue);
		*p_plan_queue = NULL;
	}
	else
		elog(NOTICE, "plan queue is already freed");


	if (p_control_queue && *p_control_queue)
	{
		free_queue(p_control_queue);
		*p_control_queue = NULL;
	}
	else
		elog(NOTICE, "control queue is already freed");

	DeallocateResultQueues();


	if (haveIngresQueues)
	{
		for (i = 0; i < NUM_INGRES_QUEUES; i++)
		{
			if (ingres_queues_init[i] != NULL)
			{
				elog(DEBUG1, "deleting ingres queue %d", i);

				free_queue(&ingres_queues[i]);
				ingres_queues[i] = NULL;

			}

		}
		shmIngresQueues = NULL;
		shmIngresQueuesInit = NULL;
		pfree(*shmIngresQueuesRefCount);
		shmIngresQueuesRefCount = NULL;
	}


	/* deallocate heapScanProgress table here */
	if(heapScanProgressTable && *heapScanProgressTable)
	{
	  hash_destroy(*heapScanProgressTable);  
	  *heapScanProgressTable = NULL;
	}
	else
	{  
	  elog(NOTICE, "heapScanProgressTable hashtable is already freed");
	}

}


/*
 * @BdtcqpmMS
 * Cleanup routine for aborting the processing of the current query in
 * the TelegraphCQ backend.
 */
void
AbortCurrentQuery(void)
{
	elog(DEBUG1, "TCQbe: Don't yet know how to abort the current query");
}

/* @EdtcqpmMS */

int
TelegraphCQMain(int argc, char *argv[])
{
	char		usrbuf[ARGV_SIZE];
	int			status;
	CommandDest dest;
	const char *commandTag;
	char		completionTag[COMPLETION_TAG_BUFSIZE];
	planqentry *pqe = NULL;
	time_t		timeval;
	struct tm	structtm;
	char *nwd;

	nwd = getenv("TCQ_BE_GPROF_WD");
	if (nwd == NULL) 
	{
		nwd = getenv("PGDATA");
	}

	elog(LOG,"Find your gprof files starting with %s at %s ",
		 getenv("GMON_OUT_PREFIX"), nwd);
	on_proc_exit(restore_working_directory, (Datum) nwd);
	
	dest = ResultQueue;

	/*
	 * this is a stopgap fix as it would be nice to have a way to report
	 * errors cleanly between the backend and frontend processes via
	 * result queues.
	 *
	 * For now, just send to the Debug output.
	 */
	whereToSendOutput = Debug;


	StrNCpy(usrbuf, GetConfigOption("telegraphcquser"), ARGV_SIZE);

	status = PostgresMain(argc, argv, usrbuf);

	/* Set the process name so we can debug it later */
	set_ps_display("tcqbackend");

	/* Initialize the port for making parallel connections for an FSched */
	init_fsched_acceptfd();

	QueryContext = AllocSetContextCreate(TopMemoryContext,
										 "QueryContext",
										 ALLOCSET_DEFAULT_MINSIZE,
										 ALLOCSET_DEFAULT_INITSIZE,
										 ALLOCSET_DEFAULT_MAXSIZE);



	setupDynCatalogStreams();	/* Initialize the dynamic catalog streams */

	time(&timeval);
	localtime_r(&timeval, &structtm);
	tcqlocaltz = DetermineLocalTimeZone(&structtm);

	do
	{
		/*
		 * @BdtcqpmMS
		 *
		 * TelegraphCQ main processing loop. When elog(ERROR) is called or
		 * some exception, we return here. I stole this from PostgresMain
		 * function
		 */

		if (sigsetjmp(Warn_restart, 1) != 0)
		{


			/*
			 * NOTE: if you are tempted to add more code in this if-block,
			 * consider the probability that it should be in
			 * AbortCurrentQuery() instead.
			 *
			 * Make sure we're not interrupted while cleaning up.  Also
			 * forget any pending QueryCancel request, since we're
			 * aborting anyway. Force InterruptHoldoffCount to a known
			 * state in case we elog'd from inside a holdoff section.
			 */
			ImmediateInterruptOK = false;
			QueryCancelPending = false;
			InterruptHoldoffCount = 1;
			CritSectionCount = 0;		/* should be unnecessary, but... */

			/*
			 * Make sure we are in a valid memory context during recovery.
			 *
			 * We use ErrorContext in hopes that it will have some free space
			 * even if we're otherwise up against it...
			 */
			MemoryContextSwitchTo(ErrorContext);

			/* Do the recovery */
			elog(DEBUG2, "Whatever abort means doing it ...");

			AbortCurrentQuery();

			/*
			 * Now return to normal top-level context and clear
			 * ErrorContext for next time.
			 */
			MemoryContextSwitchTo(TopMemoryContext);
			MemoryContextResetAndDeleteChildren(ErrorContext);

			/*
			 * Clear flag to indicate that we got out of error recovery
			 * mode successfully.  (Flag was set in elog.c before
			 * longjmp().)
			 */
			InError = false;

			/*
			 * Exit interrupt holdoff section we implicitly established
			 * above.
			 */
			RESUME_INTERRUPTS();
			elog(FATAL, "TelegraphCQ does not know how to restart");
		}

		Warn_restart_ready = true;		/* we can now handle elog(ERROR) */

		PG_SETMASK(&UnBlockSig);
		*tcqbackendinitialized = TRUE;


		MemoryContextSwitchTo(QueryContext);
		MemoryContextResetAndDeleteChildren(QueryContext);


		/* tcqoc */
		if (UseCQEddy)
		{
/* 			vmon_begin(); */
			elog(LOG,"Starting CQEddy ... ");
			RunCQEddy();
/* 			vmon_done(); */
/* 			elog(LOG, "process query for CQEDDY returned!"); */
/* 			elog(LOG, "Let's be nice and give 300 seconds for the clients .."); */
/* 			tcq_usleep(900*1000000); */
/* 			elog(PANIC,"Hold your horses ! "); */
			if (-1 == MaxTuplesScannedBeforeQuit) 
			{
				elog(LOG,"Ending CQEddy ... Calling proc_exit(0)");
				proc_exit(0);
			}
			else 
			{
				elog(LOG,"Ending CQEddy ... Going to call elog(FATAL)");
				elog(FATAL,"Done processing CQEddy ! ");
			}
		}

		/* @EdtcqpmMS */

		elog(DEBUG1, "TelegraphCQMain:  Blocking on planqdequeue");
		dequeue(plan_queue, (char **) &pqe, true, &planqentry_serializer);
		elog(DEBUG1, "TelegraphCQMain: got plan plan=%p,qt=%p,rte=%p qno = %d !",
			 pqe->plan, pqe->querytree, pqe->rangetable, pqe->queueNo);
		Assert(pqe->queueNo == 0);


		/*
		 * execute the plan
		 */
		elog(DEBUG2, "ProcessQuery");

		/* Set the query id for Fjord scheduler */
		if (UseHashGroup)
		{

			if (IsA(pqe->plan, FSched))
				((FSched *) pqe->plan)->qno = pqe->queueNo;

		}

		/* Setup the snapshot */
		SetQuerySnapshot();

		if (pqe->querytree->querySource == QSRC_ORIGINAL)
		{
			/* original stmt can override default tag string */
			ProcessQuery(pqe->querytree, pqe->plan, dest, completionTag);
			commandTag = completionTag;
		}
		else
		{
			/* stmt added by rewrite cannot override tag */
			ProcessQuery(pqe->querytree, pqe->plan, dest, NULL);
		}

		/*
		 * Enqueue a sentinel to mark the end of tuples
		 */

		/*
		 * I know this is ugly. However, the Fjord scheduler is going to
		 * free the queue when the query cancellation message arrives. The
		 * front-end no-longer exists, so sending it a null tuple is
		 * pointless, and leaves a dangling queue. I leave this in here
		 * for backward compatibility.
		 */
		if (!UseHashGroup)
		{
			result		r;

			r.type = RESULT_DONE;
			enqueue(GetQueue(pqe->queueNo), (char *) &r, true,
					&result_serializer);
		}

		elog(DEBUG1, "about to delete query context");
		if (pqe->mctx != NULL)
		{
			SHMContextDelete(pqe->mctx);
			pfree(pqe);
			pqe = NULL;
		}
	}
	while (true);

	return status;
}

void
setTCQJoinOptions()
{
	orig_enable_seqscan = enable_seqscan;
	orig_enable_indexscan = enable_indexscan;
	orig_enable_tidscan = enable_tidscan;
	orig_enable_sort = enable_sort;
	orig_enable_nestloop = enable_nestloop;
	orig_enable_mergejoin = enable_mergejoin;
	orig_enable_hashjoin = enable_hashjoin;
	enable_seqscan = true;
	enable_indexscan = false;
	enable_tidscan = false;
	enable_sort = true;
	enable_nestloop = false;
	enable_mergejoin = false;
	enable_hashjoin = true;
}

void
resetJoinOptions()
{
	enable_seqscan = orig_enable_seqscan;
	enable_indexscan = orig_enable_indexscan;
	enable_tidscan = orig_enable_tidscan;
	enable_sort = orig_enable_sort;
	enable_nestloop = orig_enable_nestloop;
	enable_mergejoin = orig_enable_mergejoin;
	enable_hashjoin = orig_enable_hashjoin;
}

/*
 *	This function returns the current timestamp every time it's called - no
 *	matter how many times in the same statement/transaction/context whatever
 */
Timestamp
GetCurrentTimestamp()
{
	Timestamp	timestamp;
	struct tm	tt;
	char		zone[MAXDATELEN + 1];
	struct tm  *tm = &tt;
	char	   *tzn = zone;
	AbsoluteTime abstime = GetCurrentAbsoluteTime();

	abstime2tm(abstime, &tcqlocaltz, tm, &tzn);
	if (tm2timestamp(tm, 0, NULL, &timestamp) != 0)
	{
		elog(ERROR, "Unable to convert ABSTIME to TIMESTAMP"
			 "\n\tGetCurrentTimestamp() internal error");
	}

	return timestamp;
}
