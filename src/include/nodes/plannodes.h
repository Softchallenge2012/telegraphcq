/*-------------------------------------------------------------------------
 *
 * plannodes.h
 *	  definitions for query plan nodes
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: plannodes.h,v 1.88 2006/02/06 01:19:46 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PLANNODES_H
#define PLANNODES_H

#include "nodes/execnodes.h"
#include "misceddy.h"			/* @deddySK */
#include "rqueue/resultq.h"
#include "utils/bit.h"

#ifdef _USE_PRNG_MT
#include "prng.h"               /* @dfluxMS */
#endif

#include "utils/window_agg.h"

/* ----------------------------------------------------------------
 *	Executor State types are used in the plannode structures
 *	so we have to include their definitions too.
 *
 *		Node Type				node information used by executor
 *
 * control nodes
 *
 *		Result					ResultState				resstate;
 *		Append					AppendState				appendstate;
 *
 * scan nodes
 *
 *		Scan ***				CommonScanState			scanstate;
 *		IndexScan				IndexScanState			indxstate;
 *		SubqueryScan			SubqueryScanState		subquerystate;
 *		FunctionScan			FunctionScanState		functionstate;
 *
 *		  (*** nodes which inherit Scan also inherit scanstate)
 *
 * join nodes
 *
 *		NestLoop				NestLoopState			nlstate;
 *		MergeJoin				MergeJoinState			mergestate;
 *		HashJoin				HashJoinState			hashjoinstate;
 *
 * materialize nodes
 *
 *		Material				MaterialState			matstate;
 *		Sort					SortState				sortstate;
 *		Unique					UniqueState				uniquestate;
 *		SetOp					SetOpState				setopstate;
 *		Limit					LimitState				limitstate;
 *		Hash					HashState				hashstate;
 *		SteM					SteMState				stemstate;
 *
 * ----------------------------------------------------------------
 */


/* ----------------------------------------------------------------
 *						node definitions
 * ----------------------------------------------------------------
 */

/* ----------------
 *		Plan node
 * ----------------
 */

typedef struct Plan
{
	NodeTag		type;

	/* estimated execution costs for plan (see costsize.c for more info) */
	Cost		startup_cost;	/* cost expended before fetching any
								 * tuples */
	Cost		total_cost;		/* total cost (assuming all tuples
								 * fetched) */

	/*
	 * planner's estimate of result size (note: LIMIT, if any, is not
	 * considered in setting plan_rows)
	 */
	double		plan_rows;		/* number of rows plan is expected to emit */
	int			plan_width;		/* average row width in bytes */

	/*
	 * execution state data.  Having Plan point to this, rather than the
	 * other way round, is 100% bogus.
	 */
	EState	   *state;			/* at execution time, state's of
								 * individual nodes point to one EState
								 * for the whole top-level plan */

	struct Instrumentation *instrument; /* Optional runtime stats for this
										 * plan node */

	/*
	 * Common structural data for all Plan types.  XXX chgParam is runtime
	 * data and should be in the EState, not here.
	 */
	List	   *targetlist;
	List	   *qual;			/* implicitly-ANDed qual conditions */
	struct Plan *lefttree;
	struct Plan *righttree;
	List	   *extParam;		/* indices of _all_ _external_ PARAM_EXEC
								 * for this plan in global
								 * es_param_exec_vals. Params from
								 * setParam from initPlan-s are not
								 * included, but their execParam-s are
								 * here!!! */
	List	   *locParam;		/* someones from setParam-s */
	List	   *chgParam;		/* list of changed ones from the above */
	List	   *initPlan;		/* Init Plan nodes (un-correlated expr
								 * subselects) */
	List	   *subPlan;		/* Other SubPlan nodes */

	/*
	 * We really need in some TopPlan node to store range table and
	 * resultRelation from Query there and get rid of Query itself from
	 * Executor. Some other stuff like below could be put there, too.
	 */
	int			nParamExec;		/* Number of them in entire query. This is
								 * to get Executor know about how many
								 * param_exec there are in query plan. */

	/* @BdfluxMS */
	
	struct Plan *parent;		/* XXX Be careful, this is adjusted only
								 * for Flux plans right now. Need a parent
								 * for FluxCons */

	bool       pushPlan;        /* Is it push plan or pull plan? */

	bool       inDone;
	
	/* @EdfluxMS */

    Node *eventClause; /* -TCQ SR */

    bool        insideFAgg;     /* Set to TRUE if this node is executing 
                                 * within an FAgg node's aggregation tree.
                                 * Controls rescan behavior. */


} Plan;

/* ----------------
 *	these are are defined to avoid confusion problems with "left"
 *	and "right" and "inner" and "outer".  The convention is that
 *	the "left" plan is the "outer" plan and the "right" plan is
 *	the inner plan, but these make the code more readable.
 * ----------------
 */
#define innerPlan(node)			(((Plan *)(node))->righttree)
#define outerPlan(node)			(((Plan *)(node))->lefttree)

/*@BfluxMS */
#define parentPlan(node)		(((Plan *)(node))->parent)
#define IsPushPlan(node)        (((Plan *)(node))->pushPlan)
#define IsInputDone(node)       (((Plan *)(node))->inDone)
#define SetInputDone(node)      (((Plan *)(node))->inDone = true)
#define SetInputNew(node)       (((Plan *)(node))->inDone = false)
/*@EfluxMS */

/*
 * ===============
 * Top-level nodes
 * ===============
 */

/* all plan nodes "derive" from the Plan structure by having the
   Plan structure as the first field.  This ensures that everything works
   when nodes are cast to Plan's.  (node pointers are frequently cast to Plan*
   when passed around generically in the executor */


/* ----------------
 *	 Result node -
 *		If no outer plan, evaluate a variable-free targetlist.
 *		If outer plan, return tuples from outer plan that satisfy
 *		given quals (we can also do a level of projection)
 * ----------------
 */
