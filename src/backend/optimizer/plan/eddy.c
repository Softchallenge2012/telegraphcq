/*-------------------------------------------------------------------------
 *
 * eddy.c
 *
 *	  Routines specific to Eddy plan management
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * This is related to the new feature:							  @dcacqSK
 *
 * External functions:
 *
 *	 add_plan_to_cq_eddy
 *
 *	mapping vars to arrays
 *
 *	sourceGetVarNo()
 *	varNoGetSourceArrayIndex()
 *
 * Internal functions:
 *
 *	 add_planitem_to_eddy
 *	 add_filter_to_matching_gsf
 *	 add_stem_to_matching_fstem
 *	 create_gsf_in_eddy
 *	 add_query_to_gsf
 *	 add_windows_to_eddy
 *	 AdjustScan
 *	 adjust_scan_and_rt
 *	 makeGSFilter
 *	 find_matching_fstem
 *	 LookupOpersForGSF
 *	 LookupOperForGSF
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/eddy.c,v 1.58 2005/12/14 20:49:23 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "miscadmin.h"

#include "catalog/pg_type.h"
#include "executor/executor.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFAgg.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFSteM.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeStrmScan.h"
#include "executor/nodeSteM.h"
#include "executor/nodeGSFilter.h"
#include "executor/nodeFFilter.h"
#include "executor/staticplan.h"
#include "nodes/plannodes.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "nodes/nodes.h"
#include "nodes/print.h"
#include "optimizer/clauses.h"
#include "optimizer/eddy.h"
#include "optimizer/var.h"
#include "optimizer/bindings.h"
#include "parser/parsetree.h"
#include "parser/parse_oper.h"
#include "parser/parse_node.h"
#include "rewrite/rewriteManip.h"
#include "rqueue/rqdest.h"
#include "telegraphcq/dyncatalog.h"
#include "telegraphcq/window.h"
#include "utils/lsyscache.h"
#include "utils/fcache.h"
#include "utils/relcache.h"
#include "utils/syscache.h"

static void adjust_binding_tlists(Eddy * e, int *srcidx, int *destidx, int size);
static void AdjustScan(Plan *s, int origrteidx, int eddyrteidx);
bool add_filter_nonGSF(Eddy * cur, Filter * filter, int32 queryid);
static bool  add_filter_to_matching_gsf(Eddy * cur, Filter * filter,
						   int32 queryid);
static void add_stem_to_matching_fstem(Eddy * cur, SteM * stem, int32 queryid,
						   WindowClause * new_wins);
static void adjust_scan_and_rt(Eddy * cur, Eddy * fresh, Query *qtree, int queryid);
static void add_query_to_gsf(GSFilter * gsf, Oper *o, Const *c, int32 queryid);
static void add_windows_to_eddy(Eddy * eddy, WindowClause * wc, int32 queryid);
static FSteM *find_matching_fstem(Eddy * cur, SteM * stem);
static GSFilter *create_gsf_in_eddy(Eddy * cur, Node *node, Const *c);
static GSFilter *makeGSFilter(Node *node, Oid consttype);
static void LookupOpersForGSF(GSFilter * gsf);
static Oper *LookupOperForGSF(char *opchar, GSFilter * gsf, bool forInsert);
static void remove_source_from_rt(Eddy * e, int rteidx);
void		add_index_stem(Eddy * cur, IndexSteM * module, int queryid);
void		setUpDependentModules(Eddy * e, int operatorid, Node *expr);
static void ProcessMarkDynCatalog(Fjord * node, int rtidx, Eddy * eddy);
static void AdjustScanBindings(StrmScan * strmscan, int *origrteidx,
				   int *eddyrteidx, int size);
static void sanity_check_rt_sources(Eddy * e);
static int	get_num_active_modules(Eddy * e);
static int	get_num_active_sources(Eddy * e);

static MetaOperator* GetNewMetaOp(Word_t signature,MOClass class);
static void UpdateMetaOperators(Eddy *e,BitArray sources,int opid,
								MOClass class,bool beg);
static void UpdateMetaOperatorsRemove(Eddy *cur, int opid);
static void recursive_propagate_jg_in_metaops(Eddy *cur, Pvoid_t joins,
											  BitArray sources, int start);
static void propagate_join_graph_in_metaops(Eddy *cur, Pvoid_t joins,BitArray);
static void add_output_to_eddy(Eddy *cur, BitArray sources, int queryid);
static void PrintMetaOperatorTransitions(Eddy *cur);

/*
 * BEGIN SIGMOD04 GROSSNESS
 *
 */
static void form_static_plan_0(Eddy *cur, Query *query);
static void form_static_plan_1(Eddy *cur, Query *query);
static void form_static_plan_2(Eddy *cur, Query *query);
static void form_static_plan_3(Eddy *cur, Query *query);
static void form_static_plan_4(Eddy *cur, Query *query);
static void form_static_plan_5(Eddy *cur, Query *query);
static void form_static_plan_6(Eddy *cur, Query *query);
static void form_static_plan_7(Eddy *cur, Query *query);

StaticPlan MyStaticPlans[8] = { { form_static_plan_0, 3 },
								{ form_static_plan_1, 2 },
								{ form_static_plan_2, 3 },
								{ form_static_plan_3, 2 },
								{ form_static_plan_4, 2 },
								{ form_static_plan_5, 2 },
								{ form_static_plan_6, 2 },
								{ form_static_plan_7, 2 } };

/*
 * END SIGMOD04 GROSSNESS
 */
static void adjust_plan(Plan  *n, int old, int new);
static void adjust_plan_node(Plan *n, int old, int new);
static void adjust_common_plan_fields(Plan *n, int old, int new);
static void adjust_agg(Agg * a, int old, int new);
static void adjust_sort(Sort * a, int old, int new);
static void adjust_group(Group *g, int old, int new);
static void adjust_eventexpr(FEventAgg * m, int old, int new);
static void adjust_eventexpr_recursively(EventExpr *eexpr, int old, int new);

static void add_fagg(Eddy * cur, FAgg * module, int32 queryid, Query *qtree);
static void add_feventagg(Eddy * cur, FEventAgg * module, int32 queryid, Query *qtree); /* - TCQ SR */

/*
 * add_plan_to_cq_eddy
 *
 * This function takes as argument "current": the current plan running in the
 * TelegraphCQ BE. This plan is allocated in the TCQ-BE's local (non-shared)
 * memory.
 *
 * The second argument is "fresh": the plan created by the PGSQL-FE's optimizer
 * in shared memory. This plan _must_ have an Eddy as the top node
 *
 * The job of this function is to stare at "new" and create appropriate Plan
 * constructs a la CACQ in the TCQ-BE's local memory.
 *
 * When this function is first called in the TCQ-BE, "current" will be NULL.
 *
 * This function MUST be called with an existing CACQ Eddy
 *
 *	cur		(IN) Currently running Eddy
 *	fresh	(IN) The new eddy plan from the PGFE
 *	freshrt (IN) The range table for the new Eddy plan from the PGFE
 *
 */
void
add_plan_to_cq_eddy(Eddy * cur, Eddy * fresh, Query *qtree, int32 queryid)
{
	int			 i;
    Pvoid_t 	 joins = NULL;	/* Join graph of fresh eddy */
	Word_t 		*Pvalue;
	Word_t 		 Index;
	Word_t 		 Rc_word;
	
	List 		*li;
	bits8 		 sources[BASIZE(TCQMAXSOURCES)];
	
	Assert(cur != NULL);
	Assert(cur->cacq);
	Assert(IsA(fresh, Eddy));

	whichquery++;                                               /* @dsigmodSK */
	
	/*
	 * Merge the sources and modules into the eddy
	 */
	adjust_scan_and_rt(cur, fresh, qtree, queryid);
	add_projection_info(cur, fresh->plan.targetlist, queryid);
	fresh->plan.targetlist = NULL;
	cur->eddystate->recursiveQueryFeedback[queryid]=qtree->recurseRelation;

	/*
	 * Now, stare at each module in the "fresh" plan and add it to the
	 * CQEddy
	 *
	 * For now, the following modules are permitted:
	 *
	 * Filter, SteM, IndexSteM
	 */
	for (i = 0; i < fresh->numModules; i++)
	{
		Plan	   *module = fresh->modules[i];

		Assert(fresh->modules[i] != NULL);
		if (IsA(module, Filter))
		{
			if (add_filter_to_matching_gsf(cur, (Filter *) module, queryid) == FALSE)
			{
				/* filter can't be shared */
				add_filter_nonGSF(cur, (Filter *) module, queryid);
			}
		}
		else if (IsA(module, SteM))
		{
			add_stem_to_matching_fstem(cur, (SteM *) module, queryid,
									   qtree->windowClause);
		}
		else if (IsA(module, IndexSteM)) 
		{
			add_index_stem(cur, (IndexSteM *) module, queryid);
		}
		else if (IsA(module, FAgg))
		{
		    add_fagg(cur, (FAgg *) module, queryid,qtree);
		}
		else if (IsA(module, FEventAgg))
		{
		    add_feventagg(cur, (FEventAgg *) module, queryid, qtree);
		}
		else
		{ 
		    elog(FATAL, "unknown module type found in fresh eddy");
		}
	}

	/*
	 * Update the eddy's "operators" mask. This is an array of bitmasks
	 * which is indexed by the sourceid.
	 *
	 * We need to do this in a separate loop because, when a stem is
	 * processed above, its dual SteM (and corresponding FSteM) might not
	 * have been processed yet.
	 *
	 */
	for (i = 0; i < fresh->numModules; i++)
	{
		Plan	   *module = fresh->modules[i];

		Assert(fresh->modules[i] != NULL);
		if (IsA(module, SteM))
		{
			SteM	   *ostem = (SteM *) module;
			FSteM	   *fdstem = find_matching_fstem(cur, ostem->dualSteM);
			FSteM	   *fstem = find_matching_fstem(cur, ostem);
			int32		source = -1;
			int			operator = -1;

			if (fstem == NULL ||
				fdstem == NULL ||
				IsA(fstem->protostem, IndexSteM))
				continue; /* must have converted to IndexSteM */ ;

			source = STEM_SOURCE((SteM *) GetRepresentativeSteMFSteM(fstem));

			/* New source */
			operator = fdstem->fjord.operator;

			/* Operator of dual */

			Assert(source < TCQMAXSOURCES);
			BitArraySetBit(cur->eddystate->operators[source], operator);
			/*
			 *  We update the join graph now for easy subsequent traversal
			 */
			JLI (Pvalue, joins, (Word_t) source);

			if ((*Pvalue) == ((Word_t) 0))					 /* New insertion */
			{
				(*Pvalue) = (Word_t) makeListi1(operator);
			}
			else 
			{
				li = (List *) (*Pvalue);
				(*Pvalue) = (Word_t) (lconsi(operator,li));
			}
		}
	}
	/*
	 *  For starters, let's print the join graph
	 */
	elog(DEBUG1,"------------------------------------------------------------");
	elog(DEBUG1,"Printing the join graph of the new query");
	Index = 0;
	JLF(Pvalue,joins,Index);
	while (Pvalue != (Word_t *) NULL)
	{
		List *l;
		elog(DEBUG1,"Source [%d] joins with ... ", (int)Index);

		li = (List *) (*Pvalue);
		foreach(l, li)
		{
			int op = lfirsti(l);
			FSteM *fs;
			int src;
			
			fs = (FSteM *) cur->modules[op];
			Assert(IsA(fs,FSteM));
			src = STEM_SOURCE((SteM *) GetRepresentativeSteMFSteM(fs));
			elog(DEBUG1, "\tOperator %d, stem on %d",op,src);
		}
		JLN(Pvalue, joins, Index);
	}
	elog(DEBUG1,"Done printing the join graph of the new query");
	elog(DEBUG1,"------------------------------------------------------------");
	/*
	 * Now we propagate the join operators in the MetaOperators chain
	 *
	 * Note that propagate_join_graph_in_metaops modifies sources so that it
	 * contains the right signature of output tuples !
	 *
	 */
/* 	if (UseOperatorPrecedence) { */
	  propagate_join_graph_in_metaops(cur,joins,sources);
/* 	} */
	if (joins != NULL) 
	{
		JLFA (Rc_word, joins);
	}
	else 
	{
		/*
		 * Unfortunately, the output signature only works if there are any joins
		 * in the query :-(
		 */
		for (i = 0; i < fresh->numSources; i++)
		{
			Plan *scan = NULL;
			int src = -1;
			
			if (fresh->sources[i] == NULL) 
			{
				continue;
			}
			scan = fresh->sources[i];
			
			if (IsA(scan, SeqScan))
			{
				src = ((SeqScan *)scan)->scanrelid - 1;
			}
			else if (IsA(scan, StrmScan))
			{
				src = ((StrmScan *)scan)->seqscan->scanrelid - 1;
			}
			else 
			{
				elog(ERROR,"Unknown type for scan [%d] ", (int)nodeTag(scan));
			}

			BitArraySetBit(sources,src);
			break;
		}
	}
	/*
	 * Handle window clauses
	 *
	 * TBD: Checking for joins is a loose hack .. we should actually only call
	 * add_windows_to_eddy if we want to check for _windowed joins_ !! (SK)
	 */
	if ((qtree->windowClause != NULL) && (joins != NULL))
	{
		add_windows_to_eddy(cur, qtree->windowClause, queryid);
	}

	/* Avoiding the creation of a per-query vestigial FOutput module - TCQ SR */
	/* add_output_to_eddy(cur, sources, queryid); */


	/*
	 *  As a sanity check, we now print our MetaOperators structure
	 */
	if (UseOperatorPrecedence)
	{
	  PrintMetaOperatorTransitions(cur);
	}

	/* new static plan dispatching method */
	if (UseStaticPlan) 
	{
		if (whichquery == MyStaticPlans[UseStaticPlanNumber].num_queries) 
		{
			elog(LOG,"Form a static plan");
			(*(MyStaticPlans[UseStaticPlanNumber].fsp_fp))(cur,qtree);
			elog(DEBUG1,"Phew .. we now print the static plan ");
			elog(DEBUG1,"------------------------------------------------------------");
			pprint(StaticEddy);
			elog(DEBUG1,"------------------------------------------------------------");
		}
	}                                                           /* @dsigmodSK */
	
}

static void
add_output_to_eddy(Eddy *cur, BitArray sources, int queryid)
{
	int i;
	int source_no;
	FOutput *fo = NULL;

	for (i = 0; i < cur->numModules; i++)
	{
		if (cur->modules[i] == NULL) 
		{
			continue;
		}
		if (IsA(cur->modules[i], FOutput))
		{
			fo = (FOutput *)(cur->modules[i]);
			if (BitArrayEqual(sources,fo->signature,TCQMAXSOURCES)) 
			{
				break;
			}
		}
	}
	if (i == cur->numModules) 
	{
		fo = makeNode(FOutput);
		add_planitem_to_eddy(cur, (Plan *)fo, EDDY_MODULE);
		elog(LOG, "add_output_to_eddy: Added an FOutput module to the CQ Eddy");
		BitArrayClone(fo->signature,sources,TCQMAXSOURCES);
		BitArrayPrintDebug(fo->signature,TCQMAXSOURCES);

		/* get all the ts_attrs for the sources on which this output's signature */
		for (source_no = 0; source_no < TCQMAXSOURCES; source_no++) 
		{
			if (BitArrayBitIsSet(fo->signature, source_no))
			{
				RangeTblEntry *rte;
				Relation	reldesc;
				TupleDesc tdesc;
												
				/* we need to find the TS attribute of this source ... */
				rte = rt_fetch(source_no + 1, cur->rangetable);
				reldesc = RelationIdGetRelation(rte->relid);
				tdesc = reldesc->rd_att;

				if (!IsAStream(reldesc->rd_rel->relkind))
				{
					fo->ts_attr[source_no] = -1;
				}
				else 
				{
                    if (NULL == tdesc->constr) {
                        elog(ERROR, "Stream has no timestamp column");
                    }
					
					fo->ts_attr[source_no] = tdesc->constr->ts_attrnum;
					
					if (fo->ts_attr[source_no] == TCQTSATTRNOP)
					{
						elog(ERROR, "No attribute with a TIMESTAMPCOLUMN contraint");
					}
				}
				
			}
		}
	}
	FjordAddQuery((Plan *) fo, queryid, (Plan *) cur);
	if (! UseTupleLineage)		   /* All tuples entering this node go to all */
	{
		BitArraySetBit(fo->candidates,queryid);
	}
	if (UseOperatorPrecedence) {
	  UpdateMetaOperators (cur,sources, ((Fjord *)fo)->operator, OUTPUTS, false);
	}
}

