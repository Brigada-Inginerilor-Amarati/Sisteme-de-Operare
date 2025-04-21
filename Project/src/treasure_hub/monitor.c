#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;
char log_msg[BUFSIZ];

void handle_shutdown(int signum) {
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s%d)\n", "Received shutdown signal (",
           signum);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  running = 0;
}

void setup_signal_handlers() {
  struct sigaction sa = {0};
  sa.sa_handler = handle_shutdown;
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGHUP, &sa, NULL);

  // Prevent zombies (if you ever fork/exec)
  struct sigaction sa_chld = {0};
  sa_chld.sa_handler = SIG_IGN;
  sa_chld.sa_flags = SA_NOCLDWAIT;
  sigaction(SIGCHLD, &sa_chld, NULL);
}

int main() {
  setup_signal_handlers();
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s%d)\n",
           "Monitor started (PID: ", getpid());
  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  // Main loop waiting for signals
  while (running) {
    pause();
  }

  // Shutdown message
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s\n", "Shutting down.");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 0;
}
