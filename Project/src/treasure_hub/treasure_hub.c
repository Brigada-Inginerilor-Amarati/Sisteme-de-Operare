
/*
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Constants
#define MAX_COMMAND_LENGTH 100
#define TREASURE_DIRECTORY "treasure_hunts"
#define MAX_PATH 256

// Global variables
pid_t monitor_pid = -1;
int monitor_running = 0;
int awaiting_response = 0;

// Signal handlers
void handle_sigusr1(int signum) {
  // Signal from monitor indicating operation complete
  awaiting_response = 0;
}

// Function prototypes
void start_monitor();
void list_hunts();
void list_treasures(const char *hunt_id);
void view_treasure(const char *hunt_id, int treasure_id);
void stop_monitor();
void process_command(char *command);
void display_help();

int main() {
  char command[MAX_COMMAND_LENGTH];

  // Set up signal handler
  struct sigaction sa;
  sa.sa_handler = handle_sigusr1;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGUSR1, &sa, NULL);

  printf("Welcome to Treasure Hub!\n");
  printf("Type 'help' to see available commands.\n");

  while (1) {
    printf("treasure_hub> ");
    fflush(stdout);

    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
      break; // Exit on EOF
    }

    // Remove trailing newline
    command[strcspn(command, "\n")] = '\0';

    if (strlen(command) == 0) {
      continue;
    }

    process_command(command);
  }

  // Clean up if necessary
  if (monitor_running) {
    stop_monitor();
  }

  return 0;
}

void process_command(char *command) {
  char *token = strtok(command, " ");

  if (token == NULL) {
    return;
  }

  if (strcmp(token, "start_monitor") == 0) {
    start_monitor();
  } else if (strcmp(token, "list_hunts") == 0) {
    if (!monitor_running) {
      printf("Error: Monitor is not running. Use start_monitor first.\n");
      return;
    }
    list_hunts();
  } else if (strcmp(token, "list_treasures") == 0) {
    if (!monitor_running) {
      printf("Error: Monitor is not running. Use start_monitor first.\n");
      return;
    }
    token = strtok(NULL, " ");
    if (token == NULL) {
      printf("Error: Hunt ID required.\n");
      return;
    }
    list_treasures(token);
  } else if (strcmp(token, "view_treasure") == 0) {
    if (!monitor_running) {
      printf("Error: Monitor is not running. Use start_monitor first.\n");
      return;
    }
    token = strtok(NULL, " ");
    if (token == NULL) {
      printf("Error: Hunt ID required.\n");
      return;
    }
    char *hunt_id = token;

    token = strtok(NULL, " ");
    if (token == NULL) {
      printf("Error: Treasure ID required.\n");
      return;
    }
    int treasure_id = atoi(token);

    view_treasure(hunt_id, treasure_id);
  } else if (strcmp(token, "stop_monitor") == 0) {
    if (!monitor_running) {
      printf("Error: Monitor is not already running.\n");
      return;
    }
    stop_monitor();
  } else if (strcmp(token, "exit") == 0) {
    if (monitor_running) {
      printf("Error: Monitor is still running. Stop it before exiting.\n");
      return;
    }
    printf("Exiting Treasure Hub. Goodbye!\n");
    exit(0);
  } else if (strcmp(token, "help") == 0) {
    display_help();
  } else {
    printf("Unknown command: %s\n", token);
    display_help();
  }
}

void start_monitor() {
  if (monitor_running) {
    printf("Monitor is already running.\n");
    return;
  }

  monitor_pid = fork();

  if (monitor_pid == -1) {
    perror("Failed to fork monitor process");
    return;
  } else if (monitor_pid == 0) {
    // Child process (monitor)
    // Set up signal handlers for monitor
    signal(SIGUSR1, SIG_IGN); // Ignore SIGUSR1 in child
    signal(SIGUSR2, SIG_DFL); // Default handler for SIGUSR2

    // Implement monitor functionality here
    // Monitor should watch for signals from the parent process
    while (1) {
      // Wait for signals from parent
      pause();
      // When receiving a signal, perform the requested operation
      // Send SIGUSR1 back to parent when done
      kill(getppid(), SIGUSR1);
      // For testing the stop_monitor feature, we delay the exit
      sleep(3); // usleep() as mentioned in the requirements
    }

    exit(0);
  } else {
    // Parent process
    monitor_running = 1;
    printf("Monitor started with PID: %d\n", monitor_pid);
  }
}

void list_hunts() {
  if (!monitor_running) {
    return;
  }

  // Send signal to monitor to list hunts
  awaiting_response = 1;
  kill(monitor_pid, SIGUSR2); // Signal monitor to list hunts

  // Wait for response
  while (awaiting_response) {
    usleep(100000); // 100ms
  }

  // Response has been received and displayed by monitor
}

void list_treasures(const char *hunt_id) {
  if (!monitor_running) {
    return;
  }

  // Implementation would involve sending a signal to the monitor
  // and possibly sharing the hunt_id through some IPC mechanism
  printf("Requesting monitor to list treasures in hunt: %s\n", hunt_id);

  // For this example, we'll simulate the request
  awaiting_response = 1;
  // In a real implementation, you'd need to pass the hunt_id to the monitor
  kill(monitor_pid, SIGUSR2);

  // Wait for response
  while (awaiting_response) {
    usleep(100000); // 100ms
  }
}

void view_treasure(const char *hunt_id, int treasure_id) {
  if (!monitor_running) {
    return;
  }

  printf(
      "Requesting monitor to show information about treasure %d in hunt: %s\n",
      treasure_id, hunt_id);

  // Similar to list_treasures, need proper IPC
  awaiting_response = 1;
  kill(monitor_pid, SIGUSR2);

  // Wait for response
  while (awaiting_response) {
    usleep(100000); // 100ms
  }
}

void stop_monitor() {
  if (!monitor_running) {
    return;
  }

  printf("Stopping monitor...\n");

  // Send termination signal to monitor
  kill(monitor_pid, SIGTERM);

  // Wait for monitor to terminate
  int status;
  waitpid(monitor_pid, &status, 0);

  if (WIFEXITED(status)) {
    printf("Monitor terminated with status: %d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("Monitor killed by signal: %d\n", WTERMSIG(status));
  }

  monitor_running = 0;
  monitor_pid = -1;
}

void display_help() {
  printf("Available commands:\n");
  printf("  start_monitor            - Start the monitoring process\n");
  printf("  list_hunts               - List all treasure hunts\n");
  printf("  list_treasures <hunt_id> - List treasures in a specific hunt\n");
  printf("  view_treasure <hunt_id> <treasure_id> - View details of a specific "
         "treasure\n");
  printf("  stop_monitor             - Stop the monitoring process\n");
  printf("  exit                     - Exit the program (monitor must be "
         "stopped first)\n");
  printf("  help                     - Display this help message\n");
}
*/
