#ifndef __SHELL_COMMANDS_H
#define __SHELL_COMMANDS_H
#include <sys/types.h>
#include <sys/wait.h>

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

typedef enum { MON_STOPPED = 0, MON_RUNNING, MON_STOPPING } monitor_state;

#define MAX_ARGS 10
extern pid_t monitor_pid;
extern monitor_state mon_state;

#endif
