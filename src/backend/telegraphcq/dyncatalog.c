/*-------------------------------------------------------------------------
 *
 * dyncatalog.c
 *
 *	Infrastructure for dynamic "catalog" streams
 *
 * Author: Sailesh Krishnamurthy <sailesh@cs.berkeley.edu>
 *
 *	WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 *	  This code is currently VERY unsafe. It works under the oh-so-fragile
 *	  assumption that only one process calls its functions at any point in
 *	  time. At present, this one process is the TelegraphCQ Back-End process,
 *	  but in future this could be called from all over the place.
 *
 *	  It is vital to think whether or not this code mucks with any global data
 *	  structures in an unsafe way.
 *
 *	  For instance:
 *
 *		 (1) Clearly the tcq_dyncats global must be initialized only once - as
 *		 long as that's done only by (say) the TelegraphCQ Back-End process
 *		 we're still good.
 *
 *		 (2) The Info_tcq_queries of the tcq_dyncats will be mucked with on
 *		 calls to tcq_queries_enter() and tcq_queries_exit() - we must be
 *		 careful to synchronize these function calls. TBD !
 *
 *	WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/dyncatalog.c,v 1.10 2004/03/24 08:11:10 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/namespace.h"
#include "executor/nodeEddy.h"
#include "rqueue/ingresq.h"
#include "rqueue/planq.h"
#include "telegraphcq/dyncatalog.h"
#include "telegraphcq/telegraphcq.h"
#include "utils/builtins.h"
#include "utils/relcache.h"


DynCatInfo *tcq_dyncats;

/*
 * Functions internal to this module (compilation unit)
 */
static void insert_tcq_queries_entry(DynCatInfo * dyncat, uint32 qnum, uint32 qid,
						 char *k, char *qstr);

static void insert_tcq_operators_entry(DynCatInfo * dyncat, uint32 opnum, uint32 opid,
						 uint16 numqrs, char *k, uint32 qid, char *opstr,
						   char *opkind, char *opdesc);

/*
 *	This funtion sets up the data structures used to access the dynamic catalog
 *	streams. It MUST only be called once and from the TelegraphCQ backend.
 *
 *	A good place to call it is TelegraphCQMain
 *
 */
void
setupDynCatalogStreams()
{
	int			i = 0;
	Relation	r = NULL;

	time_t		timeval;
	struct tm	structtm;

	tcq_dyncats = (DynCatInfo *) palloc(sizeof(DynCatInfo) * TCQ_NUMCATS);

	tcq_dyncats[TCQ_QUERIES].name = StringToNameList(getCatName(TCQ_QUERIES));
	tcq_dyncats[TCQ_OPERATORS].name = StringToNameList(getCatName(TCQ_OPERATORS));
	tcq_dyncats[TCQ_QUEUES].name = StringToNameList(getCatName(TCQ_QUEUES));

	time(&timeval);
	localtime_r(&timeval, &structtm);

	for (i = 0; i < TCQ_NUMCATS; i++)
	{
		DynCatInfo *dyncat = &(tcq_dyncats[i]);
		RangeVar   *rvar;
		Oid			streamoid = InvalidOid;

		if (dyncat->name != NIL)
		{
			rvar = makeRangeVarFromNameList(dyncat->name);
			streamoid = RangeVarGetRelid(rvar, true);
		}
		if (streamoid == InvalidOid)
		{
			dyncat->active = false;
			elog(LOG, "TCQ:dyncat: Could not find oid for (%s)",
				 NameListToString(dyncat->name));
			continue;
		}
		else
		{
			dyncat->active = true;
			dyncat->streamoid = streamoid;
			r = RelationIdGetRelation(streamoid);
			Assert(r);
		}
		Assert(IsUnarchivedStream(r->rd_rel->relkind));
		dyncat->sinfo = 
		  setupUnarchivedStreamInfo(dyncat->name,
					    ST_UNARCHIVED_SPILL, streamoid);
		dyncat->tdesc = CreateTupleDescCopyConstr(r->rd_att);
		RelationClose(r);		/* Decrement the refcount in the relcache
								 * ! */

		dyncat->nulls = (char *) palloc(sizeof(char) * (dyncat->tdesc->natts));
		MemSet(dyncat->nulls, ' ', dyncat->tdesc->natts);
		MemSet(&(dyncat->catinfo), 0, INFO_TCQ_CATINFOSIZE);
		if (dyncat->active) 
		{
			dyncat->perTupleContext = AllocSetContextCreate(
				TopMemoryContext,
				"dyncat per tuple context",
				ALLOCSET_DEFAULT_MINSIZE,
				ALLOCSET_DEFAULT_INITSIZE,
				ALLOCSET_DEFAULT_MAXSIZE);
		}
	}
}

