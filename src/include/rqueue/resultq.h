/*-------------------------------------------------------------------------
 *
 * resultq.h
 *
 * Author: Owen Cooper
 *
 *	Please see the file src/backend/rqueue/resultq.c for a general
 *	description.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: resultq.h,v 1.25 2004/07/01 23:50:14 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef RESULTQ_H
#define RESULTQ_H


typedef struct Queue
{
	int			size;			/* Size of the queue, must be first for
								 * sending queue contents across the
								 * network */
	int			qid;
	unsigned char *start;		/* start of the queues buffer */
	unsigned char *end;			/* end of the queues buffer */
	unsigned char *head;		/* head of the queue */
	unsigned char *tail;		/* tail of the queue */
	unsigned char *wall;		/* used to denote unused space at end */
	struct queue_lock *ql;		/* control queue locking behavior */
	int			bytesfree;
	int			numelts;
	double		avgfree;
	int			numcalls;
}	Queue;

/* A structure for receiving and sending the queue */
typedef struct queue_comm
{
	Queue	   *queue;
	int			pos;
	int			size;
	bool		done;

}	QueueComm;

/*
 * this structure allows for different types of queue locking behavior.
 * currently two types of locks are available queue_lock_sl, which is a
 * queue that is protected by a mutex, but returns an error code on
 * a full/empty condition.
 *
 *	queue_lock_sem provides a queue interface where clients block on dequeue
 *	or enqueue if the queue is full or empty.  This is accomplished by
 *	keeping a count of the number of full and empty slots in the queue.
 *	since we don't have an accruate notion of the number of empty slots, we
 *	initially overestimate this value, and decrease it to 0 when the dequeue
 *	returns no elements.
 */
struct queue_lock
{
	void		(*mutex_lock) (struct queue_lock *);	/* mutex for queue */
	void		(*mutex_unlock) (struct queue_lock *);
	bool		(*empty_up) (struct queue_lock *, bool isBlocking);		/* functions for
																		 * semaphore locks */
	bool		(*empty_down) (struct queue_lock *, bool isBlocking);
	bool		(*full_up) (struct queue_lock *, bool isBlocking);
	bool		(*full_down) (struct queue_lock *, bool isBlocking);
	void		(*free_lock) (struct queue_lock **);	/* destructor */
	char	   *lock_data;
};

/* Global no_lock structure for queues that do not need a lock */
extern struct queue_lock *no_lock;

/* remove all elements from the queue */
void		clear_queue(Queue * q);

/* check to see if an element of size can fit in the queue right now */
bool		check_capacity(Queue * q, int size);

typedef struct queue_serializer *q_serializer;

/*
 * a queue serializer is required to enqueue and dequeue elements from the
 * queue.  The functions say how to copy an arbitrary pointer into the queue,
 * and how to retrive the structure from the queue representation:
 *
 *	determine size: given a pointer to obj and possibly some extra info,
 *	  determine the amount of space the serialized representation will take.
 *	serialize: copy source element into the queue. return the size written
 * deserialize: recover the original structure from the flattened
 *	representation. return the queue size consumed.
 */
typedef struct queue_serializer
{
	int			(*determineSize) (char *obj, char *extrainfo);
	bool		(*serialize) (char *src, char *dest, int destlen, char *extrainfo);
	bool		(*deserialize) (char *src, char **dest, char *extrainfo);
}	queue_serializer;

/*
 * queue construction/destruction functions.
 */
void		make_queue(Queue ** q, struct queue_lock *, int size);
void		free_queue(Queue ** q);

/*
 * external interface to the queue
 */
bool		enqueue(Queue * q, char *data, bool isBlocking, struct queue_serializer * qs);
bool		dequeue(Queue * q, char **ret, bool isBlocking, struct queue_serializer * qs);



void		test_queue(int datasize, int val);


/* locking prototypes */
#define QUEUELOCK_SEM_EMPTY 0
#define QUEUELOCK_SEM_FULL	1
#define QUEUELOCK_SEM_MUTEX 2

/* Macros for testing state of the queue */
#define VQSPACETOEND(ptr, q)   (q->end - ptr)
#define QEMPTY(q) (q->bytesfree == VQSPACETOEND(q->start,q))
#define QFULL(q) ((q->bytesfree - (q->end-q->wall))==0)
#define QFREESPACE(q) ((q->bytesfree - (q->end-q->wall)))

void		queue_mutex_lock_sl(struct queue_lock *);
void		queue_mutex_unlock_sl(struct queue_lock *);
void		make_lock_sl(struct queue_lock **);
void		free_lock_sl(struct queue_lock **);
void		make_lock_nolock(struct queue_lock **);
void		free_lock_nolock(struct queue_lock **);
void		empty_nolock(struct queue_lock *);

bool		IsSemaphoreLock(struct queue_lock * ql);
void		make_lock_sem(struct queue_lock **, int empties);
void		free_lock_sem(struct queue_lock **);
void		queue_mutex_lock_sem(struct queue_lock *);
void		queue_mutex_unlock_sem(struct queue_lock *);
bool		queue_empty_up_sem(struct queue_lock *, bool isBlocking);
bool		queue_empty_down_sem(struct queue_lock *, bool isBlocking);
bool		queue_full_up_sem(struct queue_lock *, bool isBlocking);
bool		queue_full_down_sem(struct queue_lock *, bool isBlocking);

void		print_sem_lock_values(struct queue_lock * ql);

/* Networking functions */
int			send_queue(QueueComm * qc, int sockfd, bool isBlocking);
int			send_queue_b(QueueComm * qc, int sockfd);
int			send_queue_nb(QueueComm * qc, int sockfd);

void		swizzle_queue_ptrs(Queue * q);

int			receive_queue(QueueComm * qc, int sockfd, bool isBlocking);
int			receive_queue_b(QueueComm * qc, int sockfd);
int			receive_queue_nb(QueueComm * qc, int sockfd);

QueueComm   *newQueueComm(Queue *q);
#endif
