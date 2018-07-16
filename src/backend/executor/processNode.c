/*-------------------------------------------------------------------------
 *
 * processNode.c
 *
 *	 Contains dispatch functions for the new "Fjord" operators as opposed to the
 *	 standard "Interator" interface.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/processNode.c,v 1.32 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *
 *
 * Dynamically Dispatched functions
 *
 *		Implementing a new Fjord-model operator involves completing each of the
 *		following dynamically dispatched functions for your new operator:
 *
 *	  ProcessInitNode
 *	  ProcessNextNode
 *	  ProcessEndNode
 *	  ProcessRemoveQuery
 *	  ProcessMarkDynCatalog
 *
 * Generic, non-dynamically dispatched routines
 *
 *		 When implementing a new Fjord-model operator, check each of these to
 *		 ensure that they're doing the "right thing" for your operator.
 *
 *	  FjordAddQuery
 *	  FjordRemoveQuery
 *	  FjordInitialize
 *	  FjordEnd
 *
 *
 */
#include "postgres.h"

#include "executor/executor.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFilter.h"
#include "executor/nodeFFilter.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFOutput.h"
#include "executor/nodeFSplit.h"
#include "executor/nodeFSteM.h"
#include "executor/nodeGSFilter.h"
#include "executor/nodeFAgg.h"
#include "executor/nodeFEventAgg.h"
#include "executor/nodeScanModule.h"
#include "nodes/print.h"
#include "telegraphcq/dyncatalog.h"
#include "utils/memutils.h"

/* #define AMOLDEBUGON 1  */

static void ProcessNextStaticNode(Plan *node, Plan *parent);

/* -----------------------------------------------------------------------------
 * Dynamically Dispatched functions
 *
 * Implementing a new Fjord-model operator involves completing each of the
 * following dynamically dispatched functions for your new operator
 *
 * ProcessInitNode
 * ProcessNextNode
 * ProcessEndNode
 * ProcessRemoveQuery
 * ProcessMarkDynCatalog
 * -----------------------------------------------------------------------------
 */

/*
 * ProcessInitNode
 *
 * This function is responsible for a one-time initialization of a new instance
 * of a Fjord operator.
 *
 */
bool
ProcessInitNode(Plan *node, EState *estate, Plan *parent)
{
	bool		result;

	switch (nodeTag(node))
	{
		case T_ScanModule:
			result = ProcessInitScanModule((ScanModule *) node, estate, parent);
			break;
		case T_FFilter:
			result = ProcessInitFFilter((FFilter *) node, estate, parent);
			break;
		case T_Filter:
			result = ProcessInitFilter((Filter *) node, estate, parent);
			break;
		case T_GSFilter:
			result = ProcessInitGSFilter((GSFilter *) node, estate, parent);
			break;
		case T_FSteM:
			result = ProcessInitFSteM((FSteM *) node, estate, parent);
			break;
		case T_FOutput:
			result = ProcessInitFOutput((FOutput *) node, estate, parent);
			break;
		case T_FSplit:
			result = ProcessInitFSplit((FSplit *) node, estate, parent);
			break;
	case T_FAgg:
	  result = ProcessInitFAgg((FAgg *) node, estate, parent);
	  break;	
	case T_FEventAgg:
	  result = ProcessInitFEventAgg((FEventAgg *) node, estate, parent);
	  break;
		default:
			elog(LOG, "ProcessInitNode: node unsupported");
			pprint(node);
			elog(ERROR, "ProcessInitNode: node foo type %d unsupported",
				 (int) nodeTag(node));
			result = false;
			break;
	}
	return result;
}

