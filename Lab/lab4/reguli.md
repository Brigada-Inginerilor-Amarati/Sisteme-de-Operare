# Expresii regulate

```
^ -> inceputul liniei

$ -> sfârșitul liniei

. -> orice caracter (mai puțin caracterul newline, de obicei)

* -> zero sau mai multe caractere

+ -> unul sau mai multe caractere

? -> zero sau un caracter
```

{m,n} - între m și n caractere
[a b c] - seturi de caractere
[a-z] - interval de caractere
[^a-z] - seturi de caractere care nu conțin literele de la a la z
[^a-z0-9] - seturi de caractere care nu conțin literele de la a la z și cifrele de la 0 la 9
[^a-z0-9_] - seturi de caractere care nu conțin literele de la a la z, cifrele de la 0 la 9 și caracterul \_

{n} - repetă n ori
{n,} - repetă n sau mai multe ori
{,n} - repetă 0 sau n ori
{n,m} - repetă între n și m ori

## sed

sed 's/regex/replacement/flags'

Expresiile regulate sunt niște șiruri de caractere ce reprezintă șabloane sau tipare (pattern în limba engleză). Ele se construiesc pe baza unei gramatici, la fel ca și un limbaj de programare. Aceste șabloane sunt folosite pentru "recunoașterea" și manipularea unor șiruri de caractere. Analog cu expresiile aritmetice, o expresie regulată este construită prin combinarea unor expresii mai mici cu ajutorul unor operatori.

O expresie regulată are 3 tipuri de componente principale:

ancore (anchors) - folosite pentru a preciza poziționarea tiparelor relativ la textul analizat
seturi de caractere (character sets) - simboluri ce se potrivesc cu una sau mai multe caractere din text
modificatori (modifiers) - permit introducerea de repetiții în tipare

Ancore
Majoritatea comenzilor de procesare de text lucrează la nivelul liniilor. Prin urmare, metode de a referi începutul, respectiv finalul de linie devin utile în cazul expresiilor regulate. Pentru a căuta un anumit tipar la începutul liniei, se poate folosi caracterul '^'. Pentru a căuta anumite informații la finalul liniei, ne putem folosi de simbolul '$'.

Exemplul 1:

^turing$
se va potrivi doar cu șirul "turing" (nu și cu "featuring" sau "turing ").
Exemplul 2:

^joaca
se va potrivi cu șirul "joaca de copil", dar nu și cu "se joaca" sau "jocuri".

Seturi de caractere
Punctul (.) se potrivește cu orice caracter, unul singur (mai puțin caracterul newline, de obicei). Să începem cu un exemplu simplu:

