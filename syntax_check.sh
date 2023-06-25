#!/bin/bash

program="./computorv1"
make all
if [ $# -eq 0 ]; then
    equations=(
        "x=0"
        "1*x=0"
        "x^2=0"
        "2=y"
        "2=3*y"
        "2=y^6"
        "2*2=y^6"
		"-e=+e"
		)

    for equation in "${equations[@]}"; do
        echo "Running $program with equation: $equation"
        "$program" "$equation"
    done
else
    #echo "Arguments provided: $1"
    "$program" "$1"
fi

