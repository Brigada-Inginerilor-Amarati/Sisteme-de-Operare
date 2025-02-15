În limbajul C (și nu numai), zonele de memorie accesate de un program sunt următoarele:

zona de cod
zona de stivă (stack)
zona de heap
zona statică (de variabile globale)
Zona de cod  conține codul executabil al programului și bibliotecilor utilizate de către acesta. Această zonă este read-only (poate fi doar citită) la rulare (runtime).

Zona de date statice (zona variabilelor globale)

Această zonă este formată din totalitatea spațiului de memorie ocupat de variabilele alocate static. Variabilele alocate static sunt reprezentate de variabilele definite global (variabile globale) și de variabilele declarate în corpul unei funcții, dar cu cuvântul "static" în fața declarației tipului variabilei. În această zonă se poate scrie, variabilele pot fi modificate, dar dimensiunea ei nu poate fi modificată în timpul rulării programului. Această zonă de memorie este alocată static de către compilator.

Zona de stivă reprezinta o zonă de memorie adresată după politica LIFO (Last In First Out) folosind operații de PUSH (adăugare în stivă) și POP (eliminare din stivă). Zona de stivă stă la baza execuției programelor ce conțin apeluri de funcții. În limbajul C, pe stivă se găsesc următoarele: parametrii unei funcții, variabilele locale ale unei funcții, valoarea returnată de o funcție, adresa de întoarcere după apelul funcției (return address). Dat fiind faptul că și functia main() este tot o funcție și pentru aceasta se aplică aceleași reguli.

Stiva este limitată de cele mai multe ori de către sistemul de operare (sau de către compilator). În sistemele Linux dimensiunea stivei se poate obține apelând pe linie de comandă comanda ulimit astfel:

valy@staff:~$ ulimit -s
8192
Se observă că rezultatul apelului comenzii "ulimit -s " este o valoare (în kbytes) ce reprezintă dimensiunea maximă a stivei. Astfel, în cazul în care programul scris depășește această valoare, sistemul de operare va termina forțat acel program, de cele mai multe ori chiar înainte de rularea propriu-zisă deoarece se ajunge la inițializarea stivei chiar înainte de apelul funcției main().

În aceste condiții următorul program va compila, dar cu siguranță nu va rula și se va primi clasica eroare "Segmentation fault"

#include <stdio.h>
int main(void)
{
   ......
   int tab[10000000];
   ......
}
De asemenea, este important de remarcat și următoarea situație: se consideră 2 programe:

Program 1:

void function(void)
{
   int tab[1000];
   for (int i = 0; i < 1000; i++)
   {
      tab[i] = i;
   }
}
int main(void)
{
   function();
}
Program 2:

int tab[1000];
void function(void)
{
   for (int i = 0; i < 1000; i++)
   {
      tab[i] = i;
   }
}

int main(void)
{
    function();
}
Între cele 2 programe, aparent identice, există o mare diferență în termeni de performanță. În primul caz, apelul funcției "function()" durează mult mai mult decât apelul din al doilea program dat fiind faptul că în primul caz variabila "tab" (un tablou de 1000 de elemente) este stocat în zona de stivă. Astfel pentru apelul funcției "function()" sunt necesare mai multe operații de stocare în stivă decât în al doilea caz (dacă lipsesc optimizările de compilator, în cazul primului program se vor executa 1000 operații de tip PUSH și 1000 operații de tip POP în plus față de al doilea program).

Este important de menționat că în cazul primului program se poate remedia situația prin adăugarea cuvântului cheie "static" în fața declarației variabilei din interiorul funcției. În acest caz, cuvântul "static" practic "mută" variabila "tab" din zona de stivă în zona statică păstrând restricțiile de vizibilitate (deși variabila "tab" ajunge în zona variabilelor globale, ea va fi în continuare vizibilă doar în corpul funcției în care a fost declarată).

O altă utilizare a cuvântului "static" este aceea că se poate pune și în fața declarației unei variabile globale. În aceasta situație variabila globală respectivă va fi "blocată" la nivelul fișierului obiect obținut prin compilare (și nu va putea fi referențiată în nici un mod în alt fișier obiect).

Zona heap

Această zonă de memorie este utilizată pentru a stoca blocuri de memorie în urma operațiunilor de alocare dinamică. În această zonă nu sunt stocate variabilele alocate dinamic, ci zonele de memorie alocate dinamic spre care referențiază variabilele respective. Această zonă de memorie este în principal limitată doar de capabilitățile hardware ale sistemului de calcul pe care rulează.



/* De adaugat:

- despre variabile volatile

- un exemplu cu o functie cu 2 parametri si sa desenam

cum se pune pe stack un stack frame si cu pointerii

- stack & heap avantaje si dezavantaje:

viteza, cine face managementul, cum se face alocarea, etc.*/