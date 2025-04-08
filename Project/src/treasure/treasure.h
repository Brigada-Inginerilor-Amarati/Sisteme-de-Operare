#ifndef __TREASURE_H
#define __TREASURE_H

#define USER_NAME_MAX_LENGTH 16
#define CLUE_TEXT_MAX_LENGTH 64

typedef struct {
  char user_name[USER_NAME_MAX_LENGTH];
  double latitude;
  double longitude;
  char clue_text[CLUE_TEXT_MAX_LENGTH];
  int value;
} treasure;

treasure create_treasure();
void print_treasure(const treasure *t);
int is_void_treasure(const treasure *t);

#endif
