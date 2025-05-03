#include "manager_args_parser.h"
#include "../manager_cmd/help/help.h"

//=============================================================================
// Operation Management
//=============================================================================

operation read_operation(int argc, char **argv) {
  if (argc < 2)
    return OPERATION_INVALID;

  // HELP
  if (argc == 2 && strcmp(argv[1], "--help") == 0)
    return HELP;

  // ADD
  if (argc == 3 && strcmp(argv[1], "--add") == 0)
    return ADD;

  // REMOVE
  if ((argc == 3 || argc == 4) && strcmp(argv[1], "--remove") == 0)
    return REMOVE;

  // LIST
  if (strcmp(argv[1], "--list") == 0)
    return LIST;

  print_help();
  return OPERATION_INVALID;
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

char *get_hunt_id(int argc, char **argv) { return argc < 3 ? NULL : argv[2]; }

int get_treasure_id(int argc, char **argv) {
  if (argc < 4)
    return -1;
  return atoi(argv[3]);
}
