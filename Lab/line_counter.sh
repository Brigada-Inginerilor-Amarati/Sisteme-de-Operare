#!/bin/bash

# Check if directory argument is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

DIR="$1"

# Check if the provided path is a valid directory
if [ ! -d "$DIR" ]; then
    echo "Error: '$DIR' is not a valid directory"
    exit 1
fi

# Count total lines in all regular files under the directory (recursively)
TOTAL_LINES=$(find "$DIR" -type f -exec cat {} + | wc -l)

echo "Total line count in '$DIR': $TOTAL_LINES"
