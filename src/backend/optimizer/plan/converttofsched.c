/*-------------------------------------------------------------------------
 *
 * converttofsched.c
 *
 *	  Routines to convert a plan into one run by the scheduler.
 *	  Eventually will break the plan into segments at the Flux points.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/converttofsched.c,v 1.15 2005/12/14 20:49:23 phred Exp $
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
#include "optimizer/cost.h"
#include "optimizer/planner.h"
#include "optimizer/clauses.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "rewrite/rewriteHandler.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "storage/proc.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "catalog/pg_class.h"	/* @dtcqpmMS */
#include "rqueue/ingresq.h"		/* @dtcqpmMS */
#include "telegraphcq/wrapch.h" /* @dtcqpmMS */
#include "parser/parsetree.h"	/* @dwchMS	 */
#include "utils/relcache.h"		/* @dwchMS	 */
#include "rqueue/resultq.h"		/* @dfluxMS  */
#include "executor/nodeFluxProd.h"		/* @dfluxMS */
#include "access/heapam.h"		/* @dfluxMS */
#include "optimizer/converttofsched.h"	/* @dfluxMS */
#include "executor/aggHash.h"	/* @dfluxMS */
#include "access/hash.h" /* @dfluxMS */
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

#include "pgstat.h"

Plan	   *pg_convert_to_fsched(Plan *plan, Query *querytree);
static void insert_flux_into_plan(Plan *plan, FSched * f);

/* Vary the seed to get different hash functions */
ub4
calc_composite_key_hash(hash_cols * hcols, TupleTableSlot *slot, ub4 seed)
{

	int			i;
	ub4			hash_value;

	/* int hash_ret; */
	Datum		keyval;
	bool		isNull;
	TupleDesc	tupdesc;
	AttrNumber	att;

	tupdesc = slot->ttc_tupleDescriptor;

/*		hash_value = 0; */

	hash_value = seed;			/* Start with something absurd */

	for (i = 0; i < hcols->numCols; i++)
	{

		att = hcols->keyColIdx[i];


		keyval = heap_getattr(slot->val,
							  att,
							  tupdesc,
							  &isNull);

#ifdef DBG_PRINT
		elog(DEBUG1, "attno: %d attval: %d", att, (int) keyval);
#endif
		if (!isNull)
		{
			hash_value = hashFunc_multiple(keyval,
										   tupdesc->attrs[att - 1]->attlen,
										   tupdesc->attrs[att - 1]->attbyval,
										   hash_value);
/* 			hash_value = hash((ub1 *) & keyval, */
/* 							  tupdesc->attrs[att - 1]->attlen, */
/* 							  hash_value); */
		}
	}

#ifdef DBG_PRINT
	elog(DEBUG1, "hash_value: %lu", hash_value);
#endif

	return hash_value;
}

void
hash_route_init(FluxProd * fp)
{
	/* Nothing to save between invocations */
}


/* Used to determine destination partition of operator */
int
hash_route(FluxProd * fp, TupleTableSlot *slot)
{
	ub4			hash_val;

	hash_val = calc_composite_key_hash((hash_cols *) fp->rstate,
									   slot, 57);

	return (hash_val % fp->nCons);
}

void
setup_hash_route(Plan *cons, Plan *prod, FluxProd * fp)
{
	hash_cols *hcols;

	elog(DEBUG1, "Setting up hash_route: cons: %d, prod: %d",
		 nodeTag(cons), nodeTag(prod));
	
	fp->route_fun_init = hash_route_init;
	fp->route_fun = hash_route;
	fp->rstate = palloc(sizeof(hash_cols));

	hcols = (hash_cols *) fp->rstate;

	switch (nodeTag(cons))
	{
		case (T_HashGroup):
			hcols->numCols   = ((HashGroup *) cons)->agg.numCols;
			hcols->keyColIdx = ((HashGroup *) cons)->agg.grpColIdx;
			break;

		default:
			elog(ERROR, "Don't know how to assign the hash-based routing function");
			break;
	}
}

typedef struct rrstate
{

	int			next;
	int			total;

}	rrstate;

int
fixed_mod_route(FluxProd *fp, TupleTableSlot *slot) 
{
	return ( *((int *) slot) % fp->nCons );
}

int
round_robin_route(FluxProd *fp, TupleTableSlot *slot)
{
	rrstate    *rrs = (rrstate *) fp->rstate;

	rrs->next = (rrs->next + 1) % rrs->total;

	return rrs->next;
}

void
round_robin_route_init(FluxProd * fp)
{
	rrstate    *rrs;

	fp->rstate = palloc(sizeof(rrstate));
	rrs = (rrstate *) fp->rstate;
	rrs->next = -1;
	rrs->total = fp->nCons;
}

