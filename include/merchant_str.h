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
#define NUM_NAMES (600)
#define NAME_SIZE (20)

typedef struct listing {
  int item_id;
  unsigned int quantity;
} LISTING;

/*
  State of merchant in exploration mode
*/
typedef struct merchant {
  ivec2 chunk;
  vec2 coords;
  LISTING *listings;
  unsigned int listings_buf_size;
  unsigned int has_trade_route;
  unsigned int num_mercenaries;
  unsigned int num_listings;
  float relationship;
  /* Index of the global names buffer which  */
  /* indicates the name of the merchant and  */
  /* the trade ship which goes to its island */
  short name;
} MERCHANT;

#endif