typedef struct Result
{
	Plan		plan;
	Node	   *resconstantqual;
	ResultState *resstate;
} Result;

/* ----------------
 *	 Append node -
 *		Generate the concatenation of the results of sub-plans.
 *
 * Append nodes are sometimes used to switch between several result relations
 * (when the target of an UPDATE or DELETE is an inheritance set).	Such a
 * node will have isTarget true.  The Append executor is then responsible
 * for updating the executor state to point at the correct target relation
 * whenever it switches subplans.
 * ----------------
 */
typedef struct Append
{
	Plan		plan;
	List	   *appendplans;
	bool		isTarget;
	AppendState *appendstate;
} Append;

/*
 * ==========
 * Scan nodes
 * ==========
 */
typedef struct Scan
{
	Plan		plan;
	Index		scanrelid;		/* relid is index into the range table */
	CommonScanState *scanstate;
	bool		dontProject;	/* @deddySK */

	int			num_tuples;		/* @dstemsAD */
	int			num_tuples_seen_so_far; /* @dstemsAD */
	int			num_tuples_passed_quals_so_far; /* @dstemsAD */
	double		scan_selectivity;		/* @dstemsAD */
	bool        return_null_if_quals_fail;  /* @dstemsAD*/
} Scan;


/*
 * ==========
 * ScanQueue nodes
 * ==========
 */
/*@BdtcqWH */
typedef struct ScanQueue
{
	Plan		plan;
	Queue	   *queue;
	int16		qno;
	TupleDesc	tupDesc;
	ScanQueueState *scanqstate;
	q_serializer serializer;
	bool		dontProject;
    /*bool        ignorePunct;*/
        /* Set to TRUE if this ScanQueue should strip punctuation tuples out of
         * its output. */
}	ScanQueue;

/*@EdtcqWH */

/*
 * ==========
 * ScanSock node
 *
 * This node encapsulates a socket (for reading tuples directly into the
 * backend via CSV over TCP)
 * ==========
 */
typedef struct ScanSock
{
	Plan		plan;
	TupleDesc	tupDesc;
    ScanSockState   *scansockstate;
	bool		dontProject;
} ScanSock;


/* ----------------
 *		sequential scan node
 * ----------------
 */

typedef Scan SeqScan;

/*@BdwchMS */
/*
 * ----------
 * StreamScan node
 * ----------
 * Its a combination of a scan queue and seq. scan.
 * It opens a relation for reading, and reads from
 * an ingres queue.
 *
 * XXX currently the relation is just opened, and
 * nothing uses it.
 *
 * XXX yes, multiple inheritance means that we have
 * duplicated our state.
 */
/* structure used to map stream attibutes into the arguments
   for a source with bindings.	mapping[col] == -1 if it is not an
   argument, and equals the argument number otherwise.
*/
typedef struct Bindinginfo
{
	NodeTag		type;
	bool		hasbindings;
	Oid			wrapperoid;
  Oid                     mappingoid;
	int			atts;
	int		   *mapping;
  bool               *optional;
        Const          **defaults;
	List	   *targetlist;
	ProjectionInfo *projinfo;
	Oper	  **eq_opers;
	MemoryContext probecontext;
	Queue	   *probequeue;
}	Bindinginfo;

typedef struct StrmScan
{
	/*ScanQueue	scanq;*/
    ScanSock    scansock;
	SeqScan    *seqscan;
	bool		blocking;
	Oid			streamoid;
  Index       rtindex;
	char		streamtype;
    CommonScanState *scanstate;
	Bindinginfo **bindings;
	bool		useindex;
	int			numwrappers;
	List	   *pending_probes;
  TupleTableSlot *recurseOutputSlot;
  bool dependant_stream;    /* TCQOC */
	int			part_id;

    List       *tuples_to_dump;
                /* Only used when dump_unarchived_streams is set to TRUE. */

#ifdef NOT_USED
	/* @BfluxMS */
	/* XXX temp stuff for Flux testing */
	int			part_id;
	int			round;
	int			n_round;
	int			seqnum;
	Datum	   *values;
	prng_mt		prng;
	/* @EfluxMS */
#endif

}	StrmScan;

/*@EdwchMS */

/* ----------------
 *		index scan node
 * ----------------
 */
typedef struct IndexScan
{
	Scan		scan;
	List	   *indxid;
	List	   *indxqual;
	List	   *indxqualorig;
	ScanDirection indxorderdir;
	IndexScanState *indxstate;
} IndexScan;

/* ----------------
 *				tid scan node
 * ----------------
 */
typedef struct TidScan
{
	Scan		scan;
	bool		needRescan;
	List	   *tideval;
	TidScanState *tidstate;
} TidScan;

/* ----------------
 *		subquery scan node
 *
 * SubqueryScan is for scanning the output of a sub-query in the range table.
 * We need a special plan node above the sub-query's plan as a place to switch
 * execution contexts.	Although we are not scanning a physical relation,
 * we make this a descendant of Scan anyway for code-sharing purposes.
 *
 * Note: we store the sub-plan in the type-specific subplan field, not in
 * the generic lefttree field as you might expect.	This is because we do
 * not want plan-tree-traversal routines to recurse into the subplan without
 * knowing that they are changing Query contexts.
 * ----------------
 */
typedef struct SubqueryScan
{
	Scan		scan;
	Plan	   *subplan;
} SubqueryScan;

/* ----------------
 *		FunctionScan node
 * ----------------
 */
typedef struct FunctionScan
{
	Scan		scan;
	/* no other fields needed at present */
	/* scan.scanstate actually points at a FunctionScanState node */
} FunctionScan;

/*
 * ==========
 * Join nodes
 * ==========
 */

