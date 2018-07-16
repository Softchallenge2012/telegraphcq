/*-------------------------------------------------------------------------
 *
 * execMain.c
 *	  top level executor interface routines
 *
 * INTERFACE ROUTINES
 *	ExecutorStart()
 *	ExecutorRun()
 *	ExecutorEnd()
 *
 *	The old ExecutorMain() has been replaced by ExecutorStart(),
 *	ExecutorRun() and ExecutorEnd()
 *
 *	These three procedures are the external interfaces to the executor.
 *	In each case, the query descriptor and the execution state is required
 *	 as arguments
 *
 *	ExecutorStart() must be called at the beginning of any execution of any
 *	query plan and ExecutorEnd() should always be called at the end of
 *	execution of a plan.
 *
 *	ExecutorRun accepts direction and count arguments that specify whether
 *	the plan is to be executed forwards, backwards, and for how many tuples.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/execMain.c,v 1.20 2006/02/06 01:19:46 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/catalog.h"
#include "catalog/heap.h"
#include "catalog/namespace.h"
#include "commands/tablecmds.h"
#include "commands/trigger.h"
#include "executor/execdebug.h"
#include "executor/execdefs.h"
#include "miscadmin.h"
#include "optimizer/var.h"
#include "parser/parsetree.h"
#include "rqueue/rqdest.h"
#include "utils/acl.h"
#include "utils/debug.h"
#include "utils/lsyscache.h"

#include "nodes/relation.h"
#include "catalog/pg_type.h"

/* decls for local routines only used within this module */
static TupleDesc InitPlan(CmdType operation,
		 Query *parseTree,
		 Plan *plan,
		 EState *estate);
static void initResultRelInfo(ResultRelInfo *resultRelInfo,
				  Index resultRelationIndex,
				  List *rangeTable,
				  CmdType operation);
static void EndPlan(Plan *plan, EState *estate);
static TupleTableSlot *ExecutePlan(EState *estate, Plan *plan,
			CmdType operation,
			long numberTuples,
			ScanDirection direction,
			DestReceiver *destfunc);
static void ExecSelect(TupleTableSlot *slot,
		   DestReceiver *destfunc,
		   EState *estate);
static void ExecInsert(TupleTableSlot *slot, ItemPointer tupleid,
		   EState *estate);
static void ExecDelete(TupleTableSlot *slot, ItemPointer tupleid,
		   EState *estate);
static void ExecUpdate(TupleTableSlot *slot, ItemPointer tupleid,
		   EState *estate);
static TupleTableSlot *EvalPlanQualNext(EState *estate);
static void EndEvalPlanQual(EState *estate);
static void ExecCheckQueryPerms(CmdType operation, Query *parseTree,
					Plan *plan);
static void ExecCheckPlanPerms(Plan *plan, List *rangeTable,
				   CmdType operation);
static void ExecCheckRTPerms(List *rangeTable, CmdType operation);
static void ExecCheckRTEPerms(RangeTblEntry *rte, CmdType operation);

static void SetupOutputBufferState(Query *parseTree, OutputBufferState *outputState);
static void ScrollUntil(QueryDesc *queryDesc, EState *estate, Datum time);
static void SendNextRstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count);
static void SendNextIstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count);
static void SendNextDstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count);
static void SendStreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count);
/* end of local decls */

/* Whether or not to record output times in a file */
/* #define RECORD_OUTPUT_TIMES */


/* ----------------------------------------------------------------
 *		ExecutorStart
 *
 *		This routine must be called at the beginning of any execution of any
 *		query plan
 *
 *		returns a TupleDesc which describes the attributes of the tuples to
 *		be returned by the query.  (Same value is saved in queryDesc)
 *
 * NB: the CurrentMemoryContext when this is called must be the context
 * to be used as the per-query context for the query plan.	ExecutorRun()
 * and ExecutorEnd() must be called in this same memory context.
 * ----------------------------------------------------------------
 */
TupleDesc
ExecutorStart(QueryDesc *queryDesc, EState *estate)
{
	TupleDesc	result;

	/* sanity checks */
	Assert(queryDesc != NULL);

	if (queryDesc->plantree->nParamExec > 0)
	{
		estate->es_param_exec_vals = (ParamExecData *)
			palloc(queryDesc->plantree->nParamExec * sizeof(ParamExecData));
		MemSet(estate->es_param_exec_vals, 0,
			   queryDesc->plantree->nParamExec * sizeof(ParamExecData));
	}

	/*
	 * Make our own private copy of the current query snapshot data.
	 *
	 * This "freezes" our idea of which tuples are good and which are not for
	 * the life of this query, even if it outlives the current command and
	 * current snapshot.
	 */
	estate->es_snapshot = CopyQuerySnapshot();

	/*
	 * Initialize the plan
	 */
	result = InitPlan(queryDesc->operation,
					  queryDesc->parsetree,
					  queryDesc->plantree,
					  estate);

	queryDesc->tupDesc = result;

	return result;
}

/* ----------------------------------------------------------------
 *		ExecutorRun
 *
 *		This is the main routine of the executor module. It accepts
 *		the query descriptor from the traffic cop and executes the
 *		query plan.
 *
 *		ExecutorStart must have been called already.
 *
 *		If direction is NoMovementScanDirection then nothing is done
 *		except to start up/shut down the destination.  Otherwise,
 *		we retrieve up to 'count' tuples in the specified direction.
 *
 *		Note: count = 0 is interpreted as no portal limit, e.g. run to
 *		completion.
 *
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecutorRun(QueryDesc *queryDesc, EState *estate,
			ScanDirection direction, long count)
{
	CmdType		operation;
	Plan	   *plan;
	CommandDest dest;
	DestReceiver *destfunc;
	TupleTableSlot *result;

	/*
	 * sanity checks
	 */
	Assert(queryDesc != NULL);

	/*
	 * extract information from the query descriptor and the query
	 * feature.
	 */
	operation = queryDesc->operation;
	plan = queryDesc->plantree;
	dest = queryDesc->dest;

	/*
	 * startup tuple receiver
	 */
	estate->es_processed = 0;
	estate->es_lastoid = InvalidOid;

	destfunc = DestToFunction(dest);
	(*destfunc->setup) (destfunc, (int) operation,
						queryDesc->portalName, queryDesc->tupDesc);

	/*
	 * run plan
	 */
	if (direction == NoMovementScanDirection)
		result = NULL;
	else
		result = ExecutePlan(estate,
							 plan,
							 operation,
							 count,
							 direction,
							 destfunc);

	/*
	 * shutdown receiver
	 */
	(*destfunc->cleanup) (destfunc);

	return result;
}

/* ----------------------------------------------------------------
 *		ExecutorEnd
 *
 *		This routine must be called at the end of execution of any
 *		query plan
 * ----------------------------------------------------------------
 */
void
ExecutorEnd(QueryDesc *queryDesc, EState *estate)
{
	/* sanity checks */
	Assert(queryDesc != NULL);

	EndPlan(queryDesc->plantree, estate);

	if (estate->es_snapshot != NULL)
	{
		if (estate->es_snapshot->xcnt > 0)
			pfree(estate->es_snapshot->xip);
		pfree(estate->es_snapshot);
		estate->es_snapshot = NULL;
	}

	if (estate->es_param_exec_vals != NULL)
	{
		pfree(estate->es_param_exec_vals);
		estate->es_param_exec_vals = NULL;
	}
}


/*
 * ExecCheckQueryPerms
 *		Check access permissions for all relations referenced in a query.
 */
static void
ExecCheckQueryPerms(CmdType operation, Query *parseTree, Plan *plan)
{
	/*
	 * Check RTEs in the query's primary rangetable.
	 */
	ExecCheckRTPerms(parseTree->rtable, operation);

	/*
	 * Search for subplans and APPEND nodes to check their rangetables.
	 */
	ExecCheckPlanPerms(plan, parseTree->rtable, operation);
}

/*
 * ExecCheckPlanPerms
 *		Recursively scan the plan tree to check access permissions in
 *		subplans.
 */
static void
ExecCheckPlanPerms(Plan *plan, List *rangeTable, CmdType operation)
{
	List	   *subp;

	if (plan == NULL)
		return;

	/* Check subplans, which we assume are plain SELECT queries */

	foreach(subp, plan->initPlan)
	{
		SubPlan    *subplan = (SubPlan *) lfirst(subp);

		ExecCheckRTPerms(subplan->rtable, CMD_SELECT);
		ExecCheckPlanPerms(subplan->plan, subplan->rtable, CMD_SELECT);
	}
	foreach(subp, plan->subPlan)
	{
		SubPlan    *subplan = (SubPlan *) lfirst(subp);

		ExecCheckRTPerms(subplan->rtable, CMD_SELECT);
		ExecCheckPlanPerms(subplan->plan, subplan->rtable, CMD_SELECT);
	}

	/* Check lower plan nodes */

	ExecCheckPlanPerms(plan->lefttree, rangeTable, operation);
	ExecCheckPlanPerms(plan->righttree, rangeTable, operation);

	/* Do node-type-specific checks */

	switch (nodeTag(plan))
	{
		case T_SubqueryScan:
			{
				SubqueryScan *scan = (SubqueryScan *) plan;
				RangeTblEntry *rte;

				/* Recursively check the subquery */
				rte = rt_fetch(scan->scan.scanrelid, rangeTable);
				Assert(rte->rtekind == RTE_SUBQUERY);
				ExecCheckQueryPerms(operation, rte->subquery, scan->subplan);
				break;
			}
		case T_Append:
			{
				Append	   *app = (Append *) plan;
				List	   *appendplans;

				foreach(appendplans, app->appendplans)
				{
					ExecCheckPlanPerms((Plan *) lfirst(appendplans),
									   rangeTable,
									   operation);
				}
				break;
			}

		default:
			break;
	}
}

/*
 * ExecCheckRTPerms
 *		Check access permissions for all relations listed in a range table.
 */
static void
ExecCheckRTPerms(List *rangeTable, CmdType operation)
{
	List	   *lp;

	foreach(lp, rangeTable)
	{
		RangeTblEntry *rte = lfirst(lp);

		ExecCheckRTEPerms(rte, operation);
	}
}

/*
 * ExecCheckRTEPerms
 *		Check access permissions for a single RTE.
 */
