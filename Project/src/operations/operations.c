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
#include <unistd.h>

#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

/*
Parameters: <hunt_id> -> char *
Task: Add a new treasure to the specified hunt in the treasures.dat file.
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
    get_invalid_input_log_message(log_msg, &t);
    log_message(full_path, log_msg);
    return INVALID_INPUT;
  }

  if (write_treasure_to_file(&t, full_path) != NO_ERROR) {
    get_file_error_log_message(log_msg, &t);
    log_message(full_path, log_msg);
    return FILE_ERROR;
  }

  get_success_log_message(log_msg, &t);
  log_message(full_path, log_msg);

  return NO_ERROR;
}

/*
TODO: Implement the LIST operation.

Parameters: <hunt_id> -> char *
or
<hunt_id>,
*/

/*
TODO: Implement the REMOVE operation.
*/

void print_operation_error(operation_error err) {
  switch (err) {
  case DIRECTORY_ERROR:
    write(STDERR_FILENO, "Directory error\n", strlen("Directory error\n"));
    break;
  case FILE_ERROR:
    write(STDERR_FILENO, "File error\n", strlen("File error\n"));
    break;
  case OPERATION_FAILED:
    write(STDERR_FILENO, "Operation failed\n", strlen("Operation failed\n"));
    break;
  case NO_ERROR:
    write(STDERR_FILENO, "No error\n", strlen("No error\n"));
    break;
  case SYMLINK_ERROR:
    write(STDERR_FILENO, "Symlink error\n", strlen("Symlink error\n"));
    break;
  default:
    write(STDERR_FILENO, "Unknown error\n", strlen("Unknown error\n"));
    break;
  }
}
