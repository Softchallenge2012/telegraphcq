/*-------------------------------------------------------------------------
 *
 * parse_wrapper.c
 *	  TelegraphCQ code to deal with wrapper caltalog at runtime.
 *
  * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/parser/parse_wrapper.c,v 1.9 2004/08/07 01:01:37 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"
#include "parser/parse_wrapper.h"
#include "parser/parse_func.h"
#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/pg_tcq_wrapper.h"
#include "catalog/pg_namespace.h"
#include "catalog/pg_tcq_wrapper_usage.h"
#include "catalog/pg_tcq_attrs.h"
#include "catalog/pg_type.h"
#include "catalog/pg_proc.h"
#include "catalog/indexing.h"
#include "utils/syscache.h"
#include "utils/builtins.h"
#include "utils/fcache.h"
#include "utils/fmgroids.h"

#define SEPCHAR '\\'
static FunctionCachePtr loadFunctionByOid(Oid o);
char	   *pstrndupremovechar(char *str, int len, char toberemoved);


Oid
find_wrapper_oid(char *wrappername)
{
	HeapTuple	tup = NULL;
	Oid			ret = InvalidOid;

	if (wrappername == NULL)
		return ret;

	tup = SearchSysCache(WRAPPERNAME,
						 CStringGetDatum(wrappername),
						 0,
						 0,
						 0);
	if (HeapTupleIsValid(tup))
	{
		ret = HeapTupleGetOid(tup);
		ReleaseSysCache(tup);
	}
	return ret;

}

Oid
find_wrapper_function_oid(List *function_name)
{


	Oid			argtypes[FUNC_MAX_ARGS];


	if (function_name == NULL)
		return InvalidOid;


	/* needed for proper hash computation  */
	MemSet(argtypes, InvalidOid, sizeof(argtypes));
	argtypes[0] = INT4OID;

	return LookupFuncName(function_name, 1, argtypes);


}

wrapper *
load_wrapper_by_name(char *wrappername)
{
	Oid			wrapperoid = find_wrapper_oid(wrappername);

	return load_wrapper_by_oid(wrapperoid);
}

wrapper *
load_wrapper_by_oid(Oid o)
{
	wrapper    *ret = NULL;
	HeapTuple	tup = NULL;
	FormData_pg_tcq_wrapper *row = NULL;

	if (o == InvalidOid)
		return NULL;

	tup = SearchSysCache(WRAPPEROID,
						 ObjectIdGetDatum(o),
						 0,
						 0,
						 0);

	if (HeapTupleIsValid(tup))
	{
		row = (FormData_pg_tcq_wrapper *) GETSTRUCT(tup);
		ret = palloc(sizeof(wrapper));
		ret->name = pstrdup(row->wrappername.data);
		ret->wrapperoid = o;
		ret->initoid = row->initproc;
		ret->nextoid = row->nextproc;
		ret->doneoid = row->doneproc;
		ret->rescanoid = row->rescanproc;
		ret->init = NULL;
		ret->next = NULL;
		ret->done = NULL;
		ret->rescan = NULL;
		ret->info = load_attr_info(o);
		ReleaseSysCache(tup);
	}
	return ret;
}

bool
load_wrapper_functions(wrapper * w)
{
	if (w->initoid && !w->init)
		w->init = loadFunctionByOid(w->initoid);
	if (w->nextoid && !w->next)
		w->next = loadFunctionByOid(w->nextoid);
	if (w->doneoid && !w->done)
		w->done = loadFunctionByOid(w->doneoid);
	if (w->rescanoid && !w->rescan)
		w->rescan = loadFunctionByOid(w->rescanoid);
	return true;
}

void
delete_wrapper(wrapper ** w)
{
	if (*w == NULL)
		return;
	if ((*w)->name)
		pfree((*w)->name);
	if ((*w)->init)
		pfree((*w)->init);
	if ((*w)->next)
		pfree((*w)->next);
	if ((*w)->done)
		pfree((*w)->done);
	if ((*w)->rescan)
		pfree((*w)->rescan);
	if ((*w)->info)
		free_attr_info((*w)->info);
	pfree(*w);
	*w = NULL;
}

