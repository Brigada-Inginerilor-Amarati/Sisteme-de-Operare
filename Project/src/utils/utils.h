#ifndef __OP_UTILS_H
#define __OP_UTILS_H

#include "../main/treasure_manager.h"

ssize_t read_line(int fd, char *buf, size_t max_len);
operation_error add_file(const char *path);
operation_error directory_exists(const char *path);
operation_error add_directory(char *path);
off_t get_directory_total_size(const char *dir_path);
time_t get_directory_last_modified_time(const char *dir_path);
int extract_id(char *string);
operation_error id_exists(char *dir_path, int id);

#endif
