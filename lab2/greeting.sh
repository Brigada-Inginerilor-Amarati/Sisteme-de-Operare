echo "Salut, $(whoami)!"

if [ $(date +%H) -ge 6 ] && [ $(date +%H) -lt 12 ]; then
	echo "Buna dimineata!"
elif [ $(date +%H) -ge 12 ] && [ $(date +%H) -lt 18 ]; then
	echo "Buna ziua!"
elif [ $(date +%H) -ge 18 ] && [ $(date +%H) -lt 22 ]; then
	echo "Buna seara!"
else
	echo "Noapte buna!"
fi