/*
 * propagate_join_graph_in_metaops
 *
 * This function traverses "joins",the join graph, and updates the metaoperator
 * data structures appropriately - that is, for each possible input signature
 * for a join, the appropriate join operators are inserted in the MetaOpreator
 * data structure with the correct input transitions
 *
 * cur         (IN)  Current Eddy
 * joins       (IN)  Join graph (a JudyL array of Lists)
 * sources     (OUT) A bitarray that contains the signature of output modules
 *
 */ 
static void
propagate_join_graph_in_metaops(Eddy *cur, Pvoid_t joins, BitArray sources)
{
	Word_t 		 Index = 0;
	Word_t 		*Pvalue;
	bits8   	 rsources[BASIZE(TCQMAXSOURCES)];

	BitArrayClearAll(sources,TCQMAXSOURCES);
	/*
	 *  Now it's time to update the MetaOperators based on the join graph
	 */
	JLF (Pvalue, joins, Index)
	while (Pvalue != (Word_t *) NULL)	
	{
		int i = (int) Index;

		BitArraySetBit(sources,i);

		BitArrayClearAll(rsources,TCQMAXSOURCES);
		BitArraySetBit(rsources,i);
		recursive_propagate_jg_in_metaops(cur,joins,rsources,i);
		
		JLN(Pvalue, joins, Index);
	}
}

static void
recursive_propagate_jg_in_metaops(Eddy *cur, Pvoid_t joins,
								  BitArray sources, int start)
{
	Word_t 		 Index;
	Word_t 		*Pvalue;
	List   		*ops,*ol;
	bits8  		 tsources[BASIZE(TCQMAXSOURCES)];
	
	Index = (Word_t) start;
	JLF (Pvalue, joins, Index);
		
	ops = (List *) (*Pvalue);
	
	foreach(ol, ops)
	{
		int 		 op = lfirsti(ol);
		FSteM 		*fs;
		int 		 src;
		
		fs = (FSteM *) cur->modules[op];
		Assert(IsA(fs,FSteM));
		src = STEM_SOURCE((SteM *) GetRepresentativeSteMFSteM(fs));
		
		if (! BitArrayBitIsSet(sources,src)) 
		  {
		    if (UseOperatorPrecedence) {
		      UpdateMetaOperators(cur, sources, op, PROBES_AND_FILTERS,false);
		    }
		    BitArrayClone(tsources,sources,TCQMAXSOURCES);
		    BitArraySetBit(tsources,src);
		    if (UseOperatorPrecedence) {
		      recursive_propagate_jg_in_metaops(cur,joins,tsources,src);
		    }
		}
	}
	JLN(Pvalue, joins, Index);
}

/*
 *  TBD: This only needs to be done if there is a join on a source
 */

static void
add_windows_to_eddy(Eddy * eddy, WindowClause * wc, int32 queryid)
{
	List	   *wl;
	char	   *minusopname = "-";
	Node	   *minusexpr;
	char	   *le_opname = "<";
	Node	   *le_expr;

	/*
	 * Now for each window expr we need to create an appropriate GSFilter
	 */
	foreach(wl, wc->windowExprs)
	{
		WindowExpr *we = (WindowExpr *) lfirst(wl);
		Filter	   *f;

		/*
		 * Construct an expression that evaluates:
		 *
		 * (wc->max - we->tsvar) < we->rangeint (OC: hack)
		 *
		 * Where: <wc->max> is a function that fetches the maximum timestamp
		 * from a tuple (e.g. highest timestamp of any of the base tuples
		 * that make up an intermediate tuple). <we->tsvar> is an
		 * expression that pulls a timestamp out of a tuple. <we->reltime>
		 * is the window size of the current window expression, expressed
		 * as a string (e.g. "1 second").
		 */
		minusexpr = (Node *) make_op(makeList1(makeString(minusopname)), wc->max, (Node *) we->tsvar);
		replace_opid((Oper *) ((Expr *) minusexpr)->oper);
		le_expr = (Node *) make_op(makeList1(makeString(le_opname)), minusexpr, (Node *) we->rangeint);
		replace_opid((Oper *) ((Expr *) le_expr)->oper);
		f = makeNode(Filter);
		f->filterqual = le_expr;
		elog(DEBUG1,"filterqual: %s\n",getNodeInformation(eddy,f->filterqual));
		add_filter_to_matching_gsf(eddy, f, queryid);
		pfree(f);
	}
}

/* add projection info
 * input: an adjusted targetlist for the eddy being added
 * side effect:  uses the targetlist to contruct a projection info
 *	  structure, and a tuple descriptor for the query.	These are
 *	  stored in the eddy, and should be called using remove_projection_info
 */

void
add_projection_info(Eddy * eddy, List *adjustedtlist, int32 queryid)
{
	eddy->plan.targetlist = (List *) copyObject(adjustedtlist);
	ExecAssignProjectionInfo((Plan *) eddy, &eddy->eddystate->pstate);
	eddy->eddystate->resultProjectionInfo[queryid] = eddy->eddystate->pstate.cs_ProjInfo;

	/*
	 * TBD TBD: owenc - please confirm - it's my beliefe that the second
	 * arg to ExecTypeFromTL below	should now be TRUE
	 */
	eddy->eddystate->resultTupleDescriptors[queryid] = ExecTypeFromTL(eddy->plan.targetlist, true);

	/*
	 * these are being set to NULL now and are stored in the eddy. Before
	 * ExecProject is called, the appropriate values for the tupleDesc and
	 * Projection info will be put back in place.
	 */
	eddy->eddystate->pstate.cs_ProjInfo = NULL;
	eddy->plan.targetlist = NULL;
}

void
remove_projection_info(Eddy * eddy, int32 queryid)
{
	List	   *iter;

	eddy->eddystate->pstate.cs_ProjInfo =
		eddy->eddystate->resultProjectionInfo[queryid];
	foreach(iter, eddy->eddystate->pstate.cs_ProjInfo->pi_targetlist)
		pfree(lfirst(iter));
	freeList(eddy->eddystate->pstate.cs_ProjInfo->pi_targetlist);
	eddy->eddystate->pstate.cs_ProjInfo->pi_targetlist = NULL;
	ExecFreeProjectionInfo(&eddy->eddystate->pstate);
	FreeTupleDesc(eddy->eddystate->resultTupleDescriptors[queryid]);
	eddy->eddystate->resultProjectionInfo[queryid] = NULL;
	eddy->eddystate->resultTupleDescriptors[queryid] = NULL;

}

/*
 * add_filter_to_matching_gsf
 *
 * This function tries to find a Grouped Selection Filter (GSFIlter) plan node
 * in the currently running Eddy which matches the specified filter module. The
 * module is required to be a filter. If there exists no appropriate GSFilter,
 * this function calls create_gsf_in_eddy to create such a module in the
 * currently running eddy. Once an appropriate GSFilter node has been found (or
 * created anew), this function will add the specified Filter node to it.
 *
 *	cur		(IN/OUT)  Currently running Eddy
 *	filter	(IN)	  New Filter module for which a matching GSFilter is sought
 *	queryid (IN)	  Queryid for the specified Filter module
 * NOTE: the filter *is not* a copy
 *
 *
 *  returns:
 *  TRUE if the filter has been added to some grouped filter
 *  FALSE if the evaluation of this filter cannot be shared 
 */
 bool
add_filter_to_matching_gsf(Eddy * cur, Filter * filter, int32 queryid)
{
	GSFilter   *gsf = NULL;
	Node	   *fqual = NULL;
	Expr	   *fexpr = NULL;
	Oper	   *foper = NULL;
	Node	   *fnode = NULL;
	Const	   *fconst = NULL;
	List	   *fargs = NIL;

	Node	   *fa1 = NULL;
	Node	   *fa2 = NULL;
	bool		foundgsf = false;
	int			i;
	int			operator;

	fqual = filter->filterqual;

	/*
	 * Check that the filterqual is of an acceptable form:
	 *
	 * field op constant
	 *
	 * where op is one of: <, >, <=, >=, =, <>
	 */
	Assert(IsA(fqual, Expr));
	fexpr = (Expr *) fqual;
	fargs = fexpr->args;
	if (!(fexpr->opType == OP_EXPR))
		return false;
	foper = (Oper *) fexpr->oper;
	Assert(IsA(fexpr->oper, Oper));

 
	Assert(length(fargs) == 2);
	fa1 = (Node *) nth(0, fargs);
	fa2 = (Node *) nth(1, fargs);

	if (IsA(fa2, Const))
	{
		fnode = fa1;
		fconst = (Const *) fa2;
	}
	else if (IsA(fa1, Const))
	{
		fnode = fa2;
		fconst = (Const *) fa1;
	}
	else
	{
		return false;
	}


	if(!((foper->optrtype == EQ_OPTR) || (foper->optrtype == NE_OPTR) ||
		 (foper->optrtype == LT_OPTR) || (foper->optrtype == GT_OPTR) ||
		 (foper->optrtype == LE_OPTR) || (foper->optrtype == GE_OPTR)))
	{
		return false;
	}

	/*
	 * At this point, all our operator sanity checks are done.
	 *
	 * It's time to search the Eddy for a valid GSFilter
	 */
	for (i = 0; i < cur->numModules; i++)
	{
		if (cur->modules[i] == NULL)
			continue;
		if (IsA(cur->modules[i], GSFilter))
		{
			gsf = (GSFilter *) cur->modules[i];

			/*
			 * There is a GSF per var.	All operators for the GSF are
			 * contained there.
			 *
			 * We've only looked operators for a particular type comparison,
			 * so we'll have to maintain separate GSF's for each const
			 * type for right now.	In the future we may wish to expand
			 * the GSF to lookup more operator functions.
			 *
			 */
			if (equal(fnode, gsf->node) &&
				fconst->consttype == gsf->consttype)
			{
				foundgsf = TRUE;
				break;
			}
		}
	}
	if (!foundgsf)
	{
		/*
		 * No equivalent GSFilter was found, create a brand new GSFilter
		 */
		gsf = create_gsf_in_eddy(cur, fnode, fconst);
	}

	/*
	 * Find this GSF's operator id
	 */
	operator = gsf->fjord.operator;

	setUpDependentModules(cur, operator, fnode);


	/*
	 * Now add this specified Filter in the GSFilter
	 */
	/* SIGMOD GROSENESS */
	gsf->all_exprs = lcons(copyObject(fqual), gsf->all_exprs);
	/* END GROSENESS */
	add_query_to_gsf(gsf, foper, fconst, queryid);
	FjordAddQuery((Plan *) gsf, queryid, (Plan *) cur);

	/*
	 * Add this operator to the query's completion mask
	 */
	Assert(queryid < TCQMAXQUERIES);
	BitArraySetBit(cur->eddystate->completionMask[queryid], operator);
	return true;
}


/* We can't execute the Filter in a GSF, so instead we'll create a
 * FFilter node to execute the expression. Although in principle 
 * evaluation of identical exprs could be shared, right now this is
 * not implemented.
 */

bool add_filter_nonGSF(Eddy * cur, Filter * filter, int32 queryid)
{
	int32 operator=-1;
	FFilter * f = makeNode(FFilter);
	f->filterqual=copyObject(filter->filterqual);  /* memory leak */
	add_planitem_to_eddy(cur, (Plan *) f, EDDY_MODULE);
	operator=f->fjord.operator;
	setUpDependentModules(cur, operator, f->filterqual);
	FjordAddQuery((Plan *) f, queryid, (Plan *) cur);
	BitArraySetBit(cur->eddystate->completionMask[queryid], operator);
	return TRUE;
}



extern bool GrossSigmodHack;

/*
 * add_stem_to_matching_fstem
 *
 * This function tries to find a FSteM plan node in the currently running Eddy
 * which matches the specified SteM module. If there exists no appropriate SteM,
 * this function calls create_fstem_in_eddy to create such a module in the
 * currently running eddy. Once an appropriate FSteM node has been found (or
 * created anew), this function will add the parameters of the specified stem to
 * it.
 *
 * NOTE: This function will only work properly if adjust_scan_and_rt() has
 * already been called for the new query.
 *
 *	cur		(IN/OUT)  Currently running Eddy
 *	stem	(IN)	  New SteM module for which an existing FSteM is sought
 *	queryid (IN)	  Identifies the query
 *	new_wins(IN)	  Window expressions for this query.
 *
 */
void
add_stem_to_matching_fstem(Eddy * cur, SteM * stem, int32 queryid,
						   WindowClause * new_wins)
{
	FSteM	   	*fstem 		 = NULL;
	int32		 source 	 = 0;
	int			 operator 	 = 0;
	List	   	*window_list = NIL;
	WindowExpr 	*current_we  = NULL;
	bits8		 sources[BASIZE(TCQMAXSOURCES)];
	
	Assert(IsA(stem, SteM));

	elog(DEBUG1, "add_stem_to_matching_fstem");

	fstem = find_matching_fstem(cur, stem);

	if (fstem)
		Assert(IsA(fstem, FSteM));

	if (fstem == NULL)
	{
		FSteMOuterHashkey *outerhashkey = (FSteMOuterHashkey *) palloc(sizeof(FSteMOuterHashkey));
		FSteMClauses *fstemclauses = (FSteMClauses *) palloc(sizeof(FSteMClauses));

		fstem = makeNode(FSteM);	/* Make new FSteM */
		fstem->protostem = copyObject(stem);	/* Initialize data */

		fstemclauses->stemclauses = ((SteM *) fstem->protostem)->stemclauses;
		fstemclauses->interested_queries = makeListi1(queryid);

		outerhashkey->outerhashkey = copyObject(((SteM *) fstem->protostem)->outerhashkey);
		outerhashkey->fstemclauses_list = makeList1(fstemclauses);

		fstem->fstemouterhashkeys = makeList1(outerhashkey);

		Assert(IsA(fstem, FSteM));

		add_planitem_to_eddy(cur, (Plan *) fstem, EDDY_MODULE); /* Add to Eddy */
		Assert(fstem->fjord.operator ==
			   ((Fjord *) cur->modules[fstem->fjord.operator])->operator);

		source = STEM_SOURCE(((SteM *) fstem->protostem));

		ProcessMarkDynCatalog((Fjord *) fstem, source + 1, cur);

		/*
		 * TODO: We really ought to be calling
		 * GetRepresentativeSteMFSteM() on <fstem> here, but we don't,
		 * because <fstem> is not fully initialized.
		 */

		operator = fstem->fjord.operator;

		Assert(source < TCQMAXSOURCES);
		BitArraySetBit(cur->eddystate->sourceStems[source], operator);
		/*
		 *  Place the build appropriately
		 */
		BitArrayRealClearAll(sources, TCQMAXSOURCES);
		BitArraySetBit(sources, source);
		if (UseOperatorPrecedence) {
		  UpdateMetaOperators (cur, sources, operator, BUILDS,false);
		}
	}
	else
	{
		List	   *iter1;
		bool		foundMatchingOuterHashKey = false;
		bool		foundMatchingSteMClauses = false;

		elog(DEBUG1, "Found an already existing FSTEM that will be reused");

		operator = fstem->fjord.operator;

		/** Must use lappend here since the fstem->current relies on the positioning in this list */
		foreach(iter1, fstem->fstemouterhashkeys)
		{
			if (equal(stem->outerhashkey, ((FSteMOuterHashkey *) lfirst(iter1))->outerhashkey))
			{
				/* thats good */
				List	   *fstemclauses_list = ((FSteMOuterHashkey *) lfirst(iter1))->fstemclauses_list;
				List	   *iter2;

				foundMatchingOuterHashKey = true;

				/* see if the SteMClauses also turn out to be equal */
				foreach(iter2, fstemclauses_list)
				{
					GrossSigmodHack = true;
					if (equal(stem->stemclauses, ((FSteMClauses *) lfirst(iter2))->stemclauses))
					{
						GrossSigmodHack = false;
						/* even better */
						lappendi(((FSteMClauses *) lfirst(iter2))->interested_queries, queryid);
						foundMatchingSteMClauses = true;
						break;
					}
					GrossSigmodHack = false;
				}
				GrossSigmodHack = false;

				/*
				 * not found a already existing stemclauses that match --
				 * add new stemclauses
				 */
				if (!foundMatchingSteMClauses)
				{
					FSteMClauses *fstemclauses = (FSteMClauses *) palloc(sizeof(FSteMClauses));

					fstemclauses->stemclauses = copyObject(stem->stemclauses);
					fstemclauses->interested_queries = makeListi1(queryid);

					lappend(fstemclauses_list, fstemclauses);
				}

				break;
			}
		}

		/*
		 * none of the already existing outerhashkeys match .. create a
		 * new one
		 */
		if (!foundMatchingOuterHashKey)
		{
			FSteMOuterHashkey *outerhashkey = (FSteMOuterHashkey *) palloc(sizeof(FSteMOuterHashkey));
			FSteMClauses *fstemclauses = (FSteMClauses *) palloc(sizeof(FSteMClauses));

			fstemclauses->stemclauses = copyObject(stem->stemclauses);
			fstemclauses->interested_queries = makeListi1(queryid);

			outerhashkey->outerhashkey = copyObject(stem->outerhashkey);
			outerhashkey->fstemclauses_list = makeList1(fstemclauses);

			lappend(fstem->fstemouterhashkeys, outerhashkey);
		}
	}
	FjordAddQuery((Plan *) fstem, queryid, (Plan *) cur);		/* Add queryid */

	Assert(queryid < TCQMAXQUERIES);
	BitArraySetBit(cur->eddystate->completionMask[queryid], operator);

	/*
	 * Figure out what window expression (if any) applies to the new SteM,
	 * and apply that window to the appropriate FSteM.
	 */
	if (new_wins != NULL)
	{
        foreach(window_list, new_wins->windowExprs) 
		{
			current_we = (WindowExpr *) lfirst(window_list);
			
			if (SteMGetContainsRelation(cur, stem, current_we->rtref->rtindex))
			{
				/*
				 * While we're at it, make sure that the FSteM has a way
				 * to get at the timestamp fields of its tuples.
				 */
				SetTimestampAccessorFSteM(fstem, current_we->tsvar);
				AddWindowFSteM(fstem, current_we->rangeint);
				break;
			}

		}

	}




	return;
}

