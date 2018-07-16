#!/bin/bash

cd scratch

for i in www.calstatela.edu www.sjsu.edu www.ucsf.edu www.ucmerced.edu www.ucsc.edu www.irvine.edu www.indiana.edu www.iastate.edu www.minnesota.edu www.wisc.edu www.cs.wisc.edu www.ufl.edu www.usc.edu www.sonomastate.edu www.utexas.edu www.gatech.edu www.ncsu.edu www.purdue.edu www.ohio-state.edu www.eecs.umich.edu www.washington.edu www.ucla.edu www.ndsu.edu www.suny.edu www.syracuse.edu www.georgetown.edu www-usa.cricket.org www-uk.cricket.org www-rsa.cricket.org www-aus.cricket.org www-pak.cricket.org www-ind.cricket.org akamai.com cnn.com rediff.com mit.edu uiuc.edu cmu.edu www.ibm.com www.yahoo.com www.oracle.com www.inktomi.com www.cs.utexas.edu
#for i in akamai.com cnn.com rediff.com mit.edu uiuc.edu cmu.edu
do
    sleep 0.25;
    wget $i;
done
