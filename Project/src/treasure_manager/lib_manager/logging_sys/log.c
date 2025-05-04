#include "log.h"
#include <time.h>

time_t now;

//=============================================================================
// Symlink log files
//=============================================================================

int symlink_file(const char *dir_name) {
  char target_path[PATH_MAX] = "";
  snprintf(target_path, sizeof(target_path), "../%s/%s/%s", TREASURE_DIRECTORY,
           dir_name, LOG_FILE_NAME);

  char symlink_path[PATH_MAX] = "";
  snprintf(symlink_path, sizeof(symlink_path), "%s/%s.%s", LOG_DIRECTORY,
           dir_name, LOG_FILE_NAME);

  if (open(symlink_path, F_OK) == 0) {
    return NO_ERROR;
  }

  if (symlink(target_path, symlink_path) == -1 && errno != EEXIST) {
    perror("SYMLINK");
    return SYMLINK_ERROR;
  }

  return NO_ERROR;
}

//=============================================================================
// Logging System
//=============================================================================

operation_error log_message(const char *path, const char *message) {
  int fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
  if (fd == -1) {
    perror("LOG OPEN FAILED");
    perror(path);
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

//=============================================================================
// Log Messages
//=============================================================================

void get_add_failure_log_message(char *msg, treasure *t) {
  snprintf(msg, LOG_MESSAGE_MAX,
           "ADD FAILURE | INVALID INPUT | ID: %d | User: %s\n", t->id,
           t->user_name);
}

void get_add_killed_log_message(char *msg, treasure *t) {
  snprintf(msg, LOG_MESSAGE_MAX,
           "ADD FAILURE | FILE ERROR | ID: %d | User: %s\n", t->id,
           t->user_name);
}

void get_add_success_log_message(char *msg, treasure *t) {
  snprintf(msg, LOG_MESSAGE_MAX, "ADD SUCCESS | ID: %d | User: %s\n", t->id,
           t->user_name);
}

void get_search_killed_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "SEARCH FAILURE | FILE NOT FOUND | ID: %d\n",
           id);
}

void get_search_failure_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "SEARCH FAILURE | ITEM NOT FOUND | ID: %d\n",
           id);
}

void get_search_success_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "SEARCH SUCCESS | ID: %d\n", id);
}

void get_list_failure_log_message(char *msg, const char *path) {
  snprintf(msg, LOG_MESSAGE_MAX, "LIST FAILURE | ITEM NOT FOUND | Path: %s\n",
           path);
}

void get_list_success_log_message(char *msg, const char *path) {
  snprintf(msg, LOG_MESSAGE_MAX, "LIST SUCCESS | Path: %s\n", path);
}

void get_remove_killed_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "REMOVE FAILURE | FILE ERROR | ID: %d\n", id);
}

void get_remove_failure_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "REMOVE FAILURE | ITEM NOT FOUND | ID: %d\n",
           id);
}

void get_remove_success_log_message(char *msg, int id) {
  snprintf(msg, LOG_MESSAGE_MAX, "REMOVE SUCCESS | ID: %d\n", id);
}
