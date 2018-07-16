/*-------------------------------------------------------------------------
 *
 * nodeSQEddy.c
 *	  Some of the routines to handle single query (SQ) Eddy execution
 *
 * This respects the new eddy operator interface
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 * Concerns (Amol) :
 *    -- What does numSources denote ? Did we fix that problem with disparity
 *       between numSources and numRelations in case of Indexes ?
 *    -- Are the ttc_shouldFree's being used properly all over the place ?
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeSQEddy.c,v 1.19 2004/02/13 01:44:01 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>
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
#include "executor/nodeFilter.h"
#include "executor/nodeSteM.h"
#include "executor/nodeIndexSteM.h"
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

#define SQ_EDDY_SEED 1

void debugEddy(Eddy * e, IntermediateHeapTuple iht);
void printOpBits(Eddy * e, bits8 * bits);
void printSources(Eddy *e, IntermediateHeapTuple iht);
bool canRemoveQuery(Eddy *node);

void StartPromote(Eddy *eddy, SteM *stem); 
TupleTableSlot * NewExecEddy(Eddy * node);
void MigrateState(Eddy *eddy, SteM *source, SteM *target);
void AnalyzeSelectivitiesTakeCorrectiveAction(Eddy *node);

/* Static functions used for printing *//* @BdstemsAD */
void printSourceNamesGivenBitMap(Eddy * node, int bitmap);
void printRoutingPolicy(Eddy * node);

void printJoinVarsInformation(Var * var1, Var * var2);

char *getVarInformation(Eddy * node, Var * var);	/* @EdstemsAD */
int chooseSourceRandom(Eddy * e);

char * get_rel_name(Oid relid); /* in lsyscache.c */

inline int convertBitArraytoInteger(BitArray ba);

/*
 *  Conversion routine to convert from the bitarrays used to represent
 *  stem->sourceBuiltOn and also intermeidateheaptuple->t_sources to integers.
 *
 *  TCQMAXSOURCES better be small.
 */
inline int
convertBitArraytoInteger(BitArray ba)
{
    BitIndex i;

    int out = 0;
    int power = 1;

    for(i = 0; i < TCQMAXSOURCES; i++)
    {
        out += BitArrayBitIsSet(ba, i) ? power : 0;
        power *= 2;
    }

    return out;
}

/* ----------------------------------------------------------------
 *		ExecInitEddy
 * ----------------------------------------------------------------
 */
