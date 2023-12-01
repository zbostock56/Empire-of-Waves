#ifndef __CONTAINER_STR__
#define __CONTAINER_STR__

#include <items.h>
#define STACK_LIMIT (5)
#define COIN_STACK_LIMIT (100)

typedef struct item_slot {
  /* Set to EMTPY if empty  */
  int item_id;
  unsigned int quantity;
} I_SLOT;

typedef struct container {
  I_SLOT *items;
  unsigned int capacity;
} CONTAINER;

#endif
