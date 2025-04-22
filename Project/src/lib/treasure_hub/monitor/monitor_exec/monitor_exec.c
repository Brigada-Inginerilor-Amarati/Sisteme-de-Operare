#include "monitor_exec.h"

//=============================================================================
// Execute the manager binary with the given command string
//=============================================================================
void execute_manager(const char *cmd) {
  // Fork and exec "sh -c <cmd>"
  pid_t pid = fork();
  if (pid < 0) {
    write(STDOUT_FILENO, "\r\033[K", 4);
    dprintf(STDERR_FILENO, "[MONITOR] fork failed: %s\n", strerror(errno));
    return;
  }
  if (pid == 0) {
    // In child, execute the manager
    execlp("sh", "sh", "-c", cmd, (char *)NULL);
    write(STDOUT_FILENO, "\r\033[K", 4);
    dprintf(STDERR_FILENO, "[MONITOR] exec failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // Parent waits for the child
  waitpid(pid, NULL, 0);

  kill(getppid(), SIGUSR1);
}
