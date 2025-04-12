// ADD, LIST, DELETE
#ifndef __TREASURE_MANAGER_OPERATIONS_H
#define __TREASURE_MANAGER_OPERATIONS_H

#include "add/op_add.h"
#include "list/op_list.h"
#include "remove/op_remove.h"

operation_error add_treasure(char *path, int fd);

operation_error list_hunt(char *path);
operation_error list_treasure(char *path, int id);

operation_error remove_hunt(char *path);
operation_error remove_treasure(char *path, int id);

void print_operation_error(operation_error err);

#endif
