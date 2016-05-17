#!/bin/bash
>allResults
totalTime=0
counter=0 
for fileName in $1/*.txt; do
    counter=$(($counter + 1))
    ./main $fileName testOut $2 $3
    TIME=`grep TIME testOut | awk '{split($0, a, "="); print a[2]}'`
    echo $fileName "took " $TIME " seconds"
    totalTime=$(($totalTime + $TIME)) 
    cat testOut>> allResults
done
echo "Total time for all tests " $totalTime
echo "Average time" $(($totalTime / $counter))
