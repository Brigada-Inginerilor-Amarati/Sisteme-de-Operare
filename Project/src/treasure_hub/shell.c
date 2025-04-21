#include "shell.h"
#include "../lib/hub_shell_args_parser/hub_shell_args_parser.h"

char log_msg[BUFSIZ];
int monitor_pipe_fd = -1;
const char *manager_path = "bin/treasure_manager";
const char *list_cmd = "--list";
//=============================================================================
// Forward Declarations
//=============================================================================
void setup_signal_handlers(void);
void sigchld_handler(int sig);
void refresh_prompt(void);
int is_monitor_alive(void);
void cmd_start_monitor(void);
void cmd_stop_monitor(void);
int check_monitor_stopping(void);
operation_error dispatch_command(shell_command cmd, char args[][BUFSIZ],
                                 int argc);
int run_repl(void);
void send_to_monitor(const char *cmd_line);
operation_error cmd_list_hunts(char argv[][BUFSIZ], int argc);
operation_error cmd_list_treasures(char argv[][BUFSIZ], int argc);
operation_error cmd_view_treasure(char argv[][BUFSIZ], int argc);
void sig_refresh_handler(int sig);

//=============================================================================
// Signal Handling
//=============================================================================
void setup_signal_handlers(void) {
  struct sigaction sa_child = {.sa_handler = sigchld_handler,
                               .sa_flags = SA_RESTART | SA_NOCLDSTOP};
  sigaction(SIGCHLD, &sa_child, NULL);

  struct sigaction sa_refresh = {.sa_handler = sig_refresh_handler,
                                 .sa_flags = 0};
  sigaction(SIGUSR1, &sa_refresh, NULL);
}

void sig_refresh_handler(int sig) {
  usleep(100000);
  write(STDOUT_FILENO, "\r", 1);
}

void sigchld_handler(int sig) {
  int saved_errno = errno;
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (pid == shell.monitor_pid && shell.state == MON_STOPPING) {
      if (WIFEXITED(status)) {
        snprintf(log_msg, BUFSIZ, "[✓] Monitor exited (status=%d)\n",
                 WEXITSTATUS(status));
      } else if (WIFSIGNALED(status)) {
        snprintf(log_msg, BUFSIZ, "[!] Monitor died by signal %d\n",
                 WTERMSIG(status));
      }
      write(STDOUT_FILENO, log_msg, strlen(log_msg));

      shell.state = MON_STOPPED;
      shell.monitor_pid = -1;
      tcflush(STDIN_FILENO, TCIFLUSH);
      refresh_prompt();
    }
  }

  errno = saved_errno;
}

//=============================================================================
// Send a command line to the monitor via Pipe
//=============================================================================
void send_to_monitor(const char *cmd_line) {

  // error message if monitor is not running
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ,
             "[!] Monitor is not running. Please start it first.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  // append newline so read_line in monitor sees an end‑of‑line
  snprintf(log_msg, BUFSIZ, "%s\n", cmd_line);
  write(monitor_pipe_fd, log_msg, strlen(log_msg));
}

//=============================================================================
// Command implementations
//=============================================================================

// list_hunts: no args
operation_error cmd_list_hunts(char argv[][BUFSIZ], int argc) {

  if (argc != LIST_HUNTS_ARGC)
    return OPERATION_FAILED;
  char buf[BUFSIZ];
  strcpy(argv[0], list_cmd);
  snprintf(buf, sizeof(buf), "%s %s", manager_path, argv[0]);
  send_to_monitor(buf);
  return NO_ERROR;
}

// list_treasures: <hunt_id>
operation_error cmd_list_treasures(char argv[][BUFSIZ], int argc) {
  if (argc != LIST_TREASURES_ARGC)
    return OPERATION_FAILED;
  char buf[BUFSIZ];
  strcpy(argv[0], list_cmd);
  snprintf(buf, sizeof(buf), "%s %s %s", manager_path, argv[0], argv[1]);
  send_to_monitor(buf);
  return NO_ERROR;
}

// view_treasure: <hunt_id> <treasure_id>
operation_error cmd_view_treasure(char argv[][BUFSIZ], int argc) {
  if (argc != VIEW_TREASURE_ARGC)
    return OPERATION_FAILED;
  char buf[BUFSIZ];
  strcpy(argv[0], list_cmd);
  snprintf(buf, sizeof(buf), "%s %s %s %s", manager_path, argv[0], argv[1],
           argv[2]);
  send_to_monitor(buf);
  return NO_ERROR;
}

