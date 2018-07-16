#ifndef WAVELET_H
#define WAVELET_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <assert.h>

#define DOTOAST
#define NORMALIZE
#define RENDER_LOG
//#define HEAP_GENERATE_EXCEPTIONS
//#define SIMPLERENDER
//#define DUMPCOEF

#ifdef STANDALONE

#define FILEOUT

#include <stdarg.h>

#define LOG 0

void elog(int lev, const char *fmt, ...);
void PP(const char *fmt, ...);
void PL(const char *fmt, ...);
extern FILE* f;

#ifndef WIN32
typedef char bool;
#define true    ((bool) 1)
#define false   ((bool) 0)
#endif

// this structure is not used
typedef struct mhist {
	int dummy;
} mhist;

typedef struct HeapTupleData {
	double* values;
	//uint32 t_len; /* length of *t_data */
	//ItemPointerData t_self; /* SelfItemPointer */
	//Oid t_tableOid; /* table the tuple came from */
	//MemoryContext t_datamcxt; /* memory context of allocation */
	//HeapTupleHeader t_data; /* -> tuple header and data */
} HeapTupleData;
typedef HeapTupleData *HeapTuple;

typedef struct tupleDesc {
    int natts; /* Number of attributes in the tuple */
    //Form_pg_attribute *attrs; /* attrs[N] is a pointer to the description of Attribute Number N+1.  */
    //TupleConstr *constr;
    //bool tdhasoid; /* Tuple has oid attribute in its header */
} *TupleDesc;

typedef char* Pointer;
//typedef unsigned long long Datum;
struct WCS;
typedef WCS* Datum;

#define pfree(X) free(X)
#define Assert(X) assert(X)
#define strncasecmp(a,b,c) _strnicmp(a,b,c)

extern void test0();
extern void test1();
extern void test2();
extern void test3();
extern void test4();
extern void test5();
extern void test6();
extern void test7();
extern void test8();
extern void test9();
extern void test10();



#else /////////////////////////////////////////////////////

#define RENDER_HIST
#define STDERR

// BEFORE CLIENT

//#include "postgres.h"
//#include "access/hash.h"
//#include "executor/spi.h"
//#include "funcapi.h"
//#include "lib/stringinfo.h"
//#include "utils/builtins.h"
//#include "utils/elog.h"
//#include "utils/relcache.h"
//#include "utils/lsyscache.h"
////#include "access/tupdesc.h"
////#include "access/htup.h"

// AFTER CLIENT

#include "postgres.h"
//#include "access/tupdesc.h"
#include "access/htup.h"

// END CLIENT

extern Datum wcs_in(PG_FUNCTION_ARGS);
extern Datum wcs_out(PG_FUNCTION_ARGS);
extern Datum wcs_selection(PG_FUNCTION_ARGS);
extern Datum wcs_projection(PG_FUNCTION_ARGS);
extern Datum wcs_equijoin(PG_FUNCTION_ARGS);
extern Datum wcs_union(PG_FUNCTION_ARGS);
extern Datum wcs_to_mhist(PG_FUNCTION_ARGS);
extern Datum get_cached_wcs(PG_FUNCTION_ARGS);



#endif



#define PointerGetDatum(X) ((Datum) (X))
#define DatumGetPointer(X) ((Pointer) (X))
#define DatumGetWcsP(X) ((WCS *) DatumGetPointer(X))
#define WcsPGetDatum(X) PointerGetDatum(X)
#define PG_RETURN_WCS_P(wcs) return WcsPGetDatum(wcs)
#define PG_DETOAST_DATUM(datum) pg_detoast_datum((struct varlena *) DatumGetPointer(datum))
#define PG_GETARG_WCS_P(n) DatumGetWcsP(PG_DETOAST_DATUM(PG_GETARG_DATUM(n)))

//void* pcalloc(size_t c, size_t s);

//void* pcalloc(size_t count, size_t size) {
//	char* ptr;
//	size_t total = count * size;
//	for (ptr = (char*) palloc(total); ptr < ptr + total; ptr++) {
//		*ptr = (char) 0;
//	}
//	return (void*) ptr;
//}

// multi-dimensional array

