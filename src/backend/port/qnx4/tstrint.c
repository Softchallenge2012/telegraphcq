/*-------------------------------------------------------------------------
 *
 * tstrint.c
 *	  rint() test
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/port/qnx4/tstrint.c,v 1.2 2004/03/24 08:11:05 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char **argv)
{
	double		x;

	if (argc != 2)
		exit(1);

	x = strtod(argv[1], NULL);
	printf("rint( %f ) = %f\n", x, rint(x));

	return 0;
}
