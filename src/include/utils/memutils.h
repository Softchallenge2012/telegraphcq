/*-------------------------------------------------------------------------
 *
 * memutils.h
 *	  This file contains declarations for memory allocation utility
 *	  functions.  These are functions that are not quite widely used
 *	  enough to justify going in utils/palloc.h, but are still part
 *	  of the API of the memory management subsystem.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: memutils.h,v 1.12 2003/04/23 01:35:11 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef MEMUTILS_H
#define MEMUTILS_H

#include "nodes/memnodes.h"
#include <mm.h>



/*
 * MaxAllocSize
 *		Quasi-arbitrary limit on size of allocations.
 *
 * Note:
 *		There is no guarantee that allocations smaller than MaxAllocSize
 *		will succeed.  Allocation requests larger than MaxAllocSize will
 *		be summarily denied.
 *
 * XXX This is deliberately chosen to correspond to the limiting size
 * of varlena objects under TOAST.	See VARATT_MASK_SIZE in postgres.h.
 */
#define MaxAllocSize	((Size) 0x3fffffff)		/* 1 gigabyte - 1 */

#define AllocSizeIsValid(size)	(0 < (size) && (size) <= MaxAllocSize)

/*
 * All chunks allocated by any memory context manager are required to be
 * preceded by a StandardChunkHeader at a spacing of STANDARDCHUNKHEADERSIZE.
 * A currently-allocated chunk must contain a backpointer to its owning
 * context as well as the allocated size of the chunk.	The backpointer is
 * used by pfree() and repalloc() to find the context to call.	The allocated
 * size is not absolutely essential, but it's expected to be needed by any
 * reasonable implementation.
 */
typedef struct StandardChunkHeader
{
	MemoryContext context;		/* owning context */
	Size		size;			/* size of data space allocated in chunk */
#ifdef MEMORY_CONTEXT_CHECKING
	/* when debugging memory usage, also store actual requested size */
	Size		requested_size;
#endif
} StandardChunkHeader;

#define STANDARDCHUNKHEADERSIZE  MAXALIGN(sizeof(StandardChunkHeader))


/*
 * Standard top-level memory contexts.
 *
 * Only TopMemoryContext and ErrorContext are initialized by
 * MemoryContextInit() itself.
 *
 *	tcqoc
 *	Note that neither TopMemoryContext or TopSharedMemoryContext
 *	can be deleted using MemoryContextDelete or SHMContextDelete
 *	because their underlying nodes are allocated via malloc or
 *	shared memory malloc, not palloc in the appropriate context.
 */


extern DLLIMPORT MemoryContext TopMemoryContext;
extern DLLIMPORT MemoryContext ErrorContext;
extern DLLIMPORT MemoryContext PostmasterContext;
extern DLLIMPORT MemoryContext CacheMemoryContext;
extern DLLIMPORT MemoryContext QueryContext;
extern DLLIMPORT MemoryContext TopTransactionContext;
extern DLLIMPORT MemoryContext TransactionCommandContext;

extern DLLIMPORT MemoryContext TopSharedMemoryContext;
extern DLLIMPORT MemoryContext shmQueryContext;


/*
 * Memory-context-type-independent functions in mcxt.c
 */
extern void MemoryContextInit(void);
extern void MemoryContextReset(MemoryContext context);
extern void MemoryContextDelete(MemoryContext context);
extern void MemoryContextResetChildren(MemoryContext context);
extern void MemoryContextDeleteChildren(MemoryContext context);
extern void MemoryContextResetAndDeleteChildren(MemoryContext context);
extern Size GetMemoryChunkSpace(void *pointer);
extern void MemoryContextStats(MemoryContext context);

#ifdef MEMORY_CONTEXT_CHECKING
extern void MemoryContextCheck(MemoryContext context);
#endif
extern bool MemoryContextContains(MemoryContext context, void *pointer);

/*
 * This routine handles the context-type-independent part of memory
 * context creation.  It's intended to be called from context-type-
 * specific creation routines, and noplace else.
 */
extern MemoryContext MemoryContextCreate(NodeTag tag, Size size,
					MemoryContextMethods *methods,
					MemoryContext parent,
					const char *name);


/*
 * Memory-context-type-specific functions
 */

/* aset.c */
extern MemoryContext AllocSetContextCreate(MemoryContext parent,
					  const char *name,
					  Size minContextSize,
					  Size initBlockSize,
					  Size maxBlockSize);

/* shmset.c */
extern MemoryContext SHMAllocSetContextCreate(MemoryContext parent,
						 const char *name,
						 Size minContextSize,
						 Size initBlockSize,
						 Size maxBlockSize,
						 MM * cxt);

/*
 * Recommended default alloc parameters, suitable for "ordinary" contexts
 * that might hold quite a lot of data.
 */
#define ALLOCSET_DEFAULT_MINSIZE   (8 * 1024)
#define ALLOCSET_DEFAULT_INITSIZE  (8 * 1024)
#define ALLOCSET_DEFAULT_MAXSIZE   (8 * 1024 * 1024)
#define SHM_DEFAULT_SEGMENT  mm_query_segment


/* shared memory context management functions */


extern MM  *mm_query_segment;




extern void SHMContextShutdown(void);
extern void SHMContextInit(void);
extern void SHMContextReset(MemoryContext context);
extern void SHMContextDelete(MemoryContext context);
extern void SHMContextResetChildren(MemoryContext context);
extern void SHMContextDeleteChildren(MemoryContext context);
extern void SHMContextResetAndDeleteChildren(MemoryContext context);
extern void SHMContextStats(MemoryContext context);
extern void SHMContextCheck(MemoryContext context);
extern bool SHMContextContains(MemoryContext context, void *pointer);
extern MemoryContext SHMContextCreate(NodeTag tag, Size size,
				 MemoryContextMethods *methods,
				 MemoryContext parent,
				 const char *name,
				 MM * shmcxt);



struct SHMcxtstate
{
	int			refcount;
};


MM		   *SHMFindMMContext(MemoryContext ac);

#define SHMPointerIsValid(pointer) ((void*)(pointer) != NULL)

#endif   /* MEMUTILS_H */
