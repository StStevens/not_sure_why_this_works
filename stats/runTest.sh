#!/bin/bash

for fileName in ./*.txt; do
    ./main $fileName testOut $1 $2
    cat testOut >> allResults
done
