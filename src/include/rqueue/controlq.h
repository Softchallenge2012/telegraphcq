/*-------------------------------------------------------------------------
 *
 * planq.h
 *
 * Author: Owen Cooper
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: controlq.h,v 1.10 2005/01/27 01:19:46 anile Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef __controlq_h__
#define __controlq_h__

#include "postgres.h"
#include "access/heapam.h"

typedef enum
{
	QUERY_REMOVE = 1,
	INGRESQ_REMOVE = 2,
	STREAM_STARTUP = 3,
	STREAM_SHUTDOWN = 4,
	STREAM_PROBE = 5,
	SHUTDOWN = 6,
	WCH_ERROR=7,
	EXPLAIN_EDDY=8,
	CONTROLCOMMAND_UNKNOWN = 500
} ControlCommand;


typedef struct probeinfo
{
	Oid			wrapperoid;
	int		   *mapping;
	HeapTuple	probetuple;		/* schema is that of the stream */
}	probeinfo;

typedef struct wrapperstart 
{
  Oid streamoid;
} wrapperstart;

typedef struct Controlqentry
{
	union
	{
		uint32		queryid;
		uint32		ingresqid;
	  wrapperstart    w;
	}			data;
	struct probeinfo probeinfo; /* SHM: contains shmem pointers */
	ControlCommand command;
  char *error;
}	Controlqentry;



int			controlqentry_determineSize(char *data, char *extra);
bool		controlqentry_serialize(char *src, char *dest, int size, char *extra);
bool		controlqentry_deserialize(char *src, char **dest, char *extra);

extern struct queue_serializer controlqentry_serializer;


#endif
