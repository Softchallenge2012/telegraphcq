/*-------------------------------------------------------------------------
 *
 * tcqcommands.c
 *	  TelegraphCQ utilty code.
 *
  * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/commands/tcqcommands.c,v 1.27 2005/12/14 20:49:20 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/indexing.h"
#include "catalog/namespace.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_tcq_wrapper.h"
#include "catalog/pg_tcq_wrapper_usage.h"
#include "catalog/pg_type.h"
#include "commands/comment.h"
#include "commands/defrem.h"
#include "commands/tablecmds.h"
#include "commands/tcqcommands.h"
#include "miscadmin.h"
#include "parser/parse_expr.h"
#include "parser/parse_wrapper.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/relcache.h"
#include "utils/syscache.h"

#include "nodes/makefuncs.h"
    /* For makeRangeVar() */

#include "telegraphcq/wrapch.h"
    /* For makeAuxStreamName() prototype. */

#include "telegraphcq/histogram.h"
    /* For mhist_can_summarize_tuple() function. */

#define INIT_IDX		0
#define NEXT_IDX		1
#define DONE_IDX		2
#define RESCAN_IDX		3
#define MAXIDX			3

/*******************************************************************************
 * PROTOTYPES OF INTERNAL FUNCTIONS
 ******************************************************************************/
static void CheckAndInsertBindings(Oid mappingoid,
					   TupleDesc streamdesc,
					   List *bindings);

static ColumnDef * makeSimpleColumnDef(const char *colname, 
                                        const char *typename);

static void getRidOfStream(const RangeVar * stream_name, Oid stream_oid);

static void 
updateCatalogForOverload(RangeVar *backingrel, OverloadType sumtype,
        char* sumparams);

static void createOOSummaryStreams(RangeVar *backingrel, OverloadType sumtype);
static void createSampleSummaryStreams(CreateStrmStmt *stmt);

void
CreateWrapper(CreateWrapperStmt * cws)
{


	Oid			wrapperoid = InvalidOid;
	List	   *parsed_info = NIL;

	List	   *iter = NULL;
	DefElem    *defelem = NULL;
	char	   *wrappername = cws->wrappername;
	List	  **ptr = NULL;
	List	   *function_names[MAXIDX + 1];
	Oid			function_oids[MAXIDX + 1];
	Datum		values[Natts_pg_tcq_wrapper];
	char		nulls[Natts_pg_tcq_wrapper];
	int			i = 0;
	Relation	rel = NULL;
	TupleDesc	tupDesc = NULL;
	HeapTuple	tup = NULL;


	for (i = 0; i <= MAXIDX; i++)
	{
		function_names[i] = NIL;

	}

	for (i = 0; i < Natts_pg_tcq_wrapper; i++)
	{
		values[i] = PointerGetDatum(NULL);
		nulls[i] = ' ';
	}

	foreach(iter, cws->functions)
	{
		ptr = NULL;
		defelem = (DefElem *) lfirst(iter);
		if (strcasecmp(defelem->defname, CREATEWRAPPER_INIT_NAME) == 0)
			ptr = &function_names[INIT_IDX];
		else if (strcasecmp(defelem->defname, CREATEWRAPPER_NEXT_NAME) == 0)
			ptr = &function_names[NEXT_IDX];
		else if (strcasecmp(defelem->defname, CREATEWRAPPER_DONE_NAME) == 0)
			ptr = &function_names[DONE_IDX];
		else if (strcasecmp(defelem->defname, CREATEWRAPPER_RESCAN_NAME) == 0)
			ptr = &function_names[RESCAN_IDX];
		else
			elog(ERROR, "unknown name=value pair in CREATE WRAPPER");
		if (*ptr != NIL)
			elog(ERROR, "the same function may not be defined twice");


		if (IsA(defelem->arg, List))
			*ptr = (List *) defelem->arg;
		else
			elog(ERROR, "The value for each wrapper function must be a dotted name");

	}

	if (find_wrapper_oid(wrappername) != InvalidOid)
		elog(ERROR, "wrapper '%s' already exists", wrappername);

	for (i = 0; i <= MAXIDX; i++)
	{
		function_oids[i] = find_wrapper_function_oid(function_names[i]);

		if (function_oids[i] == InvalidOid && function_names[i] != NIL)
		{
			elog(ERROR, "CREATE WRAPPER: wrapper function '%s' not found",
				 NameListToString(function_names[i]));
		}
	}

	if (function_oids[NEXT_IDX] == InvalidOid)
		elog(ERROR, "a wrapper must have a NEXT function");

	values[Anum_pg_tcq_wrapper_wrappername - 1] = NameGetDatum(wrappername);
	values[Anum_pg_tcq_wrapper_initproc - 1] = 
        ObjectIdGetDatum(function_oids[INIT_IDX]);
	values[Anum_pg_tcq_wrapper_nextproc - 1] = 
        ObjectIdGetDatum(function_oids[NEXT_IDX]);
	values[Anum_pg_tcq_wrapper_doneproc - 1] = 
        ObjectIdGetDatum(function_oids[DONE_IDX]);
	values[Anum_pg_tcq_wrapper_rescanproc - 1] = 
        ObjectIdGetDatum(function_oids[RESCAN_IDX]);

	if (!parse_key_value_data(cws->info, (void *) &parsed_info))
		elog(ERROR, "wrapperinfo must be0 a comma separated key=value list");

	rel = heap_openr(WrapperRelationName, RowExclusiveLock);
	tupDesc = rel->rd_att;
	tup = heap_formtuple(tupDesc, values, nulls);
	wrapperoid = simple_heap_insert(rel, tup);

	if (parsed_info)
		save_attr_info(wrapperoid, parsed_info);

	/* Need to update indices for either the insert or update case */
	CatalogUpdateIndexes(rel, tup);

	heap_freetuple(tup);
	heap_close(rel, RowExclusiveLock);
}

