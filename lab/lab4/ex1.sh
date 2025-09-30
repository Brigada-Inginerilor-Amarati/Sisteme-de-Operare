# logs.txt -> toate liniile de forma YYYY-MM-DD
# datele diferite de 00
select_dates(){
	if [ $# -ne 2 ]; then
		echo "Usage: $0 <input_file> <output_file>"
		exit 1
	fi

	grep -E '^[1-2][0-9]{3}-[0-9]{2}-[0-9]{2}$' "$1" > "$2"
}

select_dates logs.txt output.txt
cat output.txt
