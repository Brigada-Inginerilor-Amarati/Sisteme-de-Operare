#include "treasure_manager.h"
#include "../lib/treasure_manager/manager_args_parser/manager_args_parser.h"
#include "../lib/treasure_manager/manager_cmd/operations.h"
#include "../lib/treasure_manager/treasure_operations/treasure_operations.h"

operation_error execute_operation(int argc, char *argv[]) {
  operation op = read_operation(argc, argv);
  if (op == OPERATION_INVALID)
    return OPERATION_FAILED;

  char *hunt_id = get_hunt_id(argc, argv);
  int treasure_id = get_treasure_id(argc, argv);

  switch (op) {
  case HELP:
    print_help();
    return NO_ERROR;
  case ADD:
    return add_treasure(hunt_id, STDIN_FILENO);
    break;
  case LIST:
    if (hunt_id == NULL)
      return list_hunts();
    else if (treasure_id == -1)
      return list_hunt(hunt_id);
    else
      return list_treasure(hunt_id, treasure_id);
  case REMOVE:
    if (treasure_id == -1)
      return remove_hunt(hunt_id);
    else
      return remove_treasure(hunt_id, treasure_id);
  default:
    print_help();
    exit(-1);
  }
}

int main(int argc, char *argv[]) {
  execute_operation(argc, argv);
  return 0;
}
