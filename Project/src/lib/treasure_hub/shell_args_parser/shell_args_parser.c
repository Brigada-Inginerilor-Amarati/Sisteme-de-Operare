#include "shell_args_parser.h"

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
