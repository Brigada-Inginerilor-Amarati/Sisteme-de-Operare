#include "utils.h"

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

off_t get_treasure_file_size(const char *dir_path) {
  char file_path[PATH_MAX];
  snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, TREASURE_FILE_NAME);

  struct stat st;
  if (stat(file_path, &st) == -1) {
    perror("stat failed for size");
    return 0;
  }

  return st.st_size;
}

time_t get_treasure_file_last_modified(const char *dir_path) {
  char file_path[PATH_MAX];
  snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, TREASURE_FILE_NAME);

  struct stat st;
  if (stat(file_path, &st) == -1) {
    perror("stat failed for modification time");
    return 0;
  }

  return st.st_mtime;
}

ssize_t read_line(int fd, char *buf, size_t max_len) {
  size_t i = 0;
  char ch;
  ssize_t n;

  while (i < max_len - 1) {
    n = read(fd, &ch, 1);

    if (n == 0) {
      break; // EOF
    } else if (n < 0) {
      return -1; // read error
    }

    buf[i++] = ch;

    if (ch == '\n') {
      break;
    }
  }

  buf[i] = '\0';
  return i;
}

int extract_id(char *string) {
  int id = 0;
  // the id is the first number in the string
  sscanf(string, "%d", &id);
  return id;
}

operation_error id_exists(char *path, int id) {
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("Could not open file");
    return FILE_ERROR;
  }

  char line[LINE_MAX];
  while (read_line(fd, line, LINE_MAX)) {
    int line_id = extract_id(line);
    if (line_id == id)
      return TREASURE_ALREADY_EXISTS;
  }

  return NO_ERROR;
}