/* ----------------
 *		Join node
 *
 * jointype:	rule for joining tuples from left and right subtrees
 * joinqual:	qual conditions that came from JOIN/ON or JOIN/USING
 *				(plan.qual contains conditions that came from WHERE)
 *
 * When jointype is INNER, joinqual and plan.qual are semantically
 * interchangeable.  For OUTER jointypes, the two are *not* interchangeable;
 * only joinqual is used to determine whether a match has been found for
 * the purpose of deciding whether to generate null-extended tuples.
 * (But plan.qual is still applied before actually returning a tuple.)
 * For an outer join, only joinquals are allowed to be used as the merge
 * or hash condition of a merge or hash join.
 * ----------------
 */
typedef struct Join
{
	Plan		plan;
	JoinType	jointype;
	List	   *joinqual;		/* JOIN quals (in addition to plan.qual) */
} Join;

/* ----------------
 *		nest loop join node
 * ----------------
 */
typedef struct NestLoop
{
	Join		join;
	NestLoopState *nlstate;
} NestLoop;

/* ----------------
 *		merge join node
 * ----------------
 */
typedef struct MergeJoin
{
	Join		join;
	List	   *mergeclauses;
	MergeJoinState *mergestate;
} MergeJoin;

/* ----------------
 *		hash join (probe) node
 * ----------------
 */
typedef struct HashJoin
{
	Join		join;
	List	   *hashclauses;
	Oid			hashjoinop;
	HashJoinState *hashjoinstate;
} HashJoin;

/* ----------------
 *		stem hash join (probe) node -- other than having a SteM
 *		as the inner node, there are practically no changes from the Hashjoin
 *		code, at least in terms of the data structures.
 *		For simplicity, we will keep it that way.
 * ----------------
 */
typedef struct SteMHashJoin
{
	Join		join;
	List	   *hashclauses;
	Oid			hashjoinop;
	HashJoinState *hashjoinstate;

	/*
	 * This should really be a part of the state, but since I want to use
	 * the HashJoinState as it is right now, will make this compromise.
	 */
	IntermediateHeapTupleData tempiHeapTupleData;
	bool		doProject;		/* To project or not to project */
}	SteMHashJoin;

/* ---------------
 *		aggregate node
 * ---------------
 */
typedef struct Agg
{
	Plan		plan;
	AggState   *aggstate;
} Agg;

/* BfluxMS, borrowed from class project code */

/* ---------------
 *		aggregate node
 *
 * An Agg node implements plain or grouped aggregation.  For grouped
 * aggregation, we can work with presorted input or unsorted input;
 * the latter strategy uses an internal hashtable.
 *
 * Notice the lack of any direct info about the aggregate functions to be
 * computed.  They are found by scanning the node's tlist and quals during
 * executor startup.
 *
 *
 * Note: The following comment is not true for cs186/15-415 ..
 *
 *	 (It is possible that there are no aggregate functions; this could happen if
 *	 they get optimized away by constant-folding, or if we are using the Agg
 *	 node to implement hash-based grouping.)
 *
 *	 .. it applies only to devcvs
 *
 * ---------------
 */
typedef enum AggStrategy
{
	AGG_SORTED,					/* grouped agg, input must be sorted */
	/* and per-group terminated (old method) */
	AGG_HASHED,					/* grouped agg, use internal hashtable */
	AGG_HASHED_WINDOW			/* group, window, hashed agg, uses UDFs
								 * aggs. Currently just a hack. */
}	AggStrategy;

typedef struct HashAgg
{
	Plan		plan;
	HashAggState *aggstate;
	AggStrategy aggstrategy;
	int			numCols;		/* number of grouping columns */
	AttrNumber *grpColIdx;		/* their indexes in the target list */
	long		numGroups;		/* estimated number of groups in input */
}	HashAgg;

/* EfluxMS */

/* ---------------
 *	 group node -
 *		use for queries with GROUP BY specified.
 *
 *		If tuplePerGroup is true, one tuple (with group columns only) is
 *		returned for each group and NULL is returned when there are no more
 *		groups. Otherwise, all the tuples of a group are returned with a
 *		NULL returned at the end of each group. (see nodeGroup.c for details)
 * ---------------
 */
typedef struct Group
{
	Plan		plan;
	bool		tuplePerGroup;	/* what tuples to return (see above) */
	int			numCols;		/* number of group columns */
	AttrNumber *grpColIdx;		/* indexes into the target list */
	GroupState *grpstate;
} Group;

/*@BfluxMS */
typedef struct HashGroup
{
	HashAgg		agg;			/* Contains NodeTag */
	int			nparts;			/* Number of partitions this hash group
								 * handles. Currently not used */
	int			numaggs;		/* This is redundant for now.		  */
	AggWindowSpec *wndSpecs;	/* Window specifications for the aggs */

	/* For state movement */
	bool        parentDone;
	bool        frozen;         /* For state-movement */
	bool        debug;
}	HashGroup;

/*@EfluxMS */

/*@BwndaggWH */
/* ---------------
 *	 window node -
 *		similar to the Group node but generate groups of tuples
 *		based on a window entry and exit conditions
 *
 *		NULL returned at the end of each window. (see nodeWindow.c for details)
 * ---------------
 */
typedef struct Window
{
	Plan		plan;
	WindowClause *wndClause;
	WindowState *wndstate;
} Window;

/*@EwndaggWH */

/* ----------------
 *		materialization node
 * ----------------
 */
typedef struct Material
{
	Plan		plan;
	MaterialState *matstate;
} Material;

/* ----------------
 *		sort node
 * ----------------
 */
typedef struct Sort
{
	Plan		plan;
	int			keycount;
	SortState  *sortstate;
} Sort;

/* ----------------
 *		unique node
 * ----------------
 */
typedef struct Unique
{
	Plan		plan;
	int			numCols;		/* number of columns to check for
								 * uniqueness */
	AttrNumber *uniqColIdx;		/* indexes into the target list */
	UniqueState *uniquestate;
} Unique;

