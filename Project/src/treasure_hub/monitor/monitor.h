#ifndef __MONITOR_H
#define __MONITOR_H

//=============================================================================
// Dependencies
//=============================================================================

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

//=============================================================================
// Defines and Constants
//=============================================================================
#define MANAGER_BIN "bin/treasure_manager"

//=============================================================================
// Global Variables
//=============================================================================

extern volatile sig_atomic_t running;

#endif
