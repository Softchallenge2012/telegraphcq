/*-------------------------------------------------------------------------
 *
 * miscadmin.h
 *	  this file contains general postgres administration and initialization
 *	  stuff that used to be spread out between the following files:
 *		globals.h						global variables
 *		pdir.h							directory path crud
 *		pinit.h							postgres initialization
 *		pmod.h							processing modes
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: miscadmin.h,v 1.59 2005/07/26 20:00:31 phred Exp $
 *
 * NOTES
 *	  some of the information in this file should be moved to
 *	  other files.
 *
 *-------------------------------------------------------------------------
 */
#ifndef MISCADMIN_H
#define MISCADMIN_H

#include "nodes/plannodes.h"	/* @dtcqSK */
#include "nodes/parsenodes.h"	/* @dtcqSK */
#include "rqueue/resultq.h"		/* @dtcqSK */
#include "storage/ipc.h"		/* @dtcqSK */
#include "storage/s_lock.h"
#include "telegraphcq/telegraphcqinit.h"		/* @dtcqSK */
#include "telegraphcq/fluxplans.h" /* @dfluxMS */

/*****************************************************************************
 *	  System interrupt and critical section handling
 *
 * There are two types of interrupts that a running backend needs to accept
 * without messing up its state: QueryCancel (SIGINT) and ProcDie (SIGTERM).
 * In both cases, we need to be able to clean up the current transaction
 * gracefully, so we can't respond to the interrupt instantaneously ---
 * there's no guarantee that internal data structures would be self-consistent
 * if the code is interrupted at an arbitrary instant.	Instead, the signal
 * handlers set flags that are checked periodically during execution.
 *
 * The CHECK_FOR_INTERRUPTS() macro is called at strategically located spots
 * where it is normally safe to accept a cancel or die interrupt.  In some
 * cases, we invoke CHECK_FOR_INTERRUPTS() inside low-level subroutines that
 * might sometimes be called in contexts that do *not* want to allow a cancel
 * or die interrupt.  The HOLD_INTERRUPTS() and RESUME_INTERRUPTS() macros
 * allow code to ensure that no cancel or die interrupt will be accepted,
 * even if CHECK_FOR_INTERRUPTS() gets called in a subroutine.	The interrupt
 * will be held off until CHECK_FOR_INTERRUPTS() is done outside any
 * HOLD_INTERRUPTS() ... RESUME_INTERRUPTS() section.
 *
 * Special mechanisms are used to let an interrupt be accepted when we are
 * waiting for a lock or when we are waiting for command input (but, of
 * course, only if the interrupt holdoff counter is zero).	See the
 * related code for details.
 *
 * A related, but conceptually distinct, mechanism is the "critical section"
 * mechanism.  A critical section not only holds off cancel/die interrupts,
 * but causes any elog(ERROR) or elog(FATAL) to become elog(STOP) --- that is,
 * a system-wide reset is forced.  Needless to say, only really *critical*
 * code should be marked as a critical section!  Currently, this mechanism
 * is only used for XLOG-related code.
 *
 *****************************************************************************/

/* in globals.c */
/* these are marked volatile because they are set by signal handlers: */
extern DLLIMPORT volatile bool InterruptPending;
extern volatile bool QueryCancelPending;
extern volatile bool ProcDiePending;

/* these are marked volatile because they are examined by signal handlers: */
extern volatile bool ImmediateInterruptOK;
extern volatile uint32 InterruptHoldoffCount;
extern volatile uint32 CritSectionCount;

/* in postgres.c */
extern void ProcessInterrupts(void);

#define CHECK_FOR_INTERRUPTS() \
	do { \
		if (InterruptPending) \
			ProcessInterrupts(); \
	} while(0)

#define HOLD_INTERRUPTS()  (InterruptHoldoffCount++)

#define RESUME_INTERRUPTS() \
	do { \
		Assert(InterruptHoldoffCount > 0); \
		InterruptHoldoffCount--; \
	} while(0)

#define START_CRIT_SECTION()  (CritSectionCount++)

#define END_CRIT_SECTION() \
	do { \
		Assert(CritSectionCount > 0); \
		CritSectionCount--; \
	} while(0)


