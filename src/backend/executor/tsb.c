/*-------------------------------------------------------------------------
 *
 * tsb.c
 *
 *    Naive implementation of the transient skew buffer. Similar to
 *    the Juggle operator, but it is main-memory, fixed size, and
 *    supports a couple more interfaces.
 *	  
 * Copyright (c) 2003, Regents of the University of California
 *
 *
 * DESCRIPTION
 *    You know, the usual.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/tsb.c,v 1.18 2004/01/19 21:37:51 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include "executor/tuptable.h"
#include "executor/tsb.h"
#include "miscadmin.h"
#include "rqueue/rqdest.h"
#include "access/heapam.h"

#define TupIsNull(slot) \
	((slot) == NULL || (slot)->val == NULL)

/* Note the prev and next will be un-initialized.*/
static TSB_Entry *
alloc_entry(TSB *tsb)
{
	TSB_Entry *rentry;

	/* Pull from the tail */
	if (tsb->f_tail == NULL)
		return NULL;

	rentry = tsb->f_tail;

	/* Last entry */
	if (tsb->f_head == tsb->f_tail)
	{
		tsb->f_head = tsb->f_tail = NULL;
	}
	else
	{
		tsb->f_tail = tsb->f_tail->next;
	}

	tsb->size++;

	/* Initialize values to invalid data */
	rentry->seqnum = -1; /* Invalid entry */
	rentry->dmask  = -1; /* Invalid entry */
	rentry->next   = NULL;
	rentry->prev   = NULL;

	return rentry;
}

static TSB_Entry *
alloc_entry_w_last(TSB *tsb, int part_no)
{
	TSB_Entry *rentry;

	rentry = alloc_entry(tsb);

	if (rentry) {
		return rentry;
	}

	rentry = tsb->last_ents[part_no];

	if (rentry) 
	{
		rentry->seqnum = -1;
		rentry->dmask  = -1;
		rentry->next   = NULL;
		rentry->prev   = NULL;
			
		tsb->last_ents[part_no] = NULL;
	}

	return rentry;
}

static void
free_entry(TSB *tsb, TSB_Entry *entry)
{
	entry->next = tsb->f_tail;
	tsb->f_tail = entry;

	if (tsb->f_head == NULL)
		tsb->f_head = tsb->f_tail;

	tsb->size--;

	return;
}

static void
free_entry_w_last(TSB *tsb, TSB_Entry *entry, int part_no)
{
	if (tsb->last_ents[part_no])
	{
		free_entry(tsb, entry);
		return;
	}

	tsb->last_ents[part_no] = entry;

	return;
}

void
return_last_ent_TSB(TSB *tsb, int part_no)
{
	free_entry(tsb, tsb->last_ents[part_no]);

	tsb->last_ents[part_no] = NULL;
}

/* Remove the entry from the linked list */
static void
del_entry(TSB *tsb, int part_no, TSB_Entry *tentry) 
{

	if ((tentry->rtup.type == RESULT_HEAPTUPLE) &&
		tentry->rtup.data.tuple)
	{
		/* XXX I don't check for NULL so I can explicitly find bugs */

		if (tsb->shouldFree) 
		{
			heap_freetuple(tentry->rtup.data.tuple);
		}

		tentry->rtup.data.tuple = NULL;
	}

	/* Adust the pointers so tentry isn't in the linked list. */
	if (tentry->prev)
	{
		tentry->prev->next =
		tentry->next;
	}

	if (tentry->next)
	{
		tentry->next->prev =
		tentry->prev;
	}

/* #define DEBUG_DEL */
	
#ifdef DEBUG_DEL
	elog(DEBUG1, "D-seqno: %d", tentry->seqnum);
#endif

/* 	free_entry(tsb, tentry); */
	free_entry_w_last(tsb, tentry, part_no);
	
	tsb->counts[part_no]--;

/* 	Assert(tsb->counts[part_no] >= 0); */
	
	if (tsb->counts[part_no] < 0)
	{
		elog(DEBUG1, "Whoa, tsb counts should never be negative %d",
			 tsb->counts[part_no]);
		sleep(10000);
		elog(ERROR, "Waited for a while, exiting.");
	}
}

