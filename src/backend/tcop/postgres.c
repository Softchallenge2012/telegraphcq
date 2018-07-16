

/*-------------------------------------------------------------------------
 *
 * postgres.c
 *	  POSTGRES C Backend Interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/tcop/postgres.c,v 1.115 2006/02/06 01:19:46 phred Exp $
 *
 * NOTES
 *	  this is the "main" module of the postgres backend and
 *	  hence the main module of the "traffic cop".
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include "access/xlog.h"
#include "commands/async.h"
#include "commands/trigger.h"
#include "executor/executor.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/print.h"
#include "optimizer/cost.h"
#include "optimizer/planner.h"
#include "optimizer/clauses.h"
#include "optimizer/tlist.h"
#include "optimizer/bindings.h"
#include "optimizer/eddy.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "rewrite/rewriteHandler.h"
#include "rqueue/resultq.h"		/* @dtcqSK */
#include "rqueue/rqdest.h"		/* @dtcqSK */
#include "rqueue/planq.h"		/* @dtcqSK */
#include "storage/ipc.h"
#include "storage/proc.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "telegraphcq/telegraphcq.h"	/* @dtcqSK */
#include "telegraphcq/wrapch.h" /* @dtcqSC */
#include "utils/debug.h"		/* @dtcqSK */
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "mb/pg_wchar.h"
#include "executor/nodeStrmScan.h"
#include "executor/nodeFAgg.h"

#include "pgstat.h"


void		test_queue(int, int);		/* @dtcqOC */

bool		readjust_range_table(List *old_rtable, List **new_rtable_ptr, int *new_indexes);	/* @dstemsAD */

void parse_query_in_parse_context (MemoryContext* oldcontext, 
				   MemoryContext parse_context, 
				   List **pparsetree_list, 
				   StringInfo query_string); /* @dre-orgAE */

void shallow_copy_parse_tree_in_query_context (List **pparsetree_list,
					       List **pnewtree_list); /* @dre-orgAE */

void analyze_and_rewrite_in_parse_context (MemoryContext parse_context,
					   Node** pparsetree,
					   List** pquerytree_list,
					   bool* pisRecursive); /* @dre-orgAE */

void shallow_copy_querytree_in_query_context (List **pquerytree_list,
					      List **pnewtreelist,
					      bool *pisRecursive); /* @dre-orgAE */

void convert_to_fsched_dup_in_parse_context (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree); /* @dre-orgAE */

void convert_plan_to_eddy_in_parse_context  (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree); /* @dre-orgAE */

void seqscan_to_eddy_in_parse_context_usecqeddy (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree); /* @dre-orgAE */

void seqscan_to_eddy_in_parse_context_runsqinfe (MemoryContext parse_context,
						 Plan **pplan,
						 Query **pquerytree,
						 bool *pisBlocking); /* @dre-orgAE */

void seqscan_to_eddy_in_parse_context_tcqfe (MemoryContext parse_context,
						 Plan **pplan,
						 Query **pquerytree); /* @dre-orgAE */

void assign_beplan_rt_in_parse_context (MemoryContext parse_context,
					Plan** pbePlan,
					Query** pquerytree,
					bool* pret); /* @dre-orgAE */

void append_querystr_to_pqe_in_parse_context (MemoryContext *oldcontext,
					      MemoryContext parse_context,
					      planqentry* ppqe,
					      StringInfo* query_string); /* @dre-orgAE */

void create_FEagg_node_in_parse_context (MemoryContext parse_context,
					 Plan** pfeplan,
					 List** pwl,
					 Query** pquerytree,
					 Plan** pbePlan); /* @dre-orgAE */

void add_pqe_to_pqe_list_in_parse_context (MemoryContext parse_context,
					   planqentry *ppqe,
					   List** ppqe_list); /* @dre-orgAE */
 
/* ----------------
 *		global variables
 * ----------------
 */

extern int	optind;
extern char *optarg;

char	   *debug_query_string; /* for pgmonitor and
								 * log_min_error_statement */

/* Note: whereToSendOutput is initialized for the bootstrap/standalone case */
CommandDest whereToSendOutput = Debug;

static bool dontExecute = false;

/* note: these declarations had better match tcopprot.h */

sigjmp_buf	Warn_restart;
sigjmp_buf	wch_restart;
sigjmp_buf *elog_jump_buf = &Warn_restart;
bool		Warn_restart_ready = false;
bool		InError = false;

static bool EchoQuery = true;	/* default don't echo */

/*
 * Flag to mark SIGHUP. Whenever the main loop comes around it
 * will reread the configuration file. (Better than doing the
 * reading in the signal handler, ey?)
 */
static volatile bool got_SIGHUP = false;

/* ----------------
 *		people who want to use EOF should #define DONTUSENEWLINE in
 *		tcop/tcopdebug.h
 * ----------------
 */
#ifndef TCOP_DONTUSENEWLINE
int			UseNewLine = 1;		/* Use newlines query delimiters (the
								 * default) */

#else
int			UseNewLine = 0;		/* Use EOF as query delimiters */
#endif   /* TCOP_DONTUSENEWLINE */

/*
** Flags for expensive function optimization -- JMH 3/9/92
*/
int			XfuncMode = 0;

/* ----------------------------------------------------------------
 *		decls for routines only used in this file
 * ----------------------------------------------------------------
 */
static int	InteractiveBackend(StringInfo inBuf);
static int	SocketBackend(StringInfo inBuf);
static int	ReadCommand(StringInfo inBuf);
static int	StaticReadCommand(StringInfo inBuf);
static void start_xact_command(void);
static void finish_xact_command(bool forceCommit);
static void SigHupHandler(SIGNAL_ARGS);
static void FloatExceptionHandler(SIGNAL_ARGS);
static const char *CreateCommandTag(Node *parsetree);
static List * analyze_and_rewrite_recursive_query(Node * parsetree);
static List * check_recursive_query_batch(List * querytree_list);
static List * recursive_query_batch_to_qtlist(SelectStmt * s, List *query_trees, char* portalName);
/* ----------------------------------------------------------------
 *		routines to obtain user input
 * ----------------------------------------------------------------
 */

/* ----------------
 *	InteractiveBackend() is called for user interactive connections
 *	the string entered by the user is placed in its parameter inBuf.
 *
 *	EOF is returned if end-of-file input is seen; time to shut down.
 * ----------------
 */

static int
InteractiveBackend(StringInfo inBuf)
{
	int			c;				/* character read from getc() */
	bool		end = false;	/* end-of-input flag */
	bool		backslashSeen = false;	/* have we seen a \ ? */

	/*
	 * display a prompt and obtain input from the user
	 */

	printf("backend> ");
	fflush(stdout);

	/* Reset inBuf to empty */
	inBuf->len = 0;
	inBuf->data[0] = '\0';

	for (;;)
	{
		if (UseNewLine)
		{
			/*
			 * if we are using \n as a delimiter, then read characters
			 * until the \n.
			 */
			while ((c = getc(stdin)) != EOF)
			{
				if (c == '\n')
				{
					if (backslashSeen)
					{
						/* discard backslash from inBuf */
						inBuf->data[--inBuf->len] = '\0';
						backslashSeen = false;
						continue;
					}
					else
					{
						/* keep the newline character */
						appendStringInfoChar(inBuf, '\n');
						break;
					}
				}
				else if (c == '\\')
					backslashSeen = true;
				else
					backslashSeen = false;

				appendStringInfoChar(inBuf, (char) c);
			}

			if (c == EOF)
				end = true;
		}
		else
		{
			/*
			 * otherwise read characters until EOF.
			 */
			while ((c = getc(stdin)) != EOF)
				appendStringInfoChar(inBuf, (char) c);

			if (inBuf->len == 0)
				end = true;
		}

		if (end)
			return EOF;



		/*
		 * otherwise we have a user query so process it.
		 */
		break;
	}

	/*
	 * if the query echo flag was given, print the query..
	 */
	printf("QUERY: %s\n", inBuf->data);
	if (EchoQuery)
		printf("statement: %s\n", inBuf->data);
	fflush(stdout);

	return 'Q';
}

/* ----------------
 *	SocketBackend()		Is called for frontend-backend connections
 *
 *	If the input is a query (case 'Q') then the string entered by
 *	the user is placed in its parameter inBuf.
 *
 *	If the input is a fastpath function call (case 'F') then
 *	the function call is processed in HandleFunctionRequest()
 *	(now called from PostgresMain()).
 *
 *	EOF is returned if the connection is lost.
 * ----------------
 */

static int
SocketBackend(StringInfo inBuf)
{
	int			qtype;

	/*
	 * get input from the frontend
	 */
	qtype = pq_getbyte();

	switch (qtype)
	{
		case EOF:
			/* frontend disconnected */
			break;

			/*
			 * 'Q': user entered a query
			 */
		case 'Q':
			if (pq_getstr(inBuf))
				return EOF;
			break;

			/*
			 * 'F':  calling user/system functions
			 */
		case 'F':
			if (pq_getstr(inBuf))
				return EOF;		/* ignore "string" at start of F message */
			break;

			/*
			 * 'X':  frontend is exiting
			 */
		case 'X':
			break;

			/*
			 * otherwise we got garbage from the frontend.
			 *
			 * XXX are we certain that we want to do an elog(FATAL) here?
			 * -cim 1/24/90
			 */
		default:
			elog(FATAL, "Socket command type %c unknown", qtype);
			break;
	}

	return qtype;
}

/* ----------------
 *		ReadCommand reads a command from either the frontend or
 *		standard input, places it in inBuf, and returns a char
 *		representing whether the string is a 'Q'uery or a 'F'astpath
 *		call.  EOF is returned if end of file.
 * ----------------
 */

static int
ReadCommand(StringInfo inBuf)
{
	int			result;

	if (IsUnderPostmaster)
		result = SocketBackend(inBuf);
	else
		result = InteractiveBackend(inBuf);

	return result;
}

