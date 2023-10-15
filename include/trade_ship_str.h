#ifndef __TRADE_SHIP_STR_H__
#define __TRADE_SHIP_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>

typedef struct trade_ship {
  ivec2 chunk_coords;
  vec2 coords;
  vec2 direction;
  // Index in chunk_buffer to chunk loaded by island
  unsigned int cur_chunk_index;
  // Index in chunk_buffer to target chunk loaded by island
  unsigned int target_chunk_index;
  // Utility index for tracking the updated index of the chunk in chunk_buffer
  // which represents the trade ship's current chunk
  unsigned int updated_chunk_index;
  unsigned int export_rec;
  unsigned int import_rec;
  unsigned int target_island;
  unsigned int num_mercenaries;
  float speed;
} TRADE_SHIP;

#endif
