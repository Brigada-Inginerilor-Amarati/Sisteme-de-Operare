// ADD, LIST, REMOVE
#include "operations.h"
#include "../log/log.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <time.h>
#include <unistd.h>
#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

/*
ADD
Parameters: <hunt_id> -> char *
Task: Add a new treasure to the specified hunt in the treasures.csv file.
Exception: Creates a new hunt directory if it does not exist.
*/

operation_error add_file(const char *path, const char *additional_path) {
  // check if the file exists
  char full_path[PATH_MAX] = "";
  snprintf(full_path, PATH_MAX, "%s/%s", path, additional_path);

  if (access(full_path, F_OK) == -1) {
    write(STDERR_FILENO, "File does not exist\n",
          strlen("File does not exist\n"));

    // create the file
    int fd = open(full_path, O_CREAT | O_WRONLY, PERMISSIONS);
    if (fd == -1) {
      write(STDERR_FILENO, "Failed to create file\n",
            strlen("Failed to create file\n"));
      perror("File creation failed");
      return FILE_ERROR;
    }

    close(fd);
  }

  return NO_ERROR;
}

operation_error directory_exists(const char *path) {
  DIR *dir = opendir(path);
  if (dir == NULL)
    return 0;

  closedir(dir);
  return 1;
}

operation_error add_directory(char *path) {
  const char *err_msg =
      "No hunt with this id was found.\nCreating new hunt directory.\n";
  write(STDERR_FILENO, err_msg, strlen(err_msg));

  if (mkdir(path, PERMISSIONS) == -1) {
    write(STDERR_FILENO, "Failed to create directory\n",
          strlen("Failed to create directory\n"));
    perror("Could not create directory");
    return DIRECTORY_ERROR;
  }

  return NO_ERROR;
}

operation_error add_treasure(char *path) {

  char full_path[PATH_MAX] = "";
  snprintf(full_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);

  if (!directory_exists(full_path) && add_directory(full_path) != NO_ERROR)
    return DIRECTORY_ERROR;

  if (add_file(full_path, TREASURE_FILE_NAME) != NO_ERROR ||
      add_file(full_path, LOG_FILE_NAME) != NO_ERROR)
    return FILE_ERROR;

  if (symlink_file(path) != NO_ERROR)
    return SYMLINK_ERROR;

  treasure t = create_treasure();
  char log_msg[LOG_MESSAGE_MAX] = "";
  if (is_void_treasure(&t)) {
    get_add_failure_log_message(log_msg, &t);
    log_message(path, log_msg);
    return INVALID_INPUT;
  }

  if (write_treasure_to_file(&t, full_path) != NO_ERROR) {
    get_add_killed_log_message(log_msg, &t);
    log_message(path, log_msg);
    return FILE_ERROR;
  }

  get_add_success_log_message(log_msg, &t);
  log_message(path, log_msg);

  return NO_ERROR;
}

/*
LIST
Parameters: <hunt_id> -> char *
Task: List the name of the directory, the content size and the last updated time
of the contents. List all treasures in the hunt directory. or <hunt_id>,
<treasure_id> -> char * and int Task: List the specific treasure.
*/

off_t get_directory_total_size(const char *dir_path) {
  DIR *dir = opendir(dir_path);
  if (!dir)
    return 0;

  struct dirent *entry;
  struct stat st;
  char file_path[PATH_MAX];
  off_t total_size = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
    if (stat(file_path, &st) == 0)
      total_size += st.st_size;
  }

  closedir(dir);
  return total_size;
}

time_t get_directory_last_modified_time(const char *dir_path) {
  DIR *dir = opendir(dir_path);
  if (!dir)
    return 0;

  struct dirent *entry;
  struct stat st;
  char file_path[PATH_MAX];
  time_t latest_mtime = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(file_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
    if (stat(file_path, &st) == 0 && st.st_mtime > latest_mtime)
      latest_mtime = st.st_mtime;
  }

  closedir(dir);
  return latest_mtime;
}

operation_error list_hunt(char *path) {

  char log_msg[LOG_MESSAGE_MAX];
  char full_path[PATH_MAX] = "";
  char dir_path[PATH_MAX] = "";
  snprintf(dir_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);
  snprintf(full_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);

  // print all the contents of the file
  int fd = open(full_path, O_RDONLY);
  if (fd == -1) {
    perror("LIST ERROR, FILE NOT FOUND");
    get_list_failure_log_message(log_msg, path);
    log_message(path, log_msg);
    return FILE_NOT_FOUND;
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
  log_message(path, log_msg);

  return NO_ERROR;
}

int extract_id(char *string) {
  int id = 0;
  // the id is the first number in the string
  sscanf(string, "%d", &id);
  return id;
}

operation_error list_treasure(char *path, int id) {
  if (id == 0)
    return TREASURE_NOT_FOUND;

  char log_msg[LOG_MESSAGE_MAX];
  char full_path[PATH_MAX] = "";
  snprintf(full_path, PATH_MAX, "%s/%s/%s", TREASURE_DIRECTORY, path,
           TREASURE_FILE_NAME);

  int fd = open(full_path, O_RDONLY);
  if (fd == -1) {
    get_search_killed_log_message(log_msg, id);
    log_message(path, log_msg);
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
        log_message(path, log_msg);
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
  log_message(path, log_msg);
  return TREASURE_NOT_FOUND;
}

/*
REMOVE
Parameters: <hunt_id> -> char *
Task: Remove the hunt directory.
Parameters: <hunt_id> -> char * and <treasure_id> -> int
Task: Remove the specific treasure from the hunt.
*/

operation_error remove_hunt(char *dir_name) {

  char path[PATH_MAX];
  sprintf(path, "%s/%s", TREASURE_DIRECTORY, dir_name);

  char file_path[PATH_MAX];
  sprintf(file_path, "%s/%s", path, TREASURE_FILE_NAME);

  char log_path[PATH_MAX];
  sprintf(log_path, "%s/%s", path, LOG_FILE_NAME);

  // check if the directory exists
  if (access(path, F_OK) == -1) {
    return DIRECTORY_NOT_FOUND;
  }

  // remove the treasure file
  if (unlink(file_path) != 0) {
    perror("REMOVE ERROR, FILE NOT FOUND");
    return FILE_NOT_FOUND;
  }

  // remove the log file
  if (unlink(log_path) != 0) {
    perror("REMOVE ERROR, FILE NOT FOUND");
    return FILE_NOT_FOUND;
  }

  // remove the directory
  if (rmdir(path) != 0) {
    perror("REMOVE ERROR, DIRECTORY NOT FOUND");
    return DIRECTORY_NOT_FOUND;
  }

  char log_msg[LOG_MESSAGE_MAX];

  snprintf(log_msg, LOG_MESSAGE_MAX, "Hunt %s removed successfully\n",
           dir_name);

  write(STDOUT_FILENO, log_msg, strlen(log_msg));

  return NO_ERROR;
}

operation_error remove_treasure(char *path, int id) { return NO_ERROR; }
