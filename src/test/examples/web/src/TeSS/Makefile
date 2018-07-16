# Which java compiler to use
#JAVAC = jikes
JAVAC = javac

# Subdirectories with java files:
JAVA_DIRS	:=   ./util/exceptions ./util .

# All the .java source files:
JAVA_SRC        := $(foreach dir, $(JAVA_DIRS), $(wildcard $(dir)/*.java))

# The main classes we should build:
JAVA_OBJS	:= $(JAVA_SRC:.java=.class)

%.class: %.java
	$(JAVAC) -g $<

all::	$(JAVA_OBJS)

clean:
	rm *.class

runurlt:
	java TeSS.NativeWrapper google.jsc
