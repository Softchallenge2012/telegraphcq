/*-------------------------------------------------------------------------
 *
 * resultq.c
 *	  An element based queue used for communication in a variety of
 *	  contexts in TelegraphCQ.
 *
 *	 The queue is implemented using a fixed sized circular buffer which is
 *	 allocated in the MemoryContext that is current at the time the queue is
 *	 created.
 *
 * A queue serializer is required to enqueue and dequeue elements from the
 * queue. A serializer contains the following functions:
 *
 *	determine size: given a pointer to obj and possibly some extra info,
 *	  determine the amount of space the serialized representation will take.
 *	serialize: copy source element into the queue. return the size written
 * deserialize: recover the original structure from the flattened
 *	representation. return the queue size consumed.
 *
 *	 Queue functions:
 *	 enqueue	 - enequeues an element in the queue using the provided
 *				   queue serializer.
 *	 dequeue	 - dequeues an element using the queue_serializer passed
 *				   as an argument.
 *	 make_queue  - takes a size and locking strategy and creates a queue
 *				   in the current memory context.
 *
 *	 free_queue - releases storage allocated to the queue passed in.
 *
 *	 clear_queue- resets the provided queue as if it had just been
 *				  created.
 *	 check_capaciety - check to see if an element of a given size
 *					   would fit in the queue right now.  this
 *					   function does not reserve capacity.
 *
 *	 Queues may be allocated in shared memory by using the TopSharedMemory
 *	 context or a subcontext to allocate the queue.
 *
 *	 The queue supports several locking strategies using instances of
 *	 the queue_lock structure.	The queue lock structure has pointers to
 *	 functions that can be used for mutual exclusion:
 *	 mutex_lock -- obtain a lock for mutual exclusion purposes
 *	 mutex_unlock -- release the mutex
 *
 *	 The queue lock structure also has pointers to functions that can be used
 *	 to implement blocking queues by tracking the number of full and empty
 *	 slots in the queue.  The make_lock_sem function takes an argument that
 *	 specifies the maximum number of elements in the queue.  The queue_empty
 *	 semaphore is initially set to this count.	This is actually just an
 *	 estimate of the capacity.	The enqueue function will ensure that the
 *	 queue_empty semaphore count goes to 0 when the queue is actually
 *	 full.
 *
 *	 The only implementaions of these functions are
 *	 currently with semaphores
 *	 queue_full_up --  increment the count of the number of queue elements
 *	 queue_full_down -- decrement the count of the number of queue elements
 *	 queue_empty_up  -- increment the count of empty slots
 *	 queue_empty_down  -- decrement the count of empty slots
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/rqueue/resultq.c,v 1.42 2004/07/15 20:39:56 owenc Exp $
 *
 */

#include "postgres.h"
#include "rqueue/resultq.h"
#include "storage/s_lock.h"
#include "storage/ipc.h"
#include "sys/types.h"
#include "sys/wait.h"
#include <unistd.h>
#include <errno.h>
#include "miscadmin.h"

/*
 * if the following define is included, the result queue will do checking
 * and overwrite unused memory with recognizable values.
*/
#define RESULTQ_DEBUG


/* internal queue interface */
bool		enqueue_elt(Queue *, char *data, struct queue_serializer * qs);
bool		dequeue_elt(Queue *, char **data, struct queue_serializer * qs);

/* in semaphore based queues, two semaphores are used to keep track of the
   number of full and empty queue slots.  Since this is a variable length
   element queue, we may overestimate the number of free queue slots
   (in which case enqueue reduces the  empty semaphore to 0) or underestimate
	 it in which case we loose space.


   MAXELTEST caps the estimate for the number of elements a queue can hold
   at the maximum sempaphore value for the platform or 16K if that value is
   not available.
 */
#ifdef SEMVMX
#define MAXELTEST (SEMVMX-1)
#else
#define MAXELTEST 16383
#endif

