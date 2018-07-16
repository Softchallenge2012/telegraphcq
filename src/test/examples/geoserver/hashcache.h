#ifndef HASHCACHE_H
#define HASHCACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geoserver.h"

#define DEBUG 1

typedef int bool;

#define TRUE 1
#define FALSE 0

typedef int indicator;

#define SUCCESS 1
#define FAILURE -1

#define NO_MAX_LENGTH -1
#define NEVER_FLUSH -1

#define NOT_IN_CACHE NULL

struct node
{
	char	   *key;
	char	   *value;
	struct node *next;
};

struct head_node
{
	struct node *head;
	int			bucket_length;
};

struct hash_cache
{
	int			num_buckets;
	int			max_bucket_size;

	char	   *file_path;
	int			flush_period;
	int			operation_count;

	struct head_node *buckets;
};

indicator	create_hashcache(struct hash_cache * cache, int num_buckets, int max_length, int flush_period, char *flush_path);
char	   *remove_key(struct hash_cache * cache, char *key);
indicator	add_key(struct hash_cache * cache, char *key, char *value);
char	   *get_value(struct hash_cache * cache, char *key);
indicator	load_hashcache(struct hash_cache * cache, char *path);
indicator	flush_hashcache(struct hash_cache * cache);
void		perform_operation(struct hash_cache * cache);
void		destroy_hashcache(struct hash_cache * cache);
struct node *free_node(struct node * node);
int			hash(char *key);

#endif
