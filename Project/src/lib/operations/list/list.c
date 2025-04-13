#include "list.h"
#include "../../log/log.h"
#include "../../utils/utils.h"

operation_error list_hunt(char *path) {

  char log_msg[LOG_MESSAGE_MAX];
  char dir_path[PATH_MAX];
  char treasure_file_path[PATH_MAX];
  char log_file_path[PATH_MAX];
  snprintf(dir_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);
  snprintf(treasure_file_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);
  snprintf(log_file_path, PATH_MAX, "%s/%s", dir_path, LOG_FILE_NAME);

  // check if the directory exists
  if (open(dir_path, O_DIRECTORY) == -1) {
    perror("LIST ERROR, DIRECTORY NOT FOUND");
    return DIRECTORY_NOT_FOUND;
  }

  // check if the file exists
  int fd = open(treasure_file_path, O_RDONLY);
  if (fd == -1) {
    perror("LIST ERROR, FILE NOT FOUND");
    get_list_failure_log_message(log_msg, path);
    log_message(log_file_path, log_msg);
    return DIRECTORY_NOT_FOUND;
  }

  // print the hunt name
  write(STDOUT_FILENO, "Hunt: ", strlen("Hunt: "));
  write(STDOUT_FILENO, path, strlen(path));
  write(STDOUT_FILENO, "\n", strlen("\n"));

  // print the total size of the directory
  off_t total_size = get_directory_total_size(dir_path);
  char total_size_str[BUFSIZ];
  snprintf(total_size_str, BUFSIZ, "Total size: %llu bytes\n", total_size);
  write(STDOUT_FILENO, total_size_str, strlen(total_size_str));

  // print the last modification time
  time_t latest_mtime = get_directory_last_modified_time(dir_path);
  char *time_str = ctime(&latest_mtime);
  if (time_str != NULL) {
    write(STDOUT_FILENO, "Last modified: ", strlen("Last modified: "));
    write(STDOUT_FILENO, time_str, strlen(time_str));
  }

  char buffer[BUFSIZ];
  ssize_t bytes_read;
  while ((bytes_read = read(fd, buffer, BUFSIZ)) > 0) {
    write(STDOUT_FILENO, buffer, bytes_read);
  }

  close(fd);
  get_list_success_log_message(log_msg, path);
  log_message(log_file_path, log_msg);

  return NO_ERROR;
}

operation_error list_treasure(char *path, int id) {
  if (id == 0)
    return TREASURE_NOT_FOUND;

  char log_msg[LOG_MESSAGE_MAX];
  char dir_path[PATH_MAX];
  char treasure_file_path[PATH_MAX];
  char log_file_path[PATH_MAX];
  snprintf(dir_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);
  snprintf(treasure_file_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);
  snprintf(log_file_path, PATH_MAX, "%s/%s", dir_path, LOG_FILE_NAME);

  if (open(dir_path, O_DIRECTORY) == -1) {
    perror("LIST ERROR, DIRECTORY NOT FOUND");
    return DIRECTORY_NOT_FOUND;
  }

  int fd = open(treasure_file_path, O_RDONLY);
  if (fd == -1) {
    get_search_killed_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    perror("LIST ERROR, FILE NOT FOUND");
    return FILE_NOT_FOUND;
  }

  char ch;
  char line[BUFSIZ];
  int line_idx = 0;
  ssize_t bytes_read;

  // Citește caracter cu caracter
  while ((bytes_read = read(fd, &ch, 1)) == 1) {
    if (ch == '\n' || line_idx >= BUFSIZ - 1) {
      // Final de linie -> termină stringul
      line[line_idx] = '\0';

      // Extrage și compară id
      if (extract_id(line) == id) {
        write(STDOUT_FILENO, line, strlen(line));
        write(STDOUT_FILENO, "\n", 1);
        get_search_success_log_message(log_msg, id);
        log_message(log_file_path, log_msg);
        close(fd);
        return NO_ERROR;
      }

      line_idx = 0;
    } else {
      line[line_idx++] = ch;
    }
  }

  close(fd);

  get_search_failure_log_message(log_msg, id);
  log_message(log_file_path, log_msg);
  return TREASURE_NOT_FOUND;
}
