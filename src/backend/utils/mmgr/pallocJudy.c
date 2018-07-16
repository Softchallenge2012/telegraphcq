/*-------------------------------------------------------------------------
 *
 * pallocJudy.c
 *
 *	  POSTGRES memory allocator that can be plugged into Judy
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/mmgr/pallocJudy.c,v 1.3 2004/02/13 01:44:03 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h" 

#include "utils/pallocJudy.h"

void* JudyPalloc(size_t size)
{
	void *Addr;

/* 	elog(LOG, "Calling JudyPalloc"); */
	Addr = palloc(size + EIGHT_ALIGN);
	Assert(IsFourAligned(Addr));
	if(IsEightAligned(Addr))
	{
		*((char *)(((char *)Addr) + EIGHT_ALIGN - 1)) =  WAS_EIGHT;
		return(((char *)Addr) + EIGHT_ALIGN);
	}
	else
	{
		*((char *)(((char *)Addr) + FOUR_ALIGN - 1)) =  WAS_FOUR;
		return(((char *)Addr) + FOUR_ALIGN);
	}

} // JudyMalloc()


void JudyPFree(void * PWord)
{
/* 	elog(LOG, "Calling JudyPfree"); */
	Assert(IsFourAligned(PWord));
	if(*((char *)(((char *)PWord) - 1)) == WAS_FOUR)
	{
		pfree(((char *)PWord) - FOUR_ALIGN);
	}
	else if(*((char *)(((char *)PWord) - 1)) == WAS_EIGHT)
	{
		pfree(((char *)PWord) - EIGHT_ALIGN);
	}
	else
	{
		elog(ERROR, "JudyFree: bad alignment");
	}
} // JudyPFree()