/*
 * find_matching_fstem
 *
 * This helper function finds an FSteM that matches a given SteM. If no matchign
 * FSteM exists, it returns NULL
 *
 *	cur		(IN)  Currently running Eddy
 *	stem	(IN)  New SteM module for which an existing FSteM is sought
 *
 *	RETURNS		  Matching FSteM node
 */
FSteM *
find_matching_fstem(Eddy * cur, SteM * stem)
{
	int			i;
	FSteM	   *fstem = NULL;
	bool		match = false;

	for (i = 0; i < cur->numModules; i++)		/* Search modules */
	{
		if (cur->modules[i] == NULL)
			continue;
		if (IsA(cur->modules[i], FSteM))		/* Found an FSteM ? */
		{
			fstem = (FSteM *) cur->modules[i];

			if ((match = equal(stem, GetRepresentativeSteMFSteM(fstem))))
				/* Does it match? */
				break;
		}
	}
	return (match ? fstem : NULL);
}


/*
 * create_gsf_in_eddy
 *
 * This function creates a new Grouped Selection Filter (GSFilter) plan node in
 * the currently running Eddy. The new GSFilter is created using the specified
 * operator (oper) and defined to operate on the specified relation attribute
 * (var).
 *
 * Notice that the specified "var" MUST be on the left of the operator. In other
 * words, the filter is being created for predicates of the sort:
 *
 *				  (var	 oper	const)
 *
 *				  Eg 1: (R.a > 56)
 *				  Eg 2: (S.b < 67)
 *
 *	cur  (IN/OUT)	Currently running Eddy
 *	oper (IN)		Operator the GSFilter is being specified with
 *	var  (IN)		Relation attribute the GSFilter is defined as operating over
 *
 *	RETURNS			Newly created GSFilter node
 *
 */
GSFilter *
create_gsf_in_eddy(Eddy * cur, Node *node, Const *c)
{
	/* Create and initialize new GSFilter node */
	GSFilter   *gsf = makeGSFilter((Node *) node,
								   c->consttype);

	/* Add new GSFilter node as a module of the current eddy */
	add_planitem_to_eddy(cur, (Plan *) gsf, EDDY_MODULE);
	return gsf;
}


/*
 * Lookup Oper for GSF
 *
 *	GSF's break range operators that include equality apart.  The problem is
 *	that postgres includes explicit >= and <= operators as a part of its
 *	operator list.	For range operators including equality, this function
 *	replaces the oper node in the GSF with either > or <, and looks up the
 *	equality operator.
 */
void
LookupOpersForGSF(GSFilter * gsf)
{
	/* these opers are var OP const */

	if (gsf->eqoper == NULL)
		gsf->eqoper = LookupOperForGSF("=", gsf, false);
	if (gsf->neoper == NULL)
		gsf->neoper = LookupOperForGSF("<>", gsf, false);
	if (gsf->ltoper == NULL)
		gsf->ltoper = LookupOperForGSF("<", gsf, false);
	if (gsf->gtoper == NULL)
		gsf->gtoper = LookupOperForGSF(">", gsf, false);

	if (gsf->eqoperinsert == NULL)
		gsf->eqoperinsert = LookupOperForGSF("=", gsf, true);
	if (gsf->neoperinsert == NULL)
		gsf->neoperinsert = LookupOperForGSF("<>", gsf, true);
	if (gsf->ltoperinsert == NULL)
		gsf->ltoperinsert = LookupOperForGSF("<", gsf, true);
	if (gsf->gtoperinsert == NULL)
		gsf->gtoperinsert = LookupOperForGSF(">", gsf, true);
}

Oper *
LookupOperForGSF(char *opchar, GSFilter * gsf, bool forInsert)
{

	Oid			type1 = InvalidOid;
	Oid			type2 = InvalidOid;
	Oper	   *oper = NULL;


	if (forInsert)
		type1 = type2 = gsf->consttype;
	else if (IsA(gsf->node, Expr))
		type1 = type2 = ((Expr *) gsf->node)->typeOid;

	else if (IsA(gsf->node, Var))
	{

		type1 = ((Var *) gsf->node)->vartype;
		type2 = gsf->consttype;
	}

	else
		elog(ERROR, "LookupOperForGSF: Invalid GSF node type");

	oper = lookup_oper(opchar, type1, type2);
	if (oper)
		load_oper(oper);
	return oper;


}


Oper *
lookup_oper(char *opname, Oid type1, Oid type2)
{
	Operator	op;
	Oper	   *oper = NULL;

	op = compatible_oper(makeList1(makeString(opname)), type1, type2, false);
	oper = makeOper(oprid(op), InvalidOid, BOOLOID, false);
	replace_opid(oper);
	ReleaseSysCache(op);

	return oper;
}

void
load_oper(Oper *o)
{
	if (o->op_fcache == NULL)
	{
		o->op_fcache = init_fcache(o->opid, 2,
								   CurrentMemoryContext);
	}
}


/*
 * add_query_to_gsf
 *
 * This function adds a query's predicate to an appropriate GSFilter.
 *
 * ASSUMPTION: Note that this function assumes that the correct GSFilter has
 * already been found out.
 *
 * For now this function ONLY supports LT_OPTR and GT_OPTR operators
 *
 * gsf	   (IN/OUT)  Correctly chosen GSFilter node
 * c	   (IN)		 Constant of the predicate being added
 * queryid (IN)		 The queryid for the query of this predicate
 *
 */
void
add_query_to_gsf(GSFilter * gsf, Oper *oper, Const *c, int32 queryid)
{
	GSFilter   			*node 	   = gsf;
	CommonScanState 	*scanstate = node->fjord.scanstate;			
	ExprContext 		*econtext  = scanstate->cstate.cs_ExprContext;	
	

	c = (Const *) copyObject(c);
	oper = (Oper *) copyObject(oper);

	switch (oper->optrtype)
	{
		case LE_OPTR:
			gsf->ltmap = add_to_ConstTree(gsf->ltmap, c, queryid,
									 gsf->ltoperinsert->op_fcache, true);
			Assert(verifyConstTree(econtext,
							  node->ltmap, node->ltoperinsert->op_fcache,
								   NULL,
								   false) == true);
			break;
		case LT_OPTR:
			gsf->ltmap = add_to_ConstTree(gsf->ltmap, c, queryid,
									gsf->ltoperinsert->op_fcache, false);
			Assert(verifyConstTree(econtext,
							  node->ltmap, node->ltoperinsert->op_fcache,
								   NULL,
								   false) == true);
			break;
		case GE_OPTR:
			gsf->rtmap = add_to_ConstTree(gsf->rtmap, c, queryid,
									 gsf->gtoperinsert->op_fcache, true);
			Assert(verifyConstTree(econtext,
							  node->rtmap, node->gtoperinsert->op_fcache,
								   NULL,
								   false) == true);
			break;
		case GT_OPTR:
			gsf->rtmap = add_to_ConstTree(gsf->rtmap, c, queryid,
									gsf->gtoperinsert->op_fcache, false);
			Assert(verifyConstTree(econtext,
							  node->rtmap, node->gtoperinsert->op_fcache,
								   NULL,
								   false) == true);
			break;
		case EQ_OPTR:
			gsf->eqmap = add_to_list(gsf->eqmap, c, queryid,
									 gsf->eqoperinsert->op_fcache);
			break;
		case NE_OPTR:
			gsf->nemap = add_to_list(gsf->nemap, c, queryid,
									 gsf->neoperinsert->op_fcache);
			break;
		default:
			elog(ERROR, "[create_gsf] Unsupported operator type");
	}

}

/*
 * adjust_sourceBuiltOn
 *
 *	TBD TBD TBD
 */
static void
adjust_stems_sourceBuiltOn(Eddy * fresh, int *oldidx, int *newidx, int num)
{
	int			i,
				j;
	int			oldsrc = -1,
				newsrc = -1;

	for (i = 0; i < fresh->numModules; i++)
	{
		Plan	   *m = fresh->modules[i];
		bool		foundsrc = false;

		if (fresh->modules[i] == NULL)
			continue;
		if (IsA(fresh->modules[i], SteM))
		{
			SteM	   *s = (SteM *) m;

			oldsrc = STEM_SOURCE(s);
			newsrc = -1;

			for (j = 0; j < num; j++)
			{
				if (oldidx[j] == (oldsrc + 1))	/* Ugh */
				{
					newsrc = newidx[j] - 1;		/* Double Ugh */
					foundsrc = true;
					break;
				}
			}
			if (foundsrc)
			{
				Assert(newsrc != -1);
				BitArrayClearBit(s->sourceBuiltOn, oldsrc);
				BitArraySetBit(s->sourceBuiltOn, newsrc);
			}
		}
	}
}




void adjust_plan(Plan * p, int oldidx, int newidx)
{
  if(p == NULL) return;
  adjust_plan_node(p,oldidx, newidx);
  adjust_plan((Plan*)outerPlan(p), oldidx, newidx);
  adjust_plan((Plan*)innerPlan(p), oldidx, newidx);

}

void adjust_plan_node(Plan * n, int old, int new)
{

  if(n == NULL) return;


      adjust_common_plan_fields((Plan*)n, old, new);
     switch(nodeTag(n))
	{
	case T_Agg:
	  adjust_agg((Agg *)n,old, new);
	  break;
	case T_Sort:
	  adjust_sort((Sort*)n, old, new);
	  break;
	case T_Group:
	  adjust_group((Group*)n, old, new);
	  break;
	default:
	  elog(FATAL, "maladjusted mode: don't know how to adjust node");
	  break;
	}

}


void adjust_common_plan_fields(Plan *n, int old, int new)
{
   ChangeVarNodes((Node*)n->qual, old, new,0);
   ChangeVarNodes((Node*)n->targetlist, old, new,0);
}

void adjust_agg(Agg * a, int old, int new)
{
  /* nothing to be done here */
}

void adjust_sort(Sort * a, int old, int new)
{
  /* nothing needed here for now */
}

void adjust_group(Group *g, int old, int new)
{
  /* nothing needed here for now */
}

/*
 * Both the following two functions should go away,
 * as support has been added in expression_tree_walker() for event expressions
 *
 * - TCQ SR 
*/

void
adjust_eventexpr(FEventAgg * m, int old, int new)
{

  EventExpr *eexpr = (EventExpr *)m->eventExpr;
  List *var_lists = eexpr->var_lists;
  List *p_p_list_of_vars, *p_list_of_vars, *list_of_vars;
  Var *var;
  
  elog(LOG, "adjust_eventexpr: Adjusting an FEventAgg module because of range table changes during merging new eddy with CQEddy");

  /* First, deal with the partition clause */
  foreach(p_p_list_of_vars, var_lists) {
    p_list_of_vars = lfirst(p_p_list_of_vars);
    foreach(list_of_vars, p_list_of_vars) {
      var = lfirst(list_of_vars);
      if(!IsA(var, Var))
	elog(FATAL, "adjust_eventexpr: Member of Partition By clause not a Var");
      else {
	if(var->varno == old) {
	  var->varno = new;
	  var->varnoold = new;
	}
      }
    }
  }

  /* Now deal with the tree-based event expression */
  
  if(!eexpr)
    elog(FATAL, "Backend: Event expression in FEventAgg node is NULL");

  adjust_eventexpr_recursively(eexpr, old, new);
}

void
adjust_eventexpr_recursively(EventExpr *eexpr, int old, int new)
{
  if(!eexpr)
    elog(FATAL, "adjust_eventexpr_recursively: Unexpected NULL pointer in an EventExpr");
  if(eexpr->event_op == EVENT_SIMPLE) {
    if(eexpr->var->varno == old) {
      eexpr->var->varno = new;
      eexpr->var->varnoold = new;
    }
  }
  else {
    adjust_eventexpr_recursively(eexpr->lchild, old, new);
    adjust_eventexpr_recursively(eexpr->rchild, old, new);
  }
}



/*
 * adjust_modules
 *
 *	TBD TBD TBD
 */
static void
adjust_modules(Eddy * fresh, int *oldidx, int *newidx, int num)
{
	int			i,
				j;

	for (i = 0; i < num; i++)
	{
		int			origidx = oldidx[i];
		int			eddyidx = newidx[i];

		for (j = 0; j < fresh->numModules; j++) /* adjust all modules */
		{
			Plan	   *m = fresh->modules[j];
			Filter	   *f=NULL;
			SteM	   *s=NULL;

			if (m == NULL)
				continue;

			switch (nodeTag(m))
			{
				case T_Filter:
					f = (Filter *) m;
					ChangeVarNodes(f->filterqual, origidx, eddyidx, 0);
					break;
				case T_SteM:
					s = (SteM *) m;

					ChangeVarNodes(s->innerhashkey, origidx, eddyidx, 0);
					ChangeVarNodes(s->outerhashkey, origidx, eddyidx, 0);
					ChangeVarNodes((Node *) s->stemclauses, origidx, eddyidx, 0);

					break;
				case T_IndexSteM:
					/* adjustment occurs elsewhere */
					break;

			case T_FAgg:
			  adjust_plan(((FAgg*)m)->aggPlan, origidx, eddyidx);
			  break;

			case T_FEventAgg:
			  adjust_eventexpr((FEventAgg *) m, origidx, eddyidx);
			  break;

			  break;
				default:
					pprint(m);
					elog(ERROR, "adjust_modules: node type %d unsupported",
						 (int) nodeTag(m));
					break;
			}
		}
	}
}

/*
 * adjust_node
 *
 *	TBD TBD TBD
 */
static void
adjust_node(Node *node, int *oldidx, int *newidx, int num)
{
	int			i;

	for (i = 0; i < num; i++)
	{
		int			origidx = oldidx[i];
		int			eddyidx = newidx[i];

		ChangeVarNodes((Node *) node, origidx, eddyidx, 0);
	}
}

/*
 * adjust_windowclauses
 *
 */
