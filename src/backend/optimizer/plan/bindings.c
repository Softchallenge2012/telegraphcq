/*-------------------------------------------------------------------------
 *
 * bindings.c
 *
 *		 Rutines to check all of the streams in a query to see if any of
 * them have bindings (or values which must be provided as inputs in order for
 * tuples to be produced as outputs).  If such streams exist, check to
 * ensure that these bindings will resolve to obtainable values when the
 * query is run.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "nodes/nodes.h"
#include "executor/nodeStrmScan.h"
#include "nodes/plannodes.h"
#include "nodes/parsenodes.h"
#include "parser/parse_wrapper.h"
#include "parser/parse_coerce.h"
#include "parser/parse_node.h"
#include "parser/parse_expr.h"
#include "optimizer/var.h"
#include "nodes/makefuncs.h"
#include "parser/parsetree.h"
#include "nodes/primnodes.h"
#include "utils/relcache.h"
#include "optimizer/eddy.h"
#include "optimizer/bindings.h"
#include "access/heapam.h"
#include "executor/executor.h"
#include "miscadmin.h"
#include "utils/fcache.h"
#include "utils/lsyscache.h"
/*
 * routines to check sources requiring bindings.  These routines either
 * find a non-dependant expression that can be used to evaluate the
 * binding, or return null
 */

/* find the target list to evaluate to produce the probe tuple */
static bool findBindingtlist(Eddy * e, StrmScan * s,
				 Index varno, int wrapperno, List **tlist);

/* given an attribue, recursivly find the expression which satisfies that
   binding.  This can occur as a result of a var=const qualification, or
   via reference to a var in a scan which is not itself a binding.
*/
static Node *findBindingExpr(Eddy * e, Index varno, AttrNumber varattno);


/* functions to check various eddy modules to see if they satisfy a binding. */
static Node *source_satisfies_binding(Eddy * e, Plan *p,
						 Index varno, AttrNumber varattno);
static Node *strmscan_satisfies_binding(Eddy * e, StrmScan * p,
						   Index varno, AttrNumber varattno);
static Node *seqscan_satisfies_binding(Eddy * e, SeqScan *p,
						  Index varno, AttrNumber varattno);
static Node *module_satisfies_binding(Eddy * e, Plan *p, Index varno, AttrNumber varattno, int moduleno);
static Node *stem_satisfies_binding(Eddy * e, SteM * s, Index varno, AttrNumber varattno, int moduleno);
static Node *filter_satisfies_binding(Eddy * e, Filter * f, Index varno, AttrNumber varattno);
static bool expr_can_satisify_binding(Node *n);

/* functions to produce appropriate target entries for the probe tuple's target
   list.
*/

static Var *getVar(Eddy * e, Index varno, AttrNumber attno);
static TargetEntry *getNullTargetEntry(Eddy * e, Index varno, AttrNumber attno);
static TargetEntry *getTargetEntryFromExpr(AttrNumber attno, Node *expr);
static bool beginProbeStrmScan(StrmScan * strmscan, HeapTuple t);
static bool probe_tuple_eq(HeapTuple t1, HeapTuple t2, TupleDesc td,
			   Bindinginfo * binfo, ExprContext *econtext);



#define MAXMODULES 1000
int			module_visited[MAXMODULES];