static void 
ProcessNextStaticNode(Plan *node, Plan *parent)
{
	Fjord *fjord = (Fjord *) node;
	Fjord *next;
	int i;
	
	switch (nodeTag(node)) 
	{
		case T_ScanModule:
		case T_GSFilter:
		case T_FFilter:
		case T_FSteM:
			next = fjord->outputs[StaticDriver];
			if (! QEMPTY(next->input))
			{
				do 
				{
					ProcessNextNode ((Plan *) next, parent);
				}
				while (! next->doneInput);
			}
			break;
		case T_FOutput:
			break;
		case T_FSplit:
			for (i = 0; i < fjord->numOutputs; i++) 
			{
				next = fjord->outputs[i];
				
				if (! QEMPTY(next->input))
				{
					do 
					{
						ProcessNextNode((Plan *) next, parent);
					}
					while (! next->doneInput);
				}
			}
			break;
		default:
			elog(ERROR,"ProcessNextStaticNode: Eeek .. this is bad ..");
			break;
	}
}


/*
 * ProcessNextNode
 *
 * This function is the main workhorse - called everytime there is "work" for
 * the operator
 *
 */
void
ProcessNextNode(Plan *node, Plan *parent)
{
  Fjord *fjord = (Fjord*)node;
	((Fjord *)node)->doneInput = false;
	
	switch (nodeTag(node))
	{
		case T_ScanModule:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "SCANMODULE: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextScanModule((ScanModule *) node, parent);
			break;
		case T_GSFilter:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "GSFILTER: %s %d\n", __FILE__, __LINE__);
#endif
			/* elog(DEBUG1, "GOING TO GSFILTER"); */
			ProcessNextGSFilter((GSFilter *) node, parent);
			break;
		case T_FSteM:

			/*
			 * elog(DEBUG1, "GOING TO FSTEM %s",
			 * getNodeInformation(parent, ((FSteM *)
			 * node)->protostem->innerhashkey));
			 */
#ifdef AMOLDEBUGON
			elog(DEBUG1, "FSTEM: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextFSteM((FSteM *) node, parent);
			break;
		case T_FOutput:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "FOUTPUT: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextFOutput((FOutput *) node, parent);
			break;
		case T_FSplit:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "FSPLIT: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextFSplit((FSplit *) node, parent);
			break;
		case T_FAgg:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "FAGG: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextFAgg((FAgg *) node, parent);
			break;
		case T_FFilter:
#ifdef AMOLDEBUGON
			elog(DEBUG1, "FFILTER: %s %d\n", __FILE__, __LINE__);
#endif
			ProcessNextFFilter((FFilter *) node, parent);
			break;

	case T_FEventAgg:
	  elog(DEBUG1, "FEventAgg: %s %d\n", __FILE__, __LINE__);
	  ProcessNextFEventAgg((FEventAgg *) node, parent);
	  break;
	  
		default:
			elog(ERROR, "ProcessNextNode: node type %d unsupported",
				 (int) nodeTag(node));
			break;
	}
	if (UseStaticPlan && (StaticEddy != NULL))
	{
		ProcessNextStaticNode(node,parent);
	}
	if(fjord->tempContext  && fjord->usesTempContext==true)
	  {
	    MemoryContextReset(fjord->tempContext);
	  }
}

/*
 * ProcessEndNode
 *
 * This function is called to shutdown a module prior to removal from the eddy.
 *
 */
bool
ProcessEndNode(Plan *node, Plan *parent)
{
	bool		result;

	switch (nodeTag(node))
	{
		case T_ScanModule:
			result = ProcessEndScanModule((ScanModule *) node, parent);
			break;
		case T_FFilter:
			result = ProcessEndFFilter((FFilter *) node, parent);
			break;
		case T_Filter:
			result = ProcessEndFilter((Filter *) node, parent);
			break;
		case T_GSFilter:
			result = ProcessEndGSFilter((GSFilter *) node, parent);
			break;
		case T_FSteM:
			result = ProcessEndFSteM((FSteM *) node, parent);
			break;
		case T_FOutput:
			result = ProcessEndFOutput((FOutput *) node, parent);
			break;
		case T_FSplit:
			result = ProcessEndFSplit((FSplit *) node, parent);
			break;
		case T_FAgg:
			result = ProcessEndFAgg((FAgg *) node, parent);
			break;
	case T_FEventAgg:
	  result = ProcessEndFEventAgg((FEventAgg *) node, parent);
	  break;
		default:
			elog(DEBUG1, "ProcessInitNode: node unsupported");
			pprint(node);
			elog(ERROR, "ProcessInitNode: node foo type %d unsupported",
				 (int) nodeTag(node));
			result = false;
			break;
	}
	return result;

}

