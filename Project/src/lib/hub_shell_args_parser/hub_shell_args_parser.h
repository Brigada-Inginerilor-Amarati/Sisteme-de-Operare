#ifndef __HUB_SHELL_ARGS_PARSER_H
#define __HUB_SHELL_ARGS_PARSER_H

#include "../../treasure_hub/shell.h"
#include <stdio.h>

shell_command parse_shell_cmd(char *string, char args[][BUFSIZ], int *argc);
void cmd_print_help();
void cmd_clear_screen();

#endif
