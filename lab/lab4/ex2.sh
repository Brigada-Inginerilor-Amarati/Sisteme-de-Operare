# fisier error.txt
# eliminare linii goale cu sed
# sed 's/regex/replacement/flags'
sed -i '' '/^$/d' error.txt
