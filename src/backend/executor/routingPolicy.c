/*-------------------------------------------------------------------------
 *
 * routingPolicy.c
 *	  Separating the routines for managing the routing policy a bit -- Amol
 *
 * This respects the new eddy operator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/routingPolicy.c,v 1.18 2004/03/24 08:10:56 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "postgres.h"
/* tcqoc */
#include "tcop/pquery.h"
#include "executor/executor.h"
/*end tcqoc */
#include "executor/execdebug.h"
#include "executor/nodeEddy.h"
#include "executor/nodeGSFilter.h"
#include "executor/nodeSteM.h"
#include "executor/nodeFjord.h"
#include "miscadmin.h"
#include "misceddy.h"
#include "nodes/copyrte.h"
#include "optimizer/eddy.h"
#include "parser/parsetree.h"
#include "rewrite/rewriteManip.h"
#include "rqueue/planq.h"
#include "rqueue/controlq.h"
#include "rqueue/rqdest.h"
#include "utils/bit.h"
#include "utils/memutils.h"

#define EDDY_SEED 1
#define DIST_RANGE 1000.0
#define EPSILON 0.0000000001

#define USE_BITARRAYS			/* also defined in nodeEddy.c */

extern int	convertBitArraytoInteger(BitArray ba);
extern Var *get_leftop(Expr *clause);
extern Var *get_rightop(Expr *clause);

char	   *get_rel_name(Oid relid);
List * pull_var_clause(Node *node, bool includeUpperVars);

void
reset_insert_routing_policy(Eddy * node)
{
	int			i;
	int			num_relations_in_query = node->num_relations_in_query;

	for (i = 0; i < (1 << num_relations_in_query); i++)
		node->eddystate->insertRoutingPolicy[i] = NIL;
}

void
reset_selections_routing_policy(Eddy *node) 
{
    int i;
    int num_relations_in_query = node->num_relations_in_query;

    for(i = 0; i < (1 << num_relations_in_query); i++)  
    {
        node->eddystate->selectionsRoutingPolicy[i] = NIL;
    }
}


static BitArray
recursively_set_probe_routing_policy(Eddy * node, Node *joinexpr)
{
	BitArray	ret_val = (BitArray) palloc(sizeof(bits8) * BASIZE(__MAX_RELATIONS__));

	if (IsA(joinexpr, RangeTblRef))
	{
		BitArrayClearAll(ret_val, __MAX_RELATIONS__);
		BitArraySetBit(ret_val, ((RangeTblRef *) joinexpr)->rtindex - 1);
	}
	else
	{
		BitArray	left,
					right;
		JoinExpr   *je = (JoinExpr *) joinexpr;
		Expr	   *qual;
		Var		   *left_var,
				   *right_var;
		List	   *probe_routing_modules_array;
		int			i;

		Assert(IsA(je, JoinExpr));

		left = recursively_set_probe_routing_policy(node, je->larg);
		right = recursively_set_probe_routing_policy(node, je->rarg);

		/** ret_val tuples must go to the SteM built on the qual here */
		Assert(IsA(je->quals, List));
		qual = (Expr *) lfirst((List *) je->quals);
		Assert(IsA(qual, Expr));
        Assert(length(pull_var_clause((Node *) get_leftop(qual), false)) == 1);
        Assert(length(pull_var_clause((Node *) get_rightop(qual), false)) == 1);
		left_var = lfirst(pull_var_clause((Node *) get_leftop(qual), false));
		right_var = lfirst(pull_var_clause((Node *) get_rightop(qual), false));
		Assert(IsA(left_var, Var));
		Assert(IsA(right_var, Var));

		/** First take care of left */
		probe_routing_modules_array = node->eddystate->probeRoutingPolicy[convertBitArraytoInteger(left)];

		printf("For bitarray = ");
		BitArrayPrint(left, __MAX_RELATIONS__);

		for (i = 0; i < length(probe_routing_modules_array); i++)
		{
			SteM	   *stem = (SteM *) nth(i, probe_routing_modules_array);

			if (IsA(stem, SteM))
			{
				if (stem->innerhashkey_var->varnoold == right_var->varnoold)
				{
					probe_routing_modules_array = lremove(stem, probe_routing_modules_array);
					probe_routing_modules_array = lcons(stem, probe_routing_modules_array);
					node->eddystate->probeRoutingPolicy[convertBitArraytoInteger(left)] = probe_routing_modules_array;
					pprint(stem->innerhashkey);
					break;
				}
			}
		}

		/** The right */
		probe_routing_modules_array = node->eddystate->probeRoutingPolicy[convertBitArraytoInteger(right)];

		printf("For bitarray = ");
		BitArrayPrint(right, __MAX_RELATIONS__);

		for (i = 0; i < length(probe_routing_modules_array); i++)
		{
			SteM	   *stem = (SteM *) nth(i, probe_routing_modules_array);

			if (IsA(stem, SteM))
			{
				if (stem->innerhashkey_var->varnoold == left_var->varnoold)
				{
					probe_routing_modules_array = lremove(stem, probe_routing_modules_array);
					probe_routing_modules_array = lcons(stem, probe_routing_modules_array);
					node->eddystate->probeRoutingPolicy[convertBitArraytoInteger(right)] = probe_routing_modules_array;
					pprint(stem->innerhashkey);
					break;
				}
			}
		}

		/** Put these two together for above */
		BitArrayOr(left, right, ret_val, __MAX_RELATIONS__);

		pfree(left);
		pfree(right);
	}
	return ret_val;
}


