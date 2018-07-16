#include "hashcache.h"

/* returns 0 if initialized, -1 if errors */
indicator
create_hashcache(struct hash_cache * cache, int num_buckets, int max_length, int flush_period, char *flush_path)
{
	int			i;

	if (cache == NULL)
		return FAILURE;

	if (num_buckets > 0)
		cache->num_buckets = num_buckets;
	else
		return FAILURE;

	switch (max_length)
	{
		case NO_MAX_LENGTH:
			cache->max_bucket_size = NO_MAX_LENGTH;
			break;
		case 0:
			return FAILURE;
		default:
			if (max_length < 0)
				return FAILURE;
			cache->max_bucket_size = max_length;
	}

	switch (flush_period)
	{
		case NEVER_FLUSH:
			cache->flush_period = NEVER_FLUSH;
			break;
		case 0:
			return FAILURE;
		default:
			if (flush_period < 0)
				return FAILURE;
			cache->flush_period = flush_period;
	}
	cache->operation_count = 0;

	if (flush_path != NULL)
	{
		cache->file_path = (char *) malloc(strlen(flush_path) + 1);
		strcpy(cache->file_path, flush_path);
	}

	cache->buckets = (struct head_node *) malloc(sizeof(struct head_node) * num_buckets);

	/* initialize the sets */
	for (i = 0; i < num_buckets; i++)
	{
		cache->buckets[i].head = NULL;
		cache->buckets[i].bucket_length = 0;
	}

	return SUCCESS;
}

/* strings are always COPIED into the cache */
indicator
add_key(struct hash_cache * cache, char *key, char *value)
{
	int			hashcode = hash(key);
	int			loc = hashcode % (cache->num_buckets);
	int			key_len = strlen(key);
	int			val_len = strlen(value);

	struct node *new_node = (struct node *) malloc(sizeof(struct node));

	new_node->key = (char *) malloc(sizeof(char) * key_len + 1);
	new_node->value = (char *) malloc(sizeof(char) * val_len + 1);
	strcpy(new_node->key, key);
	strcpy(new_node->value, value);

	/* now insert at the front of the list */
	new_node->next = cache->buckets[loc].head;
	cache->buckets[loc].head = new_node;

	cache->buckets[loc].bucket_length += 1;

	if (cache->max_bucket_size != NO_MAX_LENGTH)
	{
		if (cache->buckets[loc].bucket_length > cache->max_bucket_size)
		{
			struct node *current_node;
			int			i;

			printdbg("Hashcache Add", "Bucket grew too large, removing LRU");
			current_node = cache->buckets[loc].head;
			for (i = 0; i < cache->max_bucket_size - 1; i++)
				current_node = current_node->next;
			cache->buckets[loc].bucket_length--;
			free_node(current_node->next);
			current_node->next = NULL;
		}
	}

	perform_operation(cache);
	return SUCCESS;
}

/* returns removed value, NULL if not in cache */
char *
remove_key(struct hash_cache * cache, char *key)
{
	int			hashcode = hash(key);
	int			loc = hashcode % cache->num_buckets;
	struct node *current_node;
	struct node *prev_node;
	char	   *ret_value;

	current_node = cache->buckets[loc].head;
	prev_node = NULL;
	while (current_node != NULL)
	{
		if (strcmp(key, current_node->key) == 0)
		{
			/* we found it...remove it */
			prev_node->next = current_node->next;
			break;
		}
		else
		{
			/* otherwise, check next node */
			prev_node = current_node;
			current_node = current_node->next;
		}
	}

	if (current_node == NULL)
	{
		/* its not in the hashtable */
		return NOT_IN_CACHE;
	}
	else
	{
		/* its in the hashtable, current_node is it */
		cache->buckets[loc].bucket_length -= 1;
		ret_value = (char *) malloc(sizeof(char) * strlen(current_node->value));
		strcpy(ret_value, current_node->value);
		free(current_node);
	}

	perform_operation(cache);
	return ret_value;
}