//=============================================================================
// Prompt & Input Helpers
//=============================================================================
void refresh_prompt(void) {
  char time_buf[TIME_BUFFER_SIZE];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);

  strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", tm_info);
  const char *status = (shell.state == MON_RUNNING) ? "running" : "stopped";

  snprintf(log_msg, BUFSIZ, "\n%s treasure_hub [monitor: %s] $ ", time_buf,
           status);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

int is_monitor_alive(void) {
  return (shell.monitor_pid > 0) && (kill(shell.monitor_pid, 0) == 0);
}

//=============================================================================
// Monitor Control
//=============================================================================
void cmd_start_monitor(void) {
  // if the monitor is already running, return
  if (is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] Monitor already running (PID: %d)\n",
             shell.monitor_pid);
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  // init the pipes
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("pipe");
    return;
  }

  shell.monitor_pid = fork();
  if (shell.monitor_pid < 0) {
    perror("Failed to fork monitor");
    close(pipefd[0]);
    close(pipefd[1]);
    return;
  }

  if (shell.monitor_pid == 0) {
    // child: close the write‑end, dup read‑end onto STDIN, then exec
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    execl("./bin/monitor", "monitor", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
  }

  // parent: close the read‑end, keep the write‑end for later
  close(pipefd[0]);
  monitor_pipe_fd = pipefd[1];
  shell.state = MON_RUNNING;
}

void cmd_stop_monitor(void) {
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ, "[!] No monitor process is currently running.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  shell.state = MON_STOPPING;
  kill(shell.monitor_pid, SIGTERM);

  // Inform the user we’ve sent SIGTERM; actual exit message
  // comes asynchronously in handle_sigchld().
  snprintf(log_msg, BUFSIZ, "[✓] Shutdown signal sent.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

// While monitor is shutting down, intercept commands and report status
int check_monitor_stopping(void) {
  if (shell.state != MON_STOPPING)
    return 0;

  snprintf(log_msg, BUFSIZ, "[!] Monitor is shutting down, please wait.\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 1;
}

//=============================================================================
// Exit command
//=============================================================================
void cmd_exit_shell() {
  if (is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ,
             "Cannot exit while monitor is running. Use stop_monitor first.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  snprintf(log_msg, BUFSIZ, "Exiting treasure_hub shell\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  exit(0);
}

//=============================================================================
// Command Dispatch
//=============================================================================
operation_error dispatch_command(shell_command cmd, char argv[][BUFSIZ],
                                 int argc) {
  // Intercept during shutdown
  if (check_monitor_stopping())
    return NO_ERROR;

  // Go through command cases
  switch (cmd) {
  case CMD_HELP:
    cmd_print_help();
    break;
  case CMD_CLEAR:
    cmd_clear_screen();
    break;
  case CMD_START_MONITOR:
    cmd_start_monitor();
    break;
  case CMD_STOP_MONITOR:
    cmd_stop_monitor();
    break;
  case CMD_EXIT:
    cmd_exit_shell();
    break;
  case CMD_LIST_HUNTS:
    if (cmd_list_hunts(argv, argc) != NO_ERROR)
      return OPERATION_FAILED;
    break;
  case CMD_LIST_TREASURES:
    if (cmd_list_treasures(argv, argc) != NO_ERROR)
      return OPERATION_FAILED;
    break;
  case CMD_VIEW_TREASURE:
    if (cmd_view_treasure(argv, argc) != NO_ERROR)
      return OPERATION_FAILED;
    break;
  default:
    cmd_print_help();
    return OPERATION_FAILED;
  }
  return NO_ERROR;
}

//=============================================================================
// REPL (Read, Evaluate, Print, Loop)
//=============================================================================

int run_repl(void) {
  char input[BUFSIZ];
  char argv[MAX_ARGS][BUFSIZ];
  int argc = 0;
  while (1) {
    refresh_prompt();
    ssize_t bytes_read = read_line(STDIN_FILENO, input, BUFSIZ);
    if (bytes_read < 0) {
      if (errno == EINTR)
        continue;
      else
        break;
    }

    shell_command cmd = parse_shell_cmd(input, argv, &argc);

    operation_error err = dispatch_command(cmd, argv, argc);

    if (err != NO_ERROR) {
      snprintf(log_msg, BUFSIZ, "Invalid command\n");
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    }

    if (cmd == CMD_START_MONITOR)
      usleep(DELAY_START_MONITOR);
  }

  return 0;
}

//=============================================================================
// main()
//=============================================================================
int main(void) {
  setup_signal_handlers();
  return run_repl();
}
