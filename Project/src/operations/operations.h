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
  DIRECTORY_ERROR,
  DATA_FILE_ERROR,
  OPERATION_FAILED,
} operation_error;

#endif
