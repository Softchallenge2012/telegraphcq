CLASSPATH = ./postgresql.jar:./plot.jar:.
#CLASSPATH = ../pg73jdbc2.jar:./plot.jar:.

SRC = 	CQPlotter/BarChart.java \
	CQPlotter/CQPlotter.java \
	CQPlotter/CQVisualizer.java

CLASSES = $(SRC:.java=.class)


all: $(SRC) $(CLASSES)

%.class: %.java
	javac -classpath $(CLASSPATH) $<


run: all
	java -Xms50000000 -Xmx90000000 -Dtcq.Host=jdbc:postgresql://${TCQHOST}:${TCQPORT}/sample -Dtcq.User=${TCQUSER} -classpath $(CLASSPATH) CQPlotter.CQVisualizer

lottery: all
	java -classpath $(CLASSPATH) CQPlotter.CQVisualizer lottery.config

clean:
	rm $(CLASSES)