/** This is very elementary and can only handle one case. */
static void
rearrange_according_to_jointree(Eddy * node)
{
	FromExpr   *jointree = node->jointree;

    if(jointree) 
    {
        /* If fromlist contains more than 1 entry, we don't have any hints. */
        if (length(jointree->fromlist) != 1)
            return;

        /** We take care of this not being a JoinExpr inside */
        pfree(recursively_set_probe_routing_policy(node, (Node *) lfirst(jointree->fromlist)));
    }
}

void
initialize_probe_routing_policy(Eddy * node)
{
	int			i,
				j;
	int			num_relations_in_query = node->num_relations_in_query;
	List	  **probeRoutingPolicy = node->eddystate->probeRoutingPolicy;

	for (i = 0; i < (1 << num_relations_in_query); i++)
	{
		for (j = 0; j < node->numModules; j++)
		{
			if (IsA(node->modules[j], SteM))
			{
				SteM	   *stem = (SteM *) node->modules[j];

				/*
				 * WARNING !! The following code is more complicated than
				 * it looks. The "i" itself is treated as a bitarray. */
#ifdef USE_BITARRAYS
                if ((i & convertBitArraytoInteger(stem->sourceBuiltOn)) &&
                        !(i & convertBitArraytoInteger(stem->dualSteM->sourceBuiltOn)))
#else
                    if ((i & stem->sourceBuiltOnint) && !(i & stem->dualSteM->sourceBuiltOnint))
#endif
                    {
                        probeRoutingPolicy[i] = lappend(probeRoutingPolicy[i], stem->dualSteM);
                    }
			}
			else if (IsA(node->modules[j], IndexSteM))
			{
				IndexSteM  *istem = (IndexSteM *) node->modules[j];

				if (!(i & convertBitArraytoInteger(istem->sourceBuiltOn)) &&	/* NOTE: code not
																				 * identical to above */
					(i & convertBitArraytoInteger(istem->probeSource)))
					probeRoutingPolicy[i] = lappend(probeRoutingPolicy[i], istem);
			}
		}
	}

	/*
	 * Now that we have essentially listed all the Modules a tuple with a
	 * particular signature can go to, we should decide which one to send
	 * it to initially (this will be the first in the list).
	 *
	 * Currently, the first one pretty much arbitrary, though it works out
	 * fine if we are not using the fast path.
	 *
	 * We will try to use the query->jointree (copied in eddy->jointree) for
	 * this purpose.
	 */
	rearrange_according_to_jointree(node);

	/* printf("%s\n", getNodeInformation(node, ((SteM *) node->modules[0])->innerhashkey)); */

	/** Must set the weights as well... we will begin with equal weights into all of them */
	for (i = 0; i < (1 << num_relations_in_query); i++)
	{
		int			n = length(node->eddystate->probeRoutingPolicy[i]);

		if (n != 0)
		{
			node->eddystate->probeRoutingWeights[i] = (float *) palloc(sizeof(float) * n);
			for (j = 0; j < n; j++)
				node->eddystate->probeRoutingWeights[i][j] = 1.0 / n * (j + 1);
			node->eddystate->probeRoutingWeights[i][n - 1] = 1.0;		/* just to be sure */
		}
		else
			node->eddystate->probeRoutingWeights[i] = NULL;
	}
}