/*****************************************************************************
 *	  globals.h --															 *
 *****************************************************************************/

/*
 * from postmaster/postmaster.c
 */
extern bool IsUnderPostmaster;
extern bool ClientAuthInProgress;

extern int	PostmasterMain(int argc, char *argv[]);
extern void ClosePostmasterPorts(bool pgstat_too);

/*
 * from utils/init/globals.c
 */
extern bool Noversion;
extern char *DataDir;

extern DLLIMPORT int MyProcPid;
extern struct Port *MyProcPort;
extern long MyCancelKey;

extern char OutputFileName[];
extern char pg_pathname[];

/* Identifies the role of the process */
typedef enum __proc_t
{
	PROC_TCQ_FRONTEND = 0,		/* TelegraphCQ front-end process */
	PROC_TCQ_BACKEND,			/* TelegraphCQ back-end process */
	PROC_TCQ_WRAP_CH,			/* TelegraphCQ wrapper clearing house */
	PROC_HIFI_GLUE,                         /* TelegraphCQ Glue */
	PROC_POSTGRES				/* Normal postgres process, always last! */

}	proc_t;

extern bool UseHashGroup;		/* @dhashgrpMS */

extern bool UseCQEddy;			/* @tcqoc */
extern bool UseTelegraphCQ;		/* @dtcqSK */

/* extern bool IsTelegraphCQFrontend; */
/* extern bool IsTelegraphCQBackend; */
extern bool SharedMemoryTest;	/* @dtcqOC */
extern struct TupleDesc tupledesc;		/* @dtcqOC */
extern proc_t MyProcessType;	/* @dtcqMS */

extern bool enable_eddy;		/* @dstemsAD */
extern bool UseSteMHashJoins;	/* @dstemsAD */
extern bool RunSQinFE;              /* @daaGJ */

extern bool UseMigrateState;	/* @dstemsAD */
extern bool UseChangeRoutingPolicy;	/* @dstemsAD */
extern bool UseBaseSteMs;	/* @dstemsAD */
extern bool UseFastPathEddyCreate;		/* @dstemsAD */
extern bool UseMetaEddy;                                          /* @dmetaSK */
extern bool UsePrecisionSharing;
extern bool UseOperatorPrecedence;
extern bool UseTupleLineage;                                    /* @dsigmodSK */
extern bool UseSuppressOutput;
extern char *eddy_config_file_name;		                         /* @dstemsAD */
extern bool enable_smart_routing;		                           /* @dtcqSK */
extern bool enable_hifi;                                          /* @dhifiSK */
extern bool enable_memory_debugging;
extern bool fstempushfilter;
extern bool cqeddyprinttuplepath;
extern int  cqeddycheckplanqiters;
extern int  cqeddycheckcontrolqiters;
extern int  cqeddycheckcancelediters;
extern int  tcqresultqsize;
extern int  tcqrecursiveresultqsize;
extern int  cqeddyfixedplan;
extern int  MaxTuplesScannedBeforeQuit;                         /* @dsigmodSK */

extern bool enable_hifi;                  /* @dglueEW */

extern bool elog_transactions;

/* Progress tables of heap scan modules for multiple backends */
extern HTAB **heapScanProgressTable;
extern volatile slock_t *heapScanProgressTableSpinLock;

typedef int* HeapProgressArray;
typedef struct
{
  Oid relid;
  HeapProgressArray progress;
} HeapProgressLookupEnt;

#define PROGRESS_ARRAY_SIZE TCQMAXSOURCES

/*																	    @BdtcqSK
 * TelegraphCQ global data structures
 *
 * There are two forms of process-local global pointers. Those that
 * point to shared memory data structures that are allocated only once
 * (eg. result_queue, semaphore), and those that point to shared
 * memory data structures that are allocated repeatedly (eg. compiled
 * queryplan).
 *
 * For the former, it is sufficient to have a global pointer to the data
 * structure. For the latter, these globals are pointers to shared memory
 * pointers to shared memory data structures necessary for the TCQ
 * Plumbing. Whenever new data structures are added, make sure you do the
 * following:
 *
 *	 1. Define a pointer to the data structure here
 *	 2. Add code to initialize the data structure in InitializeTCQPlumbing()
 *	 3. Get your code reviewed by someone
 */