static void
adjust_windowclauses(Eddy * fresh, Query *qtree, int *oldidx, int *newidx, int num)
{
	List	   *listelem = NIL;
	WindowExpr *we = NULL;
	int			i;

	if (qtree->windowClause == NULL)
		return;

	foreach(listelem, qtree->windowClause->windowExprs)
	{
		we = (WindowExpr *) lfirst(listelem);

		/* We MOST CERTAINLY care about fixing up this var node */
		adjust_node((Node *) we->tsvar, oldidx, newidx, num);

		/* Do we really care about fixing up the rtref stuff anyways ??? */
		for (i = 0; i < num; i++)
		{
			if (we->rtref->rtindex == oldidx[i])
			{
				we->rtref->rtindex = newidx[i];
				break;
			}
		}
	}
	/* We MOST CERTAINLY care about fixing up this Node */
	adjust_node(qtree->windowClause->max, oldidx, newidx, num);
}

#ifndef max
#define  max(a,b)	  ((a)>(b)?(a):(b))
#endif /* max */

/*																	 //@BdcacqOC
 * adjust_scan_and_rt
 *
 * This function merges an incoming query's Scan and associated range table
 * entry with the range table of the current eddy query. If a relation referred
 * to in the incoming query (in the SeqScan) is not already present in the
 * eddy's rangetable, then this function will add the range table entry of that
 * relation in the queryrt to that of the eddy. Further, it will clone the
 * SeqScan node into the Eddy (if it doesn't exist) and modify it so that it
 * refers to the relation through a new relid that is valid in the eddy's range
 * table. Note that the relation may be present in the running eddy's rangetable
 * and still there may not be a corresponding Scan node in the Eddy (it might
 * have been removed).
 *
 * A SeqScan contain a relid field that is an index into the query's range
 * table.  When a query is added to a eddy, the range table of the new query
 * will need to be merged with that of the eddy.  As a result all references
 * into the rangetable in the scan must be modified as well.
 *
 * 1. grab the relid from the scan and look up its OID in the query rt.
 *
 * 2. check to see if the rte for the scan is already a part of the eddy rt.
 *	  if so, adjust using that index.
 *
 * 3. otherwise copy the rte from the queryrt, and make it a new entry
 *
 * 4. Check if a SeqScan node for that is already present in the current
 *	  Eddy. If it is not, clone the SeqScan and adjust var nodes and the scan
 *	  (vars don't necessarily need to be adjusted because execevalvar uses
 *	  only INNER,  * OUTER, or scan tuple,but this is useful for comparing vars)
 *
 *	eddy	 (IN/OUT)  Currently running Eddy which has to be modified
 *	s		 (IN)	   New scan node which might be cloned and added to the eddy
 *	modules  (IN)	   New module array which might be cloned and added to eddy
 *	numModules (IN)    Number of new modules
 *	queryrt  (IN)	   Query range table which is pointed into by the node "s"
 *  queryid  (IN)      Identifier of the fresh query
 *
 */
void
adjust_scan_and_rt(Eddy * cur, Eddy * fresh, Query *qtree, int queryid)
{
	List	   *listelem = NIL;
	RangeTblEntry *rte = NULL;
	List	   *queryrt = qtree->rtable;
	List	   *scans_to_adjust = NIL;
	List	   *iter = NIL;

	Plan	  **sources = fresh->sources;
	int			numSources = fresh->numSources;
	int			i,
				j;

	int		   *origidx;
	int		   *eddyidx;
	int		   *tempidx;
	int			nchanges = 0;
	int			unusedidx = -1;
	int			tbaseidx;

	origidx = (int *) palloc(sizeof(int) * numSources);
	tempidx = (int *) palloc(sizeof(int) * numSources);
	eddyidx = (int *) palloc(sizeof(int) * numSources);

	/*
	 * Before we begin let us set our transformation base index
     *
     * This index needs to be something guaranteed to be higher than the
     * highest entry in the range table.
     *
     * We add a safety factor, just in case ;)
     *
     * TODO: Verify at runtime that this number is high enough.
	 */
    tbaseidx = TCQMAXSOURCES + 1000000;

	for (i = 0; i < numSources; i++)
	{
		Plan	   *scan = (Plan *) sources[i];
		Plan	   *scancopy = NULL;
		int			origrteidx = -1;
		RangeTblEntry *queryrte = NULL;
		Oid			scanoid = InvalidOid;
		int			eddyrteidx = 0;
		int			idx = 1;
		ScanModule *scanmodule = NULL;
		bool		sourceExists = false;


		Assert(IsA(scan, SeqScan) ||IsA(scan, StrmScan));

		if (IsA(scan, SeqScan))
		{
			SeqScan    *seqscan = (SeqScan *) scan;

			origrteidx = seqscan->scanrelid;

		}
		else if (IsA(scan, StrmScan))
		{
			StrmScan   *strmscan = (StrmScan *) scan;

			origrteidx = strmscan->seqscan->scanrelid;

		}
		queryrte = rt_fetch(origrteidx, queryrt);
		scanoid = queryrte->relid;

		unusedidx = -1;


		foreach(listelem, cur->rangetable)
		{
			rte = (RangeTblEntry *) lfirst(listelem);
			if (rte->relid == scanoid)
			{
				eddyrteidx = idx;
				break;
			}
			if (unusedidx < 0 &&
				rte->relid == InvalidOid)
				unusedidx = idx;
			rte = NULL;

			idx++;
		}

		if (rte == NULL)
		{
			/*
			 * The necessary rte is not already in eddy rangetable
			 */
			RangeTblEntry *newrte = copyObject(queryrte);

			if (unusedidx > 0)
			{
				RangeTblEntry *entry;

				elog(DEBUG1, "asrt: using old rteno %d", unusedidx);
				entry = rt_fetch(unusedidx, cur->rangetable);
				Assert(entry->relid == InvalidOid);
				pfree(entry);
				rt_store(unusedidx, cur->rangetable, newrte);
				eddyrteidx = unusedidx;
				unusedidx = -1;
			}
			else
			{
				cur->rangetable = lappend(cur->rangetable, newrte);
				eddyrteidx = idx++;
			}
		}
	

		for (j = 0; j < cur->numSources; j++)
		{
			if (cur->sources[j] == NULL)
				continue;

			if (((ScanModule *) cur->sources[j])->fjord.operator == eddyrteidx)
			{
				sourceExists = true;
				break;
			}
		}

		if (IsA(scan, SeqScan))
		{
			((SeqScan *)scan)->scanrelid = eddyrteidx;
		}
		else if (IsA(scan, StrmScan))
		{
			((StrmScan *)scan)->seqscan->scanrelid = eddyrteidx;
		}
		else 
		{
			elog(ERROR,"Unknown type for scan [%d] ", (int)nodeTag(scan));
		}
		
		if (!sourceExists)
		{
			scancopy = (Plan *) copyObject(scan);
			AdjustScan(scancopy, origrteidx, eddyrteidx);
			if (IsA(scancopy, StrmScan) &&
				((StrmScan *) scancopy)->bindings &&
				((StrmScan *) scancopy)->useindex)
			{
				IndexSteM  *iscan =
				strmscan_to_indexstem((StrmScan *) scancopy);

				if (iscan != NULL)
				{
					scancopy = NULL;
					add_planitem_to_eddy(fresh, (Plan *) iscan,EDDY_MODULE);
				}

				/*
				 * scans with bindings that are all constant are treated
				 * as scans.  Their probe tuple will be seeded in the scan
				 * module initialization
				 */

			}

			if (scancopy)
			{

				cur->plan.state->es_range_table = cur->rangetable;
				scanmodule = makeNode(ScanModule);
				scanmodule->nodup=qtree->recurseRelation != InvalidOid;
				scanmodule->scan = scancopy;
				/* Add to Eddy */
				scans_to_adjust = lappend(scans_to_adjust, scanmodule);
				ProcessMarkDynCatalog((Fjord *) scanmodule,
							   sourceGetVarNo((Plan *) scanmodule->scan),cur);
				add_planitem_to_eddy(cur, (Plan *) scanmodule, EDDY_SOURCE);
				scanmodule->fjord.operator = eddyrteidx;
				elog(DEBUG1, "adding new scanmodule with rteidx=%d",
					 scanmodule->fjord.operator);
				Assert(sourceGetVarNo(((ScanModule *) cur->sources[eddyrteidx - 1])->scan) == eddyrteidx);

				FjordAddQuery((Plan *) scanmodule, queryid, (Plan *) cur);
				BitArraySetBit(cur->eddystate->sources[queryid],
							   eddyrteidx-1);
				if(IsA(scanmodule->scan, StrmScan))
				  {
				    StrmScan * tmp = (StrmScan*)scanmodule->scan;
				    if(tmp->bindings != NULL)
				      cur->eddystate->numsourceswithbindings++;
				  }	
			}


		}
		else
		{
			ScanModule *sm = NULL;
			int			i = 0;

			for (i = 0; i < cur->numSources; i++)
			{
				if (cur->sources[i] == NULL)
					continue;

				sm = (ScanModule *) cur->sources[i];
				if (sm->fjord.operator == eddyrteidx)
				{
					FjordAddQuery(cur->sources[i], queryid, (Plan *) cur);
					elog(DEBUG1, " found an existing source in the eddy.  "
						 "Query id %d will reuse it", queryid);
					BitArraySetBit(cur->eddystate->sources[queryid],
						       eddyrteidx-1);
					if(IsA(scan, StrmScan))
					  {
					    StrmScan * fstrm = (StrmScan*) scan;
					    StrmScan * cstrm = (StrmScan*) sm->scan;

					    if(fstrm->bindings != NULL)
					      {
						if(cstrm->bindings == NULL 
						   || ((strmscan_has_var_bindings(cstrm) == false )))
						  {
						    cstrm->bindings = _copyBindingInfos(fstrm->bindings, fstrm->numwrappers);
						    cstrm->numwrappers=fstrm->numwrappers;
						    if (strmscan_has_var_bindings(cstrm))
						      {
							sm->sourcehasvarbindings =true;
						      }
						    scans_to_adjust = lappend(scans_to_adjust, sm);
						    StrmScanInitBindingInfo(cstrm);
						    if(cstrm->bindings != NULL)
						    {
						      cur->eddystate->numsourceswithbindings++;
						    }
						  }
						
						else
						  elog(FATAL, " this is not allowed");
					      }
					  }
				}
			}
		}
	
		if (origrteidx != eddyrteidx)	/* Do we readjust modules ? */
		{
			origidx[nchanges] = origrteidx;
			tempidx[nchanges] = tbaseidx++;		/* Bump up the trans. base
												 * index */
			eddyidx[nchanges] = eddyrteidx;
			nchanges++;
		}
	}
	if (nchanges > 0)
	{
		adjust_modules(fresh, origidx, tempidx, nchanges);
		adjust_modules(fresh, tempidx, eddyidx, nchanges);
		adjust_binding_tlists(fresh, origidx, tempidx, nchanges);
		adjust_binding_tlists(fresh, tempidx, eddyidx, nchanges);
		adjust_stems_sourceBuiltOn(fresh, origidx, eddyidx, nchanges);
		adjust_node((Node *) fresh->plan.targetlist, origidx, tempidx, nchanges);
		adjust_node((Node *) fresh->plan.targetlist, tempidx, eddyidx, nchanges);
		adjust_windowclauses(fresh, qtree, origidx, tempidx, nchanges);
		adjust_windowclauses(fresh, qtree, tempidx, eddyidx, nchanges);
		foreach(iter, scans_to_adjust)
		{
			ScanModule *sm = lfirst(iter);
			StrmScan   *ss = (StrmScan *) sm->scan;

			if (!IsA(ss, StrmScan))
				continue;

			AdjustScanBindings(ss, origidx, tempidx, nchanges);
			AdjustScanBindings(ss, tempidx, eddyidx, nchanges);
		}

	}

	pfree(origidx);
	pfree(tempidx);
	pfree(eddyidx);

}	/* @EdcacqOC */


static void
adjust_binding_tlists(Eddy * e, int *srcidx, int *destidx, int size)
{
	int			modulenum = 0;



	Plan	   *module = NULL;
	StrmScan   *strmscan = NULL;


	for (modulenum = 0; modulenum < e->numModules; modulenum++)
	{
		module = e->modules[modulenum];
		if (module == NULL)
			continue;

		if (IsA(module, IndexSteM))
		{
			Plan	   *p = &(((IndexSteM *) module)->join.plan);

			strmscan = (StrmScan *) innerPlan(p);
			if (!IsA(strmscan, StrmScan))
				continue;
			AdjustScanBindings(strmscan,
							   srcidx,
							   destidx,
							   size);

		}

	}



}



static void
AdjustScanBindings(StrmScan * strmscan,
				   int *srcidx,
				   int *destidx,
				   int size)
{
	int			wrappernum = 0;
	int			i = 0;


	if (strmscan->bindings == NULL)
		return;

	for (wrappernum = 0; wrappernum < strmscan->numwrappers; wrappernum++)
	{
		if (strmscan->bindings[wrappernum])
		{
			for (i = 0; i < size; i++)
			{
				ChangeVarNodes((Node *)
							   strmscan->bindings[wrappernum]->targetlist,
							   srcidx[i], destidx[i], 0);
			}
		}
	}
}

static void
AdjustScan(Plan *s, int origrteidx, int eddyrteidx)
{
	SeqScan    *sscan = NULL;
	StrmScan   *strmscan = NULL;

	switch (nodeTag(s))
	{
		case T_SeqScan:
			sscan = (SeqScan *) s;
			ChangeVarNodes((Node *) sscan->plan.targetlist,
						   origrteidx, eddyrteidx, 0);
			ChangeVarNodes((Node *) sscan->plan.qual, origrteidx, eddyrteidx, 0);
			sscan->dontProject = true;
			sscan->scanrelid = eddyrteidx;
			break;
		case T_StrmScan:
			strmscan = (StrmScan *) s;
            Assert(strmscan->seqscan != NULL);
			strmscan->seqscan->dontProject = true;
			strmscan->rtindex = strmscan->seqscan->scanrelid = eddyrteidx;


			ChangeVarNodes((Node *) strmscan->scansock.plan.targetlist,
						   origrteidx, eddyrteidx, 0);
			ChangeVarNodes((Node *) strmscan->scansock.plan.qual, origrteidx, eddyrteidx, 0);
			ChangeVarNodes((Node *) strmscan->seqscan->plan.targetlist,
						   origrteidx, eddyrteidx, 0);
			ChangeVarNodes((Node *) strmscan->seqscan->plan.qual, origrteidx, eddyrteidx, 0);
			break;
		default:
			elog(ERROR, "adjust scan got unsupported scan type %d", nodeTag(s));
			break;
	}
}

void
remove_query(Eddy * node, int queryid)
{
	int			i = 0;
	int			numSources = node->numSources;
	int			numModules = node->numModules;

	Node * smscan;
	elog(DEBUG1, "eddy will now remove queryid %d", queryid);
	for (i = 0; i < numModules; i++)
	{
		if (node->modules[i] == NULL)
			continue;
		if (ProcessRemoveQuery(node->modules[i], queryid, (Plan *) node))
		{
			elog(NOTICE, "removing module %d from the eddy", i);
			if(IsA(node->modules[i], FEventAgg)) /* - TCQ SR */
			  {
			    /* Sanity check */
			    if(!BitArrayBitIsSet(node->eddystate->eventOperators, ((Fjord *)node->modules[i])->operator))
			      elog(FATAL, "remove_query: Something is wrong.... eventOperator bit was not sent for this Event Operator");

			    BitArrayClearBit(node->eddystate->eventOperators, ((Fjord *)node->modules[i])->operator);

			  }
			ProcessEndNode(node->modules[i], (Plan *) node);
			pfree(node->modules[i]);
			node->modules[i] = NULL;
			elog(DEBUG1,"Removing module %d from MetaOperators", i);
			if (UseOperatorPrecedence) {
			  UpdateMetaOperatorsRemove(node,i);
			  elog(DEBUG1,"Current state of MetaOperators after removal");
			  PrintMetaOperatorTransitions(node);
			}
		}
	}

	for (i = 0; i < numSources; i++)
	{
		if (node->sources[i] == NULL)
			continue;
		if (ProcessRemoveQuery(node->sources[i], queryid, (Plan *) node))
		{
			int			operator = ((Fjord *) node->sources[i])->operator;
			if(IsA(node->sources[i], ScanModule))
			  {
			    smscan = (Node*)((ScanModule*)node->sources[i])->scan;
			    if(IsA(smscan, StrmScan))
			      {
				StrmScan * tmp = (StrmScan*)smscan;
				if(tmp->bindings != NULL)
				  node->eddystate->numsourceswithbindings--;
			      } 
			  }

			ProcessEndNode(node->sources[i], (Plan *) node);
			remove_source_from_rt(node, operator);
			pfree(node->sources[i]);
			node->sources[i] = NULL;
			BitArrayClearAll(node->eddystate->operators[i], TCQMAXOPERATORS);
			BitArrayClearAll(node->eddystate->sourceStems[i], TCQMAXOPERATORS);

		}
	}

	BitArrayRealClearAll(node->eddystate->completionMask[queryid],
						 TCQMAXOPERATORS);
	BitArrayRealClearAll(node->eddystate->sources[queryid],
						 TCQMAXSOURCES);
	node->eddystate->queryIds = lremovei(queryid, node->eddystate->queryIds);
	remove_projection_info(node, queryid);
	clear_queue(GetQueue(queryid));
	FreeQueue(queryid);
	tcq_queries_exit((uint16) queryid);
	node->eddystate->numqueries--;
	node->eddystate->recursiveQueryFeedback[queryid]=InvalidOid;
	sanity_check_rt_sources(node);
}