void
init_TSB(TSB *tsb, int nparts, int max) 
{
	int i;
	TSB_Entry *tentry, *lentry;

	tsb->max_size = max + nparts; /* Need buffer ones for the tail */
	tsb->num_parts = nparts;
	tsb->size = 0;

	tsb->table     = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);

	tsb->prod_ack  = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);
	tsb->prim_ack  = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);
	tsb->sec_ack   = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);

	tsb->p_head    = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);
	tsb->s_head    = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);

	/* Allocate one for just the last, so we can make progress. */
	tsb->last_ents = (TSB_Entry **) palloc(sizeof(TSB_Entry *) * nparts);
	lentry = (TSB_Entry *) palloc(sizeof(TSB_Entry) * nparts);
	
		
	tsb->counts = (int *) palloc(sizeof(int) * nparts);


	tsb->f_head = (TSB_Entry *) palloc(sizeof(TSB_Entry) * (tsb->max_size));
	tsb->f_tail = &(tsb->f_head[(tsb->max_size) - 1]); /* Last one */

	/* Link up the free list */
	tsb->f_head->next = NULL;

	tsb->shouldFree = TRUE; /* By default, we free on entry delete */

	for (i = 1; i < (tsb->max_size); i++)
	{
		tentry = &(tsb->f_head[i]);
		tentry->next = &(tsb->f_head[i-1]);
	}

	for (i = 0; i < nparts; i++)
	{
		tentry = alloc_entry(tsb);
		(tsb->table)[i] = tentry;
		(tsb->p_head)[i] = tentry;
		(tsb->s_head)[i] = tentry;
		(tsb->prod_ack)[i] = tentry;
		(tsb->prim_ack)[i] = tentry;
		(tsb->sec_ack)[i]  = tentry;
		(tsb->last_ents)[i] = &(lentry[i]);
		tsb->counts[i] = 0;
	}
}

/* static TSB_Entry *foo = NULL; */
/* static TSB_Entry *bar = NULL; */

bool
insert_marker_end_TSB(TSB *tsb, result *r, int part_no)
{
	TSB_Entry *tentry, *pentry;

	tentry = alloc_entry(tsb);
		
	if (tentry == NULL)
		return false; /* Can't add the entry, probably full. */

	pentry = tsb->table[part_no];
	
	pentry->rtup = *r;

	pentry->seqnum = PUNCTUATION;
	pentry->dmask  = TSB_PRODUCED;    /* This guy created it */
	pentry->next   = tentry;
	tentry->prev   = pentry;

	tsb->table[part_no] = tentry;
	tsb->counts[part_no]++;

	return true;
}

bool
append_result_TSB(TSB *tsb, result *r, int seq_no, int mask, int part_no)
{
	TSB_Entry *tentry, *pentry;

/* 	tentry = alloc_entry(tsb); */

	tentry = alloc_entry_w_last(tsb, part_no);

	if (tentry == NULL)
	{
		return FALSE;
	}

	pentry = tsb->table[part_no];

	pentry->rtup = *r;
	pentry->seqnum = seq_no;
	pentry->dmask = mask;
	pentry->next = tentry;
	tentry->prev = pentry;

	tsb->table[part_no] = tentry;
	tsb->counts[part_no]++;

	return TRUE;
}