void
load_bindings_for_stream(Oid streamoid, StrmScan * strmscan)
{
	char * wrappertype=NULL;
	bool ispull=false;
	int			natts = strmscan->scansock.tupDesc->natts;
	TupleDesc tdesc = strmscan->scansock.tupDesc; 
	Oid		   *mapping_oids = NULL;
	Oid		   *wrapper_oids = NULL;
	int			mapping_oids_size = 0;
	int			i = 0;
	List	   *mappinginfo = NIL;
	List	   *wrapperinfo = NIL;
	List	   *iter = NIL;
	int			streamcol = 0,
				bindingcol = 0;
	key_value_entry *entry = NULL;
	int			mappingsize = sizeof(int) * natts;
	int                     defaultssize = sizeof(Const*)*natts;
	int                     optionalsize= sizeof(bool)*natts;
	bool		hasBindings = false;
	bool		wrapperhasbindings = false;

	strmscan->bindings = NULL;
	strmscan->numwrappers = 0;


	get_wrappers_for_stream(streamoid, &wrapper_oids,
							&mapping_oids, &mapping_oids_size);

	if (mapping_oids_size == 0)
		return;

	strmscan->bindings = palloc(sizeof(Bindinginfo *) * mapping_oids_size);
	strmscan->numwrappers = 0;
	memset(strmscan->bindings, 0, sizeof(Bindinginfo *) * mapping_oids_size);


	for (i = 0; i < mapping_oids_size; i++)
	{
		if(mapping_oids[i] == InvalidOid) 
			continue;
		strmscan->numwrappers++;
		wrapperhasbindings = false;
		strmscan->bindings[i] = makeNode(Bindinginfo);
		strmscan->bindings[i]->wrapperoid = wrapper_oids[i];
		strmscan->bindings[i]->mappingoid = mapping_oids[i];
		strmscan->bindings[i]->atts = strmscan->scansock.tupDesc->natts;
		strmscan->bindings[i]->mapping = palloc(mappingsize);
		strmscan->bindings[i]->defaults = palloc(defaultssize);
		strmscan->bindings[i]->optional = palloc(optionalsize);

		memset(strmscan->bindings[i]->mapping, -1, mappingsize);
		memset(strmscan->bindings[i]->defaults, 0, defaultssize);
		memset(strmscan->bindings[i]->optional, 0, optionalsize);
		
		mappinginfo = load_attr_info(mapping_oids[i]);
		wrapperinfo = load_attr_info(wrapper_oids[i]);
		entry = get_entry(WRAPPERTYPEKEY, wrapperinfo);
		if(entry)
			wrappertype=entry->value;
		if(wrappertype && strcasecmp(wrappertype, PULLWRAPPER_NAME)==0)
			{

				ispull=true;
				foreach(iter, mappinginfo)
					{
						entry = lfirst(iter);
						if ((sscanf(entry->key, "STREAM%d", &streamcol) == 1) &&
							(sscanf(entry->value, "BINDING%d", &bindingcol) == 1))
							{
							  key_value_entry *tmp=NULL;
							  char key[1024];
								strmscan->bindings[i]->mapping[streamcol - 1] = bindingcol;
								sprintf(key, "BINDING%d_optional", bindingcol);
								tmp = get_entry(key, mappinginfo);
								if(tmp != NULL &&
								   strcasecmp(tmp->value, "true") == 0)
								  {
								    strmscan->bindings[i]->optional[streamcol-1] = TRUE;
								  }
								else
								  {
								    strmscan->bindings[i]->optional[streamcol-1] = FALSE;
								  }
								sprintf(key, "BINDING%d_default", bindingcol);
								tmp = get_entry(key, mappinginfo);
								if(tmp != NULL)
								  {
								    Const * consttmp = make_const(makeString(tmp->value));
								    Const * converted = (Const*)coerce_type((Node*)consttmp, consttmp->consttype,
												     tdesc->attrs[streamcol-1]->atttypid,
												    COERCION_IMPLICIT, COERCE_IMPLICIT_CAST);
								    strmscan->bindings[i]->defaults[streamcol-1] = converted;
												    
								  }
								else
								  {
								    strmscan->bindings[i]->defaults[streamcol-1] = NULL;
								  }

							       
							}
						
						
					}
				
				wrapperhasbindings = true;
				hasBindings = true;
		}
		else
			{
				wrapperhasbindings=false;
			}

		entry = get_entry("dependant_stream",wrapperinfo);
		if(entry && strcasecmp(entry->value,"true")==0)
		  strmscan->dependant_stream=TRUE;
		else
		  strmscan->dependant_stream=FALSE;
		entry = get_entry("dependant_stream",mappinginfo);
		if(entry && strcasecmp(entry->value,"true")==0)
		  strmscan->dependant_stream=TRUE;

		

		free_attr_info(mappinginfo);
		free_attr_info(wrapperinfo);

		if (!wrapperhasbindings)
		{
			pfree(strmscan->bindings[i]);
			strmscan->bindings[i] = NULL;
		}


	}
	pfree(mapping_oids);
	if (!hasBindings)
	{
		pfree(strmscan->bindings);
		strmscan->bindings = NULL;
	}




}