static char *commands[] = {		/* @BdstemsAD */
	"select * from supplier where s_nationkey = '1' and s_acctbal > 9900;",		/* 0 */
	"select * from orders where o_totalprice > 100 and o_custkey = '1';",		/* 1 */
	"select * from lineitem where l_partkey = '1' and l_suppkey = '1';",		/* 2 */
	"explain analyze select * from nation, region where n_regionkey = r_regionkey;",	/* 3 */
	"select * from region r1, region r2 where r1.r_regionkey = r2.r_regionkey;",		/* 4 */
	"select * from region r1, region r2, region r3 where r1.r_regionkey = r2.r_regionkey and r2.r_regionkey = r3.r_regionkey;", /* 5 */
	"select * from region r1, region r2, region r3, region r4 where r1.r_regionkey = r2.r_regionkey and r2.r_regionkey = r3.r_regionkey and r3.r_regionkey = r4.r_regionkey;",	/* 6 */
	"select * from region r, nation n, supplier s where r_regionkey = n_regionkey and n_nationkey = s_nationkey;",		/* 7 */
	"select * from region r1, region r2, region r3 where r1.r_regionkey = r2.r_regionkey and r2.r_regionkey = r3.r_regionkey and r3.r_regionkey = 1;",	/* 8 */
	"select * from region r1, region r2, region r3 where r1.r_regionkey = r2.r_regionkey and r2.r_regionkey = r3.r_regionkey and r1.r_regionkey = 1 and r2.r_regionkey = 1 and r3.r_regionkey = 1;",	/* 9 */
	"select count(*) from nation, region where n_regionkey = r_regionkey;",		/* 10 */
	"select * from orders, customer, nation where o_custkey = c_custkey and c_nationkey = n_nationkey and n_nationkey = 1 and o_totalprice < 1000;",	/* 11 */
	"select * from partsupp, supplier, nation where ps_suppkey = s_suppkey and s_nationkey = n_nationkey and ps_availqty < 250;",		/* 12 */
	"select * from psshort, supplier, nation where ps_suppkey = s_suppkey and s_nationkey = n_nationkey;",		/* 13 */
	"select * from psshort, supplier, nation, region where ps_suppkey = s_suppkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey;",		/* 14 */
	"select count(*) from psshort, supplier, nation, region where ps_suppkey = s_suppkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey;", /* 15 */
	"select * from psshort, supplier where ps_suppkey = s_suppkey;",
	"select * from nation, supplier, partsupp where ps_suppkey = s_suppkey and s_nationkey = n_nationkey and ps_availqty < 250;",		/* 16 */
	"select * from nation, supplier, psshort where ps_suppkey = s_suppkey and s_nationkey = n_nationkey;",		/* 17 */
	"select * from test;",		/* 18 */

	/**** TPC-H Query 5 ****/
	"select * from customer, orders, lineitem, supplier, nation, region where c_custkey = o_custkey and l_orderkey = o_orderkey and l_suppkey = s_suppkey and c_nationkey = s_nationkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey and r_name = 'ASIA' and o_orderdate >= date '1994-01-01' and o_orderdate < date '1994-01-02' and l_partkey = 15593",
	"select * from customer, orders, lineitem, supplier, nation, region where c_custkey = o_custkey and l_orderkey = o_orderkey and l_suppkey = s_suppkey and c_nationkey = s_nationkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey and r_name = 'ASIA' and o_orderdate >= date '1994-01-01' and o_orderdate < date '1995-01-01'",
	"select n_name, sum(l_extendedprice * (1 - l_discount)) as revenue from customer, orders, lineitem, supplier, nation, region where c_custkey = o_custkey and l_orderkey = o_orderkey and l_suppkey = s_suppkey and c_nationkey = s_nationkey and s_nationkey = n_nationkey and n_regionkey = r_regionkey and r_name = 'ASIA' and o_orderdate >= date '1994-01-01' and o_orderdate < date '1995-01-01' group by n_name",

	/***** TPC-H Query 8 ****/
	"select extract(year from o_orderdate) as o_year, l_extendedprice * (1 - l_discount) as volume, n2.n_name as nation from part, supplier, lineitem, orders, customer, nation n1, nation n2, region where p_partkey = l_partkey and s_suppkey = l_suppkey and l_orderkey = o_orderkey and o_custkey = c_custkey and c_nationkey = n1.n_nationkey and n1.n_regionkey = r_regionkey and r_name = 'AMERICA' and s_nationkey = n2.n_nationkey and o_orderdate between date '1995-01-01' and date '1996-12-31' and p_type = 'ECONOMY ANODIZED STEEL'",
	"explain select * from part, supplier, lineitem, orders, customer, nation n1, nation n2, region where p_partkey = l_partkey and s_suppkey = l_suppkey and l_orderkey = o_orderkey and o_custkey = c_custkey and c_nationkey = n1.n_nationkey and n1.n_regionkey = r_regionkey and r_name = 'AMERICA' and s_nationkey = n2.n_nationkey and o_orderdate between date '1995-01-01' and date '1996-12-31' and p_type = 'ECONOMY ANODIZED STEEL' and l_extendedprice < 5000",
	"select * from part, lineitem, customer, nation n1, region, orders, supplier, nation n2 where p_partkey = l_partkey and s_suppkey = l_suppkey and l_orderkey = o_orderkey and o_custkey = c_custkey and c_nationkey = n1.n_nationkey and n1.n_regionkey = r_regionkey and r_name = 'AMERICA' and s_nationkey = n2.n_nationkey and o_orderdate between date '1995-01-01' and date '1996-12-31' and p_type = 'ECONOMY ANODIZED STEEL' and l_extendedprice < 5000",

	"select * from orders, customer where c_custkey < 100 and c_custkey = o_custkey and o_orderkey < 1000000",
	"select * from lineitem, orders where o_orderkey < 100 and o_orderkey = l_orderkey",
	"explain analyze select * from orders, customer, nation where c_nationkey = n_nationkey and c_custkey = 1 and c_custkey = o_custkey",

	"select * from (partsupp join supplier on ps_suppkey = s_suppkey) join nation on s_nationkey = n_nationkey where ps_availqty < 250;",		/* 12 */
	"select * from (nation join supplier on s_nationkey = n_nationkey) join partsupp on ps_suppkey = s_suppkey where ps_availqty < 250;",		/* 12 */

	"select * from region, nation, customer where n_nationkey = c_nationkey and n_regionkey = r_regionkey and c_custkey = 1;",

	"explain analyze select * from partsupp  join  ( ( supplier  join  nation  on s_nationkey=n_nationkey)  join  region  on n_regionkey=r_regionkey) on ps_suppkey=s_suppkey where r_name='ASIA';",

	"explain analyze select * from ( supplier  join  nation  on s_nationkey=n_nationkey)  join  region  on n_regionkey=r_regionkey",

	"select n_name, r_name from region, nation where n_regionkey = r_regionkey",

	"select count(*) from customer, nation, region where c_nationkey = n_nationkey and n_regionkey = r_regionkey and c_nationkey + n_nationkey = r_regionkey",

	"select s_acctbal, s_name, n_name, p_partkey, p_mfgr, s_address, s_phone, s_comment from part, supplier, partsupp, nation where p_partkey = ps_partkey and s_suppkey = ps_suppkey and p_partkey < 30 and s_nationkey = n_nationkey and n_nationkey = 1 and ps_supplycost = ( select min(ps_supplycost) from partsupp where p_partkey = ps_partkey)",
	"select s_acctbal, s_name, n_name, p_partkey, p_mfgr, s_address, s_phone, s_comment from part, supplier, partsupp, nation where p_partkey = ps_partkey and s_suppkey = ps_suppkey and p_partkey < 30 and s_nationkey = n_nationkey and n_nationkey = 1 and ps_supplycost = (select min(ps_supplycost) from partsupp)",

	"explain analyze select count(*) from lineitem, orders where o_orderkey = l_orderkey",

	"select * from (region as r1 join region as r2 on r1.r_regionkey = r2.r_regionkey) join (region as r3 join region as r4 on r3.r_regionkey =r4.r_regionkey) on r2.r_regionkey = r3.r_regionkey",

	"select * from region, nation, customer where n_nationkey = c_nationkey and n_regionkey = r_regionkey and c_custkey = 1;",

	"select * from region, nation, customer where n_nationkey = c_nationkey and n_regionkey = r_regionkey;",

	"explain select * from region, nation where n_nationkey = r_regionkey + 1;",

    "explain analyze select * from hundredk1, onek1, onek2 where hundredk1.unique2 % 1000 = onek1.unique2 and hundredk1.unique2 % 1000 = onek2.unique2;",

	"dummy"
};

static int
compute_number_of_static_commands()
{
	int			num_commands = 0;

	while (strcmp(commands[num_commands], "dummy") != 0)
		num_commands++;

	return num_commands;
}


static int	use_command = -1;	/* first query is at 0 */
static int	use_static_command = 0;

/** First invocation -- return a predefined query; second invocation -- return EOF */
static int
StaticReadCommand(StringInfo inBuf)
{
	static int	second_invocation = 0;

	if (second_invocation == 1)
		return EOF;
	else
	{
		if (use_command == -1)
		{
			int			num_commands = compute_number_of_static_commands(),
						j;

			for (j = 0; j < num_commands; j++)
				printf("%d : %s\n", j, commands[j]);
			return EOF;
		}
		else
		{
			int			index;

			second_invocation = 1;

			Assert(0 <= use_command);
			Assert(use_command < compute_number_of_static_commands());

			for (index = 0; index < strlen(commands[use_command]); index++)
				appendStringInfoChar(inBuf, commands[use_command][index]);

			return 'Q';
		}
	}
}	/* @EdstemsAD */

/*
 * Parse a query string and pass it through the rewriter.
 *
 * A list of Query nodes is returned, since the string might contain
 * multiple queries and/or the rewriter might expand one query to several.
 *
 * NOTE: this routine is no longer used for processing interactive queries,
 * but it is still needed for parsing of SQL function bodies.
 */
List *
pg_parse_and_rewrite(char *query_string,		/* string to execute */
					 Oid *typev,	/* parameter types */
					 int nargs) /* number of parameters */
{
	List	   *raw_parsetree_list;
	List	   *querytree_list;
	List	   *list_item;
	StringInfoData stri;

	initStringInfo(&stri);
	appendStringInfo(&stri, "%s", query_string);

	/*
	 * (1) parse the request string into a list of raw parse trees.
	 */
	raw_parsetree_list = pg_parse_query(&stri, typev, nargs);

	/*
	 * (2) Do parse analysis and rule rewrite.
	 */
	querytree_list = NIL;
	foreach(list_item, raw_parsetree_list)
	{
		Node	   *parsetree = (Node *) lfirst(list_item);

		querytree_list = nconc(querytree_list,
							   pg_analyze_and_rewrite(parsetree));
	}

	return querytree_list;
}

/*
 * Do raw parsing (only).
 *
 * A list of parsetrees is returned, since there might be multiple
 * commands in the given string.
 *
 * NOTE: for interactive queries, it is important to keep this routine
 * separate from the analysis & rewrite stages.  Analysis and rewriting
 * cannot be done in an aborted transaction, since they require access to
 * database tables.  So, we rely on the raw parser to determine whether
 * we've seen a COMMIT or ABORT command; when we are in abort state, other
 * commands are not processed any further than the raw parse stage.
 */
List *
pg_parse_query(StringInfo query_string, Oid *typev, int nargs)
{
	List	   *raw_parsetree_list;

	if (Log_statement)
		elog(LOG, "query: %s", query_string->data);

	if (Show_parser_stats)
		ResetUsage();

	raw_parsetree_list = parser(query_string, typev, nargs);
	/* pprint(raw_parsetree_list); */
	if (Show_parser_stats)
		ShowUsage("PARSER STATISTICS");

	return raw_parsetree_list;
}

/*
 * Given a raw parsetree (gram.y output), perform parse analysis and
 * rule rewriting.
 *
 * A list of Query nodes is returned, since either the analyzer or the
 * rewriter might expand one query to several.
 *
 * NOTE: for reasons mentioned above, this must be separate from raw parsing.
 */
List *
pg_analyze_and_rewrite(Node *parsetree)
{
	List	   *querytree_list;
	List	   *list_item;
	Query	   *querytree;
	List	   *new_list;

	/*
	 * (1) Perform parse analysis.
	 */
	if (Show_parser_stats)
		ResetUsage();

	querytree_list = parse_analyze(parsetree, NULL);

	if (Show_parser_stats)
	{
		ShowUsage("PARSE ANALYSIS STATISTICS");
		ResetUsage();
	}

	/*
	 * (2) Rewrite the queries, as necessary
	 *
	 * rewritten queries are collected in new_list.  Note there may be more
	 * or fewer than in the original list.
	 */
	new_list = NIL;
	foreach(list_item, querytree_list)
	{
		querytree = (Query *) lfirst(list_item);

		if (Debug_print_parse)
			elog_node_display(LOG, "parse tree", querytree,
							  Debug_pretty_print);

		if (querytree->commandType == CMD_UTILITY)
		{
			/* don't rewrite utilities, just dump 'em into new_list */
			new_list = lappend(new_list, querytree);
		}
		else
		{
			/* rewrite regular queries */
			List	   *rewritten = QueryRewrite(querytree);

			new_list = nconc(new_list, rewritten);
		}
	}

	querytree_list = new_list;

	if (Show_parser_stats)
		ShowUsage("REWRITER STATISTICS");

#ifdef COPY_PARSE_PLAN_TREES

	/*
	 * Optional debugging check: pass querytree output through
	 * copyObject()
	 */
	new_list = (List *) copyObject(querytree_list);
	/* This checks both copyObject() and the equal() routines... */
	if (!equal(new_list, querytree_list))
		elog(WARNING, "pg_analyze_and_rewrite: copyObject failed on parse tree");
	else
		querytree_list = new_list;
#endif

	if (Debug_print_rewritten)
		elog_node_display(LOG, "rewritten parse tree", querytree_list,
						  Debug_pretty_print);

	return querytree_list;
}


/* Generate a plan for a single query. */
Plan *
pg_plan_query(Query *querytree, MemoryContext cxt)
{
	/*
	 * tcqoc: added the memory context argument to this function so we
	 * could be sure the plan was created in the right context. calling
	 * the function with a context of NULL preserves old behavior
	 */

	Plan	   *plan;
	MemoryContext oldctx = CurrentMemoryContext;
	bool		needToSwitchBack = false;

	/* Utility commands have no plans. */
	if (querytree->commandType == CMD_UTILITY)
		return NULL;

	if (cxt == NULL)
		cxt = CurrentMemoryContext;
	if (Show_planner_stats)
		ResetUsage();


	if (cxt != CurrentMemoryContext)
	{
		MemoryContextSwitchTo(cxt);
		needToSwitchBack = true;
	}
	/* call that optimizer */
	plan = planner(querytree);

	if (needToSwitchBack)
		MemoryContextSwitchTo(oldctx);

	if (Show_planner_stats)
		ShowUsage("PLANNER STATISTICS");

#ifdef COPY_PARSE_PLAN_TREES
	/* Optional debugging check: pass plan output through copyObject() */
	{
		Plan	   *new_plan = (Plan *) copyObject(plan);

		/*
		 * equal() currently does not have routines to compare Plan nodes,
		 * so don't try to test equality here.  Perhaps fix someday?
		 */
#ifdef NOT_USED
		/* This checks both copyObject() and the equal() routines... */
		if (!equal(new_plan, plan))
			elog(WARNING, "pg_plan_query: copyObject failed on plan tree");
		else
#endif
			plan = new_plan;
	}
#endif

	/*
	 * Print plan if debugging.
	 */
	if (Debug_print_plan)
		elog_node_display(LOG, "plan", plan, Debug_pretty_print);

	return plan;
}



/* ----------------------------------------------------------------
 *		pg_exec_query_string()
 *
 *		Takes a querystring, runs the parser/utilities or
 *		parser/planner/executor over it as necessary.
 *
 * Assumptions:
 *
 * At call, we are not inside a transaction command.
 *
 * The CurrentMemoryContext after starting a transaction command must be
 * appropriate for execution of individual queries (typically this will be
 * TransactionCommandContext).	Note that this routine resets that context
 * after each individual query, so don't store anything there that
 * must outlive the call!
 *
 * parse_context references a context suitable for holding the
 * parse/rewrite trees (typically this will be QueryContext).
 * This context *must* be longer-lived than the transaction context!
 * In fact, if the query string might contain BEGIN/COMMIT commands,
 * parse_context had better outlive TopTransactionContext!
 *
 * We could have hard-wired knowledge about QueryContext and
 * TransactionCommandContext into this routine, but it seems better
 * not to, in case callers from outside this module need to use some
 * other contexts.
 *
 * ----------------------------------------------------------------
 */

