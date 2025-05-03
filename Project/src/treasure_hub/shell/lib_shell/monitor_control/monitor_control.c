#include "monitor_control.h"

char log_msg[BUFSIZ];

//=============================================================================
// Start Monitor + Init Pipes
//=============================================================================

operation_error cmd_start_monitor(void) {
  // if the monitor is already running, return
  if (is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] Monitor already running (PID: %d)\n",
             shell.monitor_pid);
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return OPERATION_FAILED;
  }

  // main process    write >--pipe--> read    child process

  // init the pipes
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("pipe");
    return OPERATION_FAILED;
  }

  shell.monitor_pid = fork();
  if (shell.monitor_pid < 0) {
    perror("Failed to fork monitor");
    close(pipefd[0]);
    close(pipefd[1]);
    return OPERATION_FAILED;
  }

  if (shell.monitor_pid == 0) {
    // child: close the write‑end, dup read‑end onto STDIN, then exec
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    execl(MONITOR_CMD_PATH, MONITOR_CMD_NAME, NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  }

  // parent: close the read‑end, keep the write‑end for later
  close(pipefd[0]);
  shell.monitor_pipe_fd = pipefd[1];
  shell.state = MON_ONLINE;

  return OPERATION_SUCCESS;
}

//=============================================================================
// Stop Monitor and Cleanup
//=============================================================================

operation_error cmd_stop_monitor(void) {
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] No monitor process is currently running.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return OPERATION_FAILED;
  }

  shell.state = MON_SHUTTING_DOWN;
  kill(shell.monitor_pid, SIGTERM);

  // close the pipes
  close(shell.monitor_pipe_fd);

  // Inform the user we’ve sent SIGTERM; actual exit message
  // comes asynchronously in handle_sigchld().
  snprintf(log_msg, BUFSIZ, "[✓] Shutdown signal sent.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return OPERATION_SUCCESS;
}

//=============================================================================
// Monitor Status Check
//=============================================================================

int is_monitor_alive(void) {
  return (shell.monitor_pid > 0) && (kill(shell.monitor_pid, 0) == 0);
}

// While monitor is shutting down, intercept commands and report status
int check_monitor_stopping(void) {
  if (shell.state != MON_SHUTTING_DOWN)
    return 0;

  snprintf(log_msg, BUFSIZ, "[!] Monitor is shutting down, please wait.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 1;
}
