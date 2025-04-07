#ifndef __CMD_ARGS_PARSER_H
#define __CMD_ARGS_PARSER_H

#include "../operations/operations.h"

operation read_operation(int argc, char **argv);
char *get_hunt_id(int argc, char **argv);
char *get_treasure_id(int argc, char **argv);
void print_parameters(char *hunt_id, char *treasure_id);
void print_operation(operation op);

#endif
