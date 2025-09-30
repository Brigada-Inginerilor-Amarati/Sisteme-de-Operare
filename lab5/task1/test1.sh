check_args(){
	if [ "$#" -ne 4 ]; then
    echo "Usage: $0 -i <input_file> -o <output_file>"
    exit 1
    fi

	if [ "$1" != "-i" ] || [ "$3" != "-o" ]; then
	    echo "Invalid arguments"
	    exit 1
	fi

	if [ ! -f "$2" ]; then
	    echo "Input file not found"
	    exit 1
	fi
}

validate_mail_and_ip(){
	local input_file="$2"
	local output_file="$4"
	for line in "$input_file"; do
		if grep -E '^.*;.*$' "$line"; then
			echo "$line" >> "$output_file"
		fi
	done
}

check_args "$@"

validate_mail_and_ip $2 $4