bool
insert_marker_start_TSB(TSB *tsb, result *r, int part_no, int mask) {

	TSB_Entry *tentry, *b_entry, *a_entry;

	tentry = alloc_entry(tsb);

	if (tentry == NULL)
		return false;

	tentry->seqnum = PUNCTUATION;
	tentry->dmask = TSB_PRODUCED | TSB_PRIMARY | TSB_SECONDARY;

	if (mask & TSB_PRIMARY)
	{
		/* Before entry */
		b_entry = tsb->p_head[part_no]->prev;
		/* After entry */
		a_entry = tsb->p_head[part_no];

		/* The ack pointer needs to be bumped back. */
		if (tsb->prim_ack[part_no] == 
			tsb->p_head[part_no])
		{
			tsb->prim_ack[part_no] = tentry;
		}

		tsb->p_head[part_no] = tentry;
		
		tsb->counts[part_no]++;
	}

	if (mask & TSB_SECONDARY)
	{
		/* Before entry */
		b_entry = tsb->s_head[part_no]->prev;
		/* After entry */
		a_entry = tsb->s_head[part_no];

		/* The ack pointer needs to be bumped back */
		if (tsb->sec_ack[part_no] == 
			tsb->s_head[part_no])
		{
			tsb->sec_ack[part_no] = tentry;
		}

		tsb->s_head[part_no] = tentry;
		tsb->counts[part_no]++;

	}

	/* Set pointers for before entry */
	if (b_entry)
	{
		b_entry->next = tentry;
	}
	tentry->prev = b_entry;

	/* Set pointers for after entry */
	if (a_entry) /* XXX Shouldn't ever be false */
	{ 
		a_entry->prev = tentry;
	}
	tentry->next = a_entry;

	tentry->dmask &= ~mask; /* mask needs to be set before deletion */
	tentry->rtup = *r;

	return true;
}

/* Insert the data for the partition number, 
   fmask says wether you can free or not */
