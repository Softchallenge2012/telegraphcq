/*-------------------------------------------------------------------------
 *
 * execProcnode.c
 *	 contains dispatch functions which call the appropriate "initialize",
 *	 "get a tuple", and "cleanup" routines for the given node type.
 *	 If the node has children, then it will presumably call ExecInitNode,
 *	 ExecProcNode, or ExecEndNode on its subnodes and do the appropriate
 *	 processing..
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/execProcnode.c,v 1.33 2005/12/14 20:49:22 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 *	 INTERFACE ROUTINES
 *		ExecInitNode	-		initialize a plan node and its subplans
 *		ExecProcNode	-		get a tuple by executing the plan node
 *		ExecEndNode		-		shut down a plan node and its subplans
 *		ExecCountSlotsNode -	count tuple slots needed by plan tree
 *		ExecGetTupType	-		get result tuple type of a plan node
 *
 *	 NOTES
 *		This used to be three files.  It is now all combined into
 *		one file so that it is easier to keep ExecInitNode, ExecProcNode,
 *		and ExecEndNode in sync when new nodes are added.
 *
 *	 EXAMPLE
 *		suppose we want the age of the manager of the shoe department and
 *		the number of employees in that department.  so we have the query:
 *
 *				retrieve (DEPT.no_emps, EMP.age)
 *				where EMP.name = DEPT.mgr and
 *					  DEPT.name = "shoe"
 *
 *		Suppose the planner gives us the following plan:
 *
 *						Nest Loop (DEPT.mgr = EMP.name)
 *						/		\
 *					   /		 \
 *				   Seq Scan		Seq Scan
 *					DEPT		  EMP
 *				(name = "shoe")
 *
 *		ExecStart() is called first.
 *		It calls InitPlan() which calls ExecInitNode() on
 *		the root of the plan -- the nest loop node.
 *
 *	  * ExecInitNode() notices that it is looking at a nest loop and
 *		as the code below demonstrates, it calls ExecInitNestLoop().
 *		Eventually this calls ExecInitNode() on the right and left subplans
 *		and so forth until the entire plan is initialized.
 *
 *	  * Then when ExecRun() is called, it calls ExecutePlan() which
 *		calls ExecProcNode() repeatedly on the top node of the plan.
 *		Each time this happens, ExecProcNode() will end up calling
 *		ExecNestLoop(), which calls ExecProcNode() on its subplans.
 *		Each of these subplans is a sequential scan so ExecSeqScan() is
 *		called.  The slots returned by ExecSeqScan() may contain
 *		tuples which contain the attributes ExecNestLoop() uses to
 *		form the tuples it returns.
 *
 *	  * Eventually ExecSeqScan() stops returning tuples and the nest
 *		loop join ends.  Lastly, ExecEnd() calls ExecEndNode() which
 *		calls ExecEndNestLoop() which in turn calls ExecEndNode() on
 *		its subplans which result in ExecEndSeqScan().
 *
 *		This should show how the executor works by having
 *		ExecInitNode(), ExecProcNode() and ExecEndNode() dispatch
 *		their work to the appopriate node support routines which may
 *		in turn call these routines themselves on their subplans.
 *
 */
#include "postgres.h"

#include "executor/executor.h"
#include "executor/instrument.h"
#include "executor/nodeAgg.h"
#include "executor/nodeAppend.h"
#include "executor/nodeEddy.h"	/* @deddySK */
#include "executor/nodeFSched.h"	/* @dfluxMS */
#include "executor/nodeGroup.h"
#include "executor/nodeHash.h"
#include "executor/nodeHashGroup.h"		/* @dhashgrpMS */
#include "executor/nodeHashjoin.h"
#include "executor/nodeIndexscan.h"
#include "executor/nodeIndexSteM.h"		/* @dstemsAD */
#include "executor/nodeTidscan.h"
#include "executor/nodeLimit.h"
#include "executor/nodeMaterial.h"
#include "executor/nodeMergejoin.h"
#include "executor/nodeNestloop.h"
#include "executor/nodeResult.h"
#include "executor/nodeScanq.h" /* @dtcqSK */
#include "executor/nodeScanSock.h" /* @dtcqSK */
#include "executor/nodeSeqscan.h"
#include "executor/nodeSetOp.h"
#include "executor/nodeSort.h"
#include "executor/nodeSteM.h"	/* @deddySK */
#include "executor/nodeFAgg.h"
#include "executor/nodeSteMHashjoin.h"
#include "executor/nodeStrmScan.h"		/* @dwchMS */
#include "executor/nodeSubplan.h"
#include "executor/nodeSubqueryscan.h"
#include "executor/nodeFunctionscan.h"
#include "executor/nodeUnique.h"
#include "executor/nodeEddy.h"	/* @deddySK */
#include "executor/nodeSteM.h"	/* @deddySK */
#include "executor/nodeStrmScan.h"		/* @dwchMS */
#include "executor/nodeWindow.h"	/* @dwndaggWH */
#include "executor/nodeHashGroup.h"		/* @dhashgrpMS */
#include "executor/nodeFSched.h"	/* @dfluxMS */
#include "executor/nodeIndexSteM.h"		/* @dstemsAD */
#include "executor/nodeSteMHashjoin.h"	/* @dstemsAD */
#include "executor/nodeWindow.h"	/* @dwndaggWH */
#include "executor/nodeFluxCons.h"		/* @dfluxMS */
#include "executor/nodeFluxProd.h"		/* @dfluxMS */
#include "executor/nodeFluxCons_FT.h"	/* @dfluxMS */
#include "executor/nodeFluxProd_FT.h"	/* @dfluxMS */
#include "executor/nodeStateMover.h"	/* @dfluxMS */
#include "executor/nodeFluxAcceptor.h"	/* @dfluxMS */
#include "executor/nodeDataGen.h"		/* @dfluxMS */
#include "executor/nodeSegCons.h"		/* @dfluxMS */
#include "executor/nodeSegProd.h"		/* @dfluxMS */
#include "executor/nodeFluxIngress.h"	/* @dfluxMS */
#include "executor/nodeFluxEgress.h"	/* @dfluxMS */
#include "miscadmin.h"
#include "tcop/tcopprot.h"

