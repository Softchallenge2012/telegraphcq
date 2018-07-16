/*-------------------------------------------------------------------------
 * @dtcqpmMS
 * ingresq.c
 * Author: Mehul Shah
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *	These are functions for allocating and free result queues for
 *	wrappers (ingres) operators.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/rqueue/ingresq.c,v 1.14 2004/03/24 08:11:06 chungwu Exp $
 *
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "miscadmin.h"

#include "access/heapam.h"
#include "rqueue/ingresq.h"
#include "rqueue/rqdest.h"
#include "storage/spin.h"
#include "telegraphcq/telegraphcq.h"
#include "telegraphcq/wrapch.h"
#include "utils/memutils.h"

/*
 *
 * Functions to activate and deactivate Ingres queues.
 * The queues themselves are just result queues.
 *
 * the get family of functions are just ways to look up the queues, and
 * are not reference counted.  It is assumed that any caller that uses
 * these functions will also call AllocIngresQ and FreeIngresQ to
 * ensure that they have recorded the use of the queue.
 */


int			adjustRefCount(int qidx, int amt);

/* Assumes the queue exists already */
Queue *
getIngresQ(IngresQIdx idx)
{
	/* this function does not effect the reference count */
	Queue	   *res;

	SpinLockAcquire(p_qp_spinlock);
	res = (*shmIngresQueues)[idx];
	SpinLockRelease(p_qp_spinlock);
	return res;
}

/* returns null if the queue does not exist */
Queue *
getIngresQ_byName(char *name)
{
	int			i = 0;
	Queue	   *res = NULL;

	SpinLockAcquire(p_qp_spinlock);
	while ((i < NUM_INGRES_QUEUES) && (res == NULL))
	{
		if (!strcmp(name, (*shmIngresQueuesInit)[i]))
			res = (*shmIngresQueues)[i];
		i++;
	}
	SpinLockRelease(p_qp_spinlock);

	return res;
}

/* For the next two routines, the caller must make sure the
   tupledesciptor is in an accessible memory context. Otherwise bad
   things will happen */

TupleDesc
getTupleDesc(IngresQIdx idx)
{

	TupleDesc	res;

	SpinLockAcquire(p_qp_spinlock);
	res = (*shmIngresQueuesTDesc)[idx];
	SpinLockRelease(p_qp_spinlock);
	return res;
}

void
setTupleDesc(IngresQIdx idx, TupleDesc tdesc)
{

	SpinLockAcquire(p_qp_spinlock);
	(*shmIngresQueuesTDesc)[idx] = tdesc;
	SpinLockRelease(p_qp_spinlock);
}

/*
 * Allocate an ingres queue.
 * If size == 0, initializes the defaults size.
 * returns the index of the queue.
 *
 * If one exists by that name, it just returns it.
 *
 * return -1, if no queues remain.
 */
IngresQIdx
allocIngresQ(int size, char *name)
{
	int			i = 0;
	int			res = -1;

	if (size == 0)
		size = DEF_INGRES_QSIZE;

	SpinLockAcquire(p_qp_spinlock);
	while ((i < NUM_INGRES_QUEUES) && (res == -1))
	{
		if ((*shmIngresQueuesInit)[i] == NULL)
		{
			MemoryContext oldContext =
			MemoryContextSwitchTo(TopSharedMemoryContext);

			/*
			 * Need to make a spin-lock so we don't block on empty and
			 * full queue
			 */
			struct queue_lock *ql;

			make_lock_sl(&ql);
			make_queue(&((*shmIngresQueues)[i]), ql, size);
			/* make_queue(&((*shmIngresQueues)[i]), NULL, size); */
			(*shmIngresQueuesInit)[i] = palloc(strlen(name) + 1);
			strcpy((*shmIngresQueuesInit)[i], name);
			MemoryContextSwitchTo(oldContext);
			res = i;
			adjustRefCount(i, 1);
			Assert((*shmIngresQueuesRefCount)[i] == 1);

			/* Does a queue of that name exist already? */
		}
		else if (!strcmp(name, (*shmIngresQueuesInit)[i]))
		{
			res = i;
			adjustRefCount(i, 1);
		}

		i++;
	}
	SpinLockRelease(p_qp_spinlock);

	return res;
}

/*
 * Releases the space allocated by the queue at index idx and sets the
 * pointer to that queue to null and marks the init array
 * appropriately.
 */
void
freeIngresQ(IngresQIdx idx)
{

	SpinLockAcquire(p_qp_spinlock);

	if ((*shmIngresQueuesInit)[idx] != NULL)
	{

		if (adjustRefCount(idx, -1) == 0)
		{
			MemoryContext oldContext =
			MemoryContextSwitchTo(TopSharedMemoryContext);

			free_queue(&((*shmIngresQueues)[idx]));
			(*shmIngresQueues)[idx] = NULL;
			pfree((*shmIngresQueuesInit)[idx]);
			(*shmIngresQueuesInit)[idx] = NULL;
			MemoryContextSwitchTo(oldContext);
			elog(NOTICE, "deleted ingres queue %d", idx);
		}

		SpinLockRelease(p_qp_spinlock);
	}
}

int
adjustRefCount(int qidx, int amt)
{
	/* must own lock */
	int			ret = 0;

	(*shmIngresQueuesRefCount)[qidx] += amt;
	ret = (*shmIngresQueuesRefCount)[qidx];
	elog(NOTICE, "adjusting refcount for queue %d by %d.  the count is %d",
		 qidx, amt, ret);
	return ret;
}

int
getIngresQueueRefCount(int qidx)
{
	int			ret = 0;

	SpinLockAcquire(p_qp_spinlock);
	ret = (*shmIngresQueuesRefCount)[qidx];
	SpinLockRelease(p_qp_spinlock);
	return ret;
}

void
IngresQInsertTuple(Queue * inq, TupleDesc desc, Datum *values, char *nulls)
{
	bool		ret;
	result		r;
	HeapTuple	tuple;

	r.type = RESULT_HEAPTUPLE;
	tuple = heap_formtuple(desc, values, nulls);
	r.data.tuple = tuple;
	ret = enqueue(inq, (char *) &r, false, &result_serializer);

	heap_freetuple(tuple);
	return;
}
