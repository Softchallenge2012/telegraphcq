/*-------------------------------------------------------------------------
 *
 * createeddy.c
 *
 *	  Routines to create an Eddy directly bypassing the optimizer.
 *	  The main function will have the same interface as "subplanner"
 *	  in planmain.c. - Amol.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/createeddy.c,v 1.12 2006/02/13 00:45:30 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include <assert.h>				/* @deddySK */

#include "access/xlog.h"
#include "commands/async.h"
#include "commands/trigger.h"
#include "commands/variable.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/print.h"
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/eddy.h"
#include "optimizer/planner.h"
#include "optimizer/var.h"
#include "optimizer/restrictinfo.h"
#include "optimizer/paths.h"
#include "optimizer/planmain.h"
#include "optimizer/tlist.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "parser/parsetree.h"
#include "rewrite/rewriteHandler.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "storage/proc.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "executor/nodeEddy.h"
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

/* #define CREATE_FILTER_NODES */

#include "pgstat.h"
#include "catalog/pg_statistic.h"
#include "utils/syscache.h"

Plan	   *eddy_subplanner(Query *root, List *flat_tlist);
Node	   *eddy_change_var_references(Eddy * eddy, Node *node);

bool		readjusting_required(List *old_rtable);
List	   *order_qual_clauses(Query *root, List *clauses);
SeqScan    *create_seqscan_plan(Path *best_path, List *tlist, List *scan_clauses);
void		create_stems_from_eq_expr(Expr *expr, SteM ** stem_left_ptr, SteM ** stem_right_ptr);
void		munge_source_into_eddy(Eddy * e, Plan *plan);
void		set_base_rel_pathlists(Query *root);
int         check_uniqueness_and_get_varno(Node *node);
char * get_func_name(Oid funcid);
RelOptInfo * find_base_rel(Query *root, int relid);

static Node *change_var_references(Node *node, int num_target_lists, List **tlists);
static bool expr_satisfies_stem_create_conditions(Expr *expr);

#ifdef CREATE_FILTER_NODES
/*
 * create_filter
 *
 * Makes a new Filter node with the supplied qualifier.
 *
 *
 */
static Filter *create_filter(Node *fqual) 
{
    List *iter;

    Filter *f = makeNode(Filter);

    int sourceFiltered_int = check_uniqueness_and_get_varno((Node *) fqual);

    f->filterqual = fqual;

    BitArrayClearAll(f->sourceFiltered, __MAX_RELATIONS__);

    BitArraySetBit(f->sourceFiltered, sourceFiltered_int - 1);

    f->cost = -1; /** indication that we don't know anything */

    if(IsA(fqual, Expr)) 
    {
        /** If it is the "delay" function, set the cost */
        foreach(iter, ((Expr *) fqual)->args)
        {
            Expr *arg = lfirst(iter);

            if(IsA(arg, Expr) && IsA(arg->oper, Func))
            {
                Const *delay_argument;
                /** Now we should check if the function is the "delay" function. */

                char *name = get_func_name(((Func *) arg->oper)->funcid);

                if(strcmp(name, "delay") == 0) 
                {
                    /** It is the "delay" function... set the cost to be the second argument,
                     *  _if_ it is a constant. */
                    delay_argument = lsecond(arg->args);

                    if(IsA(delay_argument, Const))
                    {
                        f->cost =  (int) delay_argument->constvalue;
                        break;
                    }
                }
            }
        }
    }

    return f;
}
#endif /* CREATE_FILTER_NODES */

static double
find_domain_size_for_var(Query *root, Var *var)
{
	Oid			relid;
	RelOptInfo *rel;
	HeapTuple	tuple;
	Form_pg_statistic stats;
	double		ret_val;

	if(IsA(var, Var))
    {
        relid = getrelid(var->varno, root->rtable);

        Assert(relid != InvalidOid);

        rel = find_base_rel(root, var->varno);

        Assert(rel->tuples > 0);

        tuple = SearchSysCache(STATRELATT,
                ObjectIdGetDatum(relid),
                Int16GetDatum(var->varoattno),
                0, 0);

        Assert(HeapTupleIsValid(tuple));

        stats = (Form_pg_statistic) GETSTRUCT(tuple);

        ret_val = stats->stadistinct;

        ReleaseSysCache(tuple);

        if (ret_val < -.75)             /* -1 is indication of uniqueness I think */
            ret_val = rel->tuples;

        return ret_val;
    }
    else
    {
        return -1;
    }
}