static void
ExecCheckRTEPerms(RangeTblEntry *rte, CmdType operation)
{
	Oid			relOid;
	Oid			userid;
	AclResult	aclcheck_result;

	/*
	 * Only plain-relation RTEs need to be checked here.  Subquery RTEs
	 * will be checked when ExecCheckPlanPerms finds the SubqueryScan
	 * node, and function RTEs are checked by init_fcache when the
	 * function is prepared for execution.	Join and special RTEs need no
	 * checks.
	 */
	if (rte->rtekind != RTE_RELATION)
		return;

	relOid = rte->relid;

	/*
	 * userid to check as: current user unless we have a setuid
	 * indication.
	 *
	 * Note: GetUserId() is presently fast enough that there's no harm in
	 * calling it separately for each RTE.	If that stops being true, we
	 * could call it once in ExecCheckQueryPerms and pass the userid down
	 * from there.	But for now, no need for the extra clutter.
	 */
	userid = rte->checkAsUser ? rte->checkAsUser : GetUserId();

#define CHECK(MODE)		pg_class_aclcheck(relOid, userid, MODE)

	if (rte->checkForRead)
	{
		aclcheck_result = CHECK(ACL_SELECT);
		if (aclcheck_result != ACLCHECK_OK)
			aclcheck_error(aclcheck_result, get_rel_name(relOid));
	}

	if (rte->checkForWrite)
	{
		/*
		 * Note: write access in a SELECT context means SELECT FOR UPDATE.
		 * Right now we don't distinguish that from true update as far as
		 * permissions checks are concerned.
		 */
		switch (operation)
		{
			case CMD_INSERT:
				aclcheck_result = CHECK(ACL_INSERT);
				break;
			case CMD_SELECT:
			case CMD_UPDATE:
				aclcheck_result = CHECK(ACL_UPDATE);
				break;
			case CMD_DELETE:
				aclcheck_result = CHECK(ACL_DELETE);
				break;
			default:
				elog(ERROR, "ExecCheckRTEPerms: bogus operation %d",
					 operation);
				aclcheck_result = ACLCHECK_OK;	/* keep compiler quiet */
				break;
		}
		if (aclcheck_result != ACLCHECK_OK)
			aclcheck_error(aclcheck_result, get_rel_name(relOid));
	}
}


/* ===============================================================
 * ===============================================================
						 static routines follow
 * ===============================================================
 * ===============================================================
 */

typedef struct execRowMark
{
	Relation	relation;
	Index		rti;
	char		resname[32];
} execRowMark;

typedef struct evalPlanQual
{
	Plan	   *plan;
	Index		rti;
	EState		estate;
	struct evalPlanQual *free;
} evalPlanQual;

/* ----------------------------------------------------------------
 *		InitPlan
 *
 *		Initializes the query plan: open files, allocate storage
 *		and start up the rule manager
 * ----------------------------------------------------------------
 */
static TupleDesc
InitPlan(CmdType operation, Query *parseTree, Plan *plan, EState *estate)
{
	List	   *rangeTable;
	Relation	intoRelationDesc;
	bool		do_select_into;
	TupleDesc	tupType;
	
	/*
	 * Do permissions checks.
	 */
	ExecCheckQueryPerms(operation, parseTree, plan);

	/*
	 * get information from query descriptor
	 */
	rangeTable = parseTree->rtable;

	/*
	 * initialize the node's execution state
	 */
	estate->es_range_table = rangeTable;

	/* If we're doing r/i/dstream select, then initialize the
	 * EState's OutputBufferState
	 */
	if (parseTree->isPortal && operation == CMD_SELECT &&
	    parseTree->cursorType != CURSORKIND_UNKNOWN)
	{
	  if (parseTree->windowClause == NULL)
	  {
	    elog(ERROR, "Cannot extract %s from a non-windowed query",
		 StringOfCursorType(parseTree->cursorType));
	  }

	  SetupOutputBufferState(parseTree, estate->outputBufferState);
	}

	/*
	 * if there is a result relation, initialize result relation stuff
	 */
	if (parseTree->resultRelation != 0 && operation != CMD_SELECT)
	{
		List	   *resultRelations = parseTree->resultRelations;
		int			numResultRelations;
		ResultRelInfo *resultRelInfos;

		if (resultRelations != NIL)
		{
			/*
			 * Multiple result relations (due to inheritance)
			 * parseTree->resultRelations identifies them all
			 */
			ResultRelInfo *resultRelInfo;

			numResultRelations = length(resultRelations);
			resultRelInfos = (ResultRelInfo *)
				palloc(numResultRelations * sizeof(ResultRelInfo));
			resultRelInfo = resultRelInfos;
			while (resultRelations != NIL)
			{
				initResultRelInfo(resultRelInfo,
								  lfirsti(resultRelations),
								  rangeTable,
								  operation);
				resultRelInfo++;
				resultRelations = lnext(resultRelations);
			}
		}
		else
		{
			/*
			 * Single result relation identified by
			 * parseTree->resultRelation
			 */
			numResultRelations = 1;
			resultRelInfos = (ResultRelInfo *) palloc(sizeof(ResultRelInfo));
			initResultRelInfo(resultRelInfos,
							  parseTree->resultRelation,
							  rangeTable,
							  operation);
		}

		estate->es_result_relations = resultRelInfos;
		estate->es_num_result_relations = numResultRelations;
		/* Initialize to first or only result rel */
		estate->es_result_relation_info = resultRelInfos;
	}
	else
	{
		/*
		 * if no result relation, then set state appropriately
		 */
		estate->es_result_relations = NULL;
		estate->es_num_result_relations = 0;
		estate->es_result_relation_info = NULL;
	}

	/*
	 * Detect whether we're doing SELECT INTO.  If so, set the force_oids
	 * flag appropriately so that the plan tree will be initialized with
	 * the correct tuple descriptors.
	 */
	do_select_into = false;

	if (operation == CMD_SELECT &&
		!parseTree->isPortal &&
		parseTree->into != NULL)
	{
		do_select_into = true;

		/*
		 * For now, always create OIDs in SELECT INTO; this is for
		 * backwards compatibility with pre-7.3 behavior.  Eventually we
		 * might want to allow the user to choose.
		 */
		estate->es_force_oids = true;
	}

	/*
	 * Have to lock relations selected for update
	 */
	estate->es_rowMark = NIL;
	if (parseTree->rowMarks != NIL)
	{
		List	   *l;

		foreach(l, parseTree->rowMarks)
		{
			Index		rti = lfirsti(l);
			Oid			relid = getrelid(rti, rangeTable);
			Relation	relation;
			execRowMark *erm;

			relation = heap_open(relid, RowShareLock);
			erm = (execRowMark *) palloc(sizeof(execRowMark));
			erm->relation = relation;
			erm->rti = rti;
			snprintf(erm->resname, 32, "ctid%u", rti);
			estate->es_rowMark = lappend(estate->es_rowMark, erm);
		}
	}

	/*
	 * initialize the executor "tuple" table.  We need slots for all the
	 * plan nodes, plus possibly output slots for the junkfilter(s). At
	 * this point we aren't sure if we need junkfilters, so just add slots
	 * for them unconditionally.
	 */
	{

		int			nSlots = ExecCountSlotsNode(plan);

		if (parseTree->resultRelations != NIL)
			nSlots += length(parseTree->resultRelations);
		else
			nSlots += 1;
		estate->es_tupleTable = ExecCreateTupleTable(nSlots);
	}


	/* mark EvalPlanQual not active */
	estate->es_origPlan = plan;
	estate->es_evalPlanQual = NULL;
	estate->es_evTuple = NULL;
	estate->es_evTupleNull = NULL;
	estate->es_useEvalPlan = false;

	/*
	 * initialize the private state information for all the nodes in the
	 * query tree.	This opens files, allocates storage and leaves us
	 * ready to start processing tuples.
	 */
	ExecInitNode(plan, estate, NULL);

	/*
	 * Get the tuple descriptor describing the type of tuples to return.
	 * (this is especially important if we are creating a relation with
	 * "SELECT INTO")
	 */
	tupType = ExecGetTupType(plan);		/* tuple descriptor */

	/*
	 * Initialize the junk filter if needed. SELECT and INSERT queries
	 * need a filter if there are any junk attrs in the tlist.	UPDATE and
	 * DELETE always need one, since there's always a junk 'ctid'
	 * attribute present --- no need to look first.
	 */
	{
		bool		junk_filter_needed = false;
		List	   *tlist;

		switch (operation)
		{
			case CMD_SELECT:
			case CMD_INSERT:
				foreach(tlist, plan->targetlist)
				{
					TargetEntry *tle = (TargetEntry *) lfirst(tlist);

					if (tle->resdom->resjunk)
					{
						junk_filter_needed = true;
						break;
					}
				}
				break;
			case CMD_UPDATE:
			case CMD_DELETE:
				junk_filter_needed = true;
				break;
			default:
				break;
		}

		if (junk_filter_needed)
		{
			/*
			 * If there are multiple result relations, each one needs its
			 * own junk filter.  Note this is only possible for
			 * UPDATE/DELETE, so we can't be fooled by some needing a
			 * filter and some not.
			 */
			if (parseTree->resultRelations != NIL)
			{
				List	   *subplans;
				ResultRelInfo *resultRelInfo;

				/* Top plan had better be an Append here. */
				Assert(IsA(plan, Append));
				Assert(((Append *) plan)->isTarget);
				subplans = ((Append *) plan)->appendplans;
				Assert(length(subplans) == estate->es_num_result_relations);
				resultRelInfo = estate->es_result_relations;
				while (subplans != NIL)
				{
					Plan	   *subplan = (Plan *) lfirst(subplans);
					JunkFilter *j;

					j = ExecInitJunkFilter(subplan->targetlist,
										   ExecGetTupType(subplan),
							  ExecAllocTableSlot(estate->es_tupleTable));
					resultRelInfo->ri_junkFilter = j;
					resultRelInfo++;
					subplans = lnext(subplans);
				}

				/*
				 * Set active junkfilter too; at this point ExecInitAppend
				 * has already selected an active result relation...
				 */
				estate->es_junkFilter =
					estate->es_result_relation_info->ri_junkFilter;
			}
			else
			{
				/* Normal case with just one JunkFilter */
				JunkFilter *j;

				j = ExecInitJunkFilter(plan->targetlist,
									   tupType,
							  ExecAllocTableSlot(estate->es_tupleTable));
				estate->es_junkFilter = j;
				if (estate->es_result_relation_info)
					estate->es_result_relation_info->ri_junkFilter = j;

				/* For SELECT, want to return the cleaned tuple type */
				if (operation == CMD_SELECT)
					tupType = j->jf_cleanTupType;
			}
		}
		else
			estate->es_junkFilter = NULL;
	}

	/*
	 * initialize the "into" relation
	 */
	intoRelationDesc = (Relation) NULL;

	if (do_select_into)
	{
		char	   *intoName;
		Oid			namespaceId;
		AclResult	aclresult;
		Oid			intoRelationId;
		TupleDesc	tupdesc;

		/*
		 * find namespace to create in, check permissions
		 */
		intoName = parseTree->into->relname;

		namespaceId = RangeVarGetCreationNamespace(parseTree->into);

		aclresult = pg_namespace_aclcheck(namespaceId, GetUserId(),
										  ACL_CREATE);
		if (aclresult != ACLCHECK_OK)
			aclcheck_error(aclresult,
						   get_namespace_name(namespaceId));

		/*
		 * have to copy tupType to get rid of constraints
		 */
		tupdesc = CreateTupleDescCopy(tupType);

		intoRelationId =
			heap_create_with_catalog(intoName,
									 namespaceId,
									 tupdesc,
									 RELKIND_RELATION,
									 false,
									 allowSystemTableMods);

		FreeTupleDesc(tupdesc);

		/*
		 * Advance command counter so that the newly-created relation's
		 * catalog tuples will be visible to heap_open.
		 */
		CommandCounterIncrement();

		/*
		 * If necessary, create a TOAST table for the into relation. Note
		 * that AlterTableCreateToastTable ends with
		 * CommandCounterIncrement(), so that the TOAST table will be
		 * visible for insertion.
		 */
		AlterTableCreateToastTable(intoRelationId, true);

		intoRelationDesc = heap_open(intoRelationId,
									 AccessExclusiveLock);
	}

	estate->es_into_relation_descriptor = intoRelationDesc;

	return tupType;
}

