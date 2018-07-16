/*
extern TupleTableSlot *ExecFSched(FSched * node);
extern bool ExecInitFSched(FSched * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFSched(FSched * node);
extern void ExecEndFSched(FSched * node);
extern void init_fsched_acceptfd(void);
extern TupleTableSlot *ExecHashGroup(HashGroup * node);
extern bool ExecInitHashGroup(HashGroup * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsHashGroup(HashGroup * node);
extern void ExecEndHashGroup(HashGroup * node);
extern bool ExecGetStateInitHashGroup(HashGroup *node);
extern bool ExecGetStateNextHashGroup(HashGroup *node, char *buf, int bsize);
extern bool ExecInstallStateInitHashGroup(HashGroup *node);
extern bool ExecInstallStateNextHashGroup(HashGroup *node, char *page, int bsize);
extern bool ExecInstallStateCloseHashGroup(HashGroup *node);
extern bool ExecResetStateHashGroup(HashGroup *node);
extern void ExecDumpHashGroup(HashGroup *node, FILE *fp);
extern TupleTableSlot *ExecScanQueue(ScanQueue * node);
extern bool ExecInitScanQueue(ScanQueue * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsScanQueue(ScanQueue * node);
extern void ExecEndScanQueue(ScanQueue * node);
extern TupleTableSlot *ExecStrmScan(StrmScan * node);
extern TupleTableSlot *ExecStrmScanBlocking(StrmScan * node);
extern bool ExecInitStrmScan(StrmScan * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsStrmScan(StrmScan * node);
extern void ExecEndStrmScan(StrmScan * node);
extern TupleTableSlot *ExecWindow(Window *node);
extern bool ExecInitWindow(Window *node, EState *estate, Plan *parent);
extern int	ExecCountSlotsWindow(Window *node);
extern void ExecEndWindow(Window *node);
extern void ExecReScanWindow(Window *node, ExprContext *exprCtxt, Plan *parent);
extern WindowBuffer *createWindowBuffer(WindowClause * wndClause, WindowState * wndState);
extern void freeTupleList(TupleList * p);
extern void freeWindowBuffer(WindowBuffer * wndBuf);
extern bool addTupleToWindowBuffer(WindowBuffer * wndBuf, HeapTuple tuple);
extern void advanceWindowBuffer(WindowBuffer * wndBuf);
extern HeapTuple fetchFromWindowBuffer(WindowBuffer * wndBuf);
*/

#include "postgres.h"
#include "nodes/plannodes.h"

#ifdef DISABLE_CQ_SUPPORT