/* Variables allocated once for all processes in shared memory */
extern bool *init_plumbing;
extern bool *tcqbackendinitialized;
extern bool *tcqwrapchinitialized;

extern volatile slock_t *p_qp_spinlock;
extern Queue **p_plan_queue;
extern Queue **p_control_queue;

/* The control queue to the wrapper clearinghouse */
extern Queue **p_wrap_control_queue;

extern Queue ***shmResultQueues;
extern bool **shmResultQueuesInit;
extern bits8 *shmResultQueueBitmap;
extern volatile slock_t *shmResultQueueBitmapLock;
extern Queue ***shmIngresQueues;
extern char ***shmIngresQueuesInit;
extern int **shmIngresQueuesRefCount;
extern TupleDesc **shmIngresQueuesTDesc;
extern int32 cqcancelid;		/* CQ Query Cancelation */


/* Process-local variables that point to a structure in shared-memory that
   may get freed and allocate somewhere else in shared-memory. */

extern Queue *plan_queue;		/* tcqoc */
extern void wakeup_eddy(void);

extern struct queue_lock **p_no_lock;

#define NUM_HASH_GROUPS 10000

/* Flux global variables, these are not necessarily all-global, just machine global */
extern int	fluxPort;
extern int	telegraph_nodeid;
extern int	fsched_acceptfd;
extern int	total_machines;
extern int	report_freq;
extern char *gprof_tcqbackend_directory;
extern int	flux_queue_size;
extern int	ack_queue_size;
extern int	tsb_size;
extern int	is_pair;
extern bool failed_node;
extern int	which_failed;
extern char *node_addr;
extern int	sm_queue_size;
extern fdflow_t   *fdataflow;

/* Globals for load-shedding configuration. */
extern char *load_shedding_type;
extern char *load_shedding_synopsis_interval;
extern char *load_shedding_delay_constraint;
extern char *load_shedding_first_window_start;
extern int load_shedding_num_hist_buckets;
extern int load_shedding_synopsis_buffer_size;
extern int load_shedding_square_bucket_width;
extern int load_shedding_sample_size;
extern int load_shedding_synopsis_delay;
extern int load_shedding_synopsis_op_delay;
extern bool load_shedding_disable_kept_summaries;

extern bool tcq_show_punct_in_output;




extern Timestamp * tcqwtime;

/*
 * done in storage/backendid.h for now.
 *
 * extern BackendId    MyBackendId;
 */
extern DLLIMPORT Oid MyDatabaseId;

/* Date/Time Configuration
 *
 * Constants to pass info from runtime environment:
 *	USE_POSTGRES_DATES specifies traditional postgres format for output.
 *	USE_ISO_DATES specifies ISO-compliant format for output.
 *	USE_SQL_DATES specified Oracle/Ingres-compliant format for output.
 *	USE_GERMAN_DATES specifies German-style dd.mm/yyyy date format.
 *
 * DateStyle specifies preference for date formatting for output.
 * EuroDates if client prefers dates interpreted and written w/European conventions.
 *
 * HasCTZSet if client timezone is specified by client.
 * CDayLight is the apparent daylight savings time status.
 * CTimeZone is the timezone offset in seconds.
 * CTZName is the timezone label.
 */

#define MAXTZLEN		10		/* max TZ name len, not counting tr. null */

#define USE_POSTGRES_DATES		0
#define USE_ISO_DATES			1
#define USE_SQL_DATES			2
#define USE_GERMAN_DATES		3

extern int	DateStyle;
extern bool EuroDates;
extern bool HasCTZSet;
extern bool CDayLight;
extern int	CTimeZone;
extern char CTZName[];

extern bool enableFsync;
extern bool allowSystemTableMods;
extern DLLIMPORT int SortMem;
extern int	VacuumMem;

/*
 *	A few postmaster startup options are exported here so the
 *	configuration file processor can access them.
 */