/*
 * Initialize ResultRelInfo data for one result relation
 */
static void
initResultRelInfo(ResultRelInfo *resultRelInfo,
				  Index resultRelationIndex,
				  List *rangeTable,
				  CmdType operation)
{
	Oid			resultRelationOid;
	Relation	resultRelationDesc;

	resultRelationOid = getrelid(resultRelationIndex, rangeTable);
	resultRelationDesc = heap_open(resultRelationOid, RowExclusiveLock);

	switch (resultRelationDesc->rd_rel->relkind)
	{
		case RELKIND_SEQUENCE:
			elog(ERROR, "You can't change sequence relation %s",
				 RelationGetRelationName(resultRelationDesc));
			break;
		case RELKIND_TOASTVALUE:
			elog(ERROR, "You can't change toast relation %s",
				 RelationGetRelationName(resultRelationDesc));
			break;
		case RELKIND_VIEW:
			elog(ERROR, "You can't change view relation %s",
				 RelationGetRelationName(resultRelationDesc));
			break;
	}

	MemSet(resultRelInfo, 0, sizeof(ResultRelInfo));
	resultRelInfo->type = T_ResultRelInfo;
	resultRelInfo->ri_RangeTableIndex = resultRelationIndex;
	resultRelInfo->ri_RelationDesc = resultRelationDesc;
	resultRelInfo->ri_NumIndices = 0;
	resultRelInfo->ri_IndexRelationDescs = NULL;
	resultRelInfo->ri_IndexRelationInfo = NULL;
	/* make a copy so as not to depend on relcache info not changing... */
	resultRelInfo->ri_TrigDesc = CopyTriggerDesc(resultRelationDesc->trigdesc);
	resultRelInfo->ri_TrigFunctions = NULL;
	resultRelInfo->ri_ConstraintExprs = NULL;
	resultRelInfo->ri_junkFilter = NULL;

	/*
	 * If there are indices on the result relation, open them and save
	 * descriptors in the result relation info, so that we can add new
	 * index entries for the tuples we add/update.	We need not do this
	 * for a DELETE, however, since deletion doesn't affect indexes.
	 */
	if (resultRelationDesc->rd_rel->relhasindex &&
		operation != CMD_DELETE)
		ExecOpenIndices(resultRelInfo);
}

/* ----------------------------------------------------------------
 *		EndPlan
 *
 *		Cleans up the query plan -- closes files and free up storages
 * ----------------------------------------------------------------
 */
static void
EndPlan(Plan *plan, EState *estate)
{
	ResultRelInfo *resultRelInfo;
	int			i;
	List	   *l;

	/*
	 * shut down any PlanQual processing we were doing
	 */
	if (estate->es_evalPlanQual != NULL)
		EndEvalPlanQual(estate);

	/*
	 * shut down the node-type-specific query processing
	 */
	ExecEndNode(plan, NULL);

	/*
	 * destroy the executor "tuple" table.
	 */
	ExecDropTupleTable(estate->es_tupleTable, true);
	estate->es_tupleTable = NULL;

	/*
	 * close the result relation(s) if any, but hold locks until xact
	 * commit.	Also clean up junkfilters if present.
	 */
	resultRelInfo = estate->es_result_relations;
	for (i = estate->es_num_result_relations; i > 0; i--)
	{
		/* Close indices and then the relation itself */
		ExecCloseIndices(resultRelInfo);
		heap_close(resultRelInfo->ri_RelationDesc, NoLock);
		/* Delete the junkfilter if any */
		if (resultRelInfo->ri_junkFilter != NULL)
			ExecFreeJunkFilter(resultRelInfo->ri_junkFilter);
		resultRelInfo++;
	}

	/*
	 * close the "into" relation if necessary, again keeping lock
	 */
	if (estate->es_into_relation_descriptor != NULL)
		heap_close(estate->es_into_relation_descriptor, NoLock);

	/*
	 * There might be a junkfilter without a result relation.
	 */
	if (estate->es_num_result_relations == 0 &&
		estate->es_junkFilter != NULL)
	{
		ExecFreeJunkFilter(estate->es_junkFilter);
		estate->es_junkFilter = NULL;
	}

	/*
	 * close any relations selected FOR UPDATE, again keeping locks
	 */
	foreach(l, estate->es_rowMark)
	{
		execRowMark *erm = lfirst(l);

		heap_close(erm->relation, NoLock);
	}

	/* Clean up the OutputBufferState -- remove the temporary output
	 * buffer relation, close the heap scan, etc. -- if one was used.
	 */
	if (estate->outputBufferState != NULL)
	{
	  OutputBufferState *outputState = estate->outputBufferState;

	  /* Shut down the heap scan */
	  if (outputState->outputScanner != NULL)
	  {
	    heap_endscan(outputState->outputScanner);
	  }

	  /* Close the relation */
	  if (outputState->outputBuffer != NULL)
	  {
	    relation_close(outputState->outputBuffer, AccessExclusiveLock);
	  }

	  /* Remove the buffer relation */
	  if (outputState->outputRelid != 0)
	  {
	    RemoveRelationWithOid(outputState->outputRelid, DROP_CASCADE);
	  }
	  //	  pfree(outputState);
	  //outputState = NULL;
	}
}

/* ----------------------------------------------------------------
 *		ExecutePlan
 *
 *		processes the query plan to retrieve 'numberTuples' tuples in the
 *		direction specified.
 *		Retrieves all tuples if numberTuples is 0
 *
 *		result is either a slot containing the last tuple in the case
 *		of a SELECT or NULL otherwise.
 *
 * Note: the ctid attribute is a 'junk' attribute that is removed before the
 * user can see it
 * ----------------------------------------------------------------
 */
