#include "shell_cmd.h"

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
