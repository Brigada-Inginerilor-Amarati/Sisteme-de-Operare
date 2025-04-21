#include "hub_shell_args_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

shell_t shell = {.monitor_pid = -1, .state = MON_STOPPED};
char log_msg[BUFSIZ];

void cmd_print_help() {
  char *usage_msg = "Usage: bin/treasure_hub\n\n";

  char *start_msg = "start_monitor -> starts a separate background process "
                    "that monitors the hunts and prints to the standard output "
                    "information about them when asked to\n\n";

  char *stop_msg =
      "stop_monitor -> asks the monitor to end then returns to the prompt. "
      "Prints monitor's termination state when it ends.\n\n";

  char *list_msg1 = "list_hunts -> asks the monitor to list the hunts and the "
                    "total number of treasures in each\n\n";
  char *list_msg2 =
      "list_treasures <hunt_id> -> tells the monitor to show the information "
      "about all treasures in a hunt\n\n";

  char *view_msg = "view_treasure <hunt_id> <treasure_id> -> tells the monitor "
                   "to show the information "
                   "about a treasure in hunt\n\n";

  char *exit_msg = "exit: if the monitor still runs, prints an error message, "
                   "otherwise ends the program\n";

  char help_msg[BUFSIZ];

  snprintf(help_msg, BUFSIZ, "%s%s%s%s%s%s%s", usage_msg, start_msg, stop_msg,
           list_msg1, list_msg2, view_msg, exit_msg);

  write(STDOUT_FILENO, help_msg, strlen(help_msg));
}

shell_command parse_shell_cmd(char *string, char args[MAX_ARGS][BUFSIZ],
                              int *argc) {
  // Clear everything up front
  *argc = 0;
  memset(args, 0, MAX_ARGS * BUFSIZ);

  // Make a local copy of the line so the original buffer is not modified
  char buf[BUFSIZ];
  size_t n = strnlen(string, BUFSIZ - 1);
  memcpy(buf, string, n);
  buf[n] = '\0';

  // Tokenize on space or newline
  char *tok = strtok(buf, " \n");
  if (!tok) {
    return CMD_INVALID;
  }

  // Insert the tokens into the args array
  do {
    strncpy(args[(*argc)++], tok, BUFSIZ - 1);
  } while (*argc < MAX_ARGS && (tok = strtok(NULL, " \n")) != NULL);

  // Map args[0] to an enum
  if (strcmp(args[0], "help") == 0)
    return CMD_HELP;
  else if (strcmp(args[0], "clear") == 0)
    return CMD_CLEAR;
  else if (strcmp(args[0], "start_monitor") == 0)
    return CMD_START_MONITOR;
  else if (strcmp(args[0], "stop_monitor") == 0)
    return CMD_STOP_MONITOR;
  else if (strcmp(args[0], "list_hunts") == 0)
    return CMD_LIST_HUNTS;
  else if (strcmp(args[0], "list_treasures") == 0)
    return CMD_LIST_TREASURES;
  else if (strcmp(args[0], "view_treasure") == 0)
    return CMD_VIEW_TREASURE;
  else if (strcmp(args[0], "exit") == 0)
    return CMD_EXIT;
  else
    return CMD_INVALID;
}

void cmd_clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }
