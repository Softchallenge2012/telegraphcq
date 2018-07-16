/*-------------------------------------------------------------------------
 *
 * rqdest.h
 *
 * Author: Owen Cooper
 *
 * This file contains rqdest stuff
 *
 * TBD: Needs more documentation
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: rqdest.h,v 1.23 2004/10/16 01:51:36 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef RQDEST_H
#define RQDEST_H

#include "rqueue/resultq.h"
#include "tcop/dest.h"
#include "telegraphcq/telegraphcqinit.h"
#include "utils/bit.h"

DestReceiver *resultqdest_create_DR(void);
DestReceiver *resultq_IHT_dest_create_DR(void);

/*
 * tuple serialization functions
 */




void		ReportErrorOnQueue(Queue * q, char *msg);
void		TCQHandleErrorMessageForQuery(int qno, char *msg);
void		TCQHandleErrorMessageForQueries(List *queries, char *msg);

int			result_determineSize(char *src, char *typeinfo);
bool		result_serialize(char *src, char *dest, int destsize, char *typeinfo);
bool		result_deserialize(char *src, char **dest, char *typeinfo);

int			heaptuple_determineSize(char *src, char *typeinfo);
bool		heaptuple_serialize(char *src, char *dest, int destsize, char *typeinfo);
bool		heaptuple_deserialize(char *src, char **dest, char *typeinfo);
int			varlena_determineSize(char *src, char *typeinfo);
bool		varlena_serialize(char *src, char *dest, int destsize, char *typeinfo);
bool		varlena_deserialize(char *src, char **dest, char *typeinfo);

void		CreateResultQueues(void);
void		DeallocateResultQueues(void);
Queue	   *GetQueue(uint16 qno);
uint16		AllocQueue(void);
void		FreeQueue(uint16 qno);

extern queue_serializer tuple_serializer;
extern queue_serializer result_serializer;


typedef struct DR_resultqdest
{
	DestReceiver pub;
	bits8		whereToSendOutput[BASIZE(TCQMAXQUERIES)];
	bool		isIHT;
	struct Eddy *eddy;
}	DR_resultqdest;


typedef enum
{
	RESULT_DONE = 199,
	RESULT_ERROR,
	RESULT_HEAPTUPLE,
	RESULT_FPRODSYNC,
	RESULT_FCONSSYNC,
	RESULT_FPRODALIVE,
	RESULT_PAUSEACK
}	ResultType;


typedef struct result
{
	ResultType	type;
	union
	{
		int			info;
		text	   *error;
		HeapTuple	tuple;
	}			data;
}	result;

#define ResIsNull(res) \
	((res) == NULL ||                       \
	 (((res)->type == RESULT_HEAPTUPLE) &&  \
	  ((res)->data.tuple == NULL)))

#endif   /* RQDEST_H */