/* value returned is copied, MUST BE FREED */
char *
get_value(struct hash_cache * cache, char *key)
{
	int			hashcode = hash(key);
	int			loc = hashcode % cache->num_buckets;
	char	   *ret_value;

	struct node *current_node = cache->buckets[loc].head;
	struct node *prev_node = NULL;

	while (current_node != NULL)
	{
		if (strcmp(key, current_node->key) == 0)
		{
			/* this node was just used...un-attach it from the list */
			if (prev_node != NULL)
			{
				prev_node->next = current_node->next;

				/* now reattach at the beginning */
				current_node->next = cache->buckets[loc].head;
				cache->buckets[loc].head = current_node;
			}
			else
			{
				/* its already at the front! don't do anything */
			}

			/* now return malloc'd copy of its value */
			ret_value = (char *) malloc(sizeof(char) * strlen(current_node->value) +1);
			strcpy(ret_value, current_node->value);
			return ret_value;
		}
		else
		{
			prev_node = current_node;
			current_node = current_node->next;
		}
	}

	return NOT_IN_CACHE;
}

indicator
load_hashcache(struct hash_cache * cache, char *path)
{
	FILE	   *fp = fopen(path, "r");
	char	   *input_string;
	char	   *separator;
	char	   *key;
	char	   *value;
	int			old_flush_period,
				i;

	if (fp == NULL)
		return FAILURE;

	old_flush_period = cache->flush_period;
	cache->flush_period = NEVER_FLUSH;

	input_string = (char *) malloc(sizeof(char) * 130);
	input_string = fgets(input_string, 130, fp);
	while (input_string != NULL)
	{
		/* find the separator between key and value */
		separator = strchr(input_string, (int) ',');
		key = (char *) malloc(sizeof(char) * 16);

		/* blank out key */
		for (i = 0; i < 16; i++)
			key[i] = '\0';

		/* copy out the key */
		strncpy(key, input_string, separator - input_string);
		/* value is everything after the separating comma */
		value = separator + 1;

		/* add this new pair into the cache */
		add_key(cache, key, value);

		/* free the input variables */
		free(input_string);
		input_string = (char *) malloc(sizeof(char) * 130);
		input_string = fgets(input_string, 130, fp);
		free(key);
	}
	free(input_string);

	fclose(fp);

	cache->flush_period = old_flush_period;

	return SUCCESS;
}

indicator
flush_hashcache(struct hash_cache * cache)
{
	int			i,
				ret_value;
	struct node *current_node;
	FILE	   *fp;
	char	   *output_string;

	if (cache->file_path == NULL ||
		strcmp(cache->file_path, "") == 0)
		return FAILURE;

	fp = fopen(cache->file_path, "w");

	if (fp == NULL)
		fprintf(stderr, "Error opening cache file: %s\nAborting...\n", cache->file_path);

	for (i = 0; i < cache->num_buckets; i++)
	{
		current_node = cache->buckets[i].head;
		while (current_node != NULL)
		{
			output_string = (char *) malloc(sizeof(char) * 130);
			strcpy(output_string, current_node->key);
			strcat(output_string, ",");
			strcat(output_string, current_node->value);
			strcat(output_string, "\0");
			ret_value = fputs(output_string, fp);
			if (ret_value < 0)
				fprintf(stderr, "Error writing to disk, BAD flush\n");
			free(output_string);
			current_node = current_node->next;
		}
	}

	fclose(fp);
	return SUCCESS;
}

void
destroy_hashcache(struct hash_cache * cache)
{
	struct node *current_node;
	struct head_node current_head_node;
	int			i;

	for (i = 0; i < cache->num_buckets; i++)
	{
		current_head_node = cache->buckets[i];
		current_node = current_head_node.head;
		while (current_node != NULL)
		{
			/* frees all node & contents */
			current_node = free_node(current_node);
		}
	}
	/* free the buckets */
	free(cache->buckets);
}

/* frees malloc'd contents of a node and returns its next pointer */
struct node *
free_node(struct node * node)
{
	struct node *ret = node->next;

	free(node->key);
	free(node->value);
	free(node);
	return ret;
}

void
perform_operation(struct hash_cache * cache)
{
	if (cache->flush_period != NEVER_FLUSH)
	{
		cache->operation_count += 1;
		if (cache->operation_count >= cache->flush_period)
		{
			cache->operation_count = 0;
			flush_hashcache(cache);
		}
	}
}

int
hash(char *key)
{
	/* do some magical hash function here */
	int			hashcode,
				i;

	hashcode = 0;
	for (i = 0; i < strlen(key); i++)
		hashcode = 31 * hashcode + (int) key[i];
	if (hashcode < 0)
		hashcode = -hashcode;

	return hashcode;
}
