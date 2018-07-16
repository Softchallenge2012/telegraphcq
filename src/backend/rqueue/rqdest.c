/*-------------------------------------------------------------------------
 *
 * rqdest.c
 * Author: Owen Cooper
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *	This file implements functions necessary for result delivery in
 *	telegraphCQ.
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/rqueue/rqdest.c,v 1.59 2005/06/13 02:59:27 phred Exp $
 *
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "miscadmin.h"

#include "executor/intermediateTuple.h"
#include "executor/nodeEddy.h"
#include "executor/nodeScanq.h"
#include "executor/nodeScanModule.h"
#include "rqueue/resultq.h"
#include "rqueue/rqdest.h"
#include "rqueue/ingresq.h"
#include "storage/spin.h"
#include "tcop/dest.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "access/heapam.h"
#include "utils/builtins.h"

#include <sched.h>										 /* For sched_yield() */

#define DEBUG_RQDEST

extern int numResultQueues;

queue_serializer tuple_serializer = {
	heaptuple_determineSize,
	heaptuple_serialize,
	heaptuple_deserialize
};

queue_serializer result_serializer = {
	result_determineSize,
	result_serialize,
	result_deserialize
};

static void resultqdest_setup(DestReceiver *self, int operation,
							  const char *portalName, TupleDesc typeinfo);
static void resultqdest(HeapTuple tuple, TupleDesc typeinfo, DestReceiver *self);
static void resultqdest_cleanup(DestReceiver *self);

/*
 * tcqoc
 *
 * functions to iterate over the linage bits, find matches, and
 * translate a match into the right result queue.
 *
 * initQueueSelectionIter()		 -- start interation
 * QueueSelectionIterFindNext()  -- given an iteration state, find the next
 *									matching queue and return true, or return
 *									false.	The state will be modified.
 * QueueSelectionIterIncr()		 -- done with the current element.	this
 *
 * GetQueue()					 -- translates iterator state to result queue.
 *									This function lazily initializes the queue.
 *								   if it doesn't alreay exist.
 *
 */

uint16		initQueueSelectionIter(void);
bool		QueueSelectionIterFindNext(uint16 *qno, BitArray t);

void		QueueSelectionIterIncr(uint16 **qno);

void
QueueSelectionIterIncr(uint16 **qno)
{
	(**qno)++;
	if (**qno == numResultQueues)
		*qno = NULL;
}

uint16
initQueueSelectionIter()
{
	return 0;
}

/* AllocQueue -- return the next available result queue */
uint16
AllocQueue()
{
	uint16		qno;

	SpinLockAcquire(shmResultQueueBitmapLock);
	for (qno = 0; qno < numResultQueues; qno++)
	{
		if (!BitArrayBitIsSet(shmResultQueueBitmap, qno))
		{
			BitArraySetBit(shmResultQueueBitmap, qno);
			break;
 		}

	}

	SpinLockRelease(shmResultQueueBitmapLock);

    /*
	elog(LOG, "Num of result queues. qno = %d", qno);
	elog(LOG, "Max # of result queues. numResultQueues = %d", numResultQueues);
    */

	if (qno >= numResultQueues)
		elog(ERROR, "Out of result queues (%d of %d queues used)", qno,
                numResultQueues);
	return qno;
}

/* FreeQueue -- free a result queue */
void
FreeQueue(uint16 qno)
{
	Assert(qno < numResultQueues);
	SpinLockAcquire(shmResultQueueBitmapLock);
	/* BitArrayClearBit(shmResultQueueBitmap, (1 << qno));*/ /* This looks like a bug... too large a bit index :) - TCQ SR */
	BitArrayClearBit(shmResultQueueBitmap, qno);
	/* *shmResultQueueBitmap &= ~(1 << qno); */
	SpinLockRelease(shmResultQueueBitmapLock);

}

void
CreateResultQueues()
{
	/*
	 * result queues need to be allocated in the context of a long running
	 * TCQ process becuause they contain semaphores which may be reaped by
	 * IPCSemaphoreCreate if they are presumed to live longer than their
	 * allocating process.
	 */
	int			i = 0;
	MemoryContext oldContext =
	MemoryContextSwitchTo(TopSharedMemoryContext);



	elog(DEBUG1, "result q of size is  %d", tcqresultqsize);
	for (i = 0; i < numResultQueues; i++)
	{
		if (!(*shmResultQueuesInit)[i])
		{
			make_queue(&((*shmResultQueues)[i]), NULL, tcqresultqsize);
			(*shmResultQueuesInit)[i] = true;
		}

	}
	MemoryContextSwitchTo(oldContext);

}

