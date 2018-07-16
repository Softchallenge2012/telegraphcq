/*-------------------------------------------------------------------------
 *
 * window_agg.c
 *	  Functions for computing windowed aggregates.
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/adt/window_agg.c,v 1.6 2005/07/20 16:42:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <c.h>
#include <ctype.h>
#include <limits.h>

#include "utils/builtins.h"
#include "utils/datum.h"
#include "utils/window_agg.h"
#include "nodes/execnodes.h"
#include "executor/nodeHashAgg.h"
#include "access/heapam.h"

WinAggStateFuncs winagg_statefunc_table[] =
{
	{"int4count_w",	int4count_w_getstate, int4count_w_getsize},
	{"first_url_agg", first_url_getstate, first_url_getsize}
};

typedef struct int4count_w_state
{

	int32		total_count;

}	int4count_w_state;

typedef struct int4count_w_state_new
{
	int32		total_count;
	int32       snum;
	VarChar     tmp;
}	int4count_w_state_new;

/* static */
Datum
int4count_w(PG_FUNCTION_ARGS)
{
	AggStatePerGroup pergroupstate = (AggStatePerGroup) PG_GETARG_POINTER(0);
	AggStatePerAgg peraggstate = (AggStatePerAgg) PG_GETARG_POINTER(1);
	int4count_w_state *state;

	/* Just doing initialization here */
	if (PG_ARGISNULL(0))
	{
		pergroupstate->agg_state = palloc(sizeof(int4count_w_state));
		state = (int4count_w_state *) pergroupstate->agg_state;
		state->total_count = 0;
		pergroupstate->as_size = sizeof(int4count_w_state);
		PG_RETURN_NULL();
	}

	state = (int4count_w_state *) pergroupstate->agg_state;
	state->total_count++;

	if ((state->total_count % (peraggstate->w_spec->hop_size)) == 0)
	{

		
#ifdef EXTRA_WORK
		/* Do some work so source is not the bottleneck */
		void * memory1;
		void * memory2;

		memory1 = malloc(8192*10);
		memory2 = malloc(8192*10);
		memcpy(memory1, memory2, 8192*10);
		free(memory1);
		free(memory2);
#endif
		
		
		PG_RETURN_INT32(state->total_count);
	}
	
	/* Otherwise just return a NULL */
	PG_RETURN_NULL();
}

static char **lookup_strs;
static int lookup_len;


Datum
int4count_w_old(PG_FUNCTION_ARGS)
{
	AggStatePerGroup pergroupstate = (AggStatePerGroup) PG_GETARG_POINTER(0);
	AggStatePerAgg peraggstate = (AggStatePerAgg) PG_GETARG_POINTER(1);
	int4count_w_state_new *state;
	int hop_size = peraggstate->w_spec->hop_size;
	TupleTableSlot *tuple;
	int tot_size;
	bool isNull;


	/* Just doing initialization here */
	if (PG_ARGISNULL(0))
	{
		pergroupstate->agg_state = NULL;
		pergroupstate->as_size = 0;
		PG_RETURN_NULL();
	}

	state = (int4count_w_state_new *) pergroupstate->agg_state;

	/* Scan tuple is in there */
	tuple = (TupleTableSlot *) PG_GETARG_POINTER(3);

	if (state)
	{
		state->total_count++;
		if ((state->total_count % hop_size) == 0)
		{
			pfree(pergroupstate->agg_state);
			state = NULL;
            pergroupstate->agg_state = NULL;
			pergroupstate->as_size = 0;
		}
	}

	if (state==NULL)
	{
		int i;
		
		tot_size = tuple->val->t_len;
/* 		tot_size -= 0; */
		tot_size += sizeof(int4count_w_state_new);

		state = palloc(tot_size);
		
		memcpy(&(state->tmp), tuple->val->t_data, tuple->val->t_len);

		state->total_count = 0;

		state->snum = heap_getattr(tuple->val, 1,
								   tuple->ttc_tupleDescriptor, &isNull);
		

#define MORE_WORK

#ifdef MORE_WORK
#define LIST_SIZE 1024
#define DSTRING "This dummy string ivirus."

		/* Do a check sum? */

		if (lookup_len == 0)
		{
			int dlen = strlen(DSTRING);
			
			lookup_strs = palloc(sizeof(char *)*LIST_SIZE);
			for (i = 0; i < LIST_SIZE; i++)
			{
				lookup_strs[i] = palloc(sizeof(char) * dlen);
				memcpy(lookup_strs[i], DSTRING, dlen);
			}
			lookup_len = LIST_SIZE;
		}
		
		
/* 		for (i = 0; i < tuple->val->t_len; i++) */
/* 		{ */
/* 			blah += (char) *((char *) (&(state->tmp)) + i); */
/* 		} */

		/* Do some string comparisons */

		for (i = 0; i < lookup_len; i++)
		{
			int j;
			for (j = 0; j < strlen(DSTRING); j++)
			{
				strcasecmp(lookup_strs[i % lookup_len] + j, (char *) (&(state->tmp)));
			}
		}
#endif

		pergroupstate->agg_state = (char *) state;
		pergroupstate->as_size = tot_size;
	}

	if ((state->total_count % (hop_size)) == (hop_size - 1))
	{
		int now_snum; int p_snum; 

		p_snum = state->snum;
		
		now_snum = heap_getattr(tuple->val, 1,
								tuple->ttc_tupleDescriptor, &isNull);

		
#ifdef MORE_WORK_2
				 
		pfree(pergroupstate->agg_state);
		state = (int4count_w_state_new *) pergroupstate->agg_state = NULL;
		pergroupstate->as_size = 0;

		tot_size = tuple->val->t_len;
		tot_size += sizeof(int4count_w_state_new);

		state = palloc(tot_size);
		
		memcpy(&(state->tmp), tuple->val->t_data, tuple->val->t_len);

		pergroupstate->agg_state = (char *) state;
		pergroupstate->as_size = tot_size;

		state->total_count = hop_size - 1;
		
/* 		/\* Do some work so source is not the bottleneck *\/ */
/* 		void * memory1; */
/* 		void * memory2; */

/* 		memory1 = malloc(8192*10); */
/* 		memory2 = malloc(8192*10); */
/* 		memcpy(memory1, memory2, 8192*10); */
/* 		free(memory1); */
/* 		free(memory2); */
#endif
		
		state->snum = now_snum - p_snum;

		PG_RETURN_INT32(state->snum);
	}
	
	/* Otherwise just return a NULL */
	PG_RETURN_NULL();
}

