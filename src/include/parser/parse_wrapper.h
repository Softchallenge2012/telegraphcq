/*-------------------------------------------------------------------------
 *
 * parse_wrapper.h
 *
 * Author: Owen Cooper
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: parse_wrapper.h,v 1.7 2004/08/07 01:01:38 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef PARSEWRAPPER_H
#define PARSEWRAPPER_H

#include "nodes/parsenodes.h"

/* used in calls to wrapper usage query function */
#define OIDKIND_STREAM 1
#define OIDKIND_WRAPPER 2
#define OIDKIND_MAPPING 3
#define OID_RETURN_ARRAY_SIZE 1 /* should be larger after testing */

#define WRAPPERTYPEKEY "WRAPPERTYPE"
#define HOSTNAME_KEY "HOST"
#define PORT_KEY "PORT"
#define PULLWRAPPER_NAME "PULL"
#define PUSHWRAPPER_NAME "PUSH"


typedef struct key_value_entry
{
	char	   *key;
	char	   *value;
}	key_value_entry;




typedef struct wrapper
{
	char	   *name;
	Oid			wrapperoid;
	Oid			initoid;
	Oid			nextoid;
	Oid			doneoid;
	Oid			rescanoid;
	FunctionCachePtr init;
	FunctionCachePtr next;
	FunctionCachePtr done;
	FunctionCachePtr rescan;
	List	   *info;

}	wrapper;



Oid			find_wrapper_function_oid(List *function_name);
Oid			find_wrapper_oid(char *wrappername);
wrapper    *load_wrapper_by_oid(Oid o);
wrapper    *load_wrapper_by_name(char *name);
bool		load_wrapper_functions(wrapper * w);
void		delete_wrapper(wrapper ** w);
void get_wrappers_for_stream(Oid streamoid,
						Oid **wrapper_oids,
						Oid **mapping_oids,
						int *wrapper_oids_size);

void get_streams_for_wrapper(Oid wrapperoid,
						Oid **stream_oids,
						Oid **mapping_oids,
						int *stream_oids_size);
void get_wrapper_stream_mapping(Oid queryoid,
						   int queryoid_kind,
						   Oid **return_oids,
						   Oid **mapping_oids,
						   Oid *return_oids_size);
bool		is_valid_wrapper_usage(Oid wrapperoid, Oid streamoid);
Oid get_wrapper_stream_mappingoid(Oid wrapperoid, Oid streamoid);

List	   *get_stream_name_by_oid(Oid streamoid);
bool		is_pull_wrapper(wrapper * w);

List	   *makedupNameList1(char *name1);
List	   *makedupNameList2(char *name1, char *name2);
List	   *makedupNameList3(char *name1, char *name2, char *name3);

List	   *makedupNameList1(char *name1);
List	   *makedupNameList2(char *name1, char *name2);
List	   *makedupNameList3(char *name1, char *name2, char *name3);

List	   *StringToNameList(char *str);
void		freeNameList(List *l);


/* access routines for storage in pg_tcq_attrs table */

/* in memory manipulation */
bool		parse_key_value_data(char *str, void **parsed_data);
void		free_attr_info(void *l);
void            free_key_value_entry(key_value_entry*k);
key_value_entry *   copy_key_value_entry(key_value_entry*src);
key_value_entry *get_entry(char *key, void *parsed_data);
key_value_entry *make_key_value_entry(char *key, char *value);

/* catalog manipulation */
void		delete_attr_info(Oid owneroid);
void	   *append_attr_info(Oid owneroid, void*old_info);
void	   *load_attr_info(Oid owneroid);
void		save_attr_info(Oid owneroid, List *key_value);

#endif   /* PARSEWRAPPER_H */