OverloadParams* makeOverloadParams(OverloadType type, char* paramStr)
{
	OverloadParams* params = (OverloadParams*) palloc(sizeof(OverloadParams));
	params->type = type;
	params->paramStr = paramStr;
	return params;
}

void
CreateStream(CreateStrmStmt * stmt)
{
    /* If this is an unarchived stream, create auxiliary streams for the
     * summaries of dropped and kept tuples. */
    /* TODO: Don't do this for the kind of unarchived stream that spills to
     * disk.  Make sure to keep DropStream() in sync. */

    DefineRelation((CreateStmt *) stmt, stmt->streamtype);

    elog(LOG, "CreateStream(): overload parameters are of type %d",
            stmt->overload_params->type);

    /* Modify the catalog */
    updateCatalogForOverload(stmt->statement.relation,
                stmt->overload_params->type, 
                stmt->overload_params->paramStr);


    if (stmt->streamtype == RELKIND_UNARCHIVEDSTREAM_DROP 
            && stmt->overload_params->type != OVERLOAD_BLOCK
            && stmt->overload_params->type != OVERLOAD_DROP)
    {
        switch (stmt->overload_params->type)
        {
            case OVERLOAD_COUNTS:
            case OVERLOAD_REGHIST:
            case OVERLOAD_MHIST:
            case OVERLOAD_WAVELET:
                createOOSummaryStreams(stmt->statement.relation,
                                        stmt->overload_params->type);
                break;

            case OVERLOAD_SAMPLE:
                createSampleSummaryStreams(stmt);
                break;

            default:
                elog(ERROR, "CREATE STREAM: invalid summary type %d", 
                        stmt->overload_params->type);
        }
    }
}

