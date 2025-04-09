#ifndef __LOG_H
#define __LOG_H

#include "../treasure_manager.h"

int symlink_file(const char *path);
operation_error log_message(const char *dir_name, const char *message);

#endif
