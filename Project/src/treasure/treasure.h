#ifndef __TREASURE_H
#define __TREASURE_H
#include "../treasure_manager.h"

treasure create_treasure();
void print_treasure(const treasure *t);
int is_void_treasure(const treasure *t);

#endif