#define MINELTEST 5

#define QHDRSZ (VARHDRSZ + sizeof(uint32))
#define ELTCHECK 0xccabbaee
#define PADSIZE 5
#define PAD 0x3f

/* this defines the internal representation of a queue element. */
struct qelt
{
	uint32		check;
	struct varlena v;
};

void
clear_queue(Queue * q)
{
	int			eltest = 0;
	TCQSemaphore semas;

	semas = (TCQSemaphore) (q->ql->lock_data);

	q->ql->mutex_lock(q->ql);
	q->head = q->tail = q->start;
	q->wall = q->end;
	q->bytesfree = VQSPACETOEND(q->start, q);
	q->numelts = 0;
	q->avgfree = 0;
	q->numcalls = 0;
	eltest = q->bytesfree / QHDRSZ;
	if (eltest > MAXELTEST)
	{
		eltest = MAXELTEST - 1;
	}
	if (IsSemaphoreLock(q->ql))
	{
		TCQSemaphoreInitialize(semas + QUEUELOCK_SEM_FULL, 0);
		TCQSemaphoreInitialize(semas + QUEUELOCK_SEM_EMPTY, eltest);
	}

	q->ql->mutex_unlock(q->ql);
}

bool
check_capacity(Queue * q, int size)
{
	bool		ret = false;

	q->ql->mutex_lock(q->ql);
	ret = QFREESPACE(q) >= (size + QHDRSZ);
	q->ql->mutex_unlock(q->ql);
	return ret;
}

/*
 * Returns a null queue if it could not contruct the queue. We probably should
 * have appropriate return values for errors.
 */

void
make_queue(Queue ** q, struct queue_lock * ql, int size)
{
	Queue	   *myqueue;
	int			eltest = size / QHDRSZ;

	/* Enough to hold the queue ? */
	if (size < sizeof(Queue))
	{
		*q = NULL;
		return;
	}

	/* The int is there for some data for non-blocking send of the queue */
	myqueue = (Queue *) palloc(size);

	memset(myqueue, 0, size);	/* Zero out the queue */

	if (eltest > MAXELTEST)
		eltest = MAXELTEST - 1;

	myqueue->size = size;
	myqueue->qid = 0;
	myqueue->start = (char *) (myqueue + 1);
	myqueue->end = ((char *) myqueue) + size;
	myqueue->head = myqueue->start;
	myqueue->tail = myqueue->head;
	myqueue->wall = myqueue->end;
	myqueue->bytesfree = VQSPACETOEND(myqueue->start, myqueue);
	myqueue->numelts = 0;
	myqueue->numcalls = 0;
	myqueue->avgfree = 0.0;

	if (eltest < MINELTEST)
		eltest = MINELTEST;
	if (ql)
	{
		/* takes ownership */
		myqueue->ql = ql;
	}
	else
		make_lock_sem(&myqueue->ql, eltest);
	*q = myqueue;
}


/*
 * At start qc->done must be false, and qc->done tells
 * you wether you are sone sending the queue.
 */
int
send_queue(QueueComm * qc, int sockfd, bool isBlocking)
{

	if (isBlocking)
		return send_queue_b(qc, sockfd);
	else
		return send_queue_nb(qc, sockfd);
}

/* Blocking send */
int
send_queue_b(QueueComm * qc, int sockfd)
{

	int			bread;

	qc->size = qc->queue->size;

	while (qc->pos < qc->size)
	{
		bread = write(sockfd, (((char *) qc->queue) + qc->pos),
					  (qc->size - qc->pos));
		if (bread < 0)
			return bread;		/* Error */
		qc->pos += bread;
	}

	qc->done = true;

	/* Done sending, clear the sucker */
	clear_queue(qc->queue);

	return true;
}

