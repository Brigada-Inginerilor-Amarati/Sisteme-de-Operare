// ADD, LIST, DELETE
#ifndef __TREASURE_MANAGER_OPERATIONS_H
#define __TREASURE_MANAGER_OPERATIONS_H

#include "../treasure/treasure.h"
#include "../treasure_manager.h"

// OP_COUNT is always the last element in the enum, and it's used to determine
// the number of operations.

operation_error add_treasure(char *path);

operation_error list_hunt(char *path);
operation_error list_treasure(char *path, int id);

operation_error remove_hunt(char *path);
operation_error remove_treasure(char *path, int id);

void print_operation_error(operation_error err);

#endif
