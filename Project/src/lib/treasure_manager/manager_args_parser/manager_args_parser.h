#ifndef __MANAGER_ARGS_PARSER_H
#define __MANAGER_ARGS_PARSER_H

#include "../../../treasure_manager/treasure_manager.h"

operation read_operation(int argc, char **argv);
char *get_hunt_id(int argc, char **argv);
int get_treasure_id(int argc, char **argv);
void print_parameters(char *hunt_id, int treasure_id);

#endif
