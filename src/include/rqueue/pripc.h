/*-------------------------------------------------------------------------
 *
 * pripc.h
 *
 * Author: Owen Cooper
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: pripc.h,v 1.5 2003/07/16 09:42:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

/* struct pripc */
/* { */
/*	struct queue *results; */
/*	Plan	   *query plan; */
/*	IpcSemaphoreId locks; */
/* }; */

/* to be called during initialization. */

/* bool		make_pripc(struct pripc **, int queue_size); */
/* bool		free_pripc(struct pripc **); */

/* bool		enqueue_plan(struct pripc **, Plan *); */
/* bool		dequeue_plan(struct pripc **, Plan **); */

/* void    *fetch(pripc **); */