void
pg_exec_query_string(StringInfo query_string,	/* string to execute */
					 CommandDest dest,	/* where results should go */
					 MemoryContext parse_context)		/* context for
														 * parsetrees */
{
  List * pqe_list=NIL;
  bool            isRecursive=false;   /*@tcqrecurse*/
	bool		xact_started;
	MemoryContext oldcontext;
	List	   *parsetree_list,
			   *parsetree_item;
	struct timeval start_t,
				stop_t;
	bool		save_Log_duration = Log_duration;

	debug_query_string = query_string->data;

	elog(DEBUG1, "pg_exec_query_string(): Query is '%s'", debug_query_string);
	
	/*
	 * We use save_Log_duration so "SET Log_duration = true" doesn't
	 * report incorrect time because gettimeofday() wasn't called.
	 */
	if (save_Log_duration)
		gettimeofday(&start_t, NULL);

	/*
	 * Start up a transaction command.	All queries generated by the
	 * query_string will be in this same command block, *unless* we find a
	 * BEGIN/COMMIT/ABORT statement; we have to force a new xact command
	 * after one of those, else bad things will happen in xact.c. (Note
	 * that this will possibly change current memory context.)
	 */
	start_xact_command();
	xact_started = true;

	/*
	 * Telegraph CQ Notes pg_exec_query_string has been called by the
	 * frontend and is about to parse a query in shared memory. This query
	 * might be a select in which case it will be handled by the TCQ
	 * backend, or it might be a utility command, in which case it is
	 * handled by the frontend.  Before we parse the query, we don't know
	 * which case we are in, so the query is parsed in shared memory.
	 * Since it might be a utility command, the frontend might need to
	 * free the context.
	 *
	 * NOTE: continuous queries are handled using cursors. 1. declare cursor
	 * creates a plan that is a SELECT statement. so that plan gets sent
	 * to the backend.	The frontend scanqueue plan gets turned into a
	 * portal in the frontend and is accessed by subsequent fetch
	 * statements.
	 *
	 * 2. A fetch statement is handled in the frontend, and looks up the
	 * portal plan and does some operations on it.
	 */


	/*
	 * parse_context *must* be different from the execution memory
	 * context, else the context reset at the bottom of the loop will
	 * destroy the parsetree list.	(We really ought to check that
	 * parse_context isn't a child of CurrentMemoryContext either, but
	 * that would take more cycles than it's likely to be worth.)
	 */
	Assert(parse_context != CurrentMemoryContext);


	/*
	 * Do basic parsing of the query or queries (this should be safe even
	 * if we are in aborted transaction state!)
	 */
	parse_query_in_parse_context(&oldcontext, parse_context, &parsetree_list, query_string);

	if (MyProcessType == PROC_TCQ_FRONTEND)		/* @BdtcqOC */
	{
		/* although multiple queries may be passed in  */
		/* a batch to this function in normal postgres, */
		/* unfortunately this doesn't work for us right  */
		/* now because all of the query trees are produced	*/
		/* in the same shared memory context that is blown	*/
		/* away by the backend after the first query.	*/
 		/* for now we create our own lists, and limit  */
		/* consideration to the first query in the batch, but */
		/* we may want to revisit this.  tcqoc	*/
	  List	   *newtreelist;
	  shallow_copy_parse_tree_in_query_context(&parsetree_list, &newtreelist);
	}

	/*
	 * Run through the parsetree(s) and process each one.
	 */
	foreach(parsetree_item, parsetree_list)
	{
		Node	   *parsetree = (Node *) lfirst(parsetree_item);
		const char *commandTag;
		char		completionTag[COMPLETION_TAG_BUFSIZE];
		CmdType		origCmdType;
		bool		foundOriginalQuery = false;
		List	   *querytree_list,
				   *querytree_item;

		/*
		 * First we set the command-completion tag to the main query (as
		 * opposed to each of the others that may be generated by analyze
		 * and rewrite).  Also set ps_status and do any special
		 * start-of-SQL-command processing needed by the destination.
		 */
		commandTag = CreateCommandTag(parsetree);

		switch (nodeTag(parsetree))
		{
			case T_InsertStmt:
				origCmdType = CMD_INSERT;
				break;
			case T_DeleteStmt:
				origCmdType = CMD_DELETE;
				break;
			case T_UpdateStmt:
				origCmdType = CMD_UPDATE;
				break;
			case T_SelectStmt:
				origCmdType = CMD_SELECT;
				break;
			default:
				/* Otherwise, never match commandType */
				origCmdType = CMD_UNKNOWN;
				break;
		}

		set_ps_display(commandTag);

		BeginCommand(commandTag, dest);

		/*
		 * If we are in an aborted transaction, ignore all commands except
		 * COMMIT/ABORT.  It is important that this test occur before we
		 * try to do parse analysis, rewrite, or planning, since all those
		 * phases try to do database accesses, which may fail in abort
		 * state. (It might be safe to allow some additional utility
		 * commands in this state, but not many...)
		 */
		if (IsAbortedTransactionBlockState())
		{
			bool		allowit = false;

			if (IsA(parsetree, TransactionStmt))
			{
				TransactionStmt *stmt = (TransactionStmt *) parsetree;

				if (stmt->command == COMMIT || stmt->command == ROLLBACK)
					allowit = true;
			}

			if (!allowit)
				elog(ERROR, "current transaction is aborted, "
					 "queries ignored until end of transaction block");
		}

		/* Make sure we are in a transaction command */
		if (!xact_started)
		{
			start_xact_command();
			xact_started = true;
		}

		/* If we got a cancel signal in parsing or prior command, quit */
		CHECK_FOR_INTERRUPTS();

		/*
		 * OK to analyze and rewrite this query.
		 *
		 * Switch to appropriate context for constructing querytrees (again,
		 * these must outlive the execution context).
		 */
		analyze_and_rewrite_in_parse_context(parse_context, &parsetree, &querytree_list, &isRecursive);

		if (MyProcessType == PROC_TCQ_FRONTEND) /* @BdtcqOC */
		{
			/* see comment at first occurence of this block */
		  List * newtreelist=NIL;
		  shallow_copy_querytree_in_query_context(&querytree_list, &newtreelist, &isRecursive);

		}					/* @EdtcqOC */
 			
		/*
		 * Inner loop handles the individual queries generated from a
		 * single parsetree by analysis and rewrite.
		 */
		foreach(querytree_item, querytree_list)
		{
			Query	   *querytree = (Query *) lfirst(querytree_item);
			bool		endTransactionBlock = false;
			bool		canSetTag;
			bool isLast = (lnext(querytree_item) == NIL);
			/* Make sure we are in a transaction command */
			if (!xact_started)
			{
				start_xact_command();
				xact_started = true;
			}

			/*
			 * If we got a cancel signal in analysis or prior command,
			 * quit
			 */
			CHECK_FOR_INTERRUPTS();

			/* Sanity checks for event queries - TCQ SR */

			if(querytree->eventClause && querytree->groupClause)
			  elog(ERROR, "Grouping with event processing is not built-in (or defined!)");
	
			if(querytree->eventClause && querytree->hasAggs)
			  elog(ERROR, "Currently, aggregations with event processing are not built-in (or defined!)");

			if(querytree->eventClause && ((FromExpr *)querytree->jointree)->quals)
			  elog(ERROR, "Currently, WITH clause in event processing queries is not supported \nPerhaps you can achieve the same effect by using another query on top of this one?");

			


			/* Sanity check against unwindowed aggregates - TCQ SR */
			if(querytree->isContinuousQuery && querytree->hasAggs && !querytree->windowClause->windowExprs)
			  elog(ERROR, "Aggregates in continuous queries should be windowed");
			  


			/*
			 * This query can set the completion tag if it is the original
			 * query, or if it is an INSTEAD query of the same kind as the
			 * original and we haven't yet seen the original query.
			 */
			if (querytree->querySource == QSRC_ORIGINAL)
			{
				canSetTag = true;
				foundOriginalQuery = true;
			}
			else if (!foundOriginalQuery &&
					 querytree->commandType == origCmdType &&
					 (querytree->querySource == QSRC_INSTEAD_RULE ||
					  querytree->querySource == QSRC_QUAL_INSTEAD_RULE))
				canSetTag = true;
			else
				canSetTag = false;

			/** If we are using the eddy, rearrange the rangetable and the Query if required -- Amol */
			if (enable_eddy || UseSteMHashJoins)
				readjust_query_rangetable(querytree);

			if (querytree->commandType == CMD_UTILITY)
			{
				/*
				 * process utility functions (create, destroy, etc..)
				 */
				Node	   *utilityStmt = querytree->utilityStmt;

				elog(DEBUG2, "ProcessUtility");

				/*
				 * Set snapshot if utility stmt needs one.	Most reliable
				 * way to do this seems to be to enumerate those that do
				 * not need one; this is a short list.	Transaction
				 * control, LOCK, and SET must *not* set a snapshot since
				 * they need to be executable at the start of a
				 * serializable transaction without freezing a snapshot.
				 * By extension we allow SHOW not to set a snapshot.  The
				 * other stmts listed are just efficiency hacks.  Beware
				 * of listing anything that can modify the database ---
				 * if, say, it has to update a functional index, then it
				 * had better have a snapshot.
				 */
				if (!(IsA(utilityStmt, TransactionStmt) ||
					  IsA(utilityStmt, LockStmt) ||
					  IsA(utilityStmt, VariableSetStmt) ||
					  IsA(utilityStmt, VariableShowStmt) ||
					  IsA(utilityStmt, VariableResetStmt) ||
					  IsA(utilityStmt, ConstraintsSetStmt) ||
				/* efficiency hacks from here down */
					  IsA(utilityStmt, FetchStmt) ||
					  IsA(utilityStmt, ListenStmt) ||
					  IsA(utilityStmt, NotifyStmt) ||
					  IsA(utilityStmt, UnlistenStmt) ||
					  IsA(utilityStmt, CheckPointStmt)))
					SetQuerySnapshot();

				/* end transaction block if transaction or variable stmt */
				if (IsA(utilityStmt, TransactionStmt) ||
					IsA(utilityStmt, VariableSetStmt) ||
					IsA(utilityStmt, VariableShowStmt) ||
					IsA(utilityStmt, VariableResetStmt))
					endTransactionBlock = true;

				if (canSetTag)
				{
					/* utility statement can override default tag string */
					ProcessUtility(utilityStmt, dest, completionTag);
					if (completionTag[0])
						commandTag = completionTag;
				}
				else
				{
					/* utility added by rewrite cannot override tag */
					ProcessUtility(utilityStmt, dest, NULL);
				}
			}
			else
			{
				/*
				 * process a plannable query.
				 */
				Plan	   *plan;
				uint16		qno;

				if ((UseTelegraphCQ && querytree->isContinuousQuery) || (enable_eddy))
					setTCQJoinOptions();
				SetQuerySnapshot();

				/* Create a single query plan*/
				plan = pg_plan_query(querytree, (UseTelegraphCQ ? parse_context : NULL));


				/* Put in the Event node here. There is perhaps a better place, like inside the planner,
				 but event processing is so not-postgresql, so I am keeping it here - TCQ SR */
				
				plan->eventClause = querytree->eventClause;


				if ((UseTelegraphCQ && querytree->isContinuousQuery) || (enable_eddy))
					resetJoinOptions();
				/* Can't have both on at the same time. */
				Assert((!UseSteMHashJoins) || (!enable_eddy));

				if (UseHashGroup)
				{
					convert_to_fsched_dup_in_parse_context(parse_context, &plan, &querytree);

				}
				else if (enable_eddy)
				{
					if (!UseFastPathEddyCreate)
					{
					  convert_plan_to_eddy_in_parse_context(parse_context, &plan, &querytree);

					}

					/* pprint(plan); */

					/*
					 * ((Eddy *) plan)->rangetable = querytree->rtable; --
					 * moved inside convert
					 */
				}
				else if (UseCQEddy)
				{
					elog(DEBUG1, "The system has been started in "
					   "TelegraphCQ mode, and the databases specified "
					   "for the TCQ frontend process and TCQ backend "
					   "tcqbackenddb=%s, tcqfrontenddb=%s",
					   telegraphcqdb, DatabaseName);
					seqscan_to_eddy_in_parse_context_usecqeddy(parse_context, &plan, &querytree);

				}
				else if (RunSQinFE) 
				{
					bool isBlocking = true;
					
					elog(DEBUG1, "frontend running SQ's, changing scans to non-blocking");
					isBlocking = false;
										
					seqscan_to_eddy_in_parse_context_runsqinfe (parse_context, &plan, &querytree, &isBlocking);

				}
				else if (MyProcessType == PROC_TCQ_FRONTEND)
				{
					seqscan_to_eddy_in_parse_context_tcqfe (parse_context,&plan, &querytree);

				}
				/*
				 * need to skip all this FE/BE stuff if we're running the
				 * entire streaming plan in the FE.  Added the first part of
				 * this predicate to skip straight down to the query processing
				 * if we're running everything in the front end
				 *
				 * -garrett
				 */
				if ((!RunSQinFE) && (MyProcessType == PROC_TCQ_FRONTEND &&		  /* @BdtcqMS */
									 querytree->isContinuousQuery))		
				{

					planqentry	pqe;
					ScanQueue  *scanq;
					Plan	   *bePlan,
							   *fePlan = NULL,
							   *p,
							   *lastPlan = NULL,
							   *aggPlan = NULL;

					shmQueryContext = NULL;		/* plan goes to backend */
					/* pprint(plan); */
					if (UseCQEddy)
					{
						/* @BwndaggWH */
                        /* We used to leave the Agg, Sort, Unique and Group nodes in 
						   the frontend.  Now we put them into the FAgg node in the backend
                           instead.  We still call that chunk of the plan "fePlan", though.
                         */
						for (bePlan = plan;
							 IsA(bePlan, Agg) ||
                             IsA(bePlan, Sort) ||
							 IsA(bePlan, Unique) ||
                             IsA(bePlan, Group) ||
                             IsA(bePlan, Limit)
                             ;
						     bePlan = outerPlan(bePlan));
						 

							/* @EwndaggWH */
					
					}
					else
					  {
					    bePlan = plan;
					  }
					if (!IsA(bePlan, Eddy) &&
						!IsA(bePlan, FSched))
					{
						elog(WARNING, "Sending a non-Eddy and non-FSched CQ plan to the backend!");
						elog(WARNING, "Assuming you know what you are doing.");
						/*pprint(bePlan);*/
					}
					if (IsA(bePlan, Eddy))
					{
						/*elog(LOG, "Printing Eddy..");*/
						/* pprint(bePlan); */
						List	   *queryrt;
						List	   *queryrtiter;
						RangeTblEntry *queryrte;
						bool		ret;
						assign_beplan_rt_in_parse_context (parse_context, &bePlan, &querytree, &ret);

						if (!ret)
						  {
						    if(!(isRecursive))
						      {
						      
							elog(ERROR, "required bindings are not satisfied");
						      }
						    else 
						      {
							elog(NOTICE, "warning: binding satisfiability check suspended for recursive query.  CHECK the query for problems");
						  }
						  }
						/*
						 * now traverse the beplan's rangetable and be
						 * sure that this is not a self join
						 */
						/*
						 * But don't complain if this is an event query
						 * This is temporary, while I figure out whether to have single or
						 * multiple range table entries for streams that are referenced multiple 
						 * times in an event query
						 *                            - TCQ SR
						 */
						
						queryrt = ((Eddy *) bePlan)->rangetable;

						if(!bePlan->eventClause) 
						  foreach(queryrtiter, queryrt)
						  {
						    List	   *restqueryrt = lnext(queryrtiter);
						    List	   *restqueryrtiter;
						    RangeTblEntry *restqueryrte;
						    
						    queryrte = (RangeTblEntry *) lfirst(queryrtiter);
						    foreach(restqueryrtiter, restqueryrt)
						      {
							restqueryrte = (RangeTblEntry *) lfirst(restqueryrtiter);
							if (restqueryrte->relid == queryrte->relid)
							  elog(ERROR, "SELF JOINS NOT ALLOWED");
						      }
						    
						  }
					
					}
					if(!isLast)
					{
						pqe.mctx = NULL;
					}
					else
					{
						pqe.mctx = parse_context;
					}
					pqe.rangetable = NULL;
					pqe.querytree = querytree;
					pqe.plan = bePlan;
					pqe.queueNo = qno = AllocQueue();

					append_querystr_to_pqe_in_parse_context(&oldcontext, parse_context, &pqe, &query_string);
					/*
					 * OK. we are no longer allocating in shared memory
					 * context as the backend owns that.  further we must
					 * do all access to the shared memory plan pointer
					 * *before* the enqueue, otherwise it might go away
					 * because enqueue hands off ownership of the queries
					 * context to the back end.
					 */
					MemoryContextSwitchTo(oldcontext);
					/* generate ScanQueue plan for the frontend */
					scanq = (ScanQueue *) makeNode(ScanQueue);
					scanq->plan.targetlist = copyObject(bePlan->targetlist);

					/*
					 * TBD TBD - sailesh/owenc: check the following 2nd
					 * arg to ExecTypeFromTL
					 */
					scanq->tupDesc = ExecTypeFromTL(bePlan->targetlist, true);
					scanq->serializer = &result_serializer;
					scanq->dontProject = true;

                    if (tcq_show_punct_in_output) {
                        elog(LOG, "Showing punctuation in query output.");
                    } else {
                        elog(LOG, "NOT showing punctuation in query output.");
                    }
                    /*scanq->ignorePunct = !tcq_show_punct_in_output;*/
					scanq->queue = GetQueue(qno);
					scanq->qno = qno;	/* ExecEndScanq will free */
					if (UseCQEddy)
					{
						/* @BwndaggWH */
						/* collect top nodes to be executed in frontend */
						/* then feed the ScanQueue node to them */
						for (p = plan;
							 IsA(p, Agg) ||
                             IsA(p, Group) ||
							 IsA(p, Unique) ||
                             IsA(p, Sort) ||
                             IsA(p, Limit);
							 p = outerPlan(p))
						{
							Plan	   *savedPlan = outerPlan(p);

							outerPlan(p) = NULL;		/* avoid deep copy */
							if (lastPlan == NULL)
								lastPlan = copyObject(p);
							else
							{
								outerPlan(lastPlan) = copyObject(p);
								lastPlan = outerPlan(lastPlan);
							}
							outerPlan(p) = savedPlan;
							if (fePlan == NULL)
								fePlan = lastPlan;
							if (IsA(lastPlan, Agg))
								aggPlan = lastPlan;
						}
					}
					else
						fePlan = NULL;

					if(fePlan != NULL){
					  List * wl=NIL;
					  //create FAgg node to move it from the frontend to the backend
					  create_FEagg_node_in_parse_context (parse_context, &fePlan, &wl, &querytree, &bePlan);

					}
					

					if (fePlan == NULL)
					  {
						plan = (Plan *) scanq;
						plan->targetlist=copyObject((aggPlan == NULL) ? bePlan->targetlist :
										  aggPlan->targetlist);
						scanq->tupDesc = ExecTypeFromTL(plan->targetlist, true);
			
					  }
					else
					  plan = fePlan;
					Assert(plan);

					/*
					 * while we are using the postgres executor, the
					 * ownership of shmem query tree goes to the backend
					 * so we need to make a copy here tcqoc
					 */
					querytree = (Query *) copyObject(querytree);

					/*
					 * Initialize snapshot state for query.  This has to
					 * be done before running the planner, because it
					 * might try to evaluate immutable or stable
					 * functions, which in turn might run queries.
					 */
					SetQuerySnapshot();


					/*
					 * this may be a DECLARE CURSOR command, in which case
					 * a portal will be created for the frontend plan, but
					 * not the backend plan.
					 */
					pqe.querytree->isPortal = false;
					pqe.querytree->into = NULL;
					cqcancelid = pqe.queueNo;
					pqe.isRecursive = isRecursive;
					pqe.isLast = isLast;
					if(isRecursive && !isLast)
					  {
					    add_pqe_to_pqe_list_in_parse_context (parse_context, &pqe, &pqe_list);

					  }
					else 
					  {
					    pqe.pqe_list = pqe_list;
					    /*
					     * Plan enqueued on the BE. DO NOT access to the parse_context anymore!
					     */
					    enqueue(plan_queue, (char *) &pqe, true, &planqentry_serializer);
					  }
				}

				/* if we got a cancel signal whilst planning, quit */
				CHECK_FOR_INTERRUPTS();

				/*
				 * execute the plan
				 */
				if (Show_executor_stats)
					ResetUsage();

				if (dontExecute)
				{
					/* don't execute it, just show the query plan */
					print_plan(plan, querytree);
				}
				else
				{
					elog(DEBUG2, "ProcessQuery");

					if (canSetTag)
					{
						/* statement can override default tag string */
						//debug_log_message("query execution start");
						if ((isRecursive && isLast) ||
						    !isRecursive)
						  {
						    
						    ProcessQuery(querytree,
								 plan, dest, completionTag);
						  }					

						//debug_log_message("query execution end");
						commandTag = completionTag;
						//debug_print_times();
					}
					else
					{
						if ((isRecursive && isLast) || !isRecursive)
						{
							ProcessQuery(querytree,
										 plan, dest, NULL);
							
						}

					}
				}
				/* @BdtcqWH */


				if (Show_executor_stats)
					ShowUsage("EXECUTOR STATISTICS");
			}

			/*
			 * In a query block, we want to increment the command counter
			 * between queries so that the effects of early queries are
			 * visible to subsequent ones.	In particular we'd better do
			 * so before checking constraints.
			 */
			if (!endTransactionBlock)
				CommandCounterIncrement();

			/*
			 * Clear the execution context to recover temporary memory
			 * used by the query.  NOTE: if query string contains
			 * BEGIN/COMMIT transaction commands, execution context may
			 * now be different from what we were originally passed; so be
			 * careful to clear current context not "oldcontext".
			 */
			Assert(parse_context != CurrentMemoryContext);

			MemoryContextResetAndDeleteChildren(CurrentMemoryContext);

			/*
			 * If this was a transaction control statement or a variable
			 * set/show/reset statement, commit it and arrange to start a
			 * new xact command for the next command (if any).
			 */
			if (endTransactionBlock)
			{
				finish_xact_command(true);
				xact_started = false;
			}
		}						/* end loop over queries generated from a
								 * parsetree */



		/*
		 * If this is the last parsetree of the query string, close down
		 * transaction statement before reporting command-complete.  This
		 * is so that any end-of-transaction errors are reported before
		 * the command-complete message is issued, to avoid confusing
		 * clients who will expect either a command-complete message or an
		 * error, not one and then the other.  But for compatibility with
		 * historical Postgres behavior, we do not force a transaction
		 * boundary between queries appearing in a single query string.
		 */
		if (lnext(parsetree_item) == NIL && xact_started)
		{
			finish_xact_command(false);
			xact_started = false;
		}

		/*
		 * It is possible that the original query was removed due to a DO
		 * INSTEAD rewrite rule.  If so, and if we found no INSTEAD query
		 * matching the command type, we will still have the default
		 * completion tag.	This is fine for most purposes, but it may
		 * confuse clients if it's INSERT/UPDATE/DELETE. Clients expect
		 * those tags to have counts after them (cf. ProcessQuery).
		 */
		if (!foundOriginalQuery)
		{
			if (strcmp(commandTag, "INSERT") == 0)
				commandTag = "INSERT 0 0";
			else if (strcmp(commandTag, "UPDATE") == 0)
				commandTag = "UPDATE 0";
			else if (strcmp(commandTag, "DELETE") == 0)
				commandTag = "DELETE 0";
		}

		/*
		 * Tell client that we're done with this query.  Note we emit
		 * exactly one EndCommand report for each raw parsetree, thus one
		 * for each SQL command the client sent, regardless of rewriting.
		 * (But a command aborted by error will not send an EndCommand
		 * report at all.)
		 */
		EndCommand(commandTag, dest);
		if (MyProcessType == PROC_TCQ_FRONTEND &&
			shmQueryContext == NULL)
		{
			/* only execute one query in the batch in  */
			/* continuous query mode */
			break;
		}
	}							/* end loop over parsetrees */


	/*
	 * Close down transaction statement, if one is open. (Note that this
	 * will only happen if the querystring was empty.)
	 */
	if (xact_started)
		finish_xact_command(false);

	if (save_Log_duration)
	{
		gettimeofday(&stop_t, NULL);
		if (stop_t.tv_usec < start_t.tv_usec)
		{
			stop_t.tv_sec--;
			stop_t.tv_usec += 1000000;
		}
		elog(LOG, "duration: %ld.%06ld sec",
			 (long) (stop_t.tv_sec - start_t.tv_sec),
			 (long) (stop_t.tv_usec - start_t.tv_usec));
	}

	debug_query_string = NULL;
}