/** At this time, we are treating selection operators differently from SteMs. 
 *  Initially, we will simply assign all selections to be performed as soon as
 *  possible, and we will only create Filter nodes for base relation filters.
 *  As a result, we will initialize teh routing policy so that the selections
 *  are all assigned to the base table tuples in arbitrary order.
 *  The order is the key here and that can be changed in the rethinking mode. */
void
initialize_selections_routing_policy(Eddy *node)
{
    List **selectionsRoutingPolicy = node->eddystate->selectionsRoutingPolicy;
    int i, j;

    reset_selections_routing_policy(node);

    for (i = 0; i < node->numSources; i++) 
    {
        for (j = 0; j < node->numModules; j++) 
        {
            if (IsA(node->modules[j], Filter)) 
            {
                Filter *filter = (Filter *) node->modules[j];

                if (BitArrayBitIsSet(filter->sourceFiltered, i))
                {
                    int index = convertBitArraytoInteger(filter->sourceFiltered);
                    selectionsRoutingPolicy[index] = lcons(filter, selectionsRoutingPolicy[index]);
                }
            }
        }
    }
}

/* 
 * The original routing policy  -- will just set these to be the dual SteMs
 */
void
set_dual_routing_policy(Eddy * node)
{
	List	  **probeRoutingPolicy = node->eddystate->probeRoutingPolicy;
	List	  **insertRoutingPolicy = node->eddystate->insertRoutingPolicy;
	int			num_relations_in_query = node->num_relations_in_query;
	int			i;


	return;

	Assert(0);

	reset_insert_routing_policy(node);

	for (i = 0; i < (1 << num_relations_in_query); i++)
	{
		if (probeRoutingPolicy[i] != NIL)
		{
			Node	   *n = (Node *) lfirst(probeRoutingPolicy[i]);

			if (IsA(n, SteM))
				insertRoutingPolicy[i] = makeList1(((SteM *) n)->dualSteM);
		}
	}
}

/*
 * List all SteMs to which a source tuple can go to. Non-source tuples are not inserted anywhere.
 */
void
set_shankar_stem_routing_policy(Eddy * node)
{
    List **insertRoutingPolicy = node->eddystate->insertRoutingPolicy;
    int i, j;

    reset_insert_routing_policy(node);

    for (i = 0; i < node->numSources; i++) {
        for (j = 0; j < node->numModules; j++) {
            if (IsA(node->modules[j], SteM)) {
                SteM *stem = (SteM *) node->modules[j];
                if (BitArrayBitIsSet(stem->sourceBuiltOn, i))
                    /* 					if((1 << i) & stem->sourceBuiltOn)  */
                {
                    int index = convertBitArraytoInteger(stem->sourceBuiltOn);
                    insertRoutingPolicy[index] = lcons(stem, insertRoutingPolicy[index]);
                }
            }
        }
    }
}

