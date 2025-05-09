// proces parinte, 2 procese copil, ambele scriu simultan in acelasi pipe

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/syslimits.h>
#include <unistd.h>

int main(void) {

  int pfd[2];
  pid_t pid1, pid2;

  if (pipe(pfd) < 0) {
    perror("pipe");
    exit(0);
  }

  pid1 = fork();

  if (pid1 == -1) {
    perror("fork 1");
    exit(1);
  }

  if (pid1 == 0) {
    close(pfd[0]);
    char *msg = "CHILD 1";
    write(pfd[1], msg, strlen(msg) + 1);
    // perror("child 1 writing");
    //   sleep(1);
    close(pfd[1]);
    exit(0);
  }

  pid2 = fork();

  if (pid2 == -1) {
    perror("fork 2");
    exit(2);
  }

  if (pid2 == 0) {
    close(pfd[0]);
    char *msg = "CHILD 2";
    write(pfd[1], msg, strlen(msg) + 1);
    // perror("child 2 writing");
    //  sleep(1);
    close(pfd[1]);
    exit(0);
  }

  close(pfd[1]);
  char buf[100];
  read(pfd[0], buf, sizeof(buf));
  printf("%s", buf);
  close(pfd[0]);

  wait(NULL);

  return 0;
}