/*
 * Convenience routines for starting/committing a single command.
 */
static void
start_xact_command(void)
{
	elog(DEBUG1, "StartTransactionCommand");
	StartTransactionCommand(false);

	/* Set statement timeout running, if any */
	if (StatementTimeout > 0)
		enable_sig_alarm(StatementTimeout, true);
}

static void
finish_xact_command(bool forceCommit)
{
	/* Invoke IMMEDIATE constraint triggers */
	DeferredTriggerEndQuery();

	/* Cancel any active statement timeout before committing */
	disable_sig_alarm(true);

	/* Now commit the command */
	elog(DEBUG1, "CommitTransactionCommand");

	CommitTransactionCommand(forceCommit);

#ifdef SHOW_MEMORY_STATS
	/* Print mem stats at each commit for leak tracking */
	if (ShowStats)
		MemoryContextStats(TopMemoryContext);
#endif
}

/* --------------------------------
 *		signal handler routines used in PostgresMain()
 * --------------------------------
 */

/*
 * quickdie() occurs when signalled SIGQUIT by the postmaster.
 *
 * Some backend has bought the farm,
 * so we need to stop what we're doing and exit.
 */
void
quickdie(SIGNAL_ARGS)
{
	PG_SETMASK(&BlockSig);
	elog(WARNING, "Message from PostgreSQL backend:"
		 "\n\tThe Postmaster has informed me that some other backend"
		 "\n\tdied abnormally and possibly corrupted shared memory."
		 "\n\tI have rolled back the current transaction and am"
	   "\n\tgoing to terminate your database system connection and exit."
	"\n\tPlease reconnect to the database system and repeat your query.");

	/*
	 * DO NOT proc_exit() -- we're here because shared memory may be
	 * corrupted, so we don't want to try to clean up our transaction.
	 * Just nail the windows shut and get out of town.
	 *
	 * Note we do exit(1) not exit(0).	This is to force the postmaster into
	 * a system reset cycle if some idiot DBA sends a manual SIGQUIT to a
	 * random backend.	This is necessary precisely because we don't clean
	 * up our shared memory state.
	 */

	exit(1);
}

/*
 * Shutdown signal from postmaster: abort transaction and exit
 * at soonest convenient time
 */
void
die(SIGNAL_ARGS)
{
	int			save_errno = errno;

	elog(DEBUG2, "Die caught proctype %d", MyProcessType);

	/* Don't joggle the elbow of proc_exit */
	if (!proc_exit_inprogress)
	{
		InterruptPending = true;
		ProcDiePending = true;

		/*
		 * If it's safe to interrupt, and we're waiting for input or a
		 * lock, service the interrupt immediately
		 */
		if (ImmediateInterruptOK && InterruptHoldoffCount == 0 &&
			CritSectionCount == 0)
		{
			/* bump holdoff count to make ProcessInterrupts() a no-op */
			/* until we are done getting ready for it */
			InterruptHoldoffCount++;
			DisableNotifyInterrupt();
			/* Make sure CheckDeadLock won't run while shutting down... */
			LockWaitCancel();
			InterruptHoldoffCount--;
			ProcessInterrupts();
		}
	}

	errno = save_errno;
}

/*
 * Timeout or shutdown signal from postmaster during client authentication.
 * Simply exit(0).
 *
 * XXX: possible future improvement: try to send a message indicating
 * why we are disconnecting.  Problem is to be sure we don't block while
 * doing so, nor mess up the authentication message exchange.
 */
void
authdie(SIGNAL_ARGS)
{
	exit(0);
}

/*
 * Query-cancel signal from postmaster: abort current transaction
 * at soonest convenient time
 */
static void
StatementCancelHandler(SIGNAL_ARGS)
{
	int			save_errno = errno;

	/*
	 * Don't joggle the elbow of proc_exit, nor an already-in-progress
	 * abort
	 */
	if (!proc_exit_inprogress && !InError)
	{
		InterruptPending = true;
		QueryCancelPending = true;

		/*
		 * If it's safe to interrupt, and we're waiting for a lock,
		 * service the interrupt immediately.  No point in interrupting if
		 * we're waiting for input, however.
		 */
		if (ImmediateInterruptOK && InterruptHoldoffCount == 0 &&
			CritSectionCount == 0)
		{
			/* bump holdoff count to make ProcessInterrupts() a no-op */
			/* until we are done getting ready for it */
			InterruptHoldoffCount++;
			if (LockWaitCancel())
			{
				DisableNotifyInterrupt();
				InterruptHoldoffCount--;
				ProcessInterrupts();
			}
			else
				InterruptHoldoffCount--;
		}
	}
	elog(DEBUG1, "QueryCancelHandler called\n");
	errno = save_errno;
}

