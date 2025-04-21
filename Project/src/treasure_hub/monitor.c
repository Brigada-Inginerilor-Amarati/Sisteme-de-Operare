#include "../lib/utils/utils.h"
#include "shell.h"

//=============================================================================
// Defines & Constants
//=============================================================================
#define LINE_BUF_SIZE 256
const char *MANAGER_BIN = "bin/treasure_manager";
//=============================================================================
// Global State
//=============================================================================
volatile sig_atomic_t running = 1;
char line_buf[LINE_BUF_SIZE];

//=============================================================================
// Forward Declarations
//=============================================================================
void handle_sigterm(int signum);
void setup_signal_handlers(void);
ssize_t read_command(void);
void debug_dump(ssize_t n);
void execute_command(const char *cmd);
void read_and_execute(void);
void monitor_loop(void);

//=============================================================================
// Signal Handlers
//=============================================================================
void handle_sigterm(int signum) {
  // Shutdown signal received
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

//=============================================================================
// Command Reading & Execution
//=============================================================================
ssize_t read_command(void) {
  return read_line(STDIN_FILENO, line_buf, LINE_BUF_SIZE);
}

void debug_dump(ssize_t n) {
  dprintf(STDERR_FILENO, "[DEBUG] read_line → %zd bytes: \"%.*s\"\n", n, (int)n,
          line_buf);
}

//=============================================================================
// Execute the manager binary with the given command string
//=============================================================================
void execute_manager(const char *cmd) {
  // Fork and exec "sh -c <cmd>"
  pid_t pid = fork();
  if (pid < 0) {
    dprintf(STDERR_FILENO, "[MONITOR] fork failed: %s\n", strerror(errno));
    return;
  }
  if (pid == 0) {
    // In child, execute the manager
    execlp("sh", "sh", "-c", cmd, (char *)NULL);
    dprintf(STDERR_FILENO, "[MONITOR] exec failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // Parent waits for the child
  waitpid(pid, NULL, 0);

  // refresh the terminal
  kill(getppid(), SIGUSR1);
}

//=============================================================================
// Monitor Main Loop
//=============================================================================
void monitor_loop(void) {
  while (running) {
    ssize_t n = read_line(STDIN_FILENO, line_buf, LINE_BUF_SIZE);
    if (n <= 0) {
      perror("monitor: read_line");
      break;
    }
    // Strip newline
    line_buf[strcspn(line_buf, "\r\n")] = '\0';

    write(STDOUT_FILENO, "\r\033[K", 4);

    // Execute via the manager
    execute_manager(line_buf);

    write(STDOUT_FILENO, "\n", 1);
  }
}

//=============================================================================
// main()
//=============================================================================
int main(void) {
  // Install signal handlers
  setup_signal_handlers();

  // Announce startup
  dprintf(STDOUT_FILENO, "[MONITOR] Monitor started (PID: %d)\n", getpid());

  // Enter main loop
  monitor_loop();

  sleep(1);

  dprintf(STDOUT_FILENO, "[MONITOR] Shutting down.\n");
  return EXIT_SUCCESS;
}
