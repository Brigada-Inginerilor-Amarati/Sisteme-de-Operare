#include "manager_utils.h"

#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

operation_error add_file(const char *path) {
  // check if the file exists
  if (open(path, F_OK) == -1) {
    perror("File does not exist");
    // create the file
    int fd = open(path, O_CREAT | O_WRONLY, PERMISSIONS);
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

operation_error add_directory(char *path) {
  const char *err_msg =
      "No hunt with this id was found.\nCreating new hunt directory.\n";
  write(STDERR_FILENO, err_msg, strlen(err_msg));

  if (mkdir(path, PERMISSIONS) == -1) {
    perror("Could not create directory");
    return DIRECTORY_ERROR;
  }

  return NO_ERROR;
}

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

operation_error id_exists(char *file_path, int id) {
  int fd = open(file_path, O_RDONLY);
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

int get_treasure_count(char *path) {
  char treasure_file_path[PATH_MAX];
  snprintf(treasure_file_path, PATH_MAX, "%s/%s/%s", TREASURE_DIRECTORY, path,
           TREASURE_FILE_NAME);

  int fd = open(treasure_file_path, O_RDONLY);
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