/* signal handler for floating point exception */
static void
FloatExceptionHandler(SIGNAL_ARGS)
{
	elog(ERROR, "floating point exception!"
		 " The last floating point operation either exceeded legal ranges"
		 " or was a divide by zero");
}

/* SIGHUP: set flag to re-read config file at next convenient time */
static void
SigHupHandler(SIGNAL_ARGS)
{
	got_SIGHUP = true;
}


/*
 * ProcessInterrupts: out-of-line portion of CHECK_FOR_INTERRUPTS() macro
 *
 * If an interrupt condition is pending, and it's safe to service it,
 * then clear the flag and accept the interrupt.  Called only when
 * InterruptPending is true.
 */
void
ProcessInterrupts(void)
{
	/* OK to accept interrupt now? */
	if (InterruptHoldoffCount != 0 || CritSectionCount != 0)
		return;
	InterruptPending = false;
	if (ProcDiePending)
	{
		ProcDiePending = false;
		QueryCancelPending = false;		/* ProcDie trumps QueryCancel */
		ImmediateInterruptOK = false;	/* not idle anymore */
		DisableNotifyInterrupt();
		elog(FATAL, "This connection has been terminated by the administrator.");
	}
	if (QueryCancelPending)
	{
		QueryCancelPending = false;
		ImmediateInterruptOK = false;	/* not idle anymore */
		DisableNotifyInterrupt();
		elog(ERROR, "Query was cancelled.");
	}
	/* If we get here, do nothing (probably, QueryCancelPending was reset) */
}


static void
usage(char *progname)
{
	printf("%s is the PostgreSQL stand-alone backend.  It is not\nintended to be used by normal users.\n\n", progname);

	printf("Usage:\n  %s [OPTION]... [DBNAME]\n\n", progname);
	printf("Options:\n");
#ifdef USE_ASSERT_CHECKING
	printf("  -A 1|0          enable/disable run-time assert checking\n");
#endif
	printf("  -B NBUFFERS     number of shared buffers (default %d)\n", DEF_NBUFFERS);
	printf("  -c NAME=VALUE   set run-time parameter\n");
	printf("  -d 0-5          debugging level (0 is off)\n");
	printf("  -D DATADIR      database directory\n");
	printf("  -e              use European date format\n");
	printf("  -E              echo query before execution\n");
	printf("  -F              turn fsync off\n");
	printf("  -N              do not use newline as interactive query delimiter\n");
	printf("  -o FILENAME     send stdout and stderr to given file\n");
	printf("  -P              disable system indexes\n");
	printf("  -s              show statistics after each query\n");
	printf("  -S SORT-MEM     set amount of memory for sorts (in kbytes)\n");
	printf("  --help          show this help, then exit\n");
	printf("  --version       output version information, then exit\n");
	printf("\nDeveloper options:\n");
	printf("  -f s|i|n|m|h    forbid use of some plan types\n");
	printf("  -i              do not execute queries\n");
	printf("  -O              allow system table structure changes\n");
	printf("  -t pa|pl|ex     show timings after each query\n");
	printf("  -W NUM          wait NUM seconds to allow attach from a debugger\n");
	printf("\nReport bugs to <pgsql-bugs@postgresql.org>.\n");
}

void		restore_working_directory(int code, char *pathname);		/* @BdstemsAD */

/*
 * A function to change the working directory.
 */
void
restore_working_directory(int code, char *pathname)
{
	if (pathname != NULL)
		chdir(pathname);
}	/* @EdstemsAD */


/* ----------------------------------------------------------------
 * PostgresMain
 *	   postgres main loop -- all backends, interactive or otherwise start here
 *
 * argc/argv are the command line arguments to be used.  (When being forked
 * by the postmaster, these are not the original argv array of the process.)
 * username is the (possibly authenticated) PostgreSQL user name to be used
 * for the session.
 * ----------------------------------------------------------------
 */
int
PostgresMain(int argc, char *argv[], const char *username)		/* @dtcqSK */
{
	int			flag;

	const char *DBName = NULL;
	bool		secure;
	int			errs = 0;
	int			debug_flag = 0;
	GucContext	ctx;
	GucSource	gucsource;
	char	   *tmp;

	int			firstchar;
	StringInfo	parser_input;

	char	   *potential_DataDir = NULL;
	MemoryContext compileMemoryContext; /* @dtcqSK */


	/*
	 * //@BdstemsAD To make gprof work more reasonably, we will store the
	 * current working directory around and restore it before exiting.
	 */
	char		current_directory[100]; /* @EdstemsAD */

	getcwd(current_directory, 100);		/* @dstemsAD */

	/*
	 * Set a callback handle for execution on exit to change to the
	 * originla working directory.
	 */
	on_proc_exit(restore_working_directory, (Datum) current_directory); /* @dstemsAD */

	if (MyProcessType == PROC_TCQ_BACKEND)
	{
		elog(DEBUG1, "Entering PostgresMain in the context of TelegraphCQ Backend");
	}							/* @EdtcqSK */

	/*
	 * Catch standard options before doing much else.  This even works on
	 * systems without getopt_long.
	 */
	if (!IsUnderPostmaster && argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage(argv[0]);
			exit(0);
		}
		if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0)
		{
			puts("postgres (PostgreSQL) " PG_VERSION);
			exit(0);
		}
	}

	/*
	 * initialize globals (already done if under postmaster, but not if
	 * standalone; cheap enough to do over)
	 */

	MyProcPid = getpid();

	/*
	 * Fire up essential subsystems: error and memory management
	 *
	 * If we are running under the postmaster, this is done already.
	 */
	if (!IsUnderPostmaster)
		MemoryContextInit();

	set_ps_display("startup");

	SetProcessingMode(InitProcessing);

	/*
	 * Set default values for command-line options.
	 */
	Noversion = false;
	EchoQuery = false;

	if (!IsUnderPostmaster)
	{
		InitializeGUCOptions();
		potential_DataDir = getenv("PGDATA");
	}

	/* ----------------
	 *	parse command line arguments
	 *
	 *	There are now two styles of command line layout for the backend:
	 *
	 *	For interactive use (not started from postmaster) the format is
	 *		postgres [switches] [databasename]
	 *	If the databasename is omitted it is taken to be the user name.
	 *
	 *	When started from the postmaster, the format is
	 *		postgres [secure switches] -p databasename [insecure switches]
	 *	Switches appearing after -p came from the client (via "options"
	 *	field of connection request).  For security reasons we restrict
	 *	what these switches can do.
	 * ----------------
	 */

	/* all options are allowed until '-p' */
	secure = true;
	ctx = PGC_POSTMASTER;
	gucsource = PGC_S_ARGV;		/* initial switches came from command line */

	while ((flag = getopt(argc, argv, "aA:B:c:CD:d:Eef:FghiNOPQo:p:S:st:v:W:x:z:-:")) != -1)

		switch (flag)
		{
			case 'a':
				{
					/* notused */
					break;
				}

			case 'A':
#ifdef USE_ASSERT_CHECKING
				SetConfigOption("debug_assertions", optarg, ctx, gucsource);
#else
				elog(WARNING, "Assert checking is not compiled in");
#endif
				break;

			case 'B':

				/*
				 * specify the size of buffer pool
				 */
				elog(DEBUG1, "%s\n", optarg);
				SetConfigOption("shared_buffers", optarg, ctx, gucsource);
				break;

			case 'C':

				/*
				 * don't print version string
				 */
				Noversion = true;
				break;

			case 'D':			/* PGDATA directory */
				if (secure)
					potential_DataDir = optarg;
				break;

			case 'd':			/* debug level */
				{
					debug_flag = atoi(optarg);
					/* Set server debugging level. */
					if (atoi(optarg) != 0)
					{
						char	   *debugstr = palloc(strlen("debug") + strlen(optarg) + 1);

						sprintf(debugstr, "debug%s", optarg);
						SetConfigOption("server_min_messages", debugstr, ctx, gucsource);
						pfree(debugstr);

					}
					else

						/*
						 * -d0 allows user to prevent postmaster debug
						 * from propagating to backend.  It would be nice
						 * to set it to the postgresql.conf value here.
						 */
						SetConfigOption("server_min_messages", "notice",
										ctx, gucsource);
				}
				break;

			case 'E':

				/*
				 * E - echo the query the user entered
				 */
				EchoQuery = true;
				break;

			case 'e':

				/*
				 * Use european date formats.
				 */
				SetConfigOption("datestyle", "euro", ctx, gucsource);
				break;

			case 'F':

				/*
				 * turn off fsync
				 */
				SetConfigOption("fsync", "false", ctx, gucsource);
				break;

			case 'f':

				/*
				 * f - forbid generation of certain plans
				 */
				tmp = NULL;
				switch (optarg[0])
				{
					case 's':	/* seqscan */
						tmp = "enable_seqscan";
						break;
					case 'i':	/* indexscan */
						tmp = "enable_indexscan";
						break;
					case 't':	/* tidscan */
						tmp = "enable_tidscan";
						break;
					case 'n':	/* nestloop */
						tmp = "enable_nestloop";
						break;
					case 'm':	/* mergejoin */
						tmp = "enable_mergejoin";
						break;
					case 'h':	/* hashjoin */
						tmp = "enable_hashjoin";
						break;
					default:
						errs++;
				}
				if (tmp)
					SetConfigOption(tmp, "false", ctx, gucsource);
				break;

			case 'g':
				enable_eddy = true;

                while((argv[optind] != NULL) && (argv[optind][0] != '-'))
                {
                    if(strcmp(argv[optind], "basestems") == 0)
                        UseBaseSteMs = true;
                    else if(strcmp(argv[optind], "migrate") == 0)
                        UseMigrateState = true;
                    else if(strcmp(argv[optind], "changeroutingpolicy") == 0)
                        UseChangeRoutingPolicy = true;
                    else 
                    {
                        elog(ERROR, "don't understand the --%s-- after -g\n", argv[optind]);
                    }
                    optind++;
                }
                Assert(!UseBaseSteMs || !UseMigrateState); /* shouldn't try to migrate state in the  */
                                                           /*   base stems case */

				break;

			case 'h':
				UseSteMHashJoins = true;
				break;

			case 'i':
				dontExecute = true;
				break;

			case 'N':

				/*
				 * N - Don't use newline as a query delimiter
				 */
				UseNewLine = 0;
				break;

			case 'O':

				/*
				 * allow system table structure modifications
				 */
				if (secure)		/* XXX safe to allow from client??? */
					allowSystemTableMods = true;
				break;

			case 'P':

				/*
				 * ignore system indexes
				 */
				if (secure)		/* XXX safe to allow from client??? */
					IgnoreSystemIndexes(true);
				break;

			case 'o':

				/*
				 * o - send output (stdout and stderr) to the given file
				 */
				if (secure)
					StrNCpy(OutputFileName, optarg, MAXPGPATH);
				break;

			case 'p':

				/*
				 * p - special flag passed if backend was forked by a
				 * postmaster.
				 */
				if (secure)
				{
					DBName = strdup(optarg);
					secure = false;		/* subsequent switches are NOT
										 * secure */
					ctx = PGC_BACKEND;
					gucsource = PGC_S_CLIENT;
				}
				break;

			case 'Q':
				/*
				 * -Q flag allows the TCQ FE to handle streaming queries
				 * without using the TCQ BE.  This will automatically
				 * turn symmetric hash joins on.
				 *
				 */
				elog(DEBUG1, "A TCQ FE started w/ability to run streaming queries");
				RunSQinFE = true;
				break;
				
			case 'S':
				
				/*
				 * S - amount of sort memory to use in 1k bytes
				 */
				SetConfigOption("sort_mem", optarg, ctx, gucsource);
				break;

			case 's':

				/*
				 * s - report usage statistics (timings) after each query
				 */
				SetConfigOption("show_statement_stats", "true", ctx, gucsource);
				break;

			case 't':
				/* ---------------
				 *	tell postgres to report usage statistics (timings) for
				 *	each query
				 *
				 *	-tpa[rser] = print stats for parser time of each query
				 *	-tpl[anner] = print stats for planner time of each query
				 *	-te[xecutor] = print stats for executor time of each query
				 *	caution: -s can not be used together with -t.
				 * ----------------
				 */
				tmp = NULL;
				switch (optarg[0])
				{
					case 'p':
						if (optarg[1] == 'a')
							tmp = "show_parser_stats";
						else if (optarg[1] == 'l')
							tmp = "show_planner_stats";
						else
							errs++;
						break;
					case 'e':
						tmp = "show_executor_stats";
						break;
					default:
						errs++;
						break;
				}
				if (tmp)
					SetConfigOption(tmp, "true", ctx, gucsource);
				break;

			case 'v':
				if (secure)
					FrontendProtocol = (ProtocolVersion) atoi(optarg);
				break;

			case 'W':

				/*
				 * wait N seconds to allow attach from a debugger
				 */
				elog(DEBUG1, "Sleeping for %d secs ...", atoi(optarg));
				sleep(atoi(optarg));
				break;

			case 'x':
#ifdef NOT_USED					/* planner/xfunc.h */

				/*
				 * control joey hellerstein's expensive function
				 * optimization
				 */
				if (XfuncMode != 0)
				{
					elog(WARNING, "only one -x flag is allowed");
					errs++;
					break;
				}
				if (strcmp(optarg, "off") == 0)
					XfuncMode = XFUNC_OFF;
				else if (strcmp(optarg, "nor") == 0)
					XfuncMode = XFUNC_NOR;
				else if (strcmp(optarg, "nopull") == 0)
					XfuncMode = XFUNC_NOPULL;
				else if (strcmp(optarg, "nopm") == 0)
					XfuncMode = XFUNC_NOPM;
				else if (strcmp(optarg, "pullall") == 0)
					XfuncMode = XFUNC_PULLALL;
				else if (strcmp(optarg, "wait") == 0)
					XfuncMode = XFUNC_WAIT;
				else
				{
					elog(WARNING, "use -x {off,nor,nopull,nopm,pullall,wait}");
					errs++;
				}
#endif
				break;

			case 'z':			/* @BdstemsAD */

				/*
				 * This is only used for debugging convenience, and can b
				 * be removed if somebody wants -- Amol.
				 */
				use_static_command = 1;

				use_command = atoi(optarg);

				break;			/* @EdstemsAD */

			case 'c':
			case '-':
				{
					char	   *name,
							   *value;

					ParseLongOption(optarg, &name, &value);
					if (!value)
					{
						if (flag == '-')
							elog(ERROR, "--%s requires argument", optarg);
						else
							elog(ERROR, "-c %s requires argument", optarg);
					}

					SetConfigOption(name, value, ctx, gucsource);
					free(name);
					if (value)
						free(value);
					break;
				}

			default:
				errs++;
				break;
		}

	/*
	 * -d is not the same as setting server_min_messages because it
	 * enables other output options.
	 */
	if (debug_flag >= 1)
		SetConfigOption("log_connections", "true", ctx, gucsource);
	if (debug_flag >= 2)
		SetConfigOption("log_statement", "true", ctx, gucsource);
	if (debug_flag >= 3)
		SetConfigOption("debug_print_parse", "true", ctx, gucsource);
	if (debug_flag >= 4)
		SetConfigOption("debug_print_plan", "true", ctx, gucsource);
	if (debug_flag >= 5)
		SetConfigOption("debug_print_rewritten", "true", ctx, gucsource);

	/*
	 * Post-processing for command line options.
	 */
	if (Show_statement_stats &&
		(Show_parser_stats || Show_planner_stats || Show_executor_stats))
	{
		elog(WARNING, "Query statistics are disabled because parser, planner, or executor statistics are on.");
		SetConfigOption("show_statement_stats", "false", ctx, gucsource);
	}

	if (!IsUnderPostmaster)
	{
		if (!potential_DataDir)
		{
			fprintf(stderr, "%s does not know where to find the database system "
			   "data.  You must specify the directory that contains the "
				"database system either by specifying the -D invocation "
			 "option or by setting the PGDATA environment variable.\n\n",
					argv[0]);
			proc_exit(1);
		}
		SetDataDir(potential_DataDir);
	}
	Assert(DataDir);

	/*
	 * Set up signal handlers and masks.
	 *
	 * Note that postmaster blocked all signals before forking child process,
	 * so there is no race condition whereby we might receive a signal
	 * before we have set up the handler.
	 *
	 * Also note: it's best not to use any signals that are SIG_IGNored in
	 * the postmaster.	If such a signal arrives before we are able to
	 * change the handler to non-SIG_IGN, it'll get dropped.  Instead,
	 * make a dummy handler in the postmaster to reserve the signal. (Of
	 * course, this isn't an issue for signals that are locally generated,
	 * such as SIGALRM and SIGPIPE.)
	 */

	pqsignal(SIGHUP, SigHupHandler);	/* set flag to read config file */
	pqsignal(SIGINT, StatementCancelHandler);	/* cancel current query */
	pqsignal(SIGTERM, die);		/* cancel current query and exit */
	pqsignal(SIGQUIT, quickdie);	/* hard crash time */
	pqsignal(SIGALRM, handle_sig_alarm);		/* timeout conditions */

	/*
	 * Ignore failure to write to frontend. Note: if frontend closes
	 * connection, we will notice it and exit cleanly when control next
	 * returns to outer loop.  This seems safer than forcing exit in the
	 * midst of output during who-knows-what operation...
	 */
	pqsignal(SIGPIPE, SIG_IGN);
	pqsignal(SIGUSR1, SIG_IGN); /* this signal available for use */

	pqsignal(SIGUSR2, Async_NotifyHandler);		/* flush also sinval cache */
	pqsignal(SIGFPE, FloatExceptionHandler);

	/*
	 * Reset some signals that are accepted by postmaster but not by
	 * backend
	 */
	pqsignal(SIGCHLD, SIG_DFL); /* system() requires this on some
								 * platforms */

	pqinitmask();

	/* We allow SIGQUIT (quickdie) at all times */
