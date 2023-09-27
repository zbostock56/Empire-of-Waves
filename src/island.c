#include <island.h>
/*
                                    ISLAND.c
Implements the functionality for procedural island generation, and random
merchant location generation.
*/

/*
  Assumptions:
    -> Island is already allocated from chunk generation
    -> There are still islands that can be created
    -> Island locations are generated and checked in
       chunk generation
*/
void generate_island(ISLAND *island) {
  float pnoise[I_WIDTH][I_WIDTH];
  float mask[I_WIDTH][I_WIDTH];
  int seed = generate_rand();
  memset(mask, 1.0, sizeof(mask));
  generate_mask(mask);
  /* Generate base perlin noise map */
  for (int i = 0; i < I_WIDTH; i++) {
    for (int j = 0; j < I_WIDTH; j++) {
      pnoise[i][j] = perlin(i, j, FREQ, DEPTH, seed) * mask[i][j];
    }
  }
  populate_tiles(island, pnoise);
  merchant_generate(&(island->merchant), island);
  return;
}

/*
  Generate a random integer based on the number of seconds
  and nano seconds of the system clock
*/
int generate_rand() {
  return (int) fmod((nano_time() * 1103515245.0 + 12345.0), MAX_INT_FLOAT);
}

/*
  Gets time from system clock with nano seconds
*/
double nano_time() {
  struct timespec tv;
  char time_str[32];
  if (clock_gettime(CLOCK_REALTIME, &tv)) {
    perror("Error getting time");
  }
  sprintf(time_str, "%ld.%.9ld", tv.tv_sec, tv.tv_nsec);
  return atof(time_str);
}

/*
  Generates mask that generally creates a square shape
*/
void generate_mask(float (*mask)[I_WIDTH]) {
  int cur_pos = 0;
  float cur_mult = 0.0;
  for (int i = 0; i < 64; i++) {
    for (int k = 0; k < I_WIDTH - (2 * cur_pos); k++) {
      mask[cur_pos][k + cur_pos] = cur_mult;
    }
    for (int k = 0; k < I_WIDTH - (2 * cur_pos); k++) {
      mask[I_WIDTH - 1 - cur_pos][k + cur_pos] = cur_mult;
    }
    for (int k = 0; k < I_WIDTH - (2 * cur_pos); k++) {
      mask[k + cur_pos][cur_pos] = cur_mult;
    }
    for (int k = 0; k < I_WIDTH - (2 * cur_pos); k++) {
      mask[k + cur_pos][I_WIDTH - 1 - cur_pos] = cur_mult;
    }
    cur_pos++;
    cur_mult += 1.0 / 32.0;
  }
}

/*
  Islands' tiles are populated by the "height map" generated
  from perlin noise. Here the values, normalized from 0.0 -> 1.0,
  are parsed to output different terrain based on their values.
*/
void populate_tiles(ISLAND *island, float (*pnoise)[I_WIDTH]) {
  for (int i = 0; i < I_WIDTH; i++) {
    for (int j = 0; j < I_WIDTH; j++) {
      float pixel = pnoise[i][j];
      if (pixel < 75 / 255.0) {
          pixel = 0.0;
      }
      int location = i * I_WIDTH + j;
      if (pixel > 0.7 && pixel <= 1.0) {
        // ROCK
        island->tiles[location] = ROCK;
      }
      #if 0
      else if (pixel > 0.65 && pixel <= 0.7) {
        // HIGHLAND GRASS (IN THE FUTURE)
        island->tiles[location] = GRASS;
      }
      #endif
      else if (pixel > 0.425 && pixel <= 0.7) {
        // LOWLAND GRASS (IN THE FUTURE)
        island->tiles[location] = GRASS;
      } else if (pixel > 0.35 && pixel <= 0.425) {
        // SAND
        island->tiles[location] = SAND;
      } else if (pixel > 0.0 && pixel <= 0.35) {
        // SHORE (LANDABLE)
        island->tiles[location] = SHORE;
      } else {
        // WATER
        island->tiles[location] = OCEAN;
      }
    }
  }
}

/*
  Reponsible for determining if a merchant will spawn, then
  putting it in its respective location if one will be spawning.

  Assumptions:
    -> On a given island, a grass tile ALREADY EXISTS
*/
void merchant_generate(MERCHANT *merchant, ISLAND *island) {
  /* determine if a merchant will be spawned */
  int chance = rand() % 10;
  if (chance < 0) {
    chance *= -1;
  }
  if (chance >= 2) {
    island->has_merchant = 0;
    return;
  }
  /*
    Find location for merchant to sit. If tile is not grass,
    merchant cannot spawn there.
  */
  int found_location = 0;
  int xloc_intra_island = 0;
  int yloc_intra_island = 0;
  int tile_location = 0;
  int total_locs = I_WIDTH * I_WIDTH;
  int num_visited = 0;
  /* Macro from island.h */
  while(!found_location) {
    get_tile_location(tile_location, I_WIDTH);
    num_visited++;
    if (num_visited > total_locs) {
      island->has_merchant = FALSE;
      break;
    }
    if (island->tiles[tile_location] == GRASS) {
      /* Location of merchant is relative to chunk coords */
      /* <>loc_intra_island = x or y location relative to */
      /* island coordinates. */
      xloc_intra_island = tile_location % I_WIDTH;
      yloc_intra_island = tile_location / I_WIDTH;
      merchant->coords[X] = island->coords[X] + xloc_intra_island;
      merchant->coords[Y] = island->coords[Y] + yloc_intra_island;
      island->has_merchant = TRUE;
      island->tiles[tile_location] = MERCH;
      found_location = 1;
    }
  }
  return;
}