void
DeallocateResultQueues()
{
	int			i = 0;

	if (!(init_plumbing &&
		  *init_plumbing &&
		  shmResultQueues &&
		  shmResultQueuesInit))
		return;


	for (i = 0; i < numResultQueues; i++)
	{
		if ((*shmResultQueuesInit)[i])
		{
			free_queue(&((*shmResultQueues)[i]));
			(*shmResultQueues)[i] = NULL;
			(*shmResultQueuesInit)[i] = false;
		}

	}
}


Queue *
GetQueue(uint16 qno)
{

	if ((qno < numResultQueues) && (*shmResultQueuesInit)[qno])
		return (*shmResultQueues)[qno];
	else
	{
		elog(ERROR, "result queue %d is not allocated... this"
			 " should have occured in InitializeTCQPlumbing ", qno);
	}
	return NULL;
}

bool
QueueSelectionIterFindNext(uint16 *qno, BitArray b)
{
	while (qno != NULL)
	{
		if (BitArrayBitIsSet(b, *qno)) 
		{
			return true;
		}
		QueueSelectionIterIncr(&qno);
	}
	return false;
}


/*
 * tuple serialization functions
 */

void
ReportErrorOnQueue(Queue * q, char *msg)
{
	/* note: queue must exprect result entries */
	text	   *t = DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(msg)));
	result		r;


	if (t)
	{
		r.type = RESULT_ERROR;
		r.data.error = t;
		enqueue(q, (char *) &r, false, &result_serializer);
		pfree(t);
	}

}


void
TCQHandleErrorMessageForQueries(List *queries, char *msg)
{
	List	   *iter = NULL;

	foreach(iter, queries)
		{
			TCQHandleErrorMessageForQuery(lfirsti(iter), msg);
		}
}

void
TCQHandleErrorMessageForQuery(int qno, char *msg)
{
	switch (MyProcessType)
	{
		case PROC_TCQ_FRONTEND:
				elog(ERROR, msg);
			break;
		case PROC_TCQ_BACKEND:
			{
				Queue	   *q = (*shmResultQueues)[qno];

				if (q)
					ReportErrorOnQueue(q, msg);
			}
			break;
		case PROC_TCQ_WRAP_CH:
			{
				Queue	   *q = getIngresQ(qno);

				if (q)
					ReportErrorOnQueue(q, msg);
			}
			break;
		default:
			elog(FATAL, "unsupported process type in TCQHandleErrorMessage");
	}
}


int
result_determineSize(char *targ, char *typeinfo)
{

	int			ret = sizeof(ResultType);
	result	   *elt = (result *) targ;

    /*Assert(elt->data.tuple != NULL);*/

	switch (elt->type)
	{
		case RESULT_HEAPTUPLE:
			ret += heaptuple_determineSize((char *) elt->data.tuple, typeinfo);
			break;
		case RESULT_ERROR:
			ret += varlena_determineSize((char *) elt->data.error, typeinfo);
			break;
		case RESULT_FPRODSYNC:
 			ret += sizeof(int);
			break;
		case RESULT_FPRODALIVE:
			ret += sizeof(int);
			break;
		case RESULT_PAUSEACK:
		case RESULT_FCONSSYNC:
		case RESULT_DONE:
			break;
		default:
			elog(ERROR, "invalid type for result serializer ");
	}
	return ret;
}



