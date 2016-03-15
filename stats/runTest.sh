#!/bin/bash

totalTime=0
counter=0 
for fileName in ./*.txt; do
    counter=$(($counter + 1))
    ./main $fileName "testOut$counter.out" $1 $2
    TIME=`grep TIME testOut | awk '{split($0, a, "="); print a[2]}'`
    echo $fileName "took " $TIME " seconds"
    totalTime=$(($totalTime + $TIME)) 
    cat testOut >> allResults
done
echo "Total time for all tests " $totalTime
echo "Average time" $(($totalTime / $counter))
