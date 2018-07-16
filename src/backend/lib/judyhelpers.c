/*-------------------------------------------------------------------------
 *
 * judyhelpers.c
 *
 *	  Helper functions for Judy arrays
 *
 * Notes:
 *
 *   (1) See http://judy.sourceforge.net for more information on Judy
 *   (2) Ideally we should try and roll these into Judy
 *
 *   (3) Try and keep PostgreSQL/TelegraphCQ-isms out of this compilation
 *   unit. If forced, please keep the "infected" code in a clearly marked
 *   portion of the file. This helps us contribute code easily to Judy.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/lib/judyhelpers.c,v 1.2 2004/02/13 01:44:02 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "lib/judyhelpers.h"

Pvoid_t J1Copy(Pvoid_t src)
{
	Pvoid_t 	tgt = NULL;
	int 		Rc_int;
	Word_t 		Index = 0;

	/*
	 *  TBD: Need code here to confirm that this is a Judy1 array
	 */

	J1F (Rc_int, src, Index);
	while (Rc_int != 0) 
	{
		J1S (Rc_int, tgt, Index);
		J1N (Rc_int, src, Index);
	}
	return tgt;
}