static TupleTableSlot *
ExecutePlan(EState *estate,
			Plan *plan,
			CmdType operation,
			long numberTuples,
			ScanDirection direction,
			DestReceiver *destfunc)
{
	JunkFilter *junkfilter;
	TupleTableSlot *slot;
	ItemPointer tupleid = NULL;
	ItemPointerData tuple_ctid;
	long		current_tuple_count;
	TupleTableSlot *result;

    bool window_mode = (WindowScanDirection == direction);
        /* Are we scanning an entire TCQ time window at a time? */

    if (window_mode) {
        /* The count is currently ignored in window mode. */
        numberTuples = 0;
    }

	/*
	 * initialize local variables
	 */
	slot = NULL;
	current_tuple_count = 0;
	result = NULL;

	/*
	 * Set the direction.
	 */
    if (window_mode) {
        /* The rest of PostgreSQL doesn't know about the WindowScanDirection
         * scan direction.  Let it bask in its ignorance. */
        estate->es_direction = ForwardScanDirection;
    } else {
        estate->es_direction = direction;
    }

#ifdef RECORD_OUTPUT_TIMES

	/*
	 * If desired, initialize the output time recording.
	 */
	init_recording_output_times();
#endif

	/*
	 * Loop until we've processed the proper number of tuples from the
	 * plan.
	 */

	for (;;)
	{
		/* Reset the per-output-tuple exprcontext */
		ResetPerTupleExprContext(estate);

		/*
		 * Execute the plan and obtain a tuple
		 */
lnext:	;
		if (estate->es_useEvalPlan)
		{
			slot = EvalPlanQualNext(estate);
			if (TupIsNull(slot))
				slot = ExecProcNode(plan, NULL);
		}
		else
			slot = ExecProcNode(plan, NULL);

		/*
		 * if the tuple is null, then we assume there is nothing more to
		 * process so we just return null...
		 */
		if (TupIsNull(slot))
		{
			result = NULL;
			break;
		}

        /*
		 * if we have a junk filter, then project a new tuple with the
		 * junk removed.
		 *
		 * Store this new "clean" tuple in the junkfilter's resultSlot.
		 * (Formerly, we stored it back over the "dirty" tuple, which is
		 * WRONG because that tuple slot has the wrong descriptor.)
		 *
		 * Also, extract all the junk information we need.
		 */
		if ((junkfilter = estate->es_junkFilter) != (JunkFilter *) NULL)
		{
			Datum		datum;
			HeapTuple	newTuple;
			bool		isNull;

			/*
			 * extract the 'ctid' junk attribute.
			 */
			if (operation == CMD_UPDATE || operation == CMD_DELETE)
			{
				if (!ExecGetJunkAttribute(junkfilter,
										  slot,
										  "ctid",
										  &datum,
										  &isNull))
					elog(ERROR, "ExecutePlan: NO (junk) `ctid' was found!");

				/* shouldn't ever get a null result... */
				if (isNull)
					elog(ERROR, "ExecutePlan: (junk) `ctid' is NULL!");

				tupleid = (ItemPointer) DatumGetPointer(datum);
				tuple_ctid = *tupleid;	/* make sure we don't free the
										 * ctid!! */
				tupleid = &tuple_ctid;
			}
			else if (estate->es_rowMark != NIL)
			{
				List	   *l;

		lmark:	;
				foreach(l, estate->es_rowMark)
				{
					execRowMark *erm = lfirst(l);
					Buffer		buffer;
					HeapTupleData tuple;
					TupleTableSlot *newSlot;
					int			test;

					if (!ExecGetJunkAttribute(junkfilter,
											  slot,
											  erm->resname,
											  &datum,
											  &isNull))
						elog(ERROR, "ExecutePlan: NO (junk) `%s' was found!",
							 erm->resname);

					/* shouldn't ever get a null result... */
					if (isNull)
						elog(ERROR, "ExecutePlan: (junk) `%s' is NULL!",
							 erm->resname);

					tuple.t_self = *((ItemPointer) DatumGetPointer(datum));
					test = heap_mark4update(erm->relation, &tuple, &buffer,
											estate->es_snapshot->curcid);
					ReleaseBuffer(buffer);
					switch (test)
					{
						case HeapTupleSelfUpdated:
							/* treat it as deleted; do not process */
							goto lnext;

						case HeapTupleMayBeUpdated:
							break;

						case HeapTupleUpdated:
							if (XactIsoLevel == XACT_SERIALIZABLE)
								elog(ERROR, "Can't serialize access due to concurrent update");
							if (!(ItemPointerEquals(&(tuple.t_self),
								  (ItemPointer) DatumGetPointer(datum))))
							{
								newSlot = EvalPlanQual(estate, erm->rti, &(tuple.t_self));
								if (!(TupIsNull(newSlot)))
								{
									slot = newSlot;
									estate->es_useEvalPlan = true;
									goto lmark;
								}
							}

							/*
							 * if tuple was deleted or PlanQual failed for
							 * updated tuple - we must not return this
							 * tuple!
							 */
							goto lnext;

						default:
							elog(ERROR, "Unknown status %u from heap_mark4update", test);
							return (NULL);
					}
				}
			}

			/*
			 * Finally create a new "clean" tuple with all junk attributes
			 * removed
			 */
			newTuple = ExecRemoveJunk(junkfilter, slot);

			slot = ExecStoreTuple(newTuple,		/* tuple to store */
								  junkfilter->jf_resultSlot,	/* dest slot */
								  InvalidBuffer,		/* this tuple has no
														 * buffer */
								  true);		/* tuple should be pfreed */
		}

		/*
		 * now that we have a tuple, do the appropriate thing with it..
		 * either return it to the user, add it to a relation someplace,
		 * delete it from a relation, or modify some of its attributes.
		 */

        /*
         * Check to see if we got a punctuation tuple.
         */
        if (NULL != slot->val && HeapTupleIsPunct(slot->val)) {
            if (window_mode) {
                /* Case 1: We're fetching entire windows of tuples.
                 * Assume the punctuation tuple marks the end of the window. */
                if (tcq_show_punct_in_output) {
                    result = slot;
                } 

                /* Exit from the loop. */
				break;
            

            } else {

                /* Case 2: We are fetching tuples numberTuples at a time. 
                 * Ignore the punctuation tuple if the user has requested it.
                 */
                if (!tcq_show_punct_in_output) {
                    elog(DEBUG1, "Ignoring a punctuation tuple.");
                    ExecClearTuple(slot);
                    goto lnext;
                }
            }
        }


#ifdef RECORD_OUTPUT_TIMES
		record_output_time();
#endif
		switch (operation)
		{
			case CMD_SELECT:
			  /* If we're extracing R/I/Dstream to the client, then
			   * don't call ExecSelect, which will send the result
			   * tuple to the client. We want to send the tuple
			   * ourselves, in ExecStreamedSelect.
			   */
			  if (estate->outputBufferState->cursorType == CURSORKIND_UNKNOWN)
			  {
				ExecSelect(slot,	/* slot containing tuple */
						   destfunc,	/* destination's tuple-receiver
										 * obj */
						   estate);
			  }
				result = slot;
				break;

			case CMD_INSERT:
				ExecInsert(slot, tupleid, estate);
				result = NULL;
				break;

			case CMD_DELETE:
				ExecDelete(slot, tupleid, estate);
				result = NULL;
				break;

			case CMD_UPDATE:
				ExecUpdate(slot, tupleid, estate);
				result = NULL;
				break;

			default:
				elog(LOG, "ExecutePlan: unknown operation in queryDesc");
				result = NULL;
				break;
		}

		/*
		 * check our tuple count.. if we've processed the proper number
		 * then quit, else loop again and process more tuples..
		 */
		current_tuple_count++;
		if (numberTuples == current_tuple_count)
			break;
	}

#ifdef RECORD_OUTPUT_TIMES
	finish_recording_output_times();
#endif

	/*
	 * here, result is either a slot containing a tuple in the case of a
	 * SELECT or NULL otherwise.
	 */
	return result;
}

/* ----------------------------------------------------------------
 *		ExecSelect
 *
 *		SELECTs are easy.. we just pass the tuple to the appropriate
 *		print function.  The only complexity is when we do a
 *		"SELECT INTO", in which case we insert the tuple into
 *		the appropriate relation (note: this is a newly created relation
 *		so we don't need to worry about indices or locks.)
 * ----------------------------------------------------------------
 */
static void
ExecSelect(TupleTableSlot *slot,
		   DestReceiver *destfunc,
		   EState *estate)
{
	HeapTuple	tuple;
	TupleDesc	attrtype;

	/*
	 * get the heap tuple out of the tuple table slot
	 */
	tuple = slot->val;
	attrtype = slot->ttc_tupleDescriptor;

	/*
	 * insert the tuple into the "into relation"
	 */
	if (estate->es_into_relation_descriptor != NULL)
	{
		heap_insert(estate->es_into_relation_descriptor, tuple,
					estate->es_snapshot->curcid);
		IncrAppended();
	}

	/*
	 * send the tuple to the front end (or the screen)
	 */
	(*destfunc->receiveTuple) (tuple, attrtype, destfunc);
	IncrRetrieved();
	(estate->es_processed)++;
}

/* ExecStreamedSelect
 * Sends "count" number of tuples to the client for an R/I/Dstream cursor
 * query.  Note that the cursorType must be one of CURSORKIND_R/I/DSTREAM,
 * and outputBufferState of the EState must already be set up.
 */
void
ExecStreamedSelect(QueryDesc *queryDesc, EState *estate, long count)
{
  OutputBufferState *outputState = estate->outputBufferState;
  DestReceiver *destfunc = DestToFunction(queryDesc->dest);

  Assert(outputState->cursorType != CURSORKIND_UNKNOWN &&
	 outputState->outputBuffer != NULL);

  if (outputState->outputScanner == NULL)
  {
    /* we've never used the heap scan before; open one up */

    outputState->outputScanner =
      heap_beginscan(outputState->outputBuffer,
		     SnapshotNow,
		     0,
		     (ScanKey) NULL);
  }

  if (outputState->outputWindowStartTime == 0)
  {
    /* we've never been run before; run the query for one tuple,
     * and use its timestamp as the current time
     */
    TupleTableSlot* slot;
    HeapTuple tuple;
    TupleDesc attrtype;
    Datum currentTime;
    bool isNull;

    slot = ExecutorRun(queryDesc, estate, ForwardScanDirection, 1);
    tuple = slot->val;
    attrtype = slot->ttc_tupleDescriptor;
    currentTime = heap_getattr(tuple, outputState->tsAttrNum,
			       attrtype, &isNull);

    /* HACK: find out why I need to do TGD(DGT(currentTime)) */
    outputState->outputWindowStartTime = TimestampGetDatum(DatumGetTimestamp(currentTime));    
    outputState->workingWindowStartTime = outputState->outputWindowStartTime;

    /* Insert the tuple into the relation.  Do it twice, so that we have a
     * dummy sentinel tuple at the beginning of the relation.  This is useful
     * if we want to scroll time to somewhere before the start of the relation
     * (so that ScrollUntil will have a tuple to stop at).
     */
    heap_insert(outputState->outputBuffer, tuple, estate->es_snapshot->curcid);
    heap_insert(outputState->outputBuffer, tuple, estate->es_snapshot->curcid);

    /* Skip the sentinel tuple */
    heap_getnext(outputState->outputScanner, ForwardScanDirection);
  }

  switch(outputState->cursorType)
  {
  case CURSORKIND_RSTREAM:
    SendNextRstreamTuples(queryDesc, estate, destfunc, count);
    break;

  case CURSORKIND_ISTREAM:
    SendNextIstreamTuples(queryDesc, estate, destfunc, count);
    break;

  case CURSORKIND_DSTREAM:
    SendNextDstreamTuples(queryDesc, estate, destfunc, count);
    break;

  default:
    elog(ERROR, "Cursortype not supported!");
    break;
  }

}

/* SendNextRstreamTuples
 * Send "count" RStream tuples to the client.
 */
static void
SendNextRstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count)
{
  OutputBufferState *outputState = estate->outputBufferState;

  if (!ScanDirectionIsNoMovement(outputState->timeMoveDirection))
  {
    /* the time cursor just moved, so we need to do some scrolling */
    outputState->outputWindowStartTime = outputState->newWindowStartTime;
    outputState->workingWindowStartTime = outputState->outputWindowStartTime;

    /* scroll till the new start time */
    ScrollUntil(queryDesc, estate,
		outputState->outputWindowStartTime);
    outputState->timeMoveDirection = NoMovementScanDirection;
    outputState->windowDry = false;
  }

  SendStreamTuples(queryDesc, estate, destfunc, count);
}

/* SendNextIstreamTuples
 * Send "count" IStream tuples to the client 
 */
