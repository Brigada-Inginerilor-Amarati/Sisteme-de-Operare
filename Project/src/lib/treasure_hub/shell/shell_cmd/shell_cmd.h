#ifndef __SHELL_CMD_H__
#define __SHELL_CMD_H__
#include "../../../../treasure_hub/shell/shell.h"

operation_error dispatch_command(shell_command cmd, char argv[][BUFSIZ],
                                 int argc);

operation_error cmd_list(shell_command cmd, char argv[][BUFSIZ], int argc);
int check_argc_valid(shell_command cmd, int argc);
operation_error send_to_monitor(const char *cmd_line);

#endif
