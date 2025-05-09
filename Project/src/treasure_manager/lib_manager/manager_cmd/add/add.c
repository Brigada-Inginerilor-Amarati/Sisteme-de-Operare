#include "add.h"
#include "../../logging_sys/log.h"
#include "../../manager_utils/manager_utils.h"
#include "../../treasure_operations/treasure_operations.h"

/*
ADD
Parameters: <hunt_id> -> char *
Task: Add a new treasure to the specified hunt in the treasures.csv file.
Exception: Creates a new hunt directory if it does not exist.
*/

operation_error add_treasure(char *hunt_dir, int fd) {

  prepare_paths(hunt_dir);

  if (!directory_exists(dir_path) && add_directory(dir_path) != NO_ERROR)
    return DIRECTORY_ERROR;

  if (add_file(treasure_file_path) != NO_ERROR ||
      add_file(log_file_path) != NO_ERROR)
    return FILE_ERROR;

  if (symlink_file(hunt_dir) != NO_ERROR)
    return SYMLINK_ERROR;

  treasure t;
  char log_msg[BUFSIZ];
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