bool
ExecInitSQEddy(Eddy * node, EState * estate, Plan * parent)
{
	EddyState *eddystate;
	int i;
    /*FILE *eddy_config_file;*/

    /** 
     *   The number of relations in the query is differrent from the number of source
     *   modules in soem cases -- for example, if there is an index on a source, there
     *   will not be a source module on it. 
     *   Unfortunately, most of the code below assumes otherwise, and uses node->numSources
     *   as the number of relations.
     *
     *   We will use the rangetable to determine the number of relations in the query.
     */
	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);

    node->use_dual_routing_policy = ! UseBaseSteMs;

    /*
     * Some initialiazation constants will be read from a config file
     */
    /*
    if((eddy_config_file = fopen(eddy_config_file_name, "r")) != NULL) 
    {
        fscanf(eddy_config_file, "%d", &node->use_dual_routing_policy);
        printf("Using the %s\n", node->use_dual_routing_policy ? "dual routing policy" : "stem routing policy");

        fscanf(eddy_config_file, "%d", &node->start_from_source);

        fclose(eddy_config_file);
    }
    */

	/* 
	 * assign the node's execution state
	 */
	node->plan.state = estate;

    /*
     * random stats
     */
    node->numTuplesProcessed = 0;
    node->numTuplesOutput = 0;
    node->total_delay = 0;
    {
        struct timeval t;
        gettimeofday(&t, 0);
        node->begin_time = t.tv_sec;
    }

	/* 
	 * create new EddyState for Eddy
	 */
	eddystate = makeNode(EddyState);
	node->eddystate = eddystate;

	/* 
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &eddystate->jstate);
	ExecAssignExprContext(estate, &eddystate->pstate);

#ifndef DISABLE_CQ_SUPPORT
	eddystate->pstate.cs_ExprContext->ecxt_scantuple = MakeTupleTableSlot();
#else
	eddystate->pstate.cs_ExprContext->ecxt_scantuple = ExecInitExtraTupleSlot(estate);
#endif

	/* 
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &eddystate->jstate);
	ExecInitResultTupleSlot(estate, &eddystate->pstate);
	/* XXX is this required ? */
	/* ExecInitScanTupleSlot(estate, eddystate); */

    /*
     *  Rearrange the sources so that they are in the order of their scanrelids.
     */
    {
        int i, j;
        Plan **old_source_list = node->sources;
        node->sources = (Plan **) palloc(sizeof(Plan *) * node->numSources);
        for(i = 0; i < node->numSources; i++) 
        {
            node->sources[i] = NULL;
            for(j = 0; j < node->numSources; j++) 
            {
                Assert(IsA(old_source_list[j], SeqScan));
                if(((SeqScan *) old_source_list[j])->scanrelid == i + 1)
                {
                    node->sources[i] = old_source_list[j];
                    break;
                }
            }
            Assert(node->sources[i]);
        }
    }

	/* 
	 * now initialize children
	 */
	for (i = 0; i < node->numSources; i++) 
    {
		ExecInitNode((Plan *) node->sources[i], estate, (Plan *) node);

        if(IsA(node->sources[i], SeqScan)) 
        {
            ((SeqScan *) node->sources[i])->return_null_if_quals_fail = false;
        }
	}

    assert(!node->cacq);

    /*
     * now initialize modules, ticket and time
     */
    for (i = 0; i < node->numModules; i++) 
    {
        if (! IsA(node->modules[i], Filter))
        {
            ExecInitNode(node->modules[i], estate, (Plan *) node);
        }			
    }

    /* //@BdstemsAD Initialize the sourceFinished array; repeated calls
     * to the Scan seem to repeat so this has to be globally maintained. */
    eddystate->sourceFinished = NULL;
    if (node->numSources > 0) 
    {
        eddystate->sourceFinished =
            (bool *) palloc(sizeof(bool) * node->numSources);
        eddystate->tuples_remaining_to_be_scanned =
            (int *) palloc(sizeof(int) * node->numSources);
    }

    eddystate->total_base_tuples_remaining = 0;

    for (i = 0; i < node->numSources; i++) 
    {
        eddystate->sourceFinished[i] = false;

        eddystate->total_base_tuples_remaining +=
            eddystate->tuples_remaining_to_be_scanned[i] = 
            (((SeqScan *) node->sources[i])->num_tuples + 1); /* +1 is important i think */
    }

    /* 
     * Init the routing policy arrays 
     */
    init_routing_policy(node); 

    /*
     * First initialize the probe routing policy according the query plan.
     * Note that this is done implicitly by assuming an order on the 
     * modules (that was created by the converttoeddy routines)
     */
    initialize_probe_routing_policy(node);

    /*
     * Also initialize the selections routing policy. Currently, our policy is
     * to only create Filters for base table predicates, and apply them as soon
     * as possible. More details/updates in the comments for this function. */
    initialize_selections_routing_policy(node);
    rethink_selections_routing_policy(node);

    /* 
     * Initialize the current routing policy.
     *
     * The initial policy will be pretty much arbitrarily chosen.
     *
     * A SteM contains a pointer to the dual SteM and it also contains the
     * source that the SteM is building on (as a bitmap with exactly 1 bit
     * set to 1). Now, given this setup, a given tuple "can" be routed to
     * the SteM with which "AND" results in truth and for which "AND" with the
     * dual results in false.
     *
     * Initially, we will just scan the list of modules, and the first module
     * that matches the condition will be used.
     *
     * Currently, NULL means we will just return the tuple. There
     * should be a sanity check there to make sure that the tuple is actually
     * finished.
     *
     */
    if(node->use_dual_routing_policy) 
    {
        set_dual_routing_policy(node);
    }
    else
    {
        set_shankar_stem_routing_policy(node);
    }

    printRoutingPolicy(node);	/* @EdstemsAD */

    /* 
     * Initialize the insertSteMsBySource structure.
     *
     * This basically lists all SteMs that a given source tuple can be build into.
     */


	/* 
	 * Initialize the random number generator's seed
	 */
	srandom(SQ_EDDY_SEED);

	/* //@BdstemsAD Set the stack of SteMs to be null. */
	eddystate->stack = NULL;
    eddystate->stackptr = -1;
	if (node->numModules > 0) 
    {
		eddystate->stack = (Node **) palloc(sizeof(Node *) * node->numModules);
		for (i = 0; i < node->numModules; i++) 
        {
			eddystate->stack[i] = 0;
		}
	}

    /* 
     * Initialize tuple type and projection info.
     */
    ExecAssignResultTypeFromTL((Plan *) node, &eddystate->jstate);
    ExecAssignProjectionInfo((Plan *) node, &eddystate->jstate);

 	/*
 	 * Initialize debugging
 	 */
 	eddystate->debugctx = AllocSetContextCreate(TopMemoryContext,
 												"Eddy debugging Memory Context",
 												ALLOCSET_DEFAULT_MINSIZE,
 												ALLOCSET_DEFAULT_INITSIZE,
 												ALLOCSET_DEFAULT_MAXSIZE);

    /*
     * Create the BaseTupleDesc array by looking at all sources. 
     * This doesn't fully work, as in case of indexes, we must look
     * at them as well.
     */
    Assert(node->num_relations_in_query == node->numSources);
    for(i = 0; i < node->num_relations_in_query; i++)
    {
        TupleDesc td;
        int relation_id;

        Assert(IsA(node->sources[i], SeqScan));

        relation_id = ((SeqScan *) node->sources[i])->scanrelid;

        td = ((SeqScan *) node->sources[i])->scanstate->cstate.cs_ResultTupleSlot->ttc_tupleDescriptor;

        node->base_tuple_descs[relation_id - 1] = td;
    }

    /*
     * Once this is done, go and change it for the entire tupletable.
     * WARNING !! This might be considered a serious breach of encapsulation.
     */
    for(i = 0; i < estate->es_tupleTable->size; i++)
    {
        TupleTableSlot *tts = &estate->es_tupleTable->array[i];
        tts->ttc_base_tuple_descs_ptr = node->base_tuple_descs;
    }

	return TRUE;		
}