void
remove_source_from_rt(Eddy * e, int rteidx)
{
	RangeTblEntry *rte = rt_fetch(rteidx, e->rangetable);

	elog(NOTICE, "freeing range table entry %d, with oid %d", rteidx, rte->relid);
	rte->relid = InvalidOid;
}



/*
 * add_planitem_to_eddy
 *
 * This is a general utility function which is used to add a new Plan item to
 * one of the Plan arrays of the Eddy. At this time, the Eddy has only two kinds
 * of Plan arrays: Sources and Modules.
 *
 * The specific plan array that has to be augmented is indicated with the "item"
 * enum.
 *
 *	cur		(IN/OUT) Currently running Eddy which has to be modified
 *	newplan (IN)	 New plan item which has to be added to an Eddy array
 *	item	(IN)	 An enum to indicate which plan array has to be augmented
 *
 */
void
add_planitem_to_eddy(Eddy * cur, Plan *newplan, EddyItem item)
{
	Plan	 ***parray = NULL;
	Plan	  **oldarray = NULL;
	int		   *eddyarraysize = NULL;
	int			idx = 0;

	if (MyProcessType == PROC_TCQ_BACKEND)
		Assert(IsA_Fjord(newplan));

	switch (item)
	{
		case EDDY_SOURCE:
			parray = &(cur->sources);
			for (idx = 0; idx < cur->numSources; idx++)
			{
				if (cur->sources[idx] == NULL) 
				{
					break;
				}
			}

			eddyarraysize = &cur->numSources;

			Assert((get_num_active_sources(cur) + 1) <= TCQMAXSOURCES);

			if (MyProcessType != PROC_TCQ_BACKEND) 
			{
				Assert(IsA(newplan, SeqScan) ||IsA(newplan, StrmScan));
			}
			break;

		case EDDY_MODULE:
			parray = &(cur->modules);
			for (idx = 0; idx < cur->numModules; idx++)
			{
				if (cur->modules[idx] == NULL)
					break;
			}
			eddyarraysize = &cur->numModules;

			Assert((get_num_active_modules(cur) + 1) <= TCQMAXOPERATORS);

			break;
		default:
			elog(ERROR, "[add_plan_to_eddy] Unknown Eddy item");
	}

	/*
	 * Reallocate the plan array
	 */
	if (IsA_Fjord(newplan))
	{
		((Fjord *) newplan)->operator = idx;	/* Set operator-id of
												 * module */
	}

	if (idx == *eddyarraysize)	/* no hole found */
	{
		oldarray = *parray;
		(*parray) = (Plan **) palloc(sizeof(Plan *) * ((*eddyarraysize) + 1));
		memmove(*parray, oldarray, ((*eddyarraysize) * sizeof(Plan *)));
		(*eddyarraysize)++;
	}
	else
		oldarray = NULL;

	(*parray)[idx] = newplan;

	/*
	 * Free the old plan array
	 */
	if (oldarray)
	{
		pfree(oldarray);		/* pfree doesn't like NULL */
	}

	if (IsA_Fjord(newplan) && 
	    (MyProcessType == PROC_TCQ_BACKEND))   /* tcqoc: bugfix */
	{
		ProcessInitNode(newplan, cur->plan.state, (Plan *) cur);		/* Init module */
	}
}

/*
 * makeGSFilter -
 *	  creates a GSFilter node
 */
static GSFilter *
makeGSFilter(Node *node, Oid consttype)
{
	GSFilter   *gsf = makeNode(GSFilter);

	node = copyObject(node);
	gsf->eqoper = NULL;
	gsf->neoper = NULL;
	gsf->ltoper = NULL;
	gsf->gtoper = NULL;
	gsf->eqoperinsert = NULL;
	gsf->neoperinsert = NULL;
	gsf->ltoperinsert = NULL;
	gsf->gtoperinsert = NULL;
	gsf->node = node;
	gsf->ltmap = NULL;
	gsf->rtmap = NULL;
	gsf->consttype = consttype;
	LookupOpersForGSF(gsf);

	return gsf;
}

Index sourceGetVarNo(Plan *node)
{
	switch (nodeTag(node))
	{
		case T_SeqScan:
			return ((SeqScan *) node)->scanrelid;
		case T_StrmScan:
			return GetRTIndexStrmScan((StrmScan *)node);
		default:
			elog(ERROR, "ScanModule cannot find sourceID for nodetype %d",
				 nodeTag(node));
			break;
	}
	Assert(false);
	return -1;
}

int
varNoGetSourceArrayIndex(Eddy * e, int varno)
{
	int			i = 0;

	for (i = 0; i < e->numSources; i++)
	{
		if (e->sources[i] == NULL)
			continue;
		if (sourceGetVarNo(e->sources[i]) == varno)
			return i;
	}
	return -1;
}

void add_fagg(Eddy *cur, FAgg * module, int32 queryid, Query * qtree)
{
  FAgg *fagg = copyObject(module);
  TupleDesc td = cur->eddystate->resultTupleDescriptors[queryid];
  WindowClause * wc = copyObject(qtree->windowClause);
  Assert(td);
  Assert(wc);

  add_planitem_to_eddy(cur, (Plan *) fagg, EDDY_MODULE); /* Add to Eddy */
  FjordAddQuery((Plan *) fagg, queryid, (Plan *) cur);

  FAggAddWindowExprs(fagg, wc->windowExprs);
  FAggSetTupleDesc(fagg, td);
}


void
add_feventagg(Eddy *cur, FEventAgg *module, int32 queryid, Query *qtree)
{
  FEventAgg * feventagg = copyObject(module);
  List *vars, *var;
  Var *avar;
  int i;

  add_planitem_to_eddy(cur, (Plan *) feventagg, EDDY_MODULE);
  FjordAddQuery((Plan *) feventagg, queryid, (Plan *)cur);

  /*
   * Now we will manually set the operators bit array of the eddy
   * This is probably ugly, as setUpDependentModules() is meant for  
   * this, but I am still not sure it will do want we want.
   * So, doing it manually here.
   *
   * Note that all sources for this FEventAgg have been installed before we 
   * reach here
   *              - TCQ SR 
   */
  
  vars = pull_var_clause(feventagg->eventExpr, true);
  foreach(var, vars) 
    {
      avar = (Var *)lfirst(var);
      Assert(IsA(avar, Var));

      for(i = 0 ; i < cur->numSources ; i++) 
	{
	  if((ScanModule *)cur->sources[i] == NULL)
	    continue;
	  
	  
	  if(IsA(((ScanModule *)cur->sources[i])->scan, StrmScan)) 
	    {
	      if(avar->varno == ((StrmScan *)((ScanModule *)cur->sources[i])->scan)->seqscan->scanrelid) 
		{ 
		  BitArraySetBit(cur->eddystate->operators[i], ((Fjord *)feventagg)->operator);
		  elog(LOG, "add_feventagg: Setting operator %d for source %d", ((Fjord *)feventagg)->operator, i);
		}
	    }
	  else if(IsA(((ScanModule *)cur->sources[i])->scan, SeqScan)) 
	    {
	      
	      if(avar->varno == ((SeqScan *)((ScanModule *)cur->sources[i])->scan)->scanrelid) 
		{ 
		  BitArraySetBit(cur->eddystate->operators[i], ((Fjord *)feventagg)->operator);
		  elog(LOG, "add_feventagg: Setting operator %d for source %d", ((Fjord *)feventagg)->operator, i);
		}
	    }
	  else 
	    {
	      elog(FATAL, "add_feventagg: Found a source ScanModule that is neither a StrmScan nor a SeqScan... don't know what to do!");
	    }
	}
    }
  
  BitArraySetBit(cur->eddystate->completionMask[queryid], ((Fjord *)feventagg)->operator);

  if(BitArrayBitIsSet(cur->eddystate->eventOperators, ((Fjord *)feventagg)->operator)) /* Sanity check */
    elog(FATAL, "add_feventagg: The eventOperators bit array says that my position already had an event operator!");

  BitArraySetBit(cur->eddystate->eventOperators, ((Fjord *)feventagg)->operator);
  
}


void
add_index_stem(Eddy * cur, IndexSteM * module, int queryid)
{
	int			i = 0;
	Plan	   *p = (&((IndexSteM *) module)->join.plan);
	StrmScan   *strmscan = (StrmScan *) innerPlan(p);
	FSteM	   *fstem = makeNode(FSteM);
	int	 		idx = -1;

	if (!IsA(strmscan, StrmScan))
		elog(FATAL, "add index stem only knows how to deal with StrmScan plans");

	fstem->protostem = copyObject(module);
	for (i = 0; i < cur->numModules; i++)
	{
		if (cur->modules[i] == NULL)
			continue;

		if (IsA(cur->modules[i], FSteM))
		{
			int			sourceBuiltOn;
			SteM	   *s = (SteM *) ((FSteM *) cur->modules[i])->protostem;

			if (!IsA(s, SteM))
				continue;
			sourceBuiltOn = STEM_SOURCE(s);
			if (sourceBuiltOn == (sourceGetVarNo((Plan *) strmscan) - 1))
			{
				ProcessEndNode(cur->modules[i], (Plan *) cur);
				ProcessInitNode((Plan *) fstem, cur->plan.state, (Plan *) cur);
				fstem->fjord.operator = i;
				cur->modules[i] = (Plan *) fstem;
				FjordAddQuery((Plan *) fstem, queryid, (Plan *) cur);
				idx = i;

			}


		}


	}

	Assert(idx >= 0);


	for (i = 0; i < strmscan->numwrappers; i++)
	{
		if (strmscan->bindings[i])
		{
			setUpDependentModules(cur, idx, (Node *) strmscan->bindings[i]->targetlist);

		}
	}
}

/* some modules require a tuple of a given signature before they can be
   chosen by the eddy. These dependencies are encoded as a series of
   PostJoinOperator entries.  Each of these entries contains a bitarray
   for the sources and the modules.

   The sources bit array represents the composite tuple which must be present
   before the operators in the modules bit array may be added to the ready
   modules for a tuple.

   Given an expression which represents the required dependencies for a
    module, this function adjusts the PostJoinOperator data structures
   accordingly.

   If there is only a dependency on a single source, then the module is
   added directly to the list of operators for that source.


   NOTE: in order for this function to work correctly, expr must work
		 correctly with the pull_var_clause function

*/
void
setUpDependentModules(Eddy * e,
		      int operatorid,
		      Node *expr)
{
	List	   			*iter 	= NIL;
	Var		   			*fvar 	= NULL;
	List	   			*fvars 	= NIL;
	bits8				 sources[BASIZE(TCQMAXSOURCES)];
	bool				 onevar = false;
	PostJoinOperator 	*pjo;
	int					 numsources;
	Fjord	   			*fjord 	= (Fjord *) (e->modules)[operatorid];
	MOClass 			 class;
	
	fvars = pull_var_clause(expr, false);
	BitArrayRealClearAll(sources, TCQMAXSOURCES);

	/* First create bitmap of all sources */
	foreach(iter, fvars)
	{
		fvar = (Var *) lfirst(iter);

		Assert((fvar->varno - 1) < TCQMAXSOURCES);
		BitArraySetBit(sources, fvar->varno - 1);
		ProcessMarkDynCatalog(fjord, fvar->varno, e);
	}
	numsources = BitArrayNumSet(sources, TCQMAXSOURCES);
	Assert(numsources > 0);
	onevar = (numsources == 1);
	
	if (!onevar)
	{
		pjo = findPJOfromSources(e, sources);

		if (pjo)
			BitArraySetBit(pjo->ready, operatorid);
		else
		{
			pjo = (PostJoinOperator *) palloc(sizeof(PostJoinOperator));
			BitArrayClone(pjo->sources, sources, TCQMAXSOURCES);
			BitArrayClearAll(pjo->ready, TCQMAXOPERATORS);
			BitArraySetBit(pjo->ready, operatorid);
			e->eddystate->pjolist = lcons(pjo, e->eddystate->pjolist);
		}
	}
	else
	{
		fvar = (Var *) lfirst(fvars);
		Assert((fvar->varno - 1) < TCQMAXSOURCES);
		BitArraySetBit(e->eddystate->operators[fvar->varno - 1], operatorid);
	}
	/*
	 *  When we get rid of the PJOs just use the following ..
	 */
	class = (onevar) ? FILTERS : PROBES_AND_FILTERS;
	if (UseOperatorPrecedence) {
	  UpdateMetaOperators(e, sources, operatorid, class, true);
	}
}

/*
 * ProcessMarkDynCatalog
 *
 *	This function is called when a new Fjord-model operator is added to a query
 *	plan. For each operator, it should be called with _each_ range table index
 *	the operators is involved with.  If the index refers to a dynamic catalog
 *	stream, then the operator is flagged as such. Note, that if the index does
 *	not refer to a dynamic catalog stream, then no action is taken. Thus it is
 *	vital that every Fjord that is created is first zero'd out properly. While
 *	makeNode() should do this properly, it is something to keep in mind.
 *
 *	 node	(IN)	Fjord operator
 *	 rtidx	(IN)	Range table index of a stream "node" is involved with
 *	 eddy	(IN)	The eddy the operator is associated with
 *
 *	ASSUMPTIONS
 *
 *	  1. The Fjord operator is originally initialized so that dynCat is false
 *	  2. The eddy's rangetable is valid and has > rtindex values
 */
static void
ProcessMarkDynCatalog(Fjord * node, int rtidx, Eddy * eddy)
{
	RangeTblEntry *queryrte = NULL;
	Oid			scanoid = InvalidOid;

	queryrte = rt_fetch(rtidx, eddy->rangetable);
	scanoid = queryrte->relid;

	if (isDynamicCatalogStream(scanoid))
		node->dynCat = true;
}

/*
 * This functions inspects two of the Eddy data structures (rangetable and
 * sources array) and makes sure they are sane.
 *
 * The following invariants need to be ensured:
 *
 *	1. An rte can have an InvalidOid _iff_ the corresponding is NULL
 *	2. An rte's reloid must be the same as that of the corresponding source
 *
 */
static void
sanity_check_rt_sources(Eddy * e)
{
	List	   *rtelem = NIL;
	int			i = 0;
	RangeTblEntry *rte;
	bool		aretheremodules;

	foreach(rtelem, e->rangetable)
	{
		rte = (RangeTblEntry *) lfirst(rtelem);

		if (rte->relid != getRealSourceOid(e->sources[i]))
		{
			/*
			 * Oops we did it again !
			 */
			elog(ERROR, "Bad Eddy ! Bad RangeTable (%s) ! Bad Source [%d](%s)",
				 nodeToString(rte), i, nodeToString(e->sources[i]));
			break;
		}
		i++;
	}
	aretheremodules = false;
	for (i = 0; i < e->numModules; i++)
	{
		if (e->modules[i] != NULL)
		{
			aretheremodules = true;
			break;
		}
	}
	if (!aretheremodules)
	{
		elog(LOG, "No more non-null modules (out of %d) exist", e->numModules);
	}
}

