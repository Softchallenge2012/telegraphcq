/*-------------------------------------------------------------------------
 *
 * quikwrap.c
 *
 *	  Built-in wrapper that temps a table to disk, then reads it back in
 *	  through the wrapper clearinghouse.
 *
 *	  The binary format of this temporary file is:
 *	        -> First 4 bytes contain the number of tuples in the table.
 *	        -> The next 4 bytes are a string length.
 *	        -> After that is the name of the table.
 *	        -> Thereafter, the tuples are stored as Pascal-style binary
 *	               strings with 4 bytes of length information.
 *
 *    We currently don't do anything to deal with big/little-endian issues.
 *
 *    We also only do minimal checking of the binary tuples when reading them
 *    back in.
 *
 *	  Based on the CSV wrapper code.
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/quikwrap.c,v 1.6 2004/10/12 23:05:59 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "postgres.h"
#include "fmgr.h"
#include "telegraphcq/userwrapper.h"
#include "parser/parse_wrapper.h"
#include "catalog/pg_type.h"
#include <time.h>
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/builtins.h"
#include "lib/stringinfo.h"
#include "executor/spi.h"
#include "telegraphcq/histogram.h"
    /* For get_int_field_val() function. */

#define BUFSIZE 5000
#define BINDINGBUFFERSIZE 2000

#define QUIK_BUFLEN 16384
    /* Length of our read/write buffers.  All serialized tuples are guaranteed
     * to fit into this length. */

/* Uncomment the following line to synchronize the clocks of every quikwrap 
 * wrapper in a process. */
#define QW_SINGLE_CLOCK

#ifdef QW_SINGLE_CLOCK
/* Global variable that holds the number of quikwrap wrappers in this 
 * process.  Incremented the in quik_init() and decremented in quik_done().
 *
 * See comment immediately below. */
int g_num_active_quikwraps = 0;

/* If there are multiple quikwraps running inside a process, they all use the
 * same clock to determine when to emit tuples.  The following timestamp
 * functions as "time zero" and is reset every time
 *
 * TODO: Add a flag to turn this behavior off!
 */
struct timeval g_the_epoch = {0, 0};
#endif /* QW_SINGLE_CLOCK */



PG_FUNCTION_INFO_V1(quik_dump_table);

PG_FUNCTION_INFO_V1(quik_init);
PG_FUNCTION_INFO_V1(quik_next);
PG_FUNCTION_INFO_V1(quik_done);




Datum
			quik_dump_table(PG_FUNCTION_ARGS);
Datum
			quik_init(PG_FUNCTION_ARGS);
Datum
			quik_next(PG_FUNCTION_ARGS);
Datum
			quik_done(PG_FUNCTION_ARGS);

/* Status of a connection. */
enum QuikStatus{
    NOT_INITIALIZED = 0,
    READING_NUM_TUPS,
        /* Have initialized but haven't read anything from the socket. */
    READING_REL_NAME_LEN,
    READING_REL_NAME,
        /* Have read the number of tuples; now reading the stream name. */
    READING_TUPLE_LEN,
    READING_TUPLE,
        /* Have read the stream name; now reading the tuples themselves. */
    WAITING_TO_OUTPUT_TUPLE,
        /* Have read a tuple, but we're waiting until the appropriate timeout
         * has elapsed before outputting it. */
    DONE = -1
};

/*
 * this is the private state for the user defined function.
 * An instance will be allocated by quik_init and freed
 * by quik_done.
 */
