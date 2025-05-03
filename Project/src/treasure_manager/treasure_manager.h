#ifndef __TREASURE_MANAGER_H
#define __TREASURE_MANAGER_H

//=============================================================================
// Dependencies
//=============================================================================

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

//=============================================================================
// Defines & Constants
//=============================================================================

#define TREASURE_FILE_NAME "treasures.bin"
#define TREASURE_DIRECTORY "treasure_hunts"
#define LOG_DIRECTORY "logs"
#define LOG_FILE_NAME "log"
#define LOG_MESSAGE_MAX 256

#define USER_NAME_MAX_LENGTH 24
#define CLUE_TEXT_MAX_LENGTH 80
#define PERMISSIONS (S_IRWXU | S_IRWXG | S_IRWXO)

//=============================================================================
// Main data structure
//=============================================================================

typedef struct {
  int id;
  char user_name[USER_NAME_MAX_LENGTH];
  int value;
  double latitude;
  double longitude;
  char clue_text[CLUE_TEXT_MAX_LENGTH];
} treasure;

// 128 bytes, no padding needed

//=============================================================================
// Enums for operation / error codes
//=============================================================================

typedef enum { OPERATION_INVALID, HELP, ADD, LIST, REMOVE } operation;

typedef enum {
  NO_ERROR,
  OPERATION_FAILED,
  FILE_NOT_FOUND,
  FILE_ERROR,
  DIRECTORY_NOT_FOUND,
  DIRECTORY_ERROR,
  SYMLINK_ERROR,
  INVALID_INPUT,
  TREASURE_NOT_FOUND,
  TREASURE_ALREADY_EXISTS
} operation_error;

#endif