FunctionCachePtr
loadFunctionByOid(Oid o)
{
	if (o != InvalidOid)
		return init_fcache(o, 1, CurrentMemoryContext);

	return NULL;
}

bool
is_valid_wrapper_usage(Oid wrapperoid, Oid streamoid)
{
	return SearchSysCacheExists(WRAPPEROIDUSAGE,
								ObjectIdGetDatum(wrapperoid),
								ObjectIdGetDatum(streamoid),
								0, 0);

}

Oid
get_wrapper_stream_mappingoid(Oid wrapperoid, Oid streamoid)
{
	return GetSysCacheOid(WRAPPEROIDUSAGE,
								ObjectIdGetDatum(wrapperoid),
								ObjectIdGetDatum(streamoid),
								0, 0);

}

void
get_wrappers_for_stream(Oid streamoid,
						Oid **wrapper_oids,
						Oid **mapping_oids,
						int *wrapper_oids_size)
{
	get_wrapper_stream_mapping(streamoid,
							   OIDKIND_STREAM,
							   wrapper_oids,
							   mapping_oids,
							   wrapper_oids_size);


}

void
get_streams_for_wrapper(Oid wrapperoid,
						Oid **stream_oids,
						Oid **mapping_oids,
						int *stream_oids_size)
{
	get_wrapper_stream_mapping(wrapperoid,
							   OIDKIND_WRAPPER,
							   stream_oids,
							   mapping_oids,
							   stream_oids_size);

}

void
get_wrapper_stream_mapping(Oid queryoid,
						   int queryoid_kind,
						   Oid **return_oids,
						   Oid **mapping_oids,
						   Oid *return_oids_size)
{
	HeapTuple	tup = NULL;
	HeapScanDesc scandesc = NULL;
	Relation	rel = NULL;
	ScanKeyData usageKey;
	int			numoidsreturned = 0;
	Oid			argoid = InvalidOid;
	bool		isNull = false;
	int			argattno = 0;
	int			answerattno = 0;

	if (return_oids == NULL ||
		return_oids_size == NULL)
		elog(FATAL, "wrapper_stream_mapping passed invalid oid array or array size  pointer");


	*return_oids_size = OID_RETURN_ARRAY_SIZE;
	*return_oids = palloc(OID_RETURN_ARRAY_SIZE * sizeof(Oid));
	memset(*return_oids, InvalidOid, (*return_oids_size) * sizeof(Oid));
	if (mapping_oids)
	{
		*mapping_oids = palloc(OID_RETURN_ARRAY_SIZE * sizeof(Oid));
		memset(*mapping_oids, InvalidOid, (*return_oids_size) * sizeof(Oid));
	}

	argoid = queryoid;
	if (queryoid_kind == OIDKIND_STREAM)
	{
		argattno = Anum_pg_tcq_wrapper_usage_streamoid;
		answerattno = Anum_pg_tcq_wrapper_usage_wrapperoid;
	}
	else if (queryoid_kind == OIDKIND_WRAPPER)
	{
		argattno = Anum_pg_tcq_wrapper_usage_wrapperoid;
		answerattno = Anum_pg_tcq_wrapper_usage_streamoid;
	}
	else
		elog(FATAL, "wrapper_stream_mapping called with invalid oid type argument");

	rel = heap_openr(WrapperUsageRelationName, AccessShareLock);
	ScanKeyEntryInitialize(&usageKey, 0, argattno,
						   F_OIDEQ,
						   ObjectIdGetDatum(argoid));

	scandesc = heap_beginscan(rel, SnapshotNow, 1, &usageKey);

	while (HeapTupleIsValid(tup = heap_getnext(scandesc, ForwardScanDirection)))
	{
		if (numoidsreturned == *return_oids_size)
		{
			/* need to expand the return array */
			int			newsize = (*return_oids_size) * 2;
			Oid		   *newarray = palloc(newsize * sizeof(Oid));

			memset(newarray, InvalidOid, (newsize * sizeof(Oid)));
			memcpy(newarray, *return_oids, ((*return_oids_size) * sizeof(Oid)));
			pfree(*return_oids);
			*return_oids = newarray;
			if (mapping_oids)
			{
				newarray = palloc(newsize * sizeof(Oid));
				memset(newarray, InvalidOid, (newsize * sizeof(Oid)));
				memcpy(newarray, *mapping_oids,
					   ((*return_oids_size) * sizeof(Oid)));
				pfree(*mapping_oids);
				*mapping_oids = newarray;
			}
			*return_oids_size = newsize;
		}
		if (mapping_oids)
			(*mapping_oids)[numoidsreturned] = HeapTupleGetOid(tup);
		(*return_oids)[numoidsreturned++] = heap_getattr(tup,
														 answerattno,
														 rel->rd_att,
														 &isNull);

	}


	heap_endscan(scandesc);
	heap_close(rel, AccessShareLock);


}

