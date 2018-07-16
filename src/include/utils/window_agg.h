/*-------------------------------------------------------------------------
 *
 * window_agg.h
 *	  Default windowed aggregation functions.
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: window_agg.h,v 1.4 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef WINDOW_AGG_H
#define WINDOW_AGG_H

#include "fmgr.h"

typedef struct AggWindowSpec
{

	/* Fields used for windowing specifications, in sequence number.   */
	int32		window_interval;	/* Interval preceding the latest tuple. */
	int32		hop_size;		/* Return tuple every hop_size.			*/

}	AggWindowSpec;

typedef struct WinAggStateFuncs
{
	char *fn_name;
	PGFunction getstatefn_addr;
	PGFunction getsizefn_addr;
} WinAggStateFuncs;

extern Datum int4count_w(PG_FUNCTION_ARGS);
extern Datum int4count_w_getstate(PG_FUNCTION_ARGS);
extern Datum int4count_w_getsize(PG_FUNCTION_ARGS);

extern Datum first_url_agg(PG_FUNCTION_ARGS);
extern Datum first_url_getstate(PG_FUNCTION_ARGS);
extern Datum first_url_getsize(PG_FUNCTION_ARGS);

extern WinAggStateFuncs winagg_statefunc_table[];

#endif   /* WINDOW_AGG_H */
