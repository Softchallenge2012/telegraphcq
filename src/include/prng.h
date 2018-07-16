/*-------------------------------------------------------------------------
 *

 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: prng.h,v 1.4 2003/07/16 09:42:50 sailesh Exp $
 *
 * NOTES
 *
 *	  A prng I modified from the web. Its code from the web, I take no
 *	  credit. Just modified it so that I can have private prngs
 *	  per-module, so they repeatedly generate the same numbers for
 *	  different operators.
 *
 *-------------------------------------------------------------------------
 */


/* Meserrene Twister PRNG state and functions. */

#ifndef PRNG_H
#define PRNG_H

#include "c.h"

#define MT_N			  (624) /* length of state vector */

typedef struct prng_mt
{
	uint32		state[MT_N + 1];/* state vector + 1 extra to not violate
								 * ANSI C */
	uint32	   *next;			/* next random value is computed from here		*/
	/* Please initialize left = -1 */
	int			left;			/* can *next++ this many times before
								 * reloading */
}	prng_mt;

extern void seedMT(prng_mt * prng, uint32 seed);
extern uint32 reloadMT(prng_mt * prng);
extern uint32 randomMT(prng_mt * prng);

#endif   /* PRNG_H */