/* ------------------------------------------------------------------------
 *		ExecInitNode
 *
 *		Recursively initializes all the nodes in the plan rooted
 *		at 'node'.
 *
 *		Initial States:
 *		  'node' is the plan produced by the query planner
 *
 *		returns TRUE/FALSE on whether the plan was successfully initialized
 * ------------------------------------------------------------------------
 */
bool
ExecInitNode(Plan *node, EState *estate, Plan *parent)
{
	bool		result;
	List	   *subp;

	/*
	 * do nothing when we get to the end of a leaf on tree.
	 */
	if (node == NULL)
		return FALSE;

	/* Set up instrumentation for this node if the parent has it */
	if (!node->instrument && parent && parent->instrument)
		node->instrument = InstrAlloc();

	foreach(subp, node->initPlan)
	{
		result = ExecInitSubPlan((SubPlan *) lfirst(subp), estate, node);
		if (result == FALSE)
			return FALSE;
	}

	switch (nodeTag(node))
	{
			/*
			 * control nodes
			 */
		case T_Result:
			result = ExecInitResult((Result *) node, estate, parent);
			break;

		case T_Append:
			result = ExecInitAppend((Append *) node, estate, parent);
			break;

			/*
			 * scan nodes
			 */
		case T_SeqScan:
			result = ExecInitSeqScan((SeqScan *) node, estate, parent);
			break;

		case T_IndexScan:
			result = ExecInitIndexScan((IndexScan *) node, estate, parent);
			break;

		case T_TidScan:
			result = ExecInitTidScan((TidScan *) node, estate, parent);
			break;

		case T_ScanQueue:
			result = ExecInitScanQueue((ScanQueue *) node, estate, parent);
			break;

		case T_ScanSock:
			result = ExecInitScanSock((ScanSock *) node, estate, parent);
			break;


		case T_StrmScan:
			result = ExecInitStrmScan((StrmScan *) node, estate, parent);
			break;

		case T_SubqueryScan:
			result = ExecInitSubqueryScan((SubqueryScan *) node, estate,
										  parent);
			break;

		case T_FunctionScan:
			result = ExecInitFunctionScan((FunctionScan *) node, estate,
										  parent);
			break;

			/*
			 * join nodes
			 */
		case T_NestLoop:
			result = ExecInitNestLoop((NestLoop *) node, estate, parent);
			break;

		case T_MergeJoin:
			result = ExecInitMergeJoin((MergeJoin *) node, estate, parent);
			break;

		case T_Hash:
			result = ExecInitHash((Hash *) node, estate, parent);
			break;

		case T_HashJoin:
			result = ExecInitHashJoin((HashJoin *) node, estate, parent);
			break;

			/*
			 * materialization nodes
			 */
		case T_Material:
			result = ExecInitMaterial((Material *) node, estate, parent);
			break;

		case T_Sort:
			result = ExecInitSort((Sort *) node, estate, parent);
			break;

		case T_Unique:
			result = ExecInitUnique((Unique *) node, estate, parent);
			break;

		case T_SetOp:
			result = ExecInitSetOp((SetOp *) node, estate, parent);
			break;

		case T_Limit:
			result = ExecInitLimit((Limit *) node, estate, parent);
			break;

		case T_Group:
			result = ExecInitGroup((Group *) node, estate, parent);
			break;

			/* @BwndaggWH */
			/*case T_Window:
			result = ExecInitWindow((Window *) node, estate, parent);
			break; */
			/* @EwndaggWH */

			/* @BhashgrpMS */
		case T_HashGroup:
			{
				result = ExecInitHashGroup((HashGroup *) node, estate, parent);
				break;
			}
			/* @EhashgrpMS */

			/* @BfluxMS */
		case T_FluxCons:
			{
				result = ExecInitFluxCons((FluxCons *) node, estate, parent);
				break;
			}

		case T_FluxProd:
			{
				result = ExecInitFluxProd((FluxProd *) node, estate, parent);
				break;
			}

		case T_FluxCons_FT:
			{
				result = ExecInitFluxCons_FT((FluxCons_FT *) node, estate, parent);
				break;
			}

		case T_FluxProd_FT:
			{
				result = ExecInitFluxProd_FT((FluxProd_FT *) node, estate, parent);
				break;
			}

		case T_DataGen:
			{
				result = ExecInitDataGen((DataGen *) node, estate, parent);
				break;
			}

		case T_SegCons:
			{
				result = ExecInitSegCons((SegCons *) node, estate, parent);
				break;
			}

		case T_FluxEgress:
			{
				result = ExecInitFluxEgress((FluxEgress *) node, estate, parent);
				break;
			}
			
		case T_FluxIngress:
			{
				result = ExecInitFluxIngress((FluxIngress *) node, estate, parent);
				break;
			}

		case T_StateMover:
			{
				result = ExecInitStateMover((StateMover *) node, estate, parent);
				break;
			}

		case T_SegProd:
			{
				result = ExecInitSegProd((SegProd *) node, estate, parent);
				break;
			}

		case T_FluxAcceptor:
			result = ExecInitFluxAcceptor((FluxAcceptor *) node, estate, parent);
			break;
			/* @EfluxMS */

		case T_Agg:
			result = ExecInitAgg((Agg *) node, estate, parent);
			break;
		case T_FAgg:
			result = ExecInitFAgg((FAgg *) node, estate, parent);
			break;

		case T_Eddy:			/* @BdeddySK */
			result = ExecInitEddy((Eddy *) node, estate, parent);
			break;

		case T_FSched:			/* @dfluxMS */
			result = ExecInitFSched((FSched *) node, estate, parent);
			break;

		case T_SteM:			/* @BdeddySK */
			result = ExecInitSteM((SteM *) node, estate, parent);
			break;

		case T_IndexSteM:		/* @dstemsAD */
			result = ExecInitIndexSteM((IndexSteM *) node, estate, parent);		/* @dstemsAD */
			break;				/* @dstemsAD */

		case T_SteMHashJoin:	/* @dstemsAD */
			result = ExecInitSteMHashJoin((SteMHashJoin *) node, estate, parent);		/* @dstemsAD */
			break;				/* @dstemsAD */

		case T_Filter:
			/* Error ! Filter does not support iterator interface ! */
			elog(ERROR, "ExecInitNode does not support Filter nodes");
			result = FALSE;
			break;				/* @EdeddySK */

		default:
			elog(ERROR, "ExecInitNode: node type %d unsupported",
				 (int) nodeTag(node));
			result = FALSE;
			break;
	}

	if (result != FALSE)
	{
		foreach(subp, node->subPlan)
		{
			result = ExecInitSubPlan((SubPlan *) lfirst(subp), estate, node);
			if (result == FALSE)
				return FALSE;
		}
	}

	return result;
}


