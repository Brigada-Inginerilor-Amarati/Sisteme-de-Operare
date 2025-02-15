#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  pid_t pid;
  
  if((pid=fork())<0)
  {
    printf("Eroare la fork\n");
    exit(1);
  }
  if(pid==0) /* procesul fiu */
  {
    execlp("ls","ls","-l",NULL); /* procesul va rula comanda ls */
    printf("Eroare la exec\n"); 
    /* Daca execlp s-a intors, inseamna ca programul
     nu a putut fi lansat in executie */
  }
  else /* procesul parinte */
  {
    printf("Proces parinte\n");
    exit(0);
  }
}