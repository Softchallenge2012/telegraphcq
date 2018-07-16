#include "hash.h"

/* returns 0 if initialized, -1 if errors */
indicator
create_hashtable(struct hashtable * table, int num_buckets, int max_length)
{
	if (table == NULL)
		return FAILURE;

	if (num_buckets > 0)
		table->num_buckets = num_buckets;
	else
		return FAILURE;

	if (max_length >= 0)
	{
		table->hash_cache = TRUE;
		table->max_bucket_size = max_length;
	}
	else
	{
		table->hash_cache = FALSE;
		table->max_bucket_size = -1;
	}

	table->buckets = (Node *) malloc(sizeof(struct Node) * num_buckets);

	return SUCCESS;
}

indicator
add_key(struct hashtable * table, char *key)
{
}

indicator *
remove_key(struct hashtable * table, char *key)
{
}

char *
get_value(struct hashtable * table, char *key)
{
}

indicator *
load_hash_table(struct hashtable * table)
{
}

indicator *
flush_hash_table(struct hashtable * table)
{
}

indicator *
destroy_hash_table(struct hashtable * table)
{
}
