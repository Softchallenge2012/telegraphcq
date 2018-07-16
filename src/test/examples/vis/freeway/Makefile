#CLASSPATH = ./postgresql.jar:./plot.jar:mapper:.
CLASSPATH = ../pg73jdbc2.jar:./plot.jar:mapper:.

SRC = 	edu/gui/Main.java

CLASSES = $(SRC:.java=.class)

ifndef TCQHOST
	TCQHOST=datafix.cs.berkeley.edu
endif
ifndef TCQPORT
	TCQPORT=8765
endif
ifndef TCQUSER
	TCQUSER=tcq
endif


all: $(SRC) $(CLASSES)

%.class: %.java
	javac -classpath $(CLASSPATH) $<


runtraffic: all

	java   -Dtcq.expire=true -Dtcq.dbUser=${TCQUSER} -Dtcq.telehost=${TCQHOST} -Dtcq.dbPort=${TCQPORT} -Dtcq.dbUrl=jdbc:postgresql://${TCQHOST}:${TCQPORT}/sample -Dtcq.mappermode=INCIDENTS  -D -Dtcq.windowsize=${WINDOWSIZE} -classpath $(CLASSPATH) mapper.Mapper
rungeocode: all
	java   -Dtcq.expire=true -Dtcq.dbUser=${TCQUSER} -Dtcq.telehost=${TCQHOST} -Dtcq.dbPort=${TCQPORT} -Dtcq.dbUrl=jdbc:postgresql://${TCQHOST}:${TCQPORT}/sample -Dtcq.mappermode=GEOCODE  -D -Dtcq.windowsize=${WINDOWSIZE} -classpath $(CLASSPATH) mapper.Mapper

clean:
	find . -name "*.class" -print | xargs rm
#	rm $(CLASSES)
