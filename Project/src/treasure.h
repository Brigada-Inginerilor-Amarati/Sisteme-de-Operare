#ifndef __TREASURE_H
#define __TREASURE_H

#define USER_NAME_MAX_LENGTH 40
#define CLUE_TEXT_MAX_LENGTH 64

typedef struct {
  char user_name[USER_NAME_MAX_LENGTH];
  double latitude;
  double longitude;
  char clue_text[CLUE_TEXT_MAX_LENGTH];
  int value;
} treasure;

#endif
