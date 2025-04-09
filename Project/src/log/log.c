#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

time_t now;

int symlink_file(const char *path) {
  char target_path[PATH_MAX] = "";
  snprintf(target_path, sizeof(target_path), "../%s/%s/%s", TREASURE_DIRECTORY,
           path, LOG_FILE_NAME);

  char symlink_path[PATH_MAX] = "";
  snprintf(symlink_path, sizeof(symlink_path), "%s/%s.%s", LOG_DIRECTORY, path,
           LOG_FILE_NAME);

  if (access(symlink_path, F_OK) == 0) {
    return NO_ERROR;
  }

  if (symlink(target_path, symlink_path) == -1) {
    perror("SYMLINK");
    return SYMLINK_ERROR;
  }

  return NO_ERROR;
}

operation_error log_message(const char *dir_name, const char *message) {

  char log_path[PATH_MAX] = "";
  snprintf(log_path, sizeof(log_path), "%s/%s.%s", LOG_DIRECTORY, dir_name,
           LOG_FILE_NAME);

  int fd = open(log_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
  if (fd == -1) {
    perror("LOG OPEN FAILED");
    return FILE_ERROR;
  }

  time(&now);

  // print the local time to the log file
  write(fd, ctime(&now), strlen(ctime(&now)));

  ssize_t bytes_written = write(fd, message, strlen(message));
  if (bytes_written == -1) {
    perror("LOG WRITE FAILED");
    close(fd);
    return FILE_ERROR;
  }

  write(fd, "\n", 1);

  close(fd);
  return NO_ERROR;
}
