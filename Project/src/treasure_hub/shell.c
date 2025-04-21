#include "shell.h"
#include "../lib/hub_shell_args_parser/hub_shell_args_parser.h"
#include "../lib/utils/utils.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define DELAY_START_MONITOR 500000
#define TIME_BUFFER_SIZE 32

char log_msg[BUFSIZ];

void refresh_prompt() {
  char time_buf[TIME_BUFFER_SIZE];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);

  strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", tm_info);

  const char *status = mon_state == MON_RUNNING ? "running" : "stopped";

  snprintf(log_msg, BUFSIZ, "\n%s treasure_hub [monitor: %s] $ ", time_buf,
           status);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

int is_monitor_alive() { return monitor_pid > 0 && kill(monitor_pid, 0) == 0; }

void sigchld_handler(int sig) {
  int saved_errno = errno;
  int status;
  pid_t r;

  // reap all dead children
  while ((r = waitpid(-1, &status, WNOHANG)) > 0) {
    if (r == monitor_pid) {
      if (WIFEXITED(status)) {
        snprintf(log_msg, BUFSIZ, "\n[✓] Monitor exited (status=%d)\n",
                 WEXITSTATUS(status));
      } else if (WIFSIGNALED(status)) {
        snprintf(log_msg, BUFSIZ, "\n[!] Monitor died abnormally (signal=%d)\n",
                 WTERMSIG(status));
      }
      write(STDOUT_FILENO, log_msg, strlen(log_msg));

      mon_state = MON_STOPPED;
      monitor_pid = -1;
      tcflush(STDIN_FILENO, TCIFLUSH);
    }
  }

  errno = saved_errno;
}

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

  mon_state = MON_RUNNING;
}

void stop_monitor() {
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] No monitor process is currently running.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  mon_state = MON_STOPPING;

  kill(monitor_pid, SIGTERM);
}

void setup_signal_handlers() {
  struct sigaction sa = {.sa_handler = sigchld_handler,
                         .sa_flags = SA_RESTART | SA_NOCLDSTOP};
  sigaction(SIGCHLD, &sa, NULL);
}

int check_monitor_stopping(const char *input_line) {
  if (mon_state != MON_STOPPING)
    return 0;

  int status;
  pid_t r = waitpid(monitor_pid, &status, WNOHANG);

  if (r == 0) {
    // still shutting down
    snprintf(log_msg, BUFSIZ,
             "[!] Monitor is shutting down — please wait until it stops.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
  } else if (r == monitor_pid) {
    // monitor just exited
    if (WIFEXITED(status)) {
      snprintf(log_msg, BUFSIZ, "\n[✓] Monitor exited (status=%d)\n",
               WEXITSTATUS(status));
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    } else {
      snprintf(log_msg, BUFSIZ, "\n[!] Monitor died abnormally (signal=%d)\n",
               WTERMSIG(status));
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    }

    mon_state = MON_STOPPED;
    monitor_pid = -1;
    tcflush(STDIN_FILENO, TCIFLUSH);
  }

  return 1; // always swallow the user’s command while stopping
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
    // first prompt
    refresh_prompt();

    if (read_line(STDIN_FILENO, input, BUFSIZ) < 0)
      break;

    if (check_monitor_stopping(input)) {
      refresh_prompt();
      continue;
    }

    shell_command cmd = parse_shell_cmd(input, args);

    handle_command(cmd, args);
    if (cmd == CMD_START_MONITOR)
      usleep(DELAY_START_MONITOR);
  }
  return 0;
}

int main(void) {
  setup_signal_handlers();
  init_REPL_shell();
  return 0;
}