typedef struct quikstate
{
    enum QuikStatus     status;

	char		circ_buf[QUIK_BUFLEN];
                    /* A circular buffer, but we cleverly arrange things so
                     * that any group of bytes that is used together is
                     * contiguous in the buffer. */
    char        overflow_buf[QUIK_BUFLEN];
                    /* ...and here's how we do it: by copying the beginning of
                     * the buffer into this buffer when the object we want
                     * wraps around.
                     * NOTE: Need to keep these two buffers adjacent and keep
                     * QUIK_BUFLEN a multiple of the word alignment.
                     */

    int         *tuplen_ptr;
                    /* Length of a (packed) tuple stored in buf.  Actually a
                     * pointer into circ_buf. */

    char        *orig_source_name;
                    /* The original relation that generated the dump. */

    int         read_ix;
                    /* The current read position in circ_buf */

    int         bytes_in_buf;
                    /* How many bytes of valid data are in the buffer. */

	int			readidx;
	int			writeidx;
                    /* Indexes into buf */

	int			tuplecount;
    int         tupletotal;
                    /* How many tuples we've read, and how many are in the
                     * stream. */

    int         namelen;

    TupleDesc   srcdesc;
                    /* Descriptor for the relation that was dumped to disk. */

	int			numrescans;
    /*
	char	   *error_indicator;
	char	   *done_indicator;
	int			done_indicator_size;
	int			error_indicator_size;
    */

#ifndef QW_SINGLE_CLOCK
    struct timeval start_time;
                    /* The time that this stream started. 
                     * Currently set to the time that we finish reading the
                     * relation name off the wire.
                     */
#endif /* ! QW_SINGLE_CLOCK */

    bool        last_read_failed;
                    /* Set to TRUE if the last read operation failed to return
                     * any data. */

} quikstate;


/*******************************************************************************
 * Prototypes of internal functions.
 ******************************************************************************/
/* Event handlers for the different states of quik_next() */
/* These all return TRUE if they manage to make any forward progress. */
bool handle_reading_num_tups(wrapperstate *ws, quikstate *udfstate);

bool handle_reading_rel_name_len(wrapperstate *ws, quikstate *udfstate);

bool handle_reading_rel_name(wrapperstate *ws, quikstate *udfstate);

bool handle_reading_tuple_len(wrapperstate *ws, quikstate *udfstate);

bool handle_reading_tuple(wrapperstate *ws, quikstate *udfstate);

bool handle_waiting_to_output_tuple(wrapperstate *ws, quikstate *udfstate);

/* Meta-handlers that handle subroutines of the quik_next() event handlers...
 */
bool handle_reading_generic(int fd, quikstate *udfstate,  
        const int num_to_read, bool *read_error);

void fill_buffer(int fd, quikstate *udfstate, bool *read_error);

void *get_read_ptr(quikstate *udfstate);

void consume_buffer(quikstate *udfstate, int num_to_munch);

Datum tv2text(struct timeval *tp);

/*******************************************************************************
 * External functions
 ******************************************************************************/

/* FUNCTION quik_dump_table
 * ARGUMENTS: The following arguments are packed into the PG arguments array:
 *      1) Relation name (as string)
 *      2) Output filename (as string)
 * PRECONDITIONS: The indicated relation has the same schema as your output,
 *      except that, in place of a timestamp column, there is a special first
 *      column that gives a relative time offset in microseconds.
 * POSTCONDITIONS: Fetches the indicated table, and writes out a binary version
 *      of that table in a format that the other functions in this file can
 *      understand.
 */
