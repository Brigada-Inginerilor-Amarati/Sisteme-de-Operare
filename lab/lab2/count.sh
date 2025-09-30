set_count() {
	if [ "$#" -ne 2 ]; then
		echo "Usage: count.sh <directory> <output_file>"
		exit 1
	fi

	if [ ! -d "$1" ]; then
		echo "Error: Directory '$1' does not exist."
		exit 1
	fi

	cd "$1"
	local total=0

	# clear the output file
	echo > "../$2"

	for file in *.txt; do
		local words=$(wc -c < "$file")
		echo "$file: $words" >> "../$2"
		total=$((total + words))
	done
	echo "Total words: $total" >> "../$2"
}

set_count "$@"
