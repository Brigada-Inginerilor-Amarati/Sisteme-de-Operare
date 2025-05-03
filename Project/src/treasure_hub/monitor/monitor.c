#include "monitor.h"
#include "../../lib/global_utils/global_utils.h"
#include "lib_monitor/monitor_exec/monitor_exec.h"
#include "lib_monitor/monitor_signals/monitor_signals.h"

//=============================================================================
// Global State
//=============================================================================
volatile sig_atomic_t running = 1;

//=============================================================================
// Monitor Main Loop
//=============================================================================
void monitor_loop(void) {

  char buf[BUFSIZ];

  while (running) {
    // Read data from STDIN (a pipe from the manager)
    ssize_t n = read_line(STDIN_FILENO, buf, BUFSIZ);
    if (n <= 0)
      break;

    // Reset the prompt
    write(STDOUT_FILENO, "\r\033[K", 4);

    // Execute via the manager
    execute_manager(buf);
  }
}

//=============================================================================
// main()
//=============================================================================
int main(void) {

  char msg[BUFSIZ];

  // Install signal handlers
  setup_signal_handlers();

  // Announce startup
  snprintf(msg, BUFSIZ, "[MONITOR] Monitor started (PID: %d)\n", getpid());
  write(STDOUT_FILENO, msg, strlen(msg));

  // Enter main loop
  monitor_loop();

  // Simulate prolonged shutdown
  sleep(5);

  // Announce shutdown
  snprintf(msg, BUFSIZ, "[MONITOR] Shutting down.\n");
  write(STDOUT_FILENO, msg, strlen(msg));
  return EXIT_SUCCESS;
}