bool
result_serialize(char *targ, char *buf, int bufsize, char *typeinfo)
{

	bool		ret = false;
	result	   *elt = (result *) targ;

    /*Assert(elt->data.tuple != NULL);*/

	memcpy(buf, &(elt->type), sizeof(ResultType));

	switch (elt->type)
	{
		case RESULT_HEAPTUPLE:
			ret = heaptuple_serialize((char *) elt->data.tuple,
									  buf + sizeof(ResultType),
									  bufsize - sizeof(ResultType), typeinfo);
			break;

		case RESULT_ERROR:
			ret = varlena_serialize((char *) elt->data.error,
									buf + sizeof(ResultType),
									bufsize - sizeof(ResultType), typeinfo);
			break;
			
		case RESULT_FPRODSYNC:
#ifdef DEBUG_SERIALIZE
 			elog(DEBUG1, "Serializing an fprodsync");
#endif
 			memcpy(buf+sizeof(ResultType),
 				   (char *) &(elt->data.info),
 				   sizeof(int)); /* I assume that the size is checked
 								  * before the call to this function.
 								  */
 			ret = true;
			break;

		case RESULT_FPRODALIVE:
#ifdef DEBUG_SERIALIZE
 			elog(DEBUG1, "Serializing an fprodalive");
#endif
 			memcpy(buf+sizeof(ResultType),
 				   (char *) &(elt->data.info),
 				   sizeof(int)); /* I assume that the size is checked
 								  * before the call to this function.
 								  */
 			ret = true;
			break;

		case RESULT_PAUSEACK:
#ifdef DEBUG_SERIALIZE
			elog(DEBUG1, "Serializing a pauseack");
#endif
			ret = true;
			break;
			
		case RESULT_FCONSSYNC:
#ifdef DEBUG_SERIALIZE
 			elog(DEBUG1, "Serializing an fconssync");
#endif
			ret = true;
			break;
			
		case RESULT_DONE:
			ret = true;
			break;
			
		default:
			elog(ERROR, "invalid type for result serializer ");
			break;
	}


	return ret;
}

bool
result_deserialize(char *buf, char **dest, char *typeinfo)
{
	bool		ret = false;
	result	   *elt = NULL;

	if (dest == NULL)
		return false;

	if (*dest == NULL)
	{
		*dest = palloc(sizeof(result));
	}
	
	elt = (result *) * dest;
	memcpy(&elt->type, buf, sizeof(ResultType));
	switch (elt->type)
	{
		case RESULT_HEAPTUPLE:
			ret = heaptuple_deserialize(buf + sizeof(ResultType),
										(char **) &(elt->data.tuple),
										typeinfo);
			break;

		case RESULT_ERROR:
			ret = varlena_deserialize(buf + sizeof(ResultType),
									  (char **) &(elt->data.error),
									  typeinfo);

			break;
			
 		case RESULT_FPRODSYNC:
#ifdef DEBUG_DESERIALIZE
 			elog(DEBUG1, "Deserializing an fprodsync.");
#endif
 			memcpy((char *) &(elt->data.info),
 				   buf + sizeof(ResultType),
 				   sizeof(int));
 			ret = true;
 			break;

		case RESULT_FPRODALIVE:
#ifdef DEBUG_DESERIALIZE
 			elog(DEBUG1, "Deserializing an fprodalive.");
#endif
 			memcpy((char *) &(elt->data.info),
 				   buf + sizeof(ResultType),
 				   sizeof(int));
 			ret = true;
 			break;

		case RESULT_PAUSEACK:
#ifdef DEBUG_DESERIALIZE
 			elog(DEBUG1, "Deserializing a pause-ack");
#endif
			ret = true;
			break;

 		case RESULT_FCONSSYNC:
#ifdef DEBUG_DESERIALIZE
 			elog(DEBUG1, "Deserializing an fconssync.");
#endif
			ret = true;
			break;
			
		case RESULT_DONE:
			ret = true;
			break;

		default:
			elog(ERROR, "can't determine result type ");
	}

	return ret;
}

int
varlena_determineSize(char *targ, char *typeinfo)
{
	struct varlena *v = (struct varlena *) targ;

	return VARSIZE(v);
}

bool
varlena_serialize(char *targ, char *buf, int bufsize, char *typeinfo)
{
	int			size = varlena_determineSize(targ, typeinfo);

	if (size > bufsize)
		return false;
	else
		memcpy(buf, targ, size);
	return true;
}

bool
varlena_deserialize(char *buf, char **dest, char *typeinfo)
{
	int			size = varlena_determineSize(buf, typeinfo);

	if (dest == NULL)
		return false;
	*dest = palloc(size);
	memcpy(*dest, buf, size);
	return true;
}

/*
 * determineSize
 *
 *	  given a heap tuple, determine the amount of space required to serialize it
 *	  with serializeTuple
 */
int
heaptuple_determineSize(char *targ, char *typeinfo)
{
	HeapTuple	t = (HeapTuple) targ;
	int			size = t->t_len;	/* tuple data length */

	size += sizeof(t->t_len);
	return size;
}

