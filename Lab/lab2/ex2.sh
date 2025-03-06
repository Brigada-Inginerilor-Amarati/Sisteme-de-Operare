#!/bin/bash

if [ ! $# -eq 3 ]; then
	echo "Usage: $0 <number1> <number2> <number3>"
	exit 1
fi

max=$1

for i in "$@"
do
	if [ $i -gt $max ]; then
		max=$i
	fi
done

echo "Greatest number: $max"
