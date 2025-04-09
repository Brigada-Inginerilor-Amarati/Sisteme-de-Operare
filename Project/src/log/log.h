#ifndef __LOG_H
#define __LOG_H

#include "../treasure_manager.h"

int symlink_file(const char *path);
operation_error log_message(const char *dir_name, const char *message);
void get_invalid_input_log_message(char *msg, treasure *t);
void get_success_log_message(char *msg, treasure *t);
void get_file_error_log_message(char *msg, treasure *t);

#endif
