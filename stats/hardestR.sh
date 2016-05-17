#!bin/bash
timeout=300

outOfTime=0
> hardestRResults
for puzzleNumber in `seq 1 10`; do
    echo "15 3 5 53" > hardestR.gen
    ./main hardestR.gen ./rTest.out $timeout GEN FC MRV
    sleep 1
    TIME=`grep TIME "rTest.out" | awk '{split($0, a, "="); print a[2]}'`
    totalTime=$(($totalTime+$TIME))
    NODES=`grep COUNT_NODES "rTest.out" | awk '{split($0, a, "="); print a[2]}'`
    totalNodes=$(($totalNodes + $NODES))
    SOLUTION=`grep \SOLUTION= "rTest.out" | tr -d -c '0' | awk '{ print length; }'`
    if [ $TIME -eq 300 ]
    then
        out.txt > IwasTooHard$outOfTime.txt
        outOfTime=$(($outOfTime+1))
    fi
    if [ -z $SOLUTION ]
    then
        totalSolve=$(($totalSolve + 1))
    fi
    echo $NODES
    echo $TIME
    puzzleTimes+=($TIME)
done
avgTime=$(($totalTime / 10))
var=0
for entry in `seq 1 10`; do
    echo ${puzzleTimes[$entry]}
    midWay=$(( (${puzzleTimes[$entry]} - $avgTime) ** 2 ))
    var=$(($var + $midWay))
done
var=$(($var / 10))
echo "M: " ${mToTry[$counter]} >> hardestRResults
echo "AVG TIME: " $(($totalTime / 10)) >> hardestRResults
echo "AVG NODES: " $(($totalNodes / 10)) >> hardestRResults
echo $var | awk '{print "STD TIME: ", sqrt($0)}' >> hardestRResults
echo "Solved: " $totalSolve " of 10" >> hardestRResults