/*
 * serializeTuple
 *
 *	  given a heap tuple and its descriptor, create a serialized representation
 */
bool
heaptuple_serialize(char *targ, char *buf, int bufsize, char *typeinfo)
{
	HeapTuple	tuple = (HeapTuple) targ;
	char	   *cp = buf;

	if (heaptuple_determineSize(targ, typeinfo) > bufsize)
	{
		/* OC: change this */
		Assert(false);
		return false;
	}

	memcpy(cp, &tuple->t_len, sizeof(tuple->t_len));
	cp += sizeof(tuple->t_len);
	if (tuple->t_len > 0)
		memcpy(cp, tuple->t_data, tuple->t_len);

	return true;
}

/*
 * deserializeTuple
 *
 *	  given a descriptor and a buffer, deserialize
 *
 * Allocates space for the tuple here if the passed in pointer is NULL
 */
bool
heaptuple_deserialize(char *buf, char **dest, char *typeinfo)
{
	char	   *cp = buf;
	HeapTuple	ret;

	uint32		len = 0;

	memcpy(&len, cp, sizeof(uint32));
	cp += sizeof(uint32);
	*dest = palloc(len + HEAPTUPLESIZE);
	ret = (HeapTuple) *dest;
	ret->t_datamcxt = CurrentMemoryContext;
	ret->t_len = len;
	if (ret->t_len > 0)
	{
		ret->t_data = (HeapTupleHeader) (((char *) ret) + HEAPTUPLESIZE);
		memcpy(ret->t_data, cp, ret->t_len);
	}
	else
		ret->t_data = NULL;
	return true;
}


/* ----------------
 *		Initialize: create a DestReceiver for printtup
 * ----------------
 */
DestReceiver *
resultqdest_create_DR()
{
	DR_resultqdest *self = (DR_resultqdest *) palloc(sizeof(DR_resultqdest));

	(self->pub).receiveTuple = resultqdest;
	(self->pub).setup = resultqdest_setup;
	(self->pub).cleanup = resultqdest_cleanup;
	self->isIHT = false;		/* Default without eddies */
	self->eddy = NULL;
	/* if isIHT is set, then eddy must also be set. */
	return (DestReceiver *) self;
}

static void
resultqdest_setup(DestReceiver *self, int operation,
				  const char *portalName, TupleDesc typeinfo)
{
	/* ----------------
	 * We could set up the derived attr info at this time, but we postpone it
	 * until the first call of printtup, for 3 reasons:
	 * 1. We don't waste time (compared to the old way) if there are no
	 *	  tuples at all to output.
	 * 2. Checking in printtup allows us to handle the case that the tuples
	 *	  change type midway through (although this probably can't happen in
	 *	  the current executor).
	 * 3. Right now, ExecutorRun passes a NULL for typeinfo anyway :-(
	 * ----------------
	 */
}

/*
 * This function uses the DestReceiver to decide whether to use the
 * tuple_serializer
 */