/** At this point, I think we need 3 slots */
int			
ExecCountSlotsSQEddy(Eddy * node)
{
    return 3;
}	


/** Call ExecProcNode on all modules in the stack, and return if a tuple is found. 
 *  Should make this function inline. */
static
bool execute_using_stack(Eddy *node, TupleTableSlot **slot_ptr)
{
    EddyState *eddystate = node->eddystate;

    while (!empty(node)) 
    {
        *slot_ptr = ExecProcNode((Plan *) top(node), (Plan *) node);

        if ((*slot_ptr == NULL) || TupIsNull(*slot_ptr)) 
        {
            if((eddystate->promotion_mode == true) && (top(node) == eddystate->current_promotion_stem))
            {
                /* The current_promotion_stem just returned null, which means we are done promoting */
                eddystate->promotion_mode = false;
            }

            pop(node);
        } 
        else 
            return true;
    }
    return false;
}

/**
 *
 * Read from a source using some policy. 
 *
 */

/* This is used to round robin between sources to read. */
static int last_source = 0;
#define ROUNDROBIN

static
bool read_from_a_source(Eddy *node, TupleTableSlot **slot_ptr)
{
    int i;
    Plan **sources = node->sources;
    EddyState *eddystate = node->eddystate;
    int sourceID = -1;
    bool found = false;

#ifdef RANDOM
    int offset = (int) (drand48() * (float) node->numSources);           
#else 
#ifdef ROUNDROBIN
    int offset = (last_source + 1) % node->numSources;                  
#else
#ifdef NOINTERLEAVING
    int offset = node->start_from_source;                              
#endif
#endif
#endif

    *slot_ptr = NULL;

    for (i = 0; i < node->numSources; i++) 
    {
        int j = (i + offset) % node->numSources;

        if (!eddystate->sourceFinished[j]) 
        {
            *slot_ptr = ExecProcNode(sources[j], (Plan *) node);

            (*slot_ptr)->ttc_isIntermediateTuple = false;

            sourceID = ((SeqScan *) sources[j])->scanrelid - 1;

            if (TupIsNull(*slot_ptr)) 
            {
                eddystate->sourceFinished[j] = true;
            } 
            else 
            {
                found = true;
                last_source = j;
                break;
            }
        }
    }

    if (found)
    {

        /* 
         * The base tuples must be converted to intermediateTuple format.
         * There is no need to create a new intermediateheaptuple here, since at any
         * time the Eddy can only need one of these.
         */
        IntermediateHeapTuple iht = &eddystate->iHeapTupleData;

        Assert(sourceID != -1);

        initFromBaseTuple(iht, (*slot_ptr)->val, (*slot_ptr)->ttc_tupleDescriptor, sourceID);

        /* Timestamp the tuple */
        /* iht->t_timestamp = node->numTuplesProcessed; */
        {
            struct timeval t;
            gettimeofday(&t, NULL);
            iht->t_timestamp = t.tv_sec - node->begin_time + t.tv_usec/1000000.0;
        }

        /* we seem to have lost the original val here. need to */
        /* free that in this */
        /* case, as soon as the tuple is inserted into some stem. */
        /* XXX I have forgotten what this comment means. */

        (*slot_ptr)->val = (HeapTuple) iht;
        (*slot_ptr)->ttc_isIntermediateTuple = true;
        (*slot_ptr)->ttc_shouldFree = false;

        return true;
    }
    else
        return false;

    Assert(0);
    return read_from_a_source(0, 0); /** annoying warnings */
}



static
bool read_from_a_source_random_weighted(Eddy *node, TupleTableSlot **slot_ptr)
{
    int i;
    Plan **sources = node->sources;
    EddyState *eddystate = node->eddystate;
    int random, cumulative_sum, chosen_source;

    for(;;) 
    {
        if(eddystate->total_base_tuples_remaining == 0)
            return false;

        /* Choose a random number between 0 and eddystate->total_base_tuples_remaining */
        random = drand48() * eddystate->total_base_tuples_remaining;
        /* random = .3 * eddystate->total_base_tuples_remaining; */

        /* which source does this correspond to ? */
        cumulative_sum = 0;
        chosen_source = -1;
        for(i = 0; i < node->numSources; i++)
        {
            cumulative_sum += eddystate->tuples_remaining_to_be_scanned[i];
            if(random < cumulative_sum)
            {
                chosen_source = i;
                break;
            }
        }

        Assert(chosen_source != -1);

        *slot_ptr = ExecProcNode(sources[chosen_source], (Plan *) node);

        eddystate->tuples_remaining_to_be_scanned[chosen_source]--;
        eddystate->total_base_tuples_remaining--;

        if(eddystate->tuples_remaining_to_be_scanned[chosen_source] == 0)
        {
            if(! ((SeqScan *) sources[chosen_source])->scanstate->really_done)
                eddystate->total_base_tuples_remaining
                    += eddystate->tuples_remaining_to_be_scanned[chosen_source]
                    = 10;
        }

        if(!TupIsNull(*slot_ptr))
        {
            IntermediateHeapTuple iht;
            int sourceID;

            (*slot_ptr)->ttc_isIntermediateTuple = false;

            sourceID = ((SeqScan *) sources[chosen_source])->scanrelid - 1;

            /* 
             * The base tuples must be converted to intermediateTuple format.
             * There is no need to create a new intermediateheaptuple here, since at any
             * time the Eddy can only need one of these.
             */
            iht = &eddystate->iHeapTupleData;

            initFromBaseTuple(iht, (*slot_ptr)->val, (*slot_ptr)->ttc_tupleDescriptor, sourceID);

            /* Timestamp the tuple */
            /* iht->t_timestamp = node->numTuplesProcessed; */
            {
                struct timeval t;
                gettimeofday(&t, NULL);
                iht->t_timestamp = t.tv_sec - node->begin_time + t.tv_usec/1000000.0;
            }

            /* we seem to have lost the original val here. need to */
            /* free that in this */
            /* case, as soon as the tuple is inserted into some stem. */
            /* XXX I have forgotten what this comment means. */

            (*slot_ptr)->val = (HeapTuple) iht;
            (*slot_ptr)->ttc_isIntermediateTuple = true;
            (*slot_ptr)->ttc_shouldFree = false;

            return true;
        }
        else
        {
            /** This can happen w/o tuples_remaining_to_be_scanned going down to zero. */
            if(((SeqScan *) sources[chosen_source])->scanstate->really_done)
            {
                eddystate->total_base_tuples_remaining -=
                    eddystate->tuples_remaining_to_be_scanned[chosen_source];
                eddystate->tuples_remaining_to_be_scanned[chosen_source] = 0;
            }
        }

    }
}

