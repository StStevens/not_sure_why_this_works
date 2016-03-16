#!bin/bash
mToTry=(4 8 12 16 17 18 19 20 21 22 24 28 32 36)
timeout=300

outOfTime=0
> hardestRResults
for counter in `seq 0 13`; do
    puzzleTimes=()
    totalTime=0
    totalNodes=0
    totalSolve=0
    for puzzleNumber in `seq 1 10`; do
        echo "9 3 3" ${mToTry[$counter]} > hardestR.gen
        ./main hardestR.gen rTest.out $timeout GEN FC MRV LCV > out.txt
        TIME=`grep TIME "rTest.out" | awk '{split($0, a, "="); print a[2]}'`
        totalTime=$(($totalTime+$TIME))
        NODES=`grep COUNT_NODES "rTest.out" | awk '{split($0, a, "="); print a[2]}'`
        totalNodes=$(($totalNodes + $NODES))
        SOLUTION=`grep \STATUS= "rTest.out" | tr -d -c '0' | awk '{ print length; }'`
        if [ $TIME -eq 300 ]
        then
            cat out.txt > IwasTooHard$outOfTime.txt
            outOfTime=$(($outOfTime+1))
        fi
        if [ $SOLUTION == "success" ]
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
done