void
DropStream(const RangeVar *relation, DropBehavior behavior)
{
	int			numUsages = 0;
	Oid			relOid;
	Relation	streamrel = NULL;
	int			streamrelkind = 0;

	relOid = RangeVarGetRelid(relation, false);

	streamrel = heap_open(relOid, NoLock);
	streamrelkind = streamrel->rd_rel->relkind;
	heap_close(streamrel, NoLock);

	numUsages = delete_wrapper_usages(InvalidOid, relOid);
	RemoveRelation(relation, behavior);

    /* If this is an unarchived stream, remove its auxiliary streams. */
    /* TODO: Only do this for unarchived streams of the "drop" variety,
     * as opposed to the "spill" variety. (synchronize with changes to
     * CreateStream())
     */

    if (RELKIND_UNARCHIVEDSTREAM_DROP == streamrelkind) {
        RangeVar    *droppedName = makeAuxStreamName(relation, ANT_DROPPED);
        RangeVar    *keptName = makeAuxStreamName(relation, ANT_KEPT);
//        RangeVar    *droppedCountName = 
//                        makeAuxStreamName(relation, ANT_DROPPED_COUNT);
//        RangeVar    *keptCountName = 
//                        makeAuxStreamName(relation, ANT_KEPT_COUNT);
        Oid         droppedOid;
        Oid         keptOid;
//        Oid         droppedCountOid;
//        Oid         keptCountOid;

        /* Try to grab OIDs for all the aux. streams that could possibly have
         * been created. */
        droppedOid = RangeVarGetRelid(droppedName, true);
        keptOid = RangeVarGetRelid(keptName, true);
//        droppedCountOid = RangeVarGetRelid(droppedCountName, true);
//        keptCountOid = RangeVarGetRelid(keptCountName, true);


        if (OidIsValid(droppedOid)) { 
            getRidOfStream(droppedName, droppedOid);
        }
        if (OidIsValid(keptOid)) {
            getRidOfStream(keptName, keptOid);
        }
//        if (OidIsValid(droppedCountOid)) {
//            getRidOfStream(droppedCountName, droppedCountOid);
//        }
//        if (OidIsValid(keptCountOid)) {
//            getRidOfStream(keptCountName, keptCountOid);
//        }
    }
}


void
DropWrapper(List *wrappername)
{
	Oid			wrapperoid = InvalidOid;
	int			numusages = 0;
	HeapTuple	tup = NULL;
	Relation	rel = NULL;
	char	   *name = strVal(lfirst(wrappername));



	if (length(wrappername) != 1)
	{
		elog(ERROR, "Drop wrapper does not accept dotted name");

	}



	tup = SearchSysCache(WRAPPERNAME,
						 CStringGetDatum(name),
						 0,
						 0,
						 0);

	if (HeapTupleIsValid(tup))
	{
		rel = heap_openr(WrapperRelationName, RowExclusiveLock);
		wrapperoid = HeapTupleGetOid(tup);
		simple_heap_delete(rel, &tup->t_self);
		heap_close(rel, RowExclusiveLock);
		ReleaseSysCache(tup);
	}
	else
		elog(ERROR, "DROP WRAPPER: wrapper '%s' not found", name);


	numusages = delete_wrapper_usages(wrapperoid, InvalidOid);
	delete_attr_info(wrapperoid);



}

