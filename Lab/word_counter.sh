#!/bin/bash

# Check for argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

DIR="$1"

# Check if directory exists
if [ ! -d "$DIR" ]; then
    echo "Error: '$DIR' is not a valid directory"
    exit 1
fi

# Find all regular files and pass them to wc -w
TOTAL=$(find "$DIR" -type f -exec cat {} + | wc -w)

echo "Total word count in '$DIR': $TOTAL"
