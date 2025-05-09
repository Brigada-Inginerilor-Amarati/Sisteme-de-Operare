#ifndef __MONITOR_CONTROL_H__
#define __MONITOR_CONTROL_H__

#include "../../shell.h"

int is_monitor_alive(void);
operation_error send_to_monitor(const char *cmd_line);
operation_error cmd_start_monitor(void);
operation_error cmd_stop_monitor(void);
int check_monitor_stopping(void);

#endif
