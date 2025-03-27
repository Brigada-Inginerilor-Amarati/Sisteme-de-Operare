Sa se scrie un shell script care primeste ca argumente in linie de comanda 2 fisiere: unul de intrare si unul de iesire. Shell script-ul se va apela sub forma:

./script.sh -i <input_file> -o <output_file>
Fisierul de intrare este de urmatoarea forma: pe fiecare linie se gaseste cate o intrare formata dintr-o adresa de email si o adresa de ip despartite prin punct-si-virgula (;). Exemplu:

someone@yahoo.com;192.168.0.2
nobody@somedomain.ro;192.168.7.7
Shell script-ul trebuie sa parcurga acest fisier si, pentru fiecare linie, sa scrie in fisierul de iesire OK sau ERROR daca adresa de email, respectiv adresa de ip, indeplineste anumite reguli. In cazul in care adresa de email si adresa de ip sunt valide, pe linia respectiva se va scrie in fisierul de iesire OK. In caz contrar se va scrie ERROR. Exemplu:

OK
ERROR
Regulile pentru adresa de email sunt urmatoarele:

sa nu inceapa cu cifra, poate incepe doar cu litere mari si litere mici
partea de adresa inainte de @ sa contina doar litere mari, litere mici, cifre, cratima
partea de adresa dupa @ sa contina doar litere mari, litere mici si cifre, dar sa nu inceapa cu cifra.
adresa de email trebuie sa se termine cu .com sau .ro sau .eu
adresa de email nu trebuie sa contina mai multe caractere de tip @
Adresa de ip se considera ca este formata din 4 grupuri de cate maxim 3 cifre (si minim una) despartite prin punct. Niciun grup de cifre nu are voie sa depaseasca valoarea 255. Primul grup de cifre nu are voie sa fie 0 iar ultimul grup de cifre cu are voie sa fie 255.
Se vor folosi expresii regulate. Se recomanda utilizarea comenzii grep cu parametri.
