# parent directory

BINDIR = 

# GLOBALS = -DDEBUG
GLOBALS = 

#CFLAGS = -Wall -O3 -I$(INCLUDEDIR) -L$(LIBDIR) $(SYS) $(GLOBALS)
CFLAGS = -Wall -g -lm -I$(INCLUDEDIR) -L$(LIBDIR) $(SYS) $(GLOBALS)
CC = gcc

MAIN_OBJS = loadcodes.o parselabel.o caldist.o gettrace.o tracelog.o geotrack_main.o hashcache.o dbg_lib.o

SERVER_OBJS = hashcache.o dbg_lib.o

all:	${MAIN_OBJS} Makefile 
	$(CC) $(CFLAGS) ${MAIN_OBJS} -o $(BINDIR)geotrack $(LDLIBS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(BINDIR)geoserver geoserver.c
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(BINDIR)hctest hctest.c
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(BINDIR)geoclient geoclient.c

client: $(MAIN_OBJS) Makefile
	gcc -g -o geoclient geoclient.c dbg_lib.o

server: $(MAIN_OBJS) Makefile
	gcc -g -o geoserver geoserver.c


geotrack: ${MAIN_OBJS} Makefile
	$(CC) $(CFLAGS) ${MAIN_OBJS} -o $(BINDIR)geotrack $(LDLIBS)

clean: 
	\rm -f *.o geotrack geoserver hctest

install: all 
	PGRSHOME=`pg_config --bindir`
	cp geoserver geoclient ${PGRSHOME}/bin
	cp -r codes ${PGRSHOME}/bin

veryclean: clean
	\rm -f $(BINDIR)/*

depend :
	sed -e '/^#~#~#/,$$d' < Makefile > Makefile.tmp
	echo "#~#~# Do not remove this line" >> Makefile.tmp
	gcc $(CFLAGS) -MM *.c >> Makefile.tmp
	mv -f Makefile.tmp Makefile

#~#~# Do not remove this line
loadcodes.o: loadcodes.c loadcodes.h
parselabel.o: parselabel.c parselabel.h loadcodes.h
caldist.o: caldist.c caldist.h loadcodes.h
gettrace.o: gettrace.c gettrace.h parselabel.h loadcodes.h caldist.h
tracelog.o: tracelog.c tracelog.h gettrace.h parselabel.h loadcodes.h caldist.h
geotrack_main.o: geotrack_main.c tracelog.h gettrace.h parselabel.h loadcodes.h caldist.h
hashcache.o: hashcache.c hashcache.h
dbg_lib.o: dbg_lib.c dbg_lib.h
