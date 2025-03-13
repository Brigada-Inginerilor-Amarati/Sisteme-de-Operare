#
task(){
	# creeaza numere.txt daca nu exista
	if [ ! -f "$1" ]; then
		touch "$1"
	fi

	# adauga 5 numere random intre 1 si 100
	for i in {1..5}; do
		echo $((RANDOM % 100 + 1))
	done >> "$1"

	# citeste numerele din fisier si calculeaza suma lor
	local sum=0
	while read number
	do
		sum=$((sum + number))
	done < "$1"

	# afiseaza suma
	echo "Suma numerelor din fisier este: $sum"
}

task "numere.txt"
