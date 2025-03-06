#!/bin/bash

# fisier, citim nume de la tastatura (read)
# daca fisierul exista, sortam continutul, daca nu, afisam mesaj de eroare

echo "introduceti numele fisierului:"

read file_name

if [ ! -f $file_name ]; then
	echo "Fisierul $file_name nu exista."
elif [ ! -r "$file_name" ]; then
	echo "Nu aveti drepturi de scriere pe fisierul $file_name"
else
	sort "$file_name"
fi