/* ----------------
 *		setop node
 * ----------------
 */
typedef enum SetOpCmd
{
	SETOPCMD_INTERSECT,
	SETOPCMD_INTERSECT_ALL,
	SETOPCMD_EXCEPT,
	SETOPCMD_EXCEPT_ALL
} SetOpCmd;

typedef struct SetOp
{
	Plan		plan;
	SetOpCmd	cmd;			/* what to do */
	int			numCols;		/* number of columns to check for
								 * duplicate-ness */
	AttrNumber *dupColIdx;		/* indexes into the target list */
	AttrNumber	flagColIdx;
	SetOpState *setopstate;
} SetOp;

/* ----------------
 *		limit node
 * ----------------
 */
typedef struct Limit
{
	Plan		plan;
	Node	   *limitOffset;	/* OFFSET parameter, or NULL if none */
	Node	   *limitCount;		/* COUNT parameter, or NULL if none */
	LimitState *limitstate;
} Limit;

/* ----------------
 *		hash build node
 * ----------------
 */
typedef struct Hash
{
	Plan		plan;
	Node	   *hashkey;
	HashState  *hashstate;
} Hash;

/* ----------------												  //@BdstemsAD
 *		SteM node
 * ----------------
 */
#define MAX_TO_TRACK 100

typedef struct Statistics
{
	int			num_bitsets_to_track_for_inserted;
	int			bitsets_to_track_for_inserted[MAX_TO_TRACK];
	int			num_inserted[MAX_TO_TRACK];

	int			num_bitsets_to_track_for_probed;
	int			bitsets_to_track_for_probed[MAX_TO_TRACK];
	int			num_probed[MAX_TO_TRACK];

	int			num_bitsets_to_track_for_returned;
	int			bitsets_to_track_for_returned[MAX_TO_TRACK];
	int			num_returned[MAX_TO_TRACK];
}	Statistics;

/** A common place to store all the domain size information, and maybe other
 * information in future. This is required because this is global information,
 * and not local to SteMs. SteMs and other modules will keep pointers into this
 * data structure, of which there should be only one instance.
 */
#define __MAX_DOMAINS__ 100
typedef struct DomainSizesInformation
{
    Node     *keys[__MAX_DOMAINS__];
    int     domain_sizes[__MAX_DOMAINS__];

    /** The following should be 'private' members not exposed to the outside. */
    int     num_keys; /* current number used */
} DomainSizesInformation;

typedef struct SteM
{
	Plan		plan;
	Node	   *innerhashkey;
	Node	   *outerhashkey;
    Var        *innerhashkey_var; 
    Var        *outerhashkey_var; 
	SteMState  *stemstate;
	List	   *stemclauses;
	struct SteM *dualSteM;
	bits8		sourceBuiltOn[BASIZE(TCQMAXSOURCES)];

	List	   *joinquals;		/* join quals other than the stemclauses
								 * -- this is basically copy of
								 * Hashjoin->join.joinqual */

	Plan	   *innerNode;		/* To be used if we are using SteM in the
								 * same way as Hash */

	Statistics	stats;			/* Read above for description -- NOT USED */

	int			domain_size_innerhashkey;		/** Domain sizes read off from the catalogue */
	int			domain_size_outerhashkey;

	/*
	 * Join selectivity is self-explanatory; cumulative selectivity is
	 * more complex. For a stem on R.a, we want to know how many tuples
	 * will result if we route a R tuple in that direction and it goes to
	 * the full extent possible. This will allow us to optimize locally.
	 * Ofcourse, I have no idea if this local optimization is globally
	 * optimal or not.
	 *
	 * So cumulative selectivity is not really a selectivity since the sizes
	 * of hte relations are included.
	 */
	double		join_selectivity;
	double		cumulative_selectivity;

    /* Common data stucture to maintin the domain sizes information */
    DomainSizesInformation *domain_sizes_information;

    /* Indexes for the outer and inner hash keys */
    int     outerhashkey_index;
    int     innerhashkey_index;
}	SteM;				/* @EdstemsAD */

/* symmetric hash join node */
typedef struct SymHashJoin
{
	Join join;
	List *joinqual;
	SymHashJoinState *shjstate;
	bool doProject;
	int attempts, exhaust_limit;
} SymHashJoin;

#define STEM_SOURCE(s)	   (BitArrayFirstBit((s)->sourceBuiltOn,TCQMAXSOURCES))

typedef enum EddyItem			/* @BdcacqSK */
{
	EDDY_SOURCE, EDDY_MODULE
}	EddyItem;			/* @EdcacqSK */

/* ----------------												   //@BdeddySK
 *	 Eddy node -
 * ----------------
 */
/** Very simple tree structure to store initial plan.
  *		NOT USED */
typedef struct EddyHint
{
	int			relation_no;
	struct EddyHint *left;
	struct EddyHint *right;
}	EddyHint;

typedef struct Eddy
{
	Plan		plan;
	ProjectionInfo **projectionInfo;
	Plan	  **sources;
	Plan	  **modules;		/* @dstemsAD */
	int			numSources;
	int			numModules;

	int			start_from_source;
	int			use_dual_routing_policy;

	bool		cacq;			/* @dcacqSK */
	List	   *rangetable;		/* @dcacqSK */
	EddyState  *eddystate;		/* @dstemsAD */
	int			numTuplesProcessed;		/* @dstemsAD */
	int			numTuplesOutput;	/* @dstemsAD */

	double		total_delay;	/* @dstemsAD */
	long		begin_time;		/* @dstemsAD */

	int			num_relations_in_query; /* This may not be the same as
										 * number of sources above as the
										 * numSources really refers to
										 * number of source modules */

	TupleDesc	base_tuple_descs[__MAX_RELATIONS__];	/* The global base tuple
														 * descriptors array */

	FromExpr   *jointree;		/* The original jointree -- to be kept
								 * around for remembering the hints. */

    DomainSizesInformation *domain_sizes_information;

    /* Connection state information. */
    int         acceptSock;
        /* Socket that we accept incoming connections on. */
    List        *startingSocks;
        /* Stream data sockets that haven't yet been mapped to their streams.
         */
    List        *unscannedSocks;
        /* Stream data sockets that have been associated with a relation
         * but have no ScanSock associated with them. 
         * Each element of this list is a SockState struct. */



}	Eddy;				/* @EdeddySK */

