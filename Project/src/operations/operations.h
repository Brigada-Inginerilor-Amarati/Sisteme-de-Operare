// ADD, LIST, DELETE
#ifndef __TREASURE_MANAGER_OPERATIONS_H
#define __TREASURE_MANAGER_OPERATIONS_H

#include "../treasure/treasure.h"

// OP_COUNT is always the last element in the enum, and it's used to determine
// the number of operations.

typedef enum {
  OPERATION_INVALID = -1,
  HELP,
  ADD,
  LIST,
  REMOVE,
  OP_COUNT
} operation;

typedef enum {
  NO_ERROR = 1,
  DIRECTORY_NOT_FOUND,
  DIRECTORY_ERROR,
  FILE_NOT_FOUND,
  FILE_ERROR,
  SYMLINK_ERROR,
  OPERATION_FAILED,
} operation_error;

operation_error add_treasure(char *path);
void print_operation_error(operation_error err);

#endif
