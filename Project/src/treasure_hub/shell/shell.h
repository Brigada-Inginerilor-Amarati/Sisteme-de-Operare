#ifndef __SHELL_COMMANDS_H
#define __SHELL_COMMANDS_H

//=============================================================================
// Dependencies
//=============================================================================

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

//=============================================================================
// Defines & Constants
//=============================================================================

#define DELAY_START_MONITOR 1000000
#define TIME_BUFFER_SIZE 32

#define DEFAULT_ARGC 1
#define LIST_HUNTS_ARGC 1
#define LIST_TREASURES_ARGC 2
#define VIEW_TREASURE_ARGC 3
#define MAX_ARGS 10

#define MANAGER_CMD_PATH "./bin/treasure_manager"
#define LIST_CMD "--list"
#define MONITOR_CMD_PATH "./bin/treasure_monitor"
#define MONITOR_CMD_NAME "treasure_monitor"

#define TREASURE_FILE_NAME "treasures.bin"
#define TREASURE_DIRECTORY "treasure_hunts"
#define CALCULATOR_CMD_PATH "./bin/treasure_calculator"
#define CALCULATOR_CMD_NAME "treasure_calculator"

#define COMMAND_HELP "help"
#define COMMAND_EXIT "exit"
#define COMMAND_CLEAR "clear"
#define COMMAND_START_MONITOR "start_monitor"
#define COMMAND_STOP_MONITOR "stop_monitor"
#define COMMAND_LIST_HUNTS "list_hunts"
#define COMMAND_LIST_TREASURES "list_treasures"
#define COMMAND_VIEW_TREASURE "view_treasure"
#define COMMAND_CALCULATE_SCORES "cs"
//=============================================================================
// Structs
//=============================================================================

typedef enum {
  CMD_INVALID,
  CMD_CLEAR,
  CMD_HELP,
  CMD_EXIT,
  CMD_START_MONITOR,
  CMD_STOP_MONITOR,
  CMD_LIST_HUNTS,
  CMD_LIST_TREASURES,
  CMD_VIEW_TREASURE,
  CMD_CALCULATE_SCORES
} shell_command;

typedef enum { OPERATION_FAILED, OPERATION_SUCCESS } operation_error;

typedef struct {
  pid_t monitor_pid;
  enum { MON_OFFLINE, MON_ONLINE, MON_SHUTTING_DOWN } state;
  int monitor_pipe_fd;
} shell_t;

//=============================================================================
// External Variables
//=============================================================================

extern shell_t shell;

#endif
