all:
	gcc -pg -c -I../../../src/include/ dummy_udfs.c  
	gcc -pg -shared -o dummy_udfs.so dummy_udfs.o
	/bin/cp dummy_udfs.so ../../../../lib/
