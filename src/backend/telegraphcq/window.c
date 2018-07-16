
/*
 * $RCSfile: window.c,v $
 * $Revision: 1.14 $
 * $Date: 2005/07/27 20:04:12 $
 *
 */

/*-------------------------------------------------------------------------
 *
 * window.c
 *	  Routines to handle window arithmetic.
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * DESCRIPTION
 *
 * This file contains various functions for performing computations on 
 * sliding windows over time.  These windows are defined by START AT,
 * RANGE BY, and SLIDE BY parameters.
 *
 * For a deeper description of our window semantics, see the documentation.
 *
 * TODO: Make the names of functions in this file more consistent with
 * each other.
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "executor/executor.h"
#include "telegraphcq/window.h"
#include "parser/parse_oper.h"
#include "catalog/pg_type.h"
#include "access/heapam.h"
#include "access/printtup.h"

#include "utils/timestamp.h"

/* #define DEBUG_WINDOWS  */

/* Uncomment the following line to enable debug statements in this file */
/*#define DEBUG_WINDOWS*/

#ifdef DEBUG_WINDOWS
/* Convenience debugging function to get the string representation of a
 * timestamp. Leaks memory! */
static char * ts_cstr(Timestamp ts) {
    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out, TimestampGetDatum(ts)));
}

static char * interval_cstr(Interval *iv) {
    return 
        DatumGetCString(
            DirectFunctionCall1(interval_out, IntervalPGetDatum(iv)));
}
#endif /* DEBUG_WINDOWS */

/*
 * PROTOTYPES OF INTERNAL FUNCTIONS
 */
Timestamp ts_plus_interval(Timestamp t, Interval i);

static Interval *
ValueToInterval(Value *interval_val)
{
	Interval *tmp_iv = NULL;
	Datum str = CStringGetDatum(strVal(interval_val));

    tmp_iv = DatumGetIntervalP(DirectFunctionCall3(interval_in, str,
												   ObjectIdGetDatum(InvalidOid),
												   Int32GetDatum(-1)));

	return tmp_iv;
}

static void
CreateInterval(Interval *iv, Value *val)
{
	Interval *tmp_iv = ValueToInterval(val);

	*iv = *tmp_iv;

	pfree(tmp_iv);
}


/* 
 * PUBLIC FUNCTIONS
 */

/* Create the WindowState object that corresponds to a WindowClause. */
WindowState *
createWindowState(WindowExpr *we) {
    WindowState *ret = makeNode(WindowState);
	
    /* Give us access to the timestamps in intermediate tuples. */
    Assert(we->tsvar != NULL);
    ret->tsvar = (Var*)copyObject(we->tsvar);
        /* Freed in freeWindowState() */

    ret->desc = NULL;

    /* Convert the START AT, RANGE BY, and SLIDE BY parameters into our
     * internal format.  The WindowExpr object stores them as strings. */
    Assert(NULL != we->startat);
    Assert(NULL != strVal(we->startat));
    ret->startAt = DatumGetTimestamp(
            DirectFunctionCall3(timestamp_in,
							   CStringGetDatum(strVal(we->startat)),
							   ObjectIdGetDatum(InvalidOid),
							   Int32GetDatum(-1)));


	if (we->slices == NIL) 
	{
		CreateInterval(&(ret->rangeBy), we->rangeby);
		CreateInterval(&(ret->slideBy), we->slideby);
		ret->slices = NIL;
	}
	else 
	{
		List *sl, *remainder;
		Interval *sumslice;
		Interval *slice = DatumGetIntervalP(NULL);

		Assert(we->rangeby == NULL);
		ret->slices = NIL;
		elog(LOG, " slices: ");

		sumslice = ValueToInterval(lfirst(we->slices));
		elog(LOG, "\t%s", DatumGetCString(
				 DirectFunctionCall1(interval_out,
									 IntervalPGetDatum(sumslice))));

		ret->slices = makeList1(sumslice);
		remainder = lnext(we->slices);

		ret->rangeBy = *sumslice;
		
		foreach (sl, remainder)
		{
			slice = ValueToInterval(lfirst(sl));
			
			ret->slices = lappend(ret->slices,slice);
			
			sumslice = DatumGetIntervalP(
				DirectFunctionCall2(interval_pl,
									IntervalPGetDatum(sumslice),
									IntervalPGetDatum(slice)));
			
			elog(LOG, "\t%s", DatumGetCString(
					 DirectFunctionCall1(interval_out,
										 IntervalPGetDatum(slice))));
			
		}
		elog(LOG,"Total period: %s ", DatumGetCString(
				 DirectFunctionCall1(interval_out,IntervalPGetDatum(sumslice))));
		/*
		 * The period of a sliced window is the sum of its slices
		 */
		ret->slideBy = *sumslice;
	}

    ret->lastTSSeen = DT_NOBEGIN;


    return ret;
}

