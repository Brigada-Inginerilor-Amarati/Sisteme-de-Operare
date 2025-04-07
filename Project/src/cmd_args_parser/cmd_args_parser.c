#include "cmd_args_parser.h"
#include <string.h>
#include <unistd.h>

void print_error() {
  char *err_msg = "Usage: ./treasure_manager <operation> [operands]\nAdd "
                  "--help for more information.";
  write(2, err_msg, strlen(err_msg));
}

void print_help() {
  char *help_msg =
      "Usage: ./treasure_manager <operation> [operands]\n\t--add <hunt_id> - "
      "to add a treasure to the "
      "selected hunt.\n\t--list <hunt_id> - to list all treasures within the "
      "selected hunt.\n\t--list <hunt_id> <treasure_id> - to list a specific "
      "treasure within the selected hunt.\n\t--remove <hunt_id> <treasure_id> "
      "- to remove a specific treasure from the selected hunt.\n\t--remove "
      "<hunt_id> - to remove all treasures from the selected hunt.";
  write(1, help_msg, strlen(help_msg));
}

char *get_hunt_id(int argc, char **argv) { return argc < 3 ? NULL : argv[2]; }

char *get_treasure_id(int argc, char **argv) {
  return argc < 4 ? NULL : argv[3];
}

operation read_operation(int argc, char **argv) {
  if (argc < 2) {
    print_error();
    return OPERATION_INVALID;
  }

  // HELP
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    print_help();
    return HELP;
  }

  // ADD
  if (argc == 3 && strcmp(argv[1], "--add") == 0)
    return ADD;

  // LIST
  if ((argc == 3 || argc == 4) && strcmp(argv[1], "--list") == 0)
    return LIST;

  // REMOVE
  if ((argc == 3 || argc == 4) && strcmp(argv[1], "--remove") == 0)
    return REMOVE;

  print_error();
  return OPERATION_INVALID;
}
