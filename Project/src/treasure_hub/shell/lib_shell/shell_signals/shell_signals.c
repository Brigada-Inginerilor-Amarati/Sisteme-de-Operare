#include "shell_signals.h"
#include "../shell_utils/shell_utils.h"

//=============================================================================
// Signal Handling -> SIGUSR1 from Monitor and Zombie Prevention
//=============================================================================

void sig_refresh_handler(int sig) {
  // signal to activate the stdin in the shell
  write(STDOUT_FILENO, "\r\033[K", 4);
}

void sigchld_handler(int sig) {
  char log_msg[BUFSIZ];
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if (pid == shell.monitor_pid && shell.state == MON_SHUTTING_DOWN) {
      if (WIFEXITED(status)) {
        snprintf(log_msg, BUFSIZ, "[✓] Monitor exited (status=%d)\n",
                 WEXITSTATUS(status));
      } else if (WIFSIGNALED(status)) {
        snprintf(log_msg, BUFSIZ, "[!] Monitor died by signal %d\n",
                 WTERMSIG(status));
      }
      write(STDOUT_FILENO, log_msg, strlen(log_msg));

      shell.state = MON_OFFLINE;
      shell.monitor_pid = -1;
      tcflush(STDIN_FILENO, TCIFLUSH);
      refresh_prompt();
    }
  }
}

void setup_signal_handlers(void) {
  struct sigaction sa_child = {.sa_handler = sigchld_handler,
                               .sa_flags = SA_RESTART | SA_NOCLDSTOP};
  sigaction(SIGCHLD, &sa_child, NULL);

  struct sigaction sa_refresh = {.sa_handler = sig_refresh_handler,
                                 .sa_flags = 0};
  sigaction(SIGUSR1, &sa_refresh, NULL);
}