/* Non-blocking send */
int
send_queue_nb(QueueComm * qc, int sockfd)
{

	int			bread = 0;

#ifdef RESULTQ_DEBUG
	char		dummy = 59;
#endif

	qc->size = qc->queue->size;

#ifdef RESULTQ_DEBUG
	if (qc->pos == 0)
		elog(DEBUG1, "Sending %d bytes", qc->size);
#endif

	if (qc->pos < qc->size)
	{

		bread = write(sockfd, (((char *) qc->queue) + qc->pos),
					  (qc->size - qc->pos));

		if (bread < 0)
		{

			if ((errno != EAGAIN) && (errno != EINTR))
			{
				elog(DEBUG1, "send_queue_nb - errno: %d", errno);
				perror("send_queue_nb: ");
				return bread;
			}

			/* We just need to try again */
			return true;
		}

#ifdef RESULTQ_DEBUG
		if (bread < (qc->size - qc->pos))
			elog(DEBUG1, "Sent %d bytes", bread);
#endif

		qc->pos += bread;

		return bread;
	}

#ifdef RESULTQ_DEBUG
	bread = 0;

	dummy = 59;

	while (bread < 1)
		bread = write(sockfd, &dummy, 1);
#endif

	qc->done = true;

	/* Done sending, clear the sucker */
	clear_queue(qc->queue);

	return true;
}

void
swizzle_queue_ptrs(Queue * q)
{

	unsigned char *o_start;		/* previous start */
	int			my_free;

	/* First set the lock to be no-lock */
	q->ql = *p_no_lock;

	o_start = q->start;
	q->start = (char *) (q + 1);	/* Begin after the queue structure */
	q->end = (q->end - o_start) + q->start;
	q->head = (q->head - o_start) + q->start;
	q->tail = (q->tail - o_start) + q->start;
	q->wall = (q->wall - o_start) + q->start;


	/* Need an integrity check */
	if (q->head == q->wall)
		q->head = q->start;

	if (q->tail == q->head)
	{
		if (QEMPTY(q) || QFULL(q))
			return;
		Assert(false);
	}

	if (q->tail < q->head)
		my_free = (q->wall - q->head) + (q->tail - q->start);
	else
		my_free = (q->wall - q->tail) + (q->head - q->start);

	Assert(q->bytesfree == my_free);
}

/*
 * qc->queue must be a valid queue or NULL on the first call of
 * receive_queue, and qc->pos must be 0. qc->done must be false.
 *
 * qc->done is set to true when done receiving the queue.
 *
 * If the qc->queue is valid, but the wrong size, as specified by
 * qc->queue->size, then the queue is pfreed and reallocated to the
 * appropriate size.
 *
 * The socket sockfd must be set for non-blocking I/O if isBlocking is
 * false;
 */

int
receive_queue(QueueComm * qc, int sockfd, bool isBlocking)
{

	if (isBlocking)
		return receive_queue_b(qc, sockfd);
	else
		return receive_queue_nb(qc, sockfd);
}

int
receive_queue_b(QueueComm * qc, int sockfd)
{


	int			bread = 0;

	/* Probably should use alignment macros here. */
	qc->pos = 0;

	/* Read the size of the queue */
	while (qc->pos < sizeof(int))
	{
		bread = read(sockfd, ((char *) &(qc->size)) +qc->pos,
					 (sizeof(int) - qc->pos));
		if (bread < 0)
			return bread;		/* Error */
		qc->pos += bread;
	}

	if (qc->queue == NULL)
		qc->queue = palloc(qc->size);
	else if (qc->queue->size != qc->size)
	{
		pfree(qc->queue);
		qc->queue = palloc(qc->size);
	}

	/* Set the size in the queue structure */
	qc->queue->size = qc->size;

	/* Read the queue from the socket */
	while (qc->pos < qc->size)
	{
		bread = read(sockfd, ((char *) qc->queue) + qc->pos,
					 (qc->size - qc->pos));
		if (bread < 0)
			return bread;		/* Error */
		qc->pos += bread;
	}

	swizzle_queue_ptrs(qc->queue);

	qc->done = true;

	return true;
}


