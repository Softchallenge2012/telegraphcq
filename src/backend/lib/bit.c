/*-------------------------------------------------------------------------
 *
 * bit.c
 *	  Standard bit array code.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/lib/bit.c,v 1.18 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "utils/bit.h"
#include "lib/stringinfo.h"
#include "utils/guc.h"


/*
 * BitArrayCreate()
 *
 * Create a bit array of given bit-size
 *
 * - TCQ SR
 */

BitArray
BitArrayCreate(int size)
{

  BitArray result = (BitArray) palloc(sizeof(bits8) * BASIZE(size));

  return result;
}


/*
 * BitArrayAnd
 *
 * This function performs a bitwise AND of the bitarrays "left" and "right" and
 * stores the result in "res".
 *
 *					 res = left & right
 *
 * left  (IN)	Left BitArray with num elements
 * right (IN)	Right BitArray with num elements
 * res	 (OUT)	Target BitArray with num elements
 * num	 (IN)	Number of elements in each bit array
 *
 */
void
BitArrayAnd(BitArray left, BitArray right, BitArray res, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
		res[idx] = left[idx] & right[idx];
}

/*
 * BitArrayAndSelf
 *
 * This function performs a bitwise AND of the bitarrays "left" and "right" and
 * stores the result in "left".
 *
 *					 left = left & right
 *
 * left  (IN/OUT)	BitArray with num elements
 * right (IN)		BitArray with num elements
 * num	 (IN)		Number of elements in each bit array
 *
 */
void
BitArrayAndSelf(BitArray left, BitArray right, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
		left[idx] = left[idx] & right[idx];
}

/*
 * BitArrayOr
 *
 * This function performs a bitwise OR of the bitarrays "left" or "right" or
 * stores the result in "res".
 *
 *					 res = left | right
 *
 * left  (IN)	Left BitArray with num elements
 * right (IN)	Right BitArray with num elements
 * res	 (OUT)	Target BitArray with num elements
 * num	 (IN)	Number of elements in each bit array
 *
 */
void
BitArrayOr(BitArray left, BitArray right, BitArray res, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
		res[idx] = left[idx] | right[idx];
}

/*
 * BitArrayOrSelf
 *
 * This function performs a bitwise OR of the bitarrays "left" or "right" or
 * stores the result in "left".
 *
 *					 left = left | right
 *
 * left  (IN/OUT)	BitArray with num elements
 * right (IN)		BitArray with num elements
 * num	 (IN)		Number of elements in each bit array
 *
 */
void
BitArrayOrSelf(BitArray left, BitArray right, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
		left[idx] = left[idx] | right[idx];
}

/*
 * BitArrayNot
 *
 * This function performs a bitwise Not of the bitarray "from" and stores the
 * result in "to".
 *
 *					 to = ~from
 *
 * from  (IN)	To-be-negated BitArray with num elements
 * to    (IN)	Target BitArray with num elements
 * num	 (IN)	Number of elements in each bit array
 *
 */
void
BitArrayNot(BitArray to, BitArray from, int num) 
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++) 
	{
		to[idx] = ~from[idx];
	}
}
/*
 * BitArrayNotSelf
 *
 * This function performs a bitwise NOT of the bitarrays "left" and "right" and
 * stores the result in "left".
 *
 *					 left = ~ left
 *
 * left  (IN/OUT)	BitArray with num elements
 * num	 (IN)		Number of elements in each bit array
 *
 */
void
BitArrayNotSelf(BitArray left, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
		left[idx] = ~left[idx];
}

bool
BitArrayEqual(BitArray src, BitArray dest, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
	{
		if (src[idx] != dest[idx])
			return false;
	}
	return true;
}

bool
BitArrayContained(BitArray big, BitArray small, int num)
{
	int			idx = 0;
	int			numbytes = BASIZE(num);

	for (idx = 0; idx < numbytes; idx++)
	{
		if ((small[idx] & big[idx]) != small[idx])
			return false;
	}
	return true;
}

void
BitArraySetBit(BitArray bitArray, BitIndex bitIndex)
{
	bitArray[bitIndex / BITS_PER_BYTE] |=
		(1 << (BITS_PER_BYTE - 1 - (bitIndex % BITS_PER_BYTE)));
}

void
BitArrayClearBit(BitArray bitArray, BitIndex bitIndex)
{
	bitArray[bitIndex / BITS_PER_BYTE] &=
		~(1 << (BITS_PER_BYTE - 1 - (bitIndex % BITS_PER_BYTE)));
}

bool
BitArrayBitIsSet(BitArray bitArray, BitIndex bitIndex)
{
	return ((bitArray[bitIndex / BITS_PER_BYTE] &
			 (1 << (BITS_PER_BYTE - 1 - (bitIndex % BITS_PER_BYTE)))
			 ) != 0);
}

void							/* @BdeddySK */
BitArraySetAll(BitArray bitArray, BitIndex numElements)
{
	BitIndex	i;

	for (i = 0; i < numElements; i++)
		BitArraySetBit(bitArray, i);
}

