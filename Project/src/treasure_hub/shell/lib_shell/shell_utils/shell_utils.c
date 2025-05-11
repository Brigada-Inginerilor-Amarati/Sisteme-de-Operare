#include "shell_utils.h"
#include "../monitor_control/monitor_control.h"
char log_msg[BUFSIZ];

//=============================================================================
// Prompt & Input Helpers
//=============================================================================

void refresh_prompt(void) {
  char time_buf[TIME_BUFFER_SIZE];
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);

  strftime(time_buf, sizeof(time_buf), "[%Y-%m-%d %H:%M:%S]", tm_info);
  const char *status = NULL;

  switch (monitor.state) {
  case MONITOR_ONLINE:
    status = "online";
    break;
  case MONITOR_OFFLINE:
    status = "offline";
    break;
  case MONITOR_SHUTTING_DOWN:
    status = "shutting down";
    break;
  }

  snprintf(log_msg, BUFSIZ, "\n%s treasure_hub [monitor: %s] $ ", time_buf,
           status);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

void cmd_clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }

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

  char *compute =
      "calculate_scores -> starts a process for every hunt "
      "directory and prints out the scores of users for each hunt\n\n";

  char *exit_msg =
      "exit -> if the monitor still runs, prints an error message, "
      "otherwise ends the program\n";

  snprintf(log_msg, BUFSIZ, "%s%s%s%s%s%s%s%s", usage_msg, start_msg, stop_msg,
           list_msg1, list_msg2, view_msg, compute, exit_msg);

  write(STDOUT_FILENO, log_msg, strlen(log_msg));
}

//=============================================================================
// Exit Shell
//=============================================================================

void cmd_exit_shell() {
  if (is_monitor_alive()) {
    snprintf(
        log_msg, BUFSIZ,
        "Cannot exit while monitor is running. Use 'stop_monitor' first.\n");
    write(STDOUT_FILENO, log_msg, strlen(log_msg));
    return;
  }

  snprintf(log_msg, BUFSIZ, "Exiting treasure_hub shell\n");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  exit(EXIT_SUCCESS);
}
