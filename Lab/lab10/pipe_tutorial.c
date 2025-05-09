#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/syslimits.h>
#include <unistd.h>

int main(void) {

  int pfd[2];
  pid_t pid;

  if (pipe(pfd) < 0) {
    perror("pipe");
    exit(1);
  }

  int limit = PIPE_BUF;

  pid = fork();

  if (pid < -1) {
    perror("fork");
    exit(2);
  }

  if (pid == 0) {
    close(pfd[0]); // close read end
    char *msg = "Hello World";
    write(pfd[1], msg, strlen(msg) + 1);
    close(pfd[1]);
    exit(0);
  }

  close(pfd[1]);
  char buf[100];
  read(pfd[0], buf, sizeof(buf));
  printf("%s", buf);
  close(pfd[0]);

  return 0;
}
