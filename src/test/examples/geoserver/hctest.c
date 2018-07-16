#include "hashcache.h"
#include "dbg_lib.h"
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{

	struct hash_cache *cache;
	char	   *v1;


	load_hashcache(cache, "cache.hc");
	v1 = get_value(cache, "195.157.47.70");
	printf("%s", v1);
	free(v1);

	return 0;
}