/**
 * eddy_subplanner -- can be called instead of subplanner in planmain.c
 *
 * This function constructs an entire plan consisting of joins nad scans
 * for proessing a single level of attributes.
 * It can not deal with aggregates etc; those are supposed to be done by
 * the caller.
 *
 * See note below on duplicate-free ness of flat_tlist.
 *
 */
Plan *
eddy_subplanner(Query *root, List *flat_tlist)
{
	Eddy	   *e = NULL;
	List	   *iter1,
			   *iter2,
			   *iter3;			/* list iterators */
	int			num_target_lists = 0;	/* This should also be equal to
										 * the number of relations. */
	List	   *tlists[length(root->rtable)];	/* There will never be
												 * more base relation
												 * targetlists than this */


	Assert(!readjusting_required(root->rtable));

	/* Initialize rel_lists. We don't use the other rel_lists. */
	root->base_rel_list = NIL;

	/*
	 * Construct RelOptInfo nodes for all base relations in query.
	 */
	(void) add_base_rels_to_query(root, (Node *) root->jointree);

	/*
	 * Comment from subplanner : Examine the targetlist and
	 * qualifications, adding entries to baserel targetlists for all
	 * referenced Vars.  Restrict and join clauses are added to
	 * appropriate lists belonging to the mentioned relations.	We also
	 * build lists of equijoined keys for pathkey construction.
	 *
	 * NEW: It is not clear if the fact that flat_tlist is duplicate free
	 * important, otherwisse we can replace this with actual_tlist and do
	 * away with one argument to this function.
	 */
	build_base_rel_tlists(root, flat_tlist);

	(void) distribute_quals_to_rels(root, (Node *) root->jointree);

	/*
	 * We will also let postgres construct all the access paths that may
	 * be used for the base relations.
	 */
	set_base_rel_pathlists(root);

	/*
	 * printf("EDDYSUBPLANNER: QUERY AFTER BASERELS HAVE BEEN BUILT AND
	 * PATHLISTS SET\n");
	 */
	/* pprint(root); */

	/*
	 * Create an Eddy.
	 */
	e = makeNode(Eddy);

	e->rangetable = root->rtable;

	e->plan.qual = NIL;

    e->domain_sizes_information = (DomainSizesInformation *) palloc(sizeof(DomainSizesInformation));

    init_domain_sizes_information(e->domain_sizes_information);

	/*
	 * Set the tlist for the Eddy to be flat_tlist. This gets reset above
	 * anyway so not very critical here. Remember to use this tlist to
	 * create a tupledesc for what Eddy returns in the InitCode. The call
	 * is something like ExecAssignResultTypeFromTL.
	 *
	 */
	e->plan.targetlist = flat_tlist;

    /*
     *  Create all the SeqScan's and IndexScan's that can be used for executing
     *  this Query. 
     */
    foreach(iter1, root->base_rel_list) 
    {
        RelOptInfo *roi = (RelOptInfo *) lfirst(iter1);
        Assert(IsA(roi, RelOptInfo));

        foreach(iter2, roi->pathlist)
        {
            Scan *scan;
            Path *path = (Path *) lfirst(iter2);
            List	   *tlist;
            List	   *scan_clauses;

            if(IsA(path, Path))
            {
                List *scan_clause;

                Assert(path->pathtype == T_SeqScan && "Can't handle other scan types");

                tlist = path->parent->targetlist;

                scan_clauses = get_actual_clauses(path->parent->baserestrictinfo);

                /* FIXME: ??? Sort clauses into best execution order */
                scan_clauses = order_qual_clauses(root, scan_clauses);

#ifdef CREATE_FILTER_NODES
                scan = (Scan *) create_seqscan_plan(path, tlist, NIL);
                foreach(scan_clause, scan_clauses)
                {
                    Filter *filter = create_filter((Node *) lfirst(scan_clause));
                    add_planitem_to_eddy(e, (Plan *) filter, EDDY_MODULE);
                }
#else
                scan_clause = NIL;
                scan = (Scan *) create_seqscan_plan(path, tlist, scan_clauses);
#endif /* CREATE_FILTER_NODES */

                /** Statistics */
                scan->num_tuples = (int) roi->tuples;
                scan->num_tuples_passed_quals_so_far = 0;
                scan->num_tuples_seen_so_far = 0;

                /** Add this to the targetlists */
                tlists[num_target_lists++] = tlist;

                Assert(IsA(scan, SeqScan));
                munge_source_into_eddy(e, (Plan *) scan);
            }
            else
            {
                /* SKIP THE INDEXES FOR THE MOMENT */
                /* Assert(!"Can't handle other path types"); */

                /* If we ever get around to using this, must make sure that either only one 
                 * Scan is built for a relation, or if two are built, they must have identical
                 * targetlists. */
            }
        }
    }

    /*
     *  Given that we are going to use the tuples as above from now on, ie., the 
     *  base tuples will be immediately projected to the restricted targetlists 
     *  set above, we need to "munge"/"mutate" all the other Var pointers so that
     *  they are correct. Note that, we will not remove any other attributes till
     *  the very end (at the end of Eddy) even if they are not needed further
     *  (it gets messy to do that properly), so this is fine. 
     *
     *  Once this is done, we can use all those Var's with impunity. 
     *
     *  Similar to join_references with obvious differences.
     *
     *  We will copy and reuse that code.
     */

    /* Query.targetList - not clear if we should do this. */
    // root->targetList = change_var_references(root->targetList, num_target_lists, tlists);

    /* Query.jointree - not clear if we should do this. */
    // root->jointree = change_var_references(root->jointree, num_target_lists, tlists);
    // DONE BELOW jointree = (FromExpr *) change_var_references((Node *) root->jointree, num_target_lists, tlists);

    /* Eddy.plan.targetlist */
    /* This is now done somewhere else */
    /* e->plan.targetlist = change_var_references(e->plan.targetlist, num_target_lists, tlists); */

    /* That's it for now. */

    // printf("EDDYSUBPLANNER: QUERY AFTER VAR REFERENCES HAVE BEEN MUTATED\n");
    // pprint(root);

    /*
     *  OLD: For each relation, use the RelOptInfo.joinclauses (or whatever) to create
     *  SteMs for it. Those quals that can not be passed to the SteMs (e.g. involving
     *  more than 2 relations) should be evaluated by the Eddy itself. 
     *
     *  Clauses that lead to cyclic queries can either be removed, or we can create 
     *  SteMs for those as well.
     *
     *  NEW: We will actuallly use jointree.quals for this purpose. It is not clear why 
     *  we would want to use RelOptInfo's in the first place.
     *  Don't forget to change_var_refererences them if they are used.
     */

#if 0 /** We will use the RelOptInfo's to create the SteMs instead. */
    Assert((root->jointree->quals == NULL) || IsA(root->jointree->quals, List)); /* Is this always true? */
    foreach(iter1, (List *) root->jointree->quals) 
    {
        SteM *stem1, *stem2; 
        Expr *expr = (Expr *) lfirst(iter1);

        Assert(IsA(expr, Expr));

        if(expr_satisfies_stem_create_conditions(expr))
        {
            expr = (Expr *) change_var_references((Node *) expr, num_target_lists, tlists);
            create_stems_from_expr_and_joinquals(expr, NIL, &stem1, &stem2);

            add_planitem_to_eddy(e, (Plan *) stem1, EDDY_MODULE);
            add_planitem_to_eddy(e, (Plan *) stem2, EDDY_MODULE);
        }
        else
        {
            // printf("CANT CREATE STEMS FOR THIS ONE\n");
            // pprint(expr);
            /** For now, add this to the Eddy and let Eddy handle it. 
             *  For non-equality predicates on two relations, we should really let
             *  the SteM handle it. */
            expr = (Expr *) change_var_references((Node *) expr, num_target_lists, tlists);
            e->plan.qual = lcons(expr, e->plan.qual);
        }
    }
#else
	foreach(iter1, root->base_rel_list)
	{
		RelOptInfo *roi = (RelOptInfo *) lfirst(iter1);
		int			my_id;

		Assert(IsA(roi, RelOptInfo));
		Assert(length(roi->relids) == 1);
		my_id = lfirsti(roi->relids);

		/** Skip the baserestrictinfo... that has already been taken care of while creating
		 *	the Scan's above.
		 *	We are interested in joininfo's. */

		foreach(iter2, roi->joininfo)
		{
			JoinInfo   *ji = (JoinInfo *) lfirst(iter2);
			bool		duplicate = false;

			Assert(IsA(ji, JoinInfo));

			/** There are couple of ways of avoiding duplicates. We will do this by doing anything
			 * with the expression's only if my_id is smaller than any of the remaining. */
			foreach(iter3, ji->unjoined_relids)
			{
				Assert(lfirsti(iter3) != my_id);
				if (lfirsti(iter3) < my_id)
				{
					duplicate = true;
					break;
				}
			}

			if (!duplicate)
			{
				if (length(ji->unjoined_relids) > 1)
				{
					/** If more than one relation associated here, then this must be handled by the Eddy
					 *	directly. */
					foreach(iter3, ji->jinfo_restrictinfo)
					{
						Expr	   *expr = ((RestrictInfo *) lfirst(iter3))->clause;

						expr = (Expr *) change_var_references((Node *) expr, num_target_lists, tlists);
						e->plan.qual = lcons(expr, e->plan.qual);
					}
				}
				else
				{
					/** There must be at least one for which we can create SteMs. For the rest of them,
					 *	assign them to be executed by that SteM. */

					/* First find the one on which we can build a SteM. */
					Expr	   *stemexpr = NULL;
					SteM	   *stem1,
							   *stem2;
					List	   *other_quals = NIL;

					foreach(iter3, ji->jinfo_restrictinfo)
					{
						Expr	   *expr = ((RestrictInfo *) lfirst(iter3))->clause;

						expr = (Expr *) change_var_references((Node *) expr, num_target_lists, tlists);
						if (expr_satisfies_stem_create_conditions(expr))
							stemexpr = expr;
						else
							other_quals = lcons(expr, other_quals);
					}

					Assert(stemexpr != NULL);

					create_stems_from_eq_expr(stemexpr, &stem1, &stem2);

					add_planitem_to_eddy(e, (Plan *) stem1, EDDY_MODULE);
					add_planitem_to_eddy(e, (Plan *) stem2, EDDY_MODULE);

					/** Statistics -- must find and store the domains of the two Vars */
					{
						int			domain_size1 = (int) find_domain_size_for_var(root, (Var *) stem1->innerhashkey);
						int			domain_size2 = (int) find_domain_size_for_var(root, (Var *) stem2->innerhashkey);

						stem1->domain_size_innerhashkey = domain_size1;
						stem1->domain_size_outerhashkey = domain_size2;

						stem2->domain_size_innerhashkey = domain_size2;
						stem2->domain_size_outerhashkey = domain_size1;

                        stem1->domain_sizes_information = e->domain_sizes_information;
                        stem2->domain_sizes_information = e->domain_sizes_information;

                        stem2->innerhashkey_index = stem1->outerhashkey_index = 
                            add_key_for_monitoring_domain_size(
                                    e->domain_sizes_information,  
                                    (Node *)stem1->outerhashkey);

                        stem1->innerhashkey_index = stem2->outerhashkey_index = 
                            add_key_for_monitoring_domain_size(
                                    e->domain_sizes_information, 
                                    (Node *) stem2->outerhashkey);
					}
				}
			}
		}
	}
#endif

	/** FIXME: At this point, we should make a check to see whether sufficient SteMs have been
	 *	created to execute the query. For example, for every relation there must exist at least
	 *	one SteM in which it is built and at least one in which it is probed etc etc.
	 *
	 *	FIXED: I think this check is unnecessary now given that we are using JoinInfo's and making sure
	 *	that for every joininfo, at least one pair of SteMs is created.
	 *	*/


	e->num_relations_in_query = num_target_lists;

	/*
	 * If the query contains any hints (using the JOIN keyword), note it
	 * down so that we can create the routing policy using that.
	 *
	 * Right now, we will be simplistic and just traverse the jointree to
	 * check this.
	 *
	 * FIXME: Currently we will only do this for very simple join
	 * expressions. XXX: Forget it... we will just pass the jointree on to
	 * the Eddy and let the routingPolicy routines handle this.
	 */
	e->jointree = copyObject(root->jointree);

	/* printf("EDDYSUBPLANNER: RETURNING A PLAN\n"); */
	/* pprint(e); */
	return (Plan *) e;
}

