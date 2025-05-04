#ifndef __OP_UTILS_H
#define __OP_UTILS_H

#include "../../treasure_manager.h"

// File Operations
operation_error add_file(const char *path);
operation_error directory_exists(const char *path);
operation_error add_directory(const char *path);

// File Metadata Operations
off_t get_treasure_file_size(const char *path);
time_t get_treasure_file_last_modified(const char *path);
operation_error id_exists(const char *path, int id);
int get_treasure_count(const char *path);

// External path variables
extern char dir_path[PATH_MAX];
extern char treasure_file_path[PATH_MAX];
extern char log_file_path[PATH_MAX];
extern char log_msg[BUFSIZ];

// Path Utilities
void prepare_paths(const char *hunt_dir);
void get_root_hunt_directory_path(char *path);
void get_hunt_directory_path(char *path, const char *hunt_name);
void get_treasure_file_path(char *path, const char *hunt_name);
void get_log_file_path(char *path, const char *hunt_name);
void get_symlink_directory_path(char *path);
void get_symlink_path(char *path, const char *hunt_name);

#endif