static int
get_num_active_sources(Eddy * e)
{
	int			i;
	int			ns = 0;

	for (i = 0; i < e->numSources; i++)
	{
		if (e->sources[i] != NULL)
			ns++;
	}
	return ns;
}

static int
get_num_active_modules(Eddy * e)
{
	int			i;
	int			nm = 0;

	for (i = 0; i < e->numModules; i++)
	{
		if (e->modules[i] != NULL)
			nm++;
	}
	return nm;
}

/*
 *  GetNewMetaOp
 *
 *  This function creates a new MetaOperator instance, initializes it as
 *  specified and returns it to the caller
 *
 *  signature (IN) Signature of input tuples
 *  class     (IN) Category of the MetaOperator
 */
static MetaOperator*
GetNewMetaOp(Word_t signature, MOClass class)
{
	MetaOperator 		*m = (MetaOperator *) palloc(sizeof(MetaOperator));
	
	m->class = class;
	m->input = signature;
	m->visited = false;
	BitArrayClearAll(m->ready, TCQMAXOPERATORS);

	return m;
}

/*
 * UpdateMetaOperators
 *
 * This function updates the MetaOperators data structure by adding a given
 * operator in its various positions.
 *
 * e       (IN)  The current eddy being built in the backend
 * sources (IN)  A bitmap that identifies the input signature of tuples
 * opid    (IN)  Identifies the given operator in the Eddy
 * class   (IN)  The "category" of the new operator
 * beg     (IN)  When the operator creates a new class in a given chain for a
 *               given transition, this identifies whether or not this operator
 *               class goes in the beginning or the end of the chain.
 *
 */
static void
UpdateMetaOperators(Eddy *e, BitArray sources, int opid, MOClass class,bool beg)
{
	EddyState 			*state = e->eddystate;
	Word_t 		 		 idx   = 0;
	Word_t 		 		 Rc_int;
	MetaOperator 		*metaop;
	Pvoid_t              trans = NULL;
	Word_t 				*Pvalue;
	Word_t 				 signature;
	List 				*moplist;
	List   				*ml;
	Pvoid_t 			 sig;

	if (! UseOperatorPrecedence) {
	  elog(FATAL,"UpdateMetaOperators: Should NOT be here !! ");
	}

	signature = GetBitArrayAsWord_t(sources,TCQMAXSOURCES);

	sig = GetBitArrayAsJudy1(sources,TCQMAXSOURCES);
	
	J1F (Rc_int, sig, idx);

	do 
	{
		trans = state->transitions[idx];

		JLI (Pvalue, trans, signature);

		if ((*Pvalue) == ((Word_t) 0)) 
		{
			/*
			 *  New insertion - No transitions exist yet ...
			 */
			metaop = GetNewMetaOp(signature,class);
			moplist = makeList1(metaop);
			(*Pvalue) = (Word_t) moplist;
		}
		else
		{
			MetaOperator *mo;

			moplist = (List *)(*Pvalue);
			foreach (ml, moplist) 
			{
				mo = (MetaOperator *) lfirst(ml);
				
				if (mo->class == class) 
				{
					break;
				}
			}
			if (ml == NIL) 
			{
				metaop = GetNewMetaOp(signature,class);
				if (beg) 
				{
					(*Pvalue) = (Word_t) lcons(metaop,moplist);
				}
				else 
				{
					lappend (moplist,metaop);
				}
			}
			else 
			{
				metaop = mo;
			}
		}
		/*
		 *  Finally, we have the MetaOperator. Set the ready bits already !
		 */
		BitArraySetBit(metaop->ready, opid);
		/*
		 *  Make sure we reset the transitions element in the eddystate array !
		 */ 
		state->transitions[idx] = trans;
		/*
		 *  Traverse the next source in sig
		 */ 
		J1N (Rc_int, sig, idx);
	}
	while (Rc_int != 0);
}

/*
 *  Remove all traces of operator opid from the MetaOperator list
 *
 *  Brute-force algorithm .. is there anything better ?
 */
static void
UpdateMetaOperatorsRemove(Eddy *cur, int opid)
{
	EddyState 			*state = cur->eddystate;
	MetaOperator 		*metaop;
	Pvoid_t              trans = NULL;
	Word_t 				*Pvalue;
	Word_t 				 idx;
	List 				*moplist;
	List   				*ml;
	int 				 i;
	List 				*removal;
	bool 				 needToRemove;
	

	for (i = 0; i < cur->numModules; i++) 
	{
		if (cur->sources[i] == NULL) 
		{
			continue;
		}
		trans = state->transitions[i];
		idx = 0;
		JLF (Pvalue, trans, idx);
		while (Pvalue != (Word_t *) NULL)
		{
			removal = NIL;
			needToRemove = false;
			
			moplist = (List *) (*Pvalue);

			foreach(ml, moplist) 
			{
				metaop = (MetaOperator *) lfirst(ml);
				if (BitArrayBitIsSet(metaop->ready,opid))
				{
					BitArrayClearBit(metaop->ready,opid);
					if (! BitArrayIsAnySet(metaop->ready, TCQMAXOPERATORS)) 
					{
						removal = lcons(metaop,removal);
						needToRemove = true;
					}
				}
			}
			if (needToRemove) 
			{
				foreach(ml,removal) 
				{
					metaop = (MetaOperator *) lfirst(ml);
					moplist = lremove (metaop, moplist);

					/* Zero out the struct to help dangling pointer detection. */
					MemSet(metaop, 0x0, sizeof(MetaOperator));
					pfree(metaop);
				}
				freeList(removal);
				(*Pvalue) = (Word_t) moplist;
			}
			JLN (Pvalue, trans, idx);
		}
	}
	
}

void
PrintMetaOperator(Eddy *cur, MetaOperator *mo)
{
	char *class = NULL;
	
	switch (mo->class)
	{
		case NOCLASS:
			class = "NOCLASS";
			break;
		case FILTERS:
			class = "FILTERS";
			break;
		case BUILDS:
			class = "BUILDS";
			break;
		case PROBES_AND_FILTERS:
			class = "PROBES_AND_FILTERS";
			break;
		case AGGREGATIONS:
			class = "AGGREGATIONS";
			break;
		case OUTPUTS:
			class = "OUTPUTS";
			break;
	}
	elog(DEBUG1,"\t\tClass: %s", class);
	elog(DEBUG1,"\t\tinput: %x", (int) mo->input);
	elog(DEBUG1,"\t\tvisited: %d",(int)mo->visited);
	elog(DEBUG1,"\t\tready: ");
	BitArrayPrintDebug(mo->ready, cur->numModules);
}

/*
 * PrintMetaOperatorTransitions
 *
 */ 
static void
PrintMetaOperatorTransitions(Eddy *cur)
{
	int 		 		 i;
	List 				*moplist,*ml;
	Pvoid_t 	 		 trans;
	Word_t   			*Pvalue;
	Word_t 		 		 idx;
	RangeTblEntry 		*rte;
	char 				*relname;
	
	elog(DEBUG1,"------------------------------------------------------------");
	elog(DEBUG1,"Printing current MetaOperator datastructure");

	for (i = 0; i < cur->numSources; i++)
	{
		if (cur->sources[i] == NULL) 
		{
			continue;
		}
		relname = NULL;
		trans = cur->eddystate->transitions[i];
		rte = rt_fetch(i+1, cur->rangetable);
		relname = get_rel_name(rte->relid);
		elog(DEBUG1,"\tSource [%d] [%s]",i,relname);
		pfree(relname);

		idx = 0;
		JLF (Pvalue, trans, idx);
		while (Pvalue != (Word_t *) NULL)
		{
			moplist = (List *) (*Pvalue);

			foreach(ml, moplist) 
			{
				PrintMetaOperator(cur, (MetaOperator *) lfirst(ml));
			}
			JLN (Pvalue, trans, idx);
		}
	}
	elog(DEBUG1,"Done printing current MetaOperator datastructure");
	elog(DEBUG1,"------------------------------------------------------------");
}

/*
 * BEGIN SIGMOD GROSSNESS
 */
int 	whichquery 	 	 = 0;
int 	StaticDriver 	 = -1;

Eddy 	*StaticEddy    = NULL;
bool     UseStaticPlan = false;
int UseStaticPlanNumber = -1;


static void
SetSourceBuiltOn(FSteM *fstem,BitArray ba)
{
	SteM *proto = (SteM *) GetRepresentativeSteMFSteM(fstem);
	
	BitArray sbo = proto->sourceBuiltOn;

	BitArrayClone(sbo, ba, TCQMAXSOURCES);
}

static void
KeepOuterHashkeys(FSteM *fstem, int outer, int clause)
{
	List 				*fohklist;
	List 				*fstclist;
	FSteMOuterHashkey 	*fohk;
	FSteMClauses 		*fsc;
	
	fohklist = fstem->fstemouterhashkeys;
	fohk = (FSteMOuterHashkey *) nth(outer,fohklist);

	fstclist = fohk->fstemclauses_list;
	fsc = (FSteMClauses *) nth(clause,fstclist);

	fohk->fstemclauses_list = makeList1(fsc);

	fstem->fstemouterhashkeys = makeList1(fohk);
}

static void
InitializeOutputs(int num, Fjord *fjord) 
{
	fjord->numOutputs = num;
	fjord->outputs  = (Fjord **) palloc(sizeof(Fjord*) * num);
}

static void
SetOutputQueue(int driver, Fjord *fjord, Fjord *output)
{
	fjord->outputs[driver] = output;
}

static void
InitSplitOutput(FSplit *fsplit, int num)
{
	InitializeOutputs(num, (Fjord *)fsplit);
}

static void
SetSplitOutput(FSplit *fsplit,int num,Fjord *fjord)
{
	(((Fjord *)fsplit)->outputs)[num] = fjord;
}

/*
 * These are the queries in question:
 *
 *   Q0:
 *         select    st1.a, st1.b, st1.tcqtime, st2.a, st2.b, st2.tcqtime, 
 *                   timeofday()::timestamp
 *         from      st1, st2 
 *         where     st1.a = st2.a;
 *
 *   Q1:
 *         select    st1.a, st1.b, st1.tcqtime, 
 *            	     st2.a, st2.b, st2.tcqtime, 
 *                   st3.a, st3.b, st3.tcqtime,
 *                   timeofday()::timestamp
 *         from      st1, st2, st3
 *         where     st1.a = st2.a and st2.a = st3.a;
 *
 *   Q2:
 *
 *         select    st2.a, st2.b, st2.tcqtime, 
 *          	     st3.a, st3.b, st3.tcqtime,
 *                   timeofday()::timestamp
 *         from      st2, st3 
 *         where     st2.a = st3.a;
 */
static void
form_static_plan_0(Eddy *cur, Query *query)
{
	ScanModule 	*r_scan,*s_scan,*t_scan;
	FSteM 		*r_stem,*s_stem,*t_stem,*rs_stem,*t2_stem,*s2_stem;
	FOutput 	*q0_out, *q1_out, *q2_out;
	FSplit      *rs_split,*s_split,*t_split;
	int r_source, s_source, t_source;
	int r_stemid, s_stemid, t_stemid;
	int q0_id, q1_id, q2_id;
	int 	numStaticSources = 3;
	int     numStaticModules = 12;
	
	int i;

	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];
	bits8 t_bits[BASIZE(TCQMAXSOURCES)];

	bits8 rs_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;
	t_source = 2;

	r_stemid = 0;
	s_stemid = 1;
	t_stemid = 3;	

	q0_id = 2;
	q1_id = 4;
	q2_id = 5;

	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);
	BitArrayClearAll(t_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);
	BitArraySetBit(t_bits,t_source);

	BitArrayOr(r_bits,s_bits,rs_bits,TCQMAXSOURCES);

 	/*
	 *  Create our plan objects 
	 */
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
	t_scan = copyObject(cur->sources[t_source]);

	r_stem = copyObject(cur->modules[r_stemid]);
	s_stem = copyObject(cur->modules[s_stemid]);
	t_stem = copyObject(cur->modules[t_stemid]);

	rs_stem = copyObject(s_stem);
	s2_stem = copyObject(s_stem);
	t2_stem = copyObject(t_stem);

	q0_out = copyObject(cur->modules[q0_id]);
	q1_out = copyObject(cur->modules[q1_id]);
	q2_out = copyObject(cur->modules[q2_id]);

	rs_split = makeNode(FSplit);
	s_split = makeNode(FSplit);
	t_split = makeNode(FSplit);

	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	StaticEddy->sources[2] = (Plan *) t_scan;

	StaticEddy->modules[ 0] = (Plan *) r_stem ;
	StaticEddy->modules[ 1] = (Plan *) s_stem ;
	StaticEddy->modules[ 2] = (Plan *) t_stem ;
	StaticEddy->modules[ 3] = (Plan *) rs_stem;
	StaticEddy->modules[ 4] = (Plan *) t2_stem;
	StaticEddy->modules[ 5] = (Plan *) s2_stem;
	StaticEddy->modules[ 6] = (Plan *) q0_out ;
	StaticEddy->modules[ 7] = (Plan *) q1_out ;
	StaticEddy->modules[ 8] = (Plan *) q2_out ;
	StaticEddy->modules[ 9] = (Plan *) rs_split;
	StaticEddy->modules[10] = (Plan *) s_split;
	StaticEddy->modules[11] = (Plan *) t_split;

	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	SetSourceBuiltOn(rs_stem,rs_bits);
	
	KeepOuterHashkeys(r_stem,0,0);
	KeepOuterHashkeys(s_stem,0,0);
	KeepOuterHashkeys(rs_stem,1,0);
	KeepOuterHashkeys(s2_stem,1,0);

	/* Nothing to remove for t_stem and t2_stem */

	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)t_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s_stem);
	InitializeOutputs(numStaticSources,(Fjord *)t_stem);
	InitializeOutputs(numStaticSources,(Fjord *)rs_stem);
	InitializeOutputs(numStaticSources,(Fjord *)t2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)q0_out);
	InitializeOutputs(numStaticSources,(Fjord *)q1_out);
	InitializeOutputs(numStaticSources,(Fjord *)q2_out);
	InitializeOutputs(numStaticSources,(Fjord *)rs_split);
	InitializeOutputs(numStaticSources,(Fjord *)s_split);
	InitializeOutputs(numStaticSources,(Fjord *)t_split);

	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)r_stem);
	SetOutputQueue(r_source,(Fjord *)r_stem ,(Fjord *)s_stem);
	SetOutputQueue(r_source,(Fjord *)s_stem ,(Fjord *)rs_stem);
	SetOutputQueue(r_source,(Fjord *)rs_stem,(Fjord *)rs_split);
	InitSplitOutput(rs_split,2);
	/* 2 children */
	SetSplitOutput(rs_split,0,(Fjord *)q0_out);
	SetSplitOutput(rs_split,1,(Fjord *)t_stem);
	/* 1st child */
	/* 2nd child */
	SetOutputQueue(r_source,(Fjord *)t_stem,(Fjord *)q1_out);
	
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)s_split);
	InitSplitOutput(s_split,2);
	/* 2 children */
	SetSplitOutput(s_split,0,(Fjord *)s_stem);
	SetSplitOutput(s_split,1,(Fjord *)s2_stem);
	/* 1st child */
	SetOutputQueue(s_source,(Fjord *)s_stem ,(Fjord *)r_stem);
	SetOutputQueue(s_source,(Fjord *)r_stem ,(Fjord *)rs_stem);
	SetOutputQueue(s_source,(Fjord *)rs_stem,(Fjord *)rs_split);
	SetSplitOutput(rs_split,0,(Fjord *)q0_out);
	SetSplitOutput(rs_split,1,(Fjord *)t_stem);
	SetOutputQueue(s_source,(Fjord *)t_stem,(Fjord *)q1_out);
	/* 2nd child */
	SetOutputQueue(s_source,(Fjord *)s2_stem,(Fjord *)t2_stem);
	SetOutputQueue(s_source,(Fjord *)t2_stem,(Fjord *)q2_out);

	/*
	 * Driver is t
	 */
	SetOutputQueue(t_source,(Fjord *)t_scan,(Fjord *)t_split);
	InitSplitOutput(t_split,2);
	/* 2 children */
	SetSplitOutput(t_split,0,(Fjord *)t_stem);
	SetSplitOutput(t_split,1,(Fjord *)t2_stem);
	/* 1st child */
	SetOutputQueue(t_source,(Fjord *)t_stem ,(Fjord *)rs_stem);
	SetOutputQueue(t_source,(Fjord *)rs_stem,(Fjord *)q1_out);
	/* 2nd child */
	SetOutputQueue(t_source,(Fjord *)t2_stem,(Fjord *)s2_stem);
	SetOutputQueue(t_source,(Fjord *)s2_stem,(Fjord *)q2_out);

}