Datum
quik_dump_table(PG_FUNCTION_ARGS)
{
    /* Postgres function arguments */
    char        *relname = PG_GETARG_CSTRING(0);
    char        *output_fname = PG_GETARG_CSTRING(1);

    /* Locals */
    FILE        *output_f = NULL;
    TupleDesc   desc;
    char        buf[QUIK_BUFLEN];
                    /* Should be long enough to hold any tuple we'll have. */
    char        *firstcolname = NULL;
    Oid         firstcoltype;
    StringInfo	sql = makeStringInfo();
    int         proc = -1;
    int         tupnum = -1;
    int         return_code = -1;
    int         namelen = -1;

    /* Check args */
    if (NULL == relname || NULL == output_fname) {
        elog(ERROR, "usage: quik_dump_table(relation name, output filename)");
    }

    /* Attempt to open output file. */
    output_f = fopen(output_fname, "w+");

    if (NULL == output_f) {
        elog(ERROR, "quik_dump_table(): Couldn't open output file '%s'",
                output_fname);
    }

    /* See if the relation exists. */
    desc = RelationNameGetTupleDesc(relname);

    if (NULL == desc) {
        elog(ERROR, "quik_dump_table(): Relation %s doesn't exist.", relname);
    }

    /* Get information about the first column in the relation. */
    firstcolname = desc->attrs[0]->attname.data;

    firstcoltype = desc->attrs[0]->atttypid;
        /* TODO: Verify that this is an integer. */

    /* Use the Server Programming Interface to fetch the relation sorted by the
     * first column. */
    appendStringInfo(sql, "SELECT * FROM %s ORDER BY %s", relname, 
            firstcolname);
    
    if ((return_code = SPI_connect()) < 0) {
        fclose(output_f);
		elog(ERROR, "quik_dump_table(): SPI_connect returned %d.", return_code);
        /* Unreachable */
	}

	/* Execute the query. */
	SetQuerySnapshot();		/* Don't know why we need to do this. */
	return_code = SPI_exec(sql->data, 0);
		/* The 0 here for number of tuples to retrieve apparently means to get
		 * as many tuples as possible. */
	proc = SPI_processed;

	elog(LOG, "quik_dump_table(): Query generated %d tuples.", proc);

    /* SPECIAL CASE: Empty result. */
    if (0 == proc) {
        SPI_finish();
        fclose(output_f);
        
        elog(ERROR, "quik_dump_table(): No tuples in relation.");

        /* Unreachable */
    }
    /* END SPECIAL CASE */

    /* Write the number of tuples out. */
    fwrite(&proc, sizeof(proc), 1, output_f);

    /* Write out the name of the table, as a Pascal string. */
    namelen = strlen(relname);
    fwrite(&namelen, sizeof(namelen), 1, output_f);
    fwrite(relname, namelen, 1, output_f);
    
    for (tupnum = 0; tupnum < proc; tupnum++) {
        HeapTuple   current_tup = SPI_tuptable->vals[tupnum];
        bool result = heaptuple_serialize((char*)current_tup, buf, 
                QUIK_BUFLEN, (char*)desc);
        int         len = -1;

        if (false == result) {
            SPI_finish();
            fclose(output_f);
            elog(ERROR, "quik_dump_table(): Buffer too small for tuple %d.", 
                    tupnum);
        }

        /* The serialization process sticks the length of the serialized tuple
         * into the first four bytes. */
        len = (sizeof(uint32)) + *(int*)buf; 

        fwrite(buf, len, 1, output_f);
    }


    /* Clean up after ourselves. */

    /* TODO: Should we free relname_list, relvar, etc.? */

    SPI_finish();
    
    fclose(output_f);

    PG_RETURN_BOOL(TRUE);
}

/* FUNCTION quik_init
 * ARGUMENTS: A pointer to a wrapperstate struct.
 * PRECONDITIONS: We're connected to a socket that's going to feed us the
 *      binary stream that quik_dump_table() wrote out to disk. 
 * POSTCONDITIONS: 
 */
Datum
quik_init(PG_FUNCTION_ARGS)
{
    /* Function arguments: */
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

    /* Locals: */
	/*key_value_entry *entry = NULL;*/

	struct quikstate *udfstate = NULL;

	/* check that tcqtime is the last attribute of the stream */
	if (ws->tdesc->attrs[ws->tdesc->natts - 1]->atttypid != TIMESTAMPOID)
	{
		ws->error_message = "The last column of any stream which uses the "
                "quikwrap wrapper must have datatype TIMESTAMP";
		PG_RETURN_BOOL(FALSE);
	}

    /* Allocate our persistent state. */
	ws->udfstate = palloc(sizeof(struct quikstate));
	ws->hasTuple = false;

	udfstate = (struct quikstate *) ws->udfstate;
	memset(udfstate->circ_buf, 'T', QUIK_BUFLEN);		/* debugging code */
	memset(udfstate->overflow_buf, 'T', QUIK_BUFLEN);		/* debugging code */

    udfstate->status = NOT_INITIALIZED;

	udfstate->tuplecount = 0;
	udfstate->namelen = 0;
	udfstate->tuplen_ptr = NULL;
	udfstate->read_ix = 0;
	udfstate->tupletotal = 0;
	udfstate->numrescans = 0;

    /* Fetch the tuple descriptor of the stream. */
    Assert(ws->aim != NULL);
    udfstate->srcdesc = NULL;


#ifndef QW_SINGLE_CLOCK
    memset(&(udfstate->start_time), 0x0, sizeof(udfstate->start_time));
#endif /* ! QW_SINGLE_CLOCK */

    udfstate->last_read_failed = false;

	if (ws->fd < 0)
	{
        /* Not sure why this would ever be true. */
        elog(LOG, "quik_init(): Establishing connection.");
		ws->fd = establishConnection(ws->wrapper_info);
		ws->mode = MODE_RESCAN;
	}
	else
		ws->mode = MODE_NEXT;

#ifdef QW_SINGLE_CLOCK
    /* Establish a baseline timestamp, if this is the first quikwrap out of the
     * gate. */
    if (0 == g_num_active_quikwraps) {
        gettimeofday(&g_the_epoch, NULL);
    }
    g_num_active_quikwraps++;
#endif /* QW_SINGLE_CLOCK */

	if (ws->fd < 0)
	{
		ws->error_message = "unable to open a connection to the wrapper server";
		PG_RETURN_BOOL(FALSE);
	}
	else {
        /* Done with initialization. */
        udfstate->status = READING_NUM_TUPS;
		PG_RETURN_BOOL(TRUE);
    }
}

