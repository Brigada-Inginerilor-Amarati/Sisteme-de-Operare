#ifndef __OP_LIST_H
#define __OP_LIST_H

#include "../../../treasure_manager.h"

operation_error list_hunt(const char *path);
operation_error list_treasure(const char *path, int id);
operation_error list_hunts();

#endif
