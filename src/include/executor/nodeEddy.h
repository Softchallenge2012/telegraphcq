/*-------------------------------------------------------------------------
 *
 * nodeEddy.h
 *
 * This file declares the functions that make up the implementation of the new
 * eddy operator
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeEddy.h,v 1.21 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEEDDY_H
#define NODEEDDY_H

#include "executor/intermediateTuple.h"
#include "nodes/plannodes.h"

void		RunCQEddy(void);
extern TupleTableSlot *ExecEddy(Eddy * node);
extern bool ExecInitEddy(Eddy * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsEddy(Eddy * node);
extern void ExecEndEddy(Eddy * node);
extern void cqeddyAdjustScanAndRT(Scan *s, List *eddyrt, List *queryrt);
extern void setReadyBits(IntermediateHeapTuple iht, Eddy * e);
extern HeapTuple EddyExecProject(Eddy * eddy, IntermediateHeapTuple iht, int32 queryid);
extern HeapTuple ProjectTuple(ProjectionInfo *pi,
			 TupleDesc resultDesc,
			 bool isIntermediate,
			 void *tuple);
extern void buildStems(Eddy * node, IntermediateHeapTuple iht, int sourceID);
extern PostJoinOperator *findPJOfromSources(Eddy * node, BitArray sources);
extern void cleanOperatorFromPJOList(Eddy * node, int operator);
extern void setReadyFromPJOList(Eddy * node, IntermediateHeapTuple iht);
extern void clearCompletionBits(IntermediateHeapTuple iht, List *);
extern int32 GlobalClock;

void		printEddyStats(Eddy * node);
void		printEddyStats_debug_with_level(Eddy * e, int level);
char	   *getNodeInformation(Eddy * eddy, Node *node);
void		printAllHashTables(Eddy * node);

/** Stack Functions */
Node	   *top(Eddy * node);
Node	   *pop(Eddy * node);
void		push(Eddy * node, Node *p);
bool		full(Eddy * node);
bool		empty(Eddy * node);

/** CQ Eddy Functions - in the file nodeCQEddy.c */
extern TupleTableSlot *ExecCQEddy(Eddy * node);
extern bool ExecInitCQEddy(Eddy * node, EState *estate, Plan *parent);
extern void ExecEndCQEddy(Eddy * node);
extern char *getVarInformation(Eddy * node, Var *var);	/* @EdstemsAD */
extern int	ExecCountSlotsSQEddy(Eddy * node);
extern int getTotalProbes(Eddy *node);

/** SQ Eddy Functions - in the file nodeSQEddy.c */
extern TupleTableSlot *ExecSQEddy(Eddy * node);
extern bool ExecInitSQEddy(Eddy * node, EState *estate, Plan *parent);
extern void ExecEndSQEddy(Eddy * node);

/** routing policy functions */
void		reset_insert_routing_policy(Eddy * node);
void		initialize_probe_routing_policy(Eddy * node);
void reset_selections_routing_policy(Eddy *node);
void initialize_selections_routing_policy(Eddy *node);
void		set_dual_routing_policy(Eddy * node);
void		set_shankar_stem_routing_policy(Eddy * node);
void		init_routing_policy(Eddy * node);
void		printRoutingPolicy(Eddy * node);
void		print_all_selectivities(Eddy * node);
void		compute_join_selectivities(Eddy * node);
void		compute_scan_selectivities(Eddy * node);
void		compute_cumulative_selectivities_in_all_directions(Eddy * node);
void		compute_cumulative_selectivities_in_all_directions_recursive(Eddy * node, SteM * stem);
double		compute_selectivity_for_stem(Eddy * node, SteM * stem);
void    rethink_selections_routing_policy(Eddy *node);
void rethink_routing_policy(Eddy *node);

/** Statistics */
void		init_statistics(Statistics * stats);
void		update_stats_inserted_tuple(Statistics * stats, int bitset, int num);
void		update_stats_probed_tuple(Statistics * stats, int bitset, int num);
void		update_stats_returned_tuple(Statistics * stats, int bitset, int num);
void		print_statistics(Statistics * stats);

/** Domain size estimation */
int add_key_for_monitoring_domain_size(DomainSizesInformation *dsi, Node *key);
void update_domain_size(DomainSizesInformation *dsi, int key_index, int new_value);
int get_domain_size(DomainSizesInformation *dsi, int key_index);
void init_domain_sizes_information(DomainSizesInformation *dsi);

Oid GetRecurseRelation(Eddy * eddy, uint32 queryid);
bool IsOutputQuery(Eddy * eddy,uint32 queryid);
int findRecursiveOutputQueryId(Eddy * eddy, uint32 queryid); 
List * findOutputQueryList(Eddy * eddy, List * query_ids);

extern void printTuplePath(Eddy * eddy, IntermediateHeapTuple iht,
						   bool, int32);
extern bool buildStemsNoDup(Eddy * node, IntermediateHeapTuple iht, int sourceID, bool nodup);

#endif   /* NODEEDDY_H */
