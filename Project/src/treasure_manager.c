#include "cmd_args_parser/cmd_args_parser.h"
#include "operations/operations.h"
#include "treasure.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_parameters(char *hunt_id, char *treasure_id) {
  write(1, "Hunt ID: ", strlen("Hunt ID: "));
  if (hunt_id != NULL)
    write(1, hunt_id, strlen(hunt_id));
  else
    write(1, "NULL", 4);
  write(1, "\n", 1);
  write(1, "Treasure ID: ", strlen("Treasure ID: "));
  if (treasure_id != NULL)
    write(1, treasure_id, strlen(treasure_id));
  else
    write(1, "NULL", 4);
  write(1, "\n", 1);
}

void print_operation(operation op) {
  write(1, "Operation: ", strlen("Operation: "));
  switch (op) {
  case HELP:
    write(1, "DELETE", strlen("DELETE"));
    break;
  case ADD:
    write(1, "CREATE", strlen("CREATE"));
    break;
  case LIST:
    write(1, "READ", strlen("READ"));
    break;
  case REMOVE:
    write(1, "UPDATE", strlen("UPDATE"));
    break;
  default:
    write(1, "INVALID", strlen("INVALID"));
    break;
  }
  write(1, "\n", 1);
}

int main(int argc, char *argv[]) {
  operation op = read_operation(argc, argv);

  if (op == OPERATION_INVALID)
    return -1;

  char *hunt_id = get_hunt_id(argc, argv);
  char *treasure_id = get_treasure_id(argc, argv);

  print_operation(op);
  print_parameters(hunt_id, treasure_id);

  return 0;
}
