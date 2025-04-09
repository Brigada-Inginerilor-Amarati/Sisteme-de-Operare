// ADD, LIST, DELETE
#ifndef __TREASURE_MANAGER_OPERATIONS_H
#define __TREASURE_MANAGER_OPERATIONS_H

#include "../treasure/treasure.h"
#include "../treasure_manager.h"

// OP_COUNT is always the last element in the enum, and it's used to determine
// the number of operations.

operation_error add_treasure(char *path);
void print_operation_error(operation_error err);

#endif