/*
 *	This function checks if the given Oid corresponds to that of a dynamic
 *	catalog stream.
 *
 *	soid	(IN)   Oid of the candidate stream
 *
 *	RETURNS		   true iff soid corresponds to a dynamic catalog stream
 */
bool
isDynamicCatalogStream(Oid soid)
{
	int			i;
	DynCatInfo *dyncat;

	for (i = 0; i < TCQ_NUMCATS; i++)
	{
		dyncat = &(tcq_dyncats[i]);
		if (dyncat->active && (soid == dyncat->streamoid))
			return true;
	}
	return false;
}

/*
 *	This function is a workhorse that gathers up arguments, gets the current
 *	timestamp, and makes 'em all into Datums. It then calls the
 *	IngresQInsertTuple function to actually create a new HeapTuple and insert it
 *	into the ingresQ.
 *
 *	dyncat	  (IN)	Pointer to the DynCatInfo structure for tcq_queries
 *	qnum	  (IN)	The query's unique query number
 *	qid		  (IN)	The query's queue number
 *	k		  (IN)	A character describing the "kind" of this entry
 *	qstr	  (IN)	A (possibly NULL) which is the query string
 *
 *	RETURNS			None
 */
static void
insert_tcq_queries_entry(DynCatInfo * dyncat, uint32 qnum, uint32 qid,
						 char *k, char *qstr)
{
	Datum				values[Natts_tcq_queries];
	Timestamp			timestamp;
	Datum				qstring = CStringGetDatum(qstr);
	Datum				kstring = CStringGetDatum(k);
	MemoryContext 		oldctx 	= MemoryContextSwitchTo(dyncat->perTupleContext);

	timestamp = GetCurrentTimestamp();

	values[Anum_tcq_queries_tcqtime] = PointerGetDatum(&timestamp);
	values[Anum_tcq_queries_querynum] = UInt32GetDatum(qnum);
	values[Anum_tcq_queries_queueid] = UInt32GetDatum(qid);
	values[Anum_tcq_queries_kind] = DirectFunctionCall1(textin, kstring);

	if (qstr == NULL)
	{
		values[Anum_tcq_queries_querystr] = PointerGetDatum(NULL);
		dyncat->nulls[Anum_tcq_queries_querystr] = 'n';
	}
	else
	{
		values[Anum_tcq_queries_querystr] = DirectFunctionCall1(textin, qstring);
		dyncat->nulls[Anum_tcq_queries_querystr] = ' ';
	}
	IngresQInsertTuple(dyncat->sinfo->inQ, dyncat->tdesc, values, dyncat->nulls);
	MemoryContextReset(dyncat->perTupleContext);
	MemoryContextSwitchTo(oldctx);	
	return;
}

/*
 *	This function is used to record a query entry event in TCQ. In addition,
 *	this function generates a unique identifier for the query. This identifier
 *	is necessary when recording the corresponding query exit event. However, the
 *	CQEddy will identify the query being removed by supplying its queueNo and
 *	have to use that to figure out the unique query identifier at exit time. So
 *	we must maintain a data structure (Info_tcq_queries.our_query_nums) that
 *	maps the query's queueNo. Of course, the queueNo remains fixed and in the
 *	range [0,TCQ_MAX_QUERIES) for the duration of the query. The data structure
 *	is just going to be an array of unique query numbers identified by the
 *	query's queueNo.
 *
 *	query	 (IN)  Pointer to new query
 *	rqueueNo (IN)  Index of the result queue corresponding to this query
 *
 *	RETURNS		   None
 */