static void
SendNextIstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count)
{ 
  OutputBufferState *outputState = estate->outputBufferState;

  if (ScanDirectionIsForward(outputState->timeMoveDirection))
  {
    /* if we're moving forward in time, then:
     * * set workingWindowSize to equal windowSize
     * * CHECK if there's overlap between the previous window and the next.  That
     *   is, if newWindowStartTime - outputWindowStartTime < windowSize.
     *
     * if there IS overlap, then
     * 1. scroll the scanner to outputWindowStartTime + windowSize
     * 2. set outputWindowStartTime <- newWindowStartTime
     *
     * if there IS NOT overlap, then
     * 1. scroll the scanner to newWindowStartTime
     * 2. set outputWindowStartTime <- newWindowStartTime
     */
    Datum timeDiff = DirectFunctionCall2(timestamp_mi, 
					 outputState->newWindowStartTime,
					 outputState->outputWindowStartTime);
    Datum timeScrollDest;

    /* the workingWindowSize will be the same */
    outputState->workingWindowSize = outputState->windowSize;

    if (DatumGetBool(DirectFunctionCall2(interval_lt, 
					 timeDiff, 
					 IntervalPGetDatum(&(outputState->windowSize)))))
    {
      /* There is an overlap between the previous and the new window!  We scroll
       * the heapscan to the old start time plus the window size, so we skip all
       * the tuples already in the previous window.
       */
      elog(DEBUG3, "SendNextIstreamTuples: Moving time forward with overlap\n");

      timeScrollDest = DirectFunctionCall2(timestamp_pl_span, 
					   outputState->outputWindowStartTime,
					   IntervalPGetDatum(&(outputState->windowSize)));
    }
    else
    {
      /* There is no overlap, so behave like an RStream and just scroll to the
       * new start time
       */
      elog(DEBUG3, "SendNextIstreamTuples: Moving time forward without overlap\n");

      timeScrollDest = outputState->newWindowStartTime;
    }

    ScrollUntil(queryDesc, estate, timeScrollDest);

    outputState->timeMoveDirection = NoMovementScanDirection;    
    outputState->windowDry = false;

    outputState->outputWindowStartTime = outputState->newWindowStartTime;
    outputState->workingWindowStartTime = outputState->outputWindowStartTime;
  }
  else if (ScanDirectionIsBackward(outputState->timeMoveDirection))
  {
    /* if we're moving backward in time, then
     * * scroll the scanner to newWindowStartTime
     * * CHECK IF there's overlap between the previous window and the next.  That
     *   is, if outputWindowStartTime - newWindowStartTime < windowSize
     *
     * if there IS overlap, then
     * 1. set workingWindowSize <- outputWindowStartTime - newWindowStartTime
     * 2. set outputWindowStartTime <- newWindowStartTime
     *
     * if there is NO overlap, then
     * 1. set workingWindowSize <- windowSize
     * 2. set outputWindowStartTime <- newWindowStartTime
     */

    Datum timeDiff = DirectFunctionCall2(timestamp_mi,
					 outputState->outputWindowStartTime,
					 outputState->newWindowStartTime);

    if (DatumGetBool(DirectFunctionCall2(interval_lt, timeDiff,
					 IntervalPGetDatum(&(outputState->windowSize)))))
    {
      /* There is overlap! We only want tuples up to the old start time, so our
       * window size will be different (old window start - new window start)
       */
      Interval* newWindow = DatumGetIntervalP(DirectFunctionCall2(timestamp_mi,
								  outputState->outputWindowStartTime,
								  outputState->newWindowStartTime));
      outputState->workingWindowSize = *newWindow;

      elog(DEBUG3, "SendNextIstreamTuples: moving time backward with overlap\n");
    }
    else
    {
      /* No overlap; behave like RStream */

      outputState->workingWindowSize = outputState->windowSize;

      elog(DEBUG3, "SendNextIstreamTuples: moving time backward without overlap\n");
    }

    outputState->outputWindowStartTime = outputState->newWindowStartTime;
    outputState->workingWindowStartTime = outputState->outputWindowStartTime;

    ScrollUntil(queryDesc, estate, outputState->workingWindowStartTime);

    outputState->timeMoveDirection = NoMovementScanDirection;
    outputState->windowDry = false;

  }

  SendStreamTuples(queryDesc, estate, destfunc, count);
}

/* SendNextDstreamTuples
 * Send the next "count" DStream tuples
 */
static void
SendNextDstreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count)
{
  OutputBufferState *outputState = estate->outputBufferState;

  if (ScanDirectionIsForward(outputState->timeMoveDirection))
  {
    /* if we're moving forward in time, then
     * * CHECK if there's overlap between the previous window and the next.  That
     *   is, if newWindowStartTime - outputWindowStartTime < windowSize.
     *
     * if there IS overlap, then
     * 1. workingWindowStartTime <- outputWindowStartTime
     * 2. workingWindowSize = newWindowStartTime - outputWindowStartTime
     * 3. outputWindowStartTime <- newWindowStartTime
     * 4. scroll until workingWindowStartTime
     *
     * if there is NO overlap, then send off all tuples from the previous window
     * 1. workingWindowStartTime <- outputWindowStartTime
     * 2. workingSize <- windowSize
     * 3. outputWindowStartTime <- newWindowStartTime
     */

    Datum timeDiff = DirectFunctionCall2(timestamp_mi,
					 outputState->newWindowStartTime,
					 outputState->outputWindowStartTime);
    outputState->timeMoveDirection = NoMovementScanDirection;
    
    if (DatumGetBool(DirectFunctionCall2(interval_lt,
					 timeDiff,
					 IntervalPGetDatum(&(outputState->windowSize)))))
    {
      /* There is overlap! Then we want to only get tuples between new start time
       * and the old start time.  The workingWindowSize will reflect that.  The
       * workingStartTime will also be the old start time.
       */

      Interval *newWindow = 
	DatumGetIntervalP(DirectFunctionCall2(timestamp_mi,
					      outputState->newWindowStartTime,
					      outputState->outputWindowStartTime));

      outputState->workingWindowStartTime = outputState->outputWindowStartTime;
      outputState->workingWindowSize = *newWindow;
      outputState->outputWindowStartTime = outputState->newWindowStartTime;

      elog(DEBUG3, "SendNextDstreamTuples: Moving time forward with overlap\n");

      ScrollUntil(queryDesc, estate, outputState->workingWindowStartTime);

      outputState->windowDry = false;
    }
    else
    {
      /* No overlap! Then the DStream tuples are all the tuples in the 
       * previous window.
       */

      elog(DEBUG3, "SendNextDstreamTuples: moving time forward without overlap\n");

      outputState->workingWindowStartTime = outputState->outputWindowStartTime;
      outputState->workingWindowSize = outputState->windowSize;

      outputState->outputWindowStartTime = outputState->newWindowStartTime;

      ScrollUntil(queryDesc, estate, outputState->workingWindowStartTime);
      outputState->windowDry = false;
    }
  }
  else if (ScanDirectionIsBackward(outputState->timeMoveDirection))
  {
    /* if we're moving backward in time, then...
     * * CHECK IF there's overlap between the previous window and the next.  That
     *   is, if outputWindowStartTime - newWindowStartTime < windowSize
     *
     * if there IS overlap, then
     * 1. workingWindowStartTime <- newWindowStartTime + windowSize
     * 2. workingWindowSize <- outputWindowSize + windowSize - workingWindowStartTime
     * 3. outputWindowStartTime <- newWindowStartTime
     * 4. scroll to workingWindowStartTime
     *
     * if there's no overlap, do as for moving forward
     */
    Datum timeDiff = DirectFunctionCall2(timestamp_mi,
					 outputState->outputWindowStartTime,
					 outputState->newWindowStartTime);

    outputState->timeMoveDirection = NoMovementScanDirection;

    if (DatumGetBool(DirectFunctionCall2(interval_lt, timeDiff,
					 IntervalPGetDatum(&(outputState->windowSize)))))
    {
      /* There IS overlap! Then we want all the tuples from the new window's end
       * to the old window's end
       */
      Datum newWindowEnd = 
	DirectFunctionCall2(timestamp_pl_span,
			    outputState->newWindowStartTime,
			    IntervalPGetDatum(&outputState->windowSize));
      Datum oldWindowEnd =
	DirectFunctionCall2(timestamp_pl_span,
			    outputState->outputWindowStartTime,
			    IntervalPGetDatum(&outputState->windowSize));

      outputState->workingWindowStartTime = newWindowEnd;
      outputState->workingWindowSize =
	*(DatumGetIntervalP(DirectFunctionCall2(timestamp_mi,
						oldWindowEnd, newWindowEnd)));

      outputState->outputWindowStartTime = outputState->newWindowStartTime;

      elog(DEBUG3, "SendNextDstreamTuples: Moving time forward with overlap\n");

      ScrollUntil(queryDesc, estate, outputState->workingWindowStartTime);

      outputState->windowDry = false;
    }
    else
    {
      /* No overlap! Then, send off all tuples from previous window */

      elog(DEBUG3, "SendNextDstreamTuples: moving time backward without overlap\n");

      outputState->workingWindowStartTime = outputState->outputWindowStartTime;
      outputState->workingWindowSize = outputState->windowSize;

      outputState->outputWindowStartTime = outputState->newWindowStartTime;

      ScrollUntil(queryDesc, estate, outputState->workingWindowStartTime);
      outputState->windowDry = false;
    }
  }

  SendStreamTuples(queryDesc, estate, destfunc, count);
}

/* SendStreamTuples
 * Sends off up to "count" tuples to the DestReceiver.
 * It sends tuples from the current scanner position until either it fulfills the
 * count request, or the current window runs dry.  If we need tuples not in the
 * buffer relation, we execute the query to get them.
 */
static void
SendStreamTuples(QueryDesc *queryDesc, EState *estate, DestReceiver *destfunc, long count)
{
  TupleTableSlot *slot;
  HeapTuple tuple;
  TupleDesc attrtype = queryDesc->tupDesc;
  OutputBufferState *outputState = estate->outputBufferState;
  Interval* distToStart;
  bool isNull;
  bool runQuery = false;
  int tuplesSent=0;

  elog(DEBUG3, "SendStreamTuples: outputStart: %s, workingStart: %s, workingWindow: %s\n",
       (char*)DirectFunctionCall1(timestamp_out, outputState->outputWindowStartTime),
       (char*)DirectFunctionCall1(timestamp_out, outputState->workingWindowStartTime),
       (char*)DirectFunctionCall1(interval_out, IntervalPGetDatum(&(outputState->workingWindowSize))));

  /* setup the destination receiver */
  (*destfunc->setup) (destfunc, CMD_SELECT, queryDesc->portalName, queryDesc->tupDesc);

  if (outputState->windowDry)
  {
    /* Window dry! Signal window dry again */
    tuple = NULL;
    (*destfunc->receiveTuple) (NULL, attrtype, destfunc);
    elog(DEBUG2, "Window is dry!");
  }
  else
  {
    while(tuplesSent < count)
    {
      if (!runQuery)
      {
	/* We haven't run out of tuples from our buffer relation, so keep scanning */

	tuple = heap_getnext(outputState->outputScanner, ForwardScanDirection);
	if (tuple == NULL)
	{
	  /* we've run out of tuples in the buffer! Must execute query */
	  runQuery = true;
	}
      }
      if (runQuery)
      {
	/* Execute the plan for one tuple, insert it into the buffer relation,
	 * and move the heap scanner forward (to avoid seeing it again)
	 */

	slot = ExecutePlan(estate,
			   queryDesc->plantree,
			   queryDesc->operation,
			   1,
			   ForwardScanDirection,
			   destfunc);
	tuple = slot->val;      
	heap_insert(outputState->outputBuffer, tuple, estate->es_snapshot->curcid);
	tuple = heap_getnext(outputState->outputScanner, ForwardScanDirection);
      }

      /* Calculate if the tuple we're about to send is still in the window.
       * distToStart = the tuple's timestamp - workingWindowStartTime.
       * if workingWindowSize < distToStart, then the window is dry.
       */

      outputState->outputCurrentTime = heap_getattr(tuple, outputState->tsAttrNum,
						    attrtype, &isNull);

      /* calculate the amount of time from window start to current time
       * distToStart = CurrentTime - windowStartTime
       */
      distToStart = DatumGetIntervalP(DirectFunctionCall2(timestamp_mi,
							  outputState->outputCurrentTime,
							  outputState->workingWindowStartTime));

      if (DatumGetBool(DirectFunctionCall2(interval_lt,
					   IntervalPGetDatum(&(outputState->workingWindowSize)),
					   IntervalPGetDatum(distToStart)))) 
      {
	/* Window dry; say so, and exit */

	outputState->windowDry = true;
	(*destfunc->receiveTuple) (NULL, attrtype, destfunc);
	elog(DEBUG2, "Window is dry!");
	break;
      }

      (*destfunc->receiveTuple) (tuple, attrtype, destfunc);
      IncrRetrieved();
      (estate->es_processed)++;
      tuplesSent++;
    }
  }

  /* Clean up the receiver */
  (*destfunc->cleanup) (destfunc);
}