List *
get_stream_name_by_oid(Oid streamoid)
{
	HeapTuple	tup = NULL;
	HeapTuple	nstup = NULL;

	char	   *streamname = NULL;
	char	   *schemaname = NULL;
	List	   *ret = NIL;

	tup = SearchSysCache(RELOID,
						 ObjectIdGetDatum(streamoid),
						 0,
						 0,
						 0);
	if (HeapTupleIsValid(tup))
	{
		FormData_pg_namespace *nsrow = NULL;
		FormData_pg_class *row = (FormData_pg_class *) GETSTRUCT(tup);

		if (IsAStream(row->relkind))
			streamname = pstrdup(row->relname.data);
		nstup = SearchSysCache(NAMESPACEOID,
							   ObjectIdGetDatum(row->relnamespace),
							   0,
							   0,
							   0);

		if (HeapTupleIsValid(nstup))
		{
			nsrow = (FormData_pg_namespace *) GETSTRUCT(nstup);
			schemaname = nsrow->nspname.data;
			ReleaseSysCache(nstup);
		}


		ReleaseSysCache(tup);
	}

	if (schemaname && streamname)
		ret = makedupNameList2(schemaname, streamname);
	else
		ret = makedupNameList1(schemaname);


	return ret;
}

List *
StringToNameList(char *str)
{
	char	   *cp = NULL;
	char	   *end = NULL;
	List	   *ret = NIL;
	int			i = 0;
	char	   *names[3];
	char	   *cstr = NULL;

	if (str == NULL)
		return NULL;
	else
		cstr = pstrdup(str);

	for (i = 0, cp = cstr; cp && i < 3; i++)
	{
		names[i] = strchr(cp, '.');
		if (names[i])
		{
			end = names[i];
			names[i] = cp;
			*end = '\0';
			cp = end + 1;
		}
		else
		{
			names[i] = cp;
			break;
		}


	}
	switch (i)
	{
		case 0:
			ret = makedupNameList1(names[0]);
			break;
		case 1:
			ret = makedupNameList2(names[0], names[1]);
			break;
		case 3:
			ret = makedupNameList3(names[0], names[1], names[3]);
		default:
			ret = NULL;

	}
	if (cstr)
		pfree(cstr);
	return ret;

}

List *
makedupNameList1(char *name1)
{
	return makeList1(makeString(pstrdup(name1)));
}

List *
makedupNameList2(char *name1, char *name2)
{
	return makeList2(makeString(pstrdup(name1)),
					 makeString(pstrdup(name2)));
}


List *
makedupNameList3(char *name1, char *name2, char *name3)
{
	return makeList3(makeString(pstrdup(name1)),
					 makeString(pstrdup(name2)),
					 makeString(pstrdup(name3)));
}

void
freeNameList(List *namelist)
{
	List	   *iter;
	Value	   *v = NULL;


	foreach(iter, namelist)
	{
		v = (Value *) lfirst(iter);
		if (v->type == T_String)
		{
			if (v->val.str)
				pfree(v->val.str);
		}
		pfree(v);
	}

	freeList(namelist);
}
bool
is_pull_wrapper(wrapper * w)
{
	/* the wrong implementation of this function, but it will do for now */

	bool		ret = false;
	key_value_entry *e;

	if (w)
	{
		e = get_entry(WRAPPERTYPEKEY, w->info);
		if (e && strcasecmp(e->value, PULLWRAPPER_NAME) == 0)
			ret = true;
	}

	return ret;
}


