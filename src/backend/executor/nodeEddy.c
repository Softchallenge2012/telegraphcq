/*-------------------------------------------------------------------------
 *
 * nodeEddy.c
 *	  Routines to handle Eddy execution
 *	  -- May redirect some of the routines to nodeSQEddy.c or
 *		 nodeCQEddy.c
 *
 * This respects the new eddy operator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeEddy.c,v 1.99 2004/02/13 01:44:00 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "postgres.h"
#include "miscadmin.h"
#include "misceddy.h"

#include "executor/executor.h"
#include "executor/execdebug.h"
#include "executor/nodeEddy.h"
#include "executor/nodeGSFilter.h"
#include "executor/nodeFjord.h"
#include "executor/nodeSteM.h"
#include "executor/nodeFSteM.h"
#include "nodes/copyrte.h"
#include "optimizer/clauses.h"
#include "optimizer/eddy.h"
#include "parser/parsetree.h"
#include "rewrite/rewriteManip.h"
#include "rqueue/controlq.h"
#include "rqueue/planq.h"
#include "rqueue/rqdest.h"
#include "tcop/pquery.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "utils/lsyscache.h"

#define EDDY_SEED 1

void		debugEddy(Eddy * e, IntermediateHeapTuple iht);
void		printOpBits(Eddy * e, bits8 *bits);
void		printSources(Eddy * e, IntermediateHeapTuple iht);
bool		canRemoveQuery(Eddy * node);
void		printEddyStats_debug(Eddy * e);

/* Static functions used for printing */	/* @BdstemsAD */
char	   *printSourceNamesGivenBitMap(Eddy * node, int bitmap);
void		printRoutingPolicy(Eddy * node);
void		printAllHashTables(Eddy * eddy);

int			chooseSourceRandom(Eddy * e);
char	   *getNodeInformation(Eddy * eddy, Node *node);

extern Var *get_leftop(Expr *clause);
extern Var *get_rightop(Expr *clause);

int
chooseSourceRandom(Eddy * e)
{
	int			i = random() % e->numSources;

/*	   elog(DEBUG, "Choosing from source number %d of %d", i, e->numSources); */
	return i;
}


/* ----------------------------------------------------------------
 *		ExecInitEddy
 * ----------------------------------------------------------------
 */
bool
ExecInitEddy(Eddy * node, EState *estate, Plan *parent)
{
	if (node->cacq)
		return ExecInitCQEddy(node, estate, parent);
	else
		return ExecInitSQEddy(node, estate, parent);
}

int								/* @BdstemsAD */
ExecCountSlotsEddy(Eddy * node)
{
    int			i,
                nslots = 0;

    if(! node->cacq) 
        nslots += ExecCountSlotsSQEddy(node); 

    for (i = 0; i < node->numSources; i++)
    {
        if (node->sources[i] == NULL)
			continue;
		nslots += ExecCountSlotsNode((Plan *) node->sources[i]);
	}
	for (i = 0; i < node->numModules; i++)
	{
		if (node->modules[i] == NULL)
			continue;
		if (!IsA(node->modules[i], Filter))
			nslots += ExecCountSlotsNode((Plan *) node->modules[i]);
	}
	return nslots;
}

/*
 * Stack functions
 */
bool
empty(Eddy * node)
{
	return (node->eddystate->stackptr == -1);
}

bool
full(Eddy * node)
{
	return (node->eddystate->stackptr == TCQMAXOPERATORS);
}

void
push(Eddy * node, Node *p)
{
	node->eddystate->stackptr++;
	Assert(!full(node));
	node->eddystate->stack[node->eddystate->stackptr] = p;
}

Node *
pop(Eddy * node)
{
	Node	   *p;

	Assert(!empty(node));
	p = node->eddystate->stack[node->eddystate->stackptr];
	node->eddystate->stackptr--;
	return p;
}

Node *
top(Eddy * node)
{
	Assert(!empty(node));
	return node->eddystate->stack[node->eddystate->stackptr];
}

/* ----------------------------------------------------------------------------
 *	   OLD EDDY STUFF ... use with care
 * ----------------------------------------------------------------------------
 */
