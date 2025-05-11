#ifndef __SHELL_CMD_H__
#define __SHELL_CMD_H__
#include "../../shell.h"

int dispatch_command(shell_command cmd, char argv[][BUFSIZ], int argc);

int cmd_list(shell_command cmd, char argv[][BUFSIZ], int argc);
int check_argc_valid(shell_command cmd, int argc);
int send_to_monitor(const char *cmd_line);

#endif