/*
 * change_var_references
 *
 * Based on join_references. Returns a Node * instead of a List * as
 * we will apply this code not only to target lists, but to
 * other things as well.
 *
 * Returns the new expression tree.  The original clause structure is
 * not modified.
 */
typedef struct
{
	int			num_target_lists;
	List	  **tlists;
}	change_var_references_context;

static Node *change_var_references_mutator(Node *node, change_var_references_context * context);

static Node *
change_var_references(Node *node,
					  int num_target_lists,
					  List **tlists)
{
	change_var_references_context context;

	context.num_target_lists = num_target_lists;
	context.tlists = tlists;
	return (Node *) change_var_references_mutator(node, &context);
}

static Node *
change_var_references_mutator(Node *node,
							  change_var_references_context * context)
{
	if (node == NULL)
		return NULL;
	if (IsA(node, Var))
	{
		Var		   *var = (Var *) node;
		Resdom	   *resdom;
		int			i;

		/* Look for the var in the input tlists */
		for (i = 0; i < context->num_target_lists; i++)
		{
			resdom = tlist_member((Node *) var, context->tlists[i]);
			if (resdom)
			{
				Var		   *newvar = (Var *) copyObject(var);

				/*
				 * We are not going to change the varno; just the
				 * varattno.
				 */
				newvar->varattno = resdom->resno;
				return (Node *) newvar;
			}
		}

		elog(ERROR, "change_var_references: variable not in subplan target lists -- possible reason: subqueries - talk to Amol");
	}
	return expression_tree_mutator(node,
								   change_var_references_mutator,
								   (void *) context);
}