void windowStateSetTupleDesc(WindowState *ws, TupleDesc td) {
    Assert(td != NULL);
    Assert(NULL == ws->desc);

    ws->desc = CreateTupleDescCopyConstr(td);
}

void
freeWindowState(WindowState * wndstate)
{
	if (wndstate == NULL)
		return;

    pfree(wndstate->desc);
    pfree(wndstate->tsvar);

    pfree(wndstate);

}

void
freeTupleList(TupleList * p)
{
	TupleList *prev = NULL;

	while (p) {
		if (prev != NULL) {
			heap_freetuple(prev->tuple);
			pfree(prev);
			prev = NULL;
		}
		prev = p;
		p = p->next;
	}
	if (prev != NULL) {
		heap_freetuple(prev->tuple);
		pfree(prev);
		prev = NULL;
	}
}


/*--------------------
 *             ExecReScanWindowState
 */
void
ReScanWindowState(Window * node, ExprContext exprCtxt, Plan * parent)
{
	elog(FATAL, "ojan thinks this may not be called...we will see");
}

/* FUNCTION windowStateGetFirstChange
 * PRECONDITIONS: <ws> is initialized.
 * POSTCONDITIONS: Returns the first moment in time at which the indicated
 * window will either slide or reach the end of the window.
 *
 * NOTE: Leaks memory!
 */
Timestamp windowStateGetFirstChange(WindowState *ws) {

    /*
    elog(LOG, "windowStateGetFirstChange():");
    elog(LOG, "    start at is %s", 
            DatumGetCString(
                DirectFunctionCall1(timestamp_out, 
                    TimestampGetDatum(ws->startAt))));
    elog(LOG, "    slide by is %s",
            DatumGetCString(
                DirectFunctionCall1(interval_out,
                    IntervalPGetDatum(&(ws->slideBy)))));
                    */

    /* There are no valid windows before window 1, so the first change happens 
     * when the first window expires.  The first window expires at 
     * (START AT) + (RANGE BY)
     */
    return ts_plus_interval(ws->startAt, ws->rangeBy); 
}

/* FUNCTION windowStateUpdateMaxTS
 * ARGUMENTS: <tup> is an intermediate tuple that was just passed to an
 *      aggregation node. 
 * POSTCONDITIONS: Updates the WindowState's status on the maximum timestamp it
 *      has observed from its base stream. 
 *
 * NOTE: Leaks memory!
 */
void windowStateUpdateMaxTS(WindowState *ws, HeapTuple tup)
{
    Timestamp tuple_ts = windowStateGetTS(ws, tup);

    /* If the tuple's timestamp for our base stream is > than the highest we 
     * have observed, update our observation accordingly. */
    if (DatumGetBool(DirectFunctionCall2(timestamp_gt,
                    TimestampGetDatum(tuple_ts), 
                    TimestampGetDatum(ws->lastTSSeen))))
    {
        ws->lastTSSeen = tuple_ts;
    }
}

/* FUNCTION windowStateGetMaxTS
 * POSTCONDITIONS: Returns the maximum timestamp we have observed on the base
 * stream corresponding to our window clause.
 */
Timestamp windowStateGetMaxTS(WindowState *ws) {
    return ws->lastTSSeen;
}

/* FUNCTION windowStateGetTS
 * ARGUMENTS: <tup> is an intermediate tuple.
 * POSTCONDITIONS: Fetches the timestamp that this windowstate wants to monitor
 * in the tuple.
 * NOTE: Leaks memory.
 */
