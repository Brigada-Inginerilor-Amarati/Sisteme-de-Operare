#include "../operations/operations.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char *LOG_DIRECTORY = "logs";
const char *LOG_FILE_PREFIX = "log_";
const char *TREASURE_FILE_PATH = "treasures.dat";
const char *TREASURE_DIRECTORY = "treasure_hunts";

int symlink_file(const char *path) {
  char full_path[PATH_MAX] = "";
  snprintf(full_path, sizeof(full_path), "%s/%s/%s", TREASURE_DIRECTORY, path,
           TREASURE_FILE_PATH);

  char symlink_path[PATH_MAX] = "";
  snprintf(symlink_path, sizeof(symlink_path), "%s/%s%s.txt", LOG_DIRECTORY,
           LOG_FILE_PREFIX, path);

  printf("File path: %s\n", full_path);
  printf("Symlink path: %s\n", symlink_path);

  // Check if the symlink already exists
  if (access(symlink_path, F_OK) == 0) {
    write(STDOUT_FILENO, "Symlink already exists\n",
          strlen("Symlink already exists\n"));
    return NO_ERROR;
  }

  // Create the symlink
  if (symlink(full_path, symlink_path) == -1) {
    write(STDERR_FILENO, "Failed to create symlink\n",
          strlen("Failed to create symlink\n"));
    perror("SYMLINK");
    return SYMLINK_ERROR;
  }

  return NO_ERROR; // Success
}