extern bool NetServer;
extern bool EnableSSL;
extern bool SilentMode;
extern int	MaxBackends;
extern int	ReservedBackends;
extern int	NBuffers;
extern int	PostPortNumber;
extern int	Unix_socket_permissions;
extern char *Unix_socket_group;
extern char *UnixSocketDir;
extern char *VirtualHost;


/*****************************************************************************
 *	  pdir.h --																 *
 *			POSTGRES directory path definitions.							 *
 *****************************************************************************/

extern char *DatabaseName;
extern char *DatabasePath;

/* in utils/misc/database.c */
extern void GetRawDatabaseInfo(const char *name, Oid *db_id, char *path);
extern char *ExpandDatabasePath(const char *path);

/* now in utils/init/miscinit.c */
extern void SetDatabaseName(const char *name);
extern void SetDatabasePath(const char *path);

extern char *GetUserNameFromId(Oid userid);

extern Oid	GetUserId(void);
extern void SetUserId(Oid userid);
extern Oid	GetSessionUserId(void);
extern void SetSessionUserId(Oid userid);
extern void InitializeSessionUserId(const char *username);
extern void InitializeSessionUserIdStandalone(void);
extern void SetSessionAuthorization(Oid userid);

extern void SetDataDir(const char *dir);

extern int FindExec(char *full_path, const char *argv0,
		 const char *binary_name);
extern int	CheckPathAccess(char *path, char *name, int open_mode);

#ifdef CYR_RECODE
extern void SetCharSet(void);
extern char *convertstr(unsigned char *buff, int len, int dest);
#endif

/* in utils/misc/superuser.c */
extern bool superuser(void);	/* current user is superuser */
extern bool superuser_arg(Oid userid);	/* given user is superuser */
extern bool is_dbadmin(Oid dbid);		/* current user is owner of
										 * database */


/*****************************************************************************
 *	  pmod.h --																 *
 *			POSTGRES processing mode definitions.							 *
 *****************************************************************************/

/*
 * Description:
 *		There are three processing modes in POSTGRES.  They are
 * BootstrapProcessing or "bootstrap," InitProcessing or
 * "initialization," and NormalProcessing or "normal."
 *
 * The first two processing modes are used during special times. When the
 * system state indicates bootstrap processing, transactions are all given
 * transaction id "one" and are consequently guaranteed to commit. This mode
 * is used during the initial generation of template databases.
 *
 * Initialization mode: used while starting a backend, until all normal
 * initialization is complete.	Some code behaves differently when executed
 * in this mode to enable system bootstrapping.
 *
 * If a POSTGRES binary is in normal mode, then all code may be executed
 * normally.
 */

typedef enum ProcessingMode
{
	BootstrapProcessing,		/* bootstrap creation of template database */
	InitProcessing,				/* initializing system */
	NormalProcessing			/* normal processing */
} ProcessingMode;

extern ProcessingMode Mode;

#define IsBootstrapProcessingMode() ((bool)(Mode == BootstrapProcessing))
#define IsInitProcessingMode() ((bool)(Mode == InitProcessing))
#define IsNormalProcessingMode() ((bool)(Mode == NormalProcessing))

#define SetProcessingMode(mode) \
	do { \
		AssertArg((mode) == BootstrapProcessing || \
				  (mode) == InitProcessing || \
				  (mode) == NormalProcessing); \
		Mode = (mode); \
	} while(0)

#define GetProcessingMode() Mode


/*****************************************************************************
 *	  pinit.h --															 *
 *			POSTGRES initialization and cleanup definitions.				 *
 *****************************************************************************/

/* in utils/init/postinit.c */
extern void InitPostgres(const char *dbname, const char *username);
extern void BaseInit(void);

/* in utils/init/miscinit.c */
extern bool CreateDataDirLockFile(const char *datadir, bool amPostmaster);
extern bool CreateSocketLockFile(const char *socketfile, bool amPostmaster);
extern void TouchSocketLockFile(void);
extern void RecordSharedMemoryInLockFile(unsigned long id1,
							 unsigned long id2);

extern void ValidatePgVersion(const char *path);

/* these externs do not belong here... */
extern void IgnoreSystemIndexes(bool mode);
extern bool IsIgnoringSystemIndexes(void);
extern bool IsCacheInitialized(void);

#endif   /* MISCADMIN_H */