Timestamp windowStateGetTS(WindowState *ws, HeapTuple tup) {
    bool isNull = false;
    Datum ts = heap_getattr(tup, ws->tsvar->varattno,
							ws->desc, &isNull);

    Assert(!isNull);

    return DatumGetTimestamp(ts);
}

/* Wrappers for the functions in timestamp.c.  These tend to leak memory, so
 * call them from within a temporary memory context. */
Timestamp ts_plus_interval(Timestamp t, Interval i) {
    return
        DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_span,
                    TimestampGetDatum(t), IntervalPGetDatum(&i)));
}

/*
bool ts_greater_than(Timestamp t1, Timestamp t2) {
    return
        DatumGetBool(DirectFunctionCall2(timestamp_gt,
                    TimestampGetDatum(t1), TimestampGetDatum(t2)));
}
*/
bool ts_greater_than(Timestamp t1, Timestamp t2) 
{
    return (t1 > t2);
}

bool ts_less_than(Timestamp t1, Timestamp t2) 
{
    return ts_greater_than(t2, t1);
}

/* minus */
Interval ts_mi(Timestamp t1, Timestamp t2) 
{
    Interval *retp = 
        DatumGetIntervalP(DirectFunctionCall2(timestamp_mi,
                            TimestampGetDatum(t1),
                            TimestampGetDatum(t2)));

    return *retp;
}

/* timestamp plus interval */
Timestamp ts_pl_i(Timestamp t, Interval i) 
{
    Timestamp ret = DatumGetTimestamp(DirectFunctionCall2(timestamp_pl_span,
                        TimestampGetDatum(t),
                        IntervalPGetDatum(&i)));
    return ret;
}

/* timestamp minus interval */
Timestamp ts_mi_i(Timestamp t, Interval i) 
{
    Timestamp ret = DatumGetTimestamp(DirectFunctionCall2(timestamp_mi_span,
                        TimestampGetDatum(t),
                        IntervalPGetDatum(&i)));
    return ret;
}

/* interval plus interval */
Interval i_pl_i(Interval i1, Interval i2) 
{
    Interval *retp =
        DatumGetIntervalP(DirectFunctionCall2(interval_pl,
                    IntervalPGetDatum(&i1),
                    IntervalPGetDatum(&i2)));
    return *retp;
}

/* interval minus interval */
Interval i_mi_i(Interval i1, Interval i2) 
{
    Interval *retp =
        DatumGetIntervalP(DirectFunctionCall2(interval_mi,
                    IntervalPGetDatum(&i1),
                    IntervalPGetDatum(&i2)));
    return *retp;
}

/* interval divided by interval */
double i_div_i(Interval i1, Interval i2) 
{
    double ret = DatumGetFloat8(DirectFunctionCall2(interval_div_interval,
                IntervalPGetDatum(&i1),
                IntervalPGetDatum(&i2)));
    return ret;
}


Interval i_mul(Interval i, float8 mul) 
{
    Interval *retp = 
        DatumGetIntervalP(DirectFunctionCall2(interval_mul,
                        IntervalPGetDatum(&i),
                        Float8GetDatum(mul)));
    return *retp;
}

/*
 * CONSTANTS
 */
static Interval * ONE_SECOND_P = NULL;

double interval_to_sec(Interval i) 
{
    /* Intervals can't be statically initialized */
    if (NULL == ONE_SECOND_P) {
        MemoryContext origcxt = MemoryContextSwitchTo(TopMemoryContext);
        ONE_SECOND_P = DatumGetIntervalP(
                            DirectFunctionCall3(interval_in, 
                                                CStringGetDatum("1 second"),
                                                ObjectIdGetDatum(InvalidOid),
                                                Int32GetDatum(-1)));
        MemoryContextSwitchTo(origcxt);

    }
    return i_div_i(i, *ONE_SECOND_P);
}


/* bool ts_geq(Timestamp t1, Timestamp t2) { */
/*     return  */
/*         DatumGetBool(DirectFunctionCall2(timestamp_ge, */
/*                     TimestampGetDatum(t1), TimestampGetDatum(t2))); */
/* } */

bool ts_geq(Timestamp t1, Timestamp t2) 
{
  return (t1 >= t2); 
}


