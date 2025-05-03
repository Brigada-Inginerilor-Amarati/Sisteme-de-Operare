#include "help.h"

void print_operation_error(operation_error err) {
  switch (err) {
  case DIRECTORY_ERROR:
    write(STDERR_FILENO, "Directory error\n", strlen("Directory error\n"));
    break;
  case FILE_ERROR:
    write(STDERR_FILENO, "File error\n", strlen("File error\n"));
    break;
  case OPERATION_FAILED:
    write(STDERR_FILENO, "Operation failed\n", strlen("Operation failed\n"));
    break;
  case NO_ERROR:
    write(STDERR_FILENO, "No error\n", strlen("No error\n"));
    break;
  case SYMLINK_ERROR:
    write(STDERR_FILENO, "Symlink error\n", strlen("Symlink error\n"));
    break;
  case FILE_NOT_FOUND:
    write(STDERR_FILENO, "File not found\n", strlen("File not found\n"));
    break;
  case TREASURE_NOT_FOUND:
    write(STDERR_FILENO, "Treasure not found\n",
          strlen("Treasure not found\n"));
    break;
  case TREASURE_ALREADY_EXISTS:
    write(STDERR_FILENO, "Treasure already exists\n",
          strlen("Treasure already exists\n"));
    break;
  case DIRECTORY_NOT_FOUND:
    write(STDERR_FILENO, "Directory not found\n",
          strlen("Directory not found\n"));
    break;
  default:
    write(STDERR_FILENO, "Unknown error\n", strlen("Unknown error\n"));
    break;
  }
}

void print_help() {
  char *usage_msg = "Usage: ./treasure_manager <operation> [operands]\n";

  char *list_msg1 = "--list -> Lists all treasure hunts and the number of "
                    "treasures inside each hunt\n";
  char *list_msg2 =
      "--list <hunt_id> -> Lists all treasures inside the specified hunt\n";
  char *list_msg3 = "--list <hunt_id> <treasure_id> -> Lists the specified "
                    "treasure inside the specified hunt\n";

  char *add_msg1 = "--add <hunt_id> <treasure_id> -> Adds the specified "
                   "treasure to the specified hunt\n";

  char *add_msg2 = "If the hunt does not exist, it will be created.\n";

  char *add_msg3 = "If there is a treasure with the same ID in the hunt, an "
                   "error will occur.\n";

  char *remove_msg1 =
      "--remove <hunt_id> -> Removes the entire hunt directory\n";
  char *remove_msg2 = "--remove <hunt_id> <treasure_id> -> Removes the "
                      "specified treasure from the specified hunt\n";
  char *remove_msg3 = "If the hunt does not exist, an error will occur.\n";
  char *remove_msg4 =
      "If the treasure does not exist in the hunt, an error will occur.\n";

  char help_msg[BUFSIZ];

  snprintf(help_msg, BUFSIZ, "%s%s%s%s%s%s%s%s%s%s%s", usage_msg, list_msg1,
           list_msg2, list_msg3, add_msg1, add_msg2, add_msg3, remove_msg1,
           remove_msg2, remove_msg3, remove_msg4);

  write(1, help_msg, strlen(help_msg));
}

void print_operation(operation op) {
  write(1, "Operation: ", 11);
  switch (op) {
  case HELP:
    write(1, "HELP", 4);
    break;
  case ADD:
    write(1, "ADD", 3);
    break;
  case LIST:
    write(1, "LIST", 4);
    break;
  case REMOVE:
    write(1, "REMOVE", 6);
    break;
  default:
    write(1, "INVALID", 7);
    break;
  }
  write(1, "\n", 1);
}