/* 
 * Go over all the SteMs, and see if any of them have pending probes. 
 * If yes, schedule one of the pending probes. */
static
bool schedule_some_pending_probe(Eddy *node)
{
    int i;

    for(i = 0; i < node->numModules; i++)
    {
        if(IsA(node->modules[i], SteM)) 
        {
            SteM *stem = (SteM *) node->modules[i];

            if(ExecSteMHasPendingProbes(stem))
            {
                ExecSteMSetNewOuterFromPendingProbes(stem);
                push(node, (Node *) stem);
                return true;
            }
        }
    }
    return false;
}

/* #define CHOOSE_PROBABILISTICALLY */

static SteM *
choose_probe_stem(EddyState *eddystate, int n)
{
#ifdef CHOOSE_PROBABILISTICALLY
    List *probeSteMs = eddystate->probeRoutingPolicy[n];
    List *iter;
    float rand = drand48();
    int i = 0;

    if(probeSteMs != NIL)
    {
        foreach(iter, probeSteMs)
        {
            if(rand < eddystate->probeRoutingWeights[n][i++])
                return lfirst(iter);
        }
        Assert(0);
    }

#else
    if(eddystate->probeRoutingPolicy[n] != NIL)
        return lfirst(eddystate->probeRoutingPolicy[n]);
#endif

    return NULL;
}


/* ----------------------------------------------------------------    //@BdstemsAD
 *      ExecEddy(node)
 * ----------------------------------------------------------------
 */