/* ----------------
 *		Index SteM node -- code borrowed from the NestLoop struct above.
 * ----------------
 */
typedef struct IndexSteM
{
	Join		join;
	int			sourceBuiltOnID;	/* one of these is redundant */
	bits8		sourceBuiltOn[BASIZE(TCQMAXSOURCES)];
	bits8		probeSource[BASIZE(TCQMAXSOURCES)];
	IndexSteMState *isstate;
}	IndexSteM;

/* @BdfluxMS */
/* ---------------
 *	FSched node -
 * ---------------
 */

/* Forward declarations */
typedef struct FluxAcceptor FluxAcceptor;

/* XXX Make the segs and fluxen to be growing arrays */
typedef struct FSched
{
	Plan		plan;
	int			ntups;
	int			numSegs;
	Plan	  **segs;			/* Dataflow segments to schedule */
	int			nsegs_ent;		/* Total number of entries in segs */
	Plan	   *topSeg;			/* This node returns the result  */
	int			nfluxen;		/* Total number of fluxen	  */
	Plan	  **fluxen;			/* Array of fluxen in the scheduler, and
								 * statemover */
	int			nfluxen_ent;	/* Total number of entries in fluxen */
	int			qno;			/* My query id, so I know when to end */
	bool		kill_fsched;
	FluxAcceptor *faccept;
	int			n_recover;
	int         ddeclust;       /* Degree of declustering */
}	FSched;

/*
 * This is a misnomer, its really just an Exchange
 */

typedef struct FluxProd
{
	Plan		plan;
	FSched	   *sched;
	bool		all_done;		/* Am I all connected? */
	int			node_id;		/* Which FluxProd (machine) instance am I? */
	int			fcons_idx;		/* Index for the corresponding flux-cons
								 * in the acceptor */
	int			flux_idx;		/* My index in the scheduler   */
	Queue	   *local_q;		/* Local queue, if it exists   */
	int			nCons;			/* Number of consumer entries  */
	int		   *map_c2m;		/* Map connection to real machine */
	int		   *map_m2c;		/* Map from real machine to connection */
	FluxProdState *fpstate;		/* Initialized during ExecInit */

	TupleDesc  tupDesc;
	void	   *rstate;			/* Continuation state for routing function */

	/* Routing function functions */
	void		(*route_fun_init) (struct FluxProd * fp);
	int			(*route_fun) (struct FluxProd * fp, TupleTableSlot *slot);



	
}	FluxProd;

typedef struct FluxCons
{								/* These are things initialized during
								 * node creation */
	Plan		plan;
	FSched	   *sched;
	bool		all_done;		/* Am I all connected? */
	int			node_id;		/* Which FluxCons (machine) instance am I? */
	int			fprod_idx;		/* Index for the corresponding flux-prod
								 * in the acceptor */
	int			flux_idx;		/* My index in the flux acceptor */
	Queue	   *local_q;		/* Local queue, if it exists */
	int			nProd;			/* Number of producer entries + 1 (for the
								 * local one) */
	int		   *map_c2m;		/* Map connection to real machine */
	int		   *map_m2c;		/* Map from real machine to connection */
	FluxConsState *fcstate;		/* Initialized during ExecInit */

	TupleDesc	tupDesc;

}	FluxCons;

/*
 * I know I should inherit from above, but I would much rather not
 * while I am building.  This is the fault-tolerant Flux. Rather the
 * 2-tolerant Flux.
 */

/* Various modes used by Flux */
typedef enum FluxMode
{
	FLUX_NORMAL,
	FLUX_FAIL_OVER,
	FLUX_FAIL_OVER_1,
	FLUX_TAKE_OVER,
	FLUX_TAKE_OVER_1,
	FLUX_STANDBY,
	STANDBY_NORMAL,
	STANDBY_FAIL_OVER,
	STANDBY_FAIL_OVER_1,
	STANDBY_TAKE_OVER,
	STANDBY_TAKE_OVER_1,
	STATE_MOVEMENT
}	FluxMode;

typedef enum TakeOverMode
{
	COPY_STATE,
	CATCH_UP
}	TakeOverMode;

typedef struct FluxProd_FT
{
	FluxProd	fprod;			/* Has plan in it */
	FluxRole	m_role;			/* Role as primary, secondary */
	Queue	   *local_r;		/* Ack queue - primary	 */
	Queue	   *local_q_s;		/* Forward queue		 */
	Queue	   *local_r_s;		/* Ack queue - secondary */
	FluxProdState_FT *fpstate_ft;

	bool		isPair;			/* Tells you if it is replicated */
	int			fcons_idx_s;	/* Index of the replicated fcons */

	FluxMode	mode;			/* State of the operator */

	bool		sync;			/* Tells me that my pair is synchronized */

	struct Instrumentation *instrument;

	int			take_over_type;
	int			spare_m;

	bool        isStandby;

	/* State movement */
	FluxMode    prev_mode;

	bool        do_stat;
}	FluxProd_FT;

