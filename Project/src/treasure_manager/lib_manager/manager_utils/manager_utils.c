#include "manager_utils.h"

//=============================================================================
// Path Creation
//=============================================================================

char dir_path[PATH_MAX];
char treasure_file_path[PATH_MAX];
char log_file_path[PATH_MAX];
char log_msg[BUFSIZ];

void prepare_paths(const char *hunt_dir) {
  get_hunt_directory_path(dir_path, hunt_dir);
  get_treasure_file_path(treasure_file_path, hunt_dir);
  get_log_file_path(log_file_path, hunt_dir);
}

void get_root_hunt_directory_path(char *path) { path = TREASURE_DIRECTORY; }

void get_hunt_directory_path(char *path, const char *hunt_name) {
  snprintf(path, PATH_MAX, "%s/%s", TREASURE_DIRECTORY, hunt_name);
}

void get_treasure_file_path(char *path, const char *hunt_name) {
  snprintf(path, PATH_MAX, "%s/%s/%s", TREASURE_DIRECTORY, hunt_name,
           TREASURE_FILE_NAME);
}

void get_log_file_path(char *path, const char *hunt_name) {
  snprintf(path, PATH_MAX, "%s/%s/%s", TREASURE_DIRECTORY, hunt_name,
           LOG_FILE_NAME);
}

void get_symlink_directory_path(char *path) { path = LOG_DIRECTORY; }

void get_symlink_path(char *path, const char *hunt_name) {
  snprintf(path, PATH_MAX, "%s/%s.%s", LOG_DIRECTORY, hunt_name, LOG_FILE_NAME);
}

//=============================================================================
// File / Directory Management
//=============================================================================

operation_error add_file(const char *path) {
  // check if the file exists
  if (open(path, F_OK) == -1) {
    perror(path);
    // create the file
    int fd = open(path, CREATE_FILE, PERMISSIONS);
    if (fd == -1) {
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

operation_error add_directory(const char *path) {
  const char *err_msg =
      "No hunt with this id was found.\nCreating new hunt directory.\n";
  write(STDERR_FILENO, err_msg, strlen(err_msg));

  if (mkdir(path, PERMISSIONS) == -1) {
    perror("Could not create directory");
    return DIRECTORY_ERROR;
  }

  return NO_ERROR;
}

//=============================================================================
// File / Directory Data
//=============================================================================

off_t get_treasure_file_size(const char *path) {
  struct stat st;
  if (stat(path, &st) == -1)
    return 0;

  return st.st_size;
}

time_t get_treasure_file_last_modified(const char *path) {
  struct stat st;
  if (stat(path, &st) == -1)
    return 0;

  return st.st_mtime;
}

operation_error id_exists(const char *path, int id) {
  int fd = open(path, O_RDONLY);
  if (fd == -1)
    return FILE_ERROR;

  treasure t;
  while (read(fd, &t, sizeof(treasure)) == sizeof(treasure)) {
    if (t.id == id) {
      close(fd);
      return TREASURE_ALREADY_EXISTS;
    }
  }

  close(fd);
  return NO_ERROR;
}

int get_treasure_count(const char *path) {
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("LIST ERROR, FILE NOT FOUND");
    return FILE_NOT_FOUND;
  }

  int count = 0;
  treasure t;

  while (read(fd, &t, sizeof(treasure)) == sizeof(treasure)) {
    count++;
  }

  close(fd);
  return count;
}
