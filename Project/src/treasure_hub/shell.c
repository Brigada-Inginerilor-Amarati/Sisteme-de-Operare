#include "shell.h"
#include "../lib/hub_shell_args_parser/hub_shell_args_parser.h"
#include "../lib/utils/utils.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELAY_START_MONITOR 500000
#define TIME_BUFFER_SIZE 32

pid_t monitor_pid = -1;
int monitor_running = 0;
char log_msg[BUFSIZ];

void refresh_prompt() {
  char time_buf[TIME_BUFFER_SIZE];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);

  strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", tm_info);

  const char *status = monitor_running ? "running" : "stopped";

  snprintf(log_msg, BUFSIZ, "\n%s treasure_hub [monitor: %s] $ ", time_buf,
           status);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

int is_monitor_alive() { return monitor_pid > 0 && kill(monitor_pid, 0) == 0; }

void start_monitor() {
  if (is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] Monitor already running (PID: %d)\n",
             monitor_pid);
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  monitor_pid = fork();

  if (monitor_pid < 0) {
    perror("Failed to fork monitor");
    return;
  }

  if (monitor_pid == 0) {
    // Child
    execl("./bin/monitor", "monitor", NULL);
    perror("Failed to start monitor");
    exit(EXIT_FAILURE);
  }

  monitor_running = 1;
  snprintf(log_msg, BUFSIZ, "[✓] Monitor started in background (PID: %d)\n",
           monitor_pid);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

void stop_monitor() {
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] No monitor process is currently running.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  // Send termination signal (e.g. SIGUSR2)
  if (kill(monitor_pid, SIGTERM) == -1) {
    perror("Failed to send termination signal to monitor");
    return;
  }

  // Wait for monitor to exit
  int status;
  if (waitpid(monitor_pid, &status, 0) == -1) {
    perror("Failed to wait for monitor");
  } else {
    if (WIFEXITED(status)) {
      snprintf(log_msg, BUFSIZ, "[✓] Monitor exited with status: %d\n",
               WEXITSTATUS(status));
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    } else if (WIFSIGNALED(status)) {
      snprintf(log_msg, BUFSIZ, "[!] Monitor was killed by signal: %d\n",
               WTERMSIG(status));
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    } else {
      snprintf(log_msg, BUFSIZ, "[!] Monitor terminated abnormally.\n");
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    }
  }

  monitor_running = 0;
  monitor_pid = -1;
}

operation_error handle_command(shell_command cmd, char args[][BUFSIZ]) {
  // help
  switch (cmd) {
  case CMD_HELP:
    print_help_info();
    break;
  case CMD_CLEAR:
    clear_screen();
    break;
  case CMD_START_MONITOR:
    start_monitor();
    break;
  case CMD_STOP_MONITOR:
    stop_monitor();
    break;
  case CMD_EXIT:
    exit_shell();
    break;
  default:
    write(STDOUT_FILENO, "Invalid command\n\n", 17);
    print_help_info();
    break;
  }
  return NO_ERROR;
}

int init_REPL_shell() {
  char input[BUFSIZ];
  char args[MAX_ARGS][BUFSIZ];

  while (1) {
    refresh_prompt();

    if (read_line(STDIN_FILENO, input, BUFSIZ) < 0)
      break;

    // Parse command and arguments
    shell_command cmd = parse_shell_cmd(input, args);

    // Run command
    handle_command(cmd, args);

    // Delay for the start_monitor command
    if (cmd == CMD_START_MONITOR) {
      usleep(DELAY_START_MONITOR);
    }
  }

  return 0;
}

int main(void) {
  init_REPL_shell();
  return 0;
}