bool
put_ack_TSB(TSB *tsb, TupleTableSlot *slot,
			int seqno, int part_no, int mask,
			int fmask)
{
	TSB_Entry *tentry, *pentry;
	TSB_Entry **cursor;
	bool found = false;

/* 	if ((seqno == 129) && (mask == TSB_SECONDARY) && (telegraph_nodeid == 0)) { */

/* 		elog(DEBUG1, "Got seqno 139"); */
/* 		sleep (10000); */

/* 	} */

	/* Select the appropriate cursor */
	switch(mask)
	{
		case (TSB_PRODUCED):
			cursor = &(tsb->prod_ack[part_no]);
			break;

		case (TSB_PRIMARY):
			cursor = &(tsb->prim_ack[part_no]);
			break;

		case (TSB_SECONDARY):
			cursor = &(tsb->sec_ack[part_no]);
			break;
		
		default: /* We don't support any other type of mask */
			return false;
	}

	/* Advance the ack pointer or cursor until there is nothing left. */
/* 	while (((*cursor)->dmask > 0) && */
	while (((*cursor)->dmask >= 0) &&
		   ((*cursor)->seqnum <= seqno))
	{
		if ((*cursor)->seqnum == seqno)
		{
			found = true;				
			/* Replace the current tuple with the one passed */
			if (!TupIsNull(slot))
			{
/* 				(*cursor)->htup = slot->val; */
				(*cursor)->rtup.type = RESULT_HEAPTUPLE;
				(*cursor)->rtup.data.tuple = slot->val;
				slot->val = NULL;
				slot->ttc_shouldFree = false;
			}
		}

		(*cursor)->dmask |= mask;

		if (!found)
		{
			elog(DEBUG1, "Skipping and marking seqno: %d, curr seqno: %d",
				 (*cursor)->seqnum, seqno);
			elog(DEBUG1, "Part_no: %d, Prim: %d", part_no, mask);

			if (seqno != PUNCTUATION) 
			{
				elog(DEBUG1, "Sleeping ... so you can debug.");
				sleep(10000); /* Should not be happening. */
			}
		}

		tentry = *cursor;

/* 		if (seqno == 139) { */
			
/* 			if (tentry->htup) { */
/* 				elog(DEBUG1, "139 HTUP LEN: %d", tentry->htup->t_len); */
/* 			} else { */
/* 				elog(DEBUG1, "139 HTUP LEN: %d", 0); */
/* 			} */
/* 		} */
				
		(*cursor) = (*cursor)->next;

		/* XXX This old one is wrong, don't worry about bits not turned on. */
/* 		if (tentry->dmask == fmask) */
		if ((tentry->dmask & fmask) == fmask)
		{
/* 			if (tsb->p_head[part_no] == tentry) */
/* 				tsb->p_head[part_no] = tsb->p_head[part_no]->next; */
/* 			if (tsb->s_head[part_no] == tentry) */
/* 				tsb->s_head[part_no] = tsb->s_head[part_no]->next; */
/* 			if (tentry->htup) { */
/* 				pfree(tentry->htup); */
/* 				tentry->htup = NULL; */
/* 			} */
			del_entry(tsb, part_no, tentry);
		}
	}

	/* We need to add the entry, if we did not find it. */
	if (((*cursor)->dmask < 0) && (!found))
	{
/* 		tentry = alloc_entry(tsb); */
		tentry = alloc_entry_w_last(tsb, part_no);

		if (tentry == NULL)
			return false; /* Can't add the entry, probably full. */

		pentry = tsb->table[part_no];

		pentry->rtup.type  = RESULT_HEAPTUPLE;

		if (slot)
		{
/* 			pentry->htup = slot->val; */
			pentry->rtup.data.tuple = slot->val;
			slot->val = NULL;             /* Funny way of clearing it  */
			slot->ttc_shouldFree = false; /* Don't free, we own it now */
		}
		else
		{
/* 			pentry->htup = NULL; */
			pentry->rtup.data.tuple = NULL;
		}

/* 		if (seqno == 139) { */
/* 			foo = pentry; */
/* 		} */

/* 		if (seqno == 129) { */
/* 			bar = pentry; */
/* 		} */


/* 		Assert(!BufferIsValid(slot->ttc_buffer)); /\* Make sure its not in the  */
/* 													 buffer pool *\/ */

		pentry->seqnum = seqno;
		pentry->dmask  = mask;    /* This guy created it */
		pentry->next   = tentry;
		tentry->prev   = pentry;

		tsb->table[part_no] = tentry;
		tsb->counts[part_no]++;

		(*cursor) = (*cursor)->next;

	}
	else
	{
		if (!found)
		{
			elog(DEBUG1, "Found something valid, but larger than the seqno");
			elog(DEBUG1, "DMASK: %d SEQNUM: %d ACK_SEQ: %d PRIM: %d", 
				 (*cursor)->dmask, (*cursor)->seqnum, seqno, mask);
			elog(DEBUG1, "Sleeping ... so you can debug.");
			sleep(10000);
		}
	}

/* 	if (foo) { */
/* 		elog(DEBUG1, "139 htup Addr: %x", foo->htup); */
/* 		if (foo->htup) { */
/* 			elog(DEBUG1, "BLAH SPEW 139 HLEN: %d", foo->htup->t_len); */
/* 			if (foo->htup->t_len != 44) { */
/* 				sleep(1000); */
/* 			} */
/* 		} else { */
/* 			elog(DEBUG1, "BLAH SPEW 129 HLEN: %d", 0); */
/* 		} */
/* 	} */

/* 	if (bar) { */
/* 		elog(DEBUG1, "129 htup Addr: %x", bar->htup); */
/* 		if (bar->htup) { */
/* 			elog(DEBUG1, "BLAH SPEW 129 HLEN: %d", bar->htup->t_len); */
/* /\* 			if (foo->htup->t_len != 44) { *\/ */
/* /\* 				sleep(1000); *\/ */
/* /\* 			} *\/ */
/* 		} else { */
/* 			elog(DEBUG1, "BLAH SPEW 129 HLEN: %d", 0); */
/* 		} */
/* 	} */

/* 	if (foo && bar) { */

/* 		elog(DEBUG1," Foohtup %x Barhtup BBBBBBBB %x ",  */
/* 			 foo->htup, bar->htup); */
/* 	} */
	return true;
}