void
tcq_queries_enter(planqentry * pqe)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_QUERIES]);
	Info_tcq_queries *pinfo = &(dyncat->catinfo.tcq_queries);

	StringInfo	querystr = NULL;
	uint32		qnum;
	uint32		qid;

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	querystr = pqe->querystr;
	qnum = ++pinfo->max_query_num;		/* Generate next unique query
										 * number  */
	qid = pqe->queueNo;			/* Collect the queue id */
	pinfo->cur_query_nums[pqe->queueNo] = qnum; /* Save for laters */

	insert_tcq_queries_entry(dyncat, qnum, qid, TCQ_QUERIES_ENTRY,
							 pqe->querystr->data);

	return;
}

/*
 * This function is used to record a query exit event in
 * TCQ. Info_tcq_queries.cur_query_nums data structure is used to fetch its
 * unique query number.
 *
 * queueNo (IN)   Identifier of the query's output queue
 *
 * RETURNS		  None
 *
 */
void
tcq_queries_exit(uint16 queueNo)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_QUERIES]);
	Info_tcq_queries *pinfo = &(dyncat->catinfo.tcq_queries);

	uint32		qnum;
	char	   *qstr = NULL;

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	qnum = pinfo->cur_query_nums[queueNo];

	insert_tcq_queries_entry(dyncat, qnum, queueNo, TCQ_QUERIES_EXIT, qstr);

	return;
}

/*
 *	This function is a workhorse that gathers up arguments, gets the current
 *	timestamp, and makes 'em all into Datums. It then calls the
 *	IngresQInsertTuple method to actually create a new HeapTuple and insert it
 *	into the ingresQ.
 *
 *	dyncat	  (IN)	Pointer to the DynCatInfo structure for tcq_queries
 *	opnum	  (IN)	The operator's unique operator number
 *	opid	  (IN)	The operator's operator number
 *	k		  (IN)	A character describing the "kind" of this entry
 *	qid		  (IN)	The queueid corresponding to the query related to this event
 *	opstr	  (IN)	A (possibly NULL) representing an operator
 *
 *	RETURNS			None
 */
static void
insert_tcq_operators_entry(DynCatInfo * dyncat, uint32 opnum, uint32 opid,
						 uint16 numqrs, char *k, uint32 qid, char *opstr,
						   char *opkind, char *opdesc)
{
	Timestamp			timestamp = GetCurrentTimestamp();
	Datum				opstring  = CStringGetDatum(opstr);
	Datum				kstring   = CStringGetDatum(k);
	Datum				opkstring = CStringGetDatum(opkind);
	Datum				opdescing = CStringGetDatum(opdesc);
	Datum				values[Natts_tcq_operators];
	MemoryContext 		oldctx 	  = MemoryContextSwitchTo(dyncat->perTupleContext);

	values[Anum_tcq_operators_tcqtime] = PointerGetDatum(&timestamp);
	values[Anum_tcq_operators_opnum] = UInt32GetDatum(opnum);
	values[Anum_tcq_operators_opid] = UInt32GetDatum(opid);
	values[Anum_tcq_operators_numqrs] = UInt32GetDatum(numqrs);
	values[Anum_tcq_operators_kind] = DirectFunctionCall1(textin, kstring);
	values[Anum_tcq_operators_qid] = UInt32GetDatum(qid);
	values[Anum_tcq_operators_opkind] = DirectFunctionCall1(textin, opkstring);

	if (opstr == NULL)
	{
		values[Anum_tcq_operators_opstr] = PointerGetDatum(NULL);
		dyncat->nulls[Anum_tcq_operators_opstr] = 'n';
	}
	else
	{
		values[Anum_tcq_operators_opstr] = DirectFunctionCall1(textin, opstring);
		dyncat->nulls[Anum_tcq_operators_opstr] = ' ';
	}
	if (opdesc == NULL)
	{
		values[Anum_tcq_operators_opdesc] = PointerGetDatum(NULL);
		dyncat->nulls[Anum_tcq_operators_opdesc] = 'n';
	}
	else
	{
		values[Anum_tcq_operators_opdesc] = DirectFunctionCall1(textin, opdescing);
		dyncat->nulls[Anum_tcq_operators_opdesc] = ' ';
	}
	IngresQInsertTuple(dyncat->sinfo->inQ, dyncat->tdesc, values, dyncat->nulls);
	MemoryContextReset(dyncat->perTupleContext);
	MemoryContextSwitchTo(oldctx);	

	return;
}