/* ScrollUntil
 * Scrolls the temporary output buffer relation's heap scanner to one tuple
 * BEFORE the oldest tuple that has timestamp >= the given time.  If the tuple
 * is not in the relation, run the query to get to it.
 */
static void
ScrollUntil(QueryDesc *queryDesc, EState *estate, Datum time)
{
  OutputBufferState *outputState = estate->outputBufferState;
  HeapScanDesc scanner = outputState->outputScanner;
  TupleDesc attrtype = queryDesc->tupDesc;
  bool isNull;
  HeapTuple cur_tuple = heap_getnext(scanner, NoMovementScanDirection);
  Datum cur_timestamp = heap_getattr(cur_tuple, outputState->tsAttrNum, attrtype, &isNull);

  elog(DEBUG3, "ScrollUntil: moving the window to start at %s\n",
       (char*)DirectFunctionCall1(timestamp_out, time));

  /* first, find out if we should scroll forward or backward.  If
   * current timestamp < desired time, then scan forward.
   * Else, scan backward.
   */
  if (DatumGetBool(DirectFunctionCall2(timestamp_lt,
				       cur_timestamp,
				       time)))
  {
    /* Scroll forward! While the next tuple's timestamp is still less than
     * the desired time, keep scrolling forward
     */

    bool runQuery = false;

    while(DatumGetBool(DirectFunctionCall2(timestamp_le,
					   cur_timestamp,
					   time)))
    {

      if (!runQuery)
      {
	/* Still have enough tuples in the relation */

	cur_tuple = heap_getnext(scanner, ForwardScanDirection);
	
	if (cur_tuple == NULL)
	{
	  /* Out of tuples! */
	  runQuery = true;
	}
      }

      if (runQuery)
      {
	/* Out of tuples! Execute the plan for one tuple, insert it into
	 * the relation, and move the scanner forward 
	 */

	TupleTableSlot* slot;


	slot = ExecutePlan(estate,
			   queryDesc->plantree,
			   queryDesc->operation,
			   1,
			   ForwardScanDirection,
			   NULL);

	cur_tuple = slot->val;
	heap_insert(outputState->outputBuffer, cur_tuple, estate->es_snapshot->curcid);
	cur_tuple = heap_getnext(outputState->outputScanner, ForwardScanDirection);
      }

      cur_timestamp = heap_getattr(cur_tuple, outputState->tsAttrNum,
				   attrtype, &isNull);

    }

    /* Done! But backup one tuple, since we're supposed to scroll to the tuple
     * BEFORE the desired one 
     */
    heap_getnext(outputState->outputScanner, BackwardScanDirection);
  }
  else if (DatumGetBool(DirectFunctionCall2(timestamp_gt,
					    cur_timestamp,
					    time)))
  {
    /* Scroll backward! While the current tuple's timestamp is greater than
     * the desired time, keep scrolling back
     */

    while(DatumGetBool(DirectFunctionCall2(timestamp_ge,
					   cur_timestamp,
					   time)))
    {
      cur_tuple = heap_getnext(scanner, BackwardScanDirection);
	
      if (cur_tuple == NULL)
      {
	/* We've reached the beginning of the relation, so give up */
	break;
      }
      cur_timestamp = heap_getattr(cur_tuple, outputState->tsAttrNum,
				   attrtype, &isNull);

    }
  }  
}

/* ----------------------------------------------------------------
 *		ExecInsert
 *
 *		INSERTs are trickier.. we have to insert the tuple into
 *		the base relation and insert appropriate tuples into the
 *		index relations.
 * ----------------------------------------------------------------
 */
static void
ExecInsert(TupleTableSlot *slot,
		   ItemPointer tupleid,
		   EState *estate)
{
	HeapTuple	tuple;
	ResultRelInfo *resultRelInfo;
	Relation	resultRelationDesc;
	int			numIndices;
	Oid			newId;

	/*
	 * get the heap tuple out of the tuple table slot
	 */
	tuple = slot->val;

	/*
	 * get information on the (current) result relation
	 */
	resultRelInfo = estate->es_result_relation_info;
	resultRelationDesc = resultRelInfo->ri_RelationDesc;

	/* BEFORE ROW INSERT Triggers */
	if (resultRelInfo->ri_TrigDesc &&
	  resultRelInfo->ri_TrigDesc->n_before_row[TRIGGER_EVENT_INSERT] > 0)
	{
		HeapTuple	newtuple;

		newtuple = ExecBRInsertTriggers(estate, resultRelInfo, tuple);

		if (newtuple == NULL)	/* "do nothing" */
			return;

		if (newtuple != tuple)	/* modified by Trigger(s) */
		{
			/*
			 * Insert modified tuple into tuple table slot, replacing the
			 * original.  We assume that it was allocated in per-tuple
			 * memory context, and therefore will go away by itself. The
			 * tuple table slot should not try to clear it.
			 */
			ExecStoreTuple(newtuple, slot, InvalidBuffer, false);
			tuple = newtuple;
		}
	}

	/*
	 * Check the constraints of the tuple
	 */
	if (resultRelationDesc->rd_att->constr)
		ExecConstraints("ExecInsert", resultRelInfo, slot, estate);

	/*
	 * insert the tuple
	 */
	newId = heap_insert(resultRelationDesc, tuple,
						estate->es_snapshot->curcid);

	IncrAppended();
	(estate->es_processed)++;
	estate->es_lastoid = newId;
	setLastTid(&(tuple->t_self));

	/*
	 * process indices
	 *
	 * Note: heap_insert adds a new tuple to a relation.  As a side effect,
	 * the tupleid of the new tuple is placed in the new tuple's t_ctid
	 * field.
	 */
	numIndices = resultRelInfo->ri_NumIndices;
	if (numIndices > 0)
		ExecInsertIndexTuples(slot, &(tuple->t_self), estate, false);

	/* AFTER ROW INSERT Triggers */
	if (resultRelInfo->ri_TrigDesc)
		ExecARInsertTriggers(estate, resultRelInfo, tuple);
}

/* ----------------------------------------------------------------
 *		ExecDelete
 *
 *		DELETE is like UPDATE, we delete the tuple and its
 *		index tuples.
 * ----------------------------------------------------------------
 */
static void
ExecDelete(TupleTableSlot *slot,
		   ItemPointer tupleid,
		   EState *estate)
{
	ResultRelInfo *resultRelInfo;
	Relation	resultRelationDesc;
	ItemPointerData ctid;
	int			result;

	/*
	 * get information on the (current) result relation
	 */
	resultRelInfo = estate->es_result_relation_info;
	resultRelationDesc = resultRelInfo->ri_RelationDesc;

	/* BEFORE ROW DELETE Triggers */
	if (resultRelInfo->ri_TrigDesc &&
	  resultRelInfo->ri_TrigDesc->n_before_row[TRIGGER_EVENT_DELETE] > 0)
	{
		bool		dodelete;

		dodelete = ExecBRDeleteTriggers(estate, resultRelInfo, tupleid);

		if (!dodelete)			/* "do nothing" */
			return;
	}

	/*
	 * delete the tuple
	 */
ldelete:;
	result = heap_delete(resultRelationDesc, tupleid,
						 &ctid,
						 estate->es_snapshot->curcid);
	switch (result)
	{
		case HeapTupleSelfUpdated:
			/* already deleted by self; nothing to do */
			return;

		case HeapTupleMayBeUpdated:
			break;

		case HeapTupleUpdated:
			if (XactIsoLevel == XACT_SERIALIZABLE)
				elog(ERROR, "Can't serialize access due to concurrent update");
			else if (!(ItemPointerEquals(tupleid, &ctid)))
			{
				TupleTableSlot *epqslot = EvalPlanQual(estate,
							   resultRelInfo->ri_RangeTableIndex, &ctid);

				if (!TupIsNull(epqslot))
				{
					*tupleid = ctid;
					goto ldelete;
				}
			}
			/* tuple already deleted; nothing to do */
			return;

		default:
			elog(ERROR, "Unknown status %u from heap_delete", result);
			return;
	}

	IncrDeleted();
	(estate->es_processed)++;

	/*
	 * Note: Normally one would think that we have to delete index tuples
	 * associated with the heap tuple now..
	 *
	 * ... but in POSTGRES, we have no need to do this because the vacuum
	 * daemon automatically opens an index scan and deletes index tuples
	 * when it finds deleted heap tuples. -cim 9/27/89
	 */

	/* AFTER ROW DELETE Triggers */
	if (resultRelInfo->ri_TrigDesc)
		ExecARDeleteTriggers(estate, resultRelInfo, tupleid);
}

/* ----------------------------------------------------------------
 *		ExecUpdate
 *
 *		note: we can't run UPDATE queries with transactions
 *		off because UPDATEs are actually INSERTs and our
 *		scan will mistakenly loop forever, updating the tuple
 *		it just inserted..	This should be fixed but until it
 *		is, we don't want to get stuck in an infinite loop
 *		which corrupts your database..
 * ----------------------------------------------------------------
 */
