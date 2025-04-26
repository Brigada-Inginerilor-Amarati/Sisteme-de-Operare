#include "monitor_exec.h"

//=============================================================================
// Execute the manager binary with the given command string
//=============================================================================
void execute_manager(const char *cmd) {

  char msg[BUFSIZ];

  // Fork and exec "sh -c <cmd>"
  pid_t pid = fork();

  if (pid < 0) {
    write(STDOUT_FILENO, "\r\033[K", 4);

    sprintf(msg, "[MONITOR] fork failed: %s\n", strerror(errno));
    write(STDERR_FILENO, msg, strlen(msg));

    return;
  }
  if (pid == 0) {
    // In child, execute the manager
    execlp("sh", "sh", "-c", cmd, (char *)NULL);

    // Refresh the screen and print error message
    write(STDOUT_FILENO, "\r\033[K", 4);
    sprintf(msg, "[MONITOR] exec failed: %s\n", strerror(errno));
    write(STDERR_FILENO, msg, strlen(msg));

    exit(EXIT_FAILURE);
  }
  // Parent waits for the child
  waitpid(pid, NULL, 0);

  // send signal to parent to refresh the screen
  kill(getppid(), SIGUSR1);
}
