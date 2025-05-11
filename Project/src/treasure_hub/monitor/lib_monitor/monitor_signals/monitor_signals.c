#include "monitor_signals.h"
#include <unistd.h>

//=============================================================================
// Signal Handling -> Monitor Shutdown and Zombie Prevention
//=============================================================================

void handle_sigterm(int signum) {
  char msg[BUFSIZ];
  write(STDERR_FILENO, "\r\033[K", 4);
  snprintf(msg, sizeof(msg), "[MONITOR] Received shutdown signal (%d)\n",
           signum);
  write(STDERR_FILENO, msg, strlen(msg));

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
