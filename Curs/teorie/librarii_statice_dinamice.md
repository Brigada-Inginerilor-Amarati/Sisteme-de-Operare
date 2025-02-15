Biblioteci statice și dinamice în Linux
Cerințe pentru finalizare
Biblioteci C
O bibliotecă C, în general, se poate spune că reprezintă o colecție de funcții compilate. Poate fi văzută și ca un program executabil (compilat) fără existența funcției main(). Așadar, o bibliotecă reprezintă un cod deja compilat ce poate fi inclus în programe de către dezvoltatorul de aplicații. Pentru a putea apela funcțiile din interiorul unei biblioteci este absolut necesar ca aceasta să fie însoțită de un fișier header care să exporte elementele bibliotecii (fie ele funcții sau variabile globale). Indiferent de sistemul de operare, bibliotecile pot fi statice sau dinamice. Și în Linux putem identifica aceste două tipuri de biblioteci.

Biblioteci statice
O bibliotecă statică este de fapt o arhivă (creată de obicei cu arhivatorul ar) ce include o colecție de fișiere obiect ce conțin la rândul lor funcții. Pe lângă fișierele obiect, o bibliotecă statică mai conține și un "cuprins" în care sunt enumerate funcțiile și adresele lor pentru a putea fi folosite. Funcțiile din bibliotecile statice sunt incluse în programul principal prin link-editare statică ce produce un fișier executabil. Fișierul executabil va încorpora codul bibliotecii statice. Avantajul în această situație este că programul devine de sine stătător conținând în fișierul executabil tot ce are nevoie ca să ruleze. Dezavantajele ar fi mai multe: dacă una dintre bibliotecile componente trebuie să se modifice, atunci programul ce le folosește trebuie recompilat; fișierul executabil al programului devine destul de mare ținând cont de faptul că încorporează codul bibliotecilor folosite; poate să scadă mult gradul de portabilitate al programelor.

Este necesar să se menționeze faptul că în Linux există convenția ca orice bibliotecă să aibă numele precedat de prefixul lib. Așadar, dacă se dorește realizarea unei biblioteci numite test, numele fișierul ce reprezintă biblioteca trebuie să fie libtest.

Pentru exemplificare vom considera o bibliotecă, cu numele test, formată inițial dintr-un fișier de cod (.c) și un fisier header (.h) precum și un fișier de cod (.c) ce va reprezenta programul principal.

Fișierul test.h:

```c
#ifndef **TEST_H
#define **TEST_H

void PrintTest(void);

#endif
Fișierul test.c

#include "test.h"
#include <stdio.h>

void PrintTest(void)
{
printf ("A print from the test library\n");
}
```

Pentru a compila biblioteca cu codul definit în test.c și cu declarațiile necesare exportate în test.h, se poate apela compilatorul astfel:

gcc -Wall -c -o libtest.o test.c
Prin acest apel al compilatorului se compilează test.c și se creează fișierul obiect libtest.o. Pentru a se crea biblioteca statică este necesar să se apeleze arhivatorul ar astfel:

ar rcs libtest.a libtest.o
Prin acest apel s-a creat biblioteca statică libtest.a. În continuare vom realiza un program principal executabil ce va apela funcția din această bibliotecă. Pentru aceasta avem nevoie de fișierul de bibliotecă static libtest.a precum și de fișierul header test.h pentru a putea scrie programul principal. Se menționează faptul că, în general, o bibliotecă se poate distribui prin punerea la dispoziție a fișierului bibliotecii compilate și a fișierului header aferent. Programul principal este reprezentat de fișierul main.c următor:

```c
#include "test.h"
#include <stdio.h>

int main(void)
{
printf ("A print from the main program before calling the test library function\n");
PrintTest();
return 0;
}
```

Pentru a obține programul principal vom folosi o compilare pe pași pentru a putea evidenția procedeul. În prima fază vom compila programul principal pentru a obține fișierul obiect corespunzător:

gcc -Wall -c -o main.o main.c
În urma apelului precedent s-a obținut fișierul obiect main.o. Folosind deci fișierul obiect main.o și biblioteca compilată libtest.a vom apela procesul de link-editare pentru a obține programul principal denumit prog:

gcc -Wall -o proc main.o -L . -ltest
În urma acestui apel, am obținut programul principal prog ce poate fi executat. Fișierul executabil se poate deci executa fără a mai avea nevoie explicită de biblioteca libtest.a, aceasta din urmă fiind conținută în fișierul executabil. În apelul de mai sus, parametrul -L specifică compilatorului calea unde să caute bibliotecile ce vor fi date mai departe de utilizator (în cazul de față bibliotecile vor fi căutate în directorul curent). Parametrul -l specifică o bibliotecă cu care compilatorul să realizeze link-editarea. Se poate observa că în cazul de față biblioteca dată ca și parametru se numește test, iar fișierul compilat al bibliotecii se numește libtest. De aici se poate extrage regula ce spune că un fișier bibliotecă trebuie să înceapă cu prefixul lib înaintea numelui propriu-zis.

