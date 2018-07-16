/*-------------------------------------------------------------------------
 *
 * bit.h
 *	  Standard bit array definitions.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: bit.h,v 1.16 2005/06/13 02:59:29 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef BIT_H
#define BIT_H

#include <Judy.h>

typedef bits8 *BitArray;
typedef uint32 BitIndex;

#define BAMIN(n)   ((n)/BITS_PER_BYTE)
#define BAEXT(n)   (((n) % BITS_PER_BYTE) ? 1 : 0)
#define BASIZE(n)  (BAMIN(n) + BAEXT(n))


/*
 * BitArrayCreate
 * Creates a BitArray of given bit-size
 * 
 *  - TCQ SR
 */

extern BitArray BitArrayCreate(int size);

/*
 * BitArraySetBit
 *		Sets (to 1) the value of a bit in a bit array.
 */
extern void BitArraySetBit(BitArray bitArray, BitIndex bitIndex);

/*
 * BitArrayClearBit
 *		Clears (to 0) the value of a bit in a bit array.
 */
extern void BitArrayClearBit(BitArray bitArray, BitIndex bitIndex);

/*
 * BitArrayBitIsSet
 *		True iff the bit is set (1) in a bit array.
 */
extern bool BitArrayBitIsSet(BitArray bitArray, BitIndex bitIndex);

/*																	 //@BdeddySK
 * BitArraySetAll
 *		Sets (to 1) the value of all bits in the bit array.
 */
extern void BitArraySetAll(BitArray bitarray, BitIndex numElements);

/*
 * BitArrayClearAll
 *		Clears (to 0) the value of all bits in the bit array.
 */
extern void BitArrayClearAll(BitArray bitArray, BitIndex numElements);

extern void BitArrayRealClearAll(BitArray bitArray, BitIndex numElements);

/*
 * BitArrayIsAnySet
 *		True iff any bit in the bit array is set (1)
 */
extern bool BitArrayIsAnySet(BitArray bitArray, BitIndex numElements);

/*
 * BitArrayIsAnyClear
 *		True iff any bit in the bit array is cleared (0)
 */
extern bool BitArrayIsAnyCleared(BitArray bitArray, BitIndex numElements);

/*
 * BitArrayIsAnySet
 *		Returns the number of bits in the bit array that are set (1)
 */
extern BitIndex BitArrayNumSet(BitArray bitArray, BitIndex);	/* @EdeddySK */

/*tcqoc
 *
 * Bitwise and, or operators for variable length bit arrays.  Each of these
 * functions takes in a source, a destination, and a preallocated result bit
 * array.
 *
 * the equal function takes in two bit arrays and compares them for equality
 */
extern void BitArrayAnd(BitArray left, BitArray right, BitArray res, int num);
extern void BitArrayAndSelf(BitArray left, BitArray right, int num);
extern void BitArrayNot(BitArray to, BitArray from, int num);
extern void BitArrayNotSelf(BitArray left, int num);
extern void BitArrayOr(BitArray left, BitArray right, BitArray res, int num);
extern void BitArrayOrSelf(BitArray left, BitArray right, int num);
extern bool BitArrayEqual(BitArray src, BitArray dest, int num);
extern bool BitArrayContained(BitArray big, BitArray small, int num);
extern void BitArrayClone(BitArray tgt, BitArray src, int num); /* @djoinSK */

extern int	BitArrayFirstBit(BitArray ba, int num);

extern void BitArrayPrint(BitArray ba, int num);
extern void BitArrayPrintDebug(BitArray ba, int num);
extern void BitArrayPrintLev(BitArray ba, int num, int lev);

extern Word_t GetBitArrayAsWord_t(BitArray ba, BitIndex num);
extern Pvoid_t GetBitArrayAsJudy1(BitArray ba, BitIndex num);

#endif   /* BIT_H */