/* ----------------------------------------------------------------
 *		ExecProcNode
 *
 *		Initial States:
 *		  the query tree must be initialized once by calling ExecInit.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecProcNode(Plan *node, Plan *parent)
{
	TupleTableSlot *result;

	CHECK_FOR_INTERRUPTS();

	/*
	 * deal with NULL nodes..
	 */
	if (node == NULL)
		return NULL;

	if (node->chgParam != NULL) /* something changed */
		ExecReScan(node, NULL, parent); /* let ReScan handle this */

	if (node->instrument)
		InstrStartNode(node->instrument);

	switch (nodeTag(node))
	{
			/*
			 * control nodes
			 */
		case T_Result:
			elog(WARNING, "execProcnode======t-Result");
			result = ExecResult((Result *) node);
			break;

		case T_Append:
			elog(WARNING, "execProcnode======t-Append");
			result = ExecProcAppend((Append *) node);
			break;

			/*
			 * scan nodes
			 */
		case T_SeqScan:
			elog(WARNING, "execProcnode======t-SeqScan");
			result = ExecSeqScan((SeqScan *) node);
			break;

		case T_IndexScan:
			elog(WARNING, "execProcnode======t-IndexScan");
			result = ExecIndexScan((IndexScan *) node);
			break;

		case T_TidScan:
			elog(WARNING, "execProcnode======t-TidScan");
			result = ExecTidScan((TidScan *) node);
			break;

		case T_ScanQueue:
			elog(WARNING, "execProcnode======t-ScanQueue");
			result = ExecScanQueue((ScanQueue *) node);
			break;

		case T_ScanSock:
			elog(WARNING, "execProcnode======t-ScanSock");
			result = ExecScanSock((ScanSock *) node);
			break;


		case T_StrmScan:
			elog(WARNING, "execProcnode======t-StrmScan");
			result = ExecStrmScan((StrmScan *) node);
			break;

		case T_SubqueryScan:
			elog(WARNING, "execProcnode======t-SubqueryScan");
			result = ExecSubqueryScan((SubqueryScan *) node);
			break;

		case T_FunctionScan:
			elog(WARNING, "execProcnode======t-FunctionScan");
			result = ExecFunctionScan((FunctionScan *) node);
			break;

			/*
			 * join nodes
			 */
		case T_NestLoop:
			elog(WARNING, "execProcnode======t-NestLoop");
			result = ExecNestLoop((NestLoop *) node);
			break;

		case T_MergeJoin:
			elog(WARNING, "execProcnode======t-MergeJoin");
			result = ExecMergeJoin((MergeJoin *) node);
			break;

		case T_Hash:
			elog(WARNING, "execProcnode======t-Hash");
			result = ExecHash((Hash *) node);
			break;

		case T_HashJoin:
			elog(WARNING, "execProcnode======t-HashJoin");
			result = ExecHashJoin((HashJoin *) node);
			break;

			/*
			 * materialization nodes
			 */
		case T_Material:
			elog(WARNING, "execProcnode======t-Material");
			result = ExecMaterial((Material *) node);
			break;

		case T_Sort:
			elog(WARNING, "execProcnode======t-Sort");
			result = ExecSort((Sort *) node);
			break;

		case T_Unique:
			elog(WARNING, "execProcnode======t-Unique");
			result = ExecUnique((Unique *) node);
			break;

		case T_SetOp:
			elog(WARNING, "execProcnode======t-SetOp");
			result = ExecSetOp((SetOp *) node);
			break;

		case T_Limit:
			elog(WARNING, "execProcnode======t-Limit");
			result = ExecLimit((Limit *) node);
			break;

		case T_Group:
			elog(WARNING, "execProcnode======t-Group");
			result = ExecGroup((Group *) node);
			break;

			/* @BwndaggWH */
			/*case T_Window:
			result = ExecWindow((Window *) node);
			break; */
			/* @EwndaggWH */

			/* @BhashgrpMS */
		case T_HashGroup:
			elog(WARNING, "execProcnode======t-HashGroup");
			result = ExecHashGroup((HashGroup *) node);
			break;
			/* @EhashgrpMS */

			/* @BfluxMS */
		case T_FluxCons:
			elog(WARNING, "execProcnode======t-FluxCons");
			result = ExecFluxCons((FluxCons *) node);
			break;

		case T_FluxProd:
			elog(WARNING, "execProcnode======t-FluxProd");
			result = ExecFluxProd((FluxProd *) node);
			break;

		case T_FluxCons_FT:
			elog(WARNING, "execProcnode======t-FluxCons-FT");
			result = ExecFluxCons_FT((FluxCons_FT *) node);
			break;

		case T_FluxProd_FT:
			elog(WARNING, "execProcnode======t-FluxProd_FT");
			result = ExecFluxProd_FT((FluxProd_FT *) node);
			break;

		case T_SegCons:
			elog(WARNING, "execProcnode======t-SegCons");
			result = ExecSegCons((SegCons *) node);
			break;

		case T_FluxEgress:
			elog(WARNING, "execProcnode======t-FluxEgress");
			result = ExecFluxEgress((FluxEgress *) node);
			break;

		case T_FluxIngress:
			elog(WARNING, "execProcnode======t-FluxIntegress");
			result = ExecFluxIngress((FluxIngress *) node);
			break;

		case T_SegProd:
			elog(WARNING, "execProcnode======t-SegProd");
			result = ExecSegProd((SegProd *) node);
			break;

		case T_DataGen:
			elog(WARNING, "execProcnode======t-DataGen");
			result = ExecDataGen((DataGen *) node);
			break;

		case T_StateMover:
			elog(WARNING, "execProcnode======t-StateMover");
			result = ExecStateMover((StateMover *) node);
			break;

		case T_FluxAcceptor:
			elog(WARNING, "execProcnode======t-FluxAcceptor");
			result = ExecFluxAcceptor((FluxAcceptor *) node);
			break;
			/* @EfluxMS */

		case T_Agg:
			elog(WARNING, "execProcnode======t-Agg");
			result = ExecAgg((Agg *) node);
			break;
		case T_FAgg:
			elog(WARNING, "execProcnode======t-FAgg");
			result = ExecFAgg((FAgg *) node);
			break;

		case T_Eddy:			/* @BdeddySK */
			elog(WARNING, "execProcnode======t-Eddy");
			result = ExecEddy((Eddy *) node);
			break;

		case T_FSched:			/* @dfluxMS */
			elog(WARNING, "execProcnode======t-FSched");
			result = ExecFSched((FSched *) node);
			break;

		case T_SteM:
			elog(WARNING, "execProcnode======t-Stem");
			result = ExecSteM((SteM *) node);
			break;

		case T_IndexSteM:
			elog(WARNING, "execProcnode======t-indexStem");
			result = ExecIndexSteM((IndexSteM *) node);
			break;

		case T_SteMHashJoin:	/* @BdeddySK */
			elog(WARNING, "execProcnode======t-stemhashjoin");
			result = ExecSteMHashJoin((SteMHashJoin *) node);
			break;

		case T_Filter:
			elog(WARNING, "execProcnode======t-filter");
			/* Error ! Filter nodes do not support iterator interface ! */
			elog(ERROR, "ExecProcNode unsupported for Filter !");		/* @EdeddySK */

		default:			
			elog(ERROR, "ExecProcNode: node type %d unsupported",
				 (int) nodeTag(node));
			result = NULL;
			break;
	}

	if (node->instrument)
		InstrStopNode(node->instrument, !TupIsNull(result));

	return result;
}

