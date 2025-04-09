#include "../operations/operations.h"
#include "../treasure_manager.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int symlink_file(const char *path) {
  char target_path[PATH_MAX] = "";
  snprintf(target_path, sizeof(target_path), "../%s/%s/%s", TREASURE_DIRECTORY,
           path, TREASURE_FILE_NAME);

  char symlink_path[PATH_MAX] = "";
  snprintf(symlink_path, sizeof(symlink_path), "%s/%s%s", LOG_DIRECTORY, path,
           LOG_FILE_SUFFIX);

  if (access(symlink_path, F_OK) == 0) {
    return NO_ERROR;
  }

  if (symlink(target_path, symlink_path) == -1) {
    perror("SYMLINK");
    return SYMLINK_ERROR;
  }

  return NO_ERROR;
}
