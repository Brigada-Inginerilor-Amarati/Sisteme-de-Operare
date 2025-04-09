#include "cmd_args_parser.h"
#include <stdio.h>
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
      "<hunt_id> - to remove all treasures from the selected hunt.\n";
  write(1, help_msg, strlen(help_msg));
}

void print_parameters(char *hunt_id, int treasure_id) {
  write(1, "Hunt ID: ", strlen("Hunt ID: "));

  if (hunt_id != NULL)
    write(1, hunt_id, strlen(hunt_id));
  else
    write(1, "NULL", 4);

  write(1, "\nTreasure ID: ", strlen("\nTreasure ID: "));

  if (treasure_id != -1) {
    char treasure_id_str[10];
    sprintf(treasure_id_str, "%d", treasure_id);
    write(1, treasure_id_str, strlen(treasure_id_str));
  } else
    write(1, "NULL", 4);

  write(1, "\n", 1);
}

void print_operation(operation op) {
  write(1, "Operation: ", 11);
  switch (op) {
  case HELP:
    write(1, "DELETE", 6);
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

char *get_hunt_id(int argc, char **argv) { return argc < 3 ? NULL : argv[2]; }

int get_treasure_id(int argc, char **argv) {
  if (argc < 4)
    return -1;
  return atoi(argv[3]);
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
