#ifndef __TREASURE_OPERATIONS_H
#define __TREASURE_OPERATIONS_H

#include "../../../treasure_manager/treasure_manager.h"

operation_error create_treasure(treasure *t, int fd);
void print_treasure(const treasure *t);
int is_void_treasure(const treasure *t);
void get_treasure_string(char *buffer, const treasure *t);
operation_error write_treasure_to_file(const treasure *t, const char *path);
operation_error parse_treasure_data(char *line);

#endif