/* FUNCTION quik_next()
 * ARGUMENTS: A pointer to a wrapperstate struct.
 * PRECONDITIONS: You've called quik_init() on the struct.
 * POSTCONDITIONS: Depending on the state of the connection, reads data and/or
 *      creates a tuple.
 */
Datum
quik_next(PG_FUNCTION_ARGS)
{
    /* Arguments */
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

    /* Locals */
	struct quikstate *udfstate = (struct quikstate *) ws->udfstate;
    bool        keepgoing = false;
    bool        first_time = true;

    /* Repeat as long as we keep making forward progress. */
    do { 
        udfstate->last_read_failed = false;
        
        switch (udfstate->status) {
            case READING_NUM_TUPS:
                /* Have initialized but haven't read anything from the socket. 
                 */
                keepgoing = handle_reading_num_tups(ws, udfstate);
                break;

            case READING_REL_NAME_LEN:
                /* Have read the number of tuples; now reading the length
                 * of the stream name. */
                keepgoing = handle_reading_rel_name_len(ws, udfstate);
                break;

            case READING_REL_NAME:
                /* Have read the number of tuples; now reading the stream 
                 * name itself. */
                keepgoing = handle_reading_rel_name(ws, udfstate);
                break;

            case READING_TUPLE_LEN:
                keepgoing = handle_reading_tuple_len(ws, udfstate);
                break;

            case READING_TUPLE:
                keepgoing = handle_reading_tuple(ws, udfstate);
                break;
    
            case WAITING_TO_OUTPUT_TUPLE:
                keepgoing = handle_waiting_to_output_tuple(ws, udfstate);
                break;

            default:
                elog(ERROR, "quik_next(): Invalid wrapper state!");
                /* unreachabe. */
                break;
        }

        if (udfstate->bytes_in_buf > 0) {
            ws->hasBufferedData = true;
        } else {
            ws->hasBufferedData = false;
        }

        /* Detect a closed connection. */
        if (first_time && udfstate->last_read_failed) {
            /* The connection is dead; send condolences to the wrapper
             * clearinghouse, once we've emptied out our buffer. */
            if (false == ws->hasBufferedData) {
                ws->mode = MODE_DONE;
            }
        }

        first_time = false;

    } while (true == keepgoing);

    /* Did an error occur? */
    if (NULL != ws->error_message) {
        PG_RETURN_BOOL(FALSE);
    } else {
        PG_RETURN_BOOL(TRUE);
    }
}