bool
getBindingInformation(Eddy * e)
{

	StrmScan   *src = NULL;
	int			i = 0;
	int			j = 0;

	bool		ret = true;

	for (i = 1; i <= e->numSources; i++)
	{
		if (e->sources[i] == NULL)
			continue;

		src = (StrmScan *) e->sources[i - 1];
		if (!IsA(src, StrmScan) || src->numwrappers == 0)
			continue;
		if (!src->bindings)
			continue;
		for (j = 1; j <= src->numwrappers; j++)
		{
			if (!src->bindings[j - 1])
				continue;
			if (!findBindingtlist(e, src, sourceGetVarNo((Plan *) src), j,
								  &(src->bindings[j - 1]->targetlist)))
			{
				ret = false;
				src->bindings=NULL;
				break;
			}
			else
				load_eq_opers_for_bindinginfo(src->bindings[j - 1]);

		}

	}

	return ret;
}

static bool
findBindingtlist(Eddy * e, StrmScan * scan, Index varno,
				 int wrapperno, List **tlist)
{
	Bindinginfo *b = NULL;
	int			col;
	bool		passed = true;
	Node	   *bindingExpr = NULL;


	b = scan->bindings[wrapperno - 1];

	for (col = 1; col <= b->atts; col++)
	{
		if (b->mapping[col - 1] < 0)
		{
			(*tlist) = lappend((*tlist),
							   getNullTargetEntry(e, varno, col));

			continue;
		}
		else
		{
			memset(module_visited, 0, MAXMODULES * sizeof(int));
			bindingExpr = findBindingExpr(e, varno, col);
			if (bindingExpr == NULL)
			{
				elog(NOTICE, "binding for relation %s, column %s is not satisfied",
				     get_rel_name(scan->streamoid), 
				     get_attname(scan->streamoid, col));
				passed = false;
				break;
			}
			else
			{
				(*tlist) = lappend((*tlist),
				getTargetEntryFromExpr(col, bindingExpr));
				elog(DEBUG1, "binding satisfied for var %d, column %d: %s",
					 varno, col, nodeToString(bindingExpr));

			}

		}
	}

	return passed;
}

static Node *
findBindingExpr(Eddy * e, Index varno, AttrNumber varattno)
{
	int			i = 0;
	Node	   *ret = NULL;
	Const      *def= NULL;
	bool       optional=false;
	bool       optionalset=false;
	int			idx = varNoGetSourceArrayIndex(e, varno);
	Plan	   *source = NULL;
	StrmScan *strm = NULL;
	Assert(idx >= 0);
	source = e->sources[idx];
	if(IsA(source, StrmScan))
	  {
	    strm =(StrmScan*)source;
	    if(IsA(strm, StrmScan))
	      {
		for(i=0; i < strm->numwrappers; i++)
		  {
		    if(def != NULL && 
		       strm->bindings[i]->defaults[varattno-1] != NULL)
		      {
			elog(ERROR, "stream has two wrappers with defaults for column %d",
			     varattno);
		      }

		    if(optionalset && 
		       (optional != strm->bindings[i]->optional[varattno-1]))
		      {
			elog(ERROR, "stream has two wrappers different isoptional settings for column %d",
			     varattno);
		      }
		    if(strm->bindings && strm->bindings[i] != NULL)
		      {
			def =   strm->bindings[i]->defaults[varattno-1];
			optional = optional ||  strm->bindings[i]->optional[varattno-1];
			optionalset=true;
		      }
		  }
	      }
	  }
	ret = source_satisfies_binding(e, source, varno, varattno);

	if (ret == NULL)
	{
		for (i = 0; i < e->numModules; i++)
		{
			if (e->modules[i] == NULL)
				continue;
			ret = module_satisfies_binding(e,
										   e->modules[i],
										   varno, varattno, i);
			if (ret)
				break;
		}
	}

	if(ret == NULL)
	  {
	    elog(NOTICE, "using default for var %d, col %d", varno, varattno);
	    ret = (Node*)def;
	  }
	if(optional == true && ret == NULL)
	  {
	    elog(NOTICE, "optional column for var %d, col %d was omitted", varno, varattno);
	    ret = (Node*)getNullTargetEntry(e, varno, varattno);
	  }
	return ret;

}


static Node *
source_satisfies_binding(Eddy * e,
						 Plan *p, Index varno,
						 AttrNumber varattno)
{

	Node	   *ret = NULL;

	switch (nodeTag(p))
	{
		case T_StrmScan:
			ret = strmscan_satisfies_binding(e, (StrmScan *) p, varno, varattno);
			break;
		case T_SeqScan:
			ret = seqscan_satisfies_binding(e, (SeqScan *) p, varno, varattno);
			break;
		default:
			elog(ERROR, "unknown module type in source_satisfies_bindings");
	}
	return ret;
}