void init_routing_policy(Eddy *node)
{
    int num_relations_in_query = node->num_relations_in_query;
    int i;

    Assert(num_relations_in_query < 10);	/* just a random sanity check */

    node->eddystate->insertRoutingPolicy =
        (List **) palloc(sizeof(List *) * (1 << num_relations_in_query));

    node->eddystate->probeRoutingPolicy =
        (List **) palloc(sizeof(List *) * (1 << num_relations_in_query));

    node->eddystate->selectionsRoutingPolicy = 
        (List **) palloc(sizeof(List *) * (1 << num_relations_in_query));

    node->eddystate->probeRoutingWeights =
        (float **) palloc(sizeof(float *) * (1 << num_relations_in_query));

    for (i = 0; i < (1 << num_relations_in_query); i++)
    {
        node->eddystate->insertRoutingPolicy[i] = NIL;
        node->eddystate->probeRoutingPolicy[i] = NIL;
        node->eddystate->selectionsRoutingPolicy[i] = NIL;
    }
}

extern void	printRoutingPolicy(Eddy * node);

void
rethink_routing_policy(Eddy *node)
{
    int i;
    int num_relations_in_query = node->num_relations_in_query;
    List **probeRoutingPolicy;

    /*
       set_dual_routing_policy(node);
       printf("stats: switching\n");
       printEddyStats(node);
     */

    printf("Rethinking routing policy\n");

    /* Go over the SteMs in order and assign a random float to each of them based
     * on the number of tuples probed/returned. If the ratio is too high (too many
     * tupels returned, then this number should be high too, but not 1. Choose 
     * a random number and multiply by the ratio. */
    for(i = 0; i < node->numModules; i++) 
    {
        SteMState *stemstate = ((SteM *) node->modules[i])->stemstate;
        if(stemstate->sm_tuplesProbed == 0) {
            stemstate->sm_routingNumber = drand48();
        } else {
            stemstate->sm_routingNumber = (drand48() * stemstate->sm_tuplesReturned) / stemstate->sm_tuplesProbed;
        }
    }
    
    /* For each tuple, check the ratio of tuples probed and returned for
     * each IRSteM, and switch to the one with the lowest ratio.
     * Break ties randomly. */
    probeRoutingPolicy = node->eddystate->probeRoutingPolicy;
    for (i = 0; i < (1 << num_relations_in_query); i++)
    {
        List *best = NIL;
        float bestRoutingNumber = 1000;
        List *probeRoutingPolicyForThisSignature = probeRoutingPolicy[i];
        List *iterator;
        foreach(iterator, probeRoutingPolicyForThisSignature) 
        {
            printf("for the next signature\n");
            if(lnext(iterator) != NIL) 
            {
                SteM *nextSteM = lfirst(lnext(iterator));
                if(nextSteM->stemstate->sm_routingNumber < bestRoutingNumber) 
                {
                    bestRoutingNumber = nextSteM->stemstate->sm_routingNumber;
                    best = iterator;
                }
            }
        }
        if(best != NIL) 
        {
            List *newFirst = lnext(best);
            lnext(best) = lnext(newFirst);
            lnext(newFirst) = probeRoutingPolicyForThisSignature;
            probeRoutingPolicy[i] = newFirst;
            printf("Switching routing policy\n");

            if(node->use_dual_routing_policy) {
                lfirst(node->eddystate->insertRoutingPolicy[i]) = ((SteM *) lfirst(newFirst))->dualSteM;
            }
        }
    }
}
void init_statistics(Statistics *stats);
void update_stats_inserted_tuple(Statistics *stats, int bitset, int num);
void update_stats_probed_tuple(Statistics *stats, int bitset, int num);
void update_stats_returned_tuple(Statistics *stats, int bitset, int num);
void print_statistics(Statistics *stats);

/** Some functions to monitor/update statistics */
void
init_statistics(Statistics * stats)
{
	int			i;

	for (i = 0; i < MAX_TO_TRACK; i++)
	{
		stats->num_inserted[i] = 0;
		stats->num_probed[i] = 0;
		stats->num_returned[i] = 0;
	}
}

