// 2 pipe-uri
// creeaza un copil
// copilul:
// - trimite catre parinte "Salut, tata!"
// - asteapta un raspuns de la parinte si-l afiseaza
// parintele:
// - citeste mesajul de la copil si-l afiseaza
// - trimite inapoi un mesaj "Salut, fiule!"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

  // init pipes
  int pipe_parent_to_child[2], pipe_child_to_parent[2];

  if (pipe(pipe_parent_to_child) < 0 || pipe(pipe_child_to_parent) < 0) {
    perror("pipe failed");
    return 1;
  }

  char *child_message = "Salut, tata!";
  char *parent_message = "Salut, fiule!";

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return 1;
  }

  if (pid == 0) {
    // child process
    close(pipe_parent_to_child[1]); // close write end of parent-to-child pipe
    close(pipe_child_to_parent[0]); // close read end of child-to-parent pipe

    // send message to parent
    int bytes_written = write(pipe_child_to_parent[1], child_message,
                              strlen(child_message) + 1);

    sleep(1); // wait for parent to read

    // read message from parent
    char buffer[256];
    read(pipe_parent_to_child[0], buffer, sizeof(buffer));

    if (read(pipe_parent_to_child[0], buffer, sizeof(buffer)) < 0) {
      perror("child read failed");
      return 1;
    }

    close(pipe_child_to_parent[0]);
    close(pipe_child_to_parent[1]);

    exit(EXIT_SUCCESS);
  } else {

    // parent process
    close(pipe_parent_to_child[0]); // close read end of parent-to-child pipe
    close(pipe_child_to_parent[1]); // close write end of child-to-parent pipe

    // read message from child
    char buffer[256];
    int bytes_read = read(pipe_child_to_parent[0], buffer, sizeof(buffer));

    if (bytes_read < 0) {
      perror("parent read failed");
      return 1;
    }

    printf("Parent received: %s\n", buffer);

    // send message to child
    int bytes_written = write(pipe_parent_to_child[1], parent_message,
                              strlen(parent_message) + 1);

    if (bytes_written < 0) {
      perror("parent write failed");
      return 1;
    }

    // close pipes
    close(pipe_parent_to_child[0]);
    close(pipe_parent_to_child[1]);
  }
  return 0;
}
