# usage: set_mode.sh <directory> <mode>
# modes: r w x
# set all .txt files to <mode>

set_mode() {

	if [ "$#" -ne 2 ]; then
        echo "Usage: set_mode.sh <directory> <mode>"
        exit 1
    fi

    local directory="$1"
    local mode="$2"

    if [ ! -d "$directory" ]; then
        echo "Error: Directory '$directory' does not exist."
        exit 2
    fi

    if [ "$mode" != "r" ] && [ "$mode" != "w" ] && [ "$mode" != "x" ]; then
        echo "Error: Invalid mode '$mode'."
        exit 3
    fi

    for file in "$directory"/*.txt; do
        chmod "+$mode" "$file"
    done
}

set_mode "$@"