void
update_stats_inserted_tuple(Statistics * stats, int bitset, int num)
{
	int			i;

	for (i = 0; i < stats->num_bitsets_to_track_for_inserted; i++)
	{
		if (!(stats->bitsets_to_track_for_inserted[i] ^ bitset))
		{
			stats->num_inserted[i] += num;
			break;
		}
	}
}

void
update_stats_probed_tuple(Statistics * stats, int bitset, int num)
{
	int			i;

	for (i = 0; i < stats->num_bitsets_to_track_for_probed; i++)
	{
		if (!(stats->bitsets_to_track_for_probed[i] ^ bitset))
		{
			stats->num_probed[i] += num;
			break;
		}
	}
}

void
update_stats_returned_tuple(Statistics * stats, int bitset, int num)
{
	int			i;

	for (i = 0; i < stats->num_bitsets_to_track_for_returned; i++)
	{
		if (!(stats->bitsets_to_track_for_returned[i] ^ bitset))
		{
			stats->num_returned[i] += num;
			break;
		}
	}
}

void
print_statistics(Statistics * stats)
{
	int			i;

	printf("======== Inserted Tuples =========\n");

	for (i = 0; i < stats->num_bitsets_to_track_for_inserted; i++)
		printf("	%x	%d\n", stats->bitsets_to_track_for_inserted[i], stats->num_inserted[i]);

	printf("======== Probed Tuples =========\n");

	for (i = 0; i < stats->num_bitsets_to_track_for_probed; i++)
		printf("	%x	%d\n", stats->bitsets_to_track_for_probed[i], stats->num_probed[i]);

	printf("======== Returned Tuples =========\n");

	for (i = 0; i < stats->num_bitsets_to_track_for_returned; i++)
		printf("	%x	%d\n", stats->bitsets_to_track_for_returned[i], stats->num_returned[i]);
}

/** For each relation (or SeqScan), for all joins on that relations (or SteMs), compute
 *	the cumulative selectivity in that direction. Works only for acyclic queries.
 *	This will probably have to a recursive call. */
void
compute_cumulative_selectivities_in_all_directions(Eddy * node)
{
	int			i;

	/** First compute the scan selectivities and the join selectivities and
	 * store them in respective SteMs */
	compute_scan_selectivities(node);
	compute_join_selectivities(node);

	/** Set all the cumulative selectivities to -1 so that we know what needs to
	 *	be computed. */
	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		stem->cumulative_selectivity = -1;
	}

	/** Now call recursively to compute for each SteM */
	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		compute_cumulative_selectivities_in_all_directions_recursive(node, stem);
	}
}

/** relid is an index in node->sources or relid+1 in node->rangetable. they are
 *	equivalent. */
void
compute_cumulative_selectivities_in_all_directions_recursive(Eddy * node, SteM * stem)
{
	SteM	   *dual_stem = stem->dualSteM;
	int			dual_scan_relid = dual_stem->innerhashkey_var->varnoold;	/* index into rangetable */
	SeqScan    *dual_scan = (SeqScan *) node->sources[dual_scan_relid - 1];		/* == index into sources
																				 * + 1 */

	int			i;

	/** For all *other* stems on the dual_scan_relid, multiply the cumulative's together.
	 *	Recurse if cumulative not known. */
	stem->cumulative_selectivity = stem->join_selectivity * dual_scan->num_tuples;

	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *other_stem = (SteM *) node->modules[i];

		if ((other_stem->innerhashkey_var->varnoold == dual_scan_relid) && (other_stem != dual_stem))
		{
			if (other_stem->cumulative_selectivity < -.5)
				compute_cumulative_selectivities_in_all_directions_recursive(node, other_stem);

			stem->cumulative_selectivity *= other_stem->cumulative_selectivity;
		}
	}
}

void
compute_scan_selectivities(Eddy * node)
{
	int			i;

	for (i = 0; i < node->numSources; i++)
	{
		SeqScan    *scan = (SeqScan *) node->sources[i];

        if(scan->num_tuples_passed_quals_so_far != 0)
            scan->scan_selectivity = scan->num_tuples_passed_quals_so_far * 1.0 / scan->num_tuples_seen_so_far;
        else
            scan->scan_selectivity = 1.0 / scan->num_tuples_seen_so_far; /* arbitrarily set this to be something */
	}
}