int
receive_queue_nb(QueueComm * qc, int sockfd)
{

	int			bread = 0;

#ifdef RESULTQ_DEBUG
	char		dummy = -59;
#endif

	/* Try and read the size */
	if (qc->pos < sizeof(int))
	{
		bread = read(sockfd, ((char *) &(qc->size)) +qc->pos,
					 (sizeof(int) - qc->pos));
	}


	if (bread < 0)
	{

		if ((errno != EAGAIN) && (errno != EINTR))
		{
			elog(DEBUG1, "receive_queue_nb - errno: %d", errno);
			perror("receive_queue_nb: ");
			return bread;
		}

		return true;
	}


#ifdef RESULTQ_DEBUG
	if (bread < (sizeof(int) - qc->pos))
		elog(DEBUG1, "Read %d bytes", bread);
#endif

	qc->pos += bread;

	if (qc->pos < sizeof(int))
		return bread;			/* Didn't finish reading size */

#ifdef RESULTQ_DEBUG
	elog(DEBUG1, "Read size: %d", qc->size);
#endif

/*	Assert((qc->size == 1024) || (qc->size == 256)); */

	if (qc->queue == NULL)
	{
#ifdef RESULTQ_DEBUG
		elog(DEBUG1, "Allocating a new queue");
#endif
		qc->queue = palloc(qc->size);
	}
	else if (qc->queue->size != qc->size)
	{
#ifdef RESULTQ_DEBUG
		elog(DEBUG1, "Allocating a new queue, previous size: %d",
			 qc->queue->size);
#endif
		pfree(qc->queue);
		qc->queue = palloc(qc->size);
	}

	qc->queue->size = qc->size;

	if (qc->pos < qc->size)
	{
		bread = read(sockfd, ((char *) qc->queue) + qc->pos,
					 (qc->size - qc->pos));
	}

	if (bread < 0)
	{

		if ((errno != EAGAIN) && (errno != EINTR))
		{
			elog(DEBUG1, "receive_queue_nb - errno: %d", errno);
			perror("receive_queue_nb: ");
			return bread;
		}

		return true;
	}

#ifdef RESULTQ_DEBUG
	if (bread < (qc->size - qc->pos))
		elog(DEBUG1, "Read %d bytes", bread);
#endif

	qc->pos += bread;

	if (qc->pos < qc->size)
		return bread;			/* Didn't finish reading buffer */

#ifdef RESULTQ_DEBUG
	bread = 0;
	dummy = -59;

	while (bread < 1)
		bread = read(sockfd, &dummy, 1);

	Assert(dummy == 59);
#endif

	swizzle_queue_ptrs(qc->queue);

	/* We are done with this one! */
	qc->done = true;
	return true;
}

QueueComm *
newQueueComm(Queue *q)
{
	QueueComm *qc;
	
	qc = palloc(sizeof(QueueComm));
	qc->queue  = q;
	qc->size   = (q != NULL) ? q->size : 0;
	qc->pos    = 0;
	qc->done   = (q != NULL);

	return qc;
}

void
free_queue(Queue ** q)
{
	void		(*fl) (struct queue_lock **);

	if ((*q)->ql != *p_no_lock)
	{

		fl = (*q)->ql->free_lock;
		fl(&((*q)->ql));

	}

	pfree(*q);

	(*q) = NULL;
}

