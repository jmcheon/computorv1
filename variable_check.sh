#!/bin/bash

program="./computorv1"
make all
if [ $# -eq 0 ]; then
    equations=(
        "x=0"
        "1*x + y =0"
        "1*y + y =0"
        "1*x + y = 7*y^2"
        "1*y + y = 7*y^2"
        "-x^6 - 1*y + y = 7*y^2"
        "-y^6 - 1*y + y = 7*y^2"
        "x^2 - 2*x=0"
		)

    for equation in "${equations[@]}"; do
        echo "Running $program with equation: $equation"
        "$program" "$equation"
    done
else
    #echo "Arguments provided: $1"
    "$program" "$1"
fi