void
AlterStreamWrapper(List *streamdname, List *wrapperdname,
				   List *bindings, char * info,  bool isCreate)
{
	List	   *parsed_info = NIL;
	TupleDesc	usagetdesc = NULL;
	TupleDesc	streamtdesc = NULL;
	Relation	usagerel = NULL;
	Relation	streamrel = NULL;
	HeapTuple	tup = NULL;
	int			streamrelkind = 0;
	Oid			streamoid = InvalidOid;
	Oid			wrapperoid = InvalidOid;
	Oid			wrapperusageoid = InvalidOid;
	RangeVar   *rvar = NULL;
	char	   *wrappername = NULL;

	Datum		values[Natts_pg_tcq_wrapper_usage];
	char		nulls[Natts_pg_tcq_wrapper_usage];
	int			i = 0;

	for (i = 0; i < Natts_pg_tcq_wrapper_usage; i++)
	{
		values[i] = PointerGetDatum(NULL);
		nulls[i] = ' ';
	}

	rvar = makeRangeVarFromNameList(streamdname);
	streamoid = RangeVarGetRelid(rvar, true);

	if (streamoid == InvalidOid)
		elog(ERROR, "stream '%s' not found", NameListToString(streamdname));


	streamrel = heap_open(streamoid, NoLock);
	streamtdesc = CreateTupleDescCopy(streamrel->rd_att);
	streamrelkind = streamrel->rd_rel->relkind;
	heap_close(streamrel, NoLock);

	if (!IsAStream(streamrelkind))
		elog(ERROR, "'%s' is not a stream", NameListToString(streamdname));

	if (length(wrapperdname) != 1)
	{
		elog(ERROR, "wrappers are defined using dotted names: %s",
			 NameListToString(wrapperdname));
	}

	wrappername = strVal(lfirst(wrapperdname));

	wrapperoid = find_wrapper_oid(wrappername);

	if (wrapperoid == InvalidOid)
		elog(ERROR, "wrapper '%s' not found", wrappername);


	/* arguments are fine */

	tup = SearchSysCache(WRAPPEROIDUSAGE,
						 ObjectIdGetDatum(wrapperoid),
						 ObjectIdGetDatum(streamoid),
						 0,
						 0);

	if (HeapTupleIsValid(tup))
		wrapperusageoid = HeapTupleGetOid(tup);


	if (isCreate)
	{

		if (wrapperusageoid != InvalidOid)
		{
			ReleaseSysCache(tup);
			elog(ERROR, "wrapper '%s' is already mapped to stream '%s'",
				 wrappername,
				 NameListToString(streamdname));
		}


		values[Anum_pg_tcq_wrapper_usage_wrapperoid - 1] = wrapperoid;
		values[Anum_pg_tcq_wrapper_usage_streamoid - 1] = streamoid;
		usagerel = heap_openr(WrapperUsageRelationName, RowExclusiveLock);
		usagetdesc = usagerel->rd_att;
		tup = heap_formtuple(usagetdesc, values, nulls);
		simple_heap_insert(usagerel, tup);


		/* Need to update indices for either the insert or update case */
		CatalogUpdateIndexes(usagerel, tup);
		CheckAndInsertBindings(HeapTupleGetOid(tup),
							   streamtdesc,
							   bindings);
		if(info != NULL)
		  {
		    if (!parse_key_value_data(info, (void *) &parsed_info))
		      elog(ERROR, "wrapperinfo must be a comma separated key=value list");
		    
		    if (parsed_info)
		      save_attr_info(HeapTupleGetOid(tup), parsed_info);
		  }
		heap_close(usagerel, RowExclusiveLock);
		heap_freetuple(tup);
	}
	else
	{
		if (wrapperusageoid == InvalidOid)
		{
			elog(ERROR, "stream '%s' does not receive data from wrapper '%s'",
				 NameListToString(streamdname),
				 wrappername);
		}
		usagerel = heap_openr(WrapperUsageRelationName, RowExclusiveLock);
		usagetdesc = usagerel->rd_att;
		simple_heap_delete(usagerel, &tup->t_self);
		delete_attr_info(HeapTupleHeaderGetOid(tup->t_data));
		heap_close(usagerel, RowExclusiveLock);

	}
	if (wrapperusageoid != InvalidOid)
		ReleaseSysCache(tup);
}