static void
resultqdest(HeapTuple inputTuple, TupleDesc typeinfo, DestReceiver *self)
{
	uint16		qno = 0;
	uint16	   *qnop = &qno;
	Queue	   *q = NULL;


	queue_serializer *ts;
	HeapTuple	originalTuple = inputTuple;
	HeapTuple	outputTuple = NULL;
	struct DR_resultqdest *rqdest = (struct DR_resultqdest *) self;
	Eddy	   *eddy = (Eddy *) rqdest->eddy;
	result		res;
	Oid recurseOid;
	int smid;
	ts = &result_serializer;


	/*
	 * if we are not running in CQ eddy mode, then the output goes to
	 * exactly one queue.
	 */
	if (UseCQEddy == false)
	{
		BitArrayClearAll(rqdest->whereToSendOutput, numResultQueues);
		BitArraySetBit(rqdest->whereToSendOutput, 0);
	}
	for (qno = initQueueSelectionIter();
		 QueueSelectionIterFindNext(qnop, rqdest->whereToSendOutput);
		 QueueSelectionIterIncr(&qnop))
	{

		bool		ownTuple = false;
		bool		isHeapTuple = false;

		q = GetQueue(qno);
		if (originalTuple == NULL)
		{
			outputTuple = palloc(HEAPTUPLESIZE);
			outputTuple->t_len = 0;
			outputTuple->t_data = NULL;
			ownTuple = true;
			isHeapTuple = false;
		}
		else if (eddy)
		{
		  IntermediateHeapTuple iht = (IntermediateHeapTuple)originalTuple;
		  if(iht->isAggTuple)
		  {
		      outputTuple = iht->t_base_tuples[0];
			  /*
			   *  This tuple should _only_ be freed by its original creator. The
			   *  creator can either be the root of an Agg tree hanging off an
			   *  FAgg, or the FAgg itself in the case of a punctuation
			   *  tuple. If it's the former, the Agg node's ExecStoreTuple()
			   *  call takes care of it. If it's the latter, the FAgg takes care
			   *  of it explicitly. 
			   *                                          - Sailesh (6/2/2005)
			   */
			  ownTuple = false; 
		      isHeapTuple = true;
		  }
		  else
		  {
		      outputTuple = EddyExecProject(eddy, iht, qno);
		      ownTuple = true;
		      isHeapTuple = true;
		  }
		 
		  // If the query is recursive then find the destination relation
		  if  ((recurseOid = GetRecurseRelation(eddy, qno)) != InvalidOid) 
		  {
			  // Search sources for the ScanModule responsible for this relation
			  for (smid = 0; smid < eddy->numSources; smid++) 
			  {
				  if(eddy->sources[smid] == NULL) continue;
				  if (IsA(eddy->sources[smid], ScanModule)) 
				  {
					  if (getRealSourceOid (eddy->sources[smid]) == recurseOid) 
					  {
						  ScanModule * sm = (ScanModule *) eddy->sources[smid];
						  InsertRecursiveResults(sm, outputTuple);
						  /* SJ SIGMOD05 hack - set the recursive flag on the
						   * scanmodule*/
						  sm->isRecursivelyFed = TRUE;
								  
					  }
				  }
			  }
#ifdef DEBUG_RQDEST
			  elog(DEBUG1, "Sending a tuple from the result of query %d to the"
				   " ScanModule with OID %d", qno, recurseOid);
#endif
		  }
		}
		else
		{
			outputTuple = originalTuple;
			ownTuple = false;
		}
			
		res.type = RESULT_HEAPTUPLE;
		res.data.tuple = outputTuple;
		// Send query results to output if appropriate
		if (IsOutputQuery (eddy, qno)) 
        {
          /* 
           * Note from Fred (phred@cs.berkeley.edu):
           *
           * If the queue is full, it means one of two things: 
           *  a) The client is not pulling data fast enough, in which 
           *      case we want to drop the tuple.  In the future, we 
           *      may want to drop the tuple at the front of the queue 
           *      instead (e.g. issue a dequeue(), then enqueue()).  
           *  b) The server is starving the frontend, in which case 
           *      we want to back off and let the frontend catch up on 
           *      its work.
           *
           * In either case, we're not allowed to block here. 
           *
           * My current solution to this problem is to back off by calling
           * sched_yield() (to handle case (b)), then retry the enqueue.  If
           * the enqueue still fails, we assume case (a) and drop the tuple.
           *
           * A better solution would be for the frontend to notify the
           * backend when it is starved for resources.
           *
           * An even better solution would be to elimated the frontend
           * processes and have clients connect directly to the backend.
           */
          if (enqueue(q, (char *) &res, false, ts) == false)
		  {
            /* Assume that the frontend is starved for CPU.  
             * See note above. */
            elog(LOG, "Queue %d is full... backing off", qno);
            sched_yield();

            if (enqueue(q, (char *) &res, false, ts) == false) 
			{
				elog(LOG, "Fe for queue %d blocked... discarding tuple", qno);
            }
          }
		  else 
		  {
				elog(DEBUG1, "Queue %d not full... enqueued successfully",qno);
          }
		}
	  if (ownTuple) /*  If you have ownership then free the tuple */
	  {
	      if (isHeapTuple)
			  heap_freetuple(outputTuple);
	      else
			  pfree(outputTuple);
	  }
	}
}


/* ----------------
 *		printtup_cleanup
 * ----------------
 */
static void
resultqdest_cleanup(DestReceiver *self)
{
	/* just free the structure, and go from there */
	struct DR_resultqdest *myState = (struct DR_resultqdest *) self;

	pfree(myState);
}
