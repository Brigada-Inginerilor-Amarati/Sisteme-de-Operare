#include "cmd_args_parser/cmd_args_parser.h"
#include "operations/operations.h"
#include "treasure/treasure.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int test_op(int argc, char *argv[]) {
  operation op = read_operation(argc, argv);

  if (op == OPERATION_INVALID)
    return -1;

  char *hunt_id = get_hunt_id(argc, argv);
  char *treasure_id = get_treasure_id(argc, argv);

  print_operation(op);
  print_parameters(hunt_id, treasure_id);

  return 0;
}

int main(int argc, char *argv[]) {

  treasure t = create_treasure();

  if (!is_void_treasure(&t))
    print_treasure(&t);
  else {
    write(STDOUT_FILENO, "Void treasure.\n", strlen("Void treasure.\n"));
  }

  return 0;
}
