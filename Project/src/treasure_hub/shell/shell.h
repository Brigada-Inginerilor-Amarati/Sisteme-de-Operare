#ifndef __SHELL_COMMANDS_H
#define __SHELL_COMMANDS_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
//=============================================================================
// Defines & Constants
//=============================================================================

#define DELAY_START_MONITOR 1000000
#define TIME_BUFFER_SIZE 32
#define FIFO_PERMISSIONS                                                       \
  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

#define LIST_HUNTS_ARGC 1
#define LIST_TREASURES_ARGC 2
#define VIEW_TREASURE_ARGC 3
#define MAX_ARGS 10

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
  CMD_VIEW_TREASURE
} shell_command;

typedef enum { OPERATION_FAILED, OPERATION_SUCCESS } operation_error;

typedef struct {
  pid_t monitor_pid;
  enum { MON_STOPPED, MON_RUNNING, MON_STOPPING } state;
} shell_t;

//=============================================================================
// External Variables
//=============================================================================
extern shell_t shell;

#endif
