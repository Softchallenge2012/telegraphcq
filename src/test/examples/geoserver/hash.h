#include <stdio.h>
#include <stdlib.h>

typedef int bool;

#define TRUE 1
#define FALSE 0

typedef int indicator;

#define SUCCESS 1
#define ERROR 0

struct hashcache
{
	int			num_buckets;
	bool		hash_cache;
	int			max_bucket_size;


	char	   *file_path;
	int			flush_period;

				HeadNode[] buckets;
};

struct HeadNode
{
	Node	   *next;
	int			bucket_length;
}

struct Node
{
	char	   *key;
	char	   *value;
	Node	   *next;
};
