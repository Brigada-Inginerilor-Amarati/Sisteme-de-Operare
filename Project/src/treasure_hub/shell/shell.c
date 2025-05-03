#include "shell.h"
#include "../../lib/utils/utils.h"
#include "lib_shell/shell_args_parser/shell_args_parser.h"
#include "lib_shell/shell_cmd/shell_cmd.h"
#include "lib_shell/shell_signals/shell_signals.h"
#include "lib_shell/shell_utils/shell_utils.h"

// init shell state
shell_t shell = {
    .monitor_pid = -1,
    .state = MON_STOPPED,
    .monitor_pipe_fd = -1,
};

//=============================================================================
// REPL (Read, Evaluate, Print, Loop)
//=============================================================================

int run_repl(void) {

  // init inputs and args
  char log_msg[BUFSIZ];
  char input[BUFSIZ];
  char argv[MAX_ARGS][BUFSIZ];
  int argc = 0;

  while (1) {
    refresh_prompt();
    ssize_t bytes_read = read_line(STDIN_FILENO, input, BUFSIZ);
    if (bytes_read < 0) {
      if (errno == EINTR)
        continue;
      else
        break;
    }

    shell_command cmd = parse_shell_cmd(input, argv, &argc);

    operation_error err = dispatch_command(cmd, argv, argc);

    if (err != OPERATION_SUCCESS) {
      snprintf(log_msg, BUFSIZ, "Invalid command\n");
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    }

    if (cmd == CMD_START_MONITOR)
      usleep(DELAY_START_MONITOR);
  }

  return EXIT_SUCCESS;
}

//=============================================================================
// Shell Entry Point
//=============================================================================
int main(void) {
  setup_signal_handlers();
  run_repl();

  return EXIT_SUCCESS;
}
