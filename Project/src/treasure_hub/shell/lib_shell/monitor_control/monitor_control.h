#ifndef __MONITOR_CONTROL_H__
#define __MONITOR_CONTROL_H__

#include "../../shell.h"

int is_monitor_alive(void);
int send_to_monitor(const char *cmd_line);
int cmd_start_monitor(void);
int cmd_stop_monitor(void);
int check_monitor_stopping(void);

#endif
