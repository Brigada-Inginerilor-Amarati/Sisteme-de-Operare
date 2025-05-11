#include "monitor_control.h"
#include <unistd.h>

char log_msg[BUFSIZ];

//=============================================================================
// Start Monitor + Init Pipes
//=============================================================================

int cmd_start_monitor(void) {
  // if the monitor is already running, return
  if (is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] Monitor already running (PID: %d)\n",
             monitor.pid);
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return 0;
  }

  // main process    write >--pipe--> read    child process

  // init the pipes
  int shell_to_monitor_pipefd[2];
  if (pipe(shell_to_monitor_pipefd) < 0) {
    perror("pipe");
    return 0;
  }

  int monitor_to_shell_pipefd[2];
  if (pipe(monitor_to_shell_pipefd) < 0) {
    perror("pipe");
    return 0;
  }

  monitor.pid = fork();
  if (monitor.pid < 0) {
    perror("Failed to fork monitor");
    close(shell_to_monitor_pipefd[0]);
    close(shell_to_monitor_pipefd[1]);
    close(monitor_to_shell_pipefd[0]);
    close(monitor_to_shell_pipefd[1]);
    exit(EXIT_FAILURE);
  }

  if (monitor.pid == 0) {
    // child: close the write‑end, dup read‑end onto STDIN
    // for shell -> monitor communication
    close(shell_to_monitor_pipefd[1]);
    dup2(shell_to_monitor_pipefd[0], STDIN_FILENO);
    close(shell_to_monitor_pipefd[0]);

    // child: close the read-end, dup the write-end onto STDOUT
    // for monitor -> shell communication
    close(monitor_to_shell_pipefd[0]);
    dup2(monitor_to_shell_pipefd[1], STDOUT_FILENO);
    close(monitor_to_shell_pipefd[1]);

    execl(MONITOR_CMD_PATH, MONITOR_CMD_NAME, NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  }

  // parent: close the read‑end, keep the write‑end for later
  close(shell_to_monitor_pipefd[0]);
  monitor.write_pipe_fd = shell_to_monitor_pipefd[1];

  // parent: close the write-end, keep the read-end for later
  close(monitor_to_shell_pipefd[1]);
  monitor.read_pipe_fd = monitor_to_shell_pipefd[0];

  monitor.state = MONITOR_ONLINE;

  return 1;
}

//=============================================================================
// Stop Monitor and Cleanup
//=============================================================================

int cmd_stop_monitor(void) {
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] No monitor process is currently running.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return 0;
  }

  monitor.state = MONITOR_SHUTTING_DOWN;
  kill(monitor.pid, SIGTERM);

  // close the pipes
  close(monitor.read_pipe_fd);

  // Inform the user we’ve sent SIGTERM; actual exit message
  // comes asynchronously in handle_sigchld().
  snprintf(log_msg, BUFSIZ, "[✓] Shutdown signal sent.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 1;
}

//=============================================================================
// Monitor Status Check
//=============================================================================

int is_monitor_alive(void) {
  return (monitor.pid > 0) && (kill(monitor.pid, 0) == 0);
}

// While monitor is shutting down, intercept commands and report status
int check_monitor_stopping(void) {
  if (monitor.state != MONITOR_SHUTTING_DOWN)
    return 0;

  snprintf(log_msg, BUFSIZ, "[!] Monitor is shutting down, please wait.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 1;
}
