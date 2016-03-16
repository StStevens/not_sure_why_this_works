#!bin/bash
timeout=1200

for counter in `seq 1 5`; do
    puzzleTimes=()
    ./main PH$counter.txt PH$counter.out $timeout $1 
    TIME=`grep TIME "PH$counter.out" | awk '{split($0, a, "="); print a[2]}'`
done
