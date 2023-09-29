#include <chunk.h>
/*
                                    CHUNK.c
Implements the functionality for chunk loading and unloading, random chunk
generation, and detection of which chunks to load/unload.
*/

/*
  Responsible for populating the chunk struct, which in turn fills
  the up the islands in the chunk, handled in island.c. Enemies will
  also be populated in the chunk in this function.

Assumptions:
  -> Chunk struct is fully allocated BEFORE function is called
*/
void generate_chunk(CHUNK *chunk) {
  srand((unsigned int) generate_rand());
  ivec2 isl_locations[MAX_ISLANDS];
  /* Generate random number to find how many islands will */
  /* be in a chunk */
  int num_islands = generate_rand() % MAX_ISLANDS;
  chunk->num_islands = num_islands;
  island_locator(isl_locations, num_islands);
  /* Set the seed for the merchant generation */
  /* Set the coords of the islands and generate subsequently */
  for (int i = 0; i < num_islands; i++) {
    chunk->islands[i].coords[X] = isl_locations[i][X];
    chunk->islands[i].coords[Y] = isl_locations[i][Y];
    generate_island(&chunk->islands[i]);
  }
  return;
}

/*
  Responisble for finding three locations in which islands can
  be generated such that they do not run into each other.

  NOTE: Island location values are relative to the TOP-LEFT
  point of their position

  Respresentation:
         X - Coords
  -----------------------
  |*                    |
  |                     |  Y
  |                     |  |
  |                     |  C
  |                     |  o
  |                     |  o
  |                     |  r
  |                     |  d
  |                     |  s
  |                     |
  -----------------------
  Island location is denoted by '*' in picture
*/
void island_locator(ivec2 *locs, int num_islands) {
  /* Subtract 1 to be zero-indexed */
  int bounds = C_WIDTH - I_WIDTH - 1;
  ivec2 xymax[3];
  /* generates random number then takes the remainer */
  /* of the random number relative to the bounds and */
  /* sets that the position of the island */
  int invalid = 0;
  for (int i = 0; i < num_islands; i++) {
    do {
      locs[i][X] = rand() % bounds;
      locs[i][Y] = rand() % bounds;
      xymax[i][X] = locs[i][X] + I_WIDTH;
      xymax[i][Y] = locs[i][Y] + I_WIDTH;
      bounds_check(xymax[i]);
      bounds_check(locs[i]);
      invalid = 0;
      for (int j = 0; j < i && !invalid; j++) {
        if (locs[i][X] <= (locs[j][X] + xymax[j][X]) &&
            (locs[i][X] + xymax[i][X]) >= locs[j][X] &&
            locs[i][Y] <= (locs[j][Y] + xymax[j][Y]) &&
            (locs[i][Y] + xymax[i][Y]) >= locs[j][Y]) {
          bounds_check(xymax[j]);
          bounds_check(locs[j]);
          invalid = 1;
        } else {
          invalid = 0;
        }
      }
    } while (invalid);
  }
}

void bounds_check(ivec2 loc) {
  if (loc[X] > C_WIDTH || loc[Y] > C_WIDTH || loc[X] < 0 || loc[Y] < 0) {
    fprintf(stderr, "ISLAND LOCATION OUT OF CHUNK\n");
    exit(0);
  }
  return;
}

/*
  Converts chunk coordinates to world coordinates
*/
void chunk_to_world(ivec2 chunk, vec2 chunk_coords, vec2 dest) {
  vec2 chunk_origin = {
    (chunk[0] * C_WIDTH * T_WIDTH) - (C_WIDTH * T_WIDTH / 2.0),
    (chunk[1] * C_WIDTH * T_WIDTH) + (C_WIDTH * T_WIDTH / 2.0)
  };
  vec2 to_tile = {
    chunk_coords[0] * T_WIDTH,
    -chunk_coords[1] * T_WIDTH
  };

  glm_vec2_add(chunk_origin, to_tile, dest);
}

/*
  Converts world coordinates to chunk coordinates
*/
void world_to_chunk(vec2 world, ivec2 chunk, vec2 chunk_coords) {
  if (world[0] >= 0.0) {
    chunk[0] = (world[0] + (0.5 * T_WIDTH * C_WIDTH)) / (T_WIDTH * C_WIDTH);
  } else {
    chunk[0] = (world[0] - (0.5 * T_WIDTH * C_WIDTH)) / (T_WIDTH * C_WIDTH);
  }
  if (world[1] >= 0.0) {
    chunk[1] = (world[1] + (0.5 * T_WIDTH * C_WIDTH)) / (T_WIDTH * C_WIDTH);
  } else {
    chunk[1] = (world[1] - (0.5 * T_WIDTH * C_WIDTH)) / (T_WIDTH * C_WIDTH);
  }

  vec2 chunk_origin = {
    (chunk[0] * C_WIDTH) - (C_WIDTH / 2.0),
    (chunk[1] * C_WIDTH) + (C_WIDTH / 2.0)
  };

  chunk_coords[0] = (world[0] / T_WIDTH) - chunk_origin[0];
  chunk_coords[1] = chunk_origin[1] - (world[1] / T_WIDTH);
}