bool
enqueue(Queue * q, char *data, bool isBlocking, struct queue_serializer * qs)
{
	int			tries = (q->end - q->start) / QHDRSZ;
	int			orig_tries = tries;
	bool		ret = false;
	bool		breakout = false;

    Assert(tries > 0);

	/*
	 * loop is necessary because varlena's are variable length structures.
	 * The semaphore queue_lock code overestimates the number of empty
	 * slots in the queue, so the queue may fill up (enqueue_elt returns
	 * false) before the empty slots count = 0
	 */

	while (breakout == false && ret == false && (tries-- > 0))
	{

		if (IsSemaphoreLock(q->ql))
		{

			q->ql->empty_down(q->ql, !isBlocking);		/* decrement the number
														 * of empty slots */
		}
		q->ql->mutex_lock(q->ql);
		ret = enqueue_elt(q, data, qs);
		if (QEMPTY(q))
			breakout = true;
		q->ql->mutex_unlock(q->ql);
		if (ret && IsSemaphoreLock(q->ql))
		{
			q->ql->full_up(q->ql, !isBlocking); /* increment the number of
												 * full slots */
		}
		if (!isBlocking)
			break;
	}
    if (tries <= 0) {
        elog(WARNING, "enqueue(): Giving up after %d tries.",
                orig_tries); 
    }
    /*
	if (tries <= 0)
		elog(LOG, " tries <=0 enqueue giving up \n");
        */
	return ret;
}


bool
dequeue(Queue * q, char **data, bool isBlocking, struct queue_serializer * qs)
{
	bool		ret = false;

	if (IsSemaphoreLock(q->ql))
		q->ql->full_down(q->ql, !isBlocking);
	q->ql->mutex_lock(q->ql);
	ret = dequeue_elt(q, data, qs);
	q->ql->mutex_unlock(q->ql);
	if (IsSemaphoreLock(q->ql))
	{

		if (ret)
		{
			q->ql->empty_up(q->ql, !isBlocking);		/* increment the number
														 * of empty slots */
		}
	}

	return ret;
}

bool
enqueue_elt(Queue * q, char *data, struct queue_serializer * qs)
{
	int			eltsize = qs->determineSize(data, NULL) + QHDRSZ + PADSIZE;
	int			spacetoend = VQSPACETOEND(q->tail, q);
	int			spaceavail = 0;
	bool		qfull = false;

	if (QFULL(q))
		return false;

	/* the wall doesn't matter for enqueue, so if we reach it  */
	/* then reset it to the end. */

	if (q->tail >= q->wall)
		q->wall = q->end;

	/* do we have enough space? */
	/* since we checked for full head==tail means empty here */

	if (q->tail < q->head)
		spaceavail = (q->head - q->tail);
	else
	{
		if (spacetoend < eltsize)
		{
#ifdef RESULTQ_DEBUG
			memset(q->tail, 0xff, spacetoend);
#endif
			q->wall = q->tail;
			q->tail = q->start;
			spacetoend = VQSPACETOEND(q->tail, q);
			qfull = QFULL(q);
			if (qfull)
				return false;
		}
		if (q->tail < q->head)
		{
			spaceavail = (q->head - q->tail);

		}
		else
			spaceavail = spacetoend;

	}

	if (spaceavail < eltsize)
		return false;
	else
	{
#ifdef RESULTQ_DEBUG
		int			padidx = 0;
		int			padbase = 0;
#endif
		struct qelt *qelt = (struct qelt *) q->tail;

		qelt->check = ELTCHECK;
		qelt->v.vl_len = eltsize;
#ifdef RESULTQ_DEBUG
		memset(qelt->v.vl_dat, PAD, eltsize - QHDRSZ);
#endif
		qs->serialize(data, qelt->v.vl_dat, (eltsize - QHDRSZ - PADSIZE), NULL);

#ifdef RESULTQ_DEBUG
		for (padbase = qelt->v.vl_len - QHDRSZ - PADSIZE, padidx = 0; padidx < PADSIZE; padidx++)
		{
			if ((qelt->v.vl_dat)[padbase + padidx] != PAD)
				elog(NOTICE, "RESULTQ: serialize overwrote memory in the padbuffer\n");
		}
#endif

		q->tail += eltsize;
		q->numelts++;
		q->avgfree = (q->numcalls * q->avgfree + q->bytesfree) / (q->numcalls + 1);
		q->numcalls++;
		q->bytesfree -= eltsize;
		if (q->bytesfree < 0)
			elog(ERROR, "RESULTQ: q->bytesfree is negative !!!!!!!!\n");
	}

	return true;

}

