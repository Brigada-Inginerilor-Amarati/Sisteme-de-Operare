#ifndef __TREASURE_H
#define __TREASURE_H

#include "../../treasure_manager/treasure_manager.h"

treasure create_treasure(int fd);
void print_treasure(const treasure *t);
int is_void_treasure(const treasure *t);
void get_treasure_string(char *buffer, const treasure *t);
operation_error write_treasure_to_file(const treasure *t, const char *dir_name);

#endif
