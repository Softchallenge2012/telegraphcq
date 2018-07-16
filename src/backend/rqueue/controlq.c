/*-------------------------------------------------------------------------
 *
 * controlq.c
 *
 * Author: Owen Cooper
 *
 *	Control queues are used to communicate cancellation messages between
 *	the telegraphCQ frontend process and the TelegraphCQ backend process.
 *
 *	this file contains a queue_serializer implemention for control queue
 *	entries.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 * $Id: controlq.c,v 1.6 2003/07/17 05:18:48 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"
#include "rqueue/controlq.h"
#include "rqueue/resultq.h"


#include "assert.h"

struct queue_serializer controlqentry_serializer = {controlqentry_determineSize, controlqentry_serialize, controlqentry_deserialize};


int
controlqentry_determineSize(char *targ, char *typeinfo)
{
	return sizeof(Controlqentry);
}

bool
controlqentry_serialize(char *targ, char *buf, int bufsize, char *typeinfo)
{
	Controlqentry *cqe = (Controlqentry *) targ;
	char	   *cp = buf;

	if (controlqentry_determineSize(targ, typeinfo) > bufsize)
	{
		/* OC: change this */
		assert(false);
		return false;
	}

	memcpy(cp, cqe, sizeof(Controlqentry));
	return true;
}

bool
controlqentry_deserialize(char *buf, char **dest, char *typeinfo)
{
	Controlqentry *cqe = (Controlqentry *) buf;

	*dest = palloc(sizeof(Controlqentry));
	memcpy(*dest, cqe, sizeof(Controlqentry));
	return true;
}
