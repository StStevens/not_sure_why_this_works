#!bin/bash
timeout=1200

for counter in `seq 1 5`; do
    puzzleTimes=()
    ./main PH$counter.txt PH$counter$1.out $timeout $2
    TIME=`grep TIME "PH$counter$1.out" | awk '{split($0, a, "="); print a[2]}'`
    NODES=`grep COUNT_NODES "PH$counter$1.out" | awk '{split($0, a, "="); print a[2]}'`
     
    echo "TIME: " $TIME
    echo "NODES: " $NODES
done
