CC = gcc
CCFLAGS = -g
OBJECTS = tcqsanity.o

check: tcqsanity
	./tcqsanity 

tcqsanity: $(OBJECTS)
	gcc -o  tcqsanity $(OBJECTS) -lm

clean: 
	rm tcqsanity
	rm output/*