/*
 * The queries for this static plan are:
 *
 * Q1: select * from st1, st2
 *              where st1.a > 700 AND
 *                    st1.b = st2.b;
 *
 * Q2: select * from st1, st2
 *              where st1.a > 900 AND
 *                    st1.b = st2.b;
 *
 */
static void form_static_plan_1(Eddy *cur, Query *query) 
{
	ScanModule 	*r_scan,*s_scan;
	FSteM 		*r1_stem, *r2_stem;
	FSteM 		*s1_stem, *s2_stem;
	FOutput 	*q0_out, *q1_out;
	FSplit      *r_split,*s_split;
	FFilter     *r1_filter, *r2_filter;
	GSFilter 	*gs_filter;
	
	int r_source, s_source;
	int r_stemid, s_stemid;
	int gsf_id;
	int q0_id, q1_id;
	int numStaticSources = 2;
	int numStaticModules = 10;
	
	int i;
		
	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;

	r_stemid = 1;
	s_stemid = 2;

	gsf_id = 0;
	
	/* All have same signature, so copy the single FOutput */
	q0_id = 3;
	q1_id = 3;
	
	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);

 	/*
	 *  Create our plan objects 
	 */

	r1_filter = makeNode(FFilter);
	r2_filter = makeNode(FFilter);
		
	gs_filter = (GSFilter *)cur->modules[gsf_id];
	r1_filter->filterqual = lfirst(lnext(gs_filter->all_exprs));
	r2_filter->filterqual = lfirst(gs_filter->all_exprs);
		
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
		
	r1_stem = copyObject(cur->modules[r_stemid]);
	r2_stem = copyObject(cur->modules[r_stemid]);
		
	s1_stem = copyObject(cur->modules[s_stemid]);
	s2_stem = copyObject(cur->modules[s_stemid]);
		
	q0_out = copyObject(cur->modules[q0_id]);
	q1_out = copyObject(cur->modules[q1_id]);
		
	r_split = makeNode(FSplit);
	s_split = makeNode(FSplit);
	
	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	
	StaticEddy->modules[0] = (Plan *) r1_stem ;
	StaticEddy->modules[1] = (Plan *) r2_stem ;
	StaticEddy->modules[2] = (Plan *) s1_stem;
	StaticEddy->modules[3] = (Plan *) s2_stem;
	StaticEddy->modules[4] = (Plan *) r1_filter;
	StaticEddy->modules[5] = (Plan *) r2_filter;
	StaticEddy->modules[6] = (Plan *) q0_out ;
	StaticEddy->modules[7] = (Plan *) q1_out ;
	StaticEddy->modules[8] = (Plan *) r_split;
	StaticEddy->modules[9] = (Plan *) s_split;

	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		((Fjord *) StaticEddy->sources[i])->operator = i;
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		((Fjord *) StaticEddy->modules[i])->operator = i;
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	/* DIDN'T TOUCH, MAY NEED TO CHANGE */
	KeepOuterHashkeys(r1_stem,0,0);
	KeepOuterHashkeys(r2_stem,0,0);
	KeepOuterHashkeys(s1_stem,0,0);
	KeepOuterHashkeys(s2_stem,0,0);
	
	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r1_stem);
	InitializeOutputs(numStaticSources,(Fjord *)r2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s1_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)r1_filter);
	InitializeOutputs(numStaticSources,(Fjord *)r2_filter);
	InitializeOutputs(numStaticSources,(Fjord *)q0_out);
	InitializeOutputs(numStaticSources,(Fjord *)q1_out);
	InitializeOutputs(numStaticSources,(Fjord *)r_split);
	InitializeOutputs(numStaticSources,(Fjord *)s_split);

	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)r_split);
	InitSplitOutput(r_split,2);
	SetSplitOutput(r_split,0,(Fjord *) r1_filter);
	SetSplitOutput(r_split,1,(Fjord *) r2_filter);
	
	SetOutputQueue(r_source,(Fjord *)r1_filter ,(Fjord *)r1_stem);
	SetOutputQueue(r_source,(Fjord *)r2_filter ,(Fjord *)r2_stem);
	
	SetOutputQueue(r_source,(Fjord *)r1_stem ,(Fjord *)s1_stem);
	SetOutputQueue(r_source,(Fjord *)r2_stem ,(Fjord *)s2_stem);
	
	SetOutputQueue(r_source,(Fjord *)s1_stem ,(Fjord *)q0_out);
	SetOutputQueue(r_source,(Fjord *)s2_stem ,(Fjord *)q1_out);
	
	
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)s_split);
	InitSplitOutput(s_split,2);
	SetSplitOutput(s_split,0,(Fjord *)s1_stem);
	SetSplitOutput(s_split,1,(Fjord *)s2_stem);
	
	SetOutputQueue(s_source,(Fjord *)s1_stem ,(Fjord *)r1_stem);
	SetOutputQueue(s_source,(Fjord *)s2_stem ,(Fjord *)r2_stem);
		
	SetOutputQueue(s_source,(Fjord *)r1_stem ,(Fjord *)q0_out);
	SetOutputQueue(s_source,(Fjord *)r2_stem ,(Fjord *)q1_out);
		
	elog(DEBUG1,"Phew .. we now print the static plan ");
	elog(DEBUG1,"------------------------------------------------------------");
	pprint(StaticEddy);
	elog(DEBUG1,"------------------------------------------------------------");

}

/*
 * These are the queries in question
 *
 *   Q0:
 *         select    st1.a, st1.b, st1.tcqtime, st2.a, st2.b, st2.tcqtime, 
 *                   timeofday()::timestamp
 *         from      st1, st2 
 *         where     st1.a = st2.a;
 *         window    st1 ['1 second'], st2 ['1 second']
 *
 *   Q1:
 *         select    st1.a, st1.b, st1.tcqtime, 
 *            	     st2.a, st2.b, st2.tcqtime, 
 *                   st3.a, st3.b, st3.tcqtime,
 *                   timeofday()::timestamp
 *         from      st1, st2, st3
 *         where     st1.a = st2.a and st2.a = st3.a;
 *         window    st1 ['1 second'], st2 ['1 second'], st3 ['1 second']
 *
 *   Q2:
 *
 *         select    st2.a, st2.b, st2.tcqtime, 
 *         	         st3.a, st3.b, st3.tcqtime,
 *                   timeofday()::timestamp
 *         from      st2, st3 
 *         where     st2.a = st3.a;
 *         window    st2 ['1 second'], st3 ['1 second']
 *
 */
static void
form_static_plan_2(Eddy *cur, Query *query)
{
	ScanModule 	*r_scan,*s_scan,*t_scan;
	FSteM 		*r_stem,*s_stem,*t_stem,*rs_stem,*t2_stem,*s2_stem;
	FOutput 	*q0_out, *q1_out, *q2_out;
	FSplit      *rs_split,*s_split,*t_split;
	FFilter     *rs_filter,*rst_filter,*st_filter;

	GSFilter    *gsf_rs_1_filter, *gsf_rs_2_filter;
	GSFilter    *gsf_rst_1_filter, *gsf_rst_2_filter, *gsf_rst_3_filter;
	GSFilter    *gsf_st_1_filter, *gsf_st_2_filter;

	Expr 		*rs_q1,*rs_q2;
	Expr 		*rst_q1,*rst_q2,*rst_q3;
	Expr 		*st_q1,*st_q2;
	
	int 		 r_source, s_source, t_source;
	int 		 r_stemid, s_stemid, t_stemid;
	int 		 q0_id, q1_id, q2_id;
	int          gsf_rs_1_id, gsf_rs_2_id;
	int          gsf_rst_1_id, gsf_rst_2_id, gsf_rst_3_id;
	int          gsf_st_1_id, gsf_st_2_id;
	int 		 numStaticSources = 3;
	int     	 numStaticModules = 15;
	
	int i;

	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];
	bits8 t_bits[BASIZE(TCQMAXSOURCES)];

	bits8 rs_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;
	t_source = 2;

	r_stemid = 0;
	s_stemid = 1;
	t_stemid = 5;	

	gsf_rs_1_id  = 2;
	gsf_rs_2_id  = 3;
	gsf_rst_1_id = 6;
	gsf_rst_2_id = 7;
	gsf_rst_3_id = 8;
	gsf_st_1_id  = 10;
	gsf_st_2_id  = 11;
	
	q0_id = 4;
	q1_id = 9;
	q2_id = 12;

	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);
	BitArrayClearAll(t_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);
	BitArraySetBit(t_bits,t_source);

	BitArrayOr(r_bits,s_bits,rs_bits,TCQMAXSOURCES);
	
	gsf_rs_1_filter  = (GSFilter *) cur->modules[gsf_rs_1_id ];
	gsf_rs_2_filter  = (GSFilter *) cur->modules[gsf_rs_2_id ];
	gsf_rst_1_filter = (GSFilter *) cur->modules[gsf_rst_1_id];
	gsf_rst_2_filter = (GSFilter *) cur->modules[gsf_rst_2_id];
	gsf_rst_3_filter = (GSFilter *) cur->modules[gsf_rst_3_id];
	gsf_st_1_filter  = (GSFilter *) cur->modules[gsf_st_1_id ];
	gsf_st_2_filter  = (GSFilter *) cur->modules[gsf_st_2_id ];
	
 	/*
	 *  Create our plan objects 
	 */
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
	t_scan = copyObject(cur->sources[t_source]);

	r_stem = copyObject(cur->modules[r_stemid]);
	s_stem = copyObject(cur->modules[s_stemid]);
	t_stem = copyObject(cur->modules[t_stemid]);

	rs_stem = copyObject(s_stem);
	s2_stem = copyObject(s_stem);
	t2_stem = copyObject(t_stem);

	rs_filter = makeNode(FFilter);
	rst_filter = makeNode(FFilter);
	st_filter = makeNode(FFilter);

	rs_q1 = lfirst(gsf_rs_1_filter->all_exprs);
	rs_q2 = lfirst(gsf_rs_2_filter->all_exprs);
	
	rs_filter->filterqual = (Node *) make_andclause(makeList2(rs_q1,rs_q2));

	rst_q1  = lfirst(gsf_rst_1_filter->all_exprs);
	rst_q2  = lfirst(gsf_rst_2_filter->all_exprs);
	rst_q3  = lfirst(gsf_rst_3_filter->all_exprs);
	
	rst_filter->filterqual = (Node *) make_andclause(makeList3(rst_q1,rst_q2,rst_q3));
	
	st_q1 = lfirst(gsf_st_1_filter->all_exprs);
	st_q2 = lfirst(gsf_st_2_filter->all_exprs);

	st_filter->filterqual = (Node *) make_andclause(makeList2(st_q1, st_q2));
	
	q0_out = copyObject(cur->modules[q0_id]);
	q1_out = copyObject(cur->modules[q1_id]);
	q2_out = copyObject(cur->modules[q2_id]);

	rs_split = makeNode(FSplit);
	s_split = makeNode(FSplit);
	t_split = makeNode(FSplit);

	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	StaticEddy->sources[2] = (Plan *) t_scan;

	StaticEddy->modules[ 0] = (Plan *) r_stem ;
	StaticEddy->modules[ 1] = (Plan *) s_stem ;
	StaticEddy->modules[ 2] = (Plan *) t_stem ;
	StaticEddy->modules[ 3] = (Plan *) rs_stem;
	StaticEddy->modules[ 4] = (Plan *) t2_stem;
	StaticEddy->modules[ 5] = (Plan *) s2_stem;
	StaticEddy->modules[ 6] = (Plan *) q0_out ;
	StaticEddy->modules[ 7] = (Plan *) q1_out ;
	StaticEddy->modules[ 8] = (Plan *) q2_out ;
	StaticEddy->modules[ 9] = (Plan *) rs_split;
	StaticEddy->modules[10] = (Plan *) s_split;
	StaticEddy->modules[11] = (Plan *) t_split;
	StaticEddy->modules[12] = (Plan *) rs_filter;
	StaticEddy->modules[13] = (Plan *) rst_filter;
	StaticEddy->modules[14] = (Plan *) st_filter;
	

	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	SetSourceBuiltOn(rs_stem,rs_bits);
	
	KeepOuterHashkeys(r_stem,0,0);
	KeepOuterHashkeys(s_stem,0,0);
	KeepOuterHashkeys(rs_stem,1,0);
	KeepOuterHashkeys(s2_stem,1,0);

	/* Nothing to remove for t_stem and t2_stem */
	
	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)t_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s_stem);
	InitializeOutputs(numStaticSources,(Fjord *)t_stem);
	InitializeOutputs(numStaticSources,(Fjord *)rs_stem);
	InitializeOutputs(numStaticSources,(Fjord *)t2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s2_stem);
	InitializeOutputs(numStaticSources,(Fjord *)q0_out);
	InitializeOutputs(numStaticSources,(Fjord *)q1_out);
	InitializeOutputs(numStaticSources,(Fjord *)q2_out);
	InitializeOutputs(numStaticSources,(Fjord *)rs_split);
	InitializeOutputs(numStaticSources,(Fjord *)s_split);
	InitializeOutputs(numStaticSources,(Fjord *)t_split);
	InitializeOutputs(numStaticSources,(Fjord *)rs_filter);
	InitializeOutputs(numStaticSources,(Fjord *)rst_filter);
	InitializeOutputs(numStaticSources,(Fjord *)st_filter);

	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)r_stem);
	SetOutputQueue(r_source,(Fjord *)r_stem ,(Fjord *)s_stem);
	SetOutputQueue(r_source,(Fjord *)s_stem ,(Fjord *)rs_stem);
	SetOutputQueue(r_source,(Fjord *)rs_stem,(Fjord *)rs_split);
	InitSplitOutput(rs_split,2);
	/* 2 children */
	SetSplitOutput(rs_split,0,(Fjord *)rs_filter);
	SetSplitOutput(rs_split,1,(Fjord *)t_stem);
	/* 1st child */
	SetOutputQueue(r_source,(Fjord *)rs_filter,(Fjord *)q0_out);
	/* 2nd child */
	SetOutputQueue(r_source,(Fjord *)t_stem,(Fjord *)rst_filter);
	SetOutputQueue(r_source,(Fjord *)rst_filter,(Fjord *)q1_out);
	
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)s_split);
	InitSplitOutput(s_split,2);
	/* 2 children */
	SetSplitOutput(s_split,0,(Fjord *)s_stem);
	SetSplitOutput(s_split,1,(Fjord *)s2_stem);
	/* 1st child */
	SetOutputQueue(s_source,(Fjord *)s_stem ,(Fjord *)r_stem);
	SetOutputQueue(s_source,(Fjord *)r_stem ,(Fjord *)rs_stem);
	SetOutputQueue(s_source,(Fjord *)rs_stem,(Fjord *)rs_split);
	SetSplitOutput(rs_split,0,(Fjord *)rs_filter);
	SetOutputQueue(s_source,(Fjord *)rs_filter,(Fjord *)q0_out);
	SetSplitOutput(rs_split,1,(Fjord *)t_stem);
	SetOutputQueue(s_source,(Fjord *)t_stem,(Fjord *)rst_filter);
	SetOutputQueue(s_source,(Fjord *)rst_filter,(Fjord *)q1_out);
	/* 2nd child */
	SetOutputQueue(s_source,(Fjord *)s2_stem,(Fjord *)t2_stem);
	SetOutputQueue(s_source,(Fjord *)t2_stem,(Fjord *)st_filter);
	SetOutputQueue(s_source,(Fjord *)st_filter,(Fjord *)q2_out);

	/*
	 * Driver is t
	 */
	SetOutputQueue(t_source,(Fjord *)t_scan,(Fjord *)t_split);
	InitSplitOutput(t_split,2);
	/* 2 children */
	SetSplitOutput(t_split,0,(Fjord *)t_stem);
	SetSplitOutput(t_split,1,(Fjord *)t2_stem);
	/* 1st child */
	SetOutputQueue(t_source,(Fjord *)t_stem ,(Fjord *)rs_stem);
	SetOutputQueue(t_source,(Fjord *)rs_stem,(Fjord *)rst_filter);
	SetOutputQueue(t_source,(Fjord *)rst_filter,(Fjord *)q1_out);
	/* 2nd child */
	SetOutputQueue(t_source,(Fjord *)t2_stem,(Fjord *)s2_stem);
	SetOutputQueue(t_source,(Fjord *)s2_stem,(Fjord *)st_filter);
	SetOutputQueue(t_source,(Fjord *)st_filter,(Fjord *)q2_out);

}

