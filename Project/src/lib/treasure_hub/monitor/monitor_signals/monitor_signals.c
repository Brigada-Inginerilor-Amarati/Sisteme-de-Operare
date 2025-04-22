#include "monitor_signals.h"

//=============================================================================
// Signal Handlers
//=============================================================================
void handle_sigterm(int signum) {
  // Shutdown signal received

  // go to the previous line
  write(STDOUT_FILENO, "\r\033[K", 4);

  dprintf(STDOUT_FILENO, "[MONITOR] Received shutdown signal (%d)\n", signum);
  running = 0;
}

void setup_signal_handlers(void) {
  struct sigaction sa_term = {0};
  struct sigaction sa_child = {0};

  // Shutdown signals
  sa_term.sa_handler = handle_sigterm;
  sigaction(SIGTERM, &sa_term, NULL);
  sigaction(SIGINT, &sa_term, NULL);
  sigaction(SIGHUP, &sa_term, NULL);

  // Prevent child zombies
  sa_child.sa_handler = SIG_IGN;
  sa_child.sa_flags = SA_NOCLDWAIT;
  sigaction(SIGCHLD, &sa_child, NULL);
}
