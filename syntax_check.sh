#!/bin/bash

script_name=$(basename "$0")
exercise=$(basename "$0" .sh)
program="./computorv1"

make all
if [ $# -eq 0 ]; then
	$program  \
	&& "$program" "x=0" \
	&& "$program" "1*x=0"\
	&& "$program" "x^2=0"\
	&& "$program" "2=y"\
	&& "$program" "2=3*y"\
	&& "$program" "2=y^6"\
	&& "$program" "2*2=y^6"\
	;
else
	#echo "Arguments provided: $1"
	"$program" $1
fi
