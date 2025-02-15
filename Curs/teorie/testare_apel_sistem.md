Apelurile sistem, dar și funcțiile de bibliotecă, de obicei returnează anumite valori prin care se comunică apelantului dacă acel apel s-a executat corect sau nu. Valoarea efectiv returnată se poate afla în pagina de manual a funcției respective, fie că este vorba de apel sistem sau funcție de bibliotecă. În cazul în care un apel sistem sau funcție de bibliotecă s-a terminat cu eroare este posibil ca toate operațiunile următoare efectuate de programator să fie compromise. Din aceste motive, este absolut necesar să se testeze.

De asemenea, în majoritatea situațiilor aceste apeluri, în caz de eroare, vor seta variabila globală errno (declarată în errno.h), moștenită de fiecare program. Această varibilă errno, în urma apariției unei erori este setată cu o valoare menită să explice situația eronată apărută (mai multe informații despre errno și valorile posibile se pot găsi în pagina de manual pentru errno, secțiunea 3 -> man 3 errno). Aceste valori, de multe ori, sunt greu de folosit în varianta lor inițială. Pentru a facilita obținerea erorilor apărute se pot folosi următoarele funcții:

void perror(const char \*s);
Funcția perror afișează la ieșirea standard de eroare un mesaj menit să descrie ultima eroare apărută. Practic această funcție afișează un mesaj ce "transcrie" valoarea numerică a lui errno. String-ul așteptat ca și parametru este afișat înaintea mesajului de eroare. Se poate furniza și string-ul nul "". Pagina de manual a funcției se află în secțiunea 3 (man 3 perror).

char \*strerror(int errnum);
Funcția strerror este asemănătoare cu funcția perror, dar aceasta nu afișează ci returnează string-ul ce l-ar afișa funcția perror. Practic această funcție returnează un string ce descrie valoarea erorii reprezentate de obicei de errno, dar codul numeric trebuie dat ca și parametru. Pagina de manual a funcției se află în sectțunea 3 (man 3 strerror). Un exemplu de utilizare ar putea fi următorul:

#include <string.h>
#include <errno.h>
.......
char \*error_string = strerror(errno);
Într-un mod foarte simplificat, o metodă de testare a unui apel sistem (sau funcție de bibliotecă) ar putea fi următoarea:

```c
#include <stdio.h>

int main(void){
.....
	int return_value = system_call(...);
	if (return_value < 0)
	{
	perror("mesaj");
	exit(-1);
	}
	.....
	return 0;
}
```

În exemplul de mai sus s-a folosit o funcție generică "system_call" ce se va înlocui cu apelul sistem propriu-zis. De asemenea, valoarea returnată este cea precizată în pagina de manual (este foarte probabil, în unele cazuri, să nu fie -1 în caz de eroare). O variantă similară, înlocuind perror cu strerror ar putea fi următoarea:
#include <string.h>
#include <errno.h>
#include <stdio.h>

```c
int main(void)
{
....
	int return_value = system_call(...);
	if (return_value < 0)
	{
	char \*text = strerror(errno);
	printf ("%s\n", text);
	exit(-1);
	}
....
}
```
