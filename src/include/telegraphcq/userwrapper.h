/*-------------------------------------------------------------------------
 *
 * userwrapper.h
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * TBD - Needs documentation
 *
 * $Id: userwrapper.h,v 1.14 2005/07/26 20:00:31 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef USERWRAPPER_H
#define USERWRAPPER_H
#ifdef __darwin__
#include "sys/time.h"
#endif
#include "access/tupdesc.h"

#include "access/htup.h"
#include "funcapi.h"
#include "parser/parse_wrapper.h"


typedef enum FUNCTIONMODE
{
	MODE_INIT,
	MODE_NEXT,
	MODE_DONE,
	MODE_END,
	MODE_RESCAN					/* pull source is done with the current
								 * scan, and requires a new probe tuple */
}	FunctionMode;

/* Our knowledge of whether we're generating timestamps for the current stream.
 */
enum wrapper_ts_gen_state {
    DONT_KNOW = 0,      /* We don't know, because we haven't seen any tuples.*/
    GENERATING,         /* We're generating the timestamps for this stream. */
    NOT_GENERATING      /* We're not generating timestamps, because the
                           stream comes already timestamped. */
};


typedef struct wrapperstate
{
    /***************************************************************************
     * STATE SET BY THE WRAPPER CLEARINGHOUSE
     **************************************************************************/
	int			fd;				/* the NONBLOCKING filedescriptor for the
								 * source */
	TupleDesc	tdesc;

	int			returnTupleLength;		/* length of the return tuple
										 * array (see below) */
	HeapTuple	probetuple;
	int		   *mapping_info;
	char	   *error_message;

	AttInMetadata *aim;
	AttOutMetadata *aom;
	MemoryContext perTupleContext;
                                /* tuple processing data */

	Queue	   *pending_probes;

	bool		connectionReady;
                                /* Set to TRUE when there is data available on
                                 * the network socket. */
    bool        timeoutExpired;
                                /* Set to TRUE when wakeup_time has passed */

	void	   *wrapper_info;

    int         tupleCounter;
                                /* Incremented every time the wrapper spits out
                                 * a tuple. */

    Timestamp   latestTs;

    /***************************************************************************
     * STATE SET BY THE WRAPPER 
     **************************************************************************/
    void	   *udfstate;		/* user defined wrapper state. initialized
								 * on init, passed on all calls, destroyed
								 * on close */

	FunctionMode mode;			/* used to determine which function to
								 * call next */

    Datum	   *returnTuple;	/* the return tuple, or NULL if no tuple
								 * yet.  Allocated by the wrapper
                                 * clearinghouse, filled by the wrapper. */
	char	   *returnTupleIsNull;		
                                /* null indicators for each element of
                                 * returnTuple.  Each position must be either
                                 * 'n' or ' '.  Allocated by the wrapper
                                 * clearinghouse, filled by the wrapper.  */

    bool        returnTupleIsPunct;
                                /* Set to TRUE to indicate that the return
                                 * tuple is a punctuation tuple. */

	bool		hasTuple;       /* Set to TRUE when <returnTuple> contains a 
                                 * valid tuple.  The wrapper clearinghouse
                                 * won't touch returnTuple until this flag is
                                 * set to TRUE. */


	bool		doneWithProbe;	/* wrapper sets this flag when it reaches
								 * the end of the results for a particular
								 * probe. The WCH will then call rescan on
								 * the wrapper before calling next. */

	bool		hasBufferedData;
                                /* Indicates that the wrapper has data buffered
                                 * internally.  Currently ignored by the
                                 * wrapper clearinghouse. */

    enum wrapper_ts_gen_state tsState;
                                /* Tells the information the wrapper has about
                                 * whether it is generating timestamps.
                                 * Default value is DONT_KNOW */

    /* Sleep support. */
    struct timeval wakeup_time; /* Any value other than WCH_NOT_A_TIME 
                                 * indicates when the wrapper clearinghouse 
                                 * should call the next() function of this 
                                 * wrapper. */
    bool        asleep;         /* Set to TRUE when the wrapper wants to go to 
                                 * sleep until the time indicated by
                                 * wakeup_time.  
                                 * The wrapper clearinghouse will not make any
                                 * callbacks to a wrapper that is asleep. */
    
}	wrapperstate;

int			establishConnection(void *wrapper_info);
bool		hasNewProbeTuple(wrapperstate * s);
bool		isBindingColumn(wrapperstate * s, int colno);
int			getNumBindings(wrapperstate * s);
char	   *getBindingValueByNumber(wrapperstate * s, int bindingno);
char	   *getBindingValueByStreamCol(wrapperstate * s, int streamcol);

/* Convenience functions for timestamp arithmetic (for sleep support) */
int64 tv_diff(struct timeval *earlier, struct timeval *later);
void  tv_plus_int(const struct timeval *src, int64 offset, 
        struct timeval *result);


#endif   /* USERWRAPPER_H */