TupleTableSlot *
NewExecEddy(Eddy * node)
{
	Plan **sources;
	Plan **modules;
	TupleTableSlot *srcTupleSlot = 0;
	BitIndex numModules;
	TupleTableSlot *resultSlot;
	EddyState *eddystate;
	EState *estate;
	ExprContext *econtext;
	ExprDoneCond isDone;
	bool *sourceFinished;

	/* List *probeSteMs; */
	/* List *insertSteMs; */
	List *iterator;


	/* 
	 * Fetch data from node
	 */
	estate = node->plan.state;
	eddystate = node->eddystate;
	econtext = eddystate->jstate.cs_ExprContext;
	modules = node->modules;
	sources = node->sources;
	numModules = node->numModules;
	sourceFinished = eddystate->sourceFinished;

	/* 
	 * Check to see if we're still projecting out tuples from a previous
	 * scan tuple (because there is a function-returning-set in the
	 * projection expressions).  If so, try to project another one.
	 */
	if (eddystate->jstate.cs_TupFromTlist) 
    {
		resultSlot = ExecProject(eddystate->jstate.cs_ProjInfo, &isDone);
		if (isDone == ExprMultipleResult)
			return resultSlot;
		/* Done with that source tuple... */
		eddystate->jstate.cs_TupFromTlist = false;
	}

	/* 
	 * Reset per-tuple memory context to free any expression evaluation
	 * storage allocated in the previous tuple cycle.  Note this can't
	 * happen until we're done projecting out tuples from a scan tuple.
	 */
	ResetExprContext(econtext);

	do {                            /* Infinite loop */
        bool tupleFromSource;

		CHECK_FOR_INTERRUPTS();

		/* 
         *  First the stack.
		 */
        if(! execute_using_stack(node, &srcTupleSlot)) 
        {
            /* 
             * If the stack is empty, try a source. 
             */

            if(node->eddystate->recheck_selectivities) 
            {
                AnalyzeSelectivitiesTakeCorrectiveAction(node);
                node->eddystate->recheck_selectivities = false;
            }

            if(! read_from_a_source_random_weighted(node, &srcTupleSlot)) 
            {

                /*
                 * Move to the cleanup phase and see if any of the SteMs have any pending
                 * probe tuples. Schedule one of those SteMs.
                 *
                 * NOTE: This already implements some kind of policy for hash joins.
                 */
                /* printEddyStats(node); */
                if(! schedule_some_pending_probe(node))
                {
                    /* We are really done */
                    ResetExprContext(econtext);
                    return ExecStoreTuple(NULL,
                            eddystate->jstate.cs_ProjInfo->pi_slot,
                            InvalidBuffer, true);
                }
                else
                {
                    /* printf("===> Scheduled a pending probe for some SteM\n"); */
                    /* printEddyStats(node); */
                    /* Schedule by itself does not produce tuples. We must
                     * execute the stack for that. */
                    continue;
                }
            }
            else
                tupleFromSource = true;
        }
        else
            tupleFromSource = false;

        /* 
         * One more tuple processed by eddy.
         */
        node->numTuplesProcessed++;
		/* if(!(node->numTuplesProcessed & 65535)) {  */
		/** HAHA -- figure that one out */
		/* printEddyStats(node); */
		/* } */

        /* 
         * Switch the routing policy if so desired
         */
        if(node->numTuplesProcessed % 10000 == 0) 
        {
            /** Can't do promotes here because stack is not empty */
            node->eddystate->recheck_selectivities = true;
            /* compute_selectivities_for_all_joins(node); */
            /* rethink_routing_policy(node); */
            /* printRoutingPolicy(node); */

            /* NING: rethink_selections_routing_policy(node); */
        }

        if(node->numTuplesProcessed == 800000)
            elog(ERROR, "Total: Too many tuples being generated");

		econtext->ecxt_scantuple = srcTupleSlot;

        /* debugitup((IntermediateHeapTuple) srcTupleSlot->val); */

        /** We use a completely different policy of routing if promotion mode is set to on.
         *  In that case, the routing is driven by probeSteMs in some sense.
         *  We need to find a probeSteM that is not the dual of the current stem on which
         *  promotion is being performed, and then perform insertion/deletion on that pair
         *  of stems. */
        if(eddystate->promotion_mode == false) 
        {
            /** The original way we did this is not correct any more. That code can be found at the bottom of this
             * file */
            SteM *probeSteM = NULL;
            int tupleBitArrayInt = convertBitArraytoInteger(((IntermediateHeapTuple) srcTupleSlot->val)->t_sources);
            bool passed_selections = true;

            /* First thing to do is to apply the filters */
            List *selections_to_be_applied = eddystate->selectionsRoutingPolicy[tupleBitArrayInt];

            if(selections_to_be_applied)
            {
                List *iterator;

                foreach(iterator, selections_to_be_applied)
                {
                    Filter *filter = (Filter *) lfirst(iterator);

                    Assert(IsA(filter, Filter));

                    if(! ExecEvaluateFilter(filter, econtext))
                    {
                        /** We are done with this tuple... proceed to the next tuple */
                        passed_selections = false;
                        break;
                    }
                }
            }

            /** passed_selections is true if there are no filters */

            if(passed_selections) 
            {
                /** The tuple passed all the selections... proceed to probing. */
                probeSteM = choose_probe_stem(eddystate, tupleBitArrayInt);

                if(probeSteM)
                {
                    if(! node->use_dual_routing_policy)
                    {
                        List *insertSteMs = eddystate->insertRoutingPolicy[tupleBitArrayInt];

                        /* Insert into all the stems in the list */
                        if (insertSteMs != NIL) 
                        {
                            bool first = true;
                            foreach(iterator, insertSteMs) 
                            {
                                SteM *stem = lfirst(iterator);

                                Assert(IsA(stem, SteM));

                                /* XXX The whether_to_copy field here is using a weird kinda logic that is
                                 * hardcoded here. Without the true in the beginning, it is copying
                                 * only base tuples into the first SteM they are built into. 
                                 *
                                 * There are some repercussions of this into the delete code as well,
                                 * e.g. in the SteM Remove partition kinda thing. */
                                ExecSteMInsert(stem, econtext, true || (tupleFromSource && first));

                                first = false;
                            }
                        }
                    }
                    else
                    {
                        /** Insert into the dual stem of the probeSteM */
                        ExecSteMInsert(probeSteM->dualSteM, econtext, true || tupleFromSource);
                    }

                    /** Now probe */
                    ExecSteMSetNewOuter((SteM *) probeSteM, econtext);
                    push(node, (Node *) probeSteM);
                }
                else
                {
                    /** We are done... return the tuple */
                    /** Before returning the result, we must check eddy->plan.qual */
                    if(ExecQual(node->plan.qual, econtext, false)) 
                    {
                        return ExecStoreTuple(srcTupleSlot->val,
                                eddystate->jstate.cs_ProjInfo->pi_slot,
                                InvalidBuffer, false);
                    }
                }
            }
        }
        else /* promotion_mode == true */
        {
            /* Look for a probeSteM that is not the current promotion stem */
            List *probeSteMs = eddystate->probeRoutingPolicy[convertBitArraytoInteger(((IntermediateHeapTuple) srcTupleSlot->val)->t_sources)];


            /** This should be set by looking at the tuple. Since we will be deleting the tuple from the
             *  original stem, it must be copied over if it is a base table tuple. Currently, we are
             *  copying everything (see the "true ||" below), so it doesn't really matter. */
            bool isBaseTableTuple = false;

            List *iterator;

            bool found = false;

            Assert(node->use_dual_routing_policy);
            Assert(0);

            assert(probeSteMs != NIL); /* there is no way this probestems can be null */

            foreach(iterator, probeSteMs) 
            {
                SteM *probeSteM = (SteM *) lfirst(iterator);

                assert(IsA(probeSteM, SteM));

                if(probeSteM->dualSteM->stemstate->sm_promotion_mode != true)
                {
                    /* we can use this one */
                    found = true;

                    /* Copy only if it is a base tuple */
                    ExecSteMInsert(probeSteM->dualSteM, econtext, true || isBaseTableTuple);

                    ExecSteMSetNewOuter((SteM *) probeSteM, econtext);
                    push(node, (Node *) probeSteM);

                    break;
                }
            }

            /* if not found, then we must insert it back into the SteM from which this tuple's ancestor  */
            /* originated for promotion. Note that, this does not follow the policy of not inserting */
            /* into a SteM that is already on the stack. In this particular case, it doesn't matter. */
            if(! found) 
            {
                /* just another sanity check */
                SteM *probeSteM = (SteM *) lfirst(probeSteMs);
                assert(probeSteM->dualSteM == (SteM *) eddystate->current_promotion_stem);

                assert(! isBaseTableTuple); /* why are we promoting if there is nothing to promote to. */
                ExecSteMInsert((SteM *) eddystate->current_promotion_stem, econtext, true);
            }
        }
    }
    while (true);

    /* This should never really happen, but just to stop that annoying */
    /* warning */
    Assert(0);
    return NULL;
}								/* @EdstemsAD */

