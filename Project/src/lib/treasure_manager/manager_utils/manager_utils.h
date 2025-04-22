#ifndef __OP_UTILS_H
#define __OP_UTILS_H

#include "../../../treasure_manager/treasure_manager.h"

operation_error add_file(const char *path);
operation_error directory_exists(const char *path);
operation_error add_directory(char *path);
off_t get_treasure_file_size(const char *path);
time_t get_treasure_file_last_modified(const char *path);
operation_error id_exists(char *path, int id);
int get_treasure_count(char *path);

#endif
