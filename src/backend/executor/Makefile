#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for executor
#
# Portions Copyright (c) 2003, Regents of the University of California
#
# IDENTIFICATION
#    $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/Makefile,v 1.33 2005/12/14 20:49:22 phred Exp $
#
#-------------------------------------------------------------------------

subdir = src/backend/executor
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = execAmi.o execJunk.o execMain.o \
       execProcnode.o execQual.o execScan.o execTuples.o \
       execUtils.o functions.o instrument.o nodeAppend.o nodeAgg.o nodeHash.o \
       nodeHashjoin.o nodeIndexscan.o nodeMaterial.o nodeMergejoin.o \
       nodeNestloop.o nodeFunctionscan.o nodeResult.o nodeSeqscan.o \
       nodeSetOp.o nodeSort.o nodeUnique.o nodeLimit.o nodeGroup.o \
       nodeSubplan.o nodeSubqueryscan.o nodeTidscan.o spi.o \
       nodeSQEddy.o routingPolicy.o nodeIndexSteM.o nodeSteMHashjoin.o \
       nodeEddy.o nodeSteM.o nodeFilter.o intermediateTuple.o dummy.o \
       nodeHashGroup.o tsb.o nodeFSched.o nodeFluxCons.o nodeFluxProd.o \
	   nodeFluxCons_FT.o nodeSegProd.o nodeFluxEgress.o \
       nodeFluxAcceptor.o nodeFluxProd_FT.o prng.o aggHash.o execState.o \
       processNode.o nodeCQEddy.o nodeGSFilter.o nodeStrmScan.o nodeFSteM.o \
       nodeScanq.o nodeScanSock.o nodeScanModule.o nodeHashAgg.o \
       nodeDataGen.o nodeFluxIngress.o nodeSegCons.o nodeStateMover.o nodeFOutput.o \
       nodeFSplit.o nodeFFilter.o nodeFAgg.o nodeASteM.o nodeSymHashJoin.o nodeFEventAgg.o

all: SUBSYS.o

SUBSYS.o: $(OBJS)
	$(LD) $(LDREL) $(LDOUT) SUBSYS.o $(OBJS)

depend dep:
	$(CC) -MM $(CFLAGS) *.c >depend

clean: 
	rm -f SUBSYS.o $(OBJS) 

ifeq (depend,$(wildcard depend))
include depend
endif