TupleTableSlot *				/* @BdstemsAD */
ExecSQEddy(Eddy * node)
{
	TupleTableSlot *result = NULL;
	ExprDoneCond isDone;
	/* double f; */

	int i;
    bool alreadypromoted = true;
    bool printHashTables = false;

    assert(! node->cacq);

    for (i = 0; i < 1; i++) 
    {
execeddy: 
        result = NewExecEddy(node);

        if (TupIsNull(result)) 
        {
            int j, k;

            j = k = 0; /** stupid warning !! */

            /***************
            printEddyStats(node);

            debug_log_message("stem demote start");
            for(k = 0; k < 1; k++) 
            {
                for(j = 0; j < node->numModules; j++)
                {
                    Assert(IsA(node->modules[j], SteM));

                    printf("================== BEFORE DEMOTION ======================\n");
                    printSteMHashJoinTable(((SteM *) node->modules[j])->stemstate->stemhashtable);

                    ExecSteMDemoteAllTuples((SteM *) node->modules[j]);

                    printf("================== AFTER DEMOTION ======================\n");
                    printSteMHashJoinTable(((SteM *) node->modules[j])->stemstate->stemhashtable);
                }
            }
            debug_log_message("stem demote end");
            ***********/

            if(printHashTables) 
            {
                printAllHashTables(node);
            }

            if(!alreadypromoted) 
            {
                /* We will promote SteM No. 2 -- this is particular to the region join nation join customer query */
                StartPromote(node, (SteM *) node->modules[1]);

                alreadypromoted = true;

                goto execeddy;
            } 

            break;
        }
        else
        {
            /** Update total delay using the timestamp and the current time */
            if(result->val != NULL) {
                struct timeval t;
                double d;
                gettimeofday(&t, NULL);
                d = t.tv_sec - node->begin_time + t.tv_usec/1000000.0 - 
                        ((IntermediateHeapTuple) result->val)->t_timestamp;
                /* printf("%ld %ld %f\n", t.tv_sec, t.tv_usec, d); */

                node->total_delay += d;
            }

            node->numTuplesOutput++;

            result->ttc_isIntermediateTuple = true;

            result->ttc_shouldFree = false;	/* DO NOT FREE THE BASE TUPLES */

            result = ExecProject(node->eddystate->jstate.cs_ProjInfo, &isDone);

            /* 
             *  After the project is finished, the tuple is not intermediate any more.
             *  Since the rest of the postgres code is not aware of this variable as of
             *  now, we will keep it that way and set this correctly ourselves.
             */
            result->ttc_isIntermediateTuple = false;

            /* printf("------------------------------------------------------------------------\n"); */
            /* printf("------------------------------------------------------------------------\n"); */
            /* printf("----------------------------------- %d */
            /* ---------------------------------\n", i); */
            /* printf("------------------------------------------------------------------------\n"); */
            /* printf("------------------------------------------------------------------------\n"); */
            /*  */
            /* print_slot(result); */
        }
        /* printf("Finished\n"); */
        /* exit(1); */

    }

    return result;
}								/* @EdstemsAD */


/* ----------------------------------------------------------------
 *		ExecEndEddy
 *
 *		frees any storage allocated through C routines.
 * ----------------------------------------------------------------
 */
