// QUESTIONS
// TODO handle joining non-identical dims?
// TODO select should trim cells? (i.e., partial cell inclusion?)
// TODO handle identical dim names?
// TODO how to get rid of blanket noise?

// TASKS
// TODO change wcs_in to take in string
// TODO move all decls to top
// TODO change // ... to /* ... */
// TODO add checks / assertions on integrity of wcs (as fred did for hist)
// TODO handle multiple join, proj dimensions at a time (did I already do this? I forget....)
// TODO change dims to arrays, rename vars, remove the need for toasting, work with giant structure directly!
// TODO add profiling calls
// TODO speed up equijoin
// TODO pfree stuff

#ifdef STANDALONE

#include "wavelet.h"

#define MAX_LINE 256
#define MAX_PATH 256

char baseDirPath[MAX_PATH];

#ifdef FILEOUT
FILE* outputFile = NULL;
#endif

void elog(int lev, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vprintf(fmt, argp);
	va_end(argp);
	printf("\n");
}

void* palloc(size_t c) {
	Assert( c != 0 );
	return malloc(c);
}

double get_double_field_val(HeapTuple tup, TupleDesc desc, int fnum, bool skip_ts) {
	return tup->values[fnum - 1];
}

#else

// AFTER CLIENT
#include "postgres.h"
#include "access/hash.h"
#include "catalog/namespace.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "lib/stringinfo.h"
#include "utils/builtins.h"
#include "utils/elog.h"
#include "utils/relcache.h"
#include "utils/lsyscache.h"
// END CLIENT

#include "telegraphcq/histogram.h"
#include "telegraphcq/wavelet.h"
#include "telegraphcq/shedding.h"

/*******************************************************************************
 * PROTOTYPES OF INTERNAL FUNCTIONS
 ******************************************************************************/
double getSegSize(WCS* wcs, DI* di);
double unseg(WCS* wcs, DI* di, int index);
int getSeg(WCS* wcs, DI* di, double value);
int getSegToLeft(WCS* wcs, DI* di, double value);
void* walloc(size_t c);
void* wcalloc(size_t c, size_t s);
void PI();
void PUI();
void PL(const char *fmt, ...);
void PP(const char *fmt, ...);
void init(MDA* ar, int d, int m);
void fill(MDA* mda, double v);
double get(MDA* a, int* i);
void set(MDA* a, int* i, double v);
MDA* copy(MDA* a);
void LLN_init(LLN* lln, void* value);
void LL_init(LL* ll);
void LL_add(LL* ll, LLN* lln);
void LL_remove(LL* ll, LLN* lln);
LLN* LL_next(LL* ll, LLN* lln);
bool LL_contains(LL* ll, void* o);
void PQ_init(PQ* pq, int maxCount, int (*compare)(void*, void*));
bool PQ_add(PQ* pq, void* object);
void* PQ_getMin(PQ* pq);
void PQ_perc_down(PQ* pq, int hole);
void* PQ_removeMin(PQ* pq);
bool PQ_isFull(PQ* pq);
void WC_init(WC* wc, double v);
void WC_add(WC* wc, DN* dn);
void WCP_init(WCP* wcp, int key, WC* wc);
void DI_init(DI* di, const char* name, const double min, const double max);
int WC_compare(void* left, void* right);
void WCS_init(WCS* wcs, int maxCount);
void WCS_addDim(WCS* wcs, const char* name, const double min, const double max);
int WCS_getDimX(WCS* wcs, char* name, DI** di_in);
int WCS_getDim(WCS* wcs, char* name);
void WCS_add(WCS* wcs, WC* wc);
char* serialize(char* dst, void* src, int size);
WCS* WCS_toast(WCS* wcs);
WCS* WCS_detoast(WCS* wcs);
void WCS_print(WCS* wcs);
void print_array(MDA* array);
void decomposition_step(MDA* array, int len, int d, int* ndx);
void recur(MDA* array, int len, int d, int* ndx, int l);
void nonstandard_decomposition(MDA* array);
double tsubrecur(MDA* a, int* andx, MDA* b, int* bndx, int c, int s, double v);
void trecur(MDA* a, int* andx, MDA* b, int* bndx, int c);
MDA* transhelper(MDA* a, int* andx);
MDA* trans(MDA* a);
int crecur(MDA* a, int* ndx, WCS* wcs, int l, MDA* t, int c, int ti);
void drecur(MDA* a, MDA* t, int* ndx, int* tndx, WCS* wcs, int l, int c);
int splitComma(char* dimNamesRaw, char*** pdimNames);
WCS* selectionhelper(WCS* wcs, double* l, double* h);
Datum real_WCS_selection(WCS* wcs, char* ls, char* hs);
Datum real_WCS_projection(WCS* wcs, char* dimNamesRawUnsafe);
WCS* equijoin(WCS* wcs1, WCS* wcs2, char* dimNamesRawUnsafe, int maxCount, int res, bool isUnion);
double get_double_field_val(HeapTuple tup, TupleDesc desc, int fnum, bool skip_ts);
void PI(void);
void PUI(void);
int WCP_compare(void* left, void* right);
void renderhelper(WCS* wcs, LL* coeff, int i, double** c, mhist* h);
void srrecur(WCS* wcs, MDA* ar, int* ndx, int l);
void simpleRender(WCS* wcs);
mhist* render(WCS* wcs);
int INT_compare(void* a, void* b);
WCS* real_wcs_in_helper(char* tabName, char** colNameArr, int colCount, int maxCount, int res, HeapTuple* tuples, int ntuples, TupleDesc descdest, bool doLoad, MDA* mda, int* dimLo, int* dimHi);
Datum real_wcs_in(char* tabName, char* colNamesUnsafe, char* colMinStrUnsafe, char* colMaxStrUnsafe, int maxCount, int res);


// TODO check
double get_double_field_val(HeapTuple tup, TupleDesc desc, int fnum, bool skip_ts) {
    Datum d;
    float64data val = -1;
    bool is_null = false;
    Oid typoid;
    /*int16 typlen = -1;*/
    /*bool typbyval = false;*/

    Assert(tup != NULL);
    Assert(desc != NULL);

    /* We skip over the timestamp column, if desired. */
    if (skip_ts) {
        fnum = get_attr_id_skipping_ts(fnum, desc);
    }
                
    /* Fetch the value of the tuple along this dimension. */
    /* TODO: Should we be using fastgetattr()? */
    d = heap_getattr(tup, fnum, desc, &is_null);

    Assert(!is_null);

    typoid = desc->attrs[fnum - 1]->atttypid;

    /*get_typlenbyval(typoid, &typlen, &typbyval);*/

    /*elog(LOG, "get_double_field_val(): Got type %d", typoid);*/

    switch(typoid) {
        case INT4OID:
            val = (double)DatumGetInt32(d);
            break;

        case FLOAT8OID:
            val = (double)DatumGetFloat8(d);
            break;

        case FLOAT4OID:
            val = (double)DatumGetFloat4(d);
            break;

        default:
			elog(ERROR, 
                "YANG: get_double_field_val(): Got invalid field type %d.",
                typoid);
             
    }
#if 0
    switch(typlen) {
        case 4:
            /* 8-bit integer... */
            val = DatumGetFloat4(d);
            break;

        case 8:
            val = DatumGetFloat8(d);
            break;

        default:
			elog(ERROR, "YANG: get_double_field_val(): Got invalid field length %d.",
                    typlen);
            break;
    }
#endif /* 0 */
    return val;
}

// TODO check
static HeapTuple *get_table_as_array(char *relname, char **cols, int ncols,
        int *ntuples, TupleDesc *descdest) {
	StringInfo	sql = makeStringInfo();
	int			dimid = -1, return_code = -1, proc = -1;
	int			tupnum = -1;

    HeapTuple   *ret = NULL;
        /* Will be returned. */

    MemoryContext main_context;
    MemoryContext oldcontext;

    Assert(ntuples != NULL);


    /* First, we snarf all the data out of the table.  At some point in the
     * future, we will probably do some sampling here, but for now, we just
     * snarf.
     *
     * In order to get at the tuples, we create a query, namely:
     *
     *      SELECT col1, col2, ... , coln FROM relation;
     */
	appendStringInfo(sql, "SELECT ");

	for (dimid = 0; dimid < ncols; dimid++) {

		appendStringInfo(sql, "%s.%s", relname, cols[dimid]);
        if (dimid < ncols - 1) {
            appendStringInfo(sql, ", ");
        }
	}
	appendStringInfo(sql, " FROM %s" , relname);

	elog(LOG, "get_table_as_array(): Query is '%s'.", sql->data);

    /* The Server Programming Interface is going to switch memory contexts
     * behind our back, so we need to remember what the original context was.
     */
    main_context = CurrentMemoryContext;

	/* Connect to the Server Programming Interface (see Postgresql Programmer's
	 * Reference, Chapter 17). */
	if ((return_code = SPI_connect()) < 0) {
		elog(ERROR, "get_table_as_array(): SPI_connect returned %d.", 
                return_code);
	}

	/* Execute the query. */
	SetQuerySnapshot();		/* Don't know why we need to do this. */
	return_code = SPI_exec(sql->data, 0);
		/* The 0 here for number of tuples to retrieve apparently means to get
		 * as many tuples as possible. */
	proc = SPI_processed;

	elog(LOG, "get_table_as_array(): Query generated %d tuples.", proc);

    /* Make a copy of the tuple descriptor.  See note above about memory
     * contexts... */
    oldcontext = MemoryContextSwitchTo(main_context);
    *descdest = CreateTupleDescCopy(SPI_tuptable->tupdesc); 
    MemoryContextSwitchTo(oldcontext);


    /* SPECIAL CASE: Empty result. */
    if (0 == proc) {
        *ntuples = 0;
        
        /* The caller should free the returned pointer unconditionally, so we
         * need to allocate _something_. */
        /* See note above about memory contexts. */
        oldcontext = MemoryContextSwitchTo(main_context);
        ret = palloc(1);
        MemoryContextSwitchTo(oldcontext);

        goto cleanup;
    }
    /* END SPECIAL CASE */

	/* Fetch the result tuples and put them into a giant array. */
    *ntuples = proc;

    /* See note above about memory contexts. */
    oldcontext = MemoryContextSwitchTo(main_context);
    ret = palloc(proc * sizeof(HeapTuple));
    MemoryContextSwitchTo(oldcontext);

	for (tupnum = 0; tupnum < proc; tupnum++) {
        HeapTuple   current_tup;

        /* See note above about memory contexts. */
        oldcontext = MemoryContextSwitchTo(main_context);
        current_tup = heap_copytuple(SPI_tuptable->vals[tupnum]);
        MemoryContextSwitchTo(oldcontext);

        ret[tupnum] = current_tup;
	}


cleanup:
	
	/* Disconnect from the Server Programming Interface. */
	if ((return_code = SPI_finish()) != SPI_OK_FINISH) {
		elog(ERROR, "get_table_as_array(): SPI_finish() returned %d.", 
                return_code);
	}

    return ret;
}

#endif

double getSegSize(WCS* wcs, DI* di) {
	double x = (di->max - di->min) / (double) ((1 << wcs->m) - 1);
	return x;
}

double unseg(WCS* wcs, DI* di, int index) {
	double x = ((double) index) * getSegSize(wcs, di) + di->min;
	return x;
}

int getSeg(WCS* wcs, DI* di, double value) {
	int x = (int) (value - di->min) / getSegSize(wcs, di);
	return x;
}

int getSegToLeft(WCS* wcs, DI* di, double value) {
	double x = (value - di->min) / getSegSize(wcs, di);
	if ( (int) x == x ) {
		return (int) x - 1;
	} else {
		return (int) x;
	}
	return (int) x;
}

void* walloc(size_t c) {
	Assert( c < 0x01000000 );
	if (c == 0)
		return NULL;
	else
		return palloc(c);
}

void* wcalloc(size_t c, size_t s) {
	size_t t = c * s;
	size_t i = 0;
	void* q = ((char*) walloc(t));
	char* p = (char*) q;

	for (; i < t; i++, p++) {
		*p = (char) 0;
	}
	return q;
}

int indent = 0;

void PI(void) {
	indent++;
}

void PUI(void) {
	indent--;
}

void PL(const char *fmt, ...) {
	va_list argp;
	int i;
#ifdef STDERR
	fprintf(stderr, "YANG: ");
	for (i = 0; i < indent; i++) {
		fprintf(stderr, "  ");
	}
#endif
#ifdef FILEOUT
	fprintf(outputFile, "YANG: ");
	for (i = 0; i < indent; i++) {
		fprintf(outputFile, "  ");
	}
#endif
	va_start(argp, fmt);
#ifdef STDERR
	vfprintf(stderr, fmt, argp);
#endif
#ifdef FILEOUT
	vfprintf(outputFile, fmt, argp);
#endif
	va_end(argp);
}

void PP(const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
#ifdef STDERR
	vfprintf(stderr, fmt, argp);
#endif
#ifdef FILEOUT
	vfprintf(outputFile, fmt, argp);
#endif
	va_end(argp);
}

static int global_cmpf_dimid = -1;
static TupleDesc global_cmpf_desc = NULL;
static int cmpf(const void * left, const void * right) {
    HeapTuple *left_ptr = (HeapTuple*) left;
    HeapTuple *right_ptr = (HeapTuple*) right;
    double left_val = get_double_field_val(*left_ptr, global_cmpf_desc,
            global_cmpf_dimid + 1, true);
    double right_val = get_double_field_val(*right_ptr, global_cmpf_desc,
            global_cmpf_dimid + 1, true);

    /* We return a number less than zero if left < right */   
    if (left_val < right_val) {
        return -1;
    } else if (left_val == right_val) {
        return 0;
    } else {
        return 1;
    }
}



// initialize an MDA
void init(MDA* ar, int d, int m) {
	ar->d = d;
	ar->m = m;
	ar->t = (int) pow(1 << m, d);
	//ar->a = (double*) walloc((1 << m) * sizeof(double));
	ar->a = (double*) walloc(ar->t * sizeof(double));
}

void fill(MDA* mda, double v) {
	int i;
	for (i = 0; i < mda->t; i++) {
		mda->a[i] = v;
	}
}