static Datum
generic_get_state(PG_FUNCTION_ARGS)
{
	AggStatePerGroup pergroupstate = (AggStatePerGroup) PG_GETARG_POINTER(0);

	PG_RETURN_POINTER(pergroupstate->agg_state);
}

/* Returns a pointer that encapsulates our state */
Datum
int4count_w_getstate(PG_FUNCTION_ARGS)
{
	return generic_get_state(fcinfo);
}


/* Returns the size of our state */
Datum
int4count_w_getsize(PG_FUNCTION_ARGS)
{
	PG_RETURN_INT32(sizeof(int4count_w_state));
}

typedef struct first_url_state
{
	int    count;
	/* This contains a copy of the first URL */
	VarChar url;
} first_url_state;

/* XXX Hack. These are defines right now. We'll need to fix these soon. */
/* These need to be initialized somewhere else. */
#define SEQATTNO   1
#define URLATTNO   4

Datum
first_url_agg(PG_FUNCTION_ARGS)
{
	AggStatePerGroup pergroupstate = (AggStatePerGroup) PG_GETARG_POINTER(0);
	AggStatePerAgg peraggstate = (AggStatePerAgg) PG_GETARG_POINTER(1);
	TupleTableSlot *tuple;
	first_url_state *state;
	bool isNull;
	Datum url_value;
	Size  url_size, tot_size;
	int hop_size = peraggstate->w_spec->hop_size;
	
	/* Initialization */
	if (PG_ARGISNULL(0))
	{
		pergroupstate->agg_state = NULL;
		pergroupstate->as_size = 0;
		PG_RETURN_NULL();
	}

	state = (first_url_state *) pergroupstate->agg_state;

	/* Scan tuple is in there */
	tuple = (TupleTableSlot *) PG_GETARG_POINTER(3);

	/* This one is a new tuple */
	if (state)
	{
		state->count++;
		if ((state->count % hop_size) == 0)
		{
			pfree(pergroupstate->agg_state);
			state = NULL;
            pergroupstate->agg_state = NULL;
			pergroupstate->as_size = 0;
		}
	}
	
	if (state==NULL)
	{
		TupleDesc tdesc = tuple->ttc_tupleDescriptor;
		bool typByVal = tdesc->attrs[URLATTNO - 1]->attbyval;
		int2 tupLen = tdesc->attrs[URLATTNO - 1]->attlen;
		
		url_value = heap_getattr(tuple->val, URLATTNO,
								 tuple->ttc_tupleDescriptor, &isNull);

		if (!isNull)
		{
			url_size = datumGetSize(url_value, typByVal, tupLen);
		}
		else
		{
			url_size = 0;
		}
		
		/* I know, I am creating something larger, but I don't
		   want to deal with alignment issues right now. */
		tot_size = url_size + sizeof(first_url_state);
		
		state = palloc(tot_size);
		
		memcpy(&(state->url), DatumGetPointer(url_value), url_size);

		state->count = 0;

		if (isNull) 
		{
			state->url.vl_len = 0;
		}

		pergroupstate->agg_state = (char *) state;
		pergroupstate->as_size = tot_size;
	}

	/* This triggers an output, how do we trigger a deletion? */
	if ((state->count % hop_size) == (hop_size - 1))
	{
		if ((state->url.vl_len) == 0)
		{ 
			PG_RETURN_NULL();
		}
		
		PG_RETURN_VARCHAR_P(&(state->url));
	}
	
	PG_RETURN_NULL();
}

Datum first_url_getstate(PG_FUNCTION_ARGS)
{
	return generic_get_state(fcinfo);
}

Datum
first_url_getsize(PG_FUNCTION_ARGS)
{
	AggStatePerGroup pergroupstate = (AggStatePerGroup) PG_GETARG_POINTER(0);

	PG_RETURN_INT32(pergroupstate->as_size);
}
