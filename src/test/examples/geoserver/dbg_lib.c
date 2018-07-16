#include "dbg_lib.h"

void
printdbg(char *loc, char *msg)
{
	if (DEBUG)
		printf("%s: %s\n", loc, msg);
}
