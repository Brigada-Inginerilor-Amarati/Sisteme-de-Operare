#ifndef __HUB_SHELL_ARGS_PARSER_H
#define __HUB_SHELL_ARGS_PARSER_H

#include "../../treasure_hub/shell.h"
#include <stdio.h>

shell_command parse_shell_cmd(char *string, char args[][BUFSIZ]);
void print_help_info();
void clear_screen();
void exit_shell();

#endif