// get an element of the MDA
double get(MDA* a, int* i) {
	int j, k, len = 1 << a->m;
	for (k = j = 0; k < a->d; k++) {
		j = j * len + i[k];
	}
//	PP("GET j = %d\n", j);
	Assert (j < a->t);
	return a->a[j];
}

// set an element of the MDA
void set(MDA* a, int* i, double v) {
	int j, k, len = 1 << a->m;
	for (k = j = 0; k < a->d; k++) {
		j = j * len + i[k];
	}
	Assert (j < a->t);
	a->a[j] = v;
}

// copy an MDA
MDA* copy(MDA* a) {
	MDA* b = (MDA*) walloc(sizeof(MDA));
	init(a, a->d, a->m);
	memcpy(b->a, a->a, a->t * sizeof(double));
	return b;
}

//////////////////////////////////////////////////////////////////////////////

void LLN_init(LLN* lln, void* value) {
	lln->value = value;
	lln->prev = NULL;
	lln->next = NULL;
}

void LL_init(LL* ll) {
	ll->count = 0;
	ll->first = 0;
	ll->last = 0;
}

void LL_add(LL* ll, LLN* lln) {
	if (ll->first == NULL) {
		ll->first = lln;
		ll->last = lln;
		lln->prev = NULL;
		lln->next = NULL;
	} else {
		ll->last->next = lln;
		lln->prev = ll->last;
		ll->last = lln;
		lln->next = NULL;
	}
	ll->count++;
}

void LL_remove(LL* ll, LLN* lln) {
	if (lln == NULL) {
		return;
	}
	if (lln->prev != NULL) {
		lln->prev->next = lln->next;
	}
	if (lln->next != NULL) {
		lln->next->prev = lln->prev;
	}
	if (ll->last == lln) {
		ll->last = lln->prev;
	}
	if (ll->first == lln) {
		ll->first = lln->next;
	}
	ll->count--;
}

LLN* LL_next(LL* ll, LLN* lln) {
	if (lln == NULL) {
		return ll->first;
	} else {
		return lln->next;
	}
}