bool		addTupleToWindowBuffer(WindowBuffer * wndBuf, HeapTuple tuple);
bool		ExecGetStateInitHashGroup(HashGroup * node);
bool		ExecGetStateNextHashGroup(HashGroup * node, char *buf, int bsize);
bool		ExecInitCQEddy(Eddy * node, EState *estate, Plan *parent);
bool		ExecInitFluxAcceptor(FluxAcceptor * node, EState *estate, Plan *parent);
bool		ExecInitFluxCons(FluxCons * node, EState *estate, Plan *parent);
bool		ExecInitFluxCons_FT(FluxCons_FT * node, EState *estate, Plan *parent);
bool		ExecInitFluxProd(FluxProd * node, EState *estate, Plan *parent);
bool		ExecInitFluxProd_FT(FluxProd_FT * node, EState *estate, Plan *parent);
bool		ExecInitFSched(FSched * node, EState *estate, Plan *parent);
bool		ExecInitHashGroup(HashGroup * node, EState *estate, Plan *parent);
bool		ExecInitScanQueue(ScanQueue * node, EState *estate, Plan *parent);
bool		ExecInitStrmScan(StrmScan * node, EState *estate, Plan *parent);
bool		ExecInitWindow(Window *node, EState *estate, Plan *parent);
bool		ExecInstallStateCloseHashGroup(HashGroup * node);
bool		ExecInstallStateInitHashGroup(HashGroup * node);
bool		ExecInstallStateNextHashGroup(HashGroup * node, char *page, int bsize);
bool		ExecResetStateHashGroup(HashGroup * node);
bool		ProcessAddQuery(Plan *node, int32 queryid, Plan *parent);
bool		ProcessEndFSteM(FSteM * node, Plan *parent);
bool		ProcessEndGSFilter(GSFilter * node, Plan *parent);
bool		ProcessEndNode(Plan *node, Plan *parent);
bool		ProcessInitFSteM(FSteM * node, EState *estate, Plan *parent);
bool		ProcessInitGSFilter(GSFilter * node, EState *estate, Plan *parent);
bool		ProcessInitNode(Plan *node, EState *estate, Plan *parent);
bool		ProcessRemoveQueryFSteM(FSteM * node, int32 queryid, Plan *parent);
bool		ProcessRemoveQueryGSF(GSFilter * gsfilter, int queryid, Plan *parent);
bool		ProcessRemoveQuery(Plan *node, int32 queryid, Plan *parent);
bool		verifyConstTree(ExprContext *econtext, ConstTree * ct, FunctionCachePtr fcn, Const *c, bool expectedRet);
int			ExecCountSlotsFluxAcceptor(FluxAcceptor * node);
int			ExecCountSlotsFluxCons(FluxCons * node);
int			ExecCountSlotsFluxCons_FT(FluxCons_FT * node);
int			ExecCountSlotsFluxProd(FluxProd * node);
int			ExecCountSlotsFluxProd_FT(FluxProd_FT * node);
int			ExecCountSlotsFSched(FSched * node);
int			ExecCountSlotsHashGroup(HashGroup * node);
int			ExecCountSlotsScanQueue(ScanQueue * node);
int			ExecCountSlotsStrmScan(StrmScan * node);
int			ExecCountSlotsWindow(Window *node);
int			write_b(int fd, char *buf, int size);
HeapTuple	EddyExecProject(Eddy * eddy, IntermediateHeapTuple iht, int32 queryid);
HeapTuple	fetchFromWindowBuffer(WindowBuffer * wndBuf);
PostJoinOperator *findPJOfromSources(Eddy * node, BitArray sources);
SteM	   *GetRepresentativeSteMFSteM(FSteM * node);
TupleTableSlot *ExecCQEddy(Eddy * node);
TupleTableSlot *ExecFluxAcceptor(FluxAcceptor * node);
TupleTableSlot *ExecFluxCons(FluxCons * node);
TupleTableSlot *ExecFluxCons_FT(FluxCons_FT * node);
TupleTableSlot *ExecFluxProd(FluxProd * node);
TupleTableSlot *ExecFluxProd_FT(FluxProd_FT * node);
TupleTableSlot *ExecFSched(FSched * node);
TupleTableSlot *ExecHashGroup(HashGroup * node);
TupleTableSlot *ExecScanQueue(ScanQueue * node);
TupleTableSlot *ExecStrmScanBlocking(StrmScan * node);
TupleTableSlot *ExecStrmScan(StrmScan * node);
TupleTableSlot *ExecWindow(Window *node);
void		ExecFluxCons_FTAddConn(FluxCons_FT * node, int sockfd, int node_id, int fprod_idx);
void		ExecReScanFluxCons_FT(FluxCons_FT * node, ExprContext *exprCtxt, Plan *parent);
void		AddWindowFSteM(FSteM * node, Const *window_size);
void		advanceWindowBuffer(WindowBuffer * wndBuf);
void		ClearWindowFSteM(FSteM * node);
void		EndFjord(Fjord * f);
void		ExecDumpHashGroup(HashGroup * node, FILE *fp);
void		ExecEndCQEddy(Eddy * node);
void		ExecEndFluxAcceptor(FluxAcceptor * node);
void		ExecEndFluxCons(FluxCons * node);
void		ExecEndFluxCons_FT(FluxCons_FT * node);
void		ExecEndFluxProd(FluxProd * node);
void		ExecEndFluxProd_FT(FluxProd_FT * node);
void		ExecEndFSched(FSched * node);
void		ExecEndHashGroup(HashGroup * node);
void		ExecEndScanQueue(ScanQueue * node);
void		ExecEndStrmScan(StrmScan * node);
void		ExecEndWindow(Window *node);
void		ExecFluxConsAddConn(FluxCons * node, int sockfd, int node_id, int fprod_idx);
void		ExecReScanWindow(Window *node, ExprContext *exprCtxt, Plan *parent);
void		freeTupleList(TupleList * p);
void		freeWindowBuffer(WindowBuffer * wndBuf);
void		init_fsched_acceptfd(void);
void		InitializeFjord(Fjord * f, EState *estate);
void		print_connections(FluxCons_FT * node);
/* void		print_conn_info(FluxConnInfo_FT * fcinfo); */
void		ProcessNextFSteM(FSteM * node, Plan *parent);
void		ProcessNextGSFilter(GSFilter * node, Plan *parent);
void		ProcessNextNode(Plan *node, Plan *parent);
void		RunCQEddy(void);
void		SetTimestampAccessorFSteM(FSteM * node, Var *ts_accessor);
void		wakeup_eddy(void);
WindowBuffer *createWindowBuffer(WindowClause * wndClause, WindowState * wndState);
TupleTableSlot *ExecSegCons(SegCons *node) ;
bool ExecInitSegCons(SegCons *node, EState *estate, Plan *parent) ;
int  ExecCountSlotsSegCons(SegCons *node) ;
void ExecEndSegCons(SegCons *node) ;
bool getResultFromConn(Plan *node, FluxConnInfo_FT *fcinfo, result *p_res, int conn_idx) ;

