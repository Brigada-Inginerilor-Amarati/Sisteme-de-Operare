// ADD, LIST, REMOVE
#include "operations.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>

#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

/*
TODO: Implement the ADD operation.

Parameters: <hunt_id> -> char *
Task: Add a new treasure to the specified hunt in the treasures.dat file.
Exception: Creates a new hunt directory if it does not exist.
*/

operation_error directory_exists(const char *path) {
  DIR *dir = opendir(path);
  if (dir != NULL) {
    // Directory exists
    closedir(dir);
    return DIRECTORY_ERROR;
  }
  return NO_ERROR;
}

operation_error add_directory(char *path) {
  const char *err_msg =
      "No hunt with this id was found.\nCreating new hunt directory.\n";
  write(STDERR_FILENO, err_msg, strlen(err_msg));

  if (mkdir(path, PERMISSIONS) == -1) {
    write(STDERR_FILENO, "Failed to create directory",
          strlen("Failed to create directory"));
    return DIRECTORY_ERROR;
  }

  return NO_ERROR;
}

operation_error add_treasure(char *path) {
  if (directory_exists(path) != NO_ERROR && add_directory(path) != NO_ERROR)
    return DIRECTORY_ERROR;

  return NO_ERROR;
}

/*
TODO: Implement the LIST operation.
*/

/*
TODO: Implement the REMOVE operation.
*/
