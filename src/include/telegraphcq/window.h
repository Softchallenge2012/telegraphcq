/*-------------------------------------------------------------------------
 *
 * window.h
 *	  prototypes for window.c
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: window.h,v 1.8 2005/07/26 20:00:31 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef WINDOW_H
#define WINDOW_H

/*@BwndaggWH */
#include "nodes/plannodes.h"
#include "nodes/execnodes.h"

extern WindowState * createWindowState(WindowExpr *we);
extern SlicedWindowState *createSlicedWindowState(WindowState *ws);
extern void SlicedWindowAdvance(SlicedWindowState *sws);
extern void windowStateSetTupleDesc(WindowState *ws, TupleDesc td);
extern void freeWindowState(WindowState *wndstate);
extern void freeTupleList(TupleList * p);
extern void ReScanWindowState(Window *node, ExprContext exprCtxt, Plan *parent);
extern Timestamp windowStateGetFirstChange(WindowState *ws);
extern void windowStateGetLifetime(WindowState *ws, 
        HeapTuple tup,
        Timestamp *birth_ts, 
        Timestamp *death_ts);
extern void windowStateUpdateMaxTS(WindowState *ws, HeapTuple tup);
extern Timestamp windowStateGetMaxTS(WindowState *ws);
extern Timestamp windowStateGetNextSlide(WindowState *ws, Timestamp ts);
extern Timestamp windowStateGetTS(WindowState *ws, HeapTuple tup);

extern int first_window_past(Timestamp startat, Interval *rangeby, 
        Interval * slideby, Timestamp ts);
extern int first_window_no(Timestamp startat, Interval *rangeby, 
        Interval * slideby, Timestamp ts);
extern int last_window_no(Timestamp startat, Interval *rangeby, 
        Interval * slideby, Timestamp ts);
extern Timestamp window_begin(Timestamp startat, Interval *slideby, 
        int windownum);
extern Timestamp window_end(Timestamp startat, Interval *rangeby, 
        Interval *slideby, int windownum);
bool timestamp_past_window(Timestamp ts, Timestamp startat, 
        Interval *rangeby, Interval *slideby, int windownum);

extern Timestamp ts_plus_interval(Timestamp t, Interval i);
extern bool ts_greater_than(Timestamp t1, Timestamp t2);
extern bool ts_less_than(Timestamp t1, Timestamp t2);
extern bool ts_geq(Timestamp t1, Timestamp t2);
extern bool ts_leq(Timestamp t1, Timestamp t2);
extern bool ts_equals(Timestamp t1, Timestamp t2);
extern Timestamp ts_max(Timestamp t1, Timestamp t2);
extern Timestamp ts_min(Timestamp t1, Timestamp t2);

extern Interval ts_mi(Timestamp t1, Timestamp t2);
extern Timestamp ts_pl_i(Timestamp t, Interval i);
extern Timestamp ts_mi_i(Timestamp t, Interval i);

extern Interval i_pl_i(Interval i1, Interval i2);
extern Interval i_mi_i(Interval i1, Interval i2);

extern double i_div_i(Interval i1, Interval i2);
extern Interval i_mul(Interval i, float8 mul);

extern double interval_to_sec(Interval i);

extern int64 tv_diff(struct timeval *earlier, struct timeval *later);
extern void
tv_plus_int(const struct timeval *src, int64 offset, struct timeval *result);

extern Timestamp round_timestamp(Timestamp ts, Interval win);


#endif   /* WINDOW_H */