void
delete_attr_info(Oid owneroid)
{

	HeapTuple	tup = NULL;
	HeapScanDesc scandesc = NULL;
	Relation	rel = NULL;
	ScanKeyData usageKey;

	rel = heap_openr(TcqAttrRelationName, RowExclusiveLock);
	ScanKeyEntryInitialize(&usageKey, 0,
						   Anum_pg_tcq_attrs_objoid,
						   F_OIDEQ,
						   ObjectIdGetDatum(owneroid));

	scandesc = heap_beginscan(rel, SnapshotNow, 1, &usageKey);

	while (HeapTupleIsValid(tup = heap_getnext(scandesc,
											   ForwardScanDirection)))
		simple_heap_delete(rel, &(tup->t_self));

	heap_endscan(scandesc);
	heap_close(rel, RowExclusiveLock);

}


void * 
append_attr_info(Oid owner, void *info)
{
  void *newinfo = load_attr_info(owner);
  List * ltmp=NULL;
  key_value_entry * entry = NULL;
  key_value_entry * tmp = NULL;
  foreach(ltmp, (List*)newinfo)
    {
      entry = lfirst(ltmp);
      tmp= get_entry(entry->key, info);
      if(tmp == NULL)
	{
	  info = lcons(copy_key_value_entry(entry),info);
	}
      else
	{
	  tmp->value = pstrdup(entry->value);
	}
    }
  free_attr_info(newinfo);
  newinfo=NULL;
     return info;
}

void *
load_attr_info(Oid owneroid)
{
	List	   *ret = NIL;
	HeapTuple	tup = NULL;
	HeapScanDesc scandesc = NULL;
	Relation	rel = NULL;
	ScanKeyData usageKey;
	bool		isNull;

	rel = heap_openr(TcqAttrRelationName, AccessShareLock);
	ScanKeyEntryInitialize(&usageKey, 0,
						   Anum_pg_tcq_attrs_objoid,
						   F_OIDEQ,
						   ObjectIdGetDatum(owneroid));

	scandesc = heap_beginscan(rel, SnapshotNow, 1, &usageKey);

	while (HeapTupleIsValid(tup = heap_getnext(scandesc,
											   ForwardScanDirection)))
	{
		Datum		keyattr = (Datum) NULL;
		Datum		valueattr = (Datum) NULL;

		key_value_entry *entry = palloc(sizeof(key_value_entry));

		keyattr = heap_getattr(tup,
							   Anum_pg_tcq_attrs_key,
							   rel->rd_att,
							   &isNull);
		valueattr = heap_getattr(tup,
								 Anum_pg_tcq_attrs_value,
								 rel->rd_att,
								 &isNull);
		entry->key = DatumGetCString(DirectFunctionCall1(textout,
														 keyattr));

		entry->value = DatumGetCString(DirectFunctionCall1(textout,
														   valueattr));


		ret = lcons(entry, ret);

	}

	heap_endscan(scandesc);
	heap_close(rel, AccessShareLock);
	return ret;

}