void
ExecEndSQEddy(Eddy * node)
{
	int i = 0;

    printEddyStats(node);

    /* compute_cumulative_selectivities_in_all_directions(node); */
    /* print_all_selectivities(node); */

	for (i = 0; i < node->numSources; i++) 
    {
		ExecEndNode((Plan *) node->sources[i], (Plan *) node);
	}

	for (i = 0; i < node->numModules; i++) {	/* @BdstemsAD */
		if (!IsA(node->modules[i], Filter)) {
			ExecEndNode((Plan *) node->modules[i], (Plan *) node);
		}
	}							/* @EdstemsAD */
}

/** Given a SteM, start promoting by setting some flags. Note that, this 
 *  just pushes the module on the stack, and the Eddy processing should take
 *  care of the rest. */
void
StartPromote(Eddy *eddy, SteM *stem) 
{
    assert(empty(eddy));
    assert(eddy->eddystate->promotion_mode == false);

    stem->stemstate->sm_promotion_mode = true;
    stem->stemstate->sm_current_promoted_tuple = NULL;
    stem->stemstate->sm_current_promoted_tuple_bucket_no = -1;

    eddy->eddystate->promotion_mode = true;
    eddy->eddystate->current_promotion_stem = (Node *) stem;

    push(eddy, (Node *) stem);
}

/** 
 *  Given two SteMs, perform state migration between the two. So take the first
 *  SteM (source), start promotion on it, for each of those tuples, build-probe
 *  into the other SteM (target) and its dualSteM. The returning result is then
 *  built into the original SteM (source).
 *  In the end, do an ExecSteMDemoteAllTuples on target to remove duplicates. 
 *  This ofcourse has the disadvantage of removing all the state from this SteM.
 *
 *  Couple of things: One is we should only promote those tuples that are "owned" by
 *  this SteM. Otherwise, we are actually redoing some of the work that was already
 *  done. Now the question is where is this information maintained. XXX FIXME*/
void
MigrateState(Eddy *eddy, SteM *source, SteM *target)
{
    TupleTableSlot *slot;
    SteM *probeSteM = target->dualSteM;
	ExprContext *econtext = eddy->eddystate->jstate.cs_ExprContext;

    /** Better be on the same source */
    Assert(source->innerhashkey_var->varnoold == target->innerhashkey_var->varnoold);

    /** Must demote all the tuples in source as well. This is not required if we only promote
     * base tuples. XXX */
    ExecSteMDemoteAllTuples(source);

    /** Set some stuff in the SteM itself */
    source->stemstate->sm_promotion_mode = true;
    source->stemstate->sm_current_promoted_tuple = NULL;
    source->stemstate->sm_current_promoted_tuple_bucket_no = -1;

    while((slot = ExecProcNode((Plan *) source, (Plan *) eddy)))
    {
        if ((slot == NULL) || TupIsNull(slot)) 
            break;

		econtext->ecxt_scantuple = slot;

        /* Copy only if it is a base tuple */
        ExecSteMInsert(target, econtext, true); /* target == probeSteM->dualSteM */

        ExecSteMSetNewOuter((SteM *) probeSteM, econtext);

        /** And now get tuples from this SteM and insert them back into source */
        while((slot = ExecProcNode((Plan *) probeSteM, (Plan *) eddy)))
        {
            if ((slot == NULL) || TupIsNull(slot)) 
                break;

            econtext->ecxt_scantuple = slot;

            ExecSteMInsert(source, econtext, true);
        }
    }

    source->stemstate->sm_promotion_mode = false;

    /** Bad idea but right now the quickest way to do duplicate elimination. */
    ExecSteMDemoteAllTuples(target);
}

