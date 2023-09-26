#include <island.h>
/*
                                    ISLAND.c
Implements the functionality for procedural island generation, and random
merchant location generation.
*/

/*
  Assumes that island is already allocated from chunk generation
  Assumes that there are still islands that can be created
  Assumes that island locations are generated and checked in
  chunk generation
  Assumes that the location is already determined in chunk
  generation
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
  return;
}

int generate_rand() {
  return (int) fmod((nano_time() * 1103515245.0 + 12345.0), 2147483647.0);
}

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
