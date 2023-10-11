#ifndef __TRADE_SHIP_STR_H__
#define __TRADE_SHIP_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>

typedef struct trade_ship {
  CHUNK chunk;
  CHUNK target_chunk;
  ivec2 chunk_coords;
  vec2 coords;
  vec2 direction;
  unsigned int export_rec;
  unsigned int import_rec;
  unsigned int target_island;
  unsigned int num_mercenaries;
  float speed;
} TRADE_SHIP;

#endif