static Node *
strmscan_satisfies_binding(Eddy * e, StrmScan * p,
						   Index varno, AttrNumber varattno)
{
	int			i = 0;
	StrmScan   *strmscan = (StrmScan *) p;
	Bindinginfo *b = NULL;
	bool		notbinding = true;

	if (!IsA(p, StrmScan) || strmscan->numwrappers == 0)
		return NULL;
	else
	{
		for (i = 1; i <= strmscan->numwrappers; i++)
		{
			if (strmscan->bindings == NULL)
				continue;

			b = strmscan->bindings[i - 1];
			if (b == NULL)
				continue;

			if (b->mapping[varattno - 1])
			{
				if (b->targetlist != NULL &&
					length(b->targetlist) >= varattno)
				{
					TargetEntry *t = NULL;

					t = nth(varattno - 1,
							b->targetlist);
					return t->expr;
				}
				else
				{
					notbinding = false;
					break;
				}
			}
		}
	}

	if (notbinding)
		return (Node *) getVar(e, varno, varattno);
	else
		return NULL;
}

static Node *
seqscan_satisfies_binding(Eddy * e, SeqScan *p,
						  Index varno, AttrNumber varattno)
{
	return (Node *) getVar(e, varno, varattno);
}


static Node *
module_satisfies_binding(Eddy * e, Plan *p, Index varno, AttrNumber varattno, int moduleno)
{
	Node	   *ret = NULL;

	switch (nodeTag(p))
	{
		case T_SteM:
			ret = (Node *) stem_satisfies_binding(e, (SteM *) p, varno,
												  varattno, moduleno);
			break;
		case T_Filter:
			ret = (Node *) filter_satisfies_binding(e, (Filter *) p, varno, varattno);
			break;
		default:
			elog(ERROR, "unknown plan type in module_stisfies_var");

	}
	return ret;

}

Node *
stem_satisfies_binding(Eddy * e, SteM * s, Index varno, AttrNumber varattno, int moduleno)
{
	Var		   *thisvar = NULL;
	Var		   *impliedvar = NULL;
	Var		   *v1 = NULL;
	Var		   *v2 = NULL;


	List	   *innervars = pull_var_clause(s->innerhashkey, false);
	List	   *outervars = pull_var_clause(s->outerhashkey, false);

	if (length(innervars) != 1 || length(outervars) != 1)
		return false;
	else
	{
		v1 = lfirst(innervars);
		v2 = lfirst(outervars);
		if (v1->varnoold == varno &&
			v1->varoattno == varattno)
		{
			thisvar = v1;
			impliedvar = v2;
		}
		else if ((v2->varnoold == varno) &&
				 (v2->varattno == varattno))
		{
			thisvar = v2;
			impliedvar = v1;
		}

	}
	if (thisvar == NULL || impliedvar == NULL)
		return NULL;
	else
	{
		if (module_visited[moduleno] > 0)
			return NULL;
		module_visited[moduleno]++;
		return findBindingExpr(e,
							   impliedvar->varnoold,
							   impliedvar->varoattno);
	}

}

Node *
filter_satisfies_binding(Eddy * e, Filter * f, Index varno, AttrNumber varattno)
{
	bool		foundvar = false;
	bool		foundconst = false;
	Var		   *v = NULL;
	Node	   *c = NULL;

	Expr	   *eqexpr = (Expr *) f->filterqual;		/* not be an oper until
														 * next line */

	if (nodeTag(eqexpr) != T_Expr ||
		eqexpr->opType != OP_EXPR ||
		!eqexpr->oper ||
		((Oper *) eqexpr->oper)->optrtype != EQ_OPTR)
		return NULL;
	else
	{
		List	   *iter = NIL;

		foreach(iter, eqexpr->args)
		{
			if (IsA(lfirst(iter), Var))
			{
				v = (Var *) lfirst(iter);
				if (v->varno == varno && v->varoattno == varattno)
					foundvar = true;
			}
			else if (expr_can_satisify_binding(lfirst(iter)))
			{
				foundconst = true;
				c = lfirst(iter);
			}

		}

		if (foundvar && foundconst)
			return c;
		else
			return NULL;


	}

}


