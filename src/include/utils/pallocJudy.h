/*-------------------------------------------------------------------------
 *
 * pallocJudy.h
 *
 *    POSTGRES memory allocator that can be plugged into Judy
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 *
 * $Id: pallocJudy.h,v 1.2 2003/12/16 22:56:25 mashah Exp $
 * 
 *-------------------------------------------------------------------------
 */
#include <Judy.h>

#ifndef JUDYPALLOC_H
#define JUDYPALLOC_H

#define FOUR_ALIGN 4
#define EIGHT_ALIGN (2 * FOUR_ALIGN)

#define WAS_FOUR 0
#define WAS_EIGHT 1

#define IsFourAligned(Ptr)  (((Word_t)Ptr) % FOUR_ALIGN == 0)
#define IsEightAligned(Ptr)  (((Word_t)Ptr) % EIGHT_ALIGN == 0)

void* JudyPalloc(size_t size);
void JudyPFree(void * PWord);

#endif   /* JUDYPALLOC_H */