void
init_map_c2m(int **c2m, int size)
{
	int			i;

	(*c2m) = palloc(sizeof(int) * size);
	for (i = 0; i < size; i++)
		(*c2m)[i] = i;
}

static void
insert_flux_into_plan(Plan *plan, FSched * f)
{
	hash_cols  *hcols;
	FluxCons   *fcons;
	FluxProd   *fprod;
	StrmScan   *ascan;
	Queue	   *local_q;
	FluxAcceptor *faccept;

	f->kill_fsched = false;
	f->numSegs = 3;
	f->nfluxen = f->numSegs - 1;

	f->segs = (Plan **) palloc(sizeof(Plan *) * (f->numSegs));
	f->fluxen = (Plan **) palloc(sizeof(Plan *) * (f->nfluxen));

	/* Right now, there is just a hash-group and a scan */
	fcons = makeNode(FluxCons);
	fprod = makeNode(FluxProd);

	ascan = (StrmScan *) outerPlan(plan);

	/* Look out for the error condition */
	if (!IsA(ascan, StrmScan) ||
		!IsA(plan, HashGroup))
	{
		elog(DEBUG1, "Cannot convert this to FSched plan yet");
		f->numSegs = 0;
		return;
	}

	fcons->sched = f;

	faccept = makeNode(FluxAcceptor);
	f->faccept = faccept;

	fcons->plan.targetlist = ascan->scansock.plan.targetlist;
	fprod->plan.targetlist = ascan->scansock.plan.targetlist;

	fcons->tupDesc = CreateTupleDescCopy((ascan->scansock).tupDesc);

	outerPlan(plan) = (Plan *) fcons;
	outerPlan(fprod) = (Plan *) ascan;

	make_queue(&local_q, *p_no_lock, flux_queue_size);
	fcons->local_q = local_q;
	fprod->local_q = local_q;

	fcons->all_done = false;
	fprod->all_done = false;

	fcons->nProd = total_machines;
	fprod->nCons = total_machines;

	init_map_c2m(&(fcons->map_c2m), fcons->nProd);
	init_map_c2m(&(fprod->map_c2m), fprod->nCons);

	fprod->route_fun_init = hash_route_init;
	fprod->route_fun = hash_route;
	fprod->rstate = palloc(sizeof(hash_cols));

	hcols = (hash_cols *) fprod->rstate;
	hcols->numCols = ((HashGroup *) plan)->agg.numCols;
	hcols->keyColIdx = ((HashGroup *) plan)->agg.grpColIdx;

	/* We need to set the state for the hashing function */

	/* Two dataflow segments, the top level and the bottom level */
	(f->topSeg) = plan;
	(f->segs)[0] = (Plan *) plan;
	(f->segs)[1] = (Plan *) fprod;
	(f->segs)[2] = (Plan *) faccept;

	(f->fluxen)[0] = (Plan *) fcons;
	(f->fluxen)[1] = (Plan *) fprod;

	fcons->node_id = telegraph_nodeid;
	fcons->fprod_idx = 1;
	fcons->flux_idx = 0;
	fprod->node_id = telegraph_nodeid;
	fprod->fcons_idx = 0;
	fprod->flux_idx = 1;

}

Plan *
pg_convert_to_fsched(Plan *plan, Query *querytree)
{
	Plan	   *result;
	FSched	   *f;

	elog(DEBUG1, "Folding plan into fsched.");

	/* Get the streamers in there. */
	result = pg_convert_seqscan_to_strmscan(plan, querytree, true);

	/* First convert the group-by aggs into hash-based versions. */
	result = pg_convert_to_hash_group(result, querytree);

	/* Make sure it is our plan that we want to convert */
	if (!IsA(result, HashGroup))
		return result;

	f = makeNode(FSched);

	f->plan.targetlist = plan->targetlist;

	insert_flux_into_plan(result, f);

	return (Plan *) f;
}

#ifdef NOT_USED
/* A hash function I copied from the web */


#define hashsize(n) ((ub4)1<<(n))
#define hashmask(n) (hashsize(n)-1)

/*
--------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.
For every delta with one or two bits set, and the deltas of all three
  high bits or all three low bits, whether the original value of a,b,c
  is almost all zero or is uniformly distributed,
  * If mix() is run forward or backward, at least 32 bits in a,b,c
  have at least 1/4 probability of changing.
  * If mix() is run forward, every bit of c will change between 1/3 and
  2/3 of the time.	(Well, 22/100 and 78/100 for some 2-bit deltas.)
mix() was built out of 36 single-cycle latency instructions in a
  structure that could supported 2x parallelism, like so:
	  a -= b;
	  a -= c; x = (c>>13);
	  b -= c; a ^= x;
	  b -= a; x = (a<<8);
	  c -= a; b ^= x;
	  c -= b; x = (b>>13);
	  ...
  Unfortunately, superscalar Pentiums and Sparcs can't take advantage
  of that parallelism.	They've also turned some of those single-cycle
  latency instructions into multi-cycle latency instructions.  Still,
  this is the fastest good hash I could find.  There were about 2^^68
  to choose from.  I only looked at a billion or so.
--------------------------------------------------------------------
*/
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);	\
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k		  : the key (the unaligned variable-length array of bytes)
  len	  : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 6*len+35 instructions.

