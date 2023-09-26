#include <chunk.h>
/*
                                    CHUNK.c
Implements the functionality for chunk loading and unloading, random chunk
generation, and detection of which chunks to load/unload.
*/

/*
  Converts chunk coordinates to world coordinates
*/
void chunk_to_world(ivec2 chunk, vec2 chunk_coords, vec2 dest) {
  vec2 to_chunk = { chunk[0] * C_WIDTH * T_WIDTH,
                    chunk[1] * C_WIDTH * T_WIDTH };
  vec2 to_tile = { chunk_coords[0] * T_WIDTH,
                   (C_WIDTH - chunk_coords[1]) * T_WIDTH };

  glm_vec2_add(to_chunk, to_tile, dest);
}

/*
  Converts world coordinates to chunk coordinates
*/
void world_to_chunk(vec2 world, ivec2 chunk, vec2 chunk_coords) {
  chunk[0] = world[0] / (C_WIDTH * T_WIDTH);
  chunk[1] = world[1] / (C_WIDTH * T_WIDTH);
  chunk_coords[0] = (world[0] / T_WIDTH) - (chunk[0] * C_WIDTH);
  chunk_coords[1] = C_WIDTH - ((world[1] / T_WIDTH) - (chunk[1] * C_WIDTH));
}
