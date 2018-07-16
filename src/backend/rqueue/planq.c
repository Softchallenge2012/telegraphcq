/*-------------------------------------------------------------------------
 *
 * planq.c
 *
 * Author: Owen Cooper
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: planq.c,v 1.8 2003/04/30 02:01:00 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"
#include "rqueue/planq.h"
#include "rqueue/resultq.h"

struct queue_serializer planqentry_serializer = {
	planqentry_determineSize,
	planqentry_serialize,
	planqentry_deserialize
};



int
planqentry_determineSize(char *targ, char *typeinfo)
{
	return sizeof(struct planqentry);
}
bool
planqentry_serialize(char *targ, char *buf, int bufsize, char *typeinfo)
{
	planqentry *pqe = (planqentry *) targ;
	char	   *cp = buf;

	if (planqentry_determineSize(targ, typeinfo) > bufsize)
	{
		/* OC: change this */
		Assert(false);
		return false;
	}

	memcpy(cp, pqe, sizeof(struct planqentry));
	return true;
}

bool
planqentry_deserialize(char *buf, char **dest, char *typeinfo)
{

	planqentry *pqe = (planqentry *) buf;

	*dest = palloc(sizeof(struct planqentry));
	memcpy(*dest, pqe, sizeof(struct planqentry));
	return true;
}