bool ts_leq(Timestamp t1, Timestamp t2) {
    return ts_geq(t2, t1);
}

bool ts_equals(Timestamp t1, Timestamp t2) {
    return 
        DatumGetBool(DirectFunctionCall2(timestamp_eq,
                    TimestampGetDatum(t1), TimestampGetDatum(t2)));
}


Timestamp ts_max(Timestamp t1, Timestamp t2) {
    return
        DatumGetTimestamp(DirectFunctionCall2(timestamp_larger,
                    TimestampGetDatum(t1), TimestampGetDatum(t2)));
}

Timestamp ts_min(Timestamp t1, Timestamp t2) {
    return
        DatumGetTimestamp(DirectFunctionCall2(timestamp_smaller,
                    TimestampGetDatum(t1), TimestampGetDatum(t2)));
}


static int64 ONE_MILLION = 1000000L;

/* Returns the difference in usec between two timestamps. */
int64
tv_diff(struct timeval *earlier, struct timeval *later)
{
      return ((int64) (later->tv_sec) * ONE_MILLION 
              + (int64) (later->tv_usec) 
              - (int64) (earlier->tv_sec) * ONE_MILLION 
              - (int64) (earlier->tv_usec)
              );
}

/* Adds the indicated offset (in usec) to the timeval. */
void
tv_plus_int(const struct timeval *src, int64 offset, struct timeval *result)
{
      Assert(src != NULL);
      Assert(result != NULL);

      result->tv_sec = src->tv_sec + (long) (offset / ONE_MILLION);
      result->tv_usec = src->tv_usec + (long) (offset % ONE_MILLION);

      /* Carry. */
      if (result->tv_usec > ONE_MILLION) {
              Assert(result->tv_usec < 2 * ONE_MILLION);

              result->tv_usec -= ONE_MILLION;
              result->tv_sec++;
      } else if (result->tv_usec < 0L) {
              Assert(result->tv_usec >= -ONE_MILLION);
              result->tv_usec += ONE_MILLION;
              result->tv_sec--;
      }
}

/* Rounds the indicated timestamp to the nearest <win> time units, starting
 * from UNIX time 0 */
Timestamp round_timestamp(Timestamp ts, Interval win) 
{
    struct tm tmp_tm;
    fsec_t tmp_fsec;
    /* Construct the Timestamp that corresponds to the Epoch. */
    Timestamp start;
    Timestamp ret;
    int64 quotient;

    time_t start_sec = 0UL;

    gmtime_r(&start_sec, &tmp_tm);
    tm2timestamp(&tmp_tm, 0UL, NULL, &start);

    /* SPECIAL CASE: If we're rounding to the nearest 0 time units, then just
     * return the input. */
    interval2tm(win, &tmp_tm, &tmp_fsec);
    if (0UL == mktime(&tmp_tm) && 0UL == tmp_fsec) {
        return ts;
    }
    /* END SPECIAL CASE */

   
    /* Compute the quotient (ts - start) / win, 
     * then strip off the fractional parts */
    quotient = (int64)(
        DatumGetFloat8(
            DirectFunctionCall2(interval_div_interval,
                DirectFunctionCall2(timestamp_mi,
                    TimestampGetDatum(ts),
                    TimestampGetDatum(start)
                ),
                IntervalPGetDatum(&win)
            )
        )
    );

#ifdef DEBUG_WINDOWS
    elog(LOG, "round_timestamp(): start is '%s'", ts_cstr(start));
    elog(LOG, "round_timestamp(): quotient is %lld",
            quotient);
#endif /* DEBUG_WINDOWS */

    /* Then create the new start time: start + (win * quotient) */
    ret = DatumGetTimestamp(
        DirectFunctionCall2(timestamp_pl_span,
            TimestampGetDatum(start),
            DirectFunctionCall2(interval_mul,
                IntervalPGetDatum(&win),
                Float8GetDatum((double)(quotient))
            )
        )
    );

    return ret;
}



/*
 * INTERNAL FUNCTIONS
 */


