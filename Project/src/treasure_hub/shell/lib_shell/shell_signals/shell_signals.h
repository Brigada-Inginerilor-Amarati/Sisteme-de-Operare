#ifndef __SHELL_SIGNALS_H__
#define __SHELL_SIGNALS_H__

#include "../../shell.h"

void sig_refresh_handler(int sig);
void sigchld_handler(int sig);
void setup_signal_handlers(void);

#endif