void
AnalyzeSelectivitiesTakeCorrectiveAction(Eddy *node)
{
    int i, j;
    List **probeRoutingPolicy;
    int num_relations_in_query = node->num_relations_in_query;

    compute_cumulative_selectivities_in_all_directions(node);

    for(i = 0; i < node->numModules; i++)
    {
        SteM *stem = (SteM *) node->modules[i];

        printf("For stem on %s\n", getNodeInformation(node, stem->innerhashkey));

        printf("Join selectivity is %f, cumulative selectivity is %f\n", stem->join_selectivity, stem->cumulative_selectivity);
    }

    for(i = 0; i < node->numSources; i++)
    {
        int num_stems = 0;
        int stem_number[__MAX_RELATIONS__];

        SeqScan *scan = (SeqScan *) node->sources[i];
        printf("For scan on relation %s, selectivity is %f\n", get_rel_name(rt_fetch(i + 1, node->rangetable)->relid), 
                scan->scan_selectivity);

        printf("Out of the %d tuples that passed selections for this relation, \n", scan->num_tuples_passed_quals_so_far);
        for(j = 0; j < node->numModules; j++)
        {
            SteM *stem = (SteM *) node->modules[j];
            if(stem->innerhashkey_var->varnoold == i + 1)
            {
                /* XXX note the heavy use of the fact that we are only tracking for one */
                printf("	%d were inserted into SteM %s which has cumulative selectivity %f\n", stem->stats.num_inserted[0],
                        getNodeInformation(node, stem->innerhashkey), stem->cumulative_selectivity);
                stem_number[num_stems++] = j;
            }
        }
    }

    probeRoutingPolicy = node->eddystate->probeRoutingPolicy;

    if(UseChangeRoutingPolicy)
    {
        /** Given the cumulative selectivities and tuples routed, update the routingPolicy array. */
        for (i = 0; i < (1 << num_relations_in_query); i++)
        {
            List *min = NIL;
            List *probeRoutingPolicyForThisSignature = probeRoutingPolicy[i];
            if(probeRoutingPolicyForThisSignature != NIL)
            {
                /** XXX for some reasons, these terms got defined in such a fashion that the dualSteM (the SteM you insert
                 *  into) has the correct cumulative selectivity. Must figure out exactly whats going on. */
                float min_selectivity = ((SteM *) lfirst(probeRoutingPolicyForThisSignature))->dualSteM->cumulative_selectivity;
                List *iterator;
                foreach(iterator, probeRoutingPolicyForThisSignature) 
                {
                    if(lnext(iterator) != NIL) 
                    {
                        SteM *nextSteM = lfirst(lnext(iterator));
                        if(nextSteM->dualSteM->cumulative_selectivity < min_selectivity) 
                        {
                            min_selectivity = nextSteM->dualSteM->cumulative_selectivity;
                            min = iterator;
                        }
                    }
                }
                if(min != NIL)  /* this also takes care of there being only one entry in the list */
                {
                    List *newFirst = lnext(min);
                    lnext(min) = lnext(newFirst);
                    lnext(newFirst) = probeRoutingPolicyForThisSignature;
                    probeRoutingPolicy[i] = newFirst;
                }
            }
        }
        printRoutingPolicy(node);
    }

    if(UseMigrateState)
    {
        if(node->use_dual_routing_policy) /* migration useless with original SteMs */
        {
            /** May want to migrate state */
            for(i = 0; i < node->numSources; i++)
            {
                List *probeRoutingPolicyForThisSource = probeRoutingPolicy[(1 << i)];
                List *iterator1, *iterator2;
                foreach(iterator1, probeRoutingPolicyForThisSource)
                {
                    foreach(iterator2, probeRoutingPolicyForThisSource)
                    {
                        SteM *stem1 = ((SteM *) lfirst(iterator1))->dualSteM; /* remember that dualSteM is the insert stem here */
                        SteM *stem2 = ((SteM *) lfirst(iterator2))->dualSteM;
                        printf("%s %s %f %f %d %d\n", 
                                getNodeInformation(node, stem1->innerhashkey), 
                                getNodeInformation(node, stem2->innerhashkey),
                                stem1->cumulative_selectivity,
                                stem2->cumulative_selectivity,
                                stem1->stats.num_inserted[0],
                                stem2->stats.num_inserted[0]);

                        if(stem1 != stem2)
                        {
                            if((stem1->cumulative_selectivity < stem2->cumulative_selectivity)  &&
                                    (stem1->stats.num_inserted[0] < stem2->stats.num_inserted[0])) 
                            {
                                /* printAllHashTables(node); */
                                printf("Migrating state from %s to %s\n", getNodeInformation(node, stem2->innerhashkey), 
                                        getNodeInformation(node, stem1->innerhashkey));
                                MigrateState(node, stem2, stem1);
                                /* printAllHashTables(node); */

                                /* One migration is sufficient for the moment being. */
                                goto finish;
                            }
                        }
                    }
                }
finish:
                ;
            }
        }
    }
}

/*************************** DELETED CODE
            insertSteMs = eddystate->insertRoutingPolicy[convertBitArraytoInteger(((IntermediateHeapTuple) srcTupleSlot->val)->t_sources)];

            // Insert into all the stems in the list
            if (insertSteMs != NIL) 
            {
                bool first = true;
                foreach(iterator, insertSteMs) 
                {
                    SteM *stem = lfirst(iterator);

                    assert(IsA(stem, SteM));

                    * XXX The whether_to_copy field here is using a weird kinda logic that is
                     * hardcoded here. Without the true in the beginning, it is copying
                     * only base tuples into the first SteM they are built into. 
                     *
                     * There are some repercussions of this into the delete code as well,
                     * e.g. in the SteM Remove partition kinda thing. *
                    ExecSteMInsert(stem, econtext, true || (tupleFromSource && first));

                    first = false;
                }
            }

            probeSteMs = eddystate->probeRoutingPolicy[convertBitArraytoInteger(((IntermediateHeapTuple) srcTupleSlot->val)->t_sources)];

            * Currently we can only handle probes into one SteM... I don't know
             * if it even makes sense to do it otherwise *
            if (probeSteMs != NIL)
            {
                Node *n = lfirst(probeSteMs);

                if(IsA(n, SteM)) 
                {
                    ExecSteMSetNewOuter((SteM *) n, econtext);
                    push(node, n);
                } 
                else if(IsA(n, IndexSteM))
                {
                    ExecIndexSteMSetNewOuter((IndexSteM *) n, econtext);
                    push(node, n);
                }
            } 
            else // if probeSteMs == NIL, then return the result
            {
                ** Before returning the result, we must check eddy->plan.qual *
                if(ExecQual(node->plan.qual, econtext, false)) 
                {
                    return ExecStoreTuple(srcTupleSlot->val,
                            eddystate->jstate.cs_ProjInfo->pi_slot,
                            InvalidBuffer, false);
                }
            }
*****************/