typedef struct FluxCons_FT
{								/* These are things initialized during
								 * node creation */
	FluxCons	fcons;			/* Has plan in it */
	FluxRole	m_role;			/* Role as primary, secondary */
	Queue	   *local_r;		/* Ack queue - primary	 */
	Queue	   *local_q_s;		/* Forward queue		 */
	Queue	   *local_r_s;		/* Ack queue - secondary */
	FluxConsState_FT *fcstate_ft;

	bool		isPair;			/* Tells you if it is replicated */
	int			fprod_idx_s;	/* Index of the replicated fprod */

	FluxMode	mode;			/* State of the operator */

	struct Instrumentation *instrument;

	/* Variables needed for fail-over and catch-up or replay */
	FluxProd_FT *fprod_parent;	/* Flux producer that is parent */
	int			spare_m;

	struct sm_request 	*curr_smr;
	bool        		 ready_flag;
	Queue	   			*curr_q;

	bool                 move_done;
	
	int 				 last_seqnum;
	
	bool                 isStandby;
	
} FluxCons_FT;

typedef struct SegProd
{
	FluxProd_FT fprod;
}	SegProd;

/* Forward declaration */
struct sm_request;

typedef struct SegCons
{
	FluxCons_FT fcons;
	struct sm_request *curr_smr;
	bool		ready_flag;
	Queue	   *curr_q;

	bool        move_done;
}	SegCons;

typedef struct FluxEgress
{
	FluxCons_FT fcons;
	bool        use_ts;
	int         ts_sec_col;
	int         ts_usec_col;
}	FluxEgress;

typedef struct FluxIngress
{
	FluxProd_FT fprod;
	bool        no_acks;
	int			f_part_p;
	int			f_part_s;
}	FluxIngress;

struct FluxAcceptor
{
	Plan		plan;
	FSched	   *sched;			/* Pointer to Fjord scheduler */
	int			acceptfd;		/* Port on which to accept connections */
	bool		blocked;		/* Should I service connections?	   */

	int			p_sockfd;		/* Sockfd not yet connected */
	int			p_buf[3];		/* Values sent for connection */
	int			p_buf_read;		/* Number of bytes read from p_sockfd */
	struct sockaddr_in *p_addr; /* Address information for socker */
};

typedef struct DataGen
{
	Plan		plan;
	CommonState *cstate;
	int			ncols;
	int			seqno_col;
	int			*rand_col;
/* 	int         rand_col2; */
	int			*max;
	int         nrandcols;
/* 	int         max2; */
	int			seqnum;
	Datum	   *values;
	char	   *nulls;
	/* TBD TBD : Mehul - check this */
#ifdef _USE_PRNG_MT
	prng_mt		prng;
#endif
	
	TupleDesc	tupDesc;
	int         skip;
	int         offset;

	/* For state extraction and movement */
	bool        parentDone;
	int         tmp_seqnum;

	bool        debug;

	bool        use_ts;
	int         ts_sec;
	int         ts_usec;
	int         ts_freq;
	
}	DataGen;

typedef struct StateMover
{
	FluxProd	fprod;
	Queue	   *request_queue;	/* Requests coming in from various
								 * modules.  */
	Queue	   *move_bufs;		/* Queue of buffers to move across
								 * machines. */
	Queue	   *empty_q;		/* Empty q to signal end of transmission */
	
	int			n_smovers;		/* Number of statmovers, including this
								 * one. */
	FluxConnInfo *conn_info; /* Connection to other state_movers. */
	int			n_conn;			/* Number of connections made */
	int			curr_conn;		/* Index variable */
	struct sm_request *curr_req;	/* Current state mover request */
	QueueComm  *curr_comm;		/* Current queue to send */
}	StateMover;

/* @EdfluxMS */


/* ----------------												   @BdeddySK
 *	 Filter node -
 * ----------------
 */
typedef struct Filter
{
	Plan		plan;
	CommonScanState *scanstate;
	Node	    *filterqual;
    int         cost;
	bits8		sourceFiltered[BASIZE(TCQMAXSOURCES)];
}	Filter;														 /* @EdeddySK */

/* ------------------------------------------------------------------------------
 * Begin Fjords
 * ------------------------------------------------------------------------------
 */

/* -------------------											   //@BdcacqSK
 *		Fjord node -
 * -------------------
 */
typedef struct Fjord
{
	Plan				  plan;
	List 				 *thisMOlist;
	Queue	   			 *input;
	Queue	   			 *output;
	List	   			 *usedByQueries;
	bits8       		  interestedQueries[BASIZE(TCQMAXQUERIES)];	
	int8				  operator;
	IntermediateHeapTuple iiht;
	IntermediateHeapTuple oiht;
	CommonScanState 	 *scanstate;
	MemoryContext 		  mctx;	/* memory ctx with operator lifetime */
  MemoryContext             tempContext;  /* temporary memory context automatically reset
					     after each call by the fjord */
  bool                              usesTempContext;
	bool				  dynCat;		/* processes dynamic catalog streams */

	struct Fjord 		**outputs;		/* @dsigmodSK */
	int8 		 		  numOutputs;	/* @dsigmodSK */
	int 		 		  driver;		/* @dsigmodSK */
	bool                  doneInput;	/* @dsigmodSK */
	
}	Fjord;				/* @EdcacqSK */


/* ----------------------------------								@BdcacqSK
 *	 Grouped Selection Filter node -
 * ----------------------------------
 */
typedef struct GSFilter
{
	Fjord		 fjord;
	Oper	   	*gtoper;
	Oper	   	*ltoper;
	Oper	   	*neoper;
	Oper	   	*eqoper;
	Oper	   	*gtoperinsert;
	Oper	   	*ltoperinsert;
	Oper	   	*neoperinsert;
	Oper	   	*eqoperinsert;
	Node	   	*node;
	ConstTree  	*ltmap;
	ConstTree  	*rtmap;
	List	   	*eqmap;
	List	   	*nemap;
	Oid			 consttype;
	int          tuplesEntered;
	int          tuplesExited;
	List 		*all_exprs;
}	GSFilter;			/* @EdcacqSK */

/* ---------------------
 *	 Fjord Scan node  -
 * ---------------------
 */
