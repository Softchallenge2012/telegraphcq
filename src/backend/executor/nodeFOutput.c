/*-------------------------------------------------------------------------
 *
 * nodeFOutput.c
 *
 *	  Routines to handle Query Output operations in shared CQ mode
 *
 * This operator uses the new Fjord "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFOutput.c,v 1.3 2004/02/23 22:37:16 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeFjord.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFOutput.h"
#include "miscadmin.h"
#include "nodes/nodeFuncs.h"
#include "rqueue/rqdest.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "telegraphcq/telegraphcq.h"
#include "utils/timestamp.h"

void DisplayOutputCalculations(FOutput *node, Plan *parent, int query);

/* #define AMOLDEBUGON 1 */

/* ----------------------------------------------------------------------------
 *	Dynamically-dispatched functions
 * ----------------------------------------------------------------------------
 */
/*
 * ProcessInitFOutput
 */
bool
ProcessInitFOutput(FOutput * node, EState *estate, Plan *parent)
{
	int i;
	
	DR_resultqdest *resultqdest;
	
	FjordInitialize(&(node->fjord), estate);	   /* Common initialization */

	resultqdest = (DR_resultqdest *) DestToFunction(ResultQueue);
	resultqdest->pub.setup((DestReceiver *) resultqdest, CMD_SELECT,
	  					 NULL, NULL);
	resultqdest->isIHT = true;
	resultqdest->eddy = (Eddy *) parent;

	node->resultqdest = resultqdest;
	
	node->total_entered = node->total_output = 0;

	BitArrayClearAll(node->signature, TCQMAXSOURCES);

	if (! (UseStaticPlan && (StaticEddy != NULL))) 
	{
		BitArrayClearAll(node->candidates, TCQMAXQUERIES);

		/* initialize query ART counters */
		for (i=0; i < TCQMAXQUERIES; i++) 
		{
			node->running_diff_sum[i] = -1;
			node->count[i].time = 0;
		}
		for (i=0; i < TCQMAXSOURCES; i++) 
		{
			node->ts_attr[i] = -1;
		}
	}

	return true;
}
    
/*
 * ProcessRemoveQueryFOutput
 */
bool
ProcessRemoveQueryFOutput(FOutput * node, int queryid, Plan *parent)
{
	return FjordRemoveQuery((Fjord *) node, queryid);
}

static void
UpdateOutputCalculations(FOutput * node, Plan *parent, int query)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	iiht = (IntermediateHeapTuple) input->val;

	if (UseSuppressOutput)
	{
		int i;
					
		/* this tuple needs to be output ot this query */
		Timestamp largest_ts = 0;
		Datum current_time;
		Datum diff_intvl;
		Datum new_intvl_sum;
		bool isNull;
					
		/* find the largest timestamp of the sources this query contains */
		for (i=0; i < TCQMAXSOURCES; i++) 
		{
			if (iiht->t_base_tuples[i] != NULL)
			{
				if (node->ts_attr[i] != -1)
				{
					Datum cur_ts_datum;
					Timestamp cur_ts;
								
					cur_ts_datum = heap_getattr(iiht->t_base_tuples[i],
												node->ts_attr[i] + 1,
												iiht->t_base_tuple_descs[i],
												&isNull);
					cur_ts = DatumGetTimestamp(cur_ts_datum);

					if (cur_ts > largest_ts)
						largest_ts = cur_ts;
				}
			}
		}
					
		current_time = DirectFunctionCall1(text_timestamp,
										   DirectFunctionCall1(timeofday,PointerGetDatum(NULL)));
		diff_intvl = DirectFunctionCall2(timestamp_mi,
										 current_time,
										 TimestampGetDatum(largest_ts));
#ifdef AMOLDEBUGON
		elog(INFO,"FOutput: diff_intvl=%s",
			 DatumGetCString(DirectFunctionCall1(interval_out, diff_intvl)));
#endif
		
		if (node->running_diff_sum[query] == -1) 
		{
			node->running_diff_sum[query] = diff_intvl;

			pfree(DatumGetPointer(current_time));
		}
		else 
		{
			new_intvl_sum = DirectFunctionCall2(interval_pl,
												node->running_diff_sum[query],
												diff_intvl);
			pfree(DatumGetPointer(node->running_diff_sum[query]));
			pfree(DatumGetPointer(current_time));
			pfree(DatumGetPointer(diff_intvl));

			node->running_diff_sum[query] = new_intvl_sum;
		}

		node->count[query].time += 1;
#ifdef AMOLDEBUGON					
		elog(DEBUG1,"FOutput ART calculation: count=%f, running_sum=%s",
			 node->count[query].time, DatumGetCString(DirectFunctionCall1(interval_out, node->running_diff_sum[query])));
#endif		
		
	}	
}


/* ----------------------------------------------------------------
 *		ProcessNextFOutput
 * ----------------------------------------------------------------
 */