static bool
expr_can_satisify_binding(Node *n)
{
	/*
	 * TBD:this should be a more satisfying definition as it doesn't take
	 * into account constant expressions.
	 */
	return IsA(n, Const);
}




static TargetEntry *
getTargetEntryFromExpr(AttrNumber attno, Node *expr)
{
	Oid			type_id = exprType(expr);
	Oid			type_mod = exprTypmod(expr);
	char	   *colname = "?colname?";
	Resdom	   *resnode = NULL;



	resnode = makeResdom(attno,
						 type_id,
						 type_mod,
						 colname,
						 false);

	return makeTargetEntry(resnode, expr);

}

static TargetEntry *
getNullTargetEntry(Eddy * e, Index varno, AttrNumber attno)
{
	/* first, find the type */
	Oid			reloid = rt_fetch(varno, e->rangetable)->relid;
	Relation	r = RelationIdGetRelation(reloid);
	Const	   *nullconst = makeNullConst(r->rd_att->attrs[attno - 1]->atttypid);

	RelationClose(r);
	return getTargetEntryFromExpr(attno, (Node *) nullconst);
}

static Var *
getVar(Eddy * e, Index varno, AttrNumber attno)
{
	Oid			reloid = rt_fetch(varno, e->rangetable)->relid;
	Relation	r = RelationIdGetRelation(reloid);
	Oid			coltype = r->rd_att->attrs[attno - 1]->atttypid;
	int32		coltypemod = r->rd_att->attrs[attno - 1]->atttypmod;

	RelationClose(r);
	return makeVar(varno, attno,
				   coltype, coltypemod,
				   0);
}


void
freeBindinginfo(Bindinginfo * b)
{
	/* some leaks here... projinfo etc */
	if (b->mapping)
		pfree(b->mapping);
	if (b->targetlist)
		freeList(b->targetlist);
	pfree(b);
	return;
}

bool
doneWithProbe(Plan *p, HeapTuple t)
{
	bool		ret = false;

	if (UseCQEddy == false)
		return t == NULL;

	switch (nodeTag(p))
	{
		case T_StrmScan:
			ret = doneWithProbeStrmScan((StrmScan *) p, t);
			break;
		case T_SeqScan:
			{
				SeqScan    *s = (SeqScan *) p;

				if (IsAStream(s->scanstate->css_currentRelation->rd_rel->relkind))
					ret = false;
				else
					ret = (t == NULL);
			}

			break;
		default:
			break;

	}
	return ret;
}

bool
beginProbe(Plan *p, HeapTuple t)
{
	/*
	 * true means the probe must be processed, false means its alreay
	 * pending
	 */
	bool		ret = true;

	if (UseCQEddy == false)
		return t == NULL;

	switch (nodeTag(p))
	{
		case T_StrmScan:
			ret = beginProbeStrmScan((StrmScan *) p, t);
			break;
		default:
			break;

	}
	return ret;
}

bool
beginProbeStrmScan(StrmScan * strmscan, HeapTuple t)
{
	bool		alreadyPending;
	int			i;
	int			attrno;


	TupleDesc	td = strmscan->scansock.tupDesc;

	for (i = 0; i < strmscan->numwrappers; i++)
	{
		Bindinginfo *b = strmscan->bindings[i];

		if (b == NULL)
			continue;
		for (attrno = 1; attrno <= b->atts; attrno++)
		{
			if (b->mapping[attrno - 1] >= 0)
			{
				bool		isNull = false;

				heap_getattr(t, attrno, td, &isNull);
				if (isNull)
					return false;
			}
		}
	}

	strmscan->pending_probes =
		add_probe(t, td,
				  strmscan->pending_probes,
				  strmscan->bindings,
				  strmscan->numwrappers,
                  (GetScanStateStrmScan(strmscan))->cstate.cs_ExprContext,
				  &alreadyPending);

	return !alreadyPending;
}