/*
 *	ProcessRemoveQuery
 *
 * code called to remove a query of a specified queryid from the eddy.
 * this function is responsible only for removing and destroying
 * particular query components.  The function returns true if the last
 * query has been removed from the module, false otherwise.
 */
bool
ProcessRemoveQuery(Plan *node, int queryid, Plan *parent)
{
	/*
	 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
	 *
	 * If you're adding a new Fjord model operator, you MUST use
	 * ProcessRemoveQueryCommon to remove the actual queryid from the
	 * Fjord's usedByQueries list. This also ensures that the right events
	 * are generated in the tcq_operators dynamic catalog stream
	 *
	 * TBD: Maybe the logic in ProcessRemoveQueryCommon can be moved to the
	 * end of ProcessRemoveQuery ?
	 *
	 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
	 */
	switch (nodeTag(node))
	{
		case T_ScanModule:
			return ProcessRemoveQueryScanModule((ScanModule *) node, queryid, parent);
			break;
		case T_GSFilter:
			return ProcessRemoveQueryGSF((GSFilter *) node, queryid, parent);
			break;
		case T_FSteM:
			return ProcessRemoveQueryFSteM((FSteM *) node, queryid, parent);
			break;
		case T_FOutput:
			return ProcessRemoveQueryFOutput((FOutput *) node, queryid, parent);
			break;
		case T_FSplit:
			return ProcessRemoveQueryFSplit((FSplit *) node, queryid, parent);
			break;
		case T_FAgg:
			return ProcessRemoveQueryFAgg((FAgg *) node, queryid, parent);
			break;
	case T_FEventAgg:
	  return ProcessRemoveQueryFEventAgg((FEventAgg *) node, queryid, parent);
	  break;
		default:
			elog(DEBUG1, "removal for node type %d is not done", nodeTag(node));
			return false;
	}
	return false;
}

/*
 *	ProcessPrintStats
 *
 * code called to print statistics of a specific operator
 */
void
ProcessPrintStats(Plan *node, Plan *parent)
{
	elog(INFO,"Printing stats ..");
	
	switch (nodeTag(node))
	{
		case T_ScanModule:
			ProcessPrintStatsScanModule((ScanModule *) node, parent);
			break;
		case T_GSFilter:
			ProcessPrintStatsGSFilter((GSFilter *) node, parent);
			break;
		case T_FSteM:
			ProcessPrintStatsFSteM((FSteM *) node, parent);
			break;
		case T_FOutput:
			ProcessPrintStatsFOutput((FOutput *) node, parent);
			break;
		case T_FSplit:
			ProcessPrintStatsFSplit((FSplit *) node, parent);
			break;
		case T_FAgg:
			ProcessPrintStatsFAgg((FAgg *) node, parent);
			break;
	case T_FEventAgg:
	  ProcessPrintStatsFEventAgg((FEventAgg *) node, parent);
	  break;
		default:
			elog(LOG, "Printing stats for node %d is not done",nodeTag(node));
	}
}

/* -----------------------------------------------------------------------------
 * Generic, common, functions
 *
 * When implementing a new Fjord-model operator, check each of these to ensure
 * that they're doing the "right thing" for your operator.
 *
 * FjordAddQuery
 * FjordRemoveQuery
 * FjordInitialize
 * FjordEnd
 * -----------------------------------------------------------------------------
 */

/*
 * FjordAddQuery
 *
 * This function notifies a Fjord module that a query is being added to it.  The
 * actual addition is done through module-dependant means, and is therefore not
 * a part of the interface.
 */
bool
FjordAddQuery(Plan *p, int32 queryid, Plan *parent)
{
	Fjord	   *f = (Fjord *) p;

	Assert(IsA_Fjord(p));
	if (!intMember(queryid, f->usedByQueries))
	{
		f->usedByQueries = lconsi(queryid, f->usedByQueries);
		BitArrayClearBit(f->interestedQueries,queryid);		
		tcq_operators_add(f, (Eddy *) parent, queryid);
	}
	return true;
}

