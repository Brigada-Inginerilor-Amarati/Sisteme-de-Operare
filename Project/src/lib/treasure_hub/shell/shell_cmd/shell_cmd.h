#ifndef __SHELL_CMD_H__
#define __SHELL_CMD_H__
#include "../../../../treasure_hub/shell/shell.h"

operation_error dispatch_command(shell_command cmd, char argv[][BUFSIZ],
                                 int argc);

#endif
