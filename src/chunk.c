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
  ivec2 isl_locations[MAX_ISLANDS];
  /* Generate random number to find how many islands will */
  /* be in a chunk */
  int num_islands = generate_rand() % MAX_ISLANDS;
  chunk->num_islands = num_islands;
  island_locator(isl_locations, num_islands);
  /* Set the seed for the merchant generation */
  srand((unsigned int) generate_rand());
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
      locs[i][X] = generate_rand() % bounds;
      locs[i][Y] = generate_rand() % bounds;
      xymax[i][X] = locs[i][X] + I_WIDTH;
      xymax[i][Y] = locs[i][Y] + I_WIDTH;
      bounds_check(xymax[i]);
      bounds_check(locs[i]);
      invalid = 0;
      for (int j = 0; j < i && !invalid; j++) {
        if (locs[i][X] >= locs[j][X] &&
            locs[i][X] <= xymax[j][X] &&
            locs[i][Y] >= locs[j][Y] &&
            locs[i][Y] <= locs[j][Y]) {
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