typedef struct ScanModule
{
	Fjord		fjord;
	Plan	   *scan;
	bool		isDone;
	bool		needsrescan;
	bool		sourcehasvarbindings;
  bool            ispullsource;
	int			numpendingprobes;
	int         tuplesScanned;
  int         itersIdle;
  bool  nodup;
  bool haveNewTuple; /* - TCQ SR */
  Timestamp currentTupleTimestamp; /* - TCQ SR */
  bool isRecursivelyFed;  /* - TCQ SJ */
  Queue *recurseResult;
}	ScanModule;

/* ---------------------
 *	 FSteM nodes keep an array of these structs to manage their
 *	 underlying SteMs.
 * ---------------------
 */
typedef struct SubStemInfo
{
	SteM	   *stem;			/* Pointer to the SteM.  NULL if this slot
								 * isn't used. */
}	SubStemInfo;

/* NOTE: Do NOT set this constant to 1!!! */
#define FSTEM_NUM_SUBSTEMS 3
/*
 * ---------------------											 //@BdjoinSK
 *	 Fjord SteM  node -
 *
 *	 FSteMClauses is a helper struct that maintains a stemclauses, and a list
 *	 of interested queries.
 *
 *	 FSteM maintains a list of outerhashkey's for that FSteM. For each
 *	 outerhashkey, we have a list of FSteMClauses, which in turn store a list of
 *	 interested queries.
 *
 *	 -- Amol.
 * ---------------------
 */
typedef struct FSteMClauses
{
	List	   *stemclauses;
	List	   *interested_queries;		/* list of integers */
}	FSteMClauses;

typedef struct FSteMOuterHashkey
{
	Node	   *outerhashkey;
	List	   *fstemclauses_list;
}	FSteMOuterHashkey;

typedef struct FSteM
{
	Fjord		fjord;

	/* FLAGS */
	bool		initialized;	/* Sanity check.  Set to TRUE when
								 * ProcessInitFSteM() is called. Might be
								 * TRUE by random chance, though, so be
								 * careful! */
	bool		haveprobe;
	bool		probehasresults;	/* has the current probe returned results */
	bool		doneprobe;

	List	   *fstemouterhashkeys;
	List	   *currentouterhashkey;	/* pointer into the above list */
	List	   *currentfstemclauses;	/* pointer into the
										 * FSteMOuterHashkey->fstemclauses_
										 * list */
	bool		window_expr_is_valid;

	/*
	 * We set this to FALSE when this FSteM does not have a window size.
	 */

	/*
	 * NOTE: Currently, no window size means don't keep any tuples around
	 * in the substems.
	 */

	/* SUBSTEMS */
	SteM	   *protostem;		/* When creating new SteMs off of
								 * substems[], we make copies of this
								 * SteM. */

	bool		isindexstem;

	SubStemInfo substems[FSTEM_NUM_SUBSTEMS];
	/* Treated as a circular buffer. */

	/* OUTPUT */
	IntermediateHeapTuple unqueued_output;

	/*
	 * The last output tuple that we tried to enqueue but couldn't because
	 * we ran out of space.  NULL if such a tuple doesn't exist.
	 */

	/* BOOKKEEPING */
	int			insert_ss_index;/* Index of subSTeM that we should insert
								 * the next tuple into.  Should be the
								 * newest SteM. */
	int			probe_ss_index; /* Index of subSTeM we're currently
								 * probing.  Should be between
								 * first_ss_index and insert_ss_index. */
	int			first_ss_index; /* Index of oldest subSTeM. */

	int			max_work_units; /* The maximum number of tuples the FSteM
								 * will produce or consume during a single
								 * call to processNextFSteM(). */

	Timestamp   last_build_ts;  /* Timestamp of last tuple bulit in FSteM */
	TupleDesc   build_td;       /* Cache the TupleDesc of build source */
	HeapTuple   punct;          /* Last punctuation tuple created here */
	bool        punctjoin;      /* Need to output join punctuation tuple */
	bool        punctprobe;		/* Need to bounce back probe punctuation */
	
	/* CACHED EXPRESSIONS */
	Var		   *ts_accessor;

	/*
	 * An iterator that provides access to the timestamp field of a tuple
	 * stored inside an expression context.  Used to construct
	 * *window_expr (below).
	 */

	Expr	   *same_stem_expr;

	/*
	 * A Boolean expression that takes two tuples as arguments. returns
	 * TRUE if they should go within the same substem.
	 */

	Expr	   *stem_expired_expr;

	/*
	 * A Boolean expression that takes two tuples as arguments.  The first
	 * tuple should be the newest tuple in a SteM, and the second tuple
	 * should be the newest tuple we can get our hands on. Returns TRUE if
	 * the indicated substem has expired.
	 */


	ExprContext *window_expr_args;
	/*
	 * Expression context that holds pointers to the two tuples that are
	 * currently being used as arguments to either window expression.
	 */

	Expr	   *ts_comp_expr;


	bool		sourcehasbindings;
	ScanModule *source;


	int			fstem_num_tuples_inserted;
	int			fstem_num_tuples_probed;
	int			fstem_num_tuples_returned;

	int  fstem_num_built;
	int  fstem_num_input_for_probes;
	int  fstem_num_early_eliminated;
	int  fstem_num_probe_results;
	int  fstem_num_probe_bouncebacks;
	int  fstem_num_substems_ended;

	int total_probed;
	int total_inserted;
	int total_returned;

}	FSteM;				/* @EdjoinSK */

/* ---------------------
 *	 Fjord Output module
 * ---------------------
 */
typedef struct FOutput
{
	Fjord				 fjord;
	DR_resultqdest 		*resultqdest;
	bits8 				 signature[BASIZE(TCQMAXSOURCES)];
	bits8  				 candidates[BASIZE(TCQMAXQUERIES)];
	int 				 total_entered;
	int 				 total_output;

	/* new Output data based on SIGMOD PS paper */
	Datum running_diff_sum[TCQMAXQUERIES];
	int 		ts_attr[TCQMAXSOURCES];
	Interval    count[TCQMAXQUERIES];
	
} FOutput;