typedef struct MDA {
	int d; // dimension count
	int t; // total # elts
	double* a; // the data array
	int m; // len = 2^m
} MDA;

//typedef struct DN DN;
//typedef struct WC WC;
//typedef struct WCS WCS;
//typedef struct LL LL;
//typedef struct LLN LLN;
//typedef struct PQN PQN;
//typedef struct PQ PQ;

struct DN;
struct WC;
struct WCS;
struct LL;
struct LLN;
struct PQN;
struct PQ;

//////////////////////////////////////////////////////////////////////////////

// linked list stuff

#define LL_ITERATE(ll, lln) for (lln = LL_next(ll, NULL); lln != NULL; lln = LL_next(ll, lln))
#define LL_ITERATE2(ll1, lln1, ll2, lln2) for \
	(lln1 = LL_next(ll1, NULL), \
	lln2 = LL_next(ll2, NULL); \
	lln1 != NULL && lln2 != NULL; \
	lln1 = LL_next(ll1, lln1), lln2 = LL_next(ll2, lln2))

typedef struct LL {
	int count;
	struct LLN* first;
	struct LLN* last;
} LL;

typedef struct LLN {
	struct LLN* prev;
	struct LLN* next;
	void* value;
} LLN;

//#define DN_ITERATE(wc, lln, dn) LL_ITERATE(wc->dims, lln) { DN* dn = (DN*) lln->value;

typedef struct DN {
	// rl and rh of all dimensions define the support hyper-rectangle of this wavelet coefficient.
	// This is basically the region that the coefficient's magnitude has a weight on.
	int rl; // boundary low
	int rh; // boundary high

	// This is the direction of the weight of the coefficient's magnitude in the support.
	// It's either +1 or -1 for + and -, respectively.
	int sl; // sign low
	int sh; // sign high
	
	// This is the point in the support at which the sign changes from sl to sh.
	int sc; // sign change
	
	// This is actually unneeded, but it was useful for debugging purposes.
	// It will probably be removed.
	int c; // coordinate
} DN;

// wavelet coefficient
typedef struct WC {
	bool e; // enabled
	double v; // magnitude
#ifdef NORMALIZE
	double vv; // normalized magnitude
#endif
	struct DN** a; // array of dimensions
	struct LL dims; // list of DNs
} WC;

// pair of wavelet coefficient and an integer.
// used only for rendering purposes.
typedef struct WCP {
	int key;
	WC* wc;
} WCP;

#define WAVELET_NOT_A_DIMID -1
#define WAVELET_COL_NAME_LEN 32

// priority queue stuff

#define PQ_ITERATE(pq, i, p) i = 1; \
	for (p = (pq)->nodes[1]; i < (pq)->count + 1; p = (pq)->nodes[++i])

// 
typedef struct PQ {
	int maxCount;
	int count; // this is the number of occupied slots in the PQ, not the number of enabled nodes
	void** nodes;
	int (*compare)(void*, void*);
} PQ;

#define MAX_NAME_LEN 64

// synopsis structure
typedef struct WCS {
	int length;
	int m; // 2^m = resolution per resolution
	PQ coefs;
	LL dims; // list of DIs
	char heap[1]; // TODO unhack
	// heap is basically where all related structures are serialized onto
//	char names[1][MAX_NAME_LEN]; // TODO unhack
} WCS;

// dim info
typedef struct DI {
	char name[MAX_NAME_LEN];
	double min; // lower bound of the lowest bucket in the joint frequency distribution
	double max; // lower bound of the highest bucket in the joint frequency distribution
} DI;

typedef struct wcs_stream_state {
	//wavelet *wavelet;
	WCS *wcs;
    int maxCount;
    int res;
    int* dimLo;
    int* dimHi;
} wcs_stream_state;

//extern WCS* WCS_detoast(WCS* wcs);

#include "telegraphcq/shedding.h"

/* The following functions are externally visible so that the code in
 * shedding.c can construct WCS summaries. */
extern WCS* wcs_create_stream(generic_stream_state* state, bool doLoad);
extern WCS* real_wcs_union(WCS* wcs1, WCS* wcs2);

#endif // WAVELET_H

#ifndef max
#define max(a, b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b)    (((a) < (b)) ? (a) : (b))
#endif