static void
CheckAndInsertBindings(Oid mappingoid,
					   TupleDesc streamdesc,
					   List *bindings)
{
  int  mappedsize = sizeof(int) * streamdesc->natts;
  int			bindingsize = sizeof(Binding*) * streamdesc->natts;
	char		key[1024];
	char		value[1024];
	List	   *iter = NIL;
	List	   *bindinginfo = NIL;
	char	   *val = NULL;
	int			i = 0;
	int			bindingno = 0;
	int		   *mapped = palloc(mappedsize);
	Binding            **binding_entries = palloc(bindingsize);
	Binding *bind=NULL;
	memset(mapped, -1, mappedsize);
	memset(binding_entries, 0, bindingsize);

	foreach(iter, bindings)
	{
		bind = lfirst(iter);
		val = strVal(bind->column);
		for (i = 0; i <= streamdesc->natts; i++)
		{
			if (strcasecmp(streamdesc->attrs[i]->attname.data,
						   val) == 0)
				break;
		}
		if (i < streamdesc->natts)
		  {
		    mapped[i] = bindingno;
		    binding_entries[i] = bind;
		  }
		else
		{
			elog(ERROR, "column with name '%s' not found",
				 val);
		}
		bindingno++;
	}

	for (i = 0; i < streamdesc->natts; i++)
	{
		if (mapped[i] != -1)
		{
			sprintf(key, "STREAM%d", i + 1);
			sprintf(value, "BINDING%d", mapped[i] + 1);
			bindinginfo = lcons(make_key_value_entry(key, value), bindinginfo);
			sprintf(key, "BINDING%d_optional", mapped[i]+1);
			sprintf(value, (binding_entries[i]->isOptional ? "true" : "false"));
			bindinginfo = lcons(make_key_value_entry(key, value), bindinginfo);
			if(binding_entries[i]->def)
			  {
			    sprintf(key, "BINDING%d_default", mapped[i]+1);
			    sprintf(value, "%s", strVal(binding_entries[i]->def));
			    bindinginfo = lcons(make_key_value_entry(key, value), bindinginfo);
			  }
		}
	}
	save_attr_info(mappingoid, bindinginfo);
	free_attr_info(bindinginfo);
	pfree(mapped);
	pfree(binding_entries);

}

int
delete_wrapper_usages(Oid wrapperoid, Oid streamoid)
{
	HeapTuple	tup = NULL;
	HeapScanDesc scandesc = NULL;
	Relation	rel = NULL;
	ScanKeyData usageKey;
	int			numUsages = 0;
	Oid			argoid = InvalidOid;
	int			argatno = 0;

	if (wrapperoid == InvalidOid &&
		streamoid == InvalidOid)
		elog(ERROR, "drop_wrapper_useages called with both an invalid stream and an invalid wrapper oid");

	if (wrapperoid != InvalidOid &&
		streamoid != InvalidOid)
		elog(ERROR, "drop_wrapper_usages can be called with either a stream or a wrapper, but not both");

	if (wrapperoid != InvalidOid)
	{
		argoid = wrapperoid;
		argatno = Anum_pg_tcq_wrapper_usage_wrapperoid;
	}
	else
	{
		argoid = streamoid;
		argatno = Anum_pg_tcq_wrapper_usage_streamoid;
	}

	rel = heap_openr(WrapperUsageRelationName, RowExclusiveLock);
	ScanKeyEntryInitialize(&usageKey, 0, argatno,
						   F_OIDEQ,
						   ObjectIdGetDatum(argoid));

	scandesc = heap_beginscan(rel, SnapshotNow, 1, &usageKey);

	while (HeapTupleIsValid(tup = heap_getnext(scandesc, ForwardScanDirection)))
	{
		DeleteComments(HeapTupleGetOid(tup), RelationGetRelid(rel), 0);
		simple_heap_delete(rel, &tup->t_self);
		delete_attr_info(HeapTupleGetOid(tup));
		numUsages++;

	}

	heap_endscan(scandesc);
	heap_close(rel, RowExclusiveLock);
	return numUsages;


}

RangeVar * makeAuxStreamName(const RangeVar *orig_relation, 
        enum AuxNameType type) 
{
    RangeVar *ret = makeRangeVar(orig_relation->schemaname, 
                                palloc(256));