/*
 * FjordRemoveQuery
 *
 * This function should be called by the various ProcessRemoveQuery functions of
 * each Fjord operator to perform the actual work of removing any queries extant
 * for this Fjord operator.
 *
 * f		(IN)  Pointer to Fjord model operator
 * queryid	(IN)  Queryid that is being removed
 *
 * RETURNS		True only if the last query has been removed for this operator
 *
 */
bool
FjordRemoveQuery(Fjord * f, int queryid)
{
	int			oldlen;
	int			newlen;

	oldlen = length(f->usedByQueries);
	f->usedByQueries = lremovei(queryid, f->usedByQueries);	
	f->usedByQueries = lremovei(queryid, f->usedByQueries);
	newlen = length(f->usedByQueries);
	if (oldlen != newlen) 
	{
		tcq_operators_remove(f, queryid);
	}
	return (newlen == 0) ? true : false;
}

/*
 * FjordInitialize
 *
 * This routine does a bunch of initialization common to Fjord operators
 *
 */
void
FjordInitialize(Fjord * fjord, EState *estate)
{
	CommonScanState *scanstate;
	MemoryContext octx;

	Assert(outerPlan((Plan *) fjord) == NULL);
	Assert(innerPlan((Plan *) fjord) == NULL);

	fjord->mctx = AllocSetContextCreate(TopMemoryContext,
										"Fjord Context",
										ALLOCSET_DEFAULT_MINSIZE,
										ALLOCSET_DEFAULT_INITSIZE,
										ALLOCSET_DEFAULT_MAXSIZE);
	octx = MemoryContextSwitchTo(fjord->mctx);
	make_iht_queue(&fjord->input, FJORDQUEUESIZE);
	make_iht_queue(&fjord->output, FJORDQUEUESIZE);
	if (! (UseStaticPlan && (StaticEddy != NULL)))              /* @dsigmodSK */
	{                                                           /* @dsigmodSK */
		fjord->usedByQueries = NULL;
		BitArraySetAll(fjord->interestedQueries,TCQMAXQUERIES);                        /* @dsigmodSK */
	}/* @dsigmodSK */
	
	fjord->thisMOlist = NIL;
	fjord->outputs 	  = NULL;                                   /* @dsigmodSK */
	fjord->numOutputs = 0;                                      /* @dsigmodSK */
	fjord->driver 	  = -1;                                     /* @dsigmodSK */

	fjord->plan.state = estate; /* Assign the node's execution state */

	scanstate = makeNode(CommonScanState);		/* Make new
							 * CommonScanState for
							 * node */
	fjord->scanstate = scanstate;
	fjord->scanstate->css_ScanTupleSlot = MakeTupleTableSlot();
	InitSlotForEddy(fjord->scanstate->css_ScanTupleSlot);

	ExecAssignExprContext(estate, &scanstate->cstate);	/* create expr context */
	fjord->tempContext=scanstate->cstate.cs_ExprContext->ecxt_per_tuple_memory; 
	MemoryContextSwitchTo(octx);
}

/*
 * EndFjord
 *
 * Free a Fjord Operator
 */
void
FjordEnd(Fjord * fjord, Eddy * eddy)
{
	int			operator = fjord->operator;

	free_queue(&fjord->input);
	free_queue(&fjord->output);
	ExecFreeExprContext(&fjord->scanstate->cstate);
	ExecClearTuple(fjord->scanstate->css_ScanTupleSlot);
	ExecFreeSlotIfOwned(fjord->scanstate->css_ScanTupleSlot);
	if (fjord->mctx)
	{
		MemoryContextDelete(fjord->mctx);
		fjord->mctx = NULL;

	}

	/*
	 * Clean up ready/done bits for this operator in the PostJoinOperator
	 * list
	 */
	cleanOperatorFromPJOList(eddy, operator);
}