Biblioteci dinamice
Bibliotecile dinamice sunt similare cu bibliotecile statice în sensul că reprezintă o colecție de funcții cu un cuprins la început. Bibliotecile dinamice sunt referențiate la compilare (compile-time) ca și cele statice, dar nu sunt incluse în executabil și sunt folosite doar la rulare (run-time). Practic compilatorul, în procesul de link-editare încearcă să referențieze simbolurile din cod cu cele din bibliotecile dinamice necesare, fără a le include în codul executabil (așa cum se făcea cu bibliotecile statice). De fapt, bibliotecile dinamice sunt doar "inspectate" în procesul de link-editare și nu sunt folosite efectiv. După compilare, în procesul de rulare a programului (la "run-time") bibliotecile necesare sunt folosite. Acestea fac parte din mediul de run-time al sistemului și la momentul execuției sistemul caută bibliotecile necesare și le poate link-edita dinamic atunci când este nevoie. Dacă bibliotecile nu există la momentul execuției, programul nu este lansat și sistemul generează o eroare în care explică ce bibliotecă nu a putut fi găsită.

Ca și în cazul bibliotecilor statice, o bibliotecă dinamică trebuie să fie însoțită de un fișier header corespunzător pentru a informa programatorul despre ce funcții există în bibliotecă și cum se pot apela. În Linux bibliotecile dinamice se numesc shared object, au în general exensia .so și se găsesc în /lib, /lib32, /lib64, /usr/lib. În Windows bibliotecile dinamice se numesc dynamic linked libraries și au extensia .dll.

Pentru exemplificare vom considera codul anterior.

În primul pas, vom compila biblioteca implementată în fișierele test.c și test.h astfel încât să obținem o bibliotecă dinamică (un shared object).

gcc -Wall -shared -o libtest.so test.c
Se poate observa că diferența dintre a compila o bibliotecă statică și una dinamică este că la cea dinamică compilatorului i se furnizează argumentul "-shared" și i se spune astfel să creeze o bibliotecă dinamică (un shared object).

În pasul următor, vom compila programul principal pentru a obține fișierul obiect necesar:

gcc -Wall -c -o main.o main.c
În ultumul pas vom apela din nou compilator gcc pentru a executa operațiunea de link-editare și a obține astfel fișierul executabil:

gcc -Wall -o prog -L . -ltest main.o
Apelul anterior va obține programul executabil și este similar cu cel folosit la compilarea anterioară cu biblioteca statică. Parametrul -L specifică compilatorului calea unde găsește bibliotecile ce vor fi date mai departe de utilizator (în cazul de față bibliotecile vor fi căutate în directorul curent). Parametrul -l specifică o bibliotecă cu care compilatorul să realizeze link-editarea. Se poate observa că și în cazul de față biblioteca dată ca și parametru se numește test, iar fișierul compilat al bibliotecii se numește libtest. Trebuie doar menționat faptul că în acest caz biblioteca libtest.so nu este inclusă în fișierul executabil ci s-a făcut o link-editare parțială rămânând ca biblioteca să fie folosită la run-time.

În pasul următor vom lansa în execuție programul. Fără a face alte operațiuni, în urma execuției vom primi o eroare:

valy@debiandev:~/shared/static$ ./prog
./prog: error while loading shared libraries: libtest.so: cannot open shared object file: No such file or
directory
Eroarea a apărut pentru că în procesul de link-editare la run-time sistemul nu a găsit în baza lui de date biblioteca libtest.so. Acest lucru se poate vedea și prin invocarea programului ldd. Acesta afișează la stdout toate bibliotecile dinamice referențiate de un program executabil, precum și biblioteca cu care se poate face link-editarea la run-time. În cazul nostru vom primi următorul răspuns:

valy@debiandev:~/shared/static$ ldd prog
linux-vdso.so.1 (0x00007ffe7a7e9000)
libtest.so => not found
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fe407651000)
/lib64/ld-linux-x86-64.so.2 (0x00007fe407bf2000)
În partea stângă a semnului "=>" se afișează numele bibliotecii referențiate de programul executabil, iar în partea dreaptă a semnului "=>" se afișează biblioteca cu care s-a făcut link-editarea dinamică (și adresa acestuia). Se poate observa că în cazul bibliotecii libtest.so nu s-a făcut link-editarea, cauza fiind lipsa fișierului. Acesta nu a fost găsit în baza de date a sistemului ce conține bibliotecile dinamice. Această bază de date se află în general în fișierul /etc/ld.so.cache, gestionată de programul ldconfig (ce trebuie executat cu drepturi de root). Pe lângă această bază de date, sistemul mai poate căuta bibliotecile dinamice și în alte locații.

Cea mai simplă metodă, prin care i se poate spune sistemului ce altă cale să includă în căutarea bibliotecilor, constă în setarea variabilei de mediu LD_LIBRARY_PATH astfel:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/valy/shared/static
După setarea acestei variabile de mediu cu calea directorului în care se află biblioteca exemplu, comanda ldd va da următorul răspuns:

valy@debiandev:~/shared/static$ ldd prog
linux-vdso.so.1 (0x00007ffe75568000)
libtest.so (0x00007f933d797000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f933d3f8000)
/lib64/ld-linux-x86-64.so.2 (0x00007f933db9b000)
În acest caz se observa că s-a putut realiza link-editarea cu biblioteca libtest.so. Programul poate deci rula fără probleme.