void
setReadyBits(IntermediateHeapTuple iht, Eddy * e)
{
	int			i;				/* Index of sources */

	for (i = 0; i < __MAX_RELATIONS__; i++)
	{
		if (BitArrayBitIsSet(iht->t_sources, i))
			break;
	}

	if (i >= __MAX_RELATIONS__)
		elog(ERROR, "setReadyBits: no source is set in IHT");

#ifndef DISABLE_CQ_SUPPORT

	/*
	 * Now set ready bits
	 */
	BitArrayClone(iht->t_ready, e->eddystate->operators[i], TCQMAXOPERATORS);
#endif
}

void
clearCompletionBits(IntermediateHeapTuple iht, List *queryIds)
{
#ifndef DISABLE_CQ_SUPPORT
	List	   *l = queryIds;

	/* Setting all t_queriesCompleted is done in initFromBaseTuple */
	/* BitArraySetAll(iht->t_queriesCompleted, TCQMAXQUERIES); */
	while (l)
	{
		BitArrayClearBit(iht->t_queriesCompleted, lfirsti(l));
		l = lnext(l);
	}
#endif
}

/* ----------------------------------------------------------------
 *		ExecEddy
 *
 *		Redirect.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecEddy(Eddy * node)
{
	if (node->cacq)
		return ExecCQEddy(node);
	else
		return ExecSQEddy(node);
}


/* ----------------------------------------------------------------
 *		ExecEndEddy
 *
 *		Redirect.
 * ----------------------------------------------------------------
 */
void
ExecEndEddy(Eddy * node)
{
	if (node->cacq)
		ExecEndCQEddy(node);
	else
		ExecEndSQEddy(node);
}

void
debugEddy(Eddy * e, IntermediateHeapTuple iht)
{
	printSources(e, iht);
#ifndef DISABLE_CQ_SUPPORT
	printOpBits(e, iht->t_ready);
	printOpBits(e, iht->t_done);;
#endif
}

void
printSources(Eddy * e, IntermediateHeapTuple iht)
{
	int			i = 0;

	for (i = 0; i < TCQMAXSOURCES; i++)
	{
		if (BitArrayBitIsSet(iht->t_sources, i))
		{
			elog(DEBUG1, "iht contains a tuple for source %d", i);
			pprint(e->sources[i]);
		}
	}
}

void
printOpBits(Eddy * e, bits8 *bits)
{
	int			i = 0;

	for (i = 0; i < TCQMAXOPERATORS; i++)
	{
		if (BitArrayBitIsSet(bits, i))
		{
			elog(DEBUG1, " bit is set for module %d", i);
			pprint(e->modules[i]);
		}
	}
}

void
printAllHashTables(Eddy * node)
{
	int			j;

	for (j = 0; j < node->numModules; j++)
	{
		Assert(IsA(node->modules[j], SteM));

		printf("=========================== SteM %s =========================== \n",
			   getNodeInformation(node, (Node *) ((SteM *) node->modules[j])->innerhashkey));

		printSteMHashJoinTable(((SteM *) node->modules[j])->stemstate->stemhashtable,
							   node->base_tuple_descs);
	}
}

void
printEddyStats(Eddy * e)
{
	int			i;
	MemoryContext 		oldcontext;
 
  	oldcontext = MemoryContextSwitchTo(e->eddystate->debugctx);

	/* First the stem stats */
	for (i = 0; i < e->numModules; i++)
	{
		if (e->modules[i] == NULL)
			continue;
		if (IsA(e->modules[i], SteM))
		{
			printf("stats: SteM %s : ", getNodeInformation(e, ((Node *) ((SteM *) e->modules[i])->innerhashkey)));
			SteMPrintData((SteM *) e->modules[i]);
		}
	}

	/* The total tuples produced */
	printf("stats: The eddy has processed %d tuples till now\n", e->numTuplesProcessed);
	MemoryContextReset(e->eddystate->debugctx);
	MemoryContextSwitchTo(oldcontext);
}