/*
Interval ts_minus_ts(Timestamp t1, Timestamp t2) {
    Interval *ptr =
        DatumGetIntervalP(
                DirectFunctionCall2(timestamp_mi,
                    TimestampGetDatum(t1), TimestampGetDatum(t2)));
    return *ptr;
}
*/

/* FUNCTION first_window_past
 * ARGUMENTS: <startat> is the start of the first summary window.
 *      <rangeby> is the size of each summary window.
 *      <slideby> is the amount of time between adjacent windows (windows may
 *          overlap)
 *      <ts> represents a point in time.
 * POSTCONDITIONS: Returns the index (1-based!) of the first window whose 
 *      right hand side is STRICTLY GREATER THAN the indicated timestamp.
 */
int first_window_past(Timestamp startat, Interval *rangeby, Interval * slideby,
        Timestamp ts) 
{
    /* What we're computing here is:
     *
     * integer_part(2 + (ts - (startat + rangeby)) / slideby )
     *
     * Note that the first window is window 1 
     */
    double f_windownum = 
        2.0 + 
            DatumGetFloat8(
                DirectFunctionCall2(interval_div_interval,
                    DirectFunctionCall2(timestamp_mi,
                        TimestampGetDatum(ts), 
                        DirectFunctionCall2(timestamp_pl_span,
                            TimestampGetDatum(startat),
                            IntervalPGetDatum(rangeby)
                        )
                    ),
                    IntervalPGetDatum(slideby)
                )
            );
    int windownum = (int)f_windownum;

#ifdef DEBUG_WINDOWS
        elog(LOG, "first_window_past(%s): Window %d goes from %s to %s",
            ts_cstr(ts),
            windownum, 
            ts_cstr(window_begin(startat, slideby, windownum)),
            ts_cstr(window_end(startat, rangeby, slideby, windownum)));
#endif /* DEBUG_WINDOWS */

    /* SPECIAL CASE: Sometimes, rounding error causes us to undershoot
     * the window.  Correct for this by adding 1 to the window index as 
     * needed. */
    if (ts_geq(ts, window_end(startat, rangeby, slideby, windownum))) {
#ifdef DEBUG_WINDOWS
        elog(LOG, "first_window_past(): Correcting for rounding error...");
        elog(LOG, "    ...because %s >= %s",
                ts_cstr(ts),
                ts_cstr(window_end(startat, rangeby, slideby, windownum)));
#endif /* DEBUG_WINDOWS */
        windownum++;
    }

    return windownum;
}

/* FUNCTION first_window_no
 * ARGUMENTS: <startat> is the start of the first summary window.
 *      <rangeby> is the size of each summary window.
 *      <slideby> is the amount of time between adjacent windows (windows may
 *          overlap)
 *      <ts> represents a point in time.
 * POSTCONDITIONS: Returns the first summary window into which the indicated 
 *      time falls, or -1 if the tuple does not fall into any window.
 */
int first_window_no(Timestamp startat, Interval *rangeby, Interval * slideby,
        Timestamp ts) 
{
    bool inwindow = false;

    /* First we find the index of the first window whose right hand side is
     * STRICTLY GREATER THAN the indicated timestamp. */
    int windownum = first_window_past(startat, rangeby, slideby, ts);

    /* There are no windows less than 1. */
    if (windownum < 1) windownum = 1;

    /* If the tuple falls into the indicated window, it is the earliest window
     * that could contain the tuple.  If the tuple does not fall into the
     * window, then no other window can contain the tuple. */
    /* To check whether the ts falls into the window, we check whether:
     *
     * ts >= (beginning of window) 
     */
    inwindow =
        DatumGetBool(
            DirectFunctionCall2(timestamp_ge,
                TimestampGetDatum(ts),
                TimestampGetDatum(window_begin(startat, slideby, windownum))
            )
        );

    if (inwindow) {
        return windownum;
    } else {
        return -1;
    }

}

/* FUNCTION last_window_no
 * ARGUMENTS: <startat> is the start of the first summary window.
 *      <rangeby> is the size of each summary window.
 *      <slideby> is the amount of time between adjacent windows (windows may
 *          overlap)
 *      <ts> represents a point in time.
 * POSTCONDITIONS: Returns the last summary window into which the indicated 
 *      time falls, or -1 if the tuple does not fall into any window.
 */