#ifdef HAVE_SIGPROCMASK
	sigdelset(&BlockSig, SIGQUIT);
#else
	BlockSig &= ~(sigmask(SIGQUIT));
#endif

	PG_SETMASK(&BlockSig);		/* block everything except SIGQUIT */


	if (IsUnderPostmaster)
	{
		/* noninteractive case: nothing should be left after switches */
		if (errs || argc != optind || DBName == NULL)
		{
			elog(WARNING, "%s: invalid command line arguments\nTry -? for help.",
				 argv[0]);
			proc_exit(0);		/* not 1, that causes system-wide
								 * restart... */
		}
		BaseInit();
	}
	else
	{
		/* interactive case: database name can be last arg on command line */
		if (errs || argc - optind > 1)
		{
			elog(WARNING, "%s: invalid command line arguments\nTry -? for help.",
				 argv[0]);
			proc_exit(1);
		}
		else if (argc - optind == 1)
			DBName = argv[optind];
		else if ((DBName = username) == NULL)
		{
			elog(WARNING, "%s: user name undefined and no database specified",
				 argv[0]);
			proc_exit(1);
		}

		/*
		 * On some systems our dynloader code needs the executable's
		 * pathname.  (If under postmaster, this was done already.)
		 */
		if (FindExec(pg_pathname, argv[0], "postgres") < 0)
			elog(FATAL, "%s: could not locate executable, bailing out...",
				 argv[0]);

		/*
		 * Validate we have been given a reasonable-looking DataDir (if
		 * under postmaster, assume postmaster did this already).
		 */
		ValidatePgVersion(DataDir);

		/*
		 * Create lockfile for data directory.
		 */
		if (!CreateDataDirLockFile(DataDir, false))
			proc_exit(1);

		XLOGPathInit();
		BaseInit();

		/*
		 * Start up xlog for standalone backend, and register to have it
		 * closed down at exit.
		 */
		StartupXLOG();
		on_shmem_exit(ShutdownXLOG, 0);
	}

	/*
	 * Set up additional info.
	 */

#ifdef CYR_RECODE
	SetCharSet();
#endif

	/*
	 * General initialization.
	 *
	 * NOTE: if you are tempted to add code in this vicinity, consider
	 * putting it inside InitPostgres() instead.  In particular, anything
	 * that involves database access should be there, not here.
	 */
	elog(DEBUG2, "InitPostgres");
	if (MyProcessType == PROC_TCQ_FRONTEND)
	{
		/* Set the process name so we can debug it later */
		set_ps_display("tcqfrontend");

	}
	InitPostgres(DBName, username);





	/*
	 * This is where we return if we are not going to actually read in
	 * queries, compile them, and send them back up
	 */
	SetProcessingMode(NormalProcessing);
	if ((MyProcessType == PROC_TCQ_BACKEND) ||
		(MyProcessType == PROC_TCQ_WRAP_CH))
	{
		return 1;
	}							/* @EdtcqSK */



	/*
	 * Send this backend's cancellation info to the frontend.
	 */
	if (whereToSendOutput == Remote &&
		PG_PROTOCOL_MAJOR(FrontendProtocol) >= 2)
	{
		StringInfoData buf;

		pq_beginmessage(&buf);
		pq_sendbyte(&buf, 'K');
		pq_sendint(&buf, (int32) MyProcPid, sizeof(int32));
		pq_sendint(&buf, (int32) MyCancelKey, sizeof(int32));
		pq_endmessage(&buf);
		/* Need not flush since ReadyForQuery will do it. */
	}

	if (!IsUnderPostmaster)
	{
		puts("\nPOSTGRES backend interactive interface ");
		puts("$Revision: 1.115 $ $Date: 2006/02/06 01:19:46 $\n");
	}

	/*
	 * Create the memory context we will use in the main loop.
	 *
	 * QueryContext is reset once per iteration of the main loop, ie, upon
	 * completion of processing of each supplied query string. It can
	 * therefore be used for any data that should live just as long as the
	 * query string --- parse trees, for example.
	 */
	QueryContext = AllocSetContextCreate(TopMemoryContext,
										 "QueryContext",
										 ALLOCSET_DEFAULT_MINSIZE,
										 ALLOCSET_DEFAULT_INITSIZE,
										 ALLOCSET_DEFAULT_MAXSIZE);

	/* ----------
	 * Tell the statistics collector that we're alive and
	 * to which database we belong.
	 * ----------
	 */
	pgstat_bestart();

	/*
	 * POSTGRES main processing loop begins here
	 *
	 * If an exception is encountered, processing resumes here so we abort
	 * the current transaction and start a new one.
	 */

	if (sigsetjmp(Warn_restart, 1) != 0)
	{
		printf("in Warn_restart code\n");

		/*
		 * NOTE: if you are tempted to add more code in this if-block,
		 * consider the probability that it should be in
		 * AbortTransaction() instead.
		 *
		 * Make sure we're not interrupted while cleaning up.  Also forget
		 * any pending QueryCancel request, since we're aborting anyway.
		 * Force InterruptHoldoffCount to a known state in case we elog'd
		 * from inside a holdoff section.
		 */
		ImmediateInterruptOK = false;
		QueryCancelPending = false;
		InterruptHoldoffCount = 1;
		CritSectionCount = 0;	/* should be unnecessary, but... */
		disable_sig_alarm(true);
		QueryCancelPending = false;		/* again in case timeout occurred */
		DisableNotifyInterrupt();
		debug_query_string = NULL;

		/*
		 * Make sure we are in a valid memory context during recovery.
		 *
		 * We use ErrorContext in hopes that it will have some free space
		 * even if we're otherwise up against it...
		 */
		MemoryContextSwitchTo(ErrorContext);

		/* Do the recovery */
		elog(DEBUG1, "AbortCurrentTransaction");
		AbortCurrentTransaction();



		/*
		 * Now return to normal top-level context and clear ErrorContext
		 * for next time.
		 */
		MemoryContextSwitchTo(TopMemoryContext);
		MemoryContextResetAndDeleteChildren(ErrorContext);

		/*
		 * Clear flag to indicate that we got out of error recovery mode
		 * successfully.  (Flag was set in elog.c before longjmp().)
		 */
		InError = false;

		/*
		 * Exit interrupt holdoff section we implicitly established above.
		 */
		RESUME_INTERRUPTS();
	}

	Warn_restart_ready = true;	/* we can now handle elog(ERROR) */

	PG_SETMASK(&UnBlockSig);

	/*
	 * Non-error queries loop here.
	 */

	for (;;)
	{
		/*
		 * Release storage left over from prior query cycle, and create a
		 * new query input buffer in the cleared QueryContext.
		 */
		MemoryContextSwitchTo(QueryContext);
		MemoryContextResetAndDeleteChildren(QueryContext);
		if (shmQueryContext != NULL)
		{
			/*
			 * either the context was successfully handed off to the tcq
			 * backend (which inherits responsibility for freeing it) or
			 * it is destroyed here
			 */
			SHMContextDelete(shmQueryContext);
			shmQueryContext = NULL;
		}

		parser_input = makeStringInfo();

		/*
		 * (1) tell the frontend we're ready for a new query.
		 *
		 * Note: this includes fflush()'ing the last of the prior output.
		 */
		ReadyForQuery(whereToSendOutput);

		/* ----------
		 * Tell the statistics collector what we've collected
		 * so far.
		 * ----------
		 */
		pgstat_report_tabstat();

		if (IsTransactionBlock())
		{
			set_ps_display("idle in transaction");
			pgstat_report_activity("<IDLE> in transaction");
		}
		else
		{
			set_ps_display("idle");
			pgstat_report_activity("<IDLE>");
		}

		/*
		 * (2) deal with pending asynchronous NOTIFY from other backends,
		 * and enable async.c's signal handler to execute NOTIFY directly.
		 * Then set up other stuff needed before blocking for input.
		 */
		QueryCancelPending = false;		/* forget any earlier CANCEL
										 * signal */

		EnableNotifyInterrupt();

		/* Allow "die" interrupt to be processed while waiting */
		ImmediateInterruptOK = true;
		/* and don't forget to detect one that already arrived */
		QueryCancelPending = false;
		CHECK_FOR_INTERRUPTS();

		/*
		 * (3) read a command (loop blocks here)
		 */
		if (use_static_command) /* @BdstemsAD */
			firstchar = StaticReadCommand(parser_input);
		else
		{
			firstchar = ReadCommand(parser_input);
		}						/* @EdstemsAD */

        elog(DEBUG1, "Executing query : %s", parser_input->data);	
            /* @dstemsAD */

		/*
		 * (4) disable async signal conditions again.
		 */
		ImmediateInterruptOK = false;
		QueryCancelPending = false;		/* forget any CANCEL signal */

		DisableNotifyInterrupt();

		/*
		 * (5) check for any other interesting events that happened while
		 * we slept.
		 */
		if (got_SIGHUP)
		{
			got_SIGHUP = false;
			ProcessConfigFile(PGC_SIGHUP);
		}

		/*
		 * (6) process the command.
		 */
		switch (firstchar)
		{
				/*
				 * 'F' indicates a fastpath call.
				 */
			case 'F':
				/* ----------
				 * Tell the collector what we're doing
				 * ----------
				 */
				pgstat_report_activity("<FASTPATH> function call");

				/* start an xact for this function invocation */
				start_xact_command();

				if (HandleFunctionRequest() == EOF)
				{
					/* lost frontend connection during F message input */

					/*
					 * Reset whereToSendOutput to prevent elog from
					 * attempting to send any more messages to client.
					 */
					if (whereToSendOutput == Remote)
						whereToSendOutput = None;

					proc_exit(0);
				}

				/* commit the function-invocation transaction */
				finish_xact_command(false);
				break;

				/*
				 * 'Q' indicates a user query
				 */
			case 'Q':
				if (strspn(parser_input->data, " \t\r\n") == parser_input->len)
				{
					/*
					 * if there is nothing in the input buffer, don't
					 * bother trying to parse and execute anything; just
					 * send back a quick NullCommand response.
					 */
					if (IsUnderPostmaster)
						NullCommand(Remote);
				}
				else
				{
					/*
					 * otherwise, process the input string.
					 *
					 * Note: transaction command start/end is now done within
					 * pg_exec_query_string(), not here.
					 */
					if (Show_statement_stats)
						ResetUsage();

					pgstat_report_activity(parser_input->data);

					if (MyProcessType == PROC_TCQ_FRONTEND)		/* @Bdtcq */
					{
						shmQueryContext = SHMAllocSetContextCreate(TopSharedMemoryContext,
												 "query plan subcontext",
												ALLOCSET_DEFAULT_MINSIZE,
											   ALLOCSET_DEFAULT_INITSIZE,
												ALLOCSET_DEFAULT_MAXSIZE,
													SHM_DEFAULT_SEGMENT);
						compileMemoryContext = shmQueryContext;
					}
					else
						compileMemoryContext = QueryContext;
					pg_exec_query_string(parser_input,
										 whereToSendOutput,
										 compileMemoryContext); /* @Edtcq */

					if (Show_statement_stats)
						ShowUsage("QUERY STATISTICS");
				}
				break;

				/*
				 * 'X' means that the frontend is closing down the socket.
				 * EOF means unexpected loss of frontend connection.
				 * Either way, perform normal shutdown.
				 */
			case 'X':
			case EOF:

				/*
				 * Reset whereToSendOutput to prevent elog from attempting
				 * to send any more messages to client.
				 */
				if (whereToSendOutput == Remote)
					whereToSendOutput = None;

				/*
				 * NOTE: if you are tempted to add more code here, DON'T!
				 * Whatever you had in mind to do should be set up as an
				 * on_proc_exit or on_shmem_exit callback, instead.
				 * Otherwise it will fail to be called during other
				 * backend-shutdown scenarios.
				 */
				proc_exit(0);

			default:
				elog(ERROR, "unknown frontend message was received");
		}

#ifdef MEMORY_CONTEXT_CHECKING

		/*
		 * Check all memory after each backend loop.  This is a rather
		 * weird place to do it, perhaps.
		 */
		MemoryContextCheck(TopMemoryContext);
#endif
	}							/* end of input-reading loop */

	/* can't get here because the above loop never exits */
	Assert(false);

	return 1;					/* keep compiler quiet */
}

