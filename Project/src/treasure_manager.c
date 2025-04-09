#include "treasure_manager.h"
#include "cmd_args_parser/cmd_args_parser.h"
#include "log/log.h"
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

void test_add(int argc, char *argv[]) {
  operation op = read_operation(argc, argv);

  if (op == OPERATION_INVALID)
    return;

  char *hunt_id = get_hunt_id(argc, argv);
  char *treasure_id = get_treasure_id(argc, argv);

  print_operation(op);
  print_parameters(hunt_id, treasure_id);
}

int main(int argc, char *argv[]) {

  operation_error err = log_message("hunt01", "hello");
  print_operation_error(err);
  err = log_message("hunt02", "world");
  print_operation_error(err);

  return 0;
}