int last_window_no(Timestamp startat, Interval *rangeby, Interval * slideby,
        Timestamp ts)
{
    /* First we find the max number of times we can slide without reaching the
     * indicated timestamp. */
    /* What we're computing here is:
     *
     * integer_part( (ts - startat) / slideby )
     *
     */
    double slidenum = 
        DatumGetFloat8(
            DirectFunctionCall2(interval_div_interval,
                DirectFunctionCall2(timestamp_mi,
                    TimestampGetDatum(ts), TimestampGetDatum(startat)
                ),
                IntervalPGetDatum(slideby)
            )
        );

    /* A window will start at (startat + slidenum * slideby).  If the window
     * that starts at that point in time contains the ts, that's the window;
     * otherwise, no window can contain the timestamp. */
    bool inwindow = !timestamp_past_window(ts, startat,
            rangeby, slideby, slidenum + 1);

    if (inwindow) {
         /* Note that the first window is window 1 */
        return (int)(slidenum + 1);
    } else {
        return -1;
    }
}

/* FUNCTION window_begin
 * ARGUMENTS: <startat> is the start of the first window.
 *      <slideby> is the amount of time between windows
 *      <windownum> is the window number.
 * POSTCONDITIONS: Returns the timestamp of the earliest time that could be 
 *      in the window.
 */
Timestamp window_begin(Timestamp startat, Interval *slideby, int windownum)
{
    /* What we compute is:
     *
     * startat + (slideby * (windownum - 1))
     *
     * Note that the first window is window 1.
     */
    Timestamp result = 
        DatumGetTimestamp(
            DirectFunctionCall2(timestamp_pl_span, 
                TimestampGetDatum(startat),
                DirectFunctionCall2(interval_mul,
                    IntervalPGetDatum(slideby),
                    Float8GetDatum((double)(windownum - 1.0))
                )
            )
        );

    return result;
}


/* FUNCTION window_end
 * ARGUMENTS: <startat> is the start of the first window.
 *      <rangeby> is the size of window.
 *      <slideby> is the amount of time between windows.
 *      <windownum> is the window number.
 * POSTCONDITIONS: Returns the timestamp of the last time that could be in the
 *      window.
 */
Timestamp window_end(Timestamp startat, Interval *rangeby, 
        Interval *slideby, int windownum)
{
    /* What we compute is:
     *
     * (start of window) + rangeby
     *
     * Note that the first window is window 1.
     */
    Timestamp windowstart = window_begin(startat, slideby, windownum);

    Timestamp result = 
        DatumGetTimestamp(
            DirectFunctionCall2(timestamp_pl_span, 
                TimestampGetDatum(windowstart),
                IntervalPGetDatum(rangeby)
            )
        );

    return result;
}

/* FUNCTION timestamp_past_window
 * ARGUMENTS: <ts> is the point in time you want to check.
 *      <startat> is the start of the first summary window.
 *      <rangeby> is the width of the summary windows.
 *      <slideby> is the amount of time between windows.
 *      <windownum> is the window you want to check.
 * POSTCONDITIONS: Returns true if the indicated timestamp is past the end of
 * the indicated summary window.
 *
 * NOTE: Windows start at 1!
 */
bool timestamp_past_window(Timestamp ts, Timestamp startat, 
        Interval *rangeby, Interval *slideby, int windownum) 
{
    /* To check whether the ts comes before the end of the window, we evaluate:
     *
     * ts < (end of window)
     */
    bool beforeend = 
        DatumGetBool(
            DirectFunctionCall2(timestamp_lt,
                TimestampGetDatum(ts),
                TimestampGetDatum(window_end(startat, rangeby, slideby, 
                        windownum))
            )
        );


    return (!beforeend);

}

/*
 *  FUNCTION createSlicedWindowState
 *
 *  POSTCONDITIONS: Initialized SlicedWindowState with everything all setup for
 *  further processing
 */
SlicedWindowState *createSlicedWindowState(WindowState *ws) 
{
	SlicedWindowState *sws = makeNode(SlicedWindowState);
	Interval first;

	sws->slices = ws->slices;
	sws->cur_slice = ws->slices;

	first = *((Interval *) lfirst(sws->cur_slice));
	
	sws->next_edge = ts_plus_interval (ws->startAt,first);

#ifdef DEBUG_WINDOWS
	elog(LOG,"Current slice: (%s), Next edge: (%s) ",
		 interval_cstr(&first), ts_cstr(sws->next_edge));
#endif
	return sws;
}