#ifndef HAVE_GETRUSAGE
#include "rusagestub.h"
#else
#include <sys/resource.h>
#endif   /* HAVE_GETRUSAGE */

struct rusage Save_r;
struct timeval Save_t;




void
ResetUsage(void)
{
	getrusage(RUSAGE_SELF, &Save_r);
	gettimeofday(&Save_t, NULL);
	ResetBufferUsage();
	/* ResetTupleCount(); */
}

void
ShowUsage(const char *title)
{
	StringInfoData str;
	struct timeval user,
				sys;
	struct timeval elapse_t;
	struct rusage r;
	char	   *bufusage;

	getrusage(RUSAGE_SELF, &r);
	gettimeofday(&elapse_t, NULL);
	memcpy((char *) &user, (char *) &r.ru_utime, sizeof(user));
	memcpy((char *) &sys, (char *) &r.ru_stime, sizeof(sys));
	if (elapse_t.tv_usec < Save_t.tv_usec)
	{
		elapse_t.tv_sec--;
		elapse_t.tv_usec += 1000000;
	}
	if (r.ru_utime.tv_usec < Save_r.ru_utime.tv_usec)
	{
		r.ru_utime.tv_sec--;
		r.ru_utime.tv_usec += 1000000;
	}
	if (r.ru_stime.tv_usec < Save_r.ru_stime.tv_usec)
	{
		r.ru_stime.tv_sec--;
		r.ru_stime.tv_usec += 1000000;
	}

	/*
	 * the only stats we don't show here are for memory usage -- i can't
	 * figure out how to interpret the relevant fields in the rusage
	 * struct, and they change names across o/s platforms, anyway. if you
	 * can figure out what the entries mean, you can somehow extract
	 * resident set size, shared text size, and unshared data and stack
	 * sizes.
	 */
	initStringInfo(&str);

	appendStringInfo(&str, "! system usage stats:\n");
	appendStringInfo(&str,
			"!\t%ld.%06ld elapsed %ld.%06ld user %ld.%06ld system sec\n",
					 (long) (elapse_t.tv_sec - Save_t.tv_sec),
					 (long) (elapse_t.tv_usec - Save_t.tv_usec),
					 (long) (r.ru_utime.tv_sec - Save_r.ru_utime.tv_sec),
				   (long) (r.ru_utime.tv_usec - Save_r.ru_utime.tv_usec),
					 (long) (r.ru_stime.tv_sec - Save_r.ru_stime.tv_sec),
				  (long) (r.ru_stime.tv_usec - Save_r.ru_stime.tv_usec));
	appendStringInfo(&str,
					 "!\t[%ld.%06ld user %ld.%06ld sys total]\n",
					 (long) user.tv_sec,
					 (long) user.tv_usec,
					 (long) sys.tv_sec,
					 (long) sys.tv_usec);
/* BeOS has rusage but only has some fields, and not these... */
#if defined(HAVE_GETRUSAGE)
	appendStringInfo(&str,
					 "!\t%ld/%ld [%ld/%ld] filesystem blocks in/out\n",
					 r.ru_inblock - Save_r.ru_inblock,
	/* they only drink coffee at dec */
					 r.ru_oublock - Save_r.ru_oublock,
					 r.ru_inblock, r.ru_oublock);
	appendStringInfo(&str,
		  "!\t%ld/%ld [%ld/%ld] page faults/reclaims, %ld [%ld] swaps\n",
					 r.ru_majflt - Save_r.ru_majflt,
					 r.ru_minflt - Save_r.ru_minflt,
					 r.ru_majflt, r.ru_minflt,
					 r.ru_nswap - Save_r.ru_nswap,
					 r.ru_nswap);
	appendStringInfo(&str,
	 "!\t%ld [%ld] signals rcvd, %ld/%ld [%ld/%ld] messages rcvd/sent\n",
					 r.ru_nsignals - Save_r.ru_nsignals,
					 r.ru_nsignals,
					 r.ru_msgrcv - Save_r.ru_msgrcv,
					 r.ru_msgsnd - Save_r.ru_msgsnd,
					 r.ru_msgrcv, r.ru_msgsnd);
	appendStringInfo(&str,
		 "!\t%ld/%ld [%ld/%ld] voluntary/involuntary context switches\n",
					 r.ru_nvcsw - Save_r.ru_nvcsw,
					 r.ru_nivcsw - Save_r.ru_nivcsw,
					 r.ru_nvcsw, r.ru_nivcsw);
#endif   /* HAVE_GETRUSAGE */

	bufusage = ShowBufferUsage();
	appendStringInfo(&str, "! buffer usage stats:\n%s", bufusage);
	pfree(bufusage);

	/* remove trailing newline */
	if (str.data[str.len - 1] == '\n')
		str.data[--str.len] = '\0';

	elog(LOG, "%s\n%s", title, str.data);

	pfree(str.data);
}

/* ----------------------------------------------------------------
 *		CreateCommandTag
 *
 *		utility to get a string representation of the
 *		command operation.
 * ----------------------------------------------------------------
 */
static const char *
CreateCommandTag(Node *parsetree)
{
	const char *tag;

	switch (nodeTag(parsetree))
	{
		case T_InsertStmt:
			tag = "INSERT";
			break;

		case T_DeleteStmt:
			tag = "DELETE";
			break;

		case T_UpdateStmt:
			tag = "UPDATE";
			break;

		case T_SelectStmt:
			tag = "SELECT";
			break;

		case T_TransactionStmt:
			{
				TransactionStmt *stmt = (TransactionStmt *) parsetree;

				switch (stmt->command)
				{
					case BEGIN_TRANS:
						tag = "BEGIN";
						break;

					case START:
						tag = "START TRANSACTION";
						break;

					case COMMIT:
						tag = "COMMIT";
						break;

					case ROLLBACK:
						tag = "ROLLBACK";
						break;

					default:
						tag = "???";
						break;
				}
			}
			break;

		case T_ClosePortalStmt:
			tag = "CLOSE CURSOR";
			break;

		case T_FetchStmt:
			{
				FetchStmt  *stmt = (FetchStmt *) parsetree;

				tag = (stmt->ismove) ? "MOVE" : "FETCH";
			}
			break;

		case T_CreateDomainStmt:
			tag = "CREATE DOMAIN";
			break;

		case T_CreateSchemaStmt:
			tag = "CREATE SCHEMA";
			break;

		case T_CreateStmt:
			tag = "CREATE TABLE";
			break;

		case T_DropStmt:
			switch (((DropStmt *) parsetree)->removeType)
			{
				case DROP_TABLE:
					tag = "DROP TABLE";
					break;
				case DROP_SEQUENCE:
					tag = "DROP SEQUENCE";
					break;
				case DROP_VIEW:
					tag = "DROP VIEW";
					break;
				case DROP_INDEX:
					tag = "DROP INDEX";
					break;
				case DROP_TYPE:
					tag = "DROP TYPE";
					break;
				case DROP_DOMAIN:
					tag = "DROP DOMAIN";
					break;
				case DROP_CONVERSION:
					tag = "DROP CONVERSION";
					break;
				case DROP_SCHEMA:
					tag = "DROP SCHEMA";
					break;
				case DROP_STREAM:		/* @BdtcqSK */
					tag = "DROP STREAM";
					break;
				case DROP_WRAPPER:
					tag = "DROP WRAPPER";
					break;		/* @EdtcqSK */
				default:
					tag = "???";
			}
			break;

		case T_TruncateStmt:
			tag = "TRUNCATE TABLE";
			break;

		case T_CommentStmt:
			tag = "COMMENT";
			break;

		case T_CopyStmt:
			tag = "COPY";
			break;

		case T_RenameStmt:
			if (((RenameStmt *) parsetree)->renameType == RENAME_TRIGGER)
				tag = "ALTER TRIGGER";
			else
				tag = "ALTER TABLE";
			break;

		case T_AlterTableStmt:
			tag = "ALTER TABLE";
			break;
		case T_AlterStreamStmt:
			tag = "ALTER";
			break;

		case T_GrantStmt:
			{
				GrantStmt  *stmt = (GrantStmt *) parsetree;

				tag = (stmt->is_grant) ? "GRANT" : "REVOKE";
			}
			break;

		case T_DefineStmt:
			switch (((DefineStmt *) parsetree)->defType)
			{
				case AGGREGATE:
					tag = "CREATE AGGREGATE";
					break;
				case OPERATOR:
					tag = "CREATE OPERATOR";
					break;
				case TYPE_P:
					tag = "CREATE TYPE";
					break;
				default:
					tag = "???";
			}
			break;

		case T_CompositeTypeStmt:
			tag = "CREATE TYPE";
			break;

		case T_ViewStmt:
			tag = "CREATE VIEW";
			break;

		case T_CreateFunctionStmt:
			tag = "CREATE FUNCTION";
			break;

		case T_IndexStmt:
			tag = "CREATE INDEX";
			break;

		case T_RuleStmt:
			tag = "CREATE RULE";
			break;

		case T_CreateSeqStmt:
			tag = "CREATE SEQUENCE";
			break;

		case T_CreateStrmStmt:
			tag = "CREATE";
			break;
		case T_CreateWrapperStmt:
			tag = "CREATE";
			break;

		case T_RemoveAggrStmt:
			tag = "DROP AGGREGATE";
			break;

		case T_RemoveFuncStmt:
			tag = "DROP FUNCTION";
			break;

		case T_RemoveOperStmt:
			tag = "DROP OPERATOR";
			break;

		case T_CreatedbStmt:
			tag = "CREATE DATABASE";
			break;

		case T_AlterDatabaseSetStmt:
			tag = "ALTER DATABASE";
			break;

		case T_DropdbStmt:
			tag = "DROP DATABASE";
			break;

		case T_NotifyStmt:
			tag = "NOTIFY";
			break;

		case T_ListenStmt:
			tag = "LISTEN";
			break;

		case T_UnlistenStmt:
			tag = "UNLISTEN";
			break;

		case T_LoadStmt:
			tag = "LOAD";
			break;

		case T_ClusterStmt:
			tag = "CLUSTER";
			break;

		case T_VacuumStmt:
			if (((VacuumStmt *) parsetree)->vacuum)
				tag = "VACUUM";
			else
				tag = "ANALYZE";
			break;

		case T_ExplainStmt:
			tag = "EXPLAIN";
			break;

		case T_VariableSetStmt:
			tag = "SET";
			break;

		case T_VariableShowStmt:
			tag = "SHOW";
			break;

		case T_VariableResetStmt:
			tag = "RESET";
			break;

		case T_CreateTrigStmt:
			tag = "CREATE TRIGGER";
			break;

		case T_DropPropertyStmt:
			switch (((DropPropertyStmt *) parsetree)->removeType)
			{
				case DROP_TRIGGER:
					tag = "DROP TRIGGER";
					break;
				case DROP_RULE:
					tag = "DROP RULE";
					break;
				default:
					tag = "???";
			}
			break;

		case T_CreatePLangStmt:
			tag = "CREATE LANGUAGE";
			break;

		case T_DropPLangStmt:
			tag = "DROP LANGUAGE";
			break;

		case T_CreateUserStmt:
			tag = "CREATE USER";
			break;

		case T_AlterUserStmt:
			tag = "ALTER USER";
			break;

		case T_AlterUserSetStmt:
			tag = "ALTER USER";
			break;

		case T_DropUserStmt:
			tag = "DROP USER";
			break;

		case T_LockStmt:
			tag = "LOCK TABLE";
			break;

		case T_ConstraintsSetStmt:
			tag = "SET CONSTRAINTS";
			break;

		case T_CreateGroupStmt:
			tag = "CREATE GROUP";
			break;

		case T_AlterGroupStmt:
			tag = "ALTER GROUP";
			break;

		case T_DropGroupStmt:
			tag = "DROP GROUP";
			break;

		case T_CheckPointStmt:
			tag = "CHECKPOINT";
			break;

		case T_ReindexStmt:
			tag = "REINDEX";
			break;

		case T_CreateConversionStmt:
			tag = "CREATE CONVERSION";
			break;

		case T_CreateCastStmt:
			tag = "CREATE CAST";
			break;

		case T_DropCastStmt:
			tag = "DROP CAST";
			break;

		case T_CreateOpClassStmt:
			tag = "CREATE OPERATOR CLASS";
			break;

		case T_RemoveOpClassStmt:
			tag = "DROP OPERATOR CLASS";
			break;

		case T_PrepareStmt:
			tag = "PREPARE";
			break;

		case T_ExecuteStmt:
			tag = "EXECUTE";
			break;

		case T_DeallocateStmt:
			tag = "DEALLOCATE";
			break;

		default:
			elog(LOG, "CreateCommandTag: unknown parse node type %d",
				 nodeTag(parsetree));
			tag = "???";
			break;
	}

	return tag;
}
static List * recursive_query_batch_to_qtlist(SelectStmt * s, List *query_trees, char* portalName)
{


  List * qttmp=NIL;
  if(s->op == SETOP_NONE) 
    {
      /* the results from the output query must be sent into the correct portal */
      if(s->resultClause == NULL)
	{
	  s->portalname = portalName;
	  portalName=NULL;
	}
      qttmp = pg_analyze_and_rewrite(((Node*)s));
      
      if(length(qttmp) != 1)
	{
	  elog(ERROR, "expected analyze_and_rewrite to return exactly one querytree");
	}
      query_trees = lcons(lfirst(qttmp),query_trees);
    }
  if(s->larg != NULL) query_trees=recursive_query_batch_to_qtlist(s->larg, query_trees, portalName);
  if(s->rarg != NULL) query_trees=recursive_query_batch_to_qtlist(s->rarg, query_trees, portalName);
  return query_trees;
}


