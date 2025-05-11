#include "monitor_exec.h"

//=============================================================================
// Execute the manager binary with the given command string
//=============================================================================

void execute_manager(const char *cmd) {

  char msg[BUFSIZ];

  // init the pipes
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("pipe");
    return;
  }

  // Fork and exec "sh -c <cmd>"
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return;
  }

  if (pid < 0) {
    write(STDOUT_FILENO, "\r\033[K", 4);

    sprintf(msg, "[MONITOR] fork failed: %s\n", strerror(errno));
    write(STDERR_FILENO, msg, strlen(msg));

    return;
  }

  if (pid == 0) {
    // Child
    close(pipefd[0]);               // Close read end
    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
    close(pipefd[1]);

    // In child, execute the manager
    execlp("sh", "sh", "-c", cmd, (char *)NULL);

    // Refresh the screen and print error message
    write(STDOUT_FILENO, "\r\033[K", 4);
    sprintf(msg, "[MONITOR] exec failed: %s\n", strerror(errno));
    write(STDERR_FILENO, msg, strlen(msg));

    exit(EXIT_FAILURE);
  }

  // Parent
  close(pipefd[1]); // Close write end

  // read the data from the calculator
  char buffer[BUFSIZ];
  ssize_t n;
  while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
    buffer[n] = '\0';
    write(STDOUT_FILENO, buffer, n);
  }

  // Parent waits for the child
  waitpid(pid, NULL, 0);

  // send signal to parent to refresh the screen
  kill(getppid(), SIGUSR1);
}