void
BitArrayClearAll(BitArray bitArray, BitIndex numElements)
{
	BitIndex	i;

	for (i = 0; i < numElements; i++)
		BitArrayClearBit(bitArray, i);
}

void
BitArrayRealClearAll(BitArray bitArray, BitIndex numElements)
{
	int			idx;
	int			numbytes = BASIZE(numElements);

	for (idx = 0; idx < numbytes; idx++)
		bitArray[idx] = 0;
}

bool
BitArrayIsAnySet(BitArray bitArray, BitIndex numElements)
{
	BitIndex	 i;
	int 		 numbytes = BASIZE(numElements);

/* 	for (i = 0; i < numElements; i++) */
/* 	{ */
/* 		if (BitArrayBitIsSet(bitArray, i)) */
/* 			return true; */
/* 	} */

	for (i = 0; i < numbytes; i++) 
	{
		if (bitArray[i] != 0) 
		{
			return true;
		}
	}
	return false;
}

bool
BitArrayIsAnyCleared(BitArray bitArray, BitIndex numElements)
{
	BitIndex	i;
	int 		 numbytes = BASIZE(numElements);

/* 	for (i = 0; i < numElements; i++) */
/* 	{ */
/* 		if (!BitArrayBitIsSet(bitArray, i)) */
/* 			return true; */
/* 	} */
	
	for (i = 0; i < numbytes; i++) 
	{
		if (bitArray[i] != 0xFF) 
		{
			return true;
		}
	}
	return false;
}

BitIndex
BitArrayNumSet(BitArray bitArray, BitIndex numElements)
{
	BitIndex	i;
	BitIndex	num = 0;

	for (i = 0; i < numElements; i++)
	{
		if (BitArrayBitIsSet(bitArray, i))
			num++;
	}
	return num;
}	/* @EdeddySK */

void							/* @BdjoinSK */
BitArrayClone(BitArray tgt, BitArray src, int num)
{
	memcpy(tgt, src, sizeof(bits8) * BASIZE(num));
}	/* @EdjoinSK */

/*
 * BitArrayFirstBit
 *
 * This routine returns the position of the first set bit in the BitArray
 *
 * If there is no such bit it returns -1
 */
int
BitArrayFirstBit(BitArray ba, int num)
{
	BitIndex	i,
				j;
	int			set = 0;

	for (i = 0; i < BASIZE(num); i++, set += BITS_PER_BYTE)
	{
		if (ba[i])
		{
			for (j = 0; j < BITS_PER_BYTE; j++)
			{
				if (ba[i] & (1 << (BITS_PER_BYTE - (j + 1))))
					return set + j;
			}
		}
	}
	return -1;
}

/**
 *	 self-explanatory
 */
void
BitArrayPrint(BitArray ba, int num)
{
	int			i;

	for (i = 0; i < num; i++)
	  printf(" %d ", BitArrayBitIsSet(ba, i));
	printf("\n");

}


/**
 *	 self-explanatory
 */
void
BitArrayPrintDebug(BitArray ba, int num)
{
	int			i;
	StringInfoData str;

	if (server_min_messages >= DEBUG1)
	{
		initStringInfo(&str);

		for (i = 0; i < num; i++) 
		{
			appendStringInfo(&str, " %d ", BitArrayBitIsSet(ba, i));
		}
		elog(DEBUG1, "\t\t\t %s", str.data);
		pfree(str.data);
	}
}

/**
 *	 self-explanatory
 */
void
BitArrayPrintLev(BitArray ba, int num, int lev)
{
	int			i;
	StringInfoData str;

	if (server_min_messages >= lev)
	{
		initStringInfo(&str);

		for (i = 0; i < num; i++) 
		{
			appendStringInfo(&str, " %d ", BitArrayBitIsSet(ba, i));
		}
		elog(lev, "\t\t\t %s", str.data);
		pfree(str.data);
	}
}

Word_t
GetBitArrayAsWord_t(BitArray ba, BitIndex num)
{
	Word_t 		w 		 = 0;
	BitArray    b        = (BitArray)(&w);
	int 		numbytes = BASIZE(num);
	int 		i;
	
	Assert(num <= (BITS_PER_BYTE * sizeof(Word_t)));

	if (numbytes == sizeof(Word_t)) 
	{
		w = *((Word_t *) ba);
	}
	else 
	{
		for(i = 0; i < numbytes; i++)
		{
			b[i] = ba[i];
		}
		for(i = numbytes; i < sizeof(Word_t); i++) 
		{
			b[i] = 0;
		}
	}

	return w;
}

Pvoid_t
GetBitArrayAsJudy1(BitArray ba, BitIndex num)
{
	Pvoid_t 	j = NULL;
	int 		i;
	int 		Rc_int;

	for (i = 0; i < num; i++)
	{
		if (BitArrayBitIsSet(ba,i)) 
		{
			J1S(Rc_int, j, (Word_t)i);
		}
	}

	return j;
}
