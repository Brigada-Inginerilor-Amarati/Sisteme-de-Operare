#include "shell_cmd.h"
#include "../monitor_control/monitor_control.h"
#include "../shell_utils/shell_utils.h"
char log_msg[BUFSIZ];

//=============================================================================
// List Commands + Monitor Pipe Management
//=============================================================================

operation_error send_to_monitor(const char *cmd_line) {

  // error message if monitor is not running
  if (!is_monitor_alive()) {
    snprintf(log_msg, BUFSIZ,
             "[!] Monitor is not running. Please start it first.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return OPERATION_FAILED;
  }

  // append newline so read_line in monitor sees an end‑of‑line
  snprintf(log_msg, BUFSIZ, "%s\n", cmd_line);
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
  default:
    cmd_print_help();
    return OPERATION_FAILED;
  }
  return OPERATION_SUCCESS;
}