void
printEddyStats_debug_with_level(Eddy * e, int level)
{
	int					i;
	struct timeval 		currenttime;
	MemoryContext 		oldcontext;

 	oldcontext = MemoryContextSwitchTo(e->eddystate->debugctx);

	gettimeofday(&currenttime, 0);

	elog(level, "======== PRINTING EDDY STATS ================================== at time %d.%.6d",
		 (int) currenttime.tv_sec, (int) currenttime.tv_usec);

/* 	elog(level, "Weights :"); */
/* 	for (i = 0; i < e->numModules; i++) */
/* 	{ */

/* 		if (e->modules[i] && IsA(e->modules[i], FSteM)) */
/* 		{ */
/* 			FSteM	   *fstem = (FSteM *) e->modules[i]; */
/* 			Var		   *var = fstem->protostem->innerhashkey_var; */

/* 			if (IsA(var, Var)) */
/* 				elog(level, "%s : %f ", getNodeInformation(e, (Node *) var), e->eddystate->weights[i]); */
/* 			else */
/* 				elog(level, "%f ", e->eddystate->weights[i]); */

/* 			elog(level, "inserted = %d, probed = %d, returned = %d", fstem->fstem_num_tuples_inserted, */
/* 				 fstem->fstem_num_tuples_probed, fstem->fstem_num_tuples_returned); */
/* 		} */
/* 		else */
/* 			elog(level, "%f ", e->eddystate->weights[i]); */
/* 	} */

	elog(level, "Printing stats for all modules");
	/* Print stats for all modules */
	for (i = 0; i < e->numModules; i++)
	{
		if (e->modules[i])
		{
			elog(level, "Module  %d", i);
			ProcessPrintStats(e->modules[i], (Plan *)e);
		}
	}
	elog(level, "Printing stats for all sources");
	for (i = 0; i < e->numSources; i++)
	{
		if (e->sources[i])
		{
			elog(level, "Source %d", i);
			ProcessPrintStats(e->sources[i], (Plan *)e);
		}
	}
	

	/* The total tuples produced */
	elog(level, "stats: The eddy has processed %d tuples till now",
		 e->numTuplesProcessed);
 
    elog(level, "stats: The total delay for %d tuples output is %f",
		 e->numTuplesOutput, e->total_delay);
	MemoryContextReset(e->eddystate->debugctx);
	elog(level, "======== DONE PRINTING EDDY STATS ==========================");
	MemoryContextReset(e->eddystate->debugctx);
	MemoryContextSwitchTo(oldcontext);
}

void
printEddyStats_debug(Eddy * e)
{
	printEddyStats_debug_with_level(e, INFO);
}



void							/* @BdstemsAD */
printRoutingPolicy(Eddy * node)
{
	int			i;

	int			num_relations_in_query = node->num_relations_in_query;
	MemoryContext 		oldcontext;

 	oldcontext = MemoryContextSwitchTo(node->eddystate->debugctx);

	for (i = 0; i < (1 << num_relations_in_query); i++)
	{
		List	   *insertSteMs = node->eddystate->insertRoutingPolicy[i];
        List       *selections = node->eddystate->selectionsRoutingPolicy[i];
		List	   *probeSteMs = node->eddystate->probeRoutingPolicy[i];
		List	   *iterator;

		printf("Type %s :\n", printSourceNamesGivenBitMap(node, i));

		if (insertSteMs != NIL)
		{
			foreach(iterator, insertSteMs)
			{
				SteM	   *stem = lfirst(iterator);

				assert(IsA(stem, SteM));

				printf("			Inserted into %s \n",
				  getNodeInformation(node, (Node *) stem->innerhashkey));
			}
		}

		if (selections != NIL)
		{
			foreach(iterator, selections)
			{
                Filter *filter = (Filter *) lfirst(iterator);

				assert(IsA(filter, Filter));

				printf("			Apply selection %s \n", getNodeInformation(node, filter->filterqual));
			}
		}

		if (probeSteMs != NIL)
		{
			foreach(iterator, probeSteMs)
			{
				if (IsA(lfirst(iterator), SteM))
				{
					SteM	   *stem = lfirst(iterator);

					printf("			Probed into on %s \n",
						   getNodeInformation(node, (Node *) stem->innerhashkey));
				}
				else if (IsA(lfirst(iterator), IndexSteM))
				{
					/* IndexSteM *istem = lfirst(iterator); */
					printf("			Probed into an index\n");
				}
			}
		}
		else
			printf("			go to output\n");
	}
	MemoryContextReset(node->eddystate->debugctx);
	MemoryContextSwitchTo(oldcontext);
}	/* @EdstemsAD */

char	   *					/* @BdstemsAD */
printSourceNamesGivenBitMap(Eddy * node, int bitmap)
{
	int			i;
	bool		first = true;
	StringInfoData str;

	/* see stringinfo.h for an explanation of this maneuver */
	initStringInfo(&str);

	appendStringInfo(&str, "[");
	for (i = 0; i < TCQMAXSOURCES; i++)
	{
		if (bitmap & (1 << i))
		{
			RangeTblEntry *rte = rt_fetch(i + 1, node->rangetable);
			char	   *relname;

			/* Assume it's on a real relation */
			Assert(rte->rtekind == RTE_RELATION);

			if (!first)
				appendStringInfo(&str, ",");
			first = false;

			/* We only show the rel name, not schema name */
			relname = get_rel_name(rte->relid);

			if (rte->alias == NULL)
				appendStringInfo(&str, "%s", relname);
			else
				appendStringInfo(&str, "%s.%s", rte->alias->aliasname, relname);

			pfree(relname);
		}
	}
	appendStringInfo(&str, "]");
	return str.data;
}	/* @EdstemsAD */

/*
 *	TBD TBD TBD - Amol
 *
 *	Need to:
 *
 *	  Confirm that calls to get_rel_name and use of
 *	  RangeTblEntry and Alias is correct ..
 *
 */
static void
_getVarInformation(StringInfo str, Eddy * node, Var *var)
{
	RangeTblEntry *rte = rt_fetch(var->varnoold, node->rangetable);
	char	   *relname;
	char	   *attrname = ((Value *) nth(var->varoattno - 1, rte->eref->colnames))->val.str;

	/* Assume it's on a real relation */
	Assert(rte->rtekind == RTE_RELATION);

	/* We only show the rel name, not schema name */
	relname = get_rel_name(rte->relid);

	if (rte->alias == NULL)
		appendStringInfo(str, "%s.%s", relname, attrname);
	else
		appendStringInfo(str, "%s.%s.%s", rte->alias->aliasname, relname, attrname);

	/* Free the relname palloc'ed by get_rel_name ! */
	pfree(relname);
}


static void
_getNodeInformation(StringInfo str, Eddy * eddy, Node *node)
{
	switch (nodeTag(node))
	{
		case T_Var:
			_getVarInformation(str, eddy, (Var *) node);
			break;

		case T_Const:
			appendStringInfo(str, "%d", (int) ((Const *) node)->constvalue);
			break;

		case T_Expr:
			{
				Expr	   *expr = (Expr *) node;
				Oper	   *oper;
				OptrType	optrtype;

				if (expr->opType == OP_EXPR)
				{
                    oper = (Oper *) expr->oper;
                    optrtype = oper->optrtype;
                    _getNodeInformation(str, eddy, (Node *) get_leftop(expr));
                    switch (optrtype)
                    {
                        case LT_OPTR:
                            appendStringInfo(str, " < ");
                            break;
                        case LE_OPTR:
                            appendStringInfo(str, " <= ");
                            break;
                        case GT_OPTR:
                            appendStringInfo(str, " > ");
                            break;
                        case GE_OPTR:
                            appendStringInfo(str, " >= ");
                            break;
                        case EQ_OPTR:
                            appendStringInfo(str, " = ");
                            break;
                        case NE_OPTR:
                            appendStringInfo(str, " != ");
                            break;
                        default:
                            appendStringInfo(str, " X ");
                            break;
                    }
                    _getNodeInformation(str, eddy, (Node *) get_rightop(expr));
                }
                else if(expr->opType == FUNC_EXPR)
                {
					Func	   *funcnode = (Func *) expr->oper;
					Oid			funcid = funcnode->funcid;
					char       *fname = get_func_name(funcid);
                    List *iter;

                    appendStringInfo(str, " %s(", fname);

                    pfree(fname);

                    foreach(iter, expr->args)
                    {
                        _getNodeInformation(str, eddy, (Node *) lfirst(iter));

                        appendStringInfo(str, lnext(iter) ? ", " : ")");
                    }
                }
                else 
                {
					appendStringInfo(str, "XXX");
					break;
				}
			}
			break;

		default:
			appendStringInfo(str, "XXX");
			break;
	}
}



char *
getNodeInformation(Eddy * eddy, Node *node)
{
	StringInfoData str;

	/* see stringinfo.h for an explanation of this maneuver */
	initStringInfo(&str);
	_getNodeInformation(&str, eddy, node);
	return str.data;
}