static void
ExecUpdate(TupleTableSlot *slot,
		   ItemPointer tupleid,
		   EState *estate)
{
	HeapTuple	tuple;
	ResultRelInfo *resultRelInfo;
	Relation	resultRelationDesc;
	ItemPointerData ctid;
	int			result;
	int			numIndices;

	/*
	 * abort the operation if not running transactions
	 */
	if (IsBootstrapProcessingMode())
	{
		elog(WARNING, "ExecUpdate: UPDATE can't run without transactions");
		return;
	}

	/*
	 * get the heap tuple out of the tuple table slot
	 */
	tuple = slot->val;

	/*
	 * get information on the (current) result relation
	 */
	resultRelInfo = estate->es_result_relation_info;
	resultRelationDesc = resultRelInfo->ri_RelationDesc;

	/* BEFORE ROW UPDATE Triggers */
	if (resultRelInfo->ri_TrigDesc &&
	  resultRelInfo->ri_TrigDesc->n_before_row[TRIGGER_EVENT_UPDATE] > 0)
	{
		HeapTuple	newtuple;

		newtuple = ExecBRUpdateTriggers(estate, resultRelInfo,
										tupleid, tuple);

		if (newtuple == NULL)	/* "do nothing" */
			return;

		if (newtuple != tuple)	/* modified by Trigger(s) */
		{
			/*
			 * Insert modified tuple into tuple table slot, replacing the
			 * original.  We assume that it was allocated in per-tuple
			 * memory context, and therefore will go away by itself. The
			 * tuple table slot should not try to clear it.
			 */
			ExecStoreTuple(newtuple, slot, InvalidBuffer, false);
			tuple = newtuple;
		}
	}

	/*
	 * Check the constraints of the tuple
	 *
	 * If we generate a new candidate tuple after EvalPlanQual testing, we
	 * must loop back here and recheck constraints.  (We don't need to
	 * redo triggers, however.	If there are any BEFORE triggers then
	 * trigger.c will have done mark4update to lock the correct tuple, so
	 * there's no need to do them again.)
	 */
lreplace:;
	if (resultRelationDesc->rd_att->constr)
		ExecConstraints("ExecUpdate", resultRelInfo, slot, estate);

	/*
	 * replace the heap tuple
	 */
	result = heap_update(resultRelationDesc, tupleid, tuple,
						 &ctid,
						 estate->es_snapshot->curcid);
	switch (result)
	{
		case HeapTupleSelfUpdated:
			/* already deleted by self; nothing to do */
			return;

		case HeapTupleMayBeUpdated:
			break;

		case HeapTupleUpdated:
			if (XactIsoLevel == XACT_SERIALIZABLE)
				elog(ERROR, "Can't serialize access due to concurrent update");
			else if (!(ItemPointerEquals(tupleid, &ctid)))
			{
				TupleTableSlot *epqslot = EvalPlanQual(estate,
							   resultRelInfo->ri_RangeTableIndex, &ctid);

				if (!TupIsNull(epqslot))
				{
					*tupleid = ctid;
					tuple = ExecRemoveJunk(estate->es_junkFilter, epqslot);
					slot = ExecStoreTuple(tuple,
									estate->es_junkFilter->jf_resultSlot,
										  InvalidBuffer, true);
					goto lreplace;
				}
			}
			/* tuple already deleted; nothing to do */
			return;

		default:
			elog(ERROR, "Unknown status %u from heap_update", result);
			return;
	}

	IncrReplaced();
	(estate->es_processed)++;

	/*
	 * Note: instead of having to update the old index tuples associated
	 * with the heap tuple, all we do is form and insert new index tuples.
	 * This is because UPDATEs are actually DELETEs and INSERTs and index
	 * tuple deletion is done automagically by the vacuum daemon. All we
	 * do is insert new index tuples.  -cim 9/27/89
	 */

	/*
	 * process indices
	 *
	 * heap_update updates a tuple in the base relation by invalidating it
	 * and then inserting a new tuple to the relation.	As a side effect,
	 * the tupleid of the new tuple is placed in the new tuple's t_ctid
	 * field.  So we now insert index tuples using the new tupleid stored
	 * there.
	 */

	numIndices = resultRelInfo->ri_NumIndices;
	if (numIndices > 0)
		ExecInsertIndexTuples(slot, &(tuple->t_self), estate, false);

	/* AFTER ROW UPDATE Triggers */
	if (resultRelInfo->ri_TrigDesc)
		ExecARUpdateTriggers(estate, resultRelInfo, tupleid, tuple);
}

static char *
ExecRelCheck(ResultRelInfo *resultRelInfo,
			 TupleTableSlot *slot, EState *estate)
{
	Relation	rel = resultRelInfo->ri_RelationDesc;
	int			ncheck = rel->rd_att->constr->num_check;
	ConstrCheck *check = rel->rd_att->constr->check;
	ExprContext *econtext;
	MemoryContext oldContext;
	List	   *qual;
	int			i;

	/*
	 * If first time through for this result relation, build expression
	 * nodetrees for rel's constraint expressions.  Keep them in the
	 * per-query memory context so they'll survive throughout the query.
	 */
	if (resultRelInfo->ri_ConstraintExprs == NULL)
	{
		oldContext = MemoryContextSwitchTo(estate->es_query_cxt);
		resultRelInfo->ri_ConstraintExprs =
			(List **) palloc(ncheck * sizeof(List *));
		for (i = 0; i < ncheck; i++)
		{
			qual = (List *) stringToNode(check[i].ccbin);
			resultRelInfo->ri_ConstraintExprs[i] = qual;
		}
		MemoryContextSwitchTo(oldContext);
	}

	/*
	 * We will use the EState's per-tuple context for evaluating
	 * constraint expressions (creating it if it's not already there).
	 */
	econtext = GetPerTupleExprContext(estate);

	/* Arrange for econtext's scan tuple to be the tuple under test */
	econtext->ecxt_scantuple = slot;

	/* And evaluate the constraints */
	for (i = 0; i < ncheck; i++)
	{
		qual = resultRelInfo->ri_ConstraintExprs[i];

		/*
		 * NOTE: SQL92 specifies that a NULL result from a constraint
		 * expression is not to be treated as a failure.  Therefore, tell
		 * ExecQual to return TRUE for NULL.
		 */
		if (!ExecQual(qual, econtext, true))
			return check[i].ccname;
	}

	/* NULL result means no error */
	return (char *) NULL;
}

void
ExecConstraints(const char *caller, ResultRelInfo *resultRelInfo,
				TupleTableSlot *slot, EState *estate)
{
	Relation	rel = resultRelInfo->ri_RelationDesc;
	HeapTuple	tuple = slot->val;
	TupleConstr *constr = rel->rd_att->constr;

	Assert(constr);

	if (constr->has_not_null)
	{
		int			natts = rel->rd_att->natts;
		int			attrChk;

		for (attrChk = 1; attrChk <= natts; attrChk++)
		{
			if (rel->rd_att->attrs[attrChk - 1]->attnotnull &&
				heap_attisnull(tuple, attrChk))
				elog(ERROR, "%s: Fail to add null value in not null attribute %s",
					 caller, NameStr(rel->rd_att->attrs[attrChk - 1]->attname));
		}
	}

	if (constr->num_check > 0)
	{
		char	   *failed;

		if ((failed = ExecRelCheck(resultRelInfo, slot, estate)) != NULL)
			elog(ERROR, "%s: rejected due to CHECK constraint \"%s\" on \"%s\"",
				 caller, failed, RelationGetRelationName(rel));
	}
}

/*
 * Check a modified tuple to see if we want to process its updated version
 * under READ COMMITTED rules.
 *
 * See backend/executor/README for some info about how this works.
 */
