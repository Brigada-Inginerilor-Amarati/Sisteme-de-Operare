#ifndef __CALCULATE_SCORES_H
#define __CALCULATE_SCORES_H

#include "../../treasure_manager/treasure_manager.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char user[USER_NAME_MAX_LENGTH];
  int total_score;
} user_score_t;

#endif