void
save_attr_info(Oid owneroid, List *key_value)
{
	List	   *iter = NIL;
	key_value_entry *entry;
	HeapTuple	tup;
	Relation	rel;
	Datum		row[Natts_pg_tcq_attrs];
	char		nulls[Natts_pg_tcq_attrs];
	int			i = 0;


	if (key_value == NIL)
		return;

	for (i = 0; i < Natts_pg_tcq_attrs; i++)
	{
		nulls[i] = ' ';
		row[i] = (Datum) NULL;
	}

	row[Anum_pg_tcq_attrs_objoid - 1] = ObjectIdGetDatum(owneroid);

	rel = heap_openr(TcqAttrRelationName, RowExclusiveLock);
	foreach(iter, key_value)
	{
		entry = lfirst(iter);
		row[Anum_pg_tcq_attrs_key - 1] =
			DirectFunctionCall1(textin,
								CStringGetDatum(entry->key));
		row[Anum_pg_tcq_attrs_value - 1] =
			DirectFunctionCall1(textin,
								CStringGetDatum(entry->value));
		tup = heap_formtuple(rel->rd_att, row, nulls);
		if (HeapTupleIsValid(tup))
			simple_heap_insert(rel, tup);

		pfree(DatumGetPointer(row[Anum_pg_tcq_attrs_key - 1]));
		pfree(DatumGetPointer(row[Anum_pg_tcq_attrs_value - 1]));
		pfree(tup);
	}
	heap_close(rel, RowExclusiveLock);
}


bool
parse_key_value_data(char *info_string, void **info_list)
{
	bool		return_value = true;

	char	   *str = NULL;
	char	   *start = str;

	char	   *next_entry = NULL;
	char	   *key_separator = NULL;
	int			key_len = 0;
	int			value_len = 0;
	key_value_entry *entry = NULL;

	*info_list = NIL;

	if (info_string)
		str = pstrdup(info_string);
	else
		return true;


	do
	{
		if (!str || *str == '\0')
			break;
		key_len = value_len = 0;
		next_entry = key_separator = NULL;

		while ((next_entry = strchr(str, ',')) &&
			   (next_entry > start) &&
			   (*(next_entry - 1) == SEPCHAR))
			str = next_entry + 1;
		key_separator = strchr(str, '=');
		if (key_separator != NULL)
			*key_separator = '\0';
		if (next_entry != NULL)
			*next_entry = '\0';
		if (next_entry && ((key_separator > next_entry) ||
						   key_separator == NULL))
		{
			free_attr_info(*info_list);
			return false;
		}
		entry = make_key_value_entry(str, key_separator + 1);
		(*info_list) = lcons(entry, (*info_list));
		str = next_entry;
		if (str)
			str++;
	}
	while (next_entry != NULL);
	return return_value;
}

key_value_entry *
copy_key_value_entry(key_value_entry *kve)
{
  key_value_entry *newentry = palloc(sizeof(key_value_entry));
  newentry->key = pstrdup(kve->key);
  newentry->value= pstrdup(kve->value);
return newentry;
}
				     
void
free_attr_info(void *arg)
{
	List	   *larg = (List *) arg;
	List	   *iter;

	foreach(iter, larg)
	  {
	    key_value_entry *kve = lfirst(iter);
	    free_key_value_entry(kve);
	  }
	freeList(larg);
}

void free_key_value_entry(key_value_entry*kve)
{
  if(kve->key !=NULL) pfree(kve->key);
  if(kve->value !=NULL) pfree(kve->value);
  kve->key=NULL;
  kve->value=NULL;
  pfree(kve);
}

key_value_entry *
get_entry(char *key, void *key_value_data)
{
	List	   *list = (List *) key_value_data;
	List	   *iter = NIL;
	key_value_entry *entry;

	foreach(iter, list)
	{
		entry = lfirst(iter);
		if (strcasecmp(entry->key, key) == 0)
			return entry;
	}
	return NULL;
}

key_value_entry *
make_key_value_entry(char *key, char *value)
{
	key_value_entry *entry = NULL;

	entry = (key_value_entry *) palloc(sizeof(key_value_entry));
	entry->key = pstrdup(key);
	entry->value = pstrdup(value);
	return entry;
}


char *
pstrndupremovechar(char *str, int len, char toberemoved)
{
	int			maxlen = 0;
	char	   *ret = NULL;
	char	   *sp = NULL;
	char	   *dp = NULL;
	int			copied = 0;

	if (str == NULL)
		return NULL;

	maxlen = len + 1;
	ret = palloc(maxlen);

	for (sp = str, dp = ret; copied < len; copied++)
	{
		if (*sp == toberemoved)
			sp++;
		else
			*(dp++) = *(sp++);

	}
	*dp = '\0';
	return ret;

}