void
ProcessNextFOutput(FOutput * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;					/* Common Fjord auto declarations */

	List 				*ubql;
	DR_resultqdest 		*resultqdest  = node->resultqdest;
	bool 				 needToOutput = false;
	bool				 enqsucc;
	bool 				 deqsucc;
	/*
	 * Fetch data from node
	 */
	FJORD_AUTOVAR_DEFNS;					  /* Common Fjordauto definitions */


#ifdef AMOLDEBUGON
	elog(DEBUG1,"ProcessNextFOutput");
#endif
	
	/*
	 *  Fetch next IHT from  i/p queue
	 *
	 *  NOTE:
	 *
	 *       Here we just manipulate pointers and modify the actual physical
	 *       Input IHT as necessary and then send on a pointer to the same IHT
	 */
	FJORD_DEQ_INP_PTR(deqsucc,node,&piiht);
	if (! deqsucc)
	{
		return;										/* No input ==> no output */
	}

	node->total_entered++;

	input->val = (HeapTuple) iiht;
	input->ttc_isIntermediateTuple = TRUE;

#ifdef AMOLDEBUGON
	elog(DEBUG1,"ProcessNextFOutput: Got a new tuple");
	debugitup_debug(iiht);
#endif
	if (UseTupleLineage) 
	{
		BitArrayClearAll(resultqdest->whereToSendOutput, TCQMAXQUERIES);
		foreach (ubql, fjord->usedByQueries)
		{
			int query = lfirsti(ubql);

#ifdef AMOLDEBUGON
			elog(DEBUG1,"Testing query %d", query);
#endif

			if (! BitArrayBitIsSet(iiht->t_queriesCompleted,query))
			{
#ifdef AMOLDEBUGON
				elog(DEBUG1,"Sending tuple to query %d", query);
#endif
				node->total_output++;

				UpdateOutputCalculations(node,parent,query);
				
				BitArraySetBit(resultqdest->whereToSendOutput, query);
				BitArraySetBit(iiht->t_queriesCompleted,query);
				needToOutput = true;
				node->total_output++;
			}
		}
	}
	else
	{
		BitArrayClone(resultqdest->whereToSendOutput, node->candidates, TCQMAXQUERIES);

		UpdateOutputCalculations(node,parent,lfirsti(fjord->usedByQueries));
		needToOutput = true;
		node->total_output++;
	}

	if (UseSuppressOutput)
	{
		int query = 0;
		
		for (query = 0; query < TCQMAXQUERIES; query++)
		{
			if (((int)node->count[query].time % 1000) == 0)
			{
				DisplayOutputCalculations(node, parent, query);
			}
		}
	}

	if (needToOutput && !UseSuppressOutput)
	{
										
#ifdef AMOLDEBUGON
		elog(DEBUG1, "GOT ONE: finished proc. a tuple... sending to output");
		debugitup_debug(iiht);
#endif

		resultqdest->pub.receiveTuple((HeapTuple) iiht, NULL,	/* Boo Hiss */
									  (DestReceiver *) resultqdest);
	}
	else 
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1,"GOT NO Output tuples :-(");
#endif
	}
	/*
	 *  We need to bounce back the input if there are other operators in this
	 *  MetaOperator chain. This is a pain to keep track of, but is probably an
	 *  optimization for the future. For now, let's just bounce back already.
	 */
	if (BitArrayIsAnyCleared(iiht->t_queriesCompleted, TCQMAXQUERIES)) 
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1,"Bouncing back the input ... ");
#endif
		FJORD_ENQ_OUT_PTR(enqsucc,node,&iiht);		 /* Bounce back the input */
	}
	fjord->doneInput = true;
}


void
DisplayOutputCalculations(FOutput *node, Plan *parent, int query)
{
	if (node->count[query].time != 0)
	{
		Datum art, art_text;
		/*Datum diff_sum_text;*/
        int probes;
		
#ifdef AMOLDEBUGON
		diff_sum_text = DirectFunctionCall1(interval_out,
											node->running_diff_sum[query]);
		elog(DEBUG1, "FOutput performing ART calculation: diff_sum=%s / count = %f",
			 DatumGetCString(diff_sum_text), node->count[query].time);
#endif
		art = DirectFunctionCall2(interval_div,
								  node->running_diff_sum[query],
								  Float8GetDatum(node->count[query].time));
		
		art_text = DirectFunctionCall1(interval_out,
									   art);
		if (UseStaticPlan) 
		{
			probes = getTotalProbes(StaticEddy);
		}
		else 
		{
			probes = getTotalProbes((Eddy *) parent);
		}		
		elog(INFO,"FOutput ART Calculation (Query %d): ART=%s, Number of Tuples=%f probes=%d",
			 query, DatumGetCString(art_text), node->count[query].time,probes);
	}
}
	

/* ----------------------------------------------------------------
 *		ProcessEndFOutput
 * ----------------------------------------------------------------
 */
bool
ProcessEndFOutput(FOutput * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */
	
	elog(DEBUG1,"In ProcessEndFOutput ");

	if (UseSuppressOutput)
	{
		int query = 0;
		
		elog(INFO,"BEGIN FINAL ART OUTPUT CALCULATIONS\n*****************************");
		
		for(query = 0; query < TCQMAXQUERIES; query++) 
		{
			DisplayOutputCalculations(node, parent, query);
		}
	}
	
	FjordEnd(&node->fjord, eddy);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(FOutput));

	return true;
}

/* ----------------------------------------------------------------
 *		ProcessPrintStatsFOutput_with_level
 * ----------------------------------------------------------------
 */
static void
ProcessPrintStatsFOutput_with_level(FOutput * node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	elog(level, "FOutput: [Tuples entered: %d] [Tuples ouput: %d]",
		 node->total_entered, node->total_output);
}

/* ----------------------------------------------------------------
 *		ProcessPrintStatsFOutput
 * ----------------------------------------------------------------
 */
void
ProcessPrintStatsFOutput(FOutput *node, Plan *parent)
{
	ProcessPrintStatsFOutput_with_level(node,parent,INFO);
}
