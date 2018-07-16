/*-------------------------------------------------------------------------
 *
 * bindings.h
 *
 * this file defines the external interface for for functions dealing with
 * required bindings
 *
 * This is related to the new feature:
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: bindings.h,v 1.7 2004/07/30 20:54:37 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef BINDINGS_H
#define BINDINGS_H

#include "rqueue/controlq.h"

void		load_bindings_for_stream(Oid streamoid, StrmScan * strmscan);
void		freeBindinginfo(Bindinginfo * b);
IndexSteM  *strmscan_to_indexstem(StrmScan * scan);

bool		doneWithProbeStrmScan(StrmScan * strmscan, HeapTuple t);
Oid			getSourceOid(Plan *scan, Eddy * parent);
void		NotifyWCH(Oid streamoid, ControlCommand cmd);
void		load_eq_opers_for_bindinginfo(Bindinginfo * b);
Bindinginfo ** _copyBindingInfos(Bindinginfo **from, int numwrappers);
/* returns true if the probe tuple must be processed,
   false if it is pending.
*/
bool		beginProbe(Plan *p, HeapTuple t);

/* returns true if t is a doneWithProbe tuple.
   false otherwise.
*/
bool		doneWithProbe(Plan *p, HeapTuple t);
extern bool getBindingInformation(Eddy * e);
List *add_probe(HeapTuple probe, TupleDesc td,
		  List *pending_probes,
		  Bindinginfo ** binfo,
		  int numbindings,
		  ExprContext *econtext,
		  bool *alreadyPending);

List *remove_probe(HeapTuple finished, TupleDesc td,
			 List *pending_probes,
			 Bindinginfo ** binding_info,
			 int numbindings,
			 ExprContext *econtext);

Bindinginfo ** _copyBindingInfos(Bindinginfo **from, int numwrappers);
#endif   /* BINDINGS_H */
