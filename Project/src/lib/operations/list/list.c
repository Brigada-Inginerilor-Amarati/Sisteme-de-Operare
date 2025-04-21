#include "list.h"
#include "../../log/log.h"
#include "../../treasure/treasure.h"
#include "../../utils/utils.h"

operation_error list_hunt(char *path) {

  // prepare the path variables
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
  off_t total_size = get_treasure_file_size(treasure_file_path);
  char total_size_str[BUFSIZ];
  snprintf(total_size_str, BUFSIZ, "Total size: %llu bytes\n", total_size);
  write(STDOUT_FILENO, total_size_str, strlen(total_size_str));

  // print the last modification time
  time_t latest_mtime = get_treasure_file_last_modified(treasure_file_path);
  if (latest_mtime == 0)
    write(STDOUT_FILENO, "Last modified: N/A\n",
          strlen("Last modified: N/A\n"));
  else {
    char *time_str = ctime(&latest_mtime);
    if (time_str != NULL) {
      write(STDOUT_FILENO, "Last modified: ", strlen("Last modified: "));
      write(STDOUT_FILENO, time_str, strlen(time_str));
    }
  }

  // read and print each treasure
  treasure t;
  int count = 0;
  while (read(fd, &t, sizeof(treasure)) == sizeof(treasure)) {
    write(STDOUT_FILENO, "\n--- Treasure Entry ---\n", 24);
    print_treasure(&t);
    count++;
  }

  // in case of no treasures found
  if (count == 0)
    write(STDOUT_FILENO, "\nNo treasures found.\n", 22);

  // end the list operation
  close(fd);
  get_list_success_log_message(log_msg, path);
  log_message(log_file_path, log_msg);

  return NO_ERROR;
}

operation_error list_treasure(char *path, int id) {

  // prepare the path variables
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

  // check if the treasure file exists
  int fd = open(treasure_file_path, O_RDONLY);
  if (fd == -1) {
    get_search_killed_log_message(log_msg, id);
    log_message(log_file_path, log_msg);
    perror("LIST ERROR, FILE NOT FOUND");
    return FILE_NOT_FOUND;
  }

  // write the data to stdout
  treasure t;
  while (read(fd, &t, sizeof(treasure)) == sizeof(treasure)) {
    if (t.id == id) {
      write(STDOUT_FILENO, "--- Treasure Found ---\n",
            strlen("--- Treasure Found ---\n"));

      print_treasure(&t);
      // end the search, treasure found
      get_search_success_log_message(log_msg, id);
      log_message(log_file_path, log_msg);
      close(fd);
      return NO_ERROR;
    }
  }

  // end the search, no treasure found
  close(fd);
  get_search_failure_log_message(log_msg, id);
  log_message(log_file_path, log_msg);
  return TREASURE_NOT_FOUND;
}

operation_error list_hunts() {

  // open the treasure_hunts directory and list all the directories inside
  DIR *dir;
  struct dirent *entry;

  // check if the treasure_hunts directory exists
  dir = opendir(TREASURE_DIRECTORY);
  if (dir == NULL) {
    perror("LIST ERROR, DIRECTORY NOT FOUND");
    return DIRECTORY_NOT_FOUND;
  }

  char msg[BUFSIZ];

  // parse the directory entries
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
        strcmp(entry->d_name, "..") != 0) {

      // print the hunt name and treasure count from each entry
      snprintf(msg, BUFSIZ, "Hunt %s: \nTreasure count: %d\n", entry->d_name,
               get_treasure_count(entry->d_name));
      write(STDOUT_FILENO, msg, strlen(msg));
    }
  }

  // end the hunt listing
  closedir(dir);
  return NO_ERROR;
}