static void form_static_plan_3(Eddy *cur, Query *query) 
{
	ScanModule 	*r_scan,*s_scan;
	FSteM 		*r_stem, *s_stem;
	FOutput 	*q0_out, *q1_out;
	FSplit      *r_split,*s_split;
	FFilter     *r1_filter, *r2_filter, *disjunct_filter;
	GSFilter 	*gs_filter;
	
	int r_source, s_source;
	int r_stemid, s_stemid;
	int gsf_id;
	int q0_id, q1_id;
	int numStaticSources = 2;
	int numStaticModules = 9;
	
	int i;
		
	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;

	r_stemid = 1;
	s_stemid = 2;

	gsf_id = 0;
	
	/* All have same signature, so copy the single FOutput */
	q0_id = 3;
	q1_id = 3;
	
	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);

 	/*
	 *  Create our plan objects 
	 */

	r1_filter = makeNode(FFilter);
	r2_filter = makeNode(FFilter);
	disjunct_filter = makeNode(FFilter);
	
	gs_filter = (GSFilter *)cur->modules[gsf_id];
	r1_filter->filterqual = lfirst(lnext(gs_filter->all_exprs));
	r2_filter->filterqual = lfirst(gs_filter->all_exprs);
	disjunct_filter->filterqual = (Node*)make_orclause(gs_filter->all_exprs);
			
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
		
	r_stem = copyObject(cur->modules[r_stemid]);
	s_stem = copyObject(cur->modules[s_stemid]);
			
	q0_out = copyObject(cur->modules[q0_id]);
	q1_out = copyObject(cur->modules[q1_id]);
		
	r_split = makeNode(FSplit);
	s_split = makeNode(FSplit);
	
	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	
	StaticEddy->modules[0] = (Plan *) r_stem;
	StaticEddy->modules[1] = (Plan *) s_stem;
	StaticEddy->modules[2] = (Plan *) r1_filter;
	StaticEddy->modules[3] = (Plan *) r2_filter;
	StaticEddy->modules[4] = (Plan *) q0_out ;
	StaticEddy->modules[5] = (Plan *) q1_out ;
	StaticEddy->modules[6] = (Plan *) r_split;
	StaticEddy->modules[7] = (Plan *) s_split;
	StaticEddy->modules[8] = (Plan *) disjunct_filter;
	
	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		((Fjord *) StaticEddy->sources[i])->operator = i;
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		((Fjord *) StaticEddy->modules[i])->operator = i;
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	/* DIDN'T TOUCH, MAY NEED TO CHANGE */
	KeepOuterHashkeys(r_stem,0,0);
	KeepOuterHashkeys(s_stem,0,0);
		
	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s_stem);
	InitializeOutputs(numStaticSources,(Fjord *)r1_filter);
	InitializeOutputs(numStaticSources,(Fjord *)r2_filter);
	InitializeOutputs(numStaticSources,(Fjord *)q0_out);
	InitializeOutputs(numStaticSources,(Fjord *)q1_out);
	InitializeOutputs(numStaticSources,(Fjord *)r_split);
	InitializeOutputs(numStaticSources,(Fjord *)s_split);
	InitializeOutputs(numStaticSources,(Fjord *)disjunct_filter);
	
	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)disjunct_filter);
	
	SetOutputQueue(r_source,(Fjord *) disjunct_filter, (Fjord *)r_stem);
	SetOutputQueue(r_source,(Fjord *) r_stem, (Fjord *)s_stem);
	SetOutputQueue(r_source,(Fjord *) s_stem, (Fjord *)r_split);
	
	InitSplitOutput(r_split,2);
	SetSplitOutput(r_split,0,(Fjord *) r1_filter);
	SetSplitOutput(r_split,1,(Fjord *) r2_filter);
	
	SetOutputQueue(r_source,(Fjord *)r1_filter ,(Fjord *)q0_out);
	SetOutputQueue(r_source,(Fjord *)r2_filter ,(Fjord *)q1_out);
	
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)s_stem);
	
	SetOutputQueue(s_source,(Fjord *)s_stem,(Fjord *)r_stem);
	SetOutputQueue(s_source,(Fjord *)r_stem,(Fjord *)s_split);
		
	InitSplitOutput(s_split,2);
	SetSplitOutput(s_split,0,(Fjord *)r1_filter);
	SetSplitOutput(s_split,1,(Fjord *)r2_filter);
	
	SetOutputQueue(s_source,(Fjord *)r1_filter,(Fjord *)q0_out);
	SetOutputQueue(s_source,(Fjord *)r2_filter,(Fjord *)q1_out);
	
	elog(DEBUG1,"Phew .. we now print the static plan ");
	elog(DEBUG1,"------------------------------------------------------------");
	pprint(StaticEddy);
	elog(DEBUG1,"------------------------------------------------------------");

}

/* TWO QUERIES, FOUR PREDICATES, TWO DISJUNCTIONS */
static void form_static_plan_4(Eddy *cur, Query *query) 
{
	ScanModule 	*r_scan,*s_scan;
	FSteM 		*r_stem, *s_stem;
	FOutput 	*q2_out, *q3_out;
	FSplit      *split;
	FFilter     *q2_filter, *q3_filter, *r_filter, *s_filter;
	GSFilter 	*rgs_filter, *sgs_filter;
	List *q2_clauses, *q3_clauses;
		
	int r_source, s_source;
	int r_stemid, s_stemid;
	int rgsf_id, sgsf_id;
	int q2_id, q3_id;
	int numStaticSources = 2;
	int numStaticModules = 9;
	
	int i;
		
	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;

	r_stemid = 2;
	s_stemid = 3;

	rgsf_id = 0;
	sgsf_id = 1;
		
	/* All have same signature, so copy the single FOutput */
	q2_id = 4;
	q3_id = 4;
	
	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);

 	/*
	 *  Create our plan objects 
	 */

	q2_filter = makeNode(FFilter);
	q3_filter = makeNode(FFilter);
	r_filter = makeNode(FFilter);
	s_filter = makeNode(FFilter);
	
	rgs_filter = (GSFilter *)cur->modules[rgsf_id];
	sgs_filter = (GSFilter *)cur->modules[sgsf_id];
	
	q2_clauses = NIL;
	q2_clauses = lcons(lfirst(rgs_filter->all_exprs), q2_clauses);
	q2_clauses = lcons(lfirst(sgs_filter->all_exprs), q2_clauses);

	q3_clauses = NIL;
	q3_clauses = lcons(lfirst(lnext(rgs_filter->all_exprs)), q3_clauses);
	q3_clauses = lcons(lfirst(lnext(sgs_filter->all_exprs)), q3_clauses);
					  
	q2_filter->filterqual = (Node*)make_andclause(q2_clauses);
	q3_filter->filterqual = (Node*)make_andclause(q3_clauses);
	r_filter->filterqual = (Node*)make_orclause(rgs_filter->all_exprs);
	s_filter->filterqual = (Node*)make_orclause(sgs_filter->all_exprs);
		
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
		
	r_stem = copyObject(cur->modules[r_stemid]);
	s_stem = copyObject(cur->modules[s_stemid]);
			
	q2_out = copyObject(cur->modules[q2_id]);
	q3_out = copyObject(cur->modules[q3_id]);

	((Fjord*)q3_out)->usedByQueries = lnext(((Fjord*)q3_out)->usedByQueries);
			
	split = makeNode(FSplit);
		
	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	
	StaticEddy->modules[0] = (Plan *) r_stem;
	StaticEddy->modules[1] = (Plan *) s_stem;
	StaticEddy->modules[2] = (Plan *) r_filter;
	StaticEddy->modules[3] = (Plan *) s_filter;
	StaticEddy->modules[4] = (Plan *) q2_filter;
	StaticEddy->modules[5] = (Plan *) q3_filter;
	StaticEddy->modules[6] = (Plan *) split;
	StaticEddy->modules[7] = (Plan *) q2_out;
	StaticEddy->modules[8] = (Plan *) q3_out;
		
	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		((Fjord *) StaticEddy->sources[i])->operator = i;
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		((Fjord *) StaticEddy->modules[i])->operator = i;
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	/* DIDN'T TOUCH, MAY NEED TO CHANGE */
	KeepOuterHashkeys(r_stem,0,0);
	KeepOuterHashkeys(s_stem,0,0);
		
	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s_stem);
	InitializeOutputs(numStaticSources,(Fjord *)q2_filter);
	InitializeOutputs(numStaticSources,(Fjord *)q3_filter);
	InitializeOutputs(numStaticSources,(Fjord *)q2_out);
	InitializeOutputs(numStaticSources,(Fjord *)q3_out);
	InitializeOutputs(numStaticSources,(Fjord *)r_filter);
	InitializeOutputs(numStaticSources,(Fjord *)s_filter);
	InitializeOutputs(numStaticSources,(Fjord *)split);
	
	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)r_filter);
		
	SetOutputQueue(r_source,(Fjord *) r_filter, (Fjord *)r_stem);
	SetOutputQueue(r_source,(Fjord *) r_stem, (Fjord *)s_stem);
	SetOutputQueue(r_source,(Fjord *) s_stem, (Fjord *)split);
	
	InitSplitOutput(split,2);
	SetSplitOutput(split,0,(Fjord *) q2_filter);
	SetSplitOutput(split,1,(Fjord *) q3_filter);
	
	SetOutputQueue(r_source,(Fjord *)q2_filter ,(Fjord *)q2_out);
	SetOutputQueue(r_source,(Fjord *)q3_filter ,(Fjord *)q3_out);
	
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)s_filter);

	SetOutputQueue(s_source,(Fjord *)s_filter,(Fjord *)s_stem);
	SetOutputQueue(s_source,(Fjord *)s_stem,(Fjord *)r_stem);
	SetOutputQueue(s_source,(Fjord *)r_stem,(Fjord *)split);
		
	InitSplitOutput(split,2);
	SetSplitOutput(split,0,(Fjord *)q2_filter);
	SetSplitOutput(split,1,(Fjord *)q3_filter);
	
	SetOutputQueue(s_source,(Fjord *)q2_filter,(Fjord *)q2_out);
	SetOutputQueue(s_source,(Fjord *)q3_filter,(Fjord *)q3_out);
	
	elog(DEBUG1,"Phew .. we now print the static plan ");
	elog(DEBUG1,"------------------------------------------------------------");
	pprint(StaticEddy);
	elog(DEBUG1,"------------------------------------------------------------");

}

/* Static Plan w/Tuple Lineage: TWO QUERIES, FOUR PREDICATES, GSFilter */
static void form_static_plan_5(Eddy *cur, Query *query) 
{
	ScanModule 	*r_scan,*s_scan;
	FSteM 		*r_stem, *s_stem;
	FOutput *output;
	GSFilter 	*rgs_filter, *sgs_filter;
			
	int r_source, s_source;
	int r_stemid, s_stemid;
	int rgsf_id, sgsf_id;
	int output_id;
	int numStaticSources = 2;
	int numStaticModules = 5;
	
	int i;
		
	bits8 r_bits[BASIZE(TCQMAXSOURCES)];
	bits8 s_bits[BASIZE(TCQMAXSOURCES)];

	/* Initializing the static shared plan */
	StaticEddy = makeNode(Eddy);
	StaticEddy->numSources = numStaticSources;
	StaticEddy->numModules = numStaticModules;

	r_source = 0;
	s_source = 1;

	r_stemid = 2;
	s_stemid = 3;

	rgsf_id = 0;
	sgsf_id = 1;
		
	/* All have same signature, so copy the single FOutput */
	output_id = 4;
	
	BitArrayClearAll(r_bits,TCQMAXSOURCES);
	BitArrayClearAll(s_bits,TCQMAXSOURCES);

	BitArraySetBit(r_bits,r_source);
	BitArraySetBit(s_bits,s_source);

 	/*
	 *  Create our plan objects 
	 */

	rgs_filter = (GSFilter *)cur->modules[rgsf_id];
	sgs_filter = (GSFilter *)cur->modules[sgsf_id];
	
	r_scan = copyObject(cur->sources[r_source]);
	s_scan = copyObject(cur->sources[s_source]);
		
	r_stem = copyObject(cur->modules[r_stemid]);
	s_stem = copyObject(cur->modules[s_stemid]);
			
	output = copyObject(cur->modules[output_id]);
			
	StaticEddy->sources = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numSources);
	StaticEddy->modules = (Plan **) palloc(sizeof(Plan *)*StaticEddy->numModules);
	
	StaticEddy->sources[0] = (Plan *) r_scan; 
	StaticEddy->sources[1] = (Plan *) s_scan; 
	
	StaticEddy->modules[0] = (Plan *) r_stem;
	StaticEddy->modules[1] = (Plan *) s_stem;
	StaticEddy->modules[2] = (Plan *) rgs_filter;
	StaticEddy->modules[3] = (Plan *) sgs_filter;
	StaticEddy->modules[4] = (Plan *) output;
			
	for (i = 0; i < StaticEddy->numSources; i++) 
	{
		((Fjord *) StaticEddy->sources[i])->operator = i;
		ProcessInitNode(StaticEddy->sources[i], cur->plan.state, (Plan *) cur);
	}
	for (i = 0; i < StaticEddy->numModules; i++) 
	{
		((Fjord *) StaticEddy->modules[i])->operator = i;
		ProcessInitNode(StaticEddy->modules[i], cur->plan.state, (Plan *) cur);
	}

	/* DIDN'T TOUCH, MAY NEED TO CHANGE */
	KeepOuterHashkeys(r_stem,0,0);
	KeepOuterHashkeys(s_stem,0,0);
		
	InitializeOutputs(numStaticSources,(Fjord *)r_scan);
	InitializeOutputs(numStaticSources,(Fjord *)s_scan);
	InitializeOutputs(numStaticSources,(Fjord *)r_stem);
	InitializeOutputs(numStaticSources,(Fjord *)s_stem);
	InitializeOutputs(numStaticSources,(Fjord *)rgs_filter);
	InitializeOutputs(numStaticSources,(Fjord *)sgs_filter);
	InitializeOutputs(numStaticSources,(Fjord *)output);
	
	/*
	 *  Driver is r
	 */
	SetOutputQueue(r_source,(Fjord *)r_scan ,(Fjord *)rgs_filter);
	SetOutputQueue(r_source,(Fjord *) rgs_filter, (Fjord *)r_stem);
	SetOutputQueue(r_source,(Fjord *) r_stem, (Fjord *)s_stem);
	SetOutputQueue(r_source,(Fjord *) s_stem, (Fjord *)output);
		
	/*
	 *  Driver is s
	 */
	SetOutputQueue(s_source,(Fjord *)s_scan,(Fjord *)sgs_filter);
	SetOutputQueue(s_source,(Fjord *)sgs_filter,(Fjord *)s_stem);
	SetOutputQueue(s_source,(Fjord *)s_stem,(Fjord *)r_stem);
	SetOutputQueue(s_source,(Fjord *)r_stem,(Fjord *)output);
	
	elog(DEBUG1,"Phew .. we now print the static plan ");
	elog(DEBUG1,"------------------------------------------------------------");
	pprint(StaticEddy);
	elog(DEBUG1,"------------------------------------------------------------");

}

/* copy of static plan 4 (new queries w/ new predicates) */
void
form_static_plan_6(Eddy *cur, Query *query) 
{
	form_static_plan_4(cur, query);
}

/* copy of static plan 5 (new queries w/ new predicates) */
void
form_static_plan_7(Eddy *cur, Query *query) 
{
	form_static_plan_5(cur, query);
}

#if 0
void
form_static_plan_8(Eddy *cur, Query *query)
{
}
#endif /* 0 */


/*
 * END SIGMOD GROSSNESS
 */


