#include "shell.h"
#include "../../lib/utils/utils.h"
#include "lib_shell/shell_args_parser/shell_args_parser.h"
#include "lib_shell/shell_cmd/shell_cmd.h"
#include "lib_shell/shell_signals/shell_signals.h"
#include "lib_shell/shell_utils/shell_utils.h"

// init shell state
monitor_t monitor = {.pid = -1,
                     .state = MONITOR_OFFLINE,
                     .read_pipe_fd = -1,
                     .write_pipe_fd = -1};

void read_data_from_pipe(int fd) {
  char buffer[BUFSIZ];
  ssize_t n;
  while ((n = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[n] = '\0';
    write(STDOUT_FILENO, buffer, n);
  }
}

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

    int err = dispatch_command(cmd, argv, argc);

    if (!err) {
      snprintf(log_msg, BUFSIZ, "Invalid command\n");
      write(STDOUT_FILENO, log_msg, strlen(log_msg));
    }

    if (cmd == CMD_LIST_HUNTS || cmd == CMD_LIST_TREASURES ||
        cmd == CMD_VIEW_TREASURE)
      read_data_from_pipe(monitor.read_pipe_fd);

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
