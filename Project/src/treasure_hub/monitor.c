#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;
char log_msg[BUFSIZ];

void handle_shutdown(int signum) {
  snprintf(log_msg, BUFSIZ, "\n[MONITOR] %s (%d)\n", "Received shutdown signal",
           signum);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  running = 0;
}

void handle_execute_command(int signum) {
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s (%d)\n", "Received execute command",
           signum);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
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

  // Handle execute command signal
  struct sigaction sa_exec = {0};
  sa_exec.sa_handler = handle_execute_command;
  sigaction(SIGUSR1, &sa_exec, NULL);
}

int main() {
  setup_signal_handlers();
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s (PID: %d)\n", "Monitor started",
           getpid());
  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  running = 1;

  // Main loop waiting for signals
  while (running) {
    pause();
  }

  sleep(2);

  // Shutdown message
  snprintf(log_msg, BUFSIZ, "[MONITOR] %s\n", "Shutting down.");
  write(STDOUT_FILENO, log_msg, strlen(log_msg));
  return 0;
}