/* ---------------------
 *	 Fjord Split module
 * ---------------------
 */
typedef struct FSplit
{
	Fjord 		fjord;
	int 		total_entered;
} FSplit;
	

/* ----------------												   @BdeddyOC
 *	 FFilter node - A Fjord Filter
 * ----------------
 */
typedef struct FFilter
{
	Fjord		 fjord;
	Node	   	*filterqual;
	int 		 total_entered;
	int 		 total_exited;
}	FFilter;				/* @EdeddyOC */



/* ----------------												   @BdeddyOC
 *	 FAgg node - A Fjord Aggregator
 *       Will include multiple nodes to implement an aggregate computation (Sort, Unique, Agg, Window)
 * ----------------
 */
typedef struct FAgg
{
    Fjord		 fjord;
    Plan	   	*parentPlan;
    Plan	   	*aggPlan;
    int 		 total_entered;
    int 		 total_exited;
    int          total_aggregated_this_window;

    TupleDesc    input_tdesc;
                    /* This describes the join tuples that this operator
                     * consumes. */

    TupleDesc   output_tdesc;
                    /* This describes the join tuples that this operator
                     * pulls off the aggregation tree and returns. */

    Timestamp wtime;    /* holds window time to be returned by wtime(*) agg */

    List        *window_states;
                    /* A list of WindowState objects, one per windowed base 
                     * stream */

    Timestamp   current_time;
                    /* The node's concept of real time, derived from observed
                     * timestamps. */
    Timestamp   last_slide_time;
                    /* The timestamp of the last slide operation that this node
                     * has completed. */
    Timestamp   next_slide_time;
    Timestamp   latest_slide_time;
                    /* Time at which the longest-lived currently active window
                     * will end. */
    
    TupleList   *live_tuples_head; 
    TupleList   *live_tuples_tail; 
                    /* Doubly-linked list of live intermediate tuples. */

    TupleList   *live_tuples_cur;
                    /* Our current position in the live tuples list when we are
                     * scanning them into the aggregation tree.  This is the
                     * tuple that ExecFAgg() will return the NEXT time it is
                     * called.
                     */

    ActivationGroup     *activation_groups;
    ActivationGroup     *deactivation_groups;
                    /* Groups of tuples that enter/leave the live tuples list
                     * at the same time.  */


	bool         		 slicedAgg;

    FAggState   *faggstate;

}	FAgg;				/* @EdeddyOC */


/* ---------------- 
 *	 FEventAgg node - A Fjord-based Event Aggregator
 *      
 *       - TCQ SR
 * ----------------
 */

typedef struct FEventAgg 
{
  Fjord fjord;
  Node *eventExpr; /* Actually, its an EventExpr* */
  /* Need to add an FEventAggState attribute */
  
  FEventAggState mystate;

} FEventAgg;


/* ------------------------------------------------------------------------------
 * End Fjords
 * ------------------------------------------------------------------------------
 */

/* NOTE: Do NOT set this constant to 1!!! */
#define ASTEM_NUM_SUBSTEMS 3

/* this stuff needs to get moved out of the fjord section */
typedef struct ASteM                                           
{
	Plan plan;

	/* ASteMState */
	ASteMState *astemstate;
		
	/* FLAGS */
	List 		*stemclauses;
	Node 		*outerhashkey;
	Node 		*innerhashkey;
	
	/* rest of FLAGS moved to ASteMState */

	/* SUBSTEMS */
	SteM  		*protostem;	/* When creating new SteMs off of substems[], we
							   make copies of this SteM. */

	SubStemInfo substems[ASTEM_NUM_SUBSTEMS];
    /* Treated as a circular buffer. */

	/* OUTPUT */
	/* moved to ASteMState */
	
	/* BOOKKEEPING */
	/* moved to ASteMState */

	
	/* CACHED EXPRESSIONS */
	/* moved to ASteMState */

	/* need to copy this info so windows can be added at initialization time */
	WindowClause 		*wc;
	
	
} ASteM;

#ifdef NOT_USED
/* -------------------
 *		Tee node information
 *
 *	  leftParent :				the left parent of this node
 *	  rightParent:				the right parent of this node
 * -------------------
*/
typedef struct Tee
{
	Plan		plan;
	Plan	   *leftParent;
	Plan	   *rightParent;
	TeeState   *teestate;
	char	   *teeTableName;	/* the name of the table to materialize
								 * the tee into */
	List	   *rtentries;		/* the range table for the plan below the
								 * Tee may be different than the parent
								 * plans */
}	Tee;
#endif

/* ---------------------
 *		SubPlan node
 * ---------------------
 */
typedef struct SubPlan
{
	NodeTag		type;
	Plan	   *plan;			/* subselect plan itself */
	int			plan_id;		/* dummy thing because of we haven't equal
								 * funcs for plan nodes... actually, we
								 * could put *plan itself somewhere else
								 * (TopPlan node ?)... */
	List	   *rtable;			/* range table for subselect */
	/* setParam and parParam are lists of integers (param IDs) */
	List	   *setParam;		/* non-correlated EXPR & EXISTS subqueries
								 * have to set some Params for paren Plan */
	List	   *parParam;		/* indices of corr. Vars from parent plan */
	SubLink    *sublink;		/* SubLink node from parser; holds info
								 * about what to do with subselect's
								 * results */

	/*
	 * Remaining fields are working state for executor; not used in
	 * planning
	 */
	bool		needShutdown;	/* TRUE = need to shutdown subplan */
	HeapTuple	curTuple;		/* copy of most recent tuple from subplan */
} SubPlan;

#endif   /* PLANNODES_H */
