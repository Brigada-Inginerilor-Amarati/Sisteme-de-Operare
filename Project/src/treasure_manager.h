#ifndef __TREASURE_MANAGER_H
#define __TREASURE_MANAGER_H

#define USER_NAME_MAX_LENGTH 16
#define CLUE_TEXT_MAX_LENGTH 64

typedef struct {
  char user_name[USER_NAME_MAX_LENGTH];
  double latitude;
  double longitude;
  char clue_text[CLUE_TEXT_MAX_LENGTH];
  int value;
} treasure;

typedef enum {
  OPERATION_INVALID = -1,
  HELP,
  ADD,
  LIST,
  REMOVE,
  OP_COUNT
} operation;

typedef enum {
  NO_ERROR = 1,
  DIRECTORY_NOT_FOUND,
  DIRECTORY_ERROR,
  FILE_NOT_FOUND,
  FILE_ERROR,
  SYMLINK_ERROR,
  OPERATION_FAILED,
} operation_error;

#define TREASURE_FILE_NAME "treasures.dat"
#define TREASURE_DIRECTORY "treasure_hunts"
#define LOG_DIRECTORY "logs"
#define LOG_FILE_SUFFIX ".log"

#endif
