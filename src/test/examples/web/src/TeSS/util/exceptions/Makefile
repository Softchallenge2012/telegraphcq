include ../../Makefile.inc

# Subdirectories with java files:
JAVA_DIRS	:= .

# All the .java source files:
JAVA_SRC	= FatalTelegraphException.java \
			GeneralTelegraphException.java \
			ObjectPoolClassException.java \
			OutOfSpaceException.java \
			QueryInitializationException.java \
			QueryProcessingException.java

# The main classes we should build:
JAVA_OBJS	:= $(JAVA_SRC:.java=.class)

%.class: %.java
	$(JAVAC) -g $<

all:	$(JAVA_OBJS)

clean:
	rm *.class