The best hash table sizes are powers of 2.	There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.	For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (ub1 **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);

By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

See http://burtleburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/


ub4
hash(k, length, initval)
register ub1 *k;				/* the key */
register ub4 length;			/* the length of the key */
register ub4 initval;			/* the previous hash, or an arbitrary
								 * value */
{
	register ub4 a,
				b,
				c,
				len;

	/* Set up the internal state */
	len = length;
	a = b = 0x9e3779b9;			/* the golden ratio; an arbitrary value */
	c = initval;				/* the previous hash value */

	/*---------------------------------------- handle most of the key */
	while (len >= 12)
	{
		a += (k[0] + ((ub4) k[1] << 8) + ((ub4) k[2] << 16) + ((ub4) k[3] << 24));
		b += (k[4] + ((ub4) k[5] << 8) + ((ub4) k[6] << 16) + ((ub4) k[7] << 24));
		c += (k[8] + ((ub4) k[9] << 8) + ((ub4) k[10] << 16) + ((ub4) k[11] << 24));
		mix(a, b, c);
		k += 12;
		len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += length;
	switch (len)				/* all the case statements fall through */
	{
		case 11:
			c += ((ub4) k[10] << 24);
		case 10:
			c += ((ub4) k[9] << 16);
		case 9:
			c += ((ub4) k[8] << 8);
			/* the first byte of c is reserved for the length */
		case 8:
			b += ((ub4) k[7] << 24);
		case 7:
			b += ((ub4) k[6] << 16);
		case 6:
			b += ((ub4) k[5] << 8);
		case 5:
			b += k[4];
		case 4:
			a += ((ub4) k[3] << 24);
		case 3:
			a += ((ub4) k[2] << 16);
		case 2:
			a += ((ub4) k[1] << 8);
		case 1:
			a += k[0];
			/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}

#endif

/* ----------------------------------------------------------------
 *		hashFunc_multiple
 *
 *		the hash function for routing and hash-based group-bys
 *
 *      Calculates a hash for multiple columns when the previous
 *      hash is fed back in.
 * ----------------------------------------------------------------
 */
uint32
hashFunc_multiple(Datum key, int typLen, bool byVal, uint32 prev_hash)
{
	unsigned char *k;

	if (byVal)
	{
		/*
		 * If it's a by-value data type, just hash the whole Datum value.
		 * This assumes that datatypes narrower than Datum are
		 * consistently padded (either zero-extended or sign-extended, but
		 * not random bits) to fill Datum; see the XXXGetDatum macros in
		 * postgres.h. NOTE: it would not work to do hash_any(&key, len)
		 * since this would get the wrong bytes on a big-endian machine.
		 */
		k = (unsigned char *) &key;
		typLen = sizeof(Datum);
	}
	else
	{
		if (typLen > 0)
		{
			/* fixed-width pass-by-reference type */
			k = (unsigned char *) DatumGetPointer(key);
		}
		else if (typLen == -1)
		{
			/*
			 * It's a varlena type, so 'key' points to a "struct varlena".
			 * NOTE: VARSIZE returns the "real" data length plus the
			 * sizeof the "vl_len" attribute of varlena (the length
			 * information). 'key' points to the beginning of the varlena
			 * struct, so we have to use "VARDATA" to find the beginning
			 * of the "real" data.	Also, we have to be careful to detoast
			 * the datum if it's toasted.  (We don't worry about freeing
			 * the detoasted copy; that happens for free when the
			 * per-tuple memory context is reset in ExecHashGetBucket.)
			 */
			struct varlena *vkey = PG_DETOAST_DATUM(key);

			typLen = VARSIZE(vkey) - VARHDRSZ;
			k = (unsigned char *) VARDATA(vkey);
		}
		else if (typLen == -2)
		{
			/* It's a null-terminated C string */
			typLen = strlen(DatumGetCString(key)) + 1;
			k = (unsigned char *) DatumGetPointer(key);
		}
		else
		{
			elog(ERROR, "hashFunc: Invalid typLen %d", typLen);
			k = NULL;			/* keep compiler quiet */
		}
	}

	return DatumGetUInt32(hash_any_multiple(k, typLen, prev_hash));
}