Datum
quik_done(PG_FUNCTION_ARGS)
{
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	struct quikstate *udfstate = (struct quikstate *) s->udfstate;

    /* Calculate how long we spent in this wrapper. */
    int sec_elapsed = -1;
    struct timeval now;
    gettimeofday(&now, NULL);
#ifdef QW_SINGLE_CLOCK
    sec_elapsed = now.tv_sec - g_the_epoch.tv_sec;
#else 
    sec_elapsed = now.tv_sec - udfstate->start_time;
#endif
	elog(NOTICE, "quikwrapper processed %d tuples in %d seconds", 
            udfstate->tuplecount, sec_elapsed);
    pfree(udfstate->orig_source_name);
	pfree(udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;

#ifdef QW_SINGLE_CLOCK
    g_num_active_quikwraps--;
#endif /* QW_SINGLE_CLOCK */

	PG_RETURN_BOOL(TRUE);
}


/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/


bool handle_reading_num_tups(wrapperstate *ws, quikstate *udfstate) 
{
    bool finished = handle_reading_generic(ws->fd, udfstate,
            sizeof(udfstate->tupletotal), &(udfstate->last_read_failed));

    if (finished) {
        int *ptr = (int*)(get_read_ptr(udfstate));
        udfstate->tupletotal = *ptr;
        consume_buffer(udfstate, sizeof(udfstate->tupletotal));

        elog(LOG, "Expecting %d tuples.", udfstate->tupletotal);

        /* Move to next state. */
        udfstate->status = READING_REL_NAME_LEN;
        return true;
    }

    return false;
}

bool handle_reading_rel_name_len(wrapperstate *ws, quikstate *udfstate) 
{
    bool finished = handle_reading_generic(ws->fd, udfstate,
            sizeof(udfstate->namelen), &(udfstate->last_read_failed));

    if (finished) {
        int *ptr = (int*)(get_read_ptr(udfstate));
        udfstate->namelen = *ptr;
        consume_buffer(udfstate, sizeof(udfstate->namelen));

        /* Move to next state. */
        udfstate->status = READING_REL_NAME;

        Assert(udfstate->namelen < QUIK_BUFLEN - 1);

        return true;
    }

    return false;
}

bool handle_reading_rel_name(wrapperstate *ws, quikstate *udfstate) 
{
    bool finished = handle_reading_generic(ws->fd, udfstate,
            udfstate->namelen, &(udfstate->last_read_failed));

    if (finished) {

        /* Allocate enough space for the trailing NULL! */
        udfstate->orig_source_name = palloc(udfstate->namelen + 1);
        memcpy(udfstate->orig_source_name, get_read_ptr(udfstate), 
                udfstate->namelen);
        udfstate->orig_source_name[udfstate->namelen] = '\0';
        elog(LOG, "quikwrap: Reading a dump of relation '%s'",
                udfstate->orig_source_name);

        consume_buffer(udfstate, udfstate->namelen);

        /* Fetch a tuple descriptor for this relation, so we can decode the
         * dumped stream. */
        udfstate->srcdesc = RelationNameGetTupleDesc(udfstate->orig_source_name);

        if (NULL == udfstate->srcdesc) {
            ws->error_message = "Can't get descriptor for relation.";
            return false;
        }

        /* TODO: Check for total tuple desciptor equivalence. */

        /* For now we just verify that the two relations have the same number
         * of attributes. */
        if (udfstate->srcdesc->natts != ws->returnTupleLength) {
            ws->error_message = 
                "Dump and target stream have different numbers of columns";
            return false;
        }
        
        /* Initialize the "starting" timestamp. */
#ifndef QW_SINGLE_CLOCK
        gettimeofday(&(udfstate->start_time), NULL);
#endif /* ! QW_SINGLE_CLOCK */
        
        /* Move to next state. */
        udfstate->status = READING_TUPLE_LEN;
        return true;
    }

    return false;
} 

bool handle_reading_tuple_len(wrapperstate *ws, quikstate *udfstate) 
{
    bool finished = handle_reading_generic(ws->fd, udfstate,
            sizeof(*(udfstate->tuplen_ptr)), &(udfstate->last_read_failed));

    if (finished) {
        udfstate->tuplen_ptr = (int*)(get_read_ptr(udfstate));

        /* NOTE: We do NOT advance the read pointer here, because
         * heaptuple_deserialize expects to see the tuple length adjacent to
         * the tuple itself. */

        if (0 == *(udfstate->tuplen_ptr) 
            || *(udfstate->tuplen_ptr) > 
                QUIK_BUFLEN - sizeof(*(udfstate->tuplen_ptr))) 
        {
            elog(NOTICE, "Invalid tuple length %u.", *(udfstate->tuplen_ptr));
            ws->error_message = "Invalid tuple length.";
            return false;
        }

        /* Move to next state. */
        udfstate->status = READING_TUPLE;

        return true;
    }
    
    return false;
}

bool handle_reading_tuple(wrapperstate *ws, quikstate *udfstate) 
{
    bool finished = handle_reading_generic(ws->fd, udfstate,
            *(udfstate->tuplen_ptr) + sizeof(*(udfstate->tuplen_ptr)),
            &(udfstate->last_read_failed));
        /* The length field of the serialized tuple does NOT include its own 
         * length, and we keep the length in the buffer until we're done with
         * the tuple. */

    if (finished) {
        MemoryContext oldcxt;
        int64       current_timeout = -1;

        /* Create a new tuple based on the one we've just read. */
        oldcxt = MemoryContextSwitchTo(ws->perTupleContext);
        { /* Begin per tuple memory context. */
            /* First we need to deserialize the tuple. */
            HeapTuple   cur_tup;
            int         attrnum = -1;
            bool result = heaptuple_deserialize((char*)(
                        get_read_ptr(udfstate)),
                                /* Includes both length and tuple. */
                        (char**)(&cur_tup),
                        NULL    /* typeinfo argument is ignored. */);

            if (false == result) {
                ws->error_message = "Error deserializing tuple.";
                return false;
            }
           
            for (attrnum = 2; attrnum <= udfstate->srcdesc->natts; attrnum++) {
                bool isnull = false;
                ws->returnTuple[attrnum - 2] = 
                    heap_getattr(cur_tup, attrnum, udfstate->srcdesc, &isnull);

                if (isnull) {
                    ws->returnTupleIsNull[attrnum - 2] = 'n';
                } else {
                    ws->returnTupleIsNull[attrnum - 2] = ' ';
                }
            }
            
            /* Use the first column as timing information. */
            current_timeout = 
                get_int_field_val(cur_tup, udfstate->srcdesc, 1, false);
                    /* Function in histogram.h */

            /* Set the timeout that will wake us up at the appropriate time. */
#ifdef QW_SINGLE_CLOCK
            tv_plus_int(&(g_the_epoch), current_timeout,
                    &(ws->wakeup_time));
#else 
            tv_plus_int(&(udfstate->start_time), current_timeout,
                    &(ws->wakeup_time));
#endif /* QW_SINGLE_CLOCK */


            /* We put the artifical timestamp instead of the real-time one into
             * the tuple, so that tuples will not jump from one window to the
             * next. */
            /* TODO: Convert directly to timestamp without going through text
             * part. */
#if 0
            elog(LOG, "quikwrap: timeval is <%d, %d>", 
                    (int)ws->wakeup_time.tv_sec, (int)ws->wakeup_time.tv_usec);
            elog(LOG, "quikwrap: Will output tuple %d at %s",
                    udfstate->tuplecount,
                    VARDATA((DatumGetTextP(tv2text(&(ws->wakeup_time))))));
            {
                struct timeval cur_time;
                gettimeofday(&cur_time, NULL);
                elog(LOG, "quikwrap:    Current time is %s",
                    VARDATA((DatumGetTextP(tv2text(&(cur_time))))));
            }
#endif /* 0 */                     
            ws->returnTuple[ws->returnTupleLength - 1] =
                    DirectFunctionCall1(
                        text_timestamp,
                        tv2text(&(ws->wakeup_time)));
            ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';

            /* The contents of ws->returnTuple are pointers into cur_tup, so
             * it's important that we don't call heap_freetuple() on it here!
             */

        } /* End per tuple memory context. */
        MemoryContextSwitchTo(oldcxt);

        consume_buffer(udfstate, 
            *(udfstate->tuplen_ptr) + sizeof(*(udfstate->tuplen_ptr)));


        /* Move to next state. */
        udfstate->status = WAITING_TO_OUTPUT_TUPLE;

        ws->hasTuple = false;
            /* We wait until the appropriate timeout has elapsed before
             * emitting the tuple. */

        ws->asleep = true;

        /* We return FALSE here because we want to go to sleep. */
        return false;
    }

    return false;
}

bool handle_waiting_to_output_tuple(wrapperstate *ws, quikstate *udfstate) 
{
    /* This function should only be called when the current timeout has
     * elapsed. */

    /* Don't forget Mr. Timestamp!  We want this timestamp to represent the
     * time that this tuple was sent up to the wrapper clearinghouse, so we
     * create it here. */
#if 0
    /* or not. */
    MemoryContext oldcxt = MemoryContextSwitchTo(ws->perTupleContext);
    { /* Begin per tuple memory context. */
        ws->returnTuple[ws->returnTupleLength - 1] =
                DirectFunctionCall1(
                    text_timestamp,
                    DirectFunctionCall1(timeofday,PointerGetDatum(NULL)));
        ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';
    } /* End per tuple memory context. */
    MemoryContextSwitchTo(oldcxt);
#endif /* 0 */
    
    /* Indicate to the caller that the tuple is ready for processing. */
    ws->hasTuple = true;

    /* Move to the next state. */
    udfstate->status = READING_TUPLE_LEN;

    /* We need to return FALSE here to give the caller time to pass the
     * tuple back through the clearinghouse. */
    return false;
}





/* Meta-handlers that handle subroutines of the quik_next() event handlers...
 */

/* Read data into the buffer until we have enough there to consume the
 * indicated number of bytes.
 *
 * Also makes sure the data to be consumed is contiguous.
 *
 * Sets <*read_failed> to true if the read() system call didn't return any data
 * the first time it was called.
 *
 * Returns true if it finished reading; false otherwise.
 */
bool handle_reading_generic(int fd, quikstate *udfstate,  
        const int num_to_read, bool *read_error) 
{
    Assert(num_to_read < QUIK_BUFLEN);

    /* Fill up as much of the buffer as we can. */
    fill_buffer(fd, udfstate, read_error);

    /* Do we have enough data? */
    if (udfstate->bytes_in_buf >= num_to_read) {

        /* If we wrapped around, copy the wraparound bytes past the end of the
         * buffer. */
        int num_after_read_ix = QUIK_BUFLEN - udfstate->read_ix;
        if (num_to_read > num_after_read_ix) {
            memcpy(udfstate->overflow_buf, 
                    udfstate->circ_buf, 
                    num_to_read - num_after_read_ix);
        }

        return true;
    } else {
        return false;
    }
}

/* Reads as much data as possible into the buffer. */
void fill_buffer(int fd, quikstate *udfstate, bool *read_error) {
    int     num_to_read = -1;
    int     read_offset = -1;
    int     numread = -1;
    int     try_num = -1;
    bool    first_half = false;
                /* e.g. before the read ptr. */

    for (try_num = 1; try_num <= 3; try_num++) {

        if (udfstate->bytes_in_buf == QUIK_BUFLEN) {
            /* Buffer is full. */
            return;
        }

        /* Which half of the buffer are we reading into? */
        first_half = (udfstate->bytes_in_buf 
                        >= (QUIK_BUFLEN - udfstate->read_ix));
            /* Note the >= here. */

        if (first_half) {
            num_to_read = QUIK_BUFLEN - udfstate->bytes_in_buf;
            read_offset = udfstate->bytes_in_buf - 
                            (QUIK_BUFLEN - udfstate->read_ix);
        } else {
            num_to_read = QUIK_BUFLEN - udfstate->bytes_in_buf 
                            - udfstate->read_ix;
            read_offset = udfstate->read_ix + udfstate->bytes_in_buf;
        }

        numread = read(fd, udfstate->circ_buf + read_offset, num_to_read);

        if (numread <= 0) {
            if (try_num == 1) {
                /* Read failure. */
                *read_error = true;
            }

            /* Read as much as we can. */
            return;
        }

        /* Update our write pointer. */
        udfstate->bytes_in_buf += numread;
    }
}

void *get_read_ptr(quikstate *udfstate) {
    /* Note that this pointer may be inside udfstate->overflow_buf. */
    return (udfstate->circ_buf + udfstate->read_ix);
}


/* "Consume" a number of bytes in the buffer by moving the read pointer
 * forward. */
void consume_buffer(quikstate *udfstate, int num_to_munch) {
    udfstate->read_ix += num_to_munch;
    
    /* Wrap around. */
    udfstate->read_ix %= QUIK_BUFLEN;

    udfstate->bytes_in_buf -= num_to_munch;
}


/*
 * Other internal functions.
 */

/* Convert a struct timeval into a string Datum.  Based on code in 
 * timeofday() in nabstime.c. */
Datum tv2text(struct timeval *tp) {
    char		templ[100];
	char		buf[100];
    text	   *result;
	int			len;


    strftime(templ, sizeof(templ), "%a %b %d %H:%M:%S.%%06d %Y %Z",
			 localtime((time_t *) &(tp->tv_sec)));
    /*elog(LOG, "tv2text: usec is %d", tp->tv_usec);*/
    /*elog(LOG, "tv2text: templ is '%s'", templ);*/
	snprintf(buf, sizeof(buf), templ, tp->tv_usec);

    /*elog(LOG, "tv2text: buf is '%s'", buf);*/

    /* Add a trailing NULL for c-compatiblity */
	len = VARHDRSZ + strlen(buf);
	result = (text *) palloc(len + 1);
	VARATT_SIZEP(result) = len;
	memcpy(VARDATA(result), buf, strlen(buf) + 1);
	PG_RETURN_TEXT_P(result);
}