bool
dequeue_elt(Queue * q, char **data, struct queue_serializer * qs)
{
	int			eltsize = 0;
	struct qelt *qelt = NULL;

	if (q->bytesfree == VQSPACETOEND(q->start, q))
		return false;

	if (q->head > q->wall)
	{
		elog(ERROR, "RESULTQ: q->head is past the wall... head=%p, wall=%p, end=%p, numelts=%d, bytesfree=%d \n", q->head, q->wall, q->end, q->numelts, q->bytesfree);
		return false;
	}
	if (q->head == q->wall)
	{
		q->head = q->start;
		q->wall = q->end;
	}


	qelt = (struct qelt *) q->head;
	eltsize = VARSIZE(&(qelt->v));
	if (qelt->check != ELTCHECK)
	{
		elog(ERROR, "RESULTQ: dequeue found an invalid element in the queue");
		return false;
	}

	qs->deserialize(qelt->v.vl_dat, data, NULL);
#ifdef RESULTQ_DEBUG
	memset(q->head, 0x0, eltsize);
#endif
	q->head += eltsize;
	q->bytesfree += eltsize;
	q->numelts--;
	return true;
}

void
make_lock_sem(struct queue_lock ** ql, int empties)
{
	TCQSemaphore semas;

	*ql = palloc(sizeof(struct queue_lock));
	(*ql)->mutex_lock = queue_mutex_lock_sem;
	(*ql)->mutex_unlock = queue_mutex_unlock_sem;
	(*ql)->empty_up = queue_empty_up_sem;
	(*ql)->empty_down = queue_empty_down_sem;
	(*ql)->full_up = queue_full_up_sem;
	(*ql)->full_down = queue_full_down_sem;
/*	(*ql)->lock_data = palloc(sizeof(IpcSemaphoreId)); */
/*	*((IpcSemaphoreId *) (*ql)->lock_data) = IpcSemaphoreCreate(3, IPCProtection, 0, false); */

	(*ql)->lock_data = palloc(sizeof(TCQSemaphoreData) * 3);
	semas = (TCQSemaphore) ((*ql)->lock_data);

	TCQSemaphoreCreate(semas + QUEUELOCK_SEM_MUTEX);
	TCQSemaphoreCreate(semas + QUEUELOCK_SEM_FULL);
	TCQSemaphoreCreate(semas + QUEUELOCK_SEM_EMPTY);

	(*ql)->free_lock = free_lock_sem;

/*	IpcSemaphoreSetValue(*(IpcSemaphoreId*)(*ql)->lock_data,QUEUELOCK_SEM_MUTEX,1); */
/*	IpcSemaphoreSetValue(*(IpcSemaphoreId*)(*ql)->lock_data,QUEUELOCK_SEM_FULL,0); */
/*	IpcSemaphoreSetValue(*(IpcSemaphoreId*)(*ql)->lock_data,QUEUELOCK_SEM_EMPTY,empties); */

	TCQSemaphoreInitialize(semas + QUEUELOCK_SEM_MUTEX, 1);
	TCQSemaphoreInitialize(semas + QUEUELOCK_SEM_FULL, 0);
	TCQSemaphoreInitialize(semas + QUEUELOCK_SEM_EMPTY, empties);
}


void
print_sem_lock_values(struct queue_lock * ql)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	if (!IsSemaphoreLock(ql))
		return;
	elog(DEBUG1,
		 "Semaphore Lock values for semId %d: full count is %d, "
		 "current empty count is %d",
		 semas->semId,
		 TCQSemaphoreGetValue(semas + QUEUELOCK_SEM_FULL),
		 TCQSemaphoreGetValue(semas + QUEUELOCK_SEM_EMPTY));
}

