.PHONY: sanity demosetup democlean
sanity:
	gcc -g -o tcqsanity tcqsanity.c

demosetup:
	mkdir -p scratch
	chmod u+x *.sh *.pl
	./createdb.sh 
	./refresh -f
democlean:
	./clean.sh