/*
 * This function is used to record the addition of an operator in TCQ. The entry
 * of a new operator to the TCQ is a special case of addition and is recognized
 * as such by this function. This is done because we keep track of the number of
 * queries this operator participates in, in the
 * Info.tcq.operators.optr_num_queries array. If the value there is 0 then this
 * is a new operator and the event must be marked appropriately. Of course, this
 * data structure must be appropriately updated in the tcq_operators_remove
 * function as well.
 *
 * Assumptions:
 *
 *	   This function should be called only once for every query that the
 *	   operator serves. So if the operator serves a given query more than once
 *	   (say the query has predicates (R.a > 5) and (R.a < 25) this gets recorded
 *	   as only serving one query.
 *
 * node    (IN)  Pointer to the Fjord node for this operator
 * qid	   (IN)  The queueid for the query that caused this addition
 *
 * RETURNS		 None
 */
void
tcq_operators_add(Fjord * node, Eddy * eddy, uint32 qid)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_OPERATORS]);
	Info_tcq_operators *pinfo = &(dyncat->catinfo.tcq_operators);
	char	   *opstr = nodeToString(node);
	uint32		opnum;
	uint32		opid;
	uint16		numqrs;
	char	   *kind;
	char	   *opkind;
	char	   *opdesc;

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	if (node->dynCat)			/* Make the dyncatalog more opaque to
								 * itself */
		return;

	opid = node->operator;		/* Collect the operator id */
	kind = (pinfo->num_queries[opid] == 0) ? TCQ_OPTRS_ENTRY : TCQ_OPTRS_ADD;
	numqrs = ++pinfo->num_queries[opid];
	opnum = ++pinfo->max_optr_num;		/* Generate next unique query
										 * number  */
	pinfo->cur_optr_nums[opid] = opnum; /* Save for laters */

	switch (nodeTag(node))
	{
		case T_ScanModule:
			opkind = TCQ_OPTRS_SCANMODULE;
			opdesc = NULL;
			break;
		case T_GSFilter:
			opkind = TCQ_OPTRS_GSFILTER;
			opdesc = NULL;
			break;
		case T_FSteM:
			opkind = TCQ_OPTRS_FSTEM;
			opdesc = getNodeInformation(eddy,
							  ((FSteM *) node)->protostem->innerhashkey);
			break;
		default:
			opkind = TCQ_OPTRS_UNKNOWN;
			opdesc = NULL;
			break;
	}

	insert_tcq_operators_entry(dyncat, opnum, opid, numqrs, kind, qid,
							   opstr, opkind, opdesc);
	return;
}

/*
 * This function is used to record the removal of a query (an 'R' event) from an
 * operator in TCQ. When the last query associated with the operator is removed
 * the operator itself is removed (an 'X' event). The event is generated as a
 * tuple in the tcq_operators dynamic catalog stream.
 *
 * node    (IN)  Pointer to the Fjord node for this operator
 * qid	   (IN)  The queueid for the query that caused this removal
 *
 * RETURNS		 None
 */