/* Get the data for the partition number, assume slot is cleared. */
void
get_head_TSB(TSB *tsb, TupleTableSlot *slot, int part_no, int mask)
{
	slot->ttc_shouldFree = false;

	switch(mask)
	{
		case (TSB_PRIMARY):
			if (!TSB_PRIM_PART_EMPTY(tsb, part_no))
/* 			slot->val = tsb->p_head[part_no]->htup; */
				slot->val = tsb->p_head[part_no]->rtup.data.tuple;
			return;

		case (TSB_SECONDARY):
			if (!TSB_SEC_PART_EMPTY(tsb, part_no))
/* 			slot->val = tsb->s_head[part_no]->htup; */
				slot->val = tsb->s_head[part_no]->rtup.data.tuple;
			return;

	}
}

/* XXX Warning, caller must make sure not to free the tuple! */
void
get_head_TSB_r(TSB *tsb, result *r, int part_no, int mask)
{
	switch(mask)
	{
		case (TSB_PRIMARY):
			if (!TSB_PRIM_PART_EMPTY(tsb, part_no))
/* 			slot->val = tsb->p_head[part_no]->htup; */
				*r = tsb->p_head[part_no]->rtup;
			return;

		case (TSB_SECONDARY):
			if (!TSB_SEC_PART_EMPTY(tsb, part_no))
/* 			slot->val = tsb->s_head[part_no]->htup; */
				*r = tsb->s_head[part_no]->rtup;
			return;
	}
}

/* Returns the first unacknowledged sequence number */
int 
get_unack_seqno_TSB(TSB *tsb, int part_no, int mask)
{
	switch(mask)
	{
		case (TSB_PRIMARY):
			if (!(tsb->prim_ack[part_no]->dmask < 0))
				return tsb->prim_ack[part_no]->seqnum;
			break;

		case (TSB_SECONDARY):
			if (!(tsb->sec_ack[part_no]->dmask < 0))
				return tsb->sec_ack[part_no]->seqnum;
			break;
		
		case (TSB_PRODUCED):
			if (!(tsb->prod_ack[part_no]->dmask < 0))
				return tsb->prod_ack[part_no]->seqnum;
			break;
	}
	return -1;
}

void
advance_head_TSB(TSB *tsb, int part_no, int mask)
{
	switch(mask)
	{
		case(TSB_PRIMARY):
			if (!TSB_PRIM_PART_EMPTY(tsb, part_no))
				tsb->p_head[part_no] = tsb->p_head[part_no]->next;

#ifdef DEBUG_HEAD
			elog(DEBUG1, "P-head: %d pt: %d", tsb->p_head[part_no]->prev->seqnum, part_no);
#endif
			return;

		case(TSB_SECONDARY):
			if (!TSB_SEC_PART_EMPTY(tsb, part_no))
				tsb->s_head[part_no] = tsb->s_head[part_no]->next;

#ifdef DEBUG_HEAD
			elog(DEBUG1, "S-head: %d pt: %d", tsb->s_head[part_no]->prev->seqnum, part_no);
#endif
			return;
	}
}

/* This is used for markers that do not get acknowledged */

/* Sets the head to where the ACK is */
void
reset_head_TSB(TSB *tsb, int part_no, int mask)
{
	switch(mask)
	{
		case(TSB_PRIMARY):
			tsb->p_head[part_no] = tsb->prim_ack[part_no];
			return;

		case(TSB_SECONDARY):
			tsb->s_head[part_no] = tsb->sec_ack[part_no];
			return;
	}
}

void
reset_ack_TSB(TSB *tsb, int part_no, int mask) {

	switch(mask)
	{
		case(TSB_PRIMARY):
			tsb->prim_ack[part_no] = tsb->sec_ack[part_no];
			return;

		case(TSB_SECONDARY):
			tsb->sec_ack[part_no] = tsb->prim_ack[part_no];
			return;
	}
}


