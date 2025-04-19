#include "remove.h"
#include "../../log/log.h"

operation_error remove_hunt(char *dir_name) {
  char path[PATH_MAX];
  snprintf(path, sizeof(path), "%s/%s", TREASURE_DIRECTORY, dir_name);

  DIR *dir = opendir(path);
  if (!dir) {
    perror("REMOVE ERROR, CANNOT OPEN DIRECTORY");
    return DIRECTORY_ERROR;
  }

  struct dirent *entry;
  char file_path[PATH_MAX];

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

    if (unlink(file_path) != 0) {
      perror("REMOVE ERROR, UNLINK FAILED");
      closedir(dir);
      return FILE_ERROR;
    }
  }

  closedir(dir);

  // Remove the now-empty directory
  if (rmdir(path) != 0) {
    perror("REMOVE ERROR, RMDIR FAILED");
    return DIRECTORY_ERROR;
  }

  char log_msg[LOG_MESSAGE_MAX];
  snprintf(log_msg, LOG_MESSAGE_MAX, "Hunt %s removed successfully\n",
           dir_name);
  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  return NO_ERROR;
}

operation_error remove_treasure(char *path, int id) {
  char dir_path[PATH_MAX];
  char treasure_file_path[PATH_MAX];
  char tmp_treasure_file_path[PATH_MAX];
  char log_file_path[PATH_MAX];
  char log_msg[LOG_MESSAGE_MAX];

  snprintf(dir_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);
  snprintf(treasure_file_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);
  snprintf(tmp_treasure_file_path, PATH_MAX, "%s/copy_%s", dir_path,
           TREASURE_FILE_NAME);
  snprintf(log_file_path, PATH_MAX, "%s/%s", dir_path, LOG_FILE_NAME);

  int in_fd = open(treasure_file_path, O_RDONLY);
  if (in_fd == -1) {
    perror("Failed to open original file");
    get_remove_killed_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    return FILE_NOT_FOUND;
  }

  int out_fd = open(tmp_treasure_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (out_fd == -1) {
    perror("Failed to open temp file");
    get_remove_killed_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    close(in_fd);
    return FILE_ERROR;
  }

  treasure t;
  int found = 0;

  while (read(in_fd, &t, sizeof(treasure)) == sizeof(treasure)) {
    if (t.id != id) {
      if (write(out_fd, &t, sizeof(treasure)) != sizeof(treasure)) {
        perror("Failed to write to temp file");
        close(in_fd);
        close(out_fd);
        unlink(tmp_treasure_file_path);
        get_remove_killed_log_message(log_msg, id);
        log_message(log_file_path, log_msg);
        return FILE_ERROR;
      }
    } else {
      found = 1;
    }
  }

  close(in_fd);
  close(out_fd);

  if (!found) {
    unlink(tmp_treasure_file_path);
    get_remove_failure_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    return TREASURE_NOT_FOUND;
  }

  if (rename(tmp_treasure_file_path, treasure_file_path) != 0) {
    perror("Failed to replace original file");
    get_remove_killed_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    return FILE_ERROR;
  }

  get_remove_success_log_message(log_msg, id);
  log_message(log_file_path, log_msg);

  return NO_ERROR;
}
