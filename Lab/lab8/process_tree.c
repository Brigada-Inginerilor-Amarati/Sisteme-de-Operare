#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  for (int i = 0; i < 8; i++) {
    fork();
    printf("Salut din proces PID: %d, iteratia: %d\n", getpid(), i);
    fflush(stdout);
  }

  sleep(2); // pentru a putea observa procesele în viață cu `ps` sau `pstree`
  return 0;
}