/* Resets both the ack and head pointer for the client specified by mask. */
void
reset_cursors_TSB(TSB *tsb, int part_no, int mask)
{
	TSB_Entry *entry;

	reset_ack_TSB(tsb, part_no, mask);

	switch (mask)
	{
		case (TSB_PRIMARY):
			entry = tsb->prim_ack[part_no];
			break;
		case (TSB_SECONDARY):
			entry = tsb->sec_ack[part_no];
			break;
	}

	/*
	 * Go down the list and unmark for the mask
	 */
	while (entry->dmask != -1)
	{
		entry->dmask &= ~mask;
		entry = entry->next;
	}
	
	reset_head_TSB(tsb, part_no, mask);
}

/* Invariant that all routines must maintain, 
   head >= ack in the list. */

/* Acks all tuples in the TSB for partno */
void
ack_all_TSB(TSB *tsb, int partno, int mask, int del_mask)
{
	int seqno;
	
	seqno = get_unack_seqno_TSB(tsb, partno, mask);
	
	while (seqno != -1)
	{
		put_ack_TSB(tsb, NULL, seqno, partno, mask,
					del_mask);
		seqno = get_unack_seqno_TSB(tsb, partno, mask);
	}
	/* Cannot get ahead of the head. */
	reset_head_TSB(tsb, partno, mask);
}

/* Acks all tuples in the TSB for partno, until a punctuation is
   found that has not been sent, if any. */

void
ack_to_marker_TSB(TSB *tsb, int partno, int mask, int del_mask)
{
	int seqno;
	TSB_Entry **p_ack, **p_head;
	bool cursors_crossed = false;

	/* Setup the cursors properly */
	switch (mask)
	{
		case (TSB_PRIMARY):
			p_ack  = &(tsb->prim_ack[partno]);
			p_head = &(tsb->p_head[partno]);
			break;

		case (TSB_SECONDARY):
			p_ack  = &(tsb->sec_ack[partno]);
			p_head = &(tsb->s_head[partno]);
			break;
	}

	seqno = get_unack_seqno_TSB(tsb, partno, mask);
	cursors_crossed |= ((*p_ack) == (*p_head));

	while (seqno != -1)
	{
		if ((seqno == PUNCTUATION) && cursors_crossed)
		{
			/* Found a marker, and ack cursor met or passed the head,
			   so the marker is unsent. */
			break;
		}

#define DEBUG_ATM

#ifdef DEBUG_ATM
		elog(DEBUG1, "ATM-ack: Prim: %d Seq: %d", mask, seqno);
#endif

		put_ack_TSB(tsb, NULL, seqno, partno, mask, 
					del_mask);
		seqno = get_unack_seqno_TSB(tsb, partno, mask);
		cursors_crossed |= ((*p_ack) == (*p_head));
	}

	reset_head_TSB(tsb, partno, mask);

#ifdef DEBUG_ATM
	elog(DEBUG1, "p-ack: %d, s-ack %d, p-dmask %d, s-dmask %d", 
		 tsb->prim_ack[partno]->seqnum, 
		 tsb->sec_ack[partno]->seqnum,
		 tsb->prim_ack[partno]->dmask,
		 tsb->sec_ack[partno]->dmask);
	elog(DEBUG1, "p-head: %d, s-head %d, p-dmask %d, s-dmask %d",
		 tsb->p_head[partno]->seqnum, 
		 tsb->s_head[partno]->seqnum,
		 tsb->p_head[partno]->dmask, 
		 tsb->s_head[partno]->dmask);
#endif

}