void
free_lock_sem(struct queue_lock ** ql)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) ((*ql)->lock_data);

	TCQSemaphoreKill(semas + QUEUELOCK_SEM_MUTEX);
	TCQSemaphoreKill(semas + QUEUELOCK_SEM_FULL);
	TCQSemaphoreKill(semas + QUEUELOCK_SEM_EMPTY);

	pfree((*ql)->lock_data);
	pfree(*ql);
	*ql = NULL;
}



void
queue_mutex_lock_sem(struct queue_lock * ql)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	TCQSemaphoreLock(semas + QUEUELOCK_SEM_MUTEX, true);
}


void
queue_mutex_unlock_sem(struct queue_lock * ql)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	TCQSemaphoreUnlock(semas + QUEUELOCK_SEM_MUTEX);
}

bool
queue_full_up_sem(struct queue_lock * ql, bool trylock)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	TCQSemaphoreUnlock(semas + QUEUELOCK_SEM_FULL);
	return true;
}

bool
queue_full_down_sem(struct queue_lock * ql, bool trylock)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	if (trylock)
		return TCQSemaphoreTryLock(semas + QUEUELOCK_SEM_FULL);
	else
		TCQSemaphoreLock(semas + QUEUELOCK_SEM_FULL, true);
	return true;
}

bool
queue_empty_up_sem(struct queue_lock * ql, bool trylock)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	TCQSemaphoreUnlock(semas + QUEUELOCK_SEM_EMPTY);
	return true;
}
bool
queue_empty_down_sem(struct queue_lock * ql, bool trylock)
{
	TCQSemaphore semas;

	semas = (TCQSemaphore) (ql->lock_data);

	if (trylock)
		return TCQSemaphoreTryLock(semas + QUEUELOCK_SEM_EMPTY);
	else
		TCQSemaphoreLock(semas + QUEUELOCK_SEM_EMPTY, true);
	return true;

}


void
queue_mutex_lock_sl(struct queue_lock * ql)
{
	S_LOCK((slock_t *) ql->lock_data);
}

void
queue_mutex_unlock_sl(struct queue_lock * ql)
{
	S_UNLOCK((slock_t *) ql->lock_data);
}

void
make_lock_nolock(struct queue_lock ** ql)
{
	*ql = palloc(sizeof(struct queue_lock));
	(*ql)->mutex_lock = empty_nolock;
	(*ql)->mutex_unlock = empty_nolock;
	(*ql)->empty_up = NULL;
	(*ql)->empty_down = NULL;
	(*ql)->full_up = NULL;
	(*ql)->full_down = NULL;
	(*ql)->lock_data = NULL;
	(*ql)->free_lock = free_lock_nolock;
}

void
free_lock_nolock(struct queue_lock ** ql)
{
	pfree(*ql);
	*ql = NULL;
}

void
make_lock_sl(struct queue_lock ** ql)
{
	*ql = palloc(sizeof(struct queue_lock));
	(*ql)->mutex_lock = queue_mutex_lock_sl;
	(*ql)->mutex_unlock = queue_mutex_unlock_sl;
	(*ql)->empty_up = NULL;
	(*ql)->empty_down = NULL;
	(*ql)->full_up = NULL;
	(*ql)->full_down = NULL;
	(*ql)->lock_data = palloc(sizeof(slock_t));
	S_INIT_LOCK((*ql)->lock_data);
	(*ql)->free_lock = free_lock_sl;
}


void
free_lock_sl(struct queue_lock ** ql)
{
	pfree((*ql)->lock_data);
	pfree(*ql);
	*ql = NULL;
}



bool
IsSemaphoreLock(struct queue_lock * ql)
{
	return (ql->empty_up && ql->empty_down && ql->full_up && ql->full_down);
}

void
empty_nolock(struct queue_lock * ql)
{
}