void
compute_join_selectivities(Eddy * node)
{
	int			i;

	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		Assert(IsA(stem, SteM));

		stem->join_selectivity = compute_selectivity_for_stem(node, stem);
	}
}

double
compute_selectivity_for_stem(Eddy * node, SteM * stem)
{

	double		original_selectivity;

    int domain_size_innerhashkey, domain_size_outerhashkey;

	SeqScan    *scan = (SeqScan *) node->sources[stem->innerhashkey_var->varnoold - 1];	/** ahh -- headaches with -1's */
	SeqScan    *dual_scan = (SeqScan *) node->sources[stem->outerhashkey_var->varnoold - 1];

	Assert(IsA(stem, SteM));

    /* Assert(stem->domain_size_innerhashkey);
       Assert(stem->domain_size_outerhashkey); */

    domain_size_outerhashkey = get_domain_size(stem->domain_sizes_information, stem->outerhashkey_index);
    domain_size_innerhashkey = get_domain_size(stem->domain_sizes_information, stem->innerhashkey_index);

    printf("%d %d\n", domain_size_innerhashkey, domain_size_outerhashkey);

	/* 1 / max of the two domains */
	original_selectivity = 1.0 / ((domain_size_innerhashkey < domain_size_outerhashkey) ?
		domain_size_outerhashkey : domain_size_innerhashkey);		/** always remember "?:" has lowest precedence */

    /* printf("%d %d %d %d\n", 
            scan->num_tuples_passed_quals_so_far, scan->num_tuples_seen_so_far, 
            dual_scan->num_tuples_passed_quals_so_far, dual_scan->num_tuples_seen_so_far);*/

	return original_selectivity *
		scan->num_tuples_passed_quals_so_far * 1.0 / scan->num_tuples_seen_so_far *
		dual_scan->num_tuples_passed_quals_so_far * 1.0 / dual_scan->num_tuples_seen_so_far;
}

void
print_all_selectivities(Eddy * node)
{
	int			i,
				j;

	for (i = 0; i < node->numSources; i++)
	{
		SeqScan    *scan = (SeqScan *) node->sources[i];

		printf("For scan on relation %s, selectivity is %f\n", get_rel_name(rt_fetch(i + 1, node->rangetable)->relid),
			   scan->scan_selectivity);

		printf("Out of the %d tuples that passed selections for this relation, \n", scan->num_tuples_passed_quals_so_far);
		for (j = 0; j < node->numModules; j++)
		{
			SteM	   *stem = (SteM *) node->modules[j];

			if (stem->innerhashkey_var->varnoold == i + 1)
			{
				/*
				 * XXX note the heavy use of the fact that we are only
				 * tracking for one kind of tuples (base tuples).
				 */
				printf("	%d were inserted into SteM %s\n", stem->stats.num_inserted[0],
					   getNodeInformation(node, stem->innerhashkey));
			}
		}
	}

	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		printf("For stem on %s\n", getNodeInformation(node, stem->innerhashkey));

		printf("Join selectivity is %f, cumulative selectivity is %f\n", stem->join_selectivity, stem->cumulative_selectivity);
	}
}

void compute_selectivities_for_all_operators(Eddy * node);