int
ExecCountSlotsNode(Plan *node)
{
	if (node == (Plan *) NULL)
		return 0;

	switch (nodeTag(node))
	{
			/*
			 * control nodes
			 */
		case T_Result:
			return ExecCountSlotsResult((Result *) node);

		case T_Append:
			return ExecCountSlotsAppend((Append *) node);

			/*
			 * scan nodes
			 */
		case T_SeqScan:
			return ExecCountSlotsSeqScan((SeqScan *) node);

		case T_IndexScan:
			return ExecCountSlotsIndexScan((IndexScan *) node);

		case T_TidScan:
			return ExecCountSlotsTidScan((TidScan *) node);

		case T_ScanQueue:
			return ExecCountSlotsScanQueue((ScanQueue *) node);

		case T_ScanSock:
			return ExecCountSlotsScanSock((ScanSock *) node);

		case T_StrmScan:
			return ExecCountSlotsStrmScan((StrmScan *) node);

		case T_SubqueryScan:
			return ExecCountSlotsSubqueryScan((SubqueryScan *) node);

		case T_FunctionScan:
			return ExecCountSlotsFunctionScan((FunctionScan *) node);

			/*
			 * join nodes
			 */
		case T_NestLoop:
			return ExecCountSlotsNestLoop((NestLoop *) node);

		case T_MergeJoin:
			return ExecCountSlotsMergeJoin((MergeJoin *) node);

		case T_Hash:
			return ExecCountSlotsHash((Hash *) node);

		case T_HashJoin:
			return ExecCountSlotsHashJoin((HashJoin *) node);

			/*
			 * materialization nodes
			 */
		case T_Material:
			return ExecCountSlotsMaterial((Material *) node);

		case T_Sort:
			return ExecCountSlotsSort((Sort *) node);

		case T_Unique:
			return ExecCountSlotsUnique((Unique *) node);

		case T_SetOp:
			return ExecCountSlotsSetOp((SetOp *) node);

		case T_Limit:
			return ExecCountSlotsLimit((Limit *) node);

		case T_Group:
			return ExecCountSlotsGroup((Group *) node);

			/* @BhashgrpMS */
		case T_HashGroup:
			return ExecCountSlotsHashGroup((HashGroup *) node);
			/* @EhashgrpMS */

			/* @BfluxMS */
		case T_FluxCons:
			return ExecCountSlotsFluxCons((FluxCons *) node);

		case T_FluxProd:
			return ExecCountSlotsFluxProd((FluxProd *) node);

		case T_FluxCons_FT:
			return ExecCountSlotsFluxCons_FT((FluxCons_FT *) node);

		case T_FluxProd_FT:
			return ExecCountSlotsFluxProd_FT((FluxProd_FT *) node);

		case T_SegCons:
			return ExecCountSlotsSegCons((SegCons *) node);

		case T_FluxEgress:
			return ExecCountSlotsFluxEgress((FluxEgress *) node);

		case T_FluxIngress:
			return ExecCountSlotsFluxIngress((FluxIngress *) node);

		case T_SegProd:
			return ExecCountSlotsSegProd((SegProd *) node);

		case T_DataGen:
			return ExecCountSlotsDataGen((DataGen *) node);

		case T_StateMover:
			return ExecCountSlotsStateMover((StateMover *) node);

		case T_FluxAcceptor:
			return ExecCountSlotsFluxAcceptor((FluxAcceptor *) node);
			/* @EfluxMS */

			/* @BwndaggWH */
			/*
			  case T_Window:
			return ExecCountSlotsWindow((Window *) node);
			*/
			/* @EwndaggWH */

		case T_Agg:
			return ExecCountSlotsAgg((Agg *) node);
	case T_FAgg:
			return ExecCountSlotsFAgg((FAgg *) node);

		case T_Eddy:			/* @BdeddySK */
			return ExecCountSlotsEddy((Eddy *) node);

		case T_FSched:			/* @dfluxMS */
			return ExecCountSlotsFSched((FSched *) node);

		case T_SteM:
			return ExecCountSlotsSteM((SteM *) node);

		case T_IndexSteM:
			return ExecCountSlotsIndexSteM((IndexSteM *) node);

		case T_SteMHashJoin:
			return ExecCountSlotsSteMHashJoin((SteMHashJoin *) node);

		case T_Filter:
			/* Error ! Filter nodes do not support iterator interface ! */
			elog(ERROR, "ExecCountSlotsNode unsupported for Filter(%d) !",
				 (int) nodeTag(node));
			break;				/* @BdeddySK */

		default:
			elog(ERROR, "ExecCountSlotsNode: node type %d unsupported",
				 (int) nodeTag(node));
			break;
	}
	return 0;
}