    switch (type) {
        case ANT_DROPPED:
            SET_DROPPED_STREAM_NAME(ret->relname, 256, orig_relation->relname);
            break;

        case ANT_KEPT:
            SET_KEPT_STREAM_NAME(ret->relname, 256, orig_relation->relname);
            break;

        case ANT_DROPPED_COUNT:
            SET_DROPPED_COUNT_STREAM_NAME(ret->relname, 256, 
                    orig_relation->relname);
            break;

        case ANT_KEPT_COUNT:
            SET_KEPT_COUNT_STREAM_NAME(ret->relname, 256, 
                    orig_relation->relname);
            break;

        default:
            elog(ERROR, "makeAuxStreamName(): Invalid type.");
    }

    return ret;
}


/* Function to create a column definition for a column on a system type. */
static ColumnDef *
makeSimpleColumnDef(const char *colname, const char *typename)
{
    char *colname_copy = palloc(256);
    char *typename_copy = palloc(256);
    ColumnDef *cd = makeNode(ColumnDef);

    strncpy(colname_copy, colname, 256);
    strncpy(typename_copy, typename, 256);

    cd->colname = colname_copy;

    /* Next four lines copied from SystemTypeName() in gram.c. */
    cd->typename =  makeNode(TypeName);
    cd->typename->names = 
        makeList2(makeString("pg_catalog"), makeString(typename_copy));
    cd->typename->typmod = -1;

    cd->inhcount = 0;
    cd->is_local = true;
    cd->is_not_null = false;
    cd->is_timestamp = false;
    cd->raw_default = NULL;
    cd->cooked_default = NULL;
    cd->constraints = NULL;
    cd->support = NULL;

    return cd;
}

/* Function to remove a stream once you know its OID. */
static void 
getRidOfStream(const RangeVar * stream_name, Oid stream_oid) 
{
    delete_wrapper_usages(InvalidOid, stream_oid);
    RemoveRelation(stream_name, DROP_RESTRICT);
}

/* Function that modifies the catalog entry for a stream to take into account
 * the stream's overload-handling behavior 
 *
 * ARGUMENTS: 
 * <backingrel> is the relation that serves as a placeholder for the
 *      stream.
 * <sumtype> is the type of summary to build for triaged tuples of this stream.
 * <sumparams> is a string holding parameters to pass to the summarizer.
 *
 * TODO: Verify that we are accessing the catalog in the proper manner!!!
 */
