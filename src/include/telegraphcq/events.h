/*------------------------------------------------------------------------
 * events.h
 *
 * Definitions and macros for event processing
 * 
 * @Author = Shariq
 *
 * $Id: events.h,v 1.2 2005/06/13 02:59:29 phred Exp $
 *------------------------------------------------------------------------
*/

#include "executor/intermediateTuple.h"

#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  EVENT_SIMPLE=0,
  EVENT_ANYONE,
  EVENT_BOTH,
  EVENT_FOLLOWS,
  EVENT_NOTFOLLOWS,
  EVENT_PRECEDES,
  EVENT_NOTPRECEDES
} EventOperator;



typedef IntermediateHeapTupleData Event;


typedef struct {
  
  Node *node; /* Whom to service? */
  
  Timestamp ts; /* At what time? */
  
} TimeoutRequest;

#endif /* EVENTS_H */
