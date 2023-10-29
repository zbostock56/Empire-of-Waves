#ifndef __CONTAINER_STR___
#define __CONTAINER_STR__

#include <items.h>

typedef struct item_slot {
  int item_id;
  unsigned int quantity;
} I_SLOT;

typedef struct container {
  I_SLOT *items;
  unsigned int capacity;
} CONTAINER;

#endif
