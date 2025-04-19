#include "add.h"
#include "../../log/log.h"
#include "../../treasure/treasure.h"
#include "../../utils/utils.h"

/*
ADD
Parameters: <hunt_id> -> char *
Task: Add a new treasure to the specified hunt in the treasures.csv file.
Exception: Creates a new hunt directory if it does not exist.
*/

operation_error add_treasure(char *path, int fd) {

  char dir_path[PATH_MAX] = "";
  char treasure_file_path[PATH_MAX] = "";
  char log_file_path[PATH_MAX] = "";
  snprintf(dir_path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, path);
  snprintf(treasure_file_path, PATH_MAX, "%s/%s", dir_path, TREASURE_FILE_NAME);
  snprintf(log_file_path, PATH_MAX, "%s/%s", dir_path, LOG_FILE_NAME);

  if (!directory_exists(dir_path) && add_directory(dir_path) != NO_ERROR)
    return DIRECTORY_ERROR;

  if (add_file(treasure_file_path) != NO_ERROR ||
      add_file(log_file_path) != NO_ERROR)
    return FILE_ERROR;

  if (symlink_file(path) != NO_ERROR)
    return SYMLINK_ERROR;

  treasure t;
  char log_msg[LOG_MESSAGE_MAX] = "";
  operation_error err = create_treasure(&t, fd);

  if (err != NO_ERROR) {
    get_add_failure_log_message(log_msg, &t);
    log_message(log_file_path, log_msg);
    return err;
  }

  // check if the id already exists
  if (id_exists(treasure_file_path, t.id) == TREASURE_ALREADY_EXISTS) {
    write(STDOUT_FILENO, "Treasure ID already exists\n",
          strlen("Treasure ID already exists\n"));
    get_add_failure_log_message(log_msg, &t);
    log_message(log_file_path, log_msg);
    return TREASURE_ALREADY_EXISTS;
  }

  // write the treasure to the file
  if (write_treasure_to_file(&t, treasure_file_path) != NO_ERROR) {
    get_add_killed_log_message(log_msg, &t);
    log_message(log_file_path, log_msg);
    return FILE_ERROR;
  }

  get_add_success_log_message(log_msg, &t);
  log_message(log_file_path, log_msg);

  return NO_ERROR;
}