a.z
Această expresie regulată se potrivește cu orice șir de caractere ce conține literele 'a' și 'z' între care se găsește orice caracter - dar unul singur (cu excepția caracterului newline, de obicei), cum ar fi: "axz", "aaz", "barza", dar nu "abcz".
Cele mai simple expresii regulate sunt cele care "se potrivesc" cu un singur caracter: majoritatea caracterelor (toate literele și cifrele) se potrivesc cu ele însele. Alte caractere însă au semnificație specială, și dacă dorim ca expresia regulată să se potrivească cu acel caracter, trebuie să îl cităm (quote în limba engleză). Aceasta se poate realiza prin plasarea unui backslash ('\') în fața caracterului respectiv. Expresiile regulate mai complexe se vor forma fie prin concatenare (scriere una după alta), fie cu ajutorul operatorilor ce vor fi descriși mai jos.

Atenție! Prin concatenarea a două expresii regulate rezultă o expresie regulată ce se va potrivi cu șiruri de caractere ce conțin două subșiruri adiacente ce se vor potrivi cu prima respectiv a doua expresie regulată.

O altă construcție care potrivește un singur caracter este o listă de caractere închise între paranteze drepte [ ... ]. Această expresie se va potrivi cu oricare din caracterele din listă. Astfel, expresia regulată:

compl[ei]ment
se va potrivi cu oricare din șirurile "complement", "compliment" sau "mulțumesc pentru complimentul dumneavoastră". Dacă o construcție cu paranteze drepte începe cu un '^', atunci ea se va potrivi cu orice caracter ce nu este între paranteze. De exemplu:

3[^6890]
reprezintă o expresie regulată ce se potrivește cu orice șir ce conține cifra 3 și nu conține pe poziția următoare una din cifrele 6, 8, 9 sau 0 (atenție: dacă în șirul căruia i se aplică expresia regulată nu conține după 3 nici un alt caracter, expresia nu se va potrivi!). De asemenea se pot specifica intervale întregi (considerând ordinea ASCII a caracterelor) cu ajutorul cratimei '-'. De exemplu:

[A-Za-z]
reprezintă orice literă, mică sau mare.

Atenție! Caracterele care nu se potrivesc cu ele însele și de care aminteam mai sus sunt metacaractere și operatori. O parte dintre ele, printre cele mai des utilizate și implementate în diversele variante de expresii regulate le vom descrie mai jos, alături de alte construcții.

Modificatori
Sunt definiți o serie de operatori pentru a specifica repetițiile. Ei se aplică în dreapta unei expresii regulate, făcând-o să se potrivească repetitiv:

Operator Modificare adusă

- Potrivește de 0 sau mai multe ori

* Potrivește de 1 sau mai multe ori
  ? Potrivește de 0 sau 1 ori

Parantezele rotunde ( ... ) se folosesc pentru a grupa expresiile regulate. Astfel, dacă scriem:

ab\*
aceasta semnifică un 'a' urmat de oricâte 'b'-uri (inclusiv nici unul); dar daca scriem

(ab)_
operatorul _ se aplică grupului, semnificația fiind 0 sau mai multe repetiții ale șirului de caractere "ab".

Acoladele (pentru unele versiuni de interpretoare trebuie să fie precedate de backslash) { ..., ... } ne permit să specificăm un număr minim, respectiv maxim de repetiții pentru un tipar. De exemplu:

[a-zA-Z]{4,8}
tiparul de mai sus va verifica dacă există cel puțin 4 litere mici sau mari, dar maxim 8 astfel de caractere. Din tipar poate lipsi limita minimă sau cea maximă; nu este obligatoriu să se specifice ambele.

Un alt operator util este '|', operatorul de alternare. Rezultatul lui este că se va potrivi fie expresia regulata din stânga, fie cea din dreapta:

ion (pozitiv|negativ)
se va potrivi fie cu "ion pozitiv" fie cu "ion negativ".

Pentru mai multe detalii, consultați paginile de manual grep(1) și perlre(1). Ultima prezintă expresiile regulate implementate în limbajul perl, și nu vor fi întotdeauna compatibile cu comenzi precum grep și sed. În schimb găsiți acolo o descriere mai amănunțită. Expresiile regulate pot fi folosite și din limbaje de programare precum C, vedeți regex(3) și regex(7).

Filtre
Filtrele sunt comenzi care citesc rând cu rând fișierul standard de intrare și afișează la ieșirea standard integral sau doar în parte rândurile citite, modificate sau nu, în funcție de semantica lor. Operația continuă până la întâlnirea marcajului de sfârșit de fișier. Aceste comenzi se înlănțuie des cu ajutorul pipe-urilor, pentru a le conjuga efectul. În sistemele UNIX există o serie de comenzi care se comportă ca filtre, multe dintre ele făcând parte din standardul POSIX. Majoritatea filtrelor pot însă citi date și din fișiere specificate în linia de comandă.

În continuare vom da o scurtă descriere pentru câteva filtre mai des folosite:

cat
scrie fiecare linie de la intrarea standard (sau din fișiere ale căror nume sunt date ca argumente) la ieșirea standard, fără modificări.
head
scrie primele maxim 10 linii (10 este implicit; se poate specifica numărul de linii cu argumentul -n, sau se poate preciza un număr de octeți cu argumentul -c) de la intrare către ieșirea standard.
tail
similară cu comanda head, scrie ultimele 10 linii.
Observație: Această comandă este obligată să citească întreg fișierul înainte de a scrie ceva la ieșire, astfel fiind un impediment în pipeline-izare. Se recomandă să fie folosită ultima, dacă este posibil, într-un lanț de comenzi.
sort
afișează liniile citite în mod ordonat lexicografic, implicit ordinea fiind crescătoare. Comanda are o serie de argumente utile. Citiți pagina de manual sort(1)!
uniq
elimină liniile succesive identice dintre cele de la intrarea standard. Atenție: două linii identice, dar care nu sunt citite una după alta nu vor fi depistate!
cut
tipărește porțiuni din liniile citite. Aceste secțiuni pot fi intervale de octeți, caractere sau câmpuri, în funcție de unul din argumentele -b, -c respectiv -f. În ultimul caz se vor tipări acele câmpuri delimitate de un caracter (implicit TAB, se poate preciza cu argumentul -d) care sunt specificate folosind argumentul -f în forma: lista[,lista].... O listă poate fi un simplu număr reprezentând câmpul dorit, sau poate fi de forma N-M, unde N și M sunt numere reprezentând primul, respectiv ultimul câmp ce trebuie afișat. Sau N sau M poate lipsi, în locul lor subînțelegându-se primul, respectiv ultimul câmp din linie. Aceeași notație folosită cu -b sau -c seminifică intervalul de octeți sau caractere ce se vor afișa. Exemplu:
ls -l | cut -f 1 -d ' '
va tipari doar lista de permisiuni a fișierelor din directorul curent.
tr
traduce (translate) sau șterge caractere. tr implicit traduce, caz în care trebuie date ca argumente două șiruri de caractere reprezentând două seturi. Caracterele din primul set vor fi traduse în caracterele din al doilea. Dacă numărul de caractere din seturile date nu este același, caracterele excedentare dintr-al doilea se ignoră dacă acesta e mai lung, sau se repetă ultimul caracter din al doilea set (dacă acesta e mai scurt) până la lungimea primului set. În cazul argumentului -d se dă un singur set de caractere, care vor fi eliminate la scrierea la ieșirea standard. Argumentul -s (squeeze) realizează "contracția" caracterelor din setul dat ca parametru: în cazul în care la intrare filtrul citește două sau mai multe caractere identice, din set, va fi tipărit la ieșire doar unul singur. Exemplu:
ps -x | tr -s ' ' | cut -f 2,6 -d ' '
Comanda ps -x afișează lista proceselor utilizatorului care o invocă. Cu ajutorul lui tr -s ' ' se șterg spațiile dintre coloanele afișate, iar cut -f 2,6 -d ' ' face să apară la ieșire doar coloana corespunzătoare identificatorilor de proces (coloana 2) și cea a numelui comenzii corespunzătoare procesului (coloana 6).
grep
este un filtru mai complex. Are cel puțin un parametru, care va fi interpretat ca expresie regulată. Liniile citite de la intrarea standard (sau din fișierele date ca parametri) care se potrivesc cu expresia regulată dată vor fi afișate. Există o serie de opțiuni care modifică modul de afișare. O parte le gaăsiți în lista de mai jos:
-i face comanda insensibila la diferențele dintre literele mari și cele mici
-v tipărește liniile cu care tiparul dat nu se potrivește
-n tipărește numărul liniei urmat de caracterul : urmat de linia în sine
-E forțează folosirea expresiilor regulate extinse (în mod normal se folosesc niște expresii regulate cu sintaxa foarte simplă); există comanda egrep care e echivalentă cu grep -E
-x tipărește doar liniile pentru care tiparul dat se potrivește cu întreaga linie, nu cu un subșir din aceasta
-c tipărește doar numărul de linii ce s-ar fi afișat în mod normal
Exemplu:
ps -x | grep lab9
Comenzile afișeaza liniile generate de ps corespunzătoare proceselor ce conțin în numele lor (sau în parametri) șirul "lab9".
tee
scrie la ieșirea standard ceea ce citește de la intrarea standard, în plus scrie și în fișierele ale căror nume sunt date ca parametri.
wc
afișează numărul de caractere (parametrul -m), de octeți (-c), de linii (-l) sau de cuvinte (-w), sau lungimea celei mai lungi linii întâlnite (parametrul -L). De exemplu:
cat /etc/passwd | wc -l
va afișa numărul de intrări (linii) din fișierul de parole.
xargs
citește argumente de la intrarea standard, separate prin spații sau caractere newline și execută comanda dată ca parametru (sau echo implicit) cu o listă de argumente specificate ca parametri urmate de argumentele citite de la intrare. Liniile goale vor fi ignorate. Parametrul -i șir va modifica comportamentul lui xargs astfel încât comanda dată se va executa cu argumentele precizate ca parametri în care se înlocuiește șirul șir cu argumentele citite de la intrarea standard. Exemplu:
echo "1 2 3" | xargs mkdir -v
va lansa de trei ori comanda mkdir, cu parametrii -v și câte una din cifrele 1, 2 și 3.

Comanda sed
Comanda sed derivă din editorul ed care este orientat pe linie (spre deosebire de chiar cele mai simple editoare actuale care sunt orientate pe ecran). Spre deosebire de acesta din urmă, sed nu este interactiv, ci aplică tuturor sau unui grup de linii din fișierul prelucrat (care poate fi intrarea standard dacă nu se specifică un nume de fișier ca parametru) o anumită comandă. Sintaxa (simplificată) este:

sed [-n] [-e script] [-f script-file] [script-if-no-other-script] [file...]
Optiunile -e (după care urmează o comandă) și -f (dupa care urmează un nume de fișier care conține câte o comandă pe fiecare rând) adaugă comenzi în lista celor ce vor fi aplicate asupra liniilor de text. Dacă nici -e și nici -f nu sunt găsite, primul parametru care nu este opțiune (nu începe cu cratima) este considerat comandă. Dacă după ce s-au prelucrat parametrii așa cum am descris mai rămân și alți parametri în linia de comandă, aceștia se vor considera ca nume de fișiere care trebuie procesate (în care caz sed nu se mai comportă ca filtru).

Sintaxa comenzilor sed este destul de complexă. Descrierea completă o găsiți în pagina texinfo a comenzii sed (în pagina de manual descrierea nu este foarte clară) pe care o puteți citi cu comanda info sed. În general o comandă sed este compusă dintr-o adresă sau interval de adrese de linii, urmat de un caracter ce reprezintă o acțiune de efectuat, urmat eventual de un șir de caractere a cărui semnificație depinde de acțiune.

Intervalele de adrese se specifică sub forma a doua adrese despărțite prin virgulă. O adresă poate fi un număr și atunci reprezintă numărul liniei, sau o expresie regulată încadrată de caractere '/', caz în care reprezintă prima linie (începând cu linia curentă) ce corespunde (se potrivește) expresiei regulate. O adresă mai poate fi și caracterul $, care semnifică "ultima linie a ultimului fișier de intrare".

Acțiunile corespunzătoare comenzilor nu se vor aplica, în cazul în care avem adrese, decât liniilor corespunzătoare adresei, respectiv liniilor cuprinse în intervalul dat. Cea mai des folosită acțiune (sau comanda) este cea de substituție. Ea are forma:

s/regexp/replacement/flags
unde regexp este o expresie regulată, replacement este un șir de caractere cu care se va înlocui acea parte a liniei prelucrate care corespunde expresiei regulate, iar flags este o listă de zero sau mai multe caractere dintre următoarele:
g - înlocuiește toate porțiunile care se potrivesc cu regexp, nu doar prima astfel de porțiune.
p - dacă substituția a fost făcută, tipărește linia corespunzător modificată (utilă cu argumentul -n).
numărul N - înlocuieste doar a N-a potrivire a expresiei regulate.
De exemplu:
sed 's/abc/def/g' <fisier.intrare >fisier.iesire
va prelua informațiile din fișierul fisier.intrare, va înlocui fiecare apariție a secvenței de caractere "abc", cu "def", iar la final va scrie rezultatele în fișierul fisier.iesire.

Atenție! Forwardslash-ul '/' are o semnificație specială, prin urmare dacă trebuie introdus în construcția lui sed avem 2 alternative: îl precedăm cu un backslash sau recurgem la o a doua formă a lui sed pentru subtituții. De exemplu, dacă dorim să filtrăm după /usr/local pe o listă de path-uri și să înlocuim prima apariție cu /bin putem folosi:

sed 's/\/usr\/local/\/bin/' <fisier.intrare >fisier.iesire
sau apelăm la a doua formă, având ca și separator '|':

sed 's|/usr/local|/bin|' <fisier.intrare >fisier.iesire

Simbolul '&' ne permite să referim porțiunea de text ce s-a potrivit cu expresia regulată din câmpul 2. De exemplu, dacă dorim să găsim un prenume pe o linie și să îi adăugam un ! la final, putem să folosim:

sed 's/[A-Z][a-z]\*/&!/' <fisier.intrare >fisier.iesire

Atenție! Pentru a putea folosi operatorii de regex din setul extins, este nevoie să specificăm opțiunea -r imediat după cuvântul cheie sed.

Dacă dorim să ținem minte mai multe porțiuni de text ce se potrivesc cu anumite tipare, putem include tiparele între ș𝑖
în câmpul 2, și să le referim în câmpul 3 folosind \1, \2, \3, etc. în ordinea în care apar. De exemplu, dacă dorim să inversăm 2 cuvinte, putem folosi:

echo "lorem ipsum" | sed 's/[𝑎−𝑧]∗
[𝑎−𝑧]∗
/\2 \1/'
Sau dacă dorim să păstrăm ultimul cuvânt dintr-o propoziție, putem folosi:

echo "Lorem ipsum dolor sit amet." | sed 's/[A-Za-z ]\* [𝑎−𝑧]∗
\./\1/'