void
tcq_operators_remove(Fjord * node, uint32 qid)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_OPERATORS]);
	Info_tcq_operators *pinfo = &(dyncat->catinfo.tcq_operators);
	uint32		opnum;
	uint32		opid;
	uint16		numqrs;
	char	   *kind;
	char	   *opstr = NULL;
	char	   *opkind = TCQ_OPTRS_UNKNOWN;
	char	   *opdesc = NULL;

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	if (node->dynCat)			/* Make the dyncatalog more opaque to
								 * itself */
		return;

	opid = node->operator;		/* Collect the operator id */
	numqrs = --pinfo->num_queries[opid];
	kind = (pinfo->num_queries[opid] == 0) ? TCQ_OPTRS_EXIT : TCQ_OPTRS_REMOVE;
	opnum = pinfo->cur_optr_nums[opid];
	insert_tcq_operators_entry(dyncat, opnum, opid, numqrs, kind, qid,
							   opstr, opkind, opdesc);
}

/*
 *	This function is a workhorse that gathers up arguments, gets the current
 *	timestamp, and makes 'em all into Datums. It then calls the
 *	IngresQInsertTuple method to actually create a new HeapTuple and insert it
 *	into the ingresQ.
 *
 *	dyncat	  (IN)	Pointer to the DynCatInfo structure for tcq_queries
 *	opid	  (IN)	The operator's operator number
 *	qk		  (IN)	A string describing the "kind" of this queue (input/output)
 *	k		  (IN)	A string describing the "kind" of this entry
 *
 *	RETURNS			None
 */
static void
insert_tcq_queues_entry(DynCatInfo * dyncat, uint32 opid, char *qk, char *k)
{
	Timestamp			timestamp = GetCurrentTimestamp();
	Datum				qkstring  = CStringGetDatum(qk);
	Datum				kstring   = CStringGetDatum(k);
	Datum				values[Natts_tcq_queues];
	MemoryContext 		oldctx 	  = MemoryContextSwitchTo(dyncat->perTupleContext);

	values[Anum_tcq_queues_tcqtime] = PointerGetDatum(&timestamp);
	values[Anum_tcq_queues_opid] = UInt32GetDatum(opid);
	values[Anum_tcq_queues_qkind] = DirectFunctionCall1(textin, qkstring);
	values[Anum_tcq_queues_kind] = DirectFunctionCall1(textin, kstring);

	IngresQInsertTuple(dyncat->sinfo->inQ, dyncat->tdesc, values, dyncat->nulls);
	MemoryContextReset(dyncat->perTupleContext);
	MemoryContextSwitchTo(oldctx);	

	return;
}

/*
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 *	 Note that you have to be VERY careful when using these (tcq_queues_*)
 *	 functions. They cause the generation of new tuples in the tcq_queues
 *	 dynamic catalog stream. If the operators that are used in processing
 *	 queries over the tcq_queues stream also generate events by calling this
 *	 function, things can get very bad very fast.
 *
 *	 I am NOT responsible if smoke starts coming out of your server !
 *
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 */

/*
 * This function is used to record a successful enqueue event in TCQ. The event
 * is recorded as a tuple in the tcq_queues dynamic catalog stream.
 *
 * opid    (IN)  Operator that owns the queue
 * qk	   (IN)  Identifies whether the queue is an input/output queue
 *
 * RETURNS
 */
void
tcq_queues_enqueue(uint32 opid, char *qk, char *k)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_QUEUES]);

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	insert_tcq_queues_entry(dyncat, opid, qk, k);

	return;
}

/*
 * This function is used to record a successful dequeue event in TCQ. The event
 * is recorded as a tuple in the tcq_queues dynamic catalog stream.
 *
 * opid    (IN)  Operator that owns the queue
 * qk	   (IN)  Identifies whether the queue is an input/output queue
 *
 * RETURNS		 None
 */
void
tcq_queues_dequeue(uint32 opid, char *qk, char *k)
{
	DynCatInfo *dyncat = &(tcq_dyncats[TCQ_QUEUES]);

	CHECK_DYNAMIC_CATALOGS;		/* tcq_queries catalog stream inactive */

	insert_tcq_queues_entry(dyncat, opid, qk, k);

	return;
}
