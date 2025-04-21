/*
Realizati un program  C pentru UNIX care creaza 20 de procese (inclusiv
parintele). Fiecare proces afiseaza pe ecran cate 10 linii continand tipul sau
(parinte, fiu1, fiu2, ... , fiu19), PID-ul propriu si PID-ul parintelui
(inclusiv parintele). Dupa aceea, procesele fiu se vor termina returnand valori
diferite, iar parintele va afisa valorile returnate de catre fii si PID-ul lor.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCESS_COUNT 19

int main(void) {
  pid_t pids[PROCESS_COUNT];
  int i;

  for (i = 0; i < PROCESS_COUNT; i++) {
    pid_t pid = fork();

    if (pid < 0) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }

    if (pid == 0) {
      // Proces copil
      for (int j = 0; j < 10; j++) {
        printf("Fiu%d - PID: %d, Parinte PID: %d\n", i + 1, getpid(),
               getppid());
        fflush(stdout);
      }
      exit(i + 1); // Coduri de ieșire diferite: 1 .. 19
    } else {
      // Salvăm PID-ul copilului pentru referință (opțional)
      pids[i] = pid;
    }
  }

  // Procesul parinte
  for (int j = 0; j < 10; j++) {
    printf("Parinte - PID: %d\n", getpid());
    fflush(stdout);
  }

  for (i = 0; i < PROCESS_COUNT; i++) {
    int status;
    pid_t pid = wait(&status);
    if (WIFEXITED(status)) {
      printf("Parinte: copilul cu PID %d s-a terminat cu codul %d\n", pid,
             WEXITSTATUS(status));
      fflush(stdout);
    }
  }

  return 0;
}
