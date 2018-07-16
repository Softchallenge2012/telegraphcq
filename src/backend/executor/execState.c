/*---------------------------------------------------------------------------------
 *
 * execState.c
 *	  contains dispatch functions which call the appropriate "get state" and
 *	  "install state" routines for the given node type. This is analogous
 *	  to the execProdnode.c file.
 *
 * Owner mashah
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/execState.c,v 1.9 2004/03/05 21:10:15 mashah Exp $
 *
 *---------------------------------------------------------------------------------
 */

/*
 * INTERFACE ROUTINES
 *	  ExecGetStateInit		- get the node ready for extracting state.
 *	  ExecGetStateNext		- get the next BUFSIZE worth of data for forwarding.
 *	  ExecInstallStateInit	- get the node ready for installing state.
 *	  ExecInstallStateNext	- install the next BUFSIZE worth of data.
 *	  ExecInstallStateClose - finalize the installation.
 */

#include "postgres.h"
#include "executor/execState.h"
#include "executor/nodeHashGroup.h"
#include "executor/nodeFluxProd_FT.h"
#include "executor/nodeSegProd.h"
#include "executor/nodeDataGen.h"
#include "executor/nodeFluxCons_FT.h"

bool
ExecGetStateInit(Plan *node)
{

/* Lets see if this ## trick really works */
	
#define excase_gsinit(_type_) \
		case (T_##_type_): \
			return ExecGetStateInit##_type_((_type_ *) node);

	
	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_gsinit(HashGroup);
			
		excase_gsinit(FluxProd_FT);

		excase_gsinit(DataGen);

		excase_gsinit(FluxCons_FT);

		excase_gsinit(SegProd);

		default:
			elog(DEBUG1, "No ExecGetStateInit found for %d", nodeTag(node));
			return FALSE;
	}

}

bool
ExecGetStateNext(Plan *node, char *buf, int bsize)
{

#define excase_gsnext(_type_, buf, bsize) \
		case (T_##_type_): \
			return ExecGetStateNext##_type_((_type_ *) node, buf, bsize);

	if (node == NULL)
		return TRUE;
	
	switch (nodeTag(node))
	{
		excase_gsnext(HashGroup, buf, bsize);

		excase_gsnext(FluxProd_FT, buf, bsize);

		excase_gsnext(DataGen, buf, bsize);

		excase_gsnext(FluxCons_FT, buf, bsize);

		excase_gsnext(SegProd, buf, bsize);

		default:
			elog(DEBUG1, "No ExecGetStateNext found for %d", nodeTag(node));
			return FALSE;
	}

}

bool
ExecGetStateClose(Plan * node)
{

#define excase_gsclose(_type_) \
		case (T_##_type_): \
			return ExecGetStateClose##_type_((_type_ *) node);

	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_gsclose(HashGroup);

		excase_gsclose(FluxProd_FT);

		excase_gsclose(DataGen);

		excase_gsclose(FluxCons_FT);

		excase_gsclose(SegProd);

		default:
			elog(DEBUG1, "No ExecGetStateClose found for %d", nodeTag(node));
			return FALSE;
	}
	
}

	
bool
ExecInstallStateInit(Plan *node)
{

#define excase_isinit(_type_) \
		case (T_##_type_): \
			return ExecInstallStateInit##_type_((_type_ *) node);

	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_isinit(HashGroup);

		excase_isinit(FluxProd_FT);

		excase_isinit(DataGen);

		excase_isinit(FluxCons_FT);

		excase_isinit(SegProd);

		default:
			elog(DEBUG1, "No ExecInstallStateInit found for %d", nodeTag(node));
			return FALSE;
	}

}

bool
ExecInstallStateNext(Plan *node, char *buf, int bsize)
{

#define excase_isnext(_type_, buf, bsize) \
		case (T_##_type_): \
			return ExecInstallStateNext##_type_((_type_ *) node, buf, bsize);
	
	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_isnext(HashGroup, buf, bsize);

		excase_isnext(FluxProd_FT, buf, bsize);

		excase_isnext(DataGen, buf, bsize);

		excase_isnext(FluxCons_FT, buf, bsize);

		excase_isnext(SegProd, buf, bsize);

		default:
			elog(DEBUG1, "No ExecInstallStateNext found for %d", nodeTag(node));
			return FALSE;
	}

}

bool
ExecInstallStateClose(Plan *node)
{

#define excase_isclose(_type_) \
		case (T_##_type_): \
			return ExecInstallStateClose##_type_((_type_ *) node);

	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_isclose(HashGroup);

		excase_isclose(FluxProd_FT);

		excase_isclose(DataGen);

		excase_isclose(FluxCons_FT);

		excase_isclose(SegProd);

		default:
			elog(DEBUG1, "No ExecInstallStateClose found for %d", nodeTag(node));
			return FALSE;
	}

}

bool
ExecResetState(Plan *node)
{

#define excase_rs(_type_) \
		case (T_##_type_): \
			return ExecResetState##_type_((_type_ *) node);

	if (node == NULL)
		return TRUE;

	switch (nodeTag(node))
	{
		excase_rs(HashGroup);

		excase_rs(FluxProd_FT);

		excase_rs(DataGen);

		excase_rs(FluxCons_FT);

		excase_rs(SegProd);

		default:
			elog(DEBUG1, "No ExecResetState found for %d", nodeTag(node));
			return FALSE;
	}

}

void
ExecDump(Plan *node, FILE *fp)
{

#define excase_dump(_type_) \
		case (T_##_type_): \
			return ExecDump##_type_((_type_ *) node, fp);

	if (node == NULL)
		return;

	switch (nodeTag(node))
	{
		excase_dump(HashGroup);
		
		default:
			elog(DEBUG1, "No ExecDump found for %d", nodeTag(node));
			return;
	}

}