void
dump_seq_TSB_part(TSB *tsb, int i)
{
	elog(DEBUG1, "----------------------------");
	elog(DEBUG1, "Partition: %d", i);
	elog(DEBUG1, "prod_ack: %d", tsb->prod_ack[i]->seqnum);
	elog(DEBUG1, "prim_ack: %d", tsb->prim_ack[i]->seqnum);
	elog(DEBUG1, "sec_ack: %d", tsb->sec_ack[i]->seqnum);
	elog(DEBUG1, "p_head: %d", tsb->p_head[i]->seqnum);
	elog(DEBUG1, "s_head: %d", tsb->s_head[i]->seqnum);
	elog(DEBUG1, "table: %d", tsb->table[i]->seqnum);
	elog(DEBUG1, "count: %d", tsb->counts[i]);
	elog(DEBUG1, "DMASK");
	elog(DEBUG1, "prod_ack: %d", tsb->prod_ack[i]->dmask);
	elog(DEBUG1, "prim_ack: %d", tsb->prim_ack[i]->dmask);
	elog(DEBUG1, "sec_ack: %d", tsb->sec_ack[i]->dmask);
	elog(DEBUG1, "p_head: %d", tsb->p_head[i]->dmask);
	elog(DEBUG1, "s_head: %d", tsb->s_head[i]->dmask);
	elog(DEBUG1, "table: %d", tsb->table[i]->dmask);
	elog(DEBUG1, "----------------------------");
}


void
dump_seq_TSB(TSB *tsb)
{
	int i;

	elog(DEBUG1, " --- TSB SIZE: %d ---- ", tsb->size);
	
	for (i = 0; i < tsb->num_parts; i++)
	{
		dump_seq_TSB_part(tsb, i);
	}
}

void
dump_chain(TSB_Entry *t)
{
	while (t)
	{
		elog(DEBUG1, "S: %d M: %d", t->seqnum, t->dmask);
		t = t->next;
	}
}

int
chain_len_prev(TSB_Entry *t)
{
	int i = 0;

	while (t && (t->dmask != -1))
	{
		t = t->prev;
		i++;
	}

	return i;
}

int
chain_len_next(TSB_Entry *t)
{
	int i = 0;

	while (t && (t->dmask != -1))
	{

#ifdef DEBUG_CURSORS_RESET
		elog(DEBUG1, "reset_cursors: t: %d",
			 t->seqnum);
#endif

		t = t->next;
		i++;
	}

	return i;
}

static void
swap_tsb_part(TSB *tsb, int a, int b)
{
	TSB_Entry *tmp_e;
	int tmp_c;

	/* Swap count */

	tmp_c = tsb->counts[a];
	tsb->counts[a] = tsb->counts[b];
	tsb->counts[b] = tmp_c;

	/* Swap pointers */
	tmp_e = tsb->table[a];
	tsb->table[a] = tsb->table[b];
	tsb->table[b] = tmp_e;

	tmp_e = tsb->prod_ack[a];
	tsb->prod_ack[a] = tsb->prod_ack[b];
	tsb->prod_ack[b] = tmp_e;

	tmp_e = tsb->prim_ack[a];
	tsb->prim_ack[a] = tsb->prim_ack[b];
	tsb->prim_ack[b] = tmp_e;

	tmp_e = tsb->sec_ack[a];
	tsb->sec_ack[a] = tsb->sec_ack[b];
	tsb->sec_ack[b] = tmp_e;

	tmp_e = tsb->p_head[a];
	tsb->p_head[a] = tsb->p_head[b];
	tsb->p_head[b] = tmp_e;

	tmp_e = tsb->s_head[a];
	tsb->s_head[a] = tsb->s_head[b];
	tsb->s_head[b] = tmp_e;

	tmp_e = tsb->last_ents[a];
	tsb->last_ents[a] = tsb->last_ents[b];
	tsb->last_ents[b] = tmp_e;
}

void
permute_tsb_parts(TSB *tsb, int *perm_map)
{
	int i;
	int curr_idx[tsb->num_parts];
	int tmp, dloc;

	for (i = 0; i < tsb->num_parts; i++)
	{
		curr_idx[i] = i;
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		dloc = perm_map[curr_idx[i]];
		
		while (i != dloc)
		{
			swap_tsb_part(tsb, i, dloc);
			
			/* Swap */
			tmp = curr_idx[i];
			curr_idx[i] = curr_idx[dloc];
			curr_idx[dloc] = tmp;

			dloc = perm_map[curr_idx[i]];
		}
	}
}
