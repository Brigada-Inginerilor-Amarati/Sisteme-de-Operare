#ifndef __SHELL_ARGS_PARSER_H
#define __SHELL_ARGS_PARSER_H

#include "../../../../treasure_hub/shell/shell.h"

shell_command parse_shell_cmd(char *string, char args[MAX_ARGS][BUFSIZ],
                              int *argc);

#endif
