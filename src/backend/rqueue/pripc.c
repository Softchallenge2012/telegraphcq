/*-------------------------------------------------------------------------
 *
 * pripc.c
 *
 * Author: Owen Cooper
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: pripc.c,v 1.6 2003/07/16 09:42:28 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

/* #include "resultq/pripc.h" */

/* bool */
/* make_pripc(struct pripc ** p, int queue_size) */
/* { */
/*	*p = palloc(sizeof(struct pripc)); */
/*	p->query_plan = NULL; */
/*	make_queue(&(*p)->results, queue_size); */
/*	*p->locks = IpcSemaphoreCreate(2, IPCProtect, 0, false); */
/*	plan_unlock(*p); */
/*	return true; */
/* } */

/* bool */
/* free_pripc(struct pripc ** p) */
/* { */
/*	IpcSemaphoreKill((*p)->locks); */
/*	free_queue(&(*p)->results); */
/*	pfree(*p); */
/*	*p = NULL; */
/* } */

/* void */
/* plans_up(struct pripc * p) */
/* { */
/*	IpcSemaphoreUnlock(p->locks, 0); */
/* } */

/* void */
/* plans_down(struct pripc * p) */
/* { */
/*	IpcSemaphoreLock(p->locks, 0); */
/* } */

/* void */
/* plan_lock(struct pripc * p) */
/* { */
/*	IpcSemaphoreLock(p->locks, 1); */
/* } */

/* void */
/* plan_unlock(struct pripc * p) */
/* { */
/*	IpcSemaphoreUnlock(p->locks, 1); */
/* } */
