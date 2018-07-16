for i in 1
do
echo | seq 1000 | (
        while read i
        do
            echo `expr $i + 50000`,$i
            echo $i,$i
            echo $i,$i
            sleep 0.02
        done
        ) | ./source-three.pl 127.0.0.1 5533 csvwrapper1 csvwrapper3 csvwrapper2
echo | seq 1000 | (
        while read i
        do
            echo $i,$i
            echo `expr $i + 50000`,$i
            echo $i,$i
            sleep 0.02
        done
        ) | ./source-three.pl 127.0.0.1 5533 csvwrapper1 csvwrapper3 csvwrapper2
done
