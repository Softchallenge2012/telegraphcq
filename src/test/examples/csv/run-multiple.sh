for i in  `seq 1`
do
    seq 1000 | (
    while read i
    do
        #echo `expr $i + 500000`,$i
        #k=`expr $1 - 1`
        k=$1
        for j in `seq $k`
        do
            echo $i,$i
        done
    done
    ) | ./source-multiple.pl 127.0.0.1 5533 $1 `seq $1 | sed 's/\([0-9][0-9]*\)/csvwrapper\1,public.stream\1/'`
done
