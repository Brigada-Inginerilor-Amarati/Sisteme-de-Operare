#include "shell_cmd.h"
#include "../monitor_control/monitor_control.h"
#include "../shell_utils/shell_utils.h"
#include <sys/_types/_useconds_t.h>
char log_msg[BUFSIZ];

//=============================================================================
// List Commands + Monitor Pipe Management
//=============================================================================

operation_error send_to_monitor(const char *cmd_line) {

  // error message if monitor is not running
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ,
             "[!] Monitor is not running. Use 'start_monitor' first.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return OPERATION_FAILED;
  }

  // append newline so read_line in monitor sees an end‑of‑line
  snprintf(log_msg, BUFSIZ, "%s\n", cmd_line);

  // send data to monitor via pipe
  write(shell.monitor_pipe_fd, log_msg, strlen(log_msg));

  return OPERATION_SUCCESS;
}

int check_argc_valid(shell_command cmd, int argc) {
  // printf("check_argc_valid called with cmd=%d and argc=%d\n", cmd, argc);
  switch (cmd) {
  case CMD_LIST_HUNTS:
    return argc == LIST_HUNTS_ARGC;
  case CMD_LIST_TREASURES:
    return argc == LIST_TREASURES_ARGC;
  case CMD_VIEW_TREASURE:
    return argc == VIEW_TREASURE_ARGC;
  default:
    return argc == DEFAULT_ARGC;
  }
}

operation_error cmd_list(shell_command cmd, char argv[][BUFSIZ], int argc) {
  if (!check_argc_valid(cmd, argc))
    return OPERATION_FAILED;

  char buf[BUFSIZ];
  strcpy(argv[0], LIST_CMD);
  snprintf(buf, sizeof(buf), "%s %s", MANAGER_CMD_PATH, argv[0]);

  for (int i = 1; i < argc; i++) {
    strcat(buf, " ");
    strcat(buf, argv[i]);
  }

  // printf("Sending command: %s\n", buf);

  return send_to_monitor(buf);
}

//=============================================================================
// Calculate Scores
//=============================================================================

void execute_calculator(const char *path) {
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("pipe");
    return;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return;
  }

  if (pid == 0) {
    // Child
    close(pipefd[0]);               // Close read end
    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
    close(pipefd[1]);

    execl(CALCULATOR_CMD_PATH, CALCULATOR_CMD_NAME, path, NULL);
    perror("execl failed");
    exit(EXIT_FAILURE);
  } else {
    // Parent
    close(pipefd[1]); // Close write end

    // read the data from the calculator
    char buffer[BUFSIZ];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
      buffer[n] = '\0';
      write(STDOUT_FILENO, buffer, n); // Output to terminal
    }

    close(pipefd[0]);
    waitpid(pid, NULL, 0); // Wait for child to finish
  }
}

operation_error cmd_calculate_scores() {

  DIR *dir = opendir(TREASURE_DIRECTORY);

  if (!dir) {
    perror("CALCULATE_SCORES");
    return OPERATION_FAILED;
  }

  // init the pipes
  /*
  int pipefd[2];
  if (pipe(pipefd) < 0) {
    perror("pipe");
    return OPERATION_FAILED;
  }
  */

  // go through every directory entry and create a fork
  // execute inside every fork the calculate_scores, using the path for each
  // entry as an argument

  struct dirent *entry;
  char file_path[PATH_MAX];

  while ((entry = readdir(dir)) != NULL) {

    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(file_path, sizeof(file_path), "%s/%s/%s", TREASURE_DIRECTORY,
             entry->d_name, TREASURE_FILE_NAME);

    execute_calculator(file_path);
  }

  return OPERATION_SUCCESS;
}

//=============================================================================
// Command Dispatch
//=============================================================================

operation_error dispatch_command(shell_command cmd, char argv[][BUFSIZ],
                                 int argc) {
  // Intercept during shutdown
  if (check_monitor_stopping())
    return OPERATION_SUCCESS;

  // Go through command cases
  switch (cmd) {
  case CMD_HELP:
    cmd_print_help();
    break;
  case CMD_CLEAR:
    cmd_clear_screen();
    break;
  case CMD_START_MONITOR:
    return cmd_start_monitor();
  case CMD_STOP_MONITOR:
    return cmd_stop_monitor();
  case CMD_EXIT:
    cmd_exit_shell();
    break;
  case CMD_LIST_HUNTS:
    return cmd_list(CMD_LIST_HUNTS, argv, argc);
  case CMD_LIST_TREASURES:
    return cmd_list(CMD_LIST_TREASURES, argv, argc);
  case CMD_VIEW_TREASURE:
    return cmd_list(CMD_VIEW_TREASURE, argv, argc);
  case CMD_CALCULATE_SCORES:
    return cmd_calculate_scores();
  default:
    cmd_print_help();
    return OPERATION_FAILED;
  }
  return OPERATION_SUCCESS;
}
