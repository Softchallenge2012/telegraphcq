/*-------------------------------------------------------------------------
 *
 * ipci.c
 *	  POSTGRES inter-process communication initialization code.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/storage/ipc/ipci.c,v 1.22 2005/01/27 01:19:46 anile Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"


#include "miscadmin.h"
#include "access/clog.h"
#include "access/xlog.h"
#include "storage/bufmgr.h"
#include "storage/freespace.h"
#include "storage/ipc.h"
#include "storage/lmgr.h"
#include "storage/lwlock.h"
#include "storage/pg_sema.h"
#include "storage/pg_shmem.h"
#include "storage/pmsignal.h"
#include "storage/proc.h"
#include "storage/sinval.h"
#include "storage/spin.h"
#include "utils/bit.h"

extern int numResultQueues;

/*
 * CreateSharedMemoryAndSemaphores
 *		Creates and initializes shared memory and semaphores.
 *
 * This is called by the postmaster or by a standalone backend.
 * It is NEVER called by a backend forked from the postmaster;
 * for such a backend, the shared memory is already ready-to-go.
 *
 * If "makePrivate" is true then we only need private memory, not shared
 * memory.	This is true for a standalone backend, false for a postmaster.
 */
void
CreateSharedMemoryAndSemaphores(bool makePrivate,
								int maxBackends,
								int port)
{
	int			size;
	int			numSemas;
	PGShmemHeader *seghdr;

	/*
	 * Size of the Postgres shared-memory block is estimated via
	 * moderately-accurate estimates for the big hogs, plus 100K for the
	 * stuff that's too small to bother with estimating.
	 */
	size = BufferShmemSize();
	size += LockShmemSize(maxBackends);
	size += XLOGShmemSize();
	size += CLOGShmemSize();
	size += LWLockShmemSize();
	size += SInvalShmemSize(maxBackends);
	size += FreeSpaceShmemSize();
#ifdef STABLE_MEMORY_STORAGE
	size += MMShmemSize();
#endif
	size += 100000;
	/* might as well round it off to a multiple of a typical page size */
	size += 8192 - (size % 8192);

	elog(DEBUG2, "invoking IpcMemoryCreate(size=%d)", size);

	/*
	 * Create the shmem segment
	 */
	seghdr = PGSharedMemoryCreate(size, makePrivate, port);

	/*
	 * Create semaphores
	 */
	numSemas = ProcGlobalSemas(maxBackends);
	numSemas += SpinlockSemas();
	PGReserveSemaphores(numSemas, port);

	/*
	 * Set up shared memory allocation mechanism
	 */
	InitShmemAllocation(seghdr);

	/*
	 * Now initialize LWLocks, which do shared memory allocation and are
	 * needed for InitShmemIndex.
	 */
	CreateLWLocks();

	/*
	 * Set up shmem.c index hashtable
	 */
	InitShmemIndex();

	if (UseTelegraphCQ)			/* @BdtcqSK */
	{

		elog(DEBUG1, "Postmaster: CreateSharedMemoryAndSemaphores - for TelegraphCQ");

		/*
		 * Create and init the query spinlock
		 */
		p_qp_spinlock = (slock_t *) ShmemAlloc(sizeof(slock_t));
		SpinLockInit(p_qp_spinlock);

		/*
		 * Create the flag that indicates if plumbing is done
		 */
		init_plumbing = (bool *) ShmemAlloc(sizeof(bool));
		*init_plumbing = false;

		/*
		 * Allocate the pointer to result queue in shared memory
		 */
		tcqbackendinitialized = (bool *) ShmemAlloc(sizeof(bool));
		tcqwrapchinitialized = (bool *) ShmemAlloc(sizeof(bool));
		p_plan_queue = (Queue **) ShmemAlloc(sizeof(Queue *));
		p_control_queue = (Queue **) ShmemAlloc(sizeof(Queue *));
		p_wrap_control_queue = (Queue **) ShmemAlloc(sizeof(Queue *));
		shmResultQueues = (Queue ***) ShmemAlloc(sizeof(Queue **));
		shmResultQueuesInit = (bool **) ShmemAlloc(sizeof(bool *));
		shmResultQueueBitmap = (bits8 *) ShmemAlloc(sizeof(bits8) * BASIZE(numResultQueues));
		BitArrayRealClearAll(shmResultQueueBitmap, numResultQueues);
		shmResultQueueBitmapLock = (slock_t *) ShmemAlloc(sizeof(slock_t));
		SpinLockInit(shmResultQueueBitmapLock);
		//*shmResultQueueBitmap = 0;

		/*
		 * Allocate the pointer to wrapper queues in shared memory
		 */
		shmIngresQueues = (Queue ***) ShmemAlloc(sizeof(Queue **));
		shmIngresQueuesInit = (char ***) ShmemAlloc(sizeof(char **));
		shmIngresQueuesTDesc = (TupleDesc **) ShmemAlloc(sizeof(TupleDesc *));
		shmIngresQueuesRefCount = (int **) ShmemAlloc(sizeof(int **));

		/*
		 * Allocate the pointer to the no_lock structure
		 */
		p_no_lock = (struct queue_lock **) ShmemAlloc(sizeof(struct queue_lock *));

		/* For heap scan progress tables, allocate memory for the hash
		 * table and the spin lock
		 */
		heapScanProgressTable = (HTAB**)ShmemAlloc(sizeof(HTAB *));
		heapScanProgressTableSpinLock = (slock_t *)ShmemAlloc(sizeof(slock_t));
		SpinLockInit(heapScanProgressTableSpinLock);
	}

	/*
	 * Set up xlog, clog, and buffers
	 */
	XLOGShmemInit();
	CLOGShmemInit();
	InitBufferPool();

	/*
	 * Set up lock manager
	 */
	InitLocks();
	if (InitLockTable(maxBackends) == INVALID_TABLEID)
		elog(FATAL, "Couldn't create the lock table");

	/*
	 * Set up process table
	 */
	InitProcGlobal(maxBackends);

	/*
	 * Set up shared-inval messaging
	 */
	CreateSharedInvalidationState(maxBackends);

	/*
	 * Set up free-space map
	 */
	InitFreeSpaceMap();

	/*
	 * Set up child-to-postmaster signaling mechanism
	 */
	PMSignalInit();
}