void
SlicedWindowAdvance(SlicedWindowState *sws) 
{
	Interval *slice;

	
#ifdef DEBUG_WINDOWS
	elog(LOG,"In SlicedWindowAdvance: ");
#endif

	sws->cur_slice = lnext(sws->cur_slice);
	
	if (NIL == sws->cur_slice) 
	{
		sws->cur_slice = sws->slices;
	}
	slice = (Interval *) lfirst(sws->cur_slice);
	sws->next_edge = ts_plus_interval (sws->next_edge,*slice);		
#ifdef DEBUG_WINDOWS
	elog(LOG,"New slice: (%s), Next edge: (%s) ",
		 interval_cstr(slice), ts_cstr(sws->next_edge));
#endif
}


/* FUNCTION windowStateGetLifetime
 *
 * POSTCONDITIONS: Determines when the tuple will become part of this window's
 * set of live tuples and when it will leave that set, and stores those times
 * in *birth_ts and *death_ts, respectively.
 *
 * If the tuple will never become live, sets *birth_ts to DT_NOEND.
 */
void
windowStateGetLifetime(WindowState *ws, 
					   HeapTuple tup,
					   Timestamp *birth_ts, 
					   Timestamp *death_ts)
{
    Timestamp ts = windowStateGetTS(ws, tup);
	
    int first_winno = first_window_no(ws->startAt, &(ws->rangeBy), 
            &(ws->slideBy), ts);
    int last_winno = last_window_no(ws->startAt, &(ws->rangeBy), 
            &(ws->slideBy), ts);

#ifdef DEBUG_WINDOWS
    elog(LOG, "Timestamp %s: First window is %d and last is %d.",
            ts_cstr(ts), first_winno, last_winno);
    /*
    elog(LOG, "    First window goes from %s to %s",
            ts_cstr(window_begin(ws->startAt, &(ws->slideBy), first_winno)),
            ts_cstr(window_end(ws->startAt, &(ws->rangeBy), &(ws->slideBy), 
                first_winno)));
            */

#endif /* DEBUG_WINDOWS */

    /* SPECIAL CASE: The tuple will never become live, because it does not
     * belong to any window. */
    if (-1 == first_winno) 
    {
        *birth_ts = DT_NOEND;
        return;
    }
    /* END SPECIAL CASE */

    /* If the tuple is ever live, it is live from the end of its earliest
     * window to the end of the window after its latest window. */
    *birth_ts = window_end(ws->startAt, &(ws->rangeBy), &(ws->slideBy),
            first_winno);
    *death_ts = window_end(ws->startAt, &(ws->rangeBy), &(ws->slideBy), 
            last_winno + 1);
}

/* FUNCTION windowStateGetNextSlide
 * POSTCONDITIONS: Returns the earliest point in time strictly after <ts> at
 * which a window will end.
 *
 * NOTE: Leaks memory!
 */
Timestamp
windowStateGetNextSlide(WindowState *ws, Timestamp ts)
{
    /* Compute the first window with a right-hand side past the indicated
     * timestamp.
     */
    int window_num = first_window_past(ws->startAt, &(ws->rangeBy), 
            &(ws->slideBy), ts);

#ifdef DEBUG_WINDOWS
    elog(LOG, "windowStateGetNextSlide(): Start at is %s; now is %s",
            ts_cstr(ws->startAt), ts_cstr(ts));
    elog(LOG, "    Window index is %d", window_num);
    elog(LOG, "    Window goes from %s to %s", 
            ts_cstr(window_begin(ws->startAt, &(ws->slideBy), window_num)),
            ts_cstr(window_end(ws->startAt, &(ws->rangeBy), 
                    &(ws->slideBy), window_num)));
#endif /* DEBUG_WINDOWS */

    Assert(window_num > 0);

    return 
        window_end(ws->startAt, &(ws->rangeBy), &(ws->slideBy), window_num);
}


