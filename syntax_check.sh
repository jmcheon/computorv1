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
		"5*x^0=5*x^0"
		"4*x^0=8*x^0"
		"5*x^0=4*x^0+7*x^1"
		"5*x^0+13*x^1 + 3 * x ^2 = 1*x^0 + 1*x^1"
		"6*x^0 + 11*x^1 + 5*x^2 = 1*x^0 + 1*x^1"
		"5*x^0 + 3*x^1 + 3*x^2 = 1*x^0 + 0*x^1"
		"5*x^3 + 3*x^1 + 3*x^2 = 1*x^0 + 0*x^1"
		)

    for equation in "${equations[@]}"; do
        echo "Running $program with equation: $equation"
        "$program" "$equation"
    done
else
    #echo "Arguments provided: $1"
    "$program" "$1"
fi