TupleTableSlot *ExecDataGen(DataGen *node) ;
bool ExecInitDataGen(DataGen *node, EState *estate, Plan *parent) ;
int	ExecCountSlotsDataGen(DataGen *node) ;
void ExecEndDataGen(DataGen *node) ;

TupleTableSlot *ExecFluxIngress(FluxIngress *node) ;
bool ExecInitFluxIngress(FluxIngress *node, EState *estate, Plan *parent) ;
int ExecCountSlotsFluxIngress(FluxIngress *node) ;
void ExecEndFluxIngress(FluxIngress *node) ;

TupleTableSlot *ExecStateMover(StateMover *node) ;
bool ExecInitStateMover(StateMover *node, EState *estate, Plan *parent) ;
int  ExecCountSlotsStateMover(StateMover *node) ;
void ExecEndStateMover(StateMover *node) ;
void ExecStateMoverAddConn(StateMover *node, int sockfd, int node_id, int sidx) ;

void set_failed_machine(FSched *node, int machine) ;

void ExecReScanStrmScan(StrmScan * node, ExprContext *exprCtxt, Plan *parent);
void FSteMPrintData_debug_with_level(Eddy *eddy, FSteM * node, int stem_no, int level);
bool strmscan_has_var_bindings(StrmScan * scan);
bool FjordAddQuery(Plan *p, int32 queryid, Plan *parent);
Oid getRealSourceOid(Plan *node);