static List * analyze_and_rewrite_recursive_query(Node * parsetree)
{
  List * query_trees = NIL;
	

	if(IsA(parsetree, SelectStmt))
	{
		SelectStmt *s = (SelectStmt *)parsetree;
	

		if(s->op != SETOP_RECURSIVE_NEXT)
			{
			  if(s->resultClause==NULL)
			    {
			      /* not a recursive query, handle in normal case */
			      return NIL;
			    }
			
			  else 
			    {
			      elog(ERROR, "Recursive queries occur in batches");
			    }
			}
		query_trees= recursive_query_batch_to_qtlist(s,query_trees, s->portalname);
		query_trees = check_recursive_query_batch(query_trees);    /* perform intra-query checks */
		elog(NOTICE, "query batch contains %d queries", length(query_trees));
	}
	return query_trees;
}






static List * check_recursive_query_batch(List * querytree_list)
{
/* does checking on a set of potentailly recursive queries.  returns true
 if the set is valid and recursive.  false if it is a non-recursive query
 batch. and elogs ERROR on error. 

 This function also ensures that the output plan is the last one in the 
 querytree_list.  This is essential so that pg_exec_query_string will
 do the right thing.

*/

	
	int numOutputQueries=0;
	List *iter = NIL;
	List *recursive_targets=NIL;
	Query * result_query=NULL;

	Query *qry = NULL;
	
	foreach(iter, querytree_list)
	{
		qry = (Query *) lfirst(iter);
		if(qry && qry->recurseRelation)
		{
			recursive_targets= lconsi(qry->recurseRelation, recursive_targets);
		}
		else if(qry)
			{
				qry->isRecursiveOutput = true;
				result_query = qry;
				numOutputQueries++;
				
			}

	}
	
	if(numOutputQueries >=1  )
		{
			if(numOutputQueries >1)
				elog(ERROR, "too many output queries");

			/*	
		Query * tmp;
			// pick a single query to present output in the frontend
			// make sure others will be output slaves
			outputQuery = lfirst(outputlist);
			foreach(iter, outputlist)
				{
					tmp = lfirst(iter);
					if(tmp == outputQuery) continue;
					if(!targetListCompatible(tmp, outputQuery))
						elog(ERROR, "two output queries have incompatible target lists");
				}
			*/
		}

	querytree_list = lremove(result_query, querytree_list);
	querytree_list = lappend(querytree_list, result_query); /* result plan last*/


	return querytree_list;
	
}

void parse_query_in_parse_context (MemoryContext* oldcontext, 
				   MemoryContext parse_context, 
				   List **parsetree_list, 
				   StringInfo query_string) {
  
  elog(DEBUG1, "In parse_query_in_parse_context...");
  elog(DEBUG1, "query_string: %s\n", query_string->data);

  /*
   * Switch to appropriate context for constructing parsetrees.
   */
  
  *oldcontext = MemoryContextSwitchTo(parse_context);
  elog(DEBUG1, "Switched to parse_context...");

  /*
   * Do basic parsing of the query or queries (this should be safe even
   * if we are in aborted transaction state!)
   */
  *parsetree_list = pg_parse_query(query_string, NULL, 0);
  elog(DEBUG1, "Created parsetree_list...");
  /*
   * Switch back to execution context to enter the loop.
   */
  MemoryContextSwitchTo(*oldcontext);
  elog(DEBUG1, "Switched back to oldcontext...");

}

void shallow_copy_parse_tree_in_query_context (List **pparsetree_list,
					       List **pnewtree_list) {

		MemoryContext o = MemoryContextSwitchTo(QueryContext);
		*pnewtree_list = (List *) listCopy(*pparsetree_list);

		*pparsetree_list = *pnewtree_list;
		MemoryContextSwitchTo(o);
}

void analyze_and_rewrite_in_parse_context (MemoryContext parse_context,
					   Node** pparsetree,
					   List** pquerytree_list,
					   bool* pisRecursive) {

  MemoryContext oldcontext = MemoryContextSwitchTo(parse_context);

  if(( *pquerytree_list = analyze_and_rewrite_recursive_query(*pparsetree)))
  {
    *pisRecursive=true;
  }
  else
  {
    *pisRecursive=false;
    *pquerytree_list = pg_analyze_and_rewrite(*pparsetree);
  }

  /*
   * Switch back to execution context for planning and execution.
   */
  MemoryContextSwitchTo(oldcontext);
}

void shallow_copy_querytree_in_query_context (List **pquerytree_list,
					      List **pnewtreelist,
					      bool *pisRecursive) {
			MemoryContext o = MemoryContextSwitchTo(QueryContext);
			if(*pisRecursive == false)
			{
				*pnewtreelist = makeList1(lfirst(*pquerytree_list));
			}
			else
			{
				*pnewtreelist = listCopy(*pquerytree_list);
				/* NOTE: listCopy is shallow whiile copyObject is a deep */
			}


			*pquerytree_list = *pnewtreelist;
			MemoryContextSwitchTo(o);
}

void convert_to_fsched_dup_in_parse_context (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree) {
  MemoryContext o;
  
  elog(DEBUG1, "Incorporating plan into FSched-Dup");

  o = MemoryContextSwitchTo(parse_context);
  
  elog(DEBUG1, "Incorporating plan into FSched-Dup");
  *pplan = pg_convert_to_fsched_dup(*pplan, *pquerytree);

  MemoryContextSwitchTo(o);
}

void convert_plan_to_eddy_in_parse_context  (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree) {
  MemoryContext o = MemoryContextSwitchTo(parse_context);

  elog(DEBUG1, "frontend is converting the plan to an eddy");
  *pplan = pg_convert_plan_to_eddy(*pplan, (*pquerytree)->rtable);

  /*
   * The plan may not be an Eddy. Assert(IsA(plan,
   * Eddy));
   */
  MemoryContextSwitchTo(o);
}

void seqscan_to_eddy_in_parse_context_usecqeddy (MemoryContext parse_context,
					      Plan **pplan,
					      Query **pquerytree) {
  
  MemoryContext o = MemoryContextSwitchTo(parse_context);

  /*
   * The last flag makes the streamers non-blocking,
   * true will make them blocking, i.e. they return a
   * null in the slot.
   */
  *pplan = pg_convert_seqscan_to_strmscan(*pplan, *pquerytree, false);		/* @dtcqwchMS */

  elog(DEBUG1, "frontend is converting the plan to an eddy");
  
  if (enable_eddy || ((*pquerytree)->isContinuousQuery && UseCQEddy))
    {
      if (strcasecmp(DatabaseName, telegraphcqdb) != 0)
	{
	  elog(ERROR, "The system has been started in "
	       "TelegraphCQ mode, and the databases specified "
	       "for the TCQ frontend process and TCQ backend "
	       "processes do not match.  "
	       "tcqbackenddb=%s, tcqfrontenddb=%s",
	       telegraphcqdb, DatabaseName);
	}

      /*
	if ((*querytree)->sortClause != NIL)
	elog(ERROR, "TelegraphCQ can not handle order by clauses yet");

      */
      *pplan = pg_convert_plan_to_eddy(*pplan, (*pquerytree)->rtable);
      /* pprint(plan); */
    }

  MemoryContextSwitchTo(o);
}

void seqscan_to_eddy_in_parse_context_runsqinfe (MemoryContext parse_context,
						 Plan **pplan,
						 Query **pquerytree,
						 bool *pisBlocking) {
  MemoryContext oldcontext;
  elog(DEBUG1, "frontend converting scans to streamers");
  oldcontext = MemoryContextSwitchTo(parse_context);
					
  /* Make the streamers blocking for now */
  *pplan = pg_convert_seqscan_to_strmscan(*pplan, *pquerytree, *pisBlocking);		/* @dtcqwchMS */
					
  /* if we're running SQ's in the FE, convert joins to symhashjoin's here */
  if (RunSQinFE && (*pquerytree)->isContinuousQuery)
    {
      *pplan = pg_convert_to_sym_hash_join(*pplan, *pquerytree);
    }
				
  MemoryContextSwitchTo(oldcontext);
}

void seqscan_to_eddy_in_parse_context_tcqfe (MemoryContext parse_context,
						 Plan **pplan,
						 Query **pquerytree) {
  MemoryContext oldcontext;
  elog(DEBUG1, "frontend converting scans to streamers");
  oldcontext = MemoryContextSwitchTo(parse_context);

  /* Make the streamers blocking for now */
  *pplan = pg_convert_seqscan_to_strmscan(*pplan, *pquerytree, true);		/* @dtcqwchMS */

  MemoryContextSwitchTo(oldcontext);
}

void assign_beplan_rt_in_parse_context (MemoryContext parse_context,
					Plan** pbePlan,
					Query** pquerytree,
					bool* pret) {
  MemoryContext oldcontext = MemoryContextSwitchTo(parse_context);

  ((Eddy *) (*pbePlan))->rangetable = (*pquerytree)->rtable;
  *pret = getBindingInformation((Eddy *) (*pbePlan));
  MemoryContextSwitchTo(oldcontext);
}

void append_querystr_to_pqe_in_parse_context (MemoryContext *oldcontext,
					      MemoryContext parse_context,
					      planqentry* ppqe,
					      StringInfo* query_string) {
  elog(DEBUG1, "In append_querystr_to_pqe_in_parse_context...\n");
  *oldcontext = MemoryContextSwitchTo(parse_context);
  elog(DEBUG1, "Switched mem. cxt...\n");
  ppqe->querystr = makeStringInfo();
  elog(DEBUG1, "Created querystr...\n");
  //  initStringInfo(ppqe->querystr);
  appendStringInfo(ppqe->querystr, "%s", (*query_string)->data);
  elog(DEBUG1, "Appended querystr...\n");
  MemoryContextSwitchTo(*oldcontext);
  elog(DEBUG1,"Switched back mem cxt...\n");
}

void create_FEagg_node_in_parse_context (MemoryContext parse_context,
					 Plan** pfePlan,
					 List** pwl,
					 Query** pquerytree,
					 Plan** pbePlan) {
  MemoryContext makeFAgg = MemoryContextSwitchTo(parse_context);
  FAgg * fagg =  makeNode(FAgg);

  fagg->aggPlan = copyObject(*pfePlan);

  *pfePlan = NULL;
  foreach(*pwl, (*pquerytree)->windowClause->windowExprs)
  {
    WindowExpr *we = (WindowExpr *) lfirst(*pwl);
    bool found = adjustVarWithTlist(we->tsvar,(*pbePlan)->targetlist); 
    Assert(found);
  }
  fagg->fjord.plan.targetlist = copyObject((*pbePlan)->targetlist);

  /* We don't add the window expressions to the FAgg right now, because they
   * haven't been through the query rewriter. */
  /*FAggAddWindowExprs(fagg, (*pquerytree)->windowClause->windowExprs);*/

  add_planitem_to_eddy((Eddy*)(*pbePlan), (Plan*)fagg, EDDY_MODULE);
  MemoryContextSwitchTo(makeFAgg);
}

void add_pqe_to_pqe_list_in_parse_context (MemoryContext parse_context,
					   planqentry *ppqe,
					   List** ppqe_list) {
  MemoryContext o = MemoryContextSwitchTo(parse_context);
  planqentry *pqetmp = palloc(sizeof(planqentry));
  memset(pqetmp, 0, sizeof(planqentry));
  *pqetmp = *ppqe;
  *ppqe_list = lcons(pqetmp, *ppqe_list);
  MemoryContextSwitchTo(o);
}