bool
doneWithProbeStrmScan(StrmScan * strmscan, HeapTuple t)
{
	/*
	 * an end of probe is indicated by a tuple which has NULLS in all
	 * non-binding fields and values in all other fields
	 */
	int			tuplecol = 0;
	int			wrapperno = 0;
	TupleDesc	td = strmscan->scansock.tupDesc;
	bool		isNull = false;
	Datum		colvalue;
	bool		hasbinding = false;


	if (t == NULL)
		return false;
	if (!strmscan->bindings)
		return false;

	for (tuplecol = 1; tuplecol <= td->natts; tuplecol++)
	{
		colvalue = heap_getattr(t, tuplecol, td, &isNull);
		hasbinding = false;

		for (wrapperno = 1; wrapperno <= strmscan->numwrappers; wrapperno++)
		{
			if (strmscan->bindings[wrapperno - 1] &&
			strmscan->bindings[wrapperno - 1]->mapping[tuplecol - 1] > 0)
				hasbinding = true;
		}

		if (!((hasbinding && !isNull) || (!hasbinding && isNull)))
			return false;
	}

	strmscan->pending_probes =
		remove_probe(t, td,
					 strmscan->pending_probes,
					 strmscan->bindings,
					 strmscan->numwrappers,
                     (GetScanStateStrmScan(strmscan))->cstate.cs_ExprContext);


	return true;
}



void
load_eq_opers_for_bindinginfo(Bindinginfo * b)
{
	List	   *iter = NIL;
	TargetEntry *tle = NULL;


	Oid			exprtype = InvalidOid;
	int			colno = 0;

	int			eqoperssize = sizeof(Oper) * b->atts;

	if (b->eq_opers)
		pfree(b->eq_opers);
	b->eq_opers = palloc(eqoperssize);
	memset(b->eq_opers, 0, eqoperssize);

	foreach(iter, b->targetlist)
	{
		tle = lfirst(iter);


		if (!IsA(tle->expr, Const) ||
			!((Const *) tle->expr)->constisnull)
		{
			exprtype = exprType(tle->expr);
			b->eq_opers[colno] = lookup_oper("=", exprtype, exprtype);
			Assert(b->eq_opers[colno] != NULL);
		}
		else
			b->eq_opers[colno] = NULL;

		colno++;
	}

}

bool
probe_tuple_eq(HeapTuple t1, HeapTuple t2, TupleDesc td, Bindinginfo * binfo,
			   ExprContext *econtext)
{
	Datum		valt1 = PointerGetDatum(NULL);
	Datum		valt2 = PointerGetDatum(NULL);
	bool		isNullt1 = false;
	bool		isNullt2 = false;
	Datum		result = PointerGetDatum(NULL);


	int			i;

	for (i = 1; i < td->natts; i++)
	{
		valt1 = heap_getattr(t1, i, td, &isNullt1);
		valt2 = heap_getattr(t2, i, td, &isNullt2);

		if (isNullt1 != isNullt2)
			return false;
		else if (binfo->eq_opers[i - 1] != NULL)
		{


			load_oper(binfo->eq_opers[i - 1]);
			result = DirectFunctionCall2(binfo->eq_opers[i - 1]->op_fcache->func.fn_addr,
										 valt1,
										 valt2);
			if (DatumGetBool(result) == FALSE)
				return false;

		}

	}
	return true;

}

List *
add_probe(HeapTuple probe, TupleDesc td,
		  List *pending_probes,
		  Bindinginfo ** binfo,
		  int binfosize,
		  ExprContext *econtext,
		  bool *alreadyPending)
{
	int			i;

	List	   *iter = NIL;
	HeapTuple	listtup;

	*alreadyPending = false;
	foreach(iter, pending_probes)
	{
		listtup = lfirst(iter);
		for (i = 0; i < binfosize; i++)
		{

			if (probe_tuple_eq(listtup,
							   probe,
							   td,
							   binfo[i],
							   econtext))
			{
				*alreadyPending = true;
				return pending_probes;
			}
		}
	}

	pending_probes = lcons(heap_copytuple(probe), pending_probes);
	return pending_probes;
}




List *
remove_probe(HeapTuple finished, TupleDesc td,
			 List *pending_probes,
			 Bindinginfo ** binding_info,
			 int binfosize,
			 ExprContext *econtext)
{
	List	   *iter = NIL;
	HeapTuple	listtup;
	List	   *to_be_removed = NIL;
	int			i;

	foreach(iter, pending_probes)
	{
		listtup = lfirst(iter);
		for (i = 0; i < binfosize; i++)
		{

			if (probe_tuple_eq(finished, listtup, td, binding_info[i],
							   econtext))
			{
				to_be_removed = lcons(listtup, to_be_removed);

			}

		}

	}

	foreach(iter, to_be_removed)
	{
		pending_probes = lremove(lfirst(iter), pending_probes);
		heap_freetuple(lfirst(iter));

	}
	return pending_probes;
}
