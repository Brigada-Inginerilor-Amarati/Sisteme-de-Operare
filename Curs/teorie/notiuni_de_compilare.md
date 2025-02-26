În cadrul laboratorului de Sisteme de Operare se interzice folosirea mediilor integrate de programare precum Eclipse, Code::Blocks, etc. Astfel, în cadrul acestui laborator se va lucra cu un editor de text simplu, iar compilarea se va realiza în linie de comandă folosind gcc (GNU C Compiler).

În decursul acestui laborator se încurajează cât mai mult utilizarea sistemului în linie de comandă chiar dacă acesta dispune de un mediu grafic activat. Așadar se va descuraja deschiderea editorului de texte din meniurile interfeței grafice. Nu se vor impune la laborator editoarele de texte, studenții având libertatea să folosească editoarele preferate (dacă sunt instalate).

Este important să menționăm diferența dintre un editor de document și un editor de text. Editoarele de text permit editarea fișierelor la nivel de text și în urma salvării informațiile vor fi stocate tot în format text simplu. Editoarele de document însă, precum Microsoft Word și Libre Office Writer, oferă o serie de funcționalități adiționale precum verificarea automată a gramaticii, și adesea permit includerea unor elemente vizuale mai complexe: imagini, grafice, forme, etc. În urma salvării, în acest caz, rezultă un fișier cu format specific editorului.

Exemple de editoare de text:

Windows: Notepad, Notepad++
Linux: Emacs, Gedit, vi, vim, mcedit, kate, etc.
Pentru apelarea corectă a unui editor de text din linia de comandă în Linux se va folosi următoarea sintaxă:

<editor_de_texte> <fișier_text> &
Exemplu:

emacs fisier.c &
Semnul '&' (ampersand) are rolul de a "trece" editorul de text, invocat din terminal, în background (în fundal, rulează în paralel cu preluarea de comenzi noi). În această situație, după execuție, terminalul va rămâne "liber" și va mai putea fi folosit fără a fi nevoie să închidem editorul de text. În cazul în care nu se folosește semnul '&' editorul de text va bloca terminalul respectiv și acesta nu va mai putea fi folosit decât după închiderea editorului.

Atenție! Închiderea terminalului va duce la închiderea forțată a editorului, ceea ce poate duce la pierderea datelor (dacă nu s-a salvat fișierul). Această metodă se aplică doar atunci când se folosește un mediu grafic pentru editare (GUI); în lipsa acestuia, în Linux, nu se recomandă această utilizare. La laborator se va folosi mediul grafic pentru editare.

După ce fișierul C a fost realizat și salvat se trece la compilarea acestuia. Compilarea se recomandă să se realizeze astfel:

gcc -Wall -o program_executabil fisier1.c fisier2.c .....
Compilatorul gcc acceptă furnizarea unor opțiuni de compilare, apoi acceptă o listă de fișiere C pentru a fi compilate. Opțiunile furnizate în exemplul de mai sus sunt următoarele:

-Wall (warnings all) - în urma acestei opțiuni compilatorul nu va suprima nici un mesaj de tip warning, toate fiind afișate la ieșirea standard
-o program_executabil - în lipsa acestei opțiuni programul executabil rezultat în urma compilării va fi a.out . Se recomandă folosirea acestei opțiuni pentru a diferenția clar programul executabil
În urma compilării cu succes, programul executabil se va rula astfel:

./program_executabil
sau (dacă se invocă cu argumente în linia de comandă - acestea se dau cu "spațiu" și NU cu "virgulă" )

./program_executabil arg1 arg2 arg3 .....
Se observă folosirea semnelor "./" înaintea fișierului ce reprezintă programul executabil. Acestea sunt necesare pentru a comunica interpretorului de comenzi calea relativă a fișierului executabil. Lipsa acestora va face ca interpretorul de comenzi să caute program_executabil în calea generală de incluziuni de căi a sistemului (variabila de mediu $PATH) și dacă nu o găsește se va genera o eroare. Se poate furniza și cale absolută (exempu: /home/student/program_executabil)

NU se vor accepta la laborator programe ce NU compilează sau programe ce în urma compilării conțin warning-uri !!!!!!

La început de drum, pot apărea diferite erori în timpul execuției programelor scrise în cadrul laboratorului, în special din clasa celor generate de mecanismele de protecție ale memoriei. Cel mai des întâlnit este Segmentation Fault semnalizând o accesare ilegală a unei zone de memorie: scriere în zonă read-only (ex. const char \*), dereferențierea unui pointer nul (null pointer dereferencing), depășirea limitelor unui buffer (buffer overflow) sau a stivei (stack overflow), etc. În ultimele două cazuri, gcc poate semnaliza aceste erori și prin afișarea unui mesaj de forma "**_ stack smashing detected _**: x terminated".

Pentru a depista mai ușor sursa acestor erori se poate folosi un debugger din linie de comandă precum gdb. Vom ilustra printr-un exemplu simplu utilizarea debuggerului împreună cu câteva comenzi de bază.

Să considerăm următorul cod ce conține o problemă:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
.....
int main(void)
{
char tab[5];
.....
strcpy(tab, "un text mai lung decat ar fi cazul...");
printf("%s\n", tab);
.....
}
```

La compilare nu există warning-uri, dar la execuția programului se poate observa imediat mesajul specificat anterior. La compilare se recomandă includerea opțiunii -g pentru a include informații pentru debugging.

gcc -Wall -g -o test ./test.c
./test
**_ stack smashing detected _**: ./test terminated
Aborted (core dumped)
În cazul programelor mai complexe, găsirea erorii este mai dificilă, așa că vom recurge la folosirea programului gdb. Lansarea acestuia se face prin apelare din linie de comadă urmat de numele executabilului ca și prim argument:

gdb ./test
Debuggerul va afișa de fiecare dată un mesaj conținând diferite informații despre sistem, distribuție, drepturi, etc. Se poate observa imediat după, la începutul liniei, un text de forma "(gdb)" semnalizând că programul este pregătit pentru a primi comenzi. Pentru a rula programul nostru putem folosi comanda run sau r:

(gdb) r
Putem observa diferite informații despre evenimentele apărute în timpul execuției, în special problema generată:
Starting program: /home/petra/test
**_ stack smashing detected _**: /home/petra/test terminated

Program received signal SIGABRT, Aborted.
0x00007ffff7a42428 in \_\_GI_raise (sig=sig@entry=6)
at ../sysdeps/unix/sysv/linux/raise.c:54
54 ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
Acum putem trece la identificarea instrucțiunii care generează problema, prin interogarea locației:

(gdb) where
Se va afișa cascada apelurilor ce rezultă în final în eroare, de interes fiind liniile de forma:
#5 0x0000000000400614 in main() at test.c:11
11 printf("%s\n", tab);
Acest indiciu ne arată că există o problemă cu bufferul construit de noi și examinând linia imediat anterioară putem observa că am depășit limitele predefinite.

Pentru a ieși din program, tot ce trebuie să facem este să folosim comanda quit sau q:

(gdb) q
