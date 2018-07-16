/*-------------------------------------------------------------------------
 *
 * dyncatalog.h
 *
 * This file declares the functions that are used to implement TCQ catalog
 * streams.
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: dyncatalog.h,v 1.6 2003/07/17 05:18:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef DYNCATALOG_H
#define DYNCATALOG_H

#include "rqueue/planq.h"
#include "telegraphcq/wrapch.h"

typedef enum
{
	TCQ_QUERIES = 0,
	TCQ_OPERATORS = 1,
	TCQ_QUEUES = 2,
	/* Make sure that TCQ_NUMCATS is the last element of this enum */
	TCQ_NUMCATS = 3
} TCQ_DynCatType;

#define NAME_TCQ_QUERIES				  "public.tcq_queries"
#define NAME_TCQ_OPERATORS				  "public.tcq_operators"
#define NAME_TCQ_QUEUES					  "public.tcq_queues"

#define getCatName(_cat_)				  (NAME_##_cat_)

typedef struct Info_tcq_queries
{
	uint32		max_query_num;
	uint16		cur_query_nums[TCQMAXQUERIES];
}	Info_tcq_queries;

typedef struct Info_tcq_operators
{
	uint32		max_optr_num;	/* Highest operator number issued */
	uint16		cur_optr_nums[TCQMAXOPERATORS]; /* Op number indexed by
												 * optr id */
	uint16		num_queries[TCQMAXOPERATORS];	/* Num queries for each
												 * optr  */
}	Info_tcq_operators;

typedef struct Info_tcq_queues
{
	uint32		dummy;
}	Info_tcq_queues;

typedef union CatInfo			/* Per-catalog specific information */
{
	Info_tcq_queries tcq_queries;
	Info_tcq_operators tcq_operators;
	Info_tcq_queues tcq_queues;
}	CatInfo;



typedef struct DynCatInfo
{
	bool		active;							   /* Is this catalog ready ? */
	List	   *name;						   /* Name of this catalog stream */
	unarchivedStreamInfo *sinfo;		/* Information related to this straem */
	TupleDesc	tdesc;					/* TupleDesc of tuples of this stream */
	char	   *nulls;						/* Nulls array for heap_formtuple */
	CatInfo		catinfo;				  /* Per-catalog specific information */
	Oid			streamoid;									/* Oid of stream  */
	MemoryContext perTupleContext;				  /* Per-tuple Memory Context */
}	DynCatInfo;

#define INFO_TCQ_QUERIES_SIZE		   (MAXALIGN(sizeof(Info_tcq_queries)))
#define INFO_TCQ_OPERATORS_SIZE		   (MAXALIGN(sizeof(Info_tcq_operators)))
#define INFO_TCQ_QUEUES_SIZE		   (MAXALIGN(sizeof(Info_tcq_queues)))
#define INFO_TCQ_CATINFOSIZE		   (MAXALIGN(sizeof(CatInfo)))
#define DYNCATINFOSIZE				   (MAXALIGN(sizeof(DynCatInfo)))


extern DynCatInfo *tcq_dyncats;

/*
 * Info specific to tcq_queries
 */
#define Natts_tcq_queries		   5
#define Anum_tcq_queries_tcqtime   0
#define Anum_tcq_queries_querynum  1
#define Anum_tcq_queries_queueid   2
#define Anum_tcq_queries_kind	   3
#define Anum_tcq_queries_querystr  4

#define TCQ_QUERIES_ENTRY		   "E"
#define TCQ_QUERIES_EXIT		   "X"

/*
 * Info specific to tcq_operators
 */
#define Natts_tcq_operators		   9
#define Anum_tcq_operators_tcqtime 0
#define Anum_tcq_operators_opnum   1
#define Anum_tcq_operators_opid    2
#define Anum_tcq_operators_numqrs  3
#define Anum_tcq_operators_kind    4
#define Anum_tcq_operators_qid	   5
#define Anum_tcq_operators_opstr   6
#define Anum_tcq_operators_opkind  7
#define Anum_tcq_operators_opdesc  8

#define TCQ_OPTRS_ENTRY		   "E"
#define TCQ_OPTRS_ADD		   "A"
#define TCQ_OPTRS_REMOVE	   "R"
#define TCQ_OPTRS_EXIT		   "X"

#define TCQ_OPTRS_FSTEM		   "F"
#define TCQ_OPTRS_GSFILTER	   "G"
#define TCQ_OPTRS_SCANMODULE   "M"
#define TCQ_OPTRS_UNKNOWN	   "U"

/*
 * Info specific to tcq_queues
 */
#define Natts_tcq_queues		   4
#define Anum_tcq_queues_tcqtime    0
#define Anum_tcq_queues_opid	   1
#define Anum_tcq_queues_qkind	   2
#define Anum_tcq_queues_kind	   3

#define TCQ_QUEUES_INPUT		"I"
#define TCQ_QUEUES_OUTPUT		"O"

#define TCQ_QUEUES_SUCC_ENQUEUE "E"
#define TCQ_QUEUES_FAIL_ENQUEUE "F"
#define TCQ_QUEUES_SUCC_DEQUEUE "D"
#define TCQ_QUEUES_FAIL_DEQUEUE "N"

/*
 *	Function prototypes of functions exported in dyncatalog.c
 */

extern void setupDynCatalogStreams(void);
extern bool isDynamicCatalogStream(Oid soid);

extern void tcq_queries_enter(planqentry * pqe);
extern void tcq_queries_exit(uint16 queueNo);
extern void tcq_operators_add(Fjord * node, Eddy * eddy, uint32 qid);
extern void tcq_operators_remove(Fjord * node, uint32 qid);
extern void tcq_queues_enqueue(uint32 opid, char *qk, char *k);
extern void tcq_queues_dequeue(uint32 opid, char *qk, char *k);

/*
 *	Don't hurt people who haven't enabled dynamic catalogs
 */
#define CHECK_DYNAMIC_CATALOGS		  {if (! dyncat->active) return;}

#endif   /* DYNCATALOG_H */