/* ----------------------------------------------------------------
 *		ExecEndNode
 *
 *		Recursively cleans up all the nodes in the plan rooted
 *		at 'node'.
 *
 *		After this operation, the query plan will not be able to
 *		processed any further.	This should be called only after
 *		the query plan has been fully executed.
 * ----------------------------------------------------------------
 */
void
ExecEndNode(Plan *node, Plan *parent)
{
	List	   *subp;

	/*
	 * do nothing when we get to the end of a leaf on tree.
	 */
	if (node == NULL)
		return;

	foreach(subp, node->initPlan)
		ExecEndSubPlan((SubPlan *) lfirst(subp));
	foreach(subp, node->subPlan)
		ExecEndSubPlan((SubPlan *) lfirst(subp));
	if (node->chgParam != NULL)
	{
		freeList(node->chgParam);
		node->chgParam = NULL;
	}

	switch (nodeTag(node))
	{
			/*
			 * control nodes
			 */
		case T_Result:
			ExecEndResult((Result *) node);
			break;

		case T_Append:
			ExecEndAppend((Append *) node);
			break;

			/*
			 * scan nodes
			 */
		case T_SeqScan:
			ExecEndSeqScan((SeqScan *) node);
			break;

		case T_IndexScan:
			ExecEndIndexScan((IndexScan *) node);
			break;

		case T_TidScan:
			ExecEndTidScan((TidScan *) node);
			break;

		case T_ScanQueue:
			ExecEndScanQueue((ScanQueue *) node);
			break;

		case T_ScanSock:
			ExecEndScanSock((ScanSock *) node);
			break;

		case T_StrmScan:
			ExecEndStrmScan((StrmScan *) node);
			break;

		case T_SubqueryScan:
			ExecEndSubqueryScan((SubqueryScan *) node);
			break;

		case T_FunctionScan:
			ExecEndFunctionScan((FunctionScan *) node);
			break;

			/*
			 * join nodes
			 */
		case T_NestLoop:
			ExecEndNestLoop((NestLoop *) node);
			break;

		case T_MergeJoin:
			ExecEndMergeJoin((MergeJoin *) node);
			break;

		case T_Hash:
			ExecEndHash((Hash *) node);
			break;

		case T_HashJoin:
			ExecEndHashJoin((HashJoin *) node);
			break;

			/*
			 * materialization nodes
			 */
		case T_Material:
			ExecEndMaterial((Material *) node);
			break;

		case T_Sort:
			ExecEndSort((Sort *) node);
			break;

		case T_Unique:
			ExecEndUnique((Unique *) node);
			break;

		case T_SetOp:
			ExecEndSetOp((SetOp *) node);
			break;

		case T_Limit:
			ExecEndLimit((Limit *) node);
			break;

		case T_Group:
			ExecEndGroup((Group *) node);
			break;

			/* @BwndaggWH */
			/*case T_Window:
			ExecEndWindow((Window *) node);
			break; */
			/* @EwndaggWH */

		case T_Agg:
			ExecEndAgg((Agg *) node);
			break;
		case T_FAgg:
			ExecEndFAgg((FAgg *) node);
			break;

			/* @BhashgrpMS */
		case T_HashGroup:
			ExecEndHashGroup((HashGroup *) node);
			break;
			/* @EhashgrpMS */

			/* @BfluxMS */
		case T_FluxCons:
			ExecEndFluxCons((FluxCons *) node);
			break;

		case T_FluxProd:
			ExecEndFluxProd((FluxProd *) node);
			break;

		case T_FluxCons_FT:
			ExecEndFluxCons_FT((FluxCons_FT *) node);
			break;

		case T_FluxProd_FT:
			ExecEndFluxProd_FT((FluxProd_FT *) node);
			break;

		case T_SegCons:
			ExecEndSegCons((SegCons *) node);
			break;

		case T_FluxEgress:
			ExecEndFluxEgress((FluxEgress *) node);
			break;

		case T_FluxIngress:
			ExecEndFluxIngress((FluxIngress *) node);
			break;

		case T_SegProd:
			ExecEndSegProd((SegProd *) node);
			break;

		case T_DataGen:
			ExecEndDataGen((DataGen *) node);
			break;

		case T_StateMover:
			ExecEndStateMover((StateMover *) node);
			break;

		case T_FluxAcceptor:
			ExecEndFluxAcceptor((FluxAcceptor *) node);
			break;
			/* @EfluxMS */

		case T_Eddy:			/* @BdeddySK */
			ExecEndEddy((Eddy *) node);
			break;

		case T_FSched:			/* @dfluxMS */
			ExecEndFSched((FSched *) node);
			break;

		case T_Filter:
			/* Error ! Filter nodes do not support iterator interface ! */
			elog(ERROR, "ExecEndNode unsupported for Filter !");		/* @EdeddySK */

		case T_SteM:
			ExecEndSteM((SteM *) node);
			break;

		case T_IndexSteM:
			ExecEndIndexSteM((IndexSteM *) node);
			break;

		case T_SteMHashJoin:
			ExecEndSteMHashJoin((SteMHashJoin *) node);
			break;

		default:
			elog(ERROR, "ExecEndNode: node type %d unsupported",
				 (int) nodeTag(node));
			break;
	}

	if (node->instrument)
		InstrEndLoop(node->instrument);
}