bool LL_contains(LL* ll, void* o) {
	LLN* lln;
	LL_ITERATE(ll, lln) {
		if (lln->value == o) {
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////

void PQ_init(PQ* pq, int maxCount, int (*compare)(void*, void*)) {
	Assert(maxCount > 0);
	pq->maxCount = maxCount;
	pq->count = 0;
	pq->nodes = (void**) wcalloc((maxCount + 1), sizeof(void*));
	pq->compare = compare;
}

bool PQ_add(PQ* pq, void* object) {
	int hole;
	if (pq->count == pq->maxCount) {
		return false;
	}
	pq->count++;
	hole = pq->count;
	pq->nodes[0] = object;
	for ( ; pq->compare(object, pq->nodes[hole/2]) < 0; hole >>= 1) {
		pq->nodes[hole] = pq->nodes[hole/2];
	}
	pq->nodes[hole] = object;
	return true;
}

void* PQ_getMin(PQ* pq) {
	return pq->nodes[1];
}

void PQ_perc_down(PQ* pq, int hole) {
	void* tmp = pq->nodes[hole];
	int child = 0;
	for (; hole * 2 <= pq->count; hole = child) {
		child = hole * 2;
		if (child != pq->count &&
			pq->compare(pq->nodes[child + 1], pq->nodes[child]) < 0)
		{
			child++;
		}
		if (pq->compare(pq->nodes[child], tmp) < 0) {
			pq->nodes[hole] = pq->nodes[child];
		} else {
			break;
		}
	}
	pq->nodes[hole] = tmp;
}

void* PQ_removeMin(PQ* pq) {
	void* min;
	Assert(pq->count > 0);
	min = PQ_getMin(pq);
	if (min == NULL) {
		return NULL;
	}
	pq->nodes[1] = pq->nodes[pq->count];
	pq->count--;
	PQ_perc_down(pq, 1);
	return min;
}

bool PQ_isFull(PQ* pq) {
	return pq->count == pq->maxCount;
}

//////////////////////////////////////////////////////////////////////////////

void WC_init(WC* wc, double v) {
	wc->e = true;
	wc->v = v;
	wc->a = NULL;
	LL_init(&wc->dims);
}

void WC_add(WC* wc, DN* dn) {
	LLN* lln = (LLN*) walloc(sizeof(LLN));
	LLN_init(lln, dn);
	lln->value = dn;
	LL_add(&wc->dims, lln);
}

//////////////////////////////////////////////////////////////////////////////

void WCP_init(WCP* wcp, int key, WC* wc) {
	wcp->key = key;
	wcp->wc = wc;
}

//////////////////////////////////////////////////////////////////////////////

void DI_init(DI* di, const char* name, const double min, const double max) {
	strcpy(di->name, name);
	di->min = min;
	di->max = max;
}

//////////////////////////////////////////////////////////////////////////////

int WC_compare(void* left, void* right) {
#ifdef NORMALIZE
	double l = fabs(((WC*) left)->vv);
	double r = fabs(((WC*) right)->vv);
#else
	double l = fabs(((WC*) left)->v);
	double r = fabs(((WC*) right)->v);
#endif
	if (l < r) {
		return -1;
	} else if (l > r) {
		return 1;
	} else {
		return 0;
	}
}

void WCS_init(WCS* wcs, int maxCount) {
	PQ_init(&wcs->coefs, maxCount, &WC_compare);
	LL_init(&wcs->dims);
}

void WCS_addDim(WCS* wcs, const char* name, const double min, const double max) {
	DI* di = (DI*) walloc(sizeof(DI));

	LLN* lln = (LLN*) walloc(sizeof(LLN));
	DI_init(di, name, min, max);
	LLN_init(lln, di);

	LL_add(&wcs->dims, lln);
}

int WCS_getDimX(WCS* wcs, char* name, DI** di_in) {
	int nameid = 0;
	LLN* lln = NULL;
	LL_ITERATE(&wcs->dims, lln) {
		DI* di = (DI*) lln->value;
		char* cur_name = (char*) di->name;
		if (0 == strncasecmp(cur_name, name, WAVELET_COL_NAME_LEN)) {
			*di_in = di;
			return nameid;
		}
		nameid++;
	}
	return WAVELET_NOT_A_DIMID;
}

int WCS_getDim(WCS* wcs, char* name) {
	DI* di;
	return WCS_getDimX(wcs, name, &di);
}

void WCS_add(WCS* wcs, WC* wc) {
	if (!PQ_add(&wcs->coefs, wc)) {
		PQ_removeMin(&wcs->coefs);
		PQ_add(&wcs->coefs, wc);
	}
}

char* serialize(char* dst, void* src, int size) {
	memcpy(dst, src, size);
	return dst + size;
}

WCS* WCS_toast(WCS* wcs) {
#ifdef DOTOAST
	void* p;
	int i;
	LLN* lln;
	char* retp;

	// TODO check if size correct?
	int len = sizeof(WCS)
		+ wcs->coefs.count * (sizeof(WC) + wcs->dims.count * sizeof(DN))
		+ wcs->dims.count * sizeof(DI) - 4; //  - 4 because already counted 'heap'

	WCS* ret = (WCS*) walloc(len);
	wcs->length = len;
	*ret = *wcs;

	retp = (char*) &ret->heap; // &ret->coefs.wcnodes;
	PQ_ITERATE(&wcs->coefs, i, p) {
		WC* wc = (WC*) p;
		int j = 0;
		//WC* retwc = (WC*) retp;
		//*retwc = *wc;
		retp = serialize(retp, wc, sizeof(WC));
		//ret->coefs.wcnodes[i] = *wc;
		Assert(wc->dims.count != -1);
		LL_ITERATE(&wc->dims, lln) {
			//Assert(j < wcs->dims.count);
			DN* dn = (DN*) lln->value;
			retp = serialize(retp, dn, sizeof(DN));
			//retwc->at[j] = *dn;

			j++;
			if (j == wcs->dims.count)
				break; // WARNING this may be different from wc->dims for disabled coefs
		}
		Assert(j == wcs->dims.count);
		//retp += wclen;
	}
	Assert((char*) retp == (((char*) ret) + (len - wcs->dims.count * sizeof(DI))));

//	retp = ret->names[0];
	//i = 0;
	LL_ITERATE(&wcs->dims, lln) {
		//Assert(i < wcs->dims.count);
		DI* di = (DI*) lln->value;
//		char* s = (char*) di->name;
		//strcpy(wcs->names[i], s);
		//strcpy(retp, s);
		//i++;
		retp = serialize(retp, di, sizeof(DI));
	}
	Assert((char*) retp == ((char*) ret) + len);
	return ret;
#else
	return wcs;
#endif
}

WCS* WCS_detoast(WCS* wcs) {
#ifdef DOTOAST
	int i;
	int dimcount;
	LL* ll;
	char* p = (char*) wcs->heap;
	wcs->coefs.nodes = (void**) walloc((wcs->coefs.count + 1) * sizeof(void*));
	for (i = 0; i < wcs->coefs.count; i++) {
		WC* wc = (WC*) p; //&wcs->coefs.wcnodes[i];
		LL* ll = &wc->dims;
		int j;
		p += sizeof(WC);

		wcs->coefs.nodes[i + 1] = wc;
		dimcount = wc->dims.count;
		Assert(!wc->e || dimcount == wcs->dims.count);
		LL_init(ll);

		for (j = 0; j < wcs->dims.count; j++) {
			LLN* lln = (LLN*) walloc(sizeof(LLN));
			DN* dn = (DN*) p;
			p += sizeof(DN);

			LLN_init(lln, dn);
			LL_add(ll, lln);
		}
	}

	dimcount = wcs->dims.count;
	ll = &wcs->dims;
	LL_init(ll);
//	p = wcs->names[0];
	for (i = 0; i < dimcount; i++) {
		DI* name = (DI*) p;
		LLN* lln = (LLN*) walloc(sizeof(LLN));
		p += sizeof(DI);

		LLN_init(lln, name);
		LL_add(ll, lln);
	}
	return wcs;
#else
	return wcs;
#endif
}

//////////////////////////////////////////////////////////////////////////////

// print wavelet coefficients
void WCS_print(WCS* wcs) {
	int m = wcs->m;
	int d = wcs->dims.count;
	double** a;
	double* aa;
	void* p;
	int i;
	if (d == 1) {
		aa = (double*) wcalloc((1 << m), sizeof(double));
	} else if (d == 2) {
		int i;
		a = (double**) wcalloc((1 << m), sizeof(double*));
		for (i = 0; i < (1 << m); i++) {
			a[i] = (double*) wcalloc((1 << m), sizeof(double));
		}
	}

	PQ_ITERATE(&wcs->coefs, i, p) {
		LLN* lln;
		int j = 0;
		WC* wc = (WC*) p;
		if (!wc->e) {
			continue;
		}

		// print coordinates and magnitude
		PL("[ ");
		LL_ITERATE(&wc->dims, lln) {
			DN* dn = (DN*) lln->value;
			PP("%d ", dn->c);
		}
		PP("]          % 2.3f\n", wc->v);
		PL("-----------------------\n", wc->v);

		// print region boundaries and signs
		LL_ITERATE(&wc->dims, lln) {
			DN* dn = (DN*) lln->value;
//		for (DN* dn = dn_next(wc, NULL); dn != NULL; dn = dn_next(wc, dn), j++) {
			if (dn->rl == dn->sc) {
				PL("  % 3d: % 3d     % 3d\n", j, dn->rl, dn->rh);
			} else {
				PL("  % 3d: % 3d % 3d % 3d\n", j, dn->rl, dn->sc, dn->rh);
			}
			PL("         %s       %s\n", dn->sl > 0 ? "+" : "-", dn->sh > 0 ? "+" : "-");
			j++;
		}
		PL("\n");
		if (d == 1) {
			//int* b = wcalloc((1 << m), sizeof(int));
			//LLN* lln = LL_next(&wc->dims, NULL);
			//DN* dn = (DN*) lln->value;
			//int e = 0;
			//LL_ITERATE(&wc->dims, lln) {
			//	DN* dn = (DN*) lln->value;
			//	for (int j = 0; j < (1 << m); j++) {
			//		int z = 0;
			//		if (j >= dn->rl && j < dn->sc) {
			//			z = dn->sl;
			//		} else if (j >= dn->sc && j <= dn->rh) {
			//			z = dn->sh;
			//		} else {
			//			continue;
			//		}
			//		for (int k = 0; k < (1 << m); k++) {
			//			int x, y;
			//			if (e == 1) {
			//				x = j;
			//				y = k;
			//			} else {
			//				x = k;
			//				y = j;
			//			}
			//			if (y >= rl0 && y <= rh0 &&
			//				x >= rl1 && x <= rh1) {
			//				if (b[y][x] == 0) {
			//					b[y][x] = z;
			//				} else {
			//					b[y][x] *= z;
			//				}
			//			}
			//		}
			//	}
			//}
		} else if (d == 2) {
			int** b;
			int j;
			int rl0, rh0, rl1, rh1, c0, c1;
			int e = 0;
			LLN* lln2;
			b = (int**) wcalloc((1 << m), sizeof(int));
			for (j = 0; j < (1 << m); j++) {
				b[j] = (int*) wcalloc((1 << m), sizeof(int));
			}
			{
				LLN* lln = LL_next(&wc->dims, NULL);
				DN* dn = (DN*) lln->value;
				rl0 = dn->rl;
				rh0 = dn->rh;
				c0 = dn->c;
				lln = LL_next(&wc->dims, lln);
				dn = (DN*) lln->value;
				rl1 = dn->rl;
				rh1 = dn->rh;
				c1 = dn->c;
			}
			if (c0 == 1 && c1 == 1) {
				PP("");
			}
			LL_ITERATE(&wc->dims, lln2) {
				DN* dn = (DN*) lln2->value;
				for (j = 0; j < (1 << m); j++) {
					int z = 0;
					int k;
					if (j >= dn->rl && j < dn->sc) {
						z = dn->sl;
					} else if (j >= dn->sc && j <= dn->rh) {
						z = dn->sh;
					} else {
						continue;
					}
					for (k = 0; k < (1 << m); k++) {
						int x, y;
						if (e == 1) {
							x = j;
							y = k;
						} else {
							x = k;
							y = j;
						}
						if (y >= rl0 && y <= rh0 &&
							x >= rl1 && x <= rh1) {
							if (b[y][x] == 0) {
								b[y][x] = z;
							} else {
								b[y][x] *= z;
							}
						}
					}
				}
				e++;
			}
			for (j = 0; j < (1 << m); j++) {
				int k;
				PL("| ");
				for (k = 0; k < (1 << m); k++) {
					int x = b[j][k];
					char c = ' ';
					if (x != 0) {
						if (x > 0) {
							c = '+';
						} else {
							c = '-';
						}
					}
					PP("%c | ", c);
				}
				PP("\n");
			}
			PL("\n");
			if (c0 >= 0 && c1 >= 0) {
				a[c0][c1] = wc->v;
			} else {
				Assert( c0 == -1 && c1 == -1 );
			}
		}
	}

	if (d == 2) {
		int i;
		PL("\n");
		PL("Reformatted:\n");

		for (i = 0; i < (1 << m); i++) {
			int j;
			PL("");
			for (j = 0; j < (1 << m); j++) {
				PP("% 2.3f ", a[i][j]);
			}
			PP("\n");
		}
	}
	PL("\n");
	PL("\n");
	PL("\n");
}

// d = # dims, l = dim level
void print_array(MDA* array) {
	if (array->d > 3) {
		PL("rank must be < 3!\n");
		PL("\n");
		return;
	} else if (array->d == 3) {
		int len = (1 << array->m);
		int k, j, i;
		for (k = 0; k < len; k++) {
			for (j = 0; j < len; j++) {
				PL("");
				for (i = 0; i < len; i++) {
					int ndx[3] = {k, j, i};
					PP("% 2.3f ", get(array, ndx));
				}
				PP("\n");
			}
			PL("\n");
			PL("\n");
		}
		PL("\n");
		PL("\n");
	} else if (array->d == 2) {
		int len = (1 << array->m);
		int j, i;
		for (j = 0; j < len; j++) {
			PL("");
			for (i = 0; i < len; i++) {
				int ndx[2] = {j, i};
				PP("% 2.3f ", get(array, ndx));
			}
			PP("\n");
		}
		PL("\n");
		PL("\n");
	} else {
		int len = (1 << array->m);
		int i;
		PL("");
		for (i = 0; i < len; i++) {
			int ndx[1] = {i};
			PP("% 2.3f ", get(array, ndx));
		}
		PP("\n");
	}
}

//////////////////////////////////////////////////////////////////////////////

// one step of the decomposition
void decomposition_step(MDA* array, int len, int d, int* ndx) {
	// create A' = A[0..len-1]
//	double ap[len];
	double* ap = (double*) walloc(len * sizeof(double));

	int i;
	for (i = 0; i < len / 2; i++) {
		double left, right;
		// read left and right operand values
		ndx[d] = 2 * i;
		left = get(array, ndx);
		ndx[d] = 2 * i + 1;
		right = get(array, ndx);

		// average: A'[i] = ( A[2*i] + A[2*i + 1] ) / sqrt(2)
		ap[i] = (left + right) / 2; //*** sqrt(2.0);

		// difference (avg - 2nd element): A'[h/2 + i] = ( A[2*i] - A[2*i + 1] ) / sqrt(2)
		ap[len / 2 + i] = (left - right) / 2; //*** sqrt(2.0);
	}

	// copy values back into array
	for (i = 0; i < len; i++) {
		ndx[d] = i;
		set(array, ndx, ap[i]);
	}

	//print_ndx(ndx, array->d, d);
	//PL("step(len = %d, d = %d):\n", len, d);
	//print_array(array);
}

// d = # dims, l = dim level
void recur(MDA* array, int len, int d, int* ndx, int l) {
	int i;
	for (i = 0; i < len; i++) {
		if (l != d) {
			ndx[l] = i;
		}

		if (l == array->d - 1) {
			decomposition_step(array, len, d, ndx);
		} else {
			recur(array, len, d, ndx, l + 1);
		}

		if (l == d) {
			break;
		}
	}
}

// non-standard decomposition for multiple dimensions, unlike decompose()
void nonstandard_decomposition(MDA* array) {
	// A = A ./ sqrt(len)
	int len = 1 << array->m;
//
//	PL("After / 2:\n");
//	for (int i = 0; i < array->t; i++) {
//		//array->a[i] /= sqrt((double) len);
////***		array->a[i] /= len;
//	}
//
//	print_array(array);

	// main part of the transform
//	int ndx[array->d];
	int* ndx = (int*) walloc(array->d * sizeof(int));
	while (len > 1) {
		int d;
		for (d = 0; d < array->d; d++) {
			recur(array, len, d, ndx, 0);
		}
//		print_marker(len);
		len /= 2;
	}
}

//////////////////////////////////////////////////////////////////////////////

// recursive helper for pairwise averaging and differencing
// c = 0, s = 1, v = 0
double tsubrecur(MDA* a, int* andx, MDA* b, int* bndx, int c, int s, double v) {
	if (c == a->d) {
		v += s * get(a, andx);
	} else {
		// andx = [... i ...]
		v = tsubrecur(a, andx, b, bndx, c + 1, s, v);

		if (bndx[c] == 1) {
			s *= -1;
		}
		andx[c]++; // andx = [.. i+1 ..]
		v = tsubrecur(a, andx, b, bndx, c + 1, s, v);
		andx[c]--; // andx = [... i ...]
	}
	return v;
}

// recursive helper for pairwise averaging and differencing
// c = 0
void trecur(MDA* a, int* andx, MDA* b, int* bndx, int c) {
	if (c == b->d) {
		double v = tsubrecur(a, andx, b, bndx, 0, 1, 0) / (1 << a->d);
		set(b, bndx, v);
	} else {
		bndx[c] = 0; // bndx = [... 0 ...]
		trecur(a, andx, b, bndx, c + 1);

		bndx[c] = 1; // bndx = [... 1 ...]
		trecur(a, andx, b, bndx, c + 1);
		bndx[c] = 0; // bndx = [... 0 ...]
	}
}

// perform pairwise averaging and differencing on the given coordinates of MDA a
MDA* transhelper(MDA* a, int* andx) {
	int* bndx = (int*) walloc(a->d * sizeof(int));
	MDA* b = (MDA*) walloc(sizeof(MDA));
	init(b, a->d, 1);
	trecur(a, andx, b, bndx, 0);
	return b;
}

// perform pairwise averaging and differencing on (0, 0, ...) of MDA a
MDA* trans(MDA* a) {
	int* andx = (int*) walloc(a->d * sizeof(int));
	int i;
	for (i = 0; i < a->d; i++) {
		andx[i] = 0;
	}
	return transhelper(a, andx);
}

double compute(MDA*, int*, WCS*, int);

// recursively call computewavelet
// c = 0, ti = 0
int crecur(MDA* a, int* ndx, WCS* wcs, int l, MDA* t, int c, int ti) {
	if (c == a->d) {
		double v = compute(a, ndx, wcs, l + 1);
		t->a[ti++] = v;
	} else {
		int orig = ndx[c];
		ti = crecur(a, ndx, wcs, l, t, c + 1, ti);

		ndx[c] += (1 << (a->m - l - 1));
		ti = crecur(a, ndx, wcs, l, t, c + 1, ti);
		ndx[c] = orig;
	}
	return ti;
}

// recursive helper function for constructing the synopsis
// c = 0
void drecur(MDA* a, MDA* t, int* ndx, int* tndx, WCS* wcs, int l, int c) {
	if (c == t->d) { //***
		double v;
		WC* wc;
		int i;
		WC* minwc;
		double vv;

		if (l > 0) {
			bool orig = true;
			int i;
			for (i = 0; i < t->d; i++) {
				if (tndx[i] != 0) {
					orig = false;
					break;
				}
			}
			if (orig) {
				return; // these will be overwritten
			}
		}

		v = get(t, tndx);
		minwc = (WC*) PQ_getMin(&wcs->coefs);
#ifdef NORMALIZE
		vv = v / sqrt((double) (1 << l));
		if (v == 0 || (PQ_isFull(&wcs->coefs) && minwc != NULL && fabs(vv) < fabs(minwc->vv))) {
#else
		if (v == 0 || (PQ_isFull(&wcs->coefs) && minwc != NULL && fabs(v) < fabs(minwc->v))) {
#endif
//			PL("ignoring(%d %d) %f\n", tndx[0], tndx[1], v);
			return;
		}

		wc = (WC*) walloc(sizeof(WC));
		WC_init(wc, v);
		//PL("%f\t%f\n", v, vv);
#ifdef NORMALIZE
		wc->vv = vv;
#endif
		for (i = 0; i < t->d; i++) {
			DN* dn = (DN*) walloc(sizeof(DN));
			dn->c = tndx[i] * (1 << l) + ndx[i] / (1 << (a->m - l));
//			wc->vv = wc->v;
//			int j;
//			for (j = 0; j < l; j++) {
//				wc->vv /= sqrt((double) (1 << l));
//			}
			dn->rl = ndx[i];
			dn->rh = ndx[i] + (1 << (a->m - l)) - 1;
			dn->sl = 1;
			dn->sh = tndx[i] == 0 ? 1 : -1;
			dn->sc = tndx[i] == 0 ? ndx[i] : ndx[i] + (1 << (a->m - l - 1));
			WC_add(wc, dn);
		}
		WCS_add(wcs, wc);
	} else {
		tndx[c] = 0;
		drecur(a, t, ndx, tndx, wcs, l, c + 1);

		tndx[c] = 1;
		drecur(a, t, ndx, tndx, wcs, l, c + 1);
		tndx[c] = 0;
	}
}

// main decomposition algorithm, aka ComputeWavelet
// l = 0
double compute(MDA* a, int* ndx, WCS* wcs, int l) {
	//PL("compute() start\n");
	if (l >= a->m) {
		return get(a, ndx);
	} else {
		MDA* tp;
		int* tndx;
		//PL("compute() checkpoint 1\n");
		MDA t;
		init(&t, a->d, 1);
		t.a = (double*) walloc((1 << a->d) * sizeof(double));

		//PL("compute() checkpoint 2\n");
		crecur(a, ndx, wcs, l, &t, 0, 0);
		//PL("compute() checkpoint 3\n");
		tp = trans(&t);
		//print_array(tp);

		//PL("compute() checkpoint 4\n");
		tndx = (int*) walloc(t.d * sizeof(int));
		drecur(a, tp, ndx, tndx, wcs, l, 0);

		//PL("compute() checkpoint 5\n");
		return tp->a[0];
	}
	//PL("compute() end\n");
}

// split dimNamesRaw by ',' into array dimNames
int splitComma(char* dimNamesRaw, char*** pdimNames) {
	int dimCount = 1;
	int c;
	int d = 1;
	char** dimNames;
	if (dimNamesRaw[0] == '\0') {
		return 0;
	}
	for (c = 1; dimNamesRaw[c] != '\0'; c++) {
		if (dimNamesRaw[c] == ',') {
			dimCount++;
		}
	}
	dimNames = (char**) walloc(dimCount * sizeof(char*));
	dimNames[0] = &dimNamesRaw[0];
	for (c = 1; d < dimCount; c++) {
		if (dimNamesRaw[c] == ',') {
			dimNamesRaw[c] = '\0';
			dimNames[d] = &dimNamesRaw[c + 1];
			d++;
		}
	}
	*pdimNames = dimNames;
	return dimCount;
}

// l and h correspond to ls and hs in real_WCS_selection
// This function basically iterates through every coefficient
//  and either trims down their supports
//  or gets rid of them altogether if they're completely outside the range.
WCS* selectionhelper(WCS* wcs, double* l, double* h) {
	void* p;
	int j;
	PQ_ITERATE(&wcs->coefs, j, p) {
		bool overlap = true;
		int i = 0;
		LLN* lln;
		LLN* dlln;
		WC* wc = (WC*) p;
		if (!wc->e) {
			continue;
		}
		LL_ITERATE2(&wc->dims, lln, &wcs->dims, dlln) {
			DN* dn = (DN*) lln->value;
			DI* di = (DI*) dlln->value;
			//double lo = unseg(wcs, di, dn->rl);
			//double hi = unseg(wcs, di, dn->rh);
			//if (!(l[i] <= lo && lo <= h[i] ||
			//	lo <= l[i] && l[i] <= hi))
			int lo = getSeg(wcs, di, l[i]);
			int hi = getSegToLeft(wcs, di, h[i]);
			if (!((lo <= dn->rl && dn->rl <= hi) ||
				(dn->rl <= lo && lo <= dn->rh)) || hi < lo) // TODO more elegant way around this 'hi<lo' hack?
			{
				overlap = false;
				break;
			}
			i++;
		}
		if (overlap) {
			int i = 0;
			LL_ITERATE2(&wc->dims, lln, &wcs->dims, dlln) {
				DN* dn = (DN*) lln->value;
				DI* di = (DI*) dlln->value;
				int lo = getSeg(wcs, di, l[i]);
				int hi = getSegToLeft(wcs, di, h[i]);
				dn->rl = max(lo, dn->rl);
				dn->rh = min(hi, dn->rh);
				if (dn->rh < dn->sc) {
					dn->sc = dn->rl;
					dn->sh = dn->sl;
				} else if (dn->rl >= dn->sc) {
					dn->sc = dn->rl;
					dn->sl = dn->sh;
				}
				i++;
			}
		} else {
			wc->e = false;
		}
	}

	return wcs;
}

// TODO fix the pointer/datum issues here (in the returns) & elsewhere
// performs selection in the WCS domain, essentially serving as a wrapper for selectionhelper.
// All this function does is parse the arguments.
// ls is a comma-delimited list of lower-boundaries (float64s) of the selected region.
// rs is a comma-delimited list of upper-boundaries (float64s) of the selected region.
// ls and rs must have the same ordering as the attributes in WCS.
Datum real_WCS_selection(WCS* wcs, char* ls, char* hs) {
	/*PL("real_WCS_selection start\n");*/
	double* l = NULL;
	double* h = NULL;
	char* ss[2] = {ls, hs};
	double* aa[2] = {l, h};
	int k;

	wcs = WCS_detoast(wcs);

	for (k = 0; k < 2; k++) {
		char* s = ss[k];
		double* a = NULL;

		int count = 1;
		int len = strlen(s);
		int i;
		int j;

		bool start = true;

		if (len == 0) {
			return PointerGetDatum(NULL);
		}

		for (i = 0; i < len; i++) {
			if (s[i] == ',') {
				count++;
			}
		}

		a = (double*) walloc(count * sizeof(double));

		for (i = 0, j = 0; i < len && j < count; i++) {
			if (start) {
				a[j++] = atof(s + i);
				start = false;
			} else if (s[i] == ',') {
				start = true;
			}
		}

		aa[k] = a;
	}

	l = aa[0];
	h = aa[1];

	wcs = selectionhelper(wcs, l, h);
	wcs = WCS_toast(wcs);
	PL("real_WCS_selection end\n");
	return WcsPGetDatum(wcs);
}

#ifndef STANDALONE
// wrapper for real_WCS_selection
Datum wcs_selection(PG_FUNCTION_ARGS) {
	WCS* wcs = PG_GETARG_WCS_P(0);
	char* ls = PG_GETARG_CSTRING(1);
	char* hs = PG_GETARG_CSTRING(2);
	PG_RETURN_WCS_P(real_WCS_selection(wcs, ls, hs));
}
#endif

// performs projection in the WCS domain.
// This is accomplished by flattening out the dimensions to be projected out
//  onto the dimensions that are projected in. This basically "undistributes"
//  the magnitude on all the support.
// dimNames is a comma-delimited list of attributes that will be projected in.
// dimCount is the number of dimNames. This will probably be removed later.
Datum real_WCS_projection(WCS* wcs, char* dimNamesRawUnsafe) {
	/*PL("real_WCS_projection start\n");*/
	char** dimNames;
	char* dimNamesRaw;
	int dimCount;
	int i;
	int d;
	bool* inc;
	int j = 0;
	LLN* lln;
	void* p;

	wcs = WCS_detoast(wcs);

	dimNamesRaw = walloc((strlen(dimNamesRawUnsafe) + 1) * sizeof(char));
	strcpy(dimNamesRaw, dimNamesRawUnsafe);

	dimCount = splitComma(dimNamesRaw, &dimNames);

	// create map of dimensions and whether they're projection dimensions
	d = wcs->dims.count;
	inc = (bool*) wcalloc(d, sizeof(bool));
	for (i = 0; i < dimCount; i++) {
		int j = WCS_getDim(wcs, dimNames[i]);
		inc[j] = true;
	}

	// remove dimensions from the WCS
	LL_ITERATE(&wcs->dims, lln) {
		Assert(j < d);
		if (!inc[j]) {
			LL_remove(&wcs->dims, lln);
		}
		j++;
	}

	PQ_ITERATE(&wcs->coefs, i, p) {
		WC* wc = (WC*) p;
		int j = 0;
		LLN* lln;
		if (!wc->e) {
			continue;
		}

		LL_ITERATE(&wc->dims, lln) {
			int p;
			DN* dn = (DN*) lln->value;
			// we don't want to project out this dimension
			// so move onto the next dimension
			if (inc[j++]) {
				continue;
			}
			LL_remove(&wc->dims, lln);
			p = (dn->rh - dn->sc + 1) * dn->sh + (dn->sc - dn->rl) * dn->sl;
			wc->v *= p;
			if (wc->v == 0) {
				wc->e = false;
				continue;
			}
		}
		Assert(wc->dims.count == wcs->dims.count);
	}

	wcs = WCS_toast(wcs);

	/*PL("real_WCS_projection end\n");*/
	return WcsPGetDatum(wcs);
}

#ifndef STANDALONE
// wrapper for real_WCS_projection
Datum wcs_projection(PG_FUNCTION_ARGS) {
	WCS* wcs = PG_GETARG_WCS_P(0);
	char* dimNames = PG_GETARG_CSTRING(1);
	PG_RETURN_WCS_P(real_WCS_projection(wcs, dimNames));
}
#endif // STANDALONE

// Note that this function only keeps the dimension names of the LHS of the =
// expressions in dimNamesRaw.
WCS* equijoin(WCS* wcs1, WCS* wcs2, char* dimNamesRawUnsafe, int maxCount, int res, bool isUnion) {
	char** dimNames;
	int dimCount;
	char* dimNamesRaw = NULL;


	int dimNamesLength;

	// maps dim id's in the WCS to dim names
	int* rmap1;
	int* rmap2;
	// maps dim names to dim id's in the WCS
	DI** map1;
	DI** map2;

	int k;

	int newDimCount;

	WCS* wcs = (WCS*) walloc(sizeof(WCS));

	bool hasEmptyDim = false;

	void* p1;
	int m;

	int i = 0;

    WC *wc;
    LLN* dlln = NULL;

	wcs1 = WCS_detoast(wcs1);
	wcs2 = WCS_detoast(wcs2);

	if (isUnion) {
		LLN* lln;
		int i = 0;
		Assert(dimNamesRawUnsafe == NULL);
		dimCount = wcs1->dims.count;
		dimNames = (char**) walloc(sizeof(char*) * dimCount);
		LL_ITERATE(&wcs1->dims, lln) {
			DI* di = (DI*) lln->value;
			char* name = di->name;
			int len = strnlen(name, WAVELET_COL_NAME_LEN);

			// sanity check
			Assert(len < WAVELET_COL_NAME_LEN);

			dimNames[i] = (char*) walloc(sizeof(char) * (len+1));
			strncpy(dimNames[i], name, WAVELET_COL_NAME_LEN);

			i++;
		}
	} else {
		// make a copy so that we're not changing the string in place
		dimNamesRaw = walloc((strlen(dimNamesRawUnsafe)) + 1 * sizeof(char));
		strcpy(dimNamesRaw, dimNamesRawUnsafe);

		dimCount = splitComma(dimNamesRaw, &dimNames);
	}

	// debug purposes
	dimNamesLength = strnlen(dimNames[0], WAVELET_COL_NAME_LEN);

	// maps dim id's in the WCS to dim names
	rmap1 = (int*) walloc(wcs1->dims.count * sizeof(int));
	rmap2 = (int*) walloc(wcs2->dims.count * sizeof(int));
	// maps dim names to dim id's in the WCS
	if (dimCount == 0) {
		map1 = NULL;
		map2 = NULL;
	} else {
		map1 = (DI**) wcalloc(dimCount, sizeof(DI*));
		map2 = (DI**) wcalloc(dimCount, sizeof(DI*));
	}

	// create maps & reverse maps
	// this is so that we know when we encounter the matching dim in both WCSs
	// reverse maps:
	//   from: position in the WCS's array of DIs (i = pos in wcs->dims)
	//   to: position in the given join dims (j = pos in dimNames)
	// maps:
	//   from: position in the given join dims
	//   to: the DI in the WCS corresponding to that dim
	// PSEUDO CODE
	// for each wcs, k
	//   //if isUnion
	//   //  for each dim1, i in wcs
	//   //    rmap[k][i] = i
	//   //    map[k][i] = i
	//   //    joinDims[i] = i
	//   //    if k = 2
	//   //      assert dim1.name = joinDims[i].name
	//   //else
	//     for each dim1, i in wcs
	//       for each dim2, j in joinDims
	//         if dim1.name = dim2.name (match)
	//           rmap[k][i] = j
	//           map[k][j] = i
	//           (we need to also consider joinDims in the format "col=col"
	//       if no match
	//         rmap[k][i] = -1
	for (k = 0; k < 2; k++) {
		int i = 0;
		WCS* wcs = k == 0 ? wcs1 : wcs2;
		int* rmap = k == 0 ? rmap1 : rmap2;
		DI** map = k == 0 ? map1 : map2;
		LLN* lln;
		int dimsDoneCount = 0;
//		if (isUnion) {
//			LL_ITERATE(&wcs->dims, lln) {
//				DI* di = (DI*) lln->value;
//				char* name = di->name;
//
//				// sanity check
//				Assert(strnlen(name, WAVELET_COL_NAME_LEN) < WAVELET_COL_NAME_LEN);
//
//				rmap[i] = i;
//				map[i] = i;
//				if (k == 0) {
//					dimNames[i] = (char*) walloc(strnlen(name, WAVELET_COL_NAME_LEN));
//					strncpy(dimNames[i], name, WAVELET_COL_NAME_LEN);
//				} else {
//					Assert(k == 1);
//					Assert(0 == strncasecmp(dimNames[i], name, WAVELET_COL_NAME_LEN));
//				}
//			}
//		} else {
			LL_ITERATE(&wcs->dims, lln) {
				DI* di = (DI*) lln->value;
				char* name = di->name;
				int j;
				// sanity check
				Assert(strnlen(name, WAVELET_COL_NAME_LEN) < WAVELET_COL_NAME_LEN);
				rmap[i] = -1;
				if ( dimsDoneCount < dimCount ) {
					for (j = 0; j < dimCount; j++) {
						char* equalChar = strchr(dimNames[j], '=');
						// handle the case where the column names are not the
						// same in both relations
						if (equalChar != NULL) {
							char* dimName = NULL;
							char* dimName1 = dimNames[j];
							char* dimName2 = (equalChar + 1);
							*equalChar = '\0';
							if (k == 0) {
								dimName = dimName1;
							} else {
								Assert(k == 1);
								dimName = dimName2;
							}
							if (0 == strncasecmp(dimName, name, WAVELET_COL_NAME_LEN)) {
								dimsDoneCount ++;
								rmap[i] = j;
								map[j] = di;
								*equalChar = '=';
								break;
							}
							*equalChar = '=';
						}
						if (0 == strncasecmp(dimNames[j], name, WAVELET_COL_NAME_LEN)) {
							dimsDoneCount ++;
							rmap[i] = j;
							map[j] = di;
							break;
						}
					}
				}
				i++;
			}
//		}
	}

	// We should not have modified the length of the string (restore = signs)
	Assert( dimCount == 0 ||
			dimNamesLength == strnlen(dimNames[0], WAVELET_COL_NAME_LEN) );

	// create WCS
	WCS_init(wcs, maxCount);
	wcs->m = res;

	// whether we have non-overlapping join dims
	// first copy over dims from wcs1
	// for each oldDim1, i in wcs1
	//   create newDim
	//   if rmap1[i] != -1 (oldDim1 is a join dim)
	//     oldDim2 = map2[rmap1[i]]
	//     newDim.bounds = overlap of oldDim1, oldDim2
	//   else
	//     newDim = oldDim1
	//   add newDim into wcs
	LL_ITERATE(&wcs1->dims, dlln) {
		DI* di1;
		Assert(i < wcs1->dims.count);
		di1 = (DI*) dlln->value;
		if (rmap1[i] != -1) {
			DI* di2 = map2[rmap1[i]];
			// find overlap
			double rmin = max(di1->min, di2->min);
			double rmax = min(di1->max, di2->max);
			if (rmax <= rmin) {
				rmin = rmax;
				hasEmptyDim = true;
			}
			{
				// TODO is there a safe version of strchr?
				// (also look for other instances of this)
				char* equalChar = strchr(dimNames[rmap1[i]], '=');
				if (equalChar != NULL) {
					*equalChar = '\0';
				}
			}
			WCS_addDim(wcs, dimNames[rmap1[i]], rmin, rmax);
		} else {
			WCS_addDim(wcs, di1->name, di1->min, di1->max);
		}
		i++;
	}

	// then copy over the remaining (non-join) dims from wcs2
	// for each oldDim2 in wcs2
	//   if rmap2[i] != -1 (oldDim2 is a join dim)
	//     (already added, so skip)
	//   else
	//     copy oldDim2 into wcs
	i = 0;
	LL_ITERATE(&wcs2->dims, dlln) {
		DI* di2 = (DI*) dlln->value;
		if (rmap2[i] == -1) {
			WCS_addDim(wcs, di2->name, di2->min, di2->max);
		}
		i++;
	}

	newDimCount = wcs1->dims.count + wcs2->dims.count - dimCount;
	Assert( newDimCount == wcs->dims.count );

	// trivial case
	if ( hasEmptyDim || (wcs1->coefs.count == 0 && wcs2->coefs.count == 0) ||
			(!isUnion && (wcs1->coefs.count == 0 || wcs2->coefs.count == 0))) {
		wcs = WCS_toast(wcs);
		return wcs;
	}

	// semi-trivial case: one of them is empty, so copy over all the coefs from
	// the other one
	if ( isUnion && (wcs1->coefs.count == 0 || wcs2->coefs.count == 0) ) {
		WCS* srcWcs = NULL;
		int m;
		void* p;
		int* rmap = NULL;
		if (wcs1->coefs.count == 0) {
			srcWcs = wcs2;
			rmap = rmap2;
		} else {
			Assert(wcs2->coefs.count == 0);
			srcWcs = wcs1;
			rmap = rmap1;
		}

		// now copy all the coefs
		PQ_ITERATE(&srcWcs->coefs, m, p) {
			WC* srcWc = (WC*) p;
			WC* wc;
			LLN* lln;
			int dimid = 0;
			WC* minwc;

			if (!srcWc->e) {
				continue;
			}

			// copy the wc
			wc = (WC*) walloc(sizeof(WC));
			WC_init(wc, srcWc->v);

			// copy its dims
			LL_ITERATE(&srcWc->dims, lln) {
				Assert(dimid < srcWcs->dims.count);
				if (rmap[dimid] != -1) {
					DN* srcDn = (DN*) lln->value;
					DN* dn = walloc(sizeof(DN));
					*dn = *srcDn;
					WC_add(wc, dn);
				}
				dimid++;
			}

			// add wc to wcs if there's room
			minwc = (WC*) PQ_getMin(&wcs->coefs);
			if (!(PQ_isFull(&wcs->coefs) && minwc != NULL && fabs(wc->v) < fabs(minwc->v))) {
				WCS_add(wcs, wc);
			}
		}

		wcs = WCS_toast(wcs);
		return wcs;
	}

	// let the equijoin begin
	PQ_ITERATE(&wcs1->coefs, m, p1) {
		WC* wc1 = (WC*) p1;
		void* p2;
		int n;
		if (!wc1->e) {
			continue;
		}

		PQ_ITERATE(&wcs2->coefs, n, p2) {
			WC* wc2 = (WC*) p2;
			bool overlap = true;
			int k = 0;
			int dimDoneCount = 0;
			LLN* lln1;
			LLN* dlln1;
			if (!wc2->e) {
				continue;
			}


			// determine whether the coefficients overlap
			// TODO currently O(n^2), make linear by using random access struct
			// (can't use map/rmap because they only deal with DI, not DN)
			LL_ITERATE2(&wc1->dims, lln1, &wcs1->dims, dlln1) {
				int l = 0;
				LLN* lln2;
				LLN* dlln2;
                double lo1;
                double hi1;
                double lo2;
                double hi2;

				if (dimDoneCount >= dimCount) {
					break;
				}

				LL_ITERATE2(&wc2->dims, lln2, &wcs2->dims, dlln2) {
					DI* di1;
					DI* di2;
					DN* dn1;
					DN* dn2;
					if (dimDoneCount >= dimCount) {
						break;
					}
					di1 = (DI*) dlln1->value;
					di2 = (DI*) dlln2->value;
					dn1 = (DN*) lln1->value;
					dn2 = (DN*) lln2->value;
					if (rmap1[k] != rmap2[l] || rmap1[k] == -1 || rmap2[l] == -1) {
						l++;
						continue;
					}

					lo1 = unseg(wcs1, di1, dn1->rl);
					hi1 = unseg(wcs1, di1, dn1->rh + 1); // + 1 bc want full cell span
					lo2 = unseg(wcs2, di2, dn2->rl);
					hi2 = unseg(wcs2, di2, dn2->rh + 1); // ditto

					// bug was here: dn1->rl <= dn2->rl && dn2->rh <= dn1->rh
					if (!((lo1 <= lo2 && lo2 < hi1) || 
                                (lo2 <= lo1 && lo1 < hi2))) 
                    {
						overlap = false;
						dimDoneCount = dimCount; // set for early exit condition
						break;
					}
					l++;
				}
				k++;
			}

			if (!overlap) {
				continue;
			}

			// wc = the new wc
			// for each dn1 in wc1
			//   if dn1 is not a join dim
			//     copy dn1 into wc
			//   if dn1 is a join dim or on last iteration
			//     for each dn2 in wc2
			//       if dn2 is not a join dim AND we're on last iteration
			//         copy dn2 into wc
			//       else if dn2 is a join dim
			//         merge the two dn's
			wc = (WC*) walloc(sizeof(WC));
			WC_init(wc, wc1->v * wc2->v);
			k = 0;
			dimDoneCount = 0;
			/*LLN* lln = NULL;*/
			LL_ITERATE2(&wc1->dims, lln1, &wcs1->dims, dlln1) {
				DI* di1 = (DI*) dlln1->value;
				DN* dn1 = (DN*) lln1->value;
				bool isLastRun = false;
				if (rmap1[k] == -1) {
					/*DI* di = (DI*) LL_next(&wcs->dims, dlln)->value;*/
					DN* dn = (DN*) walloc(sizeof(DN));
					*dn = *dn1;
					dn->c = -1;
					WC_add(wc, dn);
				}
				// we also want to fall through on the last run so that
				// wc2 gets a chance to have its dims added
				if (k == wcs1->dims.count - 1) {
					isLastRun = true;
				}
				if (rmap1[k] != -1 || isLastRun) {
					int l = 0;
					LLN* lln2;
					LLN* dlln2;
					LL_ITERATE2(&wc2->dims, lln2, &wcs2->dims, dlln2) {
						DI* di2 = (DI*) dlln2->value;
						DN* dn2 = (DN*) lln2->value;

						// we only want to add these at the end, after all the dims have been added
						if (isLastRun && rmap2[l] == -1) {
							/* DI* di = (DI*) LL_next(&wcs->dims, dlln)->value;
                             */
							DN* dn = (DN*) walloc(sizeof(DN));
							*dn = *dn2;
							dn->c = -1;
							WC_add(wc, dn);
						} else if (rmap1[k] == rmap2[l] && rmap2[l] != -1) {
							DI* di = (DI*) LL_next(&wcs->dims, dlln)->value;
							DN* dn = (DN*) walloc(sizeof(DN));
							double lo1 = unseg(wcs1, di1, dn1->rl);
							double hi1 = unseg(wcs1, di1, dn1->rh + 1); // + 1 bc want full cell span
							double lo2 = unseg(wcs2, di2, dn2->rl);
							double hi2 = unseg(wcs2, di2, dn2->rh + 1); // ditto
							double lo = max(lo1, lo2);
							double hi = min(hi1, hi2);
							WCS* wcss[2] = {wcs1, wcs2};
							DI* dis[2] = {di1, di2};
							DN* dns[2] = {dn1, dn2};
							int j;
							dn->rl = getSeg(wcs, di, lo);
							dn->rh = getSegToLeft(wcs, di, hi);
							dn->sl = dn->sh = 1;

							for (j = 0; j < 2; j++) {
								double sc = unseg(wcss[j], dis[j], dns[j]->sc);
								if (hi <= sc) {
									dn->sl *= dns[j]->sl;
									dn->sh *= dns[j]->sl;
								} else if (lo >= sc) {
									dn->sl *= dns[j]->sh;
									dn->sh *= dns[j]->sh;
								} else {
									dn->sl *= dns[j]->sl;
									dn->sh *= dns[j]->sh;
								}
							}

							if (dn->sl == dn->sh) {
								dn->sc = dn->rl;
							} else {
								double max = -DBL_MAX;
								for (j = 0; j < 2; j++) {
									double sc = unseg(wcss[j], dis[j], dns[j]->sc);
									if (lo <= sc && sc < hi && sc > max) {
										max = sc;
									}
								}
								dn->sc = getSeg(wcs, di, max);
							}

							Assert( dn->rl <= dn->sc && dn->sc <= dn->rh );
							dimDoneCount++;
							dn->c = -1;
							WC_add(wc, dn);
						}
						l++;
					}
				}
				k++;
			}

            {
			WC* minwc = (WC*) PQ_getMin(&wcs->coefs);
			if (!(PQ_isFull(&wcs->coefs) && minwc != NULL && fabs(wc->v) < fabs(minwc->v))) {
				WCS_add(wcs, wc);
			}
            }
		}
	}

	wcs = WCS_toast(wcs);

	return wcs;
}

#ifndef STANDALONE
Datum wcs_equijoin(PG_FUNCTION_ARGS) {
	WCS* wcs1 = PG_GETARG_WCS_P(0);
	WCS* wcs2 = PG_GETARG_WCS_P(1);
	char* dimNames = PG_GETARG_CSTRING(2);
	int maxCount = PG_GETARG_INT32(3);
	int res = PG_GETARG_INT32(4);
	PG_RETURN_WCS_P(equijoin(wcs1, wcs2, dimNames, maxCount, res, false));
}
#endif

WCS* real_wcs_union(WCS* wcs1, WCS* wcs2) {
	int maxCount = 256;
	int res = 4;
	WCS* srcWcsArray[2];
	WCS* newWcs;

	srcWcsArray[0] = wcs1 = WCS_detoast(wcs1);
	srcWcsArray[1] = wcs2 = WCS_detoast(wcs2);

	// create the new WCS
	newWcs = walloc(sizeof(WCS));
	WCS_init(newWcs, maxCount);
	newWcs->m = res;

	// copy dimension info from one of them
	{
		LLN* lln1;
		LLN* lln2;
		LL_ITERATE2(&wcs1->dims, lln1, &wcs2->dims, lln2) {
			DI* di1 = (DI*) lln1->value;
			DI* di2 = (DI*) lln2->value;
			WCS_addDim(newWcs, di1->name, min(di1->min, di2->min), max(di1->max, di2->max));
		}
	}

	// start copying the coefs
	{
		int srcWcsIndex = 0;
		// for each of the two src WCSs
		for (srcWcsIndex = 0; srcWcsIndex < 2; srcWcsIndex++) {
			WCS* srcWcs = srcWcsArray[srcWcsIndex];
			void* pSrcWc;
			int coefIndex;

			// for each coef
			PQ_ITERATE(&srcWcs->coefs, coefIndex, pSrcWc) {
				WC* srcWc = (WC*) pSrcWc;
				WC* newWc;
				LLN* lln;
				int dimIndex = 0;
				WC* minWc;
				if (!srcWc->e) {
					continue;
				}

				// copy the WC
				newWc = (WC*) walloc(sizeof(WC));
				WC_init(newWc, srcWc->v);

				// copy its dims
				LL_ITERATE(&srcWc->dims, lln) {
					DN* dn;
					DN* srcDn;
					Assert(dimIndex < srcWcs->dims.count);
					srcDn = (DN*) lln->value;
					dn = walloc(sizeof(DN));
					*dn = *srcDn;
					WC_add(newWc, dn);
					dimIndex++;
				}

				// insert the new WC if there's space
				minWc = (WC*) PQ_getMin(&newWcs->coefs);
				if (PQ_isFull(&newWcs->coefs)) {
					Assert(minWc != NULL);
					if (fabs(newWc->v) < fabs(minWc->v)) {
						WCS_add(newWcs, newWc);
					} else {
						// drop it.
					}
				} else {
					WCS_add(newWcs, newWc);
				}
			}
		}
	}

	newWcs = WCS_toast(newWcs);
	return newWcs;

	//return equijoin(wcs1, wcs2, NULL, maxCount, res, true);
}

#ifndef STANDALONE
Datum wcs_union(PG_FUNCTION_ARGS) {
	WCS* wcs1 = PG_GETARG_WCS_P(0);
	WCS* wcs2 = PG_GETARG_WCS_P(1);
	PG_RETURN_WCS_P(real_wcs_union(wcs1, wcs2));
}
#endif

// TODO check: should flip?
int WCP_compare(void* left, void* right) {
	int l = ((WCP*) left)->key;
	int r = ((WCP*) right)->key;
	return l < r ? -1 : l > r ? 1 : 0;
}

void renderhelper(WCS* wcs, LL* coeff, int i, double** c, mhist* h) {
#ifdef RENDER_HIST
    int* minvals;
    int* maxvals;
#endif
	PQ pq;
	int count;
	LLN* lln;
	int prev;
	LL temp1;
	//PL("\n");
	//PL("renderhelper start (i = %d)\n", i);
	//PI();
	if (coeff->count == 0) {
		//PUI();
		//PL("renderhelper end\n");
		//PL("\n");
		PL("[empty WCS]\n");
		return;
	}

	//PL("\n");
	//PL("\n");
	//PL("render(i = %d)\n", i);
	//PL("\n");
	if (i >= wcs->dims.count) {
		double count = 0;
		WC* mwc = NULL;
		LLN* lln;
		//PI();
		LL_ITERATE(coeff, lln) {
			int sign = 1;
			int k;
			WC* wc = (WC*) lln->value;
			mwc = wc;
//			PL("");
			for (k = 0; k < wc->dims.count; k++) {
				DN* dn = wc->a[k];
				if (dn->rl < dn->sc) {
					sign *= dn->sl;
				} else {
					sign *= dn->sh;
				}
//				PP("%d ", dn->c);
			}
			//PL("%d * %f \n", sign, wc->v);
			count += sign * wc->v;
		}
		//PUI();

		if (count == 0) {
			//PUI();
			//PL("renderhelper end\n");
			//PL("\n");
			return;
		}

#ifdef RENDER_HIST
		minvals = (int*) palloc(wcs->dims.count * sizeof(int));
		maxvals = (int*) palloc(wcs->dims.count * sizeof(int));
#endif

		// output (w.r.boundary, count)
#ifdef RENDER_LOG
		PL("v = % 2.3f\n", count);
#endif
		if (mwc != NULL) { // TODO check
			DN* dn1;
			DN* dn2;
			int j = 0;
			LL_ITERATE(&wcs->dims, lln) {
				DI* di = (DI*) lln->value;
				DN* dn = mwc->a[j];
				Assert(j < wcs->dims.count);
#ifdef RENDER_HIST
				// TODO change to use unseg() instead?
				minvals[j] = (int) dn->rl;
				maxvals[j] = (int) dn->rh;
#endif
#ifdef RENDER_LOG
				PL("\t%d : [ %d : %d ] or [ %f : %f ]\n", j,
					dn->rl, dn->rh, unseg(wcs, di, dn->rl), unseg(wcs, di, dn->rh + 1));
#endif
				if (c != NULL) {
					switch (j) {
						case 0:
							dn1 = dn;
							break;
						case 1:
							dn2 = dn;
							break;
					}
				}

				j++;
			}
			if (c != NULL) {
				int j, k;
				for (j = dn1->rl; j <= dn1->rh; j++) {
					for (k = dn2->rl; k <= dn2->rh; k++) {
						c[j][k] += count;
					}
				}
			}
		}
		//MHIST_SET_NUM_BUCKETS_USED(h, 0);

#ifdef RENDER_HIST
		//for (int dimIndex = 0; dimIndex < wcs->dims.count; dimIndex++)
		{
			int dimIndex = 0;
			LLN* lln;
			LL_ITERATE(&wcs->dims, lln) {
				DI* di = (DI*) lln->value;
				double min = unseg(wcs, di, minvals[dimIndex]);
				double max = unseg(wcs, di, maxvals[dimIndex] + 1); // + 1 for full cell span
				Assert(min >= INT_MIN && min <= INT_MAX);
				Assert(max >= INT_MIN && max <= INT_MAX);
				minvals[dimIndex] = (int) min;
				maxvals[dimIndex] = (int) max;
				dimIndex++;
			}
		}
		add_bucket_to_mhist(h, minvals, maxvals, count);
		pfree(minvals);
		pfree(maxvals);
#endif

		//PUI();
		//PL("renderhelper end\n");
		//PL("\n");
		return;
	}

	PQ_init(&pq, 3 * wcs->coefs.count, WCP_compare); // TODO check
	count = 0;

	LL_ITERATE(coeff, lln) {
		WC* wc = (WC*) lln->value;
		//DN* dn = (DN*) wc->a[i]->value;
		DN* dn = wc->a[i];
		WCP* wcp;

		wcp = (WCP*) walloc(sizeof(WCP));
		WCP_init(wcp, dn->rl - 1, wc);
		PQ_add(&pq, wcp);
//		PL("pq.enqueue(%d %d . %d) size %d\n", wc->a[0]->c, wc->a[1]->c, wcp->key, pq.count);

		wcp = (WCP*) walloc(sizeof(WCP));
		WCP_init(wcp, dn->rh, wc);
		PQ_add(&pq, wcp);
//		PL("pq.enqueue(%d %d . %d) size %d\n", wc->a[0]->c, wc->a[1]->c, wcp->key, pq.count);

		if (dn->rl < dn->sc && dn->sc <= dn->rh) {
			wcp = (WCP*) walloc(sizeof(WCP));
			WCP_init(wcp, dn->sc - 1, wc);
			PQ_add(&pq, wcp);
//			PL("pq.enqueue(%d %d . %d) size %d\n", wc->a[0]->c, wc->a[1]->c, wcp->key, pq.count);
		}

		count++;
	}

	//PL("\n");
	//PL("count = %d\n", count);
	//PL("\n");

	prev = INT_MIN;
	LL_init(&temp1);
	while (pq.count > 0) {
		int topkey;
		LL temp2;
		LL_init(&temp2);
		topkey = ((WCP*) PQ_getMin(&pq))->key;

		//PL("topkey is now %d\n", topkey);
		//PL("----------------\n");

		// dequeue all elements with key = topkey from PQ pq and insert into temp1
		while (pq.count > 0) {
			WCP* wcp = (WCP*) PQ_getMin(&pq);
			if (wcp->key != topkey) {
				break;
			}
			PQ_removeMin(&pq);

			if (!LL_contains(&temp1, wcp->wc)) {
				LLN* lln = (LLN*) walloc(sizeof(LLN));
				LLN_init(lln, wcp->wc);
				LL_add(&temp1, lln);
				//PL("pq.dequeue(%d %d) -> temp1 (size %d)\n", wcp->wc->a[0]->c, wcp->wc->a[1]->c, temp1.count);
			}
		}

		LL_ITERATE(&temp1, lln) {
			WC* wc = (WC*) lln->value;
			DN* dn = wc->a[i];
			int low = prev + 1;
			int high = topkey;

			//PL("checking(%d %d) from temp1\n", wc->a[0]->c, wc->a[1]->c);
			if (wc->a[i]->rh < low) {
				//PL("removing(%d %d) from temp1\n", wc->a[0]->c, wc->a[1]->c);
				LL_remove(&temp1, lln);
			}

			// if overlap
			if ((low <= dn->rl && dn->rl <= high) ||
				(dn->rl <= low && low <= dn->rh))
			{
				//PL("overlap with (%d %d): adding(%d %d) to temp2\n", low, high, wc->a[0]->c, wc->a[1]->c);

				// NOTE we don't need to modify the LL because render() doesn't use it
				// WARNING this means that we can no longer safely perform operations on the WCS
				// TODO check that warning...is it really true?

				// copy the WC
				WC* prime = (WC*) walloc(sizeof(WC));
				DN* newdn;
				LLN* plln;
				*prime = *wc;

				// copy & change the DN
				newdn = (DN*) walloc(sizeof(DN));
				*newdn = *dn;
				newdn->rl = low;
				newdn->rh = high;

				// copy the WC array, and change element pointing to the changed DN
				if ( prime->dims.count > 0 ) {
					int j;
					prime->a = (DN**) walloc(prime->dims.count * sizeof(DN*));
					for (j = 0; j < wc->dims.count; j++) {
						prime->a[j] = wc->a[j];
					}
					prime->a[i] = newdn;
				}

				// insert new WC into temp2
				plln = (LLN*) walloc(sizeof(LLN));
				plln->value = prime;
				LL_add(&temp2, plln);
			}
		}
		renderhelper(wcs, &temp2, i + 1, c, h);
		prev = topkey;
	}

	//PUI();
	//PL("renderhelper end\n");
	//PL("\n");
}

void srrecur(WCS* wcs, MDA* ar, int* ndx, int l) {
	if (l < wcs->dims.count) {
		int i;
		for (i = 0; i < (1 << wcs->m); i++) {
			ndx[l] = i;
			srrecur(wcs, ar, ndx, l + 1);
		}
	} else {
		int i;
		void* p;
		LLN* lln;
		double v;
		PQ_ITERATE(&wcs->coefs, i, p) {
			WC* wc = (WC*) p;
			LLN* lln;
			int j;
			int d;
			bool inside;
			if (!wc->e) {
				continue;
			}
			j = 0;
			wc->a = (DN**) walloc(wcs->dims.count * sizeof(DN*));
			LL_ITERATE (&wc->dims, lln) {
				wc->a[j] = (DN*) lln->value;
				j++;
			}
			inside = true;
			for (d = 0; d < wcs->dims.count; d++) {
				if (!(wc->a[d]->rl <= ndx[d] && ndx[d] <= wc->a[d]->rh)) {
					inside = false;
					break;
				}
			}
			if (inside)
			{
				double v = 1;
				for (d = 0; d < wcs->dims.count; d++) {
					if (ndx[d] < wc->a[d]->sc) {
						v *= wc->a[d]->sl;
					} else {
						v *= wc->a[d]->sh;
					}
				}
				v *= wc->v;
				set(ar, ndx, get(ar, ndx) + v);
			}
			pfree(wc->a);
		}

		PL("");
		for (i = 0; i < wcs->dims.count; i++) {
			PP("% 2d ", ndx[i]);
		}
		PP("(");
		i = 0;
		LL_ITERATE(&wcs->dims, lln) {
			DI* di = (DI*) lln->value;
			PP("% 2.3f ", unseg(wcs, di, ndx[i]));
			i++;
		}
		v = get(ar, ndx);
		PP("):% 2.3f\n", v);
	}
}

void simpleRender(WCS* wcs) {
    MDA ar;
    int* ndx;
	int i;
	void* p;
	int l;
	double** a;
	int y, x;
	wcs = WCS_detoast(wcs);
	//if (wcs->dims.count != 2) {
		ndx = (int*) walloc(sizeof(int) * wcs->dims.count);
		init(&ar, wcs->dims.count, wcs->m);
		fill(&ar, 0);
		srrecur(wcs, &ar, ndx, 0);
		print_array(&ar);
		return;
	//}

	l = (1 << wcs->m);
	a = (double**) wcalloc(l, sizeof(double*));
	for (i = 0; i < l; i++) {
		a[i] = (double*) wcalloc(l, sizeof(double));
	}
	//for (int y = 0; y < l; y++) {
	//	PL("");
	//	for (int x = 0; x < l; x++) {
	//		PP("% 2.3f ", a[y][x]);
	//	}
	//	PP("\n");
	//}
	for (y = 0; y < l; y++) {
		PL("");
		for (x = 0; x < l; x++) {
			PQ_ITERATE(&wcs->coefs, i, p) {
				LLN* lln;
				WC* wc = (WC*) p;
				int j = 0;
				if (!wc->e) {
					continue;
				}
				wc->a = (DN**) walloc(2 * sizeof(DN*));
				LL_ITERATE (&wc->dims, lln) {
					wc->a[j] = (DN*) lln->value;
					j++;
				}
				if (wc->a[0]->rl <= y && y <= wc->a[0]->rh &&
					wc->a[1]->rl <= x && x <= wc->a[1]->rh)
				{
					double v = 1;
					if (y < wc->a[0]->sc) {
						v *= wc->a[0]->sl;
					} else {
						v *= wc->a[0]->sh;
					}
					if (x < wc->a[1]->sc) {
						v *= wc->a[1]->sl;
					} else {
						v *= wc->a[1]->sh;
					}
					v *= wc->v;
					a[y][x] += v;
				}
			}
			PP("% 2.3f ", a[y][x]);
		}
		PP("\n");
	}
	PL("\n");
}

// m  = 0

mhist* render(WCS* wcs) {
	int m;
	void* p;
	int j;
	double** c;
	int s;
	mhist* h;
	LL ll;
	wcs = WCS_detoast(wcs);
    m = wcs->m;
#ifndef SIMPLERENDER
	//PL("render start\n");
#ifdef DUMPCOEF
	WCS_print(wcs);
#endif
	LL_init(&ll);
	PQ_ITERATE(&wcs->coefs, j, p) {
		//PL("checkpoint 1\n");
		WC* wc = (WC*) p;
		LLN* lln;
		//PL("checkpoint 1.1\n");
		if (!wc->e) {
			//PL("checkpoint 1.2\n");
			continue;
		}
		//PL("%f\t\t%f\n", wc->vv, wc->v);
		if (wc->dims.count > 0) {
			int k = 0;
			LLN* dlln;
			//PL("checkpoint 1.3\n");
			wc->a = (DN**) walloc(wc->dims.count * sizeof(DN*));
			//PL("checkpoint 2\n");
			LL_ITERATE(&wc->dims, dlln) {
				//PL("checkpoint 3\n");
				DN* dn = (DN*) dlln->value;
				Assert (k < wc->dims.count);
				wc->a[k++] = dn;
			}
			Assert(k == wcs->dims.count);
		}

		//PL("checkpoint 4\n");
		lln = (LLN*) walloc(sizeof(LLN));
		LLN_init(lln, wc);
		LL_add(&ll, lln);
	}

	c = NULL;
	s = 1 << m;
	if (wcs->dims.count == 2 && m > 0) {
		int i;
		c = (double**) walloc(s * sizeof(double*));
		for (i = 0; i < s; i++) {
			c[i] = (double*) wcalloc(s, sizeof(double));
		}
	}
	h = NULL;
#ifdef RENDER_HIST
    {
	// TODO be more conservative with size estimate, use create_bigger_mhist as needed
#define MAX_BUCKETS 65536
	int nbuckets = min(MAX_BUCKETS, pow(1 << wcs->m, wcs->dims.count));
	int dimid = 0;
	LLN* lln;
	h = create_mhist(nbuckets, wcs->dims.count);
	MHIST_SET_NUM_BUCKETS_USED(h, 0);
	/* We're assuming that the entire histogram was zeroed out;
	 * otherwise, we would have to pad the rest of the name field with
	 * zeros to avoid screwing up postgres hashing. */
	//char** cols = (char**) palloc(sizeof(char*) * wcs->dims.count);
	LL_ITERATE(&wcs->dims, lln) {
		DI* di = (DI*) lln->value;
		MHIST_SET_NAME_DIM(h, dimid, dimid);
		snprintf(MHIST_DIM_NAME(h, dimid), MHIST_COL_NAME_LEN,
				"%s", di->name);
//		snprintf(MHIST_DIM_NAME(h, dimid), MHIST_COL_NAME_LEN,
//				"%s.%s", "dummy", di->name);
		dimid++;
	}
    MHIST_SET_NUM_DIM_NAMES(h, wcs->dims.count);
	renderhelper(wcs, &ll, 0, c, h);
	return h;
    }
#else
	if (c != NULL) {
		int i, j;
		for (i = 0; i < s; i++) {
			PL("|");
			for (j = 0; j < s; j++) {
				PP("% 1.3f |", c[i][j]);
			}
			PP("\n");
		}
	}
	return NULL;
	//PL("render end\n");
    
#endif // RENDER_HIST
#else
#ifdef DUMPCOEF
	WCS_print(wcs);
#endif
	simpleRender(wcs);
#endif
}

int INT_compare(void* a, void* b) {
	int l = *((int*) a);
	int r = *((int*) b);
	return l < r ? -1 : l > r ? 1 : 0;
}

#ifdef STANDALONE
HeapTuple *get_table_as_array(char *tableName, char **cols, int ncols,
		int *ntuples, TupleDesc *descdest)
{
	descdest = (TupleDesc*) walloc(sizeof(TupleDesc));
	int groups = 0;
	int format = -1; // 0 = groups, 1 = 2D, 2 = 3D
	char tablePath[MAX_PATH];
	char dims[MAX_PATH];
	strcpy(tablePath, baseDirPath);
	strcat(tablePath, tableName);
	strcat(tablePath, ".txt");
	FILE* dataFile = fopen(tablePath, "r");
	if (NULL == dataFile) {
		goto parse_error;
	}
	if (4 != fscanf(dataFile,
				"Columns = %s\nFormat = %d\nTupleCount = %d\nGroupCount = %d\n",
				dims, &format, ntuples, &groups)) {
		goto parse_error;
	}
	int* counts = (int*) walloc(groups * sizeof(int));
	double** values =  (double**) walloc(groups * sizeof(double*));
	for (int j = 0; j < groups; j++) {
		values[j] = (double*) walloc(ncols * sizeof(double));
	}

	switch (format) {
		case 1:
			{
				// read dimension info
				int dimCount = -1;
				if (1 != fscanf(dataFile, "DimensionCount = %d\n", &dimCount)) {
					goto parse_error;
				}
				// read counts
				for (int i = 0; i < groups; i++) {
					if (1 != fscanf(dataFile, "%d", &counts[i])) {
						goto parse_error;
					}
					for (int j = 0; j < dimCount; j++) {
						if (1 != fscanf(dataFile, "%lf", &values[i][j])) {
							goto parse_error;
						}
					}
				}
				break;
			}
		case 2:
			{
				// read dimension info
				double xmin, xmax, xdelta;
				if (3 != fscanf(dataFile, "XMin = %lf\nXMax = %lf\nXDelta = %lf\n", &xmin, &xmax, &xdelta)) {
					goto parse_error;
				}
				xmax += xdelta;
				double ymin, ymax, ydelta;
				if (3 != fscanf(dataFile, "YMin = %lf\nYMax = %lf\nYDelta = %lf\n", &ymin, &ymax, &ydelta)) {
					goto parse_error;
				}
				ymax += ydelta;

				// read cells
				double x = xmin;
				double y = ymin;
				int tmp;
				int i = 0;
				while (1 == fscanf(dataFile, "%d", &tmp) && i < groups) {
					if (tmp != 0) {
						counts[i] = tmp;
						values[i][0] = y;
						values[i][1] = x;
						i++;
					}
					x += xdelta;
					if (x == xmax) {
						x = xmin;
						y += ydelta;
					}
				}
				break;
			}
		case 3:
			{
				// read dimension info
				double xmin, xmax, xdelta;
				if (3 != fscanf(dataFile, "XMin = %lf\nXMax = %lf\nXDelta = %lf\n", &xmin, &xmax, &xdelta)) {
					goto parse_error;
				}
				xmax += xdelta;
				double ymin, ymax, ydelta;
				if (3 != fscanf(dataFile, "YMin = %lf\nYMax = %lf\nYDelta = %lf\n", &ymin, &ymax, &ydelta)) {
					goto parse_error;
				}
				ymax += ydelta;
				double zmin, zmax, zdelta;
				if (3 != fscanf(dataFile, "ZMin = %lf\nZMax = %lf\nZDelta = %lf\n", &zmin, &zmax, &zdelta)) {
					goto parse_error;
				}
				zmax += zdelta;

				// read cells
				double x = xmin;
				double y = ymin;
				double z = zmin;
				int tmp;
				int i = 0;
				while (1 == fscanf(dataFile, "%d", &tmp) && i < groups) {
					if (tmp != 0) {
						counts[i] = tmp;
						values[i][0] = z;
						values[i][1] = y;
						values[i][2] = x;
						i++;
					}
					x += xdelta;
					if (x == xmax) {
						x = xmin;
						y += ydelta;
						if (y == ymax) {
							y = ymin;
							z += zdelta;
						}
					}
				}
				break;
			}
		default:
			goto parse_error;
	}

	HeapTuple* a = (HeapTuple*) walloc(*ntuples * sizeof(HeapTuple));
	for (int j = 0; j < *ntuples; j++) {
		a[j] = (HeapTuple) walloc(sizeof(HeapTupleData));
		a[j]->values = (double*) wcalloc(ncols, sizeof(double));
	}

	int i = 0;
	for (int g = 0; g < groups; g++) {
		for (int j = 0; j < counts[g]; j++) {
			Assert(i < *ntuples);
			for (int k = 0; k < ncols; k++) {
				a[i]->values[k] = values[g][k];
			}
			i++;
		}
	}

	if (dataFile != NULL) {
		fclose(dataFile);
	}

	return a;

parse_error:
	PL("error opening %s.txt\n", tableName);
	return NULL;
}

/* FUNCTION get_timestamp_col
 * ARGUMENTS: <desc> is a tuple descriptor.
 * PRECONDITIONS: <desc> represents a stream.
 * POSTCONDITIONS: Returns the attrid of the timestamp attribute for the
 *      stream. 
 */
int get_timestamp_col(TupleDesc desc) 
{
    Assert(desc != NULL);

    if (NULL == desc->constr) {
        /* No constraints, so no timestamp. */
        return TCQTSATTRNOP;
    }

    /* Someone decided to make ts_attrnum 0-based, rather than 1-based.  Rather
     * than correct that mistake, we add 1 to convert to the actual attribute
     * number. */
    /* TODO: Fix the original problem. */
    return 1 + (desc->constr->ts_attrnum);
}

/* FUNCTION get_num_non_timestamp_attrs
 * ARGUMENTS: <desc> points to a tuple descriptor.
 * POSTCONDITIONS: Returns the number of attributes of <desc> that are not
 *      timestamps.
 */
int get_num_non_timestamp_attrs(TupleDesc desc) 
{
    int ts_col = -1;
    Assert(desc != NULL);

    ts_col = get_timestamp_col(desc);

    if (ts_col != TCQTSATTRNOP) {
        /* Have a timestamp column. */
        return desc->natts - 1;
    } else {
        /* No timestamp. */
        return desc->natts;
    }

}

#endif

// NOTE this function requires the first batch of tuples to include a min and max
WCS* real_wcs_in_helper(char* tabName, char** colNameArr, int colCount, int maxCount, int res, HeapTuple* tuples, int ntuples, TupleDesc descdest, bool doLoad, MDA* mda, int* dimLo, int* dimHi) {
	int m = res;
	double* ranges;
	double* mins;
	WCS* wcs;
	int* ndx;
	int d;
	/*PL("done getting tuples\n");*/
	init(mda, colCount, m);
	fill(mda, 0);
	ranges = (double*) walloc(colCount * sizeof(double));
	mins = (double*) walloc(colCount * sizeof(double));
	wcs = (WCS*) walloc(sizeof(WCS));
	wcs->m = m;
	ndx = (int*) wcalloc(colCount, sizeof(int));
	WCS_init(wcs, maxCount);

	//PL("checkpoint 1\n");
	for (d = 0; d < colCount; d++) {
		double min, max;
		// Dimensions were specified.
		if (dimLo != NULL) {
			Assert(dimLo != NULL && dimHi != NULL);
			min = dimLo[d];
			max = dimHi[d];
		// Dimensions were not specified, so we have to try to determine the range.
		} else {
			Assert(dimLo == NULL && dimHi == NULL);
			if (ntuples == 0) {
				min = 0;
				max = 1;
			} else {
				HeapTuple first;
				HeapTuple last;
				global_cmpf_dimid = d;
				global_cmpf_desc = descdest;
				qsort(tuples, ntuples, sizeof(HeapTuple), &cmpf);
				first = tuples[0];
				last = tuples[ntuples - 1];
				min = get_double_field_val(first, descdest, d + 1, true);
				max = get_double_field_val(last, descdest, d + 1, true);
			}
		}
		mins[d] = min;
		ranges[d] = (max - min) / (double) ((1 << m) - 1); // TODO handle 0
		//LLN* lln = (LLN*) walloc(sizeof(LLN));
		//DI* di = (DI*) walloc(sizeof(DI));
		WCS_addDim(wcs, colNameArr[d], min, max);
	}

	{
		int i, d;
		for (i = 0; i < ntuples; i++) {
			HeapTuple tuple = tuples[i];
			int* ndx = (int*) walloc(wcs->dims.count * sizeof(int));
			for (d = 0; d < wcs->dims.count; d++) {
				double value = get_double_field_val(tuple, descdest, d + 1, true);
				ndx[d] = (int) (value - mins[d]) / ranges[d];
			}
			set(mda, ndx, get(mda, ndx) + 1);
		}
	}

	//PL("checkpoint 3\n");
	/*print_array(mda);*/

    if (doLoad) {
        //PL("checkpoint 4\n");
        compute(mda, ndx, wcs, 0);
    }

//	WCS_print(wcs);
//	simpleRender(wcs);
//	PL("ahahahahahahaha\n");
//	render(wcs);

	/*PL("real_wcs_in end\n");*/
	return wcs;
}

//WCS* wcs_load(generic_stream_state* state) {
//    wcs_stream_state* ws = state->ds_state.ws;
//    WCS* wcs = ws->wcs;
//    int i;
//    for (i = 0; i < state->tb.count ; i++) {
//        int d;
//        for (d = 0; d < wcs->dims.count; d++) {
//            double value = get_double_field_val(tuple, state->tb.desc, d + 1, true);
//        }
//    }
//}

// creating and loading in one step
// TODO change this so that creating/loading are distinct steps?
WCS* wcs_create_stream(generic_stream_state* state, bool doLoad) {
    wcs_stream_state *ws = &state->ds_state.ws;
    int num_cols = get_num_non_timestamp_attrs(state->input_desc);
    int dimid = -1;
    char* colnames[num_cols];
	char * relname;
	MDA mda;
	WCS* wcs;

	Relation r;
	/* Fill in information about the different dimensions. */
	for (dimid = 0; dimid < num_cols; dimid++) {
		int attrid = get_attr_id_skipping_ts(dimid + 1, state->input_desc);
		Form_pg_attribute   attr = state->input_desc->attrs[attrid-1];
		char * relname;
		char* name;

		/* The OID of the relation is stored in the tuple descriptor's
		 * attribute fields; different fields could come from different
		 * relations, so we compute the name for each field. */
		r = RelationIdGetRelation(attr->attrelid);

		/* Need to check whether this relation exists (the column might be
		 * an intermediate result). */
		if (NULL == r) {
			relname = "(unknown)";
		} else {
			relname = RelationGetRelationName(r);
		}

		/* TODO: Should we call RelationClose() here to decrement a reference
		 * count? */

		name = palloc(WAVELET_COL_NAME_LEN * sizeof(char));
		snprintf(name, WAVELET_COL_NAME_LEN,
				"%s.%s", relname, (state->input_desc->attrs[attrid-1])->attname.data);
		colnames[dimid] = name;
	}

	// TODO good idea?
	/* Set the relation name to that of the last col */
	if (NULL == r) {
		relname = "(unknown)";
	} else {
		relname = RelationGetRelationName(r);
	}

	wcs = real_wcs_in_helper(relname, colnames, num_cols, ws->maxCount, ws->res, state->tb.buf, state->tb.count, state->input_desc, doLoad, &mda, ws->dimLo, ws->dimHi);
	wcs = WCS_toast(wcs);
    return wcs;
}

// creates the WCS on relation tabName for the dimensions in colNames.
// colNames is a comma-delimited list of attributes on the relation.
// maxCountS is a string representing a positive integer.
//  It is the maximum number of wavelet coefficients to maintain.
//  The arg type will probably be changed to an integer later.
// res is a string representing a positive integer.
//  2^res = resolution of the joint frequency distribution array.
//  (Basically, it's the number of "buckets" along each dimension.)
//  The arg type will probably be changed to an integer later.
Datum real_wcs_in(char* tabName, char* colNamesUnsafe, char* colMinStrUnsafe, char* colMaxStrUnsafe, int maxCount, int res) {
    MDA mda;
	int len = strlen(colNamesUnsafe);
	char* colNames = walloc((len+1) * sizeof(char));
	int ntuples;
	int colCount = 1;
	int i;
	char** colNameArr;
	int relNameLen;
	bool start;
	int colIndex;
	TupleDesc descdest;
	HeapTuple* tuples;
	strcpy(colNames, colNamesUnsafe);
	for (i = 0; i < len; i++) {
		if (colNames[i] == ',') {
			colNames[i] = '\0';
			colCount++;
		}
	}
	colNameArr = (char**) walloc(colCount * sizeof(char*));
	relNameLen = strnlen(tabName, WAVELET_COL_NAME_LEN);

	start = true;
	for (i = 0, colIndex = 0; i < len; i++) {
		Assert(colIndex < colCount);
		if (start) {
			colNameArr[colIndex] = colNames + i;
			start = false;
		} else if (colNames[i] == '\0') {
			start = true;
			colIndex++;
		}
	}
	Assert(colCount == colIndex + 1);

	tuples = get_table_as_array(tabName, colNameArr, colCount, &ntuples, &descdest);

	// Prepend "relname." to each of the dimension names.
	for (i = 0; i < colCount; i++) {
		int dimNameLen = strnlen(colNameArr[i], WAVELET_COL_NAME_LEN);
		char* fullDimName;
		Assert(relNameLen + dimNameLen + 1 < WAVELET_COL_NAME_LEN);
		fullDimName = (char*) walloc(sizeof(char) *
				(relNameLen + dimNameLen + 1 + 1)); // +1 for ".", +1 for "\0"
		snprintf(fullDimName, WAVELET_COL_NAME_LEN, "%s.%s",
				tabName, colNameArr[i]);
		colNameArr[i] = fullDimName;
	}

	// Parse the list of numbers. Note that we are converting comma-delimited strings of ints into arrays of doubles.
    {
	char* colMinStr = colMinStrUnsafe;
	char* colMaxStr = colMaxStrUnsafe;
	int strIndex;
	int* colMin = walloc(colCount * sizeof(int));
	int* colMax = walloc(colCount * sizeof(int));
    WCS* wcs;
	for (strIndex = 0; strIndex < 2; strIndex++) {
		char* colBoundStr = strIndex == 0 ? colMinStr : colMaxStr;
		int* colBound = strIndex == 0 ? colMin : colMax;
		char* lastStart = colBoundStr;
		int colBoundStrLen = strlen(colBoundStr);
		int j = 0;
		for (i = 0; i < colBoundStrLen; i++) {
			if (colBoundStr[i] == ',') {
				colBoundStr[i] = '\0';
				colBound[j++] = atoi(lastStart);
				lastStart = &colBoundStr[i+1];
				Assert(j < colCount);
			}
		}
		colBound[j] = atoi(lastStart);
		Assert(colCount - 1 == j);
	}

    wcs = real_wcs_in_helper(tabName, colNameArr, colCount, maxCount, res, tuples, ntuples, descdest, TRUE, &mda, colMin, colMax);
//    WCS* wcs = real_wcs_in_helper(tabName, colNameArr, colCount, maxCount, res, tuples, ntuples, descdest, TRUE, &mda, NULL, NULL);
	wcs = WCS_toast(wcs);
    return WcsPGetDatum(wcs);
    }
}

#ifndef STANDALONE
// serves as a wrapper function for real_wcs_in
Datum wcs_in(PG_FUNCTION_ARGS) {
	char* tabName = PG_GETARG_CSTRING(0); // table name
	char* colNames = PG_GETARG_CSTRING(1); // column names
	char* colMinStr = PG_GETARG_CSTRING(2); // column ranges
	char* colMaxStr = PG_GETARG_CSTRING(3); // column ranges
	int maxCountS = PG_GETARG_INT32(4); // max coefficient count
	int res = PG_GETARG_INT32(5); // max resolution
//	PG_RETURN_WCS_P(real_wcs_in(tabName, colNames, maxCountS, res));
	PG_RETURN_WCS_P(real_wcs_in(tabName, colNames, colMinStr, colMaxStr, maxCountS, res));
}
#endif

//void render_table(WCS* wcs) {
//	FuncCallContext		    *funcctx = NULL; 
//	wcs_to_table_state      *state = NULL;
///*	TupleDesc			    tupdesc;*/
//	char				    typename[128];
//
//	if (SRF_IS_FIRSTCALL()) { 
//		MemoryContext		    oldcontext; 
//
//		funcctx = SRF_FIRSTCALL_INIT(); 
//
//		/*elog(LOG, "mhist_to_table(): Starting first-call setup.");*/
//
//		
//		/* Create the data structure that will remember the status of this
//		 * function. */
//		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx); 
//		state = palloc(sizeof(*state));
//		MemoryContextSwitchTo(oldcontext);
//
//		state->current_wavelet = 0;
//		state->num_wavelets = wcs->coefs.count;
//
//		/* Generate the name of the type that corresponds to the histogram's
//		 * number of dimensions. */
//		snprintf(typename, sizeof(typename), "__wcs%d", wcs->dims.count);
//
//        /* Postgres SRF crap has been moved to this function: */
//		srf_firstcall_junk(funcctx, state,
//				wcs->coefs.count, typename);
//
//		/*elog(LOG, "mhist_to_table(): Done with first-call setup.");*/
//	}
//
//	/* Stuff that gets done every time the function is called: */
//	funcctx = SRF_PERCALL_SETUP();
//
//	state = (wcs_to_table_state*)(funcctx->user_fctx);
//
//	while (state->current_wavelet < state->num_wavelets) {
//		WC* wc = (WC*) wcs->coefs.nodes[state->current_wavelet];
//		if (wc->e)
//			break;
//		state->current_wavelet++;
//	}
//
//	/* Are we done returning tuples? */
//	if (state->current_wavelet < state->num_wavelets) {
//		char        **values;
//		int         numvalues = -1;
//		HeapTuple   tuple;
//		Datum       result;
//		int         valid = -1;
//
//        /*
//        elog(LOG, "mhist_to_table(): Generating tuple for bucket %d (%d total).",
//                state->current_bucket, state->num_buckets);
//                */
//
//		/* Not done returning tuples -- construct the next one. */
//		/* For the sake of simplicity, we convert the field values into strings
//		 * and then back into values. */
//
//        numvalues = 2 * hist->dims.count + 2;
//			/* Number of items in row == 2 + (2 x number of dimensions:
//             *   1 column for the bucketID (not strictly necessary)
//             *   1 column for the bucket's count
//             *   2 columns per dimension to hold min and max values.
//             */
//
//        /*elog(LOG, "mhist_to_table():     This tuple has %d fields.",
//                numvalues);*/
//
//		/* Build up the array of string values. */
//		values = (char**)palloc(sizeof(char*) * (numvalues));
//
//		for (valid = 0; valid < numvalues; valid++) {
//			values[valid] = (char*)palloc(32 * sizeof(char));
//		}
//
//		/* Fill in the array. */
//		for (valid = 0; valid < numvalues; valid++) {
//            if (0 == valid) {
//                /* First entry is the bucketid. */
//                snprintf(values[valid], 32, "%d", state->current_wavelet);
//
//            } else if (numvalues - 1 == valid) {
//                /* Last entry is the bucket count. */
//                mhist_bucket *cur_bucket = 
//                    MHIST_BUCKET_PTR(hist, state->current_bucket);
//                /*
//                snprintf(values[valid], 32, "%d", 
//                        (int)(cur_bucket->count + 0.5));
//                        */
//                    /* Round up if necessary */
//                snprintf(values[valid], 32, "%f", 
//                        (cur_bucket->count));
//                 
//            } else {
//                /* valid > 0 and < (numvalues-1) */
//                /* Entries between first and last are the bounds of the
//                 * rectangle that comprises this bucket.  These entries
//                 * alternate between the lower and upper bound. */
//                int dimid = (valid - 1) / 2;
//                bool is_min = (valid % 2 == 1);
//                    /* Is this a lower or upper bound? */
//                mhist_bucket *cur_bucket = 
//                    MHIST_BUCKET_PTR(hist, state->current_bucket);
//                snprintf(values[valid], 32, "%d", 
//                        is_min ?
//                        cur_bucket->diminfo[dimid].min_val
//                        : cur_bucket->diminfo[dimid].max_val);
//            }
//
//            /*elog(LOG, "mhist_to_table():     Value %d is '%s'.",
//                    valid, values[valid]);*/
//
//		}
//
//        /*elog(LOG, "mhist_to_table():     Done generating values.");*/
//
//		/* Construct the tuple for the strings. */
//		tuple = BuildTupleFromCStrings(funcctx->attinmeta, values);
//
//        /*elog(LOG, "mhist_to_table():     Done building tuple.");*/
//
//		/* Make the tuple into a datum. */ 
//		result = TupleGetDatum(funcctx->slot, tuple);
//
//		/* Clean up. */
//		for (valid = 0; valid < hist->hdr.numDims; valid++) {
//            /*elog(LOG, "mhist_to_table(): Freeing field %d of a tuple.",
//                    valid);*/
//			pfree(values[valid]);
//		}
//        /*elog(LOG, "mhist_to_table(): Freeing holder for tuple %d.",
//                state->current_bucket);*/
//		pfree(values);
//
//        /*elog(LOG, "mhist_to_table(): Done generating tuple for bucket %d.",
//                state->current_bucket);*/
//
//        ++(state->current_bucket);
//
//		// PROF_EXIT_FUNCTION("mhist_to_table");
//
//		SRF_RETURN_NEXT(funcctx, result); 
//	} else { 
//        /*elog(LOG, "mhist_to_table(): Cleaning up.");*/
//
//		/* here we are done returning items, and just need to clean up: */ 
//        srf_cleanup(funcctx);
//		
//		// PROF_EXIT_FUNCTION("mhist_to_table");
//
//		SRF_RETURN_DONE(funcctx); 
//	}
//}


#ifndef STANDALONE
#define OUTPUT_BUF_LEN 256
// renders the WCS to human-readable form to the log (and returns "blah").
// Currently simpleRender() is used, but it's inefficient,
//  and it outputs the value for each coordinate in the rendered array.
//  Once render() is debugged, that will be used.
//  It is capable of displaying (multi-dimensional) ranges of values.

//  this is currently a dummy stub that always returns "blah"
Datum wcs_out(PG_FUNCTION_ARGS) {
	/*PL("wcs_out start\n");*/
	WCS* wcs = PG_GETARG_WCS_P(0);
	char* output = (char*) walloc(OUTPUT_BUF_LEN);
	char* outputCursor = NULL;
	wcs = WCS_detoast(wcs);

	// write some basic information
	snprintf(output, OUTPUT_BUF_LEN, "WCS: %d coefs, %d dims ( ",
			wcs->coefs.count, wcs->dims.count);

	// now write the dimensions
	{
		LLN* lln;
		LL_ITERATE(&wcs->dims, lln) {
			DI* di = (DI*) lln->value;
			outputCursor = output + strnlen(output, OUTPUT_BUF_LEN);
			snprintf(outputCursor, OUTPUT_BUF_LEN, "%s%s", di->name,
					lln->next == NULL ? " )" : ", ");
		}
	}

	/*PL("wcs_out end\n");*/
	PG_RETURN_CSTRING(output);
}

// converts to histogram for rendering purposes
Datum wcs_to_mhist(PG_FUNCTION_ARGS) {
	/*PL("wcs_out start\n");*/
	WCS* wcs = PG_GETARG_WCS_P(0);
	PG_RETURN_MHIST_P(render(wcs));
}
#endif

// gets a cached WCS for the specified relation name
Datum get_cached_wcs(PG_FUNCTION_ARGS) {
	char		*cachename = PG_GETARG_CSTRING(0);

    WCS         *result = NULL;

    int         attrno = 1;
        /* We always fetch the first attribute. */

    bool        isnull = true;

    RangeVar   *relvar;
	Relation	rel;
	List	   *relname_list;

    Snapshot    snapshot;

    TupleDesc   cachedesc;

    HeapTuple   tup;
    HeapScanDesc scan;

	/* Open relation and get the tuple descriptor for it.  Code copied from
     * RelationNameGetTupleDesc().
     *
     * We leave the relation open so we can do a scan.
     */
	relname_list = stringToQualifiedNameList(cachename, 
            "RelationNameGetTupleDesc");
	relvar = makeRangeVarFromNameList(relname_list);
	rel = relation_openrv(relvar, AccessShareLock);
	cachedesc = RelationGetDescr(rel);

    /* Fetch the first column of the first tuple. */
    if (cachedesc->attrs[attrno - 1]->atttypid != WCSTYPE) {
        elog(ERROR, "First column type ID %d doesn't match WCSTYPE type %d.",
                cachedesc->attrs[0]->atttypid, WCSTYPE);
    }

    /* We have to create our own "query snapshot", because we may be running
     * in the back end.  This is most unkosher.  Oh, well. */
    snapshot = GetSnapshotData(false);

    scan = heap_beginscan(rel, snapshot, 0, (ScanKey)NULL);
        /* TODO: Figure out what the "ScanKey" argument does */

    tup = heap_getnext(scan, ForwardScanDirection);

    result = DatumGetWcsP(heap_getattr(tup, attrno, cachedesc, &isnull));

    if (isnull) {
        elog(ERROR, "Got a null tuple instead of a WCS");
    }

    /* We're done with the scan. */
    heap_endscan(scan);
	relation_close(rel, AccessShareLock);

    /* TODO: Clean up!  We'll be called from inside continuous queries. */
	
	// XXX do we have to toast?

    PG_RETURN_WCS_P(result);
}



//////////////////////////////////////////


#ifdef STANDALONE

// read one line, skipping leading tabs
// return false if we're at EOF
bool readLine(FILE* queryFile, char* line) {
	int lineIndex = 0;
	bool startedReading = false;
	bool endReading = false;
	bool atEof = false;
	bool comment = false;
	while (true) {
		char c = fgetc(queryFile);
		switch (c) {
			case EOF:
				atEof = true;
			case '\n':
				if (comment) {
					comment = false;
				} else {
					endReading = true;
				}
				break;
			case '#':
				if (!startedReading) {
					comment = true;
				} else {
					startedReading = true;
				}
				break;
			case '\t':
				break;
			default:
				if (!comment) {
					startedReading = true;
				}
				break;
		}
		if (endReading) {
			break;
		}
		if (startedReading) {
			line[lineIndex] = c;
			lineIndex++;
		}
	}
	line[lineIndex] = '\0';
	return !atEof;
}

// recursive function that parses the test query files, line by line
Datum parseQuery(FILE* queryFile) {
	// read line
	char function[MAX_LINE];
	if (!readLine(queryFile, function)) {
		return NULL;
	}

	// determine the function called
	if (strcmp(function, "equijoin") == 0) {
		WCS* wcs1 = parseQuery(queryFile);
		if (wcs1 == NULL) {
			goto parse_error;
		}

		WCS* wcs2 = parseQuery(queryFile);
		if (wcs2 == NULL) {
			goto parse_error;
		}

		char dims[MAX_LINE];
		if (!readLine(queryFile, dims)) {
			goto parse_error;
		}

		char line[MAX_LINE];

		if (!readLine(queryFile, line)) {
			goto parse_error;
		}
		int maxCoefCount = atoi(line);

		if (!readLine(queryFile, line)) {
			goto parse_error;
		}
		int res = atoi(line);

		return equijoin(wcs1, wcs2, dims, maxCoefCount, res);
	} else if (strcmp(function, "select") == 0) {
		WCS* wcs = parseQuery(queryFile);
		if (wcs == NULL) {
			goto parse_error;
		}

		char lowerBounds[MAX_LINE];
		if (!readLine(queryFile, lowerBounds)) {
			goto parse_error;
		}
		
		char upperBounds[MAX_LINE];
		if (!readLine(queryFile, upperBounds)) {
			goto parse_error;
		}

		return real_WCS_selection(wcs, lowerBounds, upperBounds);
//	} else if (strcmp(function, "union") == 0) {
//		WCS* wcs = parseQuery(queryFile);
//		if (wcs == NULL) {
//			goto parse_error;
//		}
//
//		char dims[MAX_LINE];
//		if (!readLine(queryFile, dims)) {
//			goto parse_error;
//		}
//
//		return real_WCS_projection(wcs, dims);
	} else if (strcmp(function, "project") == 0) {
		WCS* wcs = parseQuery(queryFile);
		if (wcs == NULL) {
			goto parse_error;
		}

		char dims[MAX_LINE];
		if (!readLine(queryFile, dims)) {
			goto parse_error;
		}

		return real_WCS_projection(wcs, dims);
	} else if (strcmp(function, "in") == 0) {
		char table[MAX_LINE];
		if (!readLine(queryFile, table)) {
			goto parse_error;
		}

		char dims[MAX_LINE];
		if (!readLine(queryFile, dims)) {
			goto parse_error;
		}

		char line[MAX_LINE];

		if (!readLine(queryFile, line)) {
			goto parse_error;
		}
		int maxCoefCount = atoi(line);

		if (!readLine(queryFile, line)) {
			goto parse_error;
		}
		int resolution = atoi(line);

		return real_wcs_in(table, dims, maxCoefCount, resolution);
	} else if (strcmp(function, "dump") == 0) {
		WCS* wcs = parseQuery(queryFile);
		if (wcs == NULL) {
			goto parse_error;
		}

		PL("---DUMP BEGIN---\n");
		WCS_print(wcs);
		PL("---DUMP END---\n");

		return wcs;
	} else if (strcmp(function, "out") == 0) {
		WCS* wcs = parseQuery(queryFile);
		if (wcs == NULL) {
			goto parse_error;
		}

		PL("---OUTPUT BEGIN---\n");
		render(wcs);
		PL("---OUTPUT END---\n");

		return wcs;
	} else {
		return NULL;
	}

parse_error:
	PL("got a NULL value\n");
	return NULL;
}

// run tests
int main(int argc, char* argv[]) {
	// get the base dir path
	if (argc != 2) {
		fprintf(stderr, "wavelet <base dir>\n");
		return 1;
	}
	strcpy(baseDirPath, argv[1]);
	if (baseDirPath[strlen(baseDirPath) - 1] != '\\') {
		strcat(baseDirPath, "\\");
	}

	// get the path bases
	char inputPath[MAX_PATH];
	char outputPath[MAX_PATH];
	char queryPath[MAX_PATH];
	strcpy(inputPath, baseDirPath);
	strcpy(outputPath, baseDirPath);
	strcpy(queryPath, baseDirPath);

	// get the input path
	strcat(inputPath, "input.txt");
	FILE* inputFile = fopen(inputPath, "r");

	// get the rest of the query/output path bases
	strcat(queryPath, "query");
	strcat(outputPath, "output");
	int outputAppendStart = strlen(outputPath);
	int queryAppendStart = strlen(queryPath);

	// perform the tests
	char testId[MAX_PATH];
	while (true) {
		// get test ID
		int result = fscanf(inputFile, "%s\n", testId);
		if (result == EOF) {
			break;
		}

		// get output file
		strcpy(outputPath + outputAppendStart, testId);
		strcat(outputPath, ".txt");
		outputFile = fopen(outputPath, "w");

		// get query file
		strcpy(queryPath + queryAppendStart, testId);
		strcat(queryPath, ".txt");
		FILE* queryFile = fopen(queryPath, "r");

		// parse query file
		parseQuery(queryFile);

		// close the file
		fclose(queryFile);
		fclose(outputFile);
	}
	return 0;
}

#endif