/** Use the domain sizes and the selectivities of the relations. */
void
compute_selectivities_for_all_operators(Eddy * node)
{
	double		scan_selectivities[node->numSources];
	int			i,
				j;

	for (i = 0; i < node->numSources; i++)
	{
		SeqScan    *scan = (SeqScan *) node->sources[i];

		scan_selectivities[i] = scan->num_tuples_passed_quals_so_far * 1.0 / scan->num_tuples_seen_so_far;
		printf("For scan on relation %s, selectivity is %f\n", get_rel_name(rt_fetch(i + 1, node->rangetable)->relid),
			   scan_selectivities[i]);

		printf("Out of the %d tuples that passed selections for this relation, \n", scan->num_tuples_passed_quals_so_far);
		for (j = 0; j < node->numModules; j++)
		{
			SteM	   *stem = (SteM *) node->modules[j];

			if (stem->innerhashkey_var->varnoold == i + 1)
			{
				/*
				 * XXX note the heavy use of the fact that we are only
				 * tracking for one
				 */
				printf("	%d were inserted into SteM %s\n", stem->stats.num_inserted[0],
					   getNodeInformation(node, stem->innerhashkey));
			}
		}
	}

	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		printf("For join between %s and %s\n", getNodeInformation(node, stem->innerhashkey),
			   getNodeInformation(node, stem->outerhashkey));

		printf("Selectivity is %f\n", compute_selectivity_for_stem(node, stem));
	}
}

static double
compute_selectivity_for_stem_1(SteM * stem)
{
	int			num_one,
				num_two,
				num_returned;

	/** Same tuples that were inserted in one SteM were probed into the other... shouldn't add
	 *	them up like that. */
	num_one = stem->stats.num_inserted[0];		/* +
												 * dual_stem->stats.num_pro
												 * bed[0]; */
	num_two = stem->stats.num_probed[0];		/* +
												 * dual_stem->stats.num_ins
												 * erted[0]; */
	num_returned = stem->stats.num_returned[0] + stem->dualSteM->stats.num_returned[0];

	if (num_one && num_two)
		return 1.0 * num_returned / (num_one * num_two);
	else
		return .5;
}

void		compute_selectivities_for_all_joins_1(Eddy * node);

/** Use the "Statistics" to compute selectivities. This probably makes a bunch of assumptions
 *	about the arrival order etc. */
void
compute_selectivities_for_all_joins_1(Eddy * node)
{
	int			i;

	for (i = 0; i < node->numModules; i++)
	{
		SteM	   *stem = (SteM *) node->modules[i];

		Assert(IsA(stem, SteM));

		printf("For join between %s and %s\n", getNodeInformation(node, stem->innerhashkey),
			   getNodeInformation(node, stem->outerhashkey));

		printf("Selectivity is %f\n", compute_selectivity_for_stem_1(stem));
	}
}


void
rethink_selections_routing_policy(Eddy *node)
{
    int i;

    /** Just print out the costs of all the filters. */
    for(i = 0; i < node->numModules; i++)
    {
        if(IsA(node->modules[i], Filter)) 
        {
            Filter *f = (Filter *) node->modules[i];
            printf("For filter %s, the cost is %d\n", getNodeInformation(node, f->filterqual), f->cost);
        }
    }

    return;
}

/**************************************************************************************************
 *
 * DOMAIN SIZES MANIPULATIONS
 *
 *************************************************************************************************/
void
init_domain_sizes_information(DomainSizesInformation *dsi)
{
    dsi->num_keys = 0;
}

/* return the key index */
int
add_key_for_monitoring_domain_size(DomainSizesInformation *dsi, Node *key)
{
    /* search */
    int i;
    for(i = 0; i < dsi->num_keys; i++) 
    {
        if(equal(dsi->keys[i], key))
            return i;
    }

    /* nope */
    dsi->keys[dsi->num_keys] = key;
    dsi->domain_sizes[dsi->num_keys] = 1; /* default to begin */

    dsi->num_keys++;

    return dsi->num_keys - 1;
}

void
update_domain_size(DomainSizesInformation *dsi, int key_index, int new_value)
{
    /** simple scheme that assumes that all domains are numbered 1 - MAX */
    if(dsi->domain_sizes[key_index] < new_value)
        dsi->domain_sizes[key_index] = new_value;
}

int 
get_domain_size(DomainSizesInformation *dsi, int key_index)
{
    Assert(key_index < dsi->num_keys);

    return dsi->domain_sizes[key_index];
}