/* @BdtcqWH */
/* this functions returns the tuple slot to be returned by this plan */
/* it must be called after the plan has been initialized, i.e., InitPlan */
/* has been called. */
TupleTableSlot *
ExecGetTupSlot(Plan *node)
{
	TupleTableSlot *slot = NULL;

	if (node == NULL)
		return NULL;

	switch (nodeTag(node))
	{
		case T_Result:
			{
				ResultState *resstate = ((Result *) node)->resstate;

				slot = resstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_SeqScan:
			{
				CommonScanState *scanstate = ((SeqScan *) node)->scanstate;

				slot = scanstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_NestLoop:
			{
				NestLoopState *nlstate = ((NestLoop *) node)->nlstate;

				slot = nlstate->jstate.cs_ResultTupleSlot;
			}
			break;

		case T_Append:
			{
				AppendState *appendstate = ((Append *) node)->appendstate;

				slot = appendstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_IndexScan:
			{
				CommonScanState *scanstate = ((IndexScan *) node)->scan.scanstate;

				slot = scanstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_TidScan:
			{
				CommonScanState *scanstate = ((TidScan *) node)->scan.scanstate;

				slot = scanstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_ScanQueue:
			{
				ScanQueueState *scanqstate = ((ScanQueue *) node)->scanqstate;

				slot = scanqstate->cstate.cs_ResultTupleSlot;
			}
			break;

        case T_ScanSock:
			{
				ScanSockState *scansockstate = 
                    ((ScanSock *)node)->scansockstate;

				slot = scansockstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_StrmScan:
			{

				StrmScan   *ss = (StrmScan *) node;
				ScanSockState *scansockstate = ss->scansock.scansockstate;
				CommonScanState *scanstate = 
				  GetScanStateStrmScan((StrmScan*)ss);

				if (IsHeapAccessedStream(ss->streamtype)) {
				  slot = scanstate->cstate.cs_ResultTupleSlot;
				} else if (IsQueueAccessedStream(ss->streamtype)) {
				  slot = scansockstate->cstate.cs_ResultTupleSlot;
				}
				else {
					elog(ERROR, "strmscan contains an invalid stream type attribute");
				}
				
			}
			break;

		case T_SubqueryScan:
			{
				CommonScanState *scanstate = ((SubqueryScan *) node)->scan.scanstate;

				slot = scanstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_FunctionScan:
			{
				CommonScanState *scanstate = ((FunctionScan *) node)->scan.scanstate;

				slot = scanstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_Material:
			{
				MaterialState *matstate = ((Material *) node)->matstate;

				slot = matstate->csstate.css_ScanTupleSlot;
			}
			break;

		case T_Sort:
			{
				SortState  *sortstate = ((Sort *) node)->sortstate;

				slot = sortstate->csstate.css_ScanTupleSlot;
			}
			break;

		case T_Agg:
			{
				AggState   *aggstate = ((Agg *) node)->aggstate;

				slot = aggstate->csstate.cstate.cs_ResultTupleSlot;
			}
			break;

	case T_FAgg:
	  {
	    slot = ((FAgg*)node)->fjord.scanstate->cstate.cs_ResultTupleSlot;
	    break;

	  }
		case T_Group:
			{
				GroupState *grpstate = ((Group *) node)->grpstate;

				slot = grpstate->csstate.cstate.cs_ResultTupleSlot;
			}
			break;

			/* @BhashgrpMS */
		case T_HashGroup:
			{
				AggState   *aggstate = ((Agg *) node)->aggstate;

				slot = aggstate->csstate.cstate.cs_ResultTupleSlot;
			}
			break;
			/* @EhashgrpMS */

			/* @BfluxMS */
		case T_FluxCons:
				slot = ((FluxCons *) node)->fcstate->csstate.cstate.cs_ResultTupleSlot;
			break;
			/* TBD TBD : Mehul - check this case T_FluxProd: */
		case T_FluxProd:
				slot = ((FluxProd *) node)->fpstate->csstate.css_ScanTupleSlot;
			break;

		case T_FluxCons_FT:
				slot = ((FluxCons_FT *) node)->fcstate_ft->csstate.cstate.cs_ResultTupleSlot;
			break;

		case T_FluxProd_FT:
				slot = ((FluxProd_FT *) node)->fpstate_ft->csstate.css_ScanTupleSlot;
			break;

		case T_SegCons:
				slot = ((SegCons *) node)->fcons.fcstate_ft->csstate.cstate.cs_ResultTupleSlot;
			break;

		case T_FluxEgress:
			    slot = ((FluxEgress *) node)->fcons.fcstate_ft->csstate.cstate.cs_ResultTupleSlot;
			break;

		case T_FluxIngress:
				slot = ((FluxIngress *) node)->fprod.fpstate_ft->csstate.cstate.cs_ResultTupleSlot;
			break;

		case T_SegProd:
			{
				slot = ((FluxProd_FT *) node)->fpstate_ft->csstate.css_ScanTupleSlot;
			}
			break;

		case T_DataGen:
				slot = ((DataGen *) node)->cstate->cs_ResultTupleSlot;
			break;

		case T_StateMover:
				slot = NULL;

		case T_FluxAcceptor:
				slot = NULL;
			break;
			/* @EfluxMS */

			/* @BwndaggWH */
			/*
		case T_Window:
			{
				WindowState *wndstate = ((Window *) node)->wndstate;

				slot = wndstate->csstate.cstate.cs_ResultTupleSlot;
			}
			break;
			*/
			/* @EwndaggWH */

		case T_Hash:
			{
				HashState  *hashstate = ((Hash *) node)->hashstate;

				slot = hashstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_Unique:
			{
				UniqueState *uniquestate = ((Unique *) node)->uniquestate;

				slot = uniquestate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_SetOp:
			{
				SetOpState *setopstate = ((SetOp *) node)->setopstate;

				slot = setopstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_Limit:
			{
				LimitState *limitstate = ((Limit *) node)->limitstate;

				slot = limitstate->cstate.cs_ResultTupleSlot;
			}
			break;

		case T_MergeJoin:
			{
				MergeJoinState *mergestate = ((MergeJoin *) node)->mergestate;

				slot = mergestate->jstate.cs_ResultTupleSlot;
			}
			break;

		case T_HashJoin:
			{
				HashJoinState *hashjoinstate = ((HashJoin *) node)->hashjoinstate;

				slot = hashjoinstate->jstate.cs_ResultTupleSlot;
			}
			break;

		case T_Eddy:			/* @BdeddySK */
			{
				EddyState  *eddystate = ((Eddy *) node)->eddystate;

				slot = eddystate->jstate.cs_ResultTupleSlot;
			}
			break;				/* @EdeddySK */

		case T_SteMHashJoin:
			{
				HashJoinState *hashjoinstate = ((SteMHashJoin *) node)->hashjoinstate;

				slot = hashjoinstate->jstate.cs_ResultTupleSlot;
			}
			break;

		case T_FSched:
			slot = ExecGetTupSlot(((FSched *) node)->topSeg);
			break;

		default:

			/*
			 * should never get here
			 */
			elog(ERROR, "ExecGetTupSlot: node type %d unsupported",
				 (int) nodeTag(node));
			return NULL;
	}
	return slot;
}

/* @EdtcqWH */

/* ----------------------------------------------------------------
 *		ExecGetTupType
 *
 *		this gives you the tuple descriptor for tuples returned
 *		by this node.  I really wish I could ditch this routine,
 *		but since not all nodes store their type info in the same
 *		place, we have to do something special for each node type.
 *
 * ----------------------------------------------------------------
 */
TupleDesc
ExecGetTupType(Plan *node)
{

	TupleTableSlot *slot = ExecGetTupSlot(node);

	return slot->ttc_tupleDescriptor;
}
