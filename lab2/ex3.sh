#!/bin/bash

if [ ! $# -eq 3 ]; then
	echo "Usage: $0 <operand1> <operation> <operand2>"
	exit 1
fi

operand1=$1
operation=$2
operand2=$3

case $operation in
	"+") result=$((operand1 + operand2));;
	"-") result=$((operand1 - operand2));;
	"*") result=$((operand1 * operand2));;
	"/") result=$((operand1 / operand2));;
	*)
		echo "Error: Invalid operation"
		exit 1;;
esac

echo "Result: $result"
