/* $Id: random.c,v 1.1.1.1 2003/04/17 23:03:56 sailesh Exp $ */

#include "c.h"

#include <stdlib.h>
#include <math.h>
#include <errno.h>

long
random()
{
	return lrand48();
}