static void 
updateCatalogForOverload(RangeVar *backingrel, OverloadType sumtype,
        char* sumparams) 
{
    /* Local variables */
    Relation rel;
    Oid relid;
    HeapTuple tup, newtup;
#define MAX_PARAM_COUNT 8
    char *paramStrs[MAX_PARAM_COUNT];
    int i;
    Relation pg_class_rel;

    /* Parse the comma-delimited parameters string into up to MAX_PARAM_COUNT 
     * parameters. We do the parsing in place... */
    for (i = 0; i < MAX_PARAM_COUNT; i++) {
        paramStrs[i] = "";
            /* Use a default value that won't crash atof() */
    }

    if (NULL != sumparams) {
        int paramno = 0;
        int paramStrLen = strlen(sumparams);
        paramStrs[paramno++] = sumparams;
        for (i = 0; i < paramStrLen; i++) {
            if (',' == sumparams[i]) {
                sumparams[i] = '\0';
                if (i+1 < paramStrLen) paramStrs[paramno++] = &(sumparams[i+1]);

                if (paramno > MAX_PARAM_COUNT) {
                    elog(ERROR, "Too many parameters for summary");
                }
            }
        }
    }

    /* TODO: Check validity of arguments. */

    /*
     * modify the tuple in pg_class to update the reloverloadtype attribute
     * TODO is all this correct/safe?!?!?!?!?!
     */
    CommandCounterIncrement();

    /* Open the backing relation for the stream with an exclusive lock */
    rel = heap_openrv(backingrel, AccessExclusiveLock);

    /* Get a write lock on the row of the catalog table pg_class_rel */
    pg_class_rel = heap_openr(RelationRelationName, RowExclusiveLock);

    relid = RangeVarGetRelid(backingrel, false);
    Assert(RelationGetRelid(rel) == relid);
        /* The two methods of fetching the relid should always return the same
         * result. */

    tup = SearchSysCache(RELOID, ObjectIdGetDatum(relid), 0, 0, 0);
    newtup = heap_copytuple(tup);

    /* Tuples in the catalog table hold objects.  Pull out the Form_pg_class
     * object and set the appropriate fields. */
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadtype = sumtype;

    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam1 = atof(paramStrs[0]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam2 = atof(paramStrs[1]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam3 = atof(paramStrs[2]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam4 = atof(paramStrs[3]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam5 = atof(paramStrs[4]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam6 = atof(paramStrs[5]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam7 = atof(paramStrs[6]);
    ((Form_pg_class)GETSTRUCT(newtup))->reloverloadparam8 = atof(paramStrs[7]);

    /* TODO: Should we be doing the following (done for creating new relations)
     * */
/*        HeapTuple tup = heap_addheader(Natts_pg_class_fixed, 
                true, CLASS_TUPLE_SIZE, (void*) rel->rd_rel); */
/*        HeapTupleSetOid(tup, relid); */

    /* Now we can do the actual update */
    simple_heap_update(pg_class_rel, &newtup->t_self, newtup); 
    CatalogUpdateIndexes(pg_class_rel, newtup);

    heap_freetuple(newtup);
    ReleaseSysCache(tup);

    /* Close the relations.  The locks will be released at transaction commit.
     */
    heap_close(pg_class_rel, NoLock);
    heap_close(rel, NoLock);
}


/* Function that creates the summary streams for a relation.  Assumes that the
 * summary streams produce one OODBMS object per summary window. 
 *
 * ARGUMENTS: 
 * <backingrel> is the relation that we use as a placeholder for the stream.
 * <sumtype> defines how we will summarize triaged tuples for the stream.
 */
static void createOOSummaryStreams(RangeVar *backingrel, OverloadType sumtype)
{
    char        *syn_type = NULL;
    CreateStmt  *drop_stream = NULL;
    CreateStmt  *kept_stream = NULL;
    ColumnDef   *syn_cd = NULL;
    ColumnDef   *temp_cd = NULL;
    ColumnDef   *oldest_ts_cd = NULL;
    ColumnDef   *newest_ts_cd = NULL;

    /* Convert the summary tupe into an OO datatype. */
    /* TODO: Do this with a catalog lookup */
    switch (sumtype) {
        case OVERLOAD_COUNTS:
            syn_type = "int4";
            break;
        case OVERLOAD_REGHIST:
            /* We implement fixed-grid histograms as mhists for now. */
            syn_type = "mhist";
            break;
        case OVERLOAD_MHIST:
            syn_type = "mhist";
            break;
        case OVERLOAD_WAVELET:
            syn_type = "wcs";
            break;
        default:
            elog(ERROR, 
                "Can't create single-object summary for summary type %d", 
                sumtype);
            /* unreachable */
    }


    elog(LOG, "CreateStream(): Creating auxiliary streams.");

    /* Will we be using mhists for this summary?  We currently use them
     * whenever possible. */
    /* The call to DefineRelation() above created all the metadata we need.
     * Fetch that metadata... */
    /*
    r = relation_open(reloid, NoLock);
    use_mhist = mhist_can_summarize_tuple(RelationGetDescr(r));
    relation_close(r, NoLock);
    */

    /* Fill in values that I copied out of ddd after typing in 
     * "create stream __[streamname]_dropped(summary mhist) 
     *      type unarchived"
     */
    drop_stream = makeNode(CreateStmt);

    /* First we create a range variable to represent the new relation's
     * name. */
    drop_stream->relation = 
        makeAuxStreamName(backingrel, ANT_DROPPED);

    /* Create a ColumnDef object that represents a single MHIST column,
     * and one for each of the timestamp columns. */
    syn_cd = makeSimpleColumnDef("summary", syn_type);
    /* TEMPORARY: TelegraphCQ has trouble joining on timestamps, so we 
     * create an auxiliary integer column to join the summary streams
     * together. */
    temp_cd = makeSimpleColumnDef("window_num", "int4");


    oldest_ts_cd = makeSimpleColumnDef("prev_tcqtime", "timestamp");
    newest_ts_cd = makeSimpleColumnDef("tcqtime", "timestamp");

    newest_ts_cd->is_timestamp = true;


    drop_stream->tableElts = makeList4(syn_cd, temp_cd, oldest_ts_cd, 
                                        newest_ts_cd);

    /* Fill in miscellaneous details. */
    drop_stream->inhRelations = NULL;
    drop_stream->constraints = NULL;

    drop_stream->hasoids = true;

    /* TODO: Add the timestamp constraint. */

    
    /* Create the descriptor for the "kept summary" stream by making a copy
     * of the other descriptor and changing the name. */
    kept_stream = (CreateStmt*)(copyObject(drop_stream));
    kept_stream->relation = makeAuxStreamName(backingrel, ANT_KEPT);

    /* Now we can actually create the streams. */
    elog(LOG, "CreateStream():     'Dropped' stream: %s.%s",
            drop_stream->relation->schemaname,
            drop_stream->relation->relname);

    DefineRelation(drop_stream, RELKIND_ARCHIVEDSTREAM);

    elog(LOG, "CreateStream():     'Kept' stream: %s.%s",
            kept_stream->relation->schemaname,
            kept_stream->relation->relname);

    DefineRelation(kept_stream, RELKIND_ARCHIVEDSTREAM);

    /* TODO: Hide the summary streams from view. */

}

/* Function that creates the summary streams for a relation.  Assumes that the
 * summary streams are based on sampling.
 *
 * ARGUMENTS: 
 * <stmt> is the CREATE STREAM statement for the original stream.
 */
static void createSampleSummaryStreams(CreateStrmStmt *stmt)
{
    CreateStmt  *drop_stream;
    CreateStmt  *kept_stream;
    RangeVar    *backingrel = stmt->statement.relation;

    /* Convert the CreateStrmStmt into a vanilla CreateStmt temporarily so that
     * we can make copies of it (minus the overload params), then make copies
     * and change the original back. */
    CreateStmt  *stmt_cast = &(stmt->statement);
    NodeTag     orig_tag = stmt_cast->type;
    stmt_cast->type = T_CreateStmt;

    drop_stream = (CreateStmt*)(copyObject(stmt_cast));
    kept_stream = (CreateStmt*)(copyObject(stmt_cast));
    stmt_cast->type = orig_tag;

    /* Change the stream names in place */
    drop_stream->relation = makeAuxStreamName(backingrel, ANT_DROPPED);
    kept_stream->relation = makeAuxStreamName(backingrel, ANT_KEPT);

    /* Add the __samplemult column.  It needs to be the last column. */
    {
        ColumnDef   *samplemult_cd = NULL;
        /* Add the __samplemult field to the summary streams. */
        samplemult_cd = makeSimpleColumnDef("__samplemult", "float4");

        lappend(drop_stream->tableElts, (void*)samplemult_cd);
        lappend(kept_stream->tableElts, (void*)samplemult_cd);
    }

    /* Now we can actually create the streams. */
    elog(LOG, "CreateStream():     'Dropped' stream: %s.%s",
            drop_stream->relation->schemaname,
            drop_stream->relation->relname);

    DefineRelation(drop_stream, RELKIND_UNARCHIVEDSTREAM_DROP);

    elog(LOG, "CreateStream():     'Kept' stream: %s.%s",
            kept_stream->relation->schemaname,
            kept_stream->relation->relname);

    DefineRelation(kept_stream, RELKIND_UNARCHIVEDSTREAM_DROP);

    /* TODO: Hide the summary streams from view. */
}
