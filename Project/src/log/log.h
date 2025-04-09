#ifndef __LOG_H
#define __LOG_H

#include "../treasure_manager.h"

int symlink_file(const char *path);
operation_error log_message(const char *dir_name, const char *message);

void get_add_success_log_message(char *msg, treasure *t);
void get_add_failure_log_message(char *msg, treasure *t);
void get_add_killed_log_message(char *msg, treasure *t);

void get_search_success_log_message(char *msg, int id);
void get_search_failure_log_message(char *msg, int id);
void get_search_killed_log_message(char *msg, int id);

void get_list_failure_log_message(char *msg, char *path);
void get_list_success_log_message(char *msg, char *path);
#endif
