#ifndef __MERCHANT_STR_H__
#define __MERCHANT_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>
/*
                              MERCHANT_STR.h
Describes the struct representing an NPC merchant’s state. Can be included by
any file which needs to read/manipulate merchant state.
*/

#define MAX_MERCENARIES (4)

typedef struct listing {
  int item_id;
  unsigned int quantity;
  int barter_range;
} LISTING;

/*
  State of merchant in exploration mode
*/
typedef struct merchant {
  ivec2 chunk;
  vec2 coords;
  LISTING *listings;
  unsigned int listings_buf_size;
  unsigned int num_mercenaries;
  unsigned int num_listings;
  float relationship;
} MERCHANT;

#endif