bool		addTupleToWindowBuffer(WindowBuffer * wndBuf, HeapTuple tuple)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecGetStateInitHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecGetStateNextHashGroup(HashGroup * node, char *buf, int bsize)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitCQEddy(Eddy * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFluxAcceptor(FluxAcceptor * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFluxCons(FluxCons * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFluxCons_FT(FluxCons_FT * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFluxProd(FluxProd * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFluxProd_FT(FluxProd_FT * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitFSched(FSched * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitHashGroup(HashGroup * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitScanQueue(ScanQueue * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitStrmScan(StrmScan * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInitWindow(Window *node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInstallStateCloseHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInstallStateInitHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecInstallStateNextHashGroup(HashGroup * node, char *page, int bsize)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ExecResetStateHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessAddQuery(Plan *node, int32 queryid, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessEndFSteM(FSteM * node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessEndGSFilter(GSFilter * node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessEndNode(Plan *node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessInitFSteM(FSteM * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessInitGSFilter(GSFilter * node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessInitNode(Plan *node, EState *estate, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessRemoveQueryFSteM(FSteM * node, int32 queryid, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessRemoveQueryGSF(GSFilter * gsfilter, int queryid, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		ProcessRemoveQuery(Plan *node, int32 queryid, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
bool		verifyConstTree(ExprContext *econtext, ConstTree * ct, FunctionCachePtr fcn, Const *c, bool expectedRet)
{
	Assert(!"CQ Eddy not enabled");
	return FALSE;
}
int			ExecCountSlotsFluxAcceptor(FluxAcceptor * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsFluxCons(FluxCons * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsFluxCons_FT(FluxCons_FT * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsFluxProd(FluxProd * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsFluxProd_FT(FluxProd_FT * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsFSched(FSched * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsScanQueue(ScanQueue * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsStrmScan(StrmScan * node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			ExecCountSlotsWindow(Window *node)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
int			write_b(int fd, char *buf, int size)
{
	Assert(!"CQ Eddy not enabled");
	return -1;
}
HeapTuple	EddyExecProject(Eddy * eddy, IntermediateHeapTuple iht, int32 queryid)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
HeapTuple	fetchFromWindowBuffer(WindowBuffer * wndBuf)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
PostJoinOperator *findPJOfromSources(Eddy * node, BitArray sources)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
SteM	   *GetRepresentativeSteMFSteM(FSteM * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecCQEddy(Eddy * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFluxAcceptor(FluxAcceptor * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFluxCons(FluxCons * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFluxCons_FT(FluxCons_FT * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFluxProd(FluxProd * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFluxProd_FT(FluxProd_FT * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecFSched(FSched * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecScanQueue(ScanQueue * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecStrmScanBlocking(StrmScan * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecStrmScan(StrmScan * node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
TupleTableSlot *ExecWindow(Window *node)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}
void		ExecFluxCons_FTAddConn(FluxCons_FT * node, int sockfd, int node_id, int fprod_idx)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecReScanFluxCons_FT(FluxCons_FT * node, ExprContext *exprCtxt, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
}
void		AddWindowFSteM(FSteM * node, Const *window_size)
{
	Assert(!"CQ Eddy not enabled");
}
void		advanceWindowBuffer(WindowBuffer * wndBuf)
{
	Assert(!"CQ Eddy not enabled");
}
void		ClearWindowFSteM(FSteM * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		EndFjord(Fjord * f)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecDumpHashGroup(HashGroup * node, FILE *fp)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndCQEddy(Eddy * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFluxAcceptor(FluxAcceptor * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFluxCons(FluxCons * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFluxCons_FT(FluxCons_FT * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFluxProd(FluxProd * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFluxProd_FT(FluxProd_FT * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndFSched(FSched * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndHashGroup(HashGroup * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndScanQueue(ScanQueue * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndStrmScan(StrmScan * node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecEndWindow(Window *node)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecFluxConsAddConn(FluxCons * node, int sockfd, int node_id, int fprod_idx)
{
	Assert(!"CQ Eddy not enabled");
}
void		ExecReScanWindow(Window *node, ExprContext *exprCtxt, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
}
void		freeTupleList(TupleList * p)
{
	Assert(!"CQ Eddy not enabled");
}
void		freeWindowBuffer(WindowBuffer * wndBuf)
{
	Assert(!"CQ Eddy not enabled");
}
void		init_fsched_acceptfd(void)
{
	Assert(!"CQ Eddy not enabled");
}
void		InitializeFjord(Fjord * f, EState *estate)
{
	Assert(!"CQ Eddy not enabled");
}
void		print_connections(FluxCons_FT * node)
{
	Assert(!"CQ Eddy not enabled");
}
/* void		print_conn_info(FluxConnInfo_FT * fcinfo)
{
	Assert(!"CQ Eddy not enabled");
}
*/
void		ProcessNextFSteM(FSteM * node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
}
void		ProcessNextGSFilter(GSFilter * node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
}
void		ProcessNextNode(Plan *node, Plan *parent)
{
	Assert(!"CQ Eddy not enabled");
}
void		RunCQEddy()
{
	Assert(!"CQ Eddy not enabled");
}
void		SetTimestampAccessorFSteM(FSteM * node, Var *ts_accessor)
{
	Assert(!"CQ Eddy not enabled");
}
void		wakeup_eddy(void)
{
	Assert(!"CQ Eddy not enabled");
}
WindowBuffer *createWindowBuffer(WindowClause * wndClause, WindowState * wndState)
{
	Assert(!"CQ Eddy not enabled");
	return NULL;
}

TupleTableSlot *ExecSegCons(SegCons *node) { Assert(! "CQ Eddy not enabled"); return NULL; }
bool ExecInitSegCons(SegCons *node, EState *estate, Plan *parent) { Assert(! "CQ Eddy not enabled"); return 0; }
int  ExecCountSlotsSegCons(SegCons *node) { Assert(! "CQ Eddy not enabled"); return 0; }
void ExecEndSegCons(SegCons *node) { Assert(! "CQ Eddy not enabled"); }
bool getResultFromConn(Plan *node, FluxConnInfo_FT *fcinfo, result *p_res, int conn_idx) { Assert(! "CQ Eddy not enabled"); return 0; }

TupleTableSlot *ExecDataGen(DataGen *node) { Assert(! "CQ Eddy not enabled"); return NULL; }
bool ExecInitDataGen(DataGen *node, EState *estate, Plan *parent) { Assert(! "CQ Eddy not enabled"); return 0; }
int	ExecCountSlotsDataGen(DataGen *node) { Assert(! "CQ Eddy not enabled"); return 0; }
void ExecEndDataGen(DataGen *node) { Assert(! "CQ Eddy not enabled"); }

TupleTableSlot *ExecFluxIngress(FluxIngress *node) { Assert(! "CQ Eddy not enabled"); return NULL; }
bool ExecInitFluxIngress(FluxIngress *node, EState *estate, Plan *parent) { Assert(! "CQ Eddy not enabled"); return 0; }
int ExecCountSlotsFluxIngress(FluxIngress *node) { Assert(! "CQ Eddy not enabled"); return 0; }
void ExecEndFluxIngress(FluxIngress *node) { Assert(! "CQ Eddy not enabled"); }

TupleTableSlot *ExecStateMover(StateMover *node) { Assert(! "CQ Eddy not enabled"); return NULL; }
bool ExecInitStateMover(StateMover *node, EState *estate, Plan *parent) { Assert(! "CQ Eddy not enabled"); return 0; }
int  ExecCountSlotsStateMover(StateMover *node) { Assert(! "CQ Eddy not enabled"); return 0; }
void ExecEndStateMover(StateMover *node) { Assert(! "CQ Eddy not enabled"); }
void ExecStateMoverAddConn(StateMover *node, int sockfd, int node_id, int sidx) { Assert(! "CQ Eddy not enabled"); }

void set_failed_machine(FSched *node, int machine) { Assert(! "CQ Eddy not enabled"); }

void ExecReScanStrmScan(StrmScan * node, ExprContext *exprCtxt, Plan *parent) { Assert(! "CQ Eddy not enabled"); }
void FSteMPrintData_debug_with_level(Eddy *eddy, FSteM * node, int stem_no, int level) { Assert(! "CQ Eddy not enabled"); }
bool strmscan_has_var_bindings(StrmScan * scan) { Assert(! "CQ Eddy not enabled"); return 0; }
bool FjordAddQuery(Plan *p, int32 queryid, Plan *parent) { Assert(! "CQ Eddy not enabled"); return 0; }
Oid getRealSourceOid(Plan *node) { Assert(! "CQ Eddy not enabled"); return 0; }

#endif /* DISABLE_CQ_SUPPORT */