TupleTableSlot *
EvalPlanQual(EState *estate, Index rti, ItemPointer tid)
{
	evalPlanQual *epq;
	EState	   *epqstate;
	Relation	relation;
	HeapTupleData tuple;
	HeapTuple	copyTuple = NULL;
	int			rtsize;
	bool		endNode;

	Assert(rti != 0);

	/*
	 * find relation containing target tuple
	 */
	if (estate->es_result_relation_info != NULL &&
		estate->es_result_relation_info->ri_RangeTableIndex == rti)
		relation = estate->es_result_relation_info->ri_RelationDesc;
	else
	{
		List	   *l;

		relation = NULL;
		foreach(l, estate->es_rowMark)
		{
			if (((execRowMark *) lfirst(l))->rti == rti)
			{
				relation = ((execRowMark *) lfirst(l))->relation;
				break;
			}
		}
		if (relation == NULL)
			elog(ERROR, "EvalPlanQual: can't find RTE %d", (int) rti);
	}

	/*
	 * fetch tid tuple
	 *
	 * Loop here to deal with updated or busy tuples
	 */
	tuple.t_self = *tid;
	for (;;)
	{
		Buffer		buffer;

		if (heap_fetch(relation, SnapshotDirty, &tuple, &buffer, false, NULL))
		{
			TransactionId xwait = SnapshotDirty->xmax;

			if (TransactionIdIsValid(SnapshotDirty->xmin))
				elog(ERROR, "EvalPlanQual: t_xmin is uncommitted ?!");

			/*
			 * If tuple is being updated by other transaction then we have
			 * to wait for its commit/abort.
			 */
			if (TransactionIdIsValid(xwait))
			{
				ReleaseBuffer(buffer);
				XactLockTableWait(xwait);
				continue;
			}

			/*
			 * We got tuple - now copy it for use by recheck query.
			 */
			copyTuple = heap_copytuple(&tuple);
			ReleaseBuffer(buffer);
			break;
		}

		/*
		 * Oops! Invalid tuple. Have to check is it updated or deleted.
		 * Note that it's possible to get invalid SnapshotDirty->tid if
		 * tuple updated by this transaction. Have we to check this ?
		 */
		if (ItemPointerIsValid(&(SnapshotDirty->tid)) &&
			!(ItemPointerEquals(&(tuple.t_self), &(SnapshotDirty->tid))))
		{
			/* updated, so look at the updated copy */
			tuple.t_self = SnapshotDirty->tid;
			continue;
		}

		/*
		 * Deleted or updated by this transaction; forget it.
		 */
		return NULL;
	}

	/*
	 * For UPDATE/DELETE we have to return tid of actual row we're
	 * executing PQ for.
	 */
	*tid = tuple.t_self;

	/*
	 * Need to run a recheck subquery.	Find or create a PQ stack entry.
	 */
	epq = (evalPlanQual *) estate->es_evalPlanQual;
	rtsize = length(estate->es_range_table);
	endNode = true;

	if (epq != NULL && epq->rti == 0)
	{
		/* Top PQ stack entry is idle, so re-use it */
		Assert(!(estate->es_useEvalPlan) &&
			   epq->estate.es_evalPlanQual == NULL);
		epq->rti = rti;
		endNode = false;
	}

	/*
	 * If this is request for another RTE - Ra, - then we have to check
	 * wasn't PlanQual requested for Ra already and if so then Ra' row was
	 * updated again and we have to re-start old execution for Ra and
	 * forget all what we done after Ra was suspended. Cool? -:))
	 */
	if (epq != NULL && epq->rti != rti &&
		epq->estate.es_evTuple[rti - 1] != NULL)
	{
		do
		{
			evalPlanQual *oldepq;

			/* pop previous PlanQual from the stack */
			epqstate = &(epq->estate);
			oldepq = (evalPlanQual *) epqstate->es_evalPlanQual;
			Assert(oldepq->rti != 0);
			/* stop execution */
			ExecEndNode(epq->plan, NULL);
			ExecDropTupleTable(epqstate->es_tupleTable, true);
			epqstate->es_tupleTable = NULL;
			heap_freetuple(epqstate->es_evTuple[epq->rti - 1]);
			epqstate->es_evTuple[epq->rti - 1] = NULL;
			/* push current PQ to freePQ stack */
			oldepq->free = epq;
			epq = oldepq;
			estate->es_evalPlanQual = (Pointer) epq;
		} while (epq->rti != rti);
	}

	/*
	 * If we are requested for another RTE then we have to suspend
	 * execution of current PlanQual and start execution for new one.
	 */
	if (epq == NULL || epq->rti != rti)
	{
		/* try to reuse plan used previously */
		evalPlanQual *newepq = (epq != NULL) ? epq->free : NULL;

		if (newepq == NULL)		/* first call or freePQ stack is empty */
		{
			newepq = (evalPlanQual *) palloc(sizeof(evalPlanQual));
			newepq->free = NULL;

			/*
			 * Each stack level has its own copy of the plan tree.	This
			 * is wasteful, but necessary as long as plan nodes point to
			 * exec state nodes rather than vice versa.  Note that
			 * copyfuncs.c doesn't attempt to copy the exec state nodes,
			 * which is a good thing in this situation.
			 */
			newepq->plan = copyObject(estate->es_origPlan);

			/*
			 * Init stack level's EState.  We share top level's copy of
			 * es_result_relations array and other non-changing status. We
			 * need our own tupletable, es_param_exec_vals, and other
			 * changeable state.
			 */
			epqstate = &(newepq->estate);
			memcpy(epqstate, estate, sizeof(EState));
			epqstate->es_direction = ForwardScanDirection;
			if (estate->es_origPlan->nParamExec > 0)
				epqstate->es_param_exec_vals = (ParamExecData *)
					palloc(estate->es_origPlan->nParamExec *
						   sizeof(ParamExecData));
			epqstate->es_tupleTable = NULL;
			epqstate->es_per_tuple_exprcontext = NULL;

			/*
			 * Each epqstate must have its own es_evTupleNull state, but
			 * all the stack entries share es_evTuple state.  This allows
			 * sub-rechecks to inherit the value being examined by an
			 * outer recheck.
			 */
			epqstate->es_evTupleNull = (bool *) palloc(rtsize * sizeof(bool));
			if (epq == NULL)
			{
				/* first PQ stack entry */
				epqstate->es_evTuple = (HeapTuple *)
					palloc(rtsize * sizeof(HeapTuple));
				memset(epqstate->es_evTuple, 0, rtsize * sizeof(HeapTuple));
			}
			else
			{
				/* later stack entries share the same storage */
				epqstate->es_evTuple = epq->estate.es_evTuple;
			}
		}
		else
		{
			/* recycle previously used EState */
			epqstate = &(newepq->estate);
		}
		/* push current PQ to the stack */
		epqstate->es_evalPlanQual = (Pointer) epq;
		epq = newepq;
		estate->es_evalPlanQual = (Pointer) epq;
		epq->rti = rti;
		endNode = false;
	}

	Assert(epq->rti == rti);
	epqstate = &(epq->estate);

	/*
	 * Ok - we're requested for the same RTE.  Unfortunately we still have
	 * to end and restart execution of the plan, because ExecReScan
	 * wouldn't ensure that upper plan nodes would reset themselves.  We
	 * could make that work if insertion of the target tuple were
	 * integrated with the Param mechanism somehow, so that the upper plan
	 * nodes know that their children's outputs have changed.
	 */
	if (endNode)
	{
		/* stop execution */
		ExecEndNode(epq->plan, NULL);
		ExecDropTupleTable(epqstate->es_tupleTable, true);
		epqstate->es_tupleTable = NULL;
	}

	/*
	 * free old RTE' tuple, if any, and store target tuple where
	 * relation's scan node will see it
	 */
	if (epqstate->es_evTuple[rti - 1] != NULL)
		heap_freetuple(epqstate->es_evTuple[rti - 1]);
	epqstate->es_evTuple[rti - 1] = copyTuple;

	/*
	 * Initialize for new recheck query; be careful to copy down state
	 * that might have changed in top EState.
	 */
	epqstate->es_result_relation_info = estate->es_result_relation_info;
	epqstate->es_junkFilter = estate->es_junkFilter;
	if (estate->es_origPlan->nParamExec > 0)
		memset(epqstate->es_param_exec_vals, 0,
			   estate->es_origPlan->nParamExec * sizeof(ParamExecData));
	epqstate->es_force_oids = estate->es_force_oids;
	memset(epqstate->es_evTupleNull, false, rtsize * sizeof(bool));
	epqstate->es_useEvalPlan = false;
	Assert(epqstate->es_tupleTable == NULL);
	epqstate->es_tupleTable =
		ExecCreateTupleTable(estate->es_tupleTable->size);

	ExecInitNode(epq->plan, epqstate, NULL);

	return EvalPlanQualNext(estate);
}

static TupleTableSlot *
EvalPlanQualNext(EState *estate)
{
	evalPlanQual *epq = (evalPlanQual *) estate->es_evalPlanQual;
	EState	   *epqstate = &(epq->estate);
	evalPlanQual *oldepq;
	TupleTableSlot *slot;

	Assert(epq->rti != 0);

lpqnext:;
	slot = ExecProcNode(epq->plan, NULL);

	/*
	 * No more tuples for this PQ. Continue previous one.
	 */
	if (TupIsNull(slot))
	{
		/* stop execution */
		ExecEndNode(epq->plan, NULL);
		ExecDropTupleTable(epqstate->es_tupleTable, true);
		epqstate->es_tupleTable = NULL;
		heap_freetuple(epqstate->es_evTuple[epq->rti - 1]);
		epqstate->es_evTuple[epq->rti - 1] = NULL;
		/* pop old PQ from the stack */
		oldepq = (evalPlanQual *) epqstate->es_evalPlanQual;
		if (oldepq == (evalPlanQual *) NULL)
		{
			epq->rti = 0;		/* this is the first (oldest) */
			estate->es_useEvalPlan = false;		/* PQ - mark as free and	  */
			return (NULL);		/* continue Query execution   */
		}
		Assert(oldepq->rti != 0);
		/* push current PQ to freePQ stack */
		oldepq->free = epq;
		epq = oldepq;
		epqstate = &(epq->estate);
		estate->es_evalPlanQual = (Pointer) epq;
		goto lpqnext;
	}

	return (slot);
}

static void
EndEvalPlanQual(EState *estate)
{
	evalPlanQual *epq = (evalPlanQual *) estate->es_evalPlanQual;
	EState	   *epqstate = &(epq->estate);
	evalPlanQual *oldepq;

	if (epq->rti == 0)			/* plans already shutdowned */
	{
		Assert(epq->estate.es_evalPlanQual == NULL);
		return;
	}

	for (;;)
	{
		/* stop execution */
		ExecEndNode(epq->plan, NULL);
		ExecDropTupleTable(epqstate->es_tupleTable, true);
		epqstate->es_tupleTable = NULL;
		if (epqstate->es_evTuple[epq->rti - 1] != NULL)
		{
			heap_freetuple(epqstate->es_evTuple[epq->rti - 1]);
			epqstate->es_evTuple[epq->rti - 1] = NULL;
		}
		/* pop old PQ from the stack */
		oldepq = (evalPlanQual *) epqstate->es_evalPlanQual;
		if (oldepq == (evalPlanQual *) NULL)
		{
			epq->rti = 0;		/* this is the first (oldest) */
			estate->es_useEvalPlan = false;		/* PQ - mark as free */
			break;
		}
		Assert(oldepq->rti != 0);
		/* push current PQ to freePQ stack */
		oldepq->free = epq;
		epq = oldepq;
		epqstate = &(epq->estate);
		estate->es_evalPlanQual = (Pointer) epq;
	}
}


/* SetupOutputBufferState
 * Sets up the OutputBufferState to be used for the given query, and
 * defines the temporary output buffer relation.
 */
static void
SetupOutputBufferState(Query *parseTree, OutputBufferState *outputState)
{
  List *attrList = NIL;
  List *tlist = parseTree->targetList;
  List* t;
  RangeVar *rel = makeNode(RangeVar);
  CreateStmt *createStmt = makeNode(CreateStmt);
  Oid relid;
  List *wl;
  bool foundTimestamp = false;

  outputState->cursorType = parseTree->cursorType;

  /* create the attributes list for an output tuple */
  foreach(t, tlist)
  {
    TargetEntry *entry = lfirst(t);
    Resdom *res = entry->resdom;

    if (!res->resjunk) 
    {
      ColumnDef *def = makeNode(ColumnDef);
      TypeName *typename = makeNode(TypeName);
		
      def->colname = pstrdup(res->resname);
      typename->typeid = res->restype;
      typename->typmod = res->restypmod;
      def->typename = typename;
      
      def->inhcount = 0;
      def->is_local = true;

      def->is_not_null = false;
      def->raw_default = NULL;
      def->cooked_default = NULL;
      def->constraints = NIL;
      def->support = NULL;

      if (res->restype == TIMESTAMPOID)
      {
	/* found the timestamp attribute number! */
	outputState->tsAttrNum = res->resno;
	def->is_timestamp = true;
	foundTimestamp = true;
      } 
      else 
      {
	def->is_timestamp = false;
      }
      
      attrList = lappend(attrList, def);
    }    
  }

  if (!foundTimestamp)
  {
    elog(ERROR, "Cannot extract %s because query output does not have timestamp attribute",
	 StringOfCursorType(outputState->cursorType));
  }

  /* find the windowSize of the query (currently we just use the last window
   * of the windows in windowExprs 
   */
  foreach(wl, parseTree->windowClause->windowExprs)
  {
    WindowExpr *we = (WindowExpr *) lfirst(wl);
    Interval* window = DatumGetIntervalP(we->rangeint->constvalue);
    outputState->windowSize = (*window);
  }

  /* the initial working window size is the default */
  outputState->workingWindowSize = outputState->windowSize;

  if (outputState->cursorType == CURSORKIND_DSTREAM)
  {
    /* DStream starts out with the empty set */
    outputState->windowDry = true;
  }
  else
  {
    outputState->windowDry = false;
  }

  /* Define the temporary output buffer relation */
  rel->catalogname = NULL;
  rel->schemaname = NULL;
  rel->relname = (char*) palloc(32 * sizeof(char));
  sprintf(rel->relname, "__outputBuffer_%d", newoid());
  rel->istemp = false;
  
  createStmt->relation = rel;
  createStmt->tableElts = attrList;
  createStmt->inhRelations = NIL;
  createStmt->constraints = NIL;
  createStmt->hasoids = true;

  relid = DefineRelation(createStmt, RELKIND_UNARCHIVEDSTREAM_SPILL);

  /* Open the relation for reading */
  outputState->outputBuffer = heap_open(relid, AccessExclusiveLock);	  

  /* Save the relid of the relation */
  outputState->outputRelid = relid;

  pfree(rel->relname);
}



