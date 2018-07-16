/*----------------------------------------------------------------------
 *
 * nodeFEventAgg.h
 *
 * The event aggregation operator - uses the Fjord-based ProcessNode 
 * interface. 
 *
 *
 * $Id: nodeFEventAgg.h,v 1.2 2005/06/13 02:59:28 phred Exp $
 *
 *----------------------------------------------------------------------
 */

#ifndef NODEFEVENTAGG_H
#define NODEFEVENTAGG_H


extern bool ProcessInitFEventAgg(FEventAgg * node, EState * es, Plan * parent);
extern bool ProcessEndFEventAgg(FEventAgg * node, Plan * parent);
extern void ProcessNextFEventAgg(FEventAgg * node, Plan * parent);
extern bool ProcessRemoveQueryFEventAgg(FEventAgg * node, int queryid, Plan * parent);
extern void ProcessPrintStatsFEventAgg(FEventAgg * node, Plan * parent);

typedef struct {

  Event *event;
  List *matched; /* A list of events this event has already seen 
		  * and was matched up with.
		  * To avoid firing the same complex event multiple times
		  * */

  bool requestedTimeout; /* Already requested a timeout for me? */

  bool marked; /* A  utility attribute for marking this event entry as "seen"  */

} EventEntry;


#endif   /* NODEFEVENTAGG_H */
