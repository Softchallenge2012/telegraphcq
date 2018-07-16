/*
 * rmgr.c
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * Resource managers definition
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/backend/access/transam/rmgr.c,v 1.5 2004/03/24 08:10:50 chungwu Exp $
 */
#include "postgres.h"

#include "access/clog.h"
#include "access/gist.h"
#include "access/hash.h"
#include "access/heapam.h"
#include "access/nbtree.h"
#include "access/rtree.h"
#include "access/streamheapam.h"
#include "access/xact.h"
#include "access/xlog.h"
#include "storage/smgr.h"
#include "commands/sequence.h"


RmgrData	RmgrTable[] = {
	{"XLOG", xlog_redo, xlog_undo, xlog_desc},
	{"Transaction", xact_redo, xact_undo, xact_desc},
	{"Storage", smgr_redo, smgr_undo, smgr_desc},
	{"CLOG", clog_redo, clog_undo, clog_desc},
	{"Reserved 4", NULL, NULL, NULL},
	{"Reserved 5", NULL, NULL, NULL},
	{"Reserved 6", NULL, NULL, NULL},
	{"Reserved 7", NULL, NULL, NULL},
	{"Reserved 8", NULL, NULL, NULL},
	{"Reserved 9", NULL, NULL, NULL},
	{"Heap", heap_redo, heap_undo, heap_desc},
	{"Btree", btree_redo, btree_undo, btree_desc},
	{"Hash", hash_redo, hash_undo, hash_desc},
	{"Rtree", rtree_redo, rtree_undo, rtree_desc},
	{"Gist", gist_redo, gist_undo, gist_desc},
	{"Sequence", seq_redo, seq_undo, seq_desc},
	{"StreamHeap", streamheap_redo, streamheap_undo, streamheap_desc}
};
