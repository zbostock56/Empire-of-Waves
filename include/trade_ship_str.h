#ifndef __TRADE_SHIP_STR_H__
#define __TRADE_SHIP_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>

#define MERCENARIES_CAPACITY (10)
#define MAX_TRADE_SHIP_DESC (64)
#define STEER_SPEED (3.0)

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
  // Number of mercenaries assigned to the trade ship
  unsigned int num_mercenaries;
  int moving;
  // Used to describe the name of the trade ship
  // when adding mercenaries to it
  char desc[MAX_TRADE_SHIP_DESC];
  float speed;
  // Indicates the current step in the death animation of the trade ship in the
  // case it is plundered by an enemy ship. If -1.0, the trade ship has not yet
  // been plundered. If > 0.0, the death animation is currently active, and if
  // 0.0, the trade_ship is ready for deletion
  float death_animation;
} TRADE_SHIP;

#endif
