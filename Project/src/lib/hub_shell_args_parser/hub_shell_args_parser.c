#include "hub_shell_args_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pid_t monitor_pid;
int monitor_running;

void print_help_info() {
  char *usage_msg = "\nUsage: bin/treasure_hub\n\n";

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
                   "otherwise ends the program\n\n";

  char help_msg[BUFSIZ];

  snprintf(help_msg, BUFSIZ, "%s%s%s%s%s%s%s", usage_msg, start_msg, stop_msg,
           list_msg1, list_msg2, view_msg, exit_msg);

  write(1, help_msg, strlen(help_msg));
}

shell_command parse_shell_cmd(char *string, char args[][BUFSIZ]) {
  int argc = 0;

  // init the args array

  memset(args, 0, MAX_ARGS * BUFSIZ);

  char *token = strtok(string, " \n");
  if (!token)
    return CMD_INVALID;

  shell_command cmd = CMD_INVALID;

  if (strcmp(token, "help") == 0)
    cmd = CMD_HELP;
  else if (strcmp(token, "clear") == 0)
    cmd = CMD_CLEAR;
  else if (strcmp(token, "start_monitor") == 0)
    cmd = CMD_START_MONITOR;
  else if (strcmp(token, "stop_monitor") == 0)
    cmd = CMD_STOP_MONITOR;
  else if (strcmp(token, "list_hunts") == 0)
    cmd = CMD_LIST_HUNTS;
  else if (strcmp(token, "list_treasures") == 0)
    cmd = CMD_LIST_TREASURES;
  else if (strcmp(token, "view_treasure") == 0)
    cmd = CMD_VIEW_TREASURE;
  else if (strcmp(token, "exit") == 0)
    cmd = CMD_EXIT;
  else
    cmd = CMD_INVALID;

  if (cmd == CMD_INVALID)
    return CMD_INVALID;

  strcpy(args[(argc)++], token); // Store the command itself

  // Collect additional arguments
  while ((token = strtok(NULL, " \n")) != NULL && argc < 3) {
    strcpy(args[argc], token);
    argc++;
  }

  return cmd;
}

void print_command(shell_command cmd) {
  switch (cmd) {
  case CMD_HELP:
    write(STDOUT_FILENO, "help: prints the available commands\n",
          strlen("help: prints the available commands\n"));
    break;
  case CMD_START_MONITOR:
    write(STDOUT_FILENO, "start_monitor: starts the monitor\n",
          strlen("start_monitor: starts the monitor\n"));
    break;
  case CMD_STOP_MONITOR:
    write(STDOUT_FILENO, "stop_monitor: stops the monitor\n",
          strlen("stop_monitor: stops the monitor\n"));
    break;
  case CMD_LIST_HUNTS:
    write(STDOUT_FILENO, "list_hunts: lists the hunts\n",
          strlen("list_hunts: lists the hunts\n"));
    break;
  case CMD_LIST_TREASURES:
    write(STDOUT_FILENO, "list_treasures: lists the treasures\n",
          strlen("list_treasures: lists the treasures\n"));
    break;
  case CMD_VIEW_TREASURE:
    write(STDOUT_FILENO, "view_treasure: views a treasure\n",
          strlen("view_treasure: views a treasure\n"));
    break;
  case CMD_EXIT:
    write(STDOUT_FILENO, "exit: exits the program\n",
          strlen("exit: exits the program\n"));
    break;
  default:
    write(STDOUT_FILENO, "Invalid command\n", strlen("Invalid command\n"));
    break;
  }
}

void clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }

void exit_shell() {
  if (monitor_running) {
    perror("Cannot exit while monitor is running. Use stop_monitor first.");
    return;
  }

  write(STDOUT_FILENO, "Exiting treasure_hub shell\n",
        strlen("Exiting treasure_hub shell\n"));

  exit(0);
}