Node *
eddy_change_var_references(Eddy * eddy, Node *node)
{
	int			num_target_lists = eddy->num_relations_in_query;
	List	   *tlists[num_target_lists];
	int			i;

	for (i = 0; i < eddy->num_relations_in_query; i++)
	{
		Plan	   *scan = eddy->sources[i];

		Assert(IsA(scan, SeqScan) &&"Can't handle non SeqScan sources right now");
		tlists[i] = scan->targetlist;
	}

	return change_var_references(node, num_target_lists, tlists);
}

/** This code duplicated in convertoeddy.c */
static bool
check_if_unique_var(Node *node)
{
	List	   *allvars = pull_var_clause(node, false);
	List	   *iter;
	int			varno = -1;

	foreach(iter, allvars)
	{
		Var		   *v = (Var *) lfirst(iter);

		Assert(IsA(v, Var));
		if (varno == -1)
			varno = v->varno;
		else if (varno != v->varno)
			return false;
	}

	freeList(allvars);

	/** If varno = -1, then there is no Var in here. This
	 *	also breaks the condition. */

	return (varno != -1);
}

/** Expr must be an equality and should have two sides both of which
 *	must refer to exactly one Var each. */
static bool
expr_satisfies_stem_create_conditions(Expr *expr)
{
	Oper	   *oper = (Oper *) expr->oper;

	return IsA(oper, Oper) &&(oper->optrtype == EQ_OPTR) &&
		check_if_unique_var((Node *) get_leftop(expr)) &&
		check_if_unique_var((Node *) get_rightop(expr));
	IsA(get_leftop(expr), Var) &&		/*** XXX This doesn't allow R.a = S.b + 1  */
		IsA(get_rightop(expr), Var);
}
