#include <island.h>
#include <trade_str.h>
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
int generate_island(ISLAND *island) {
  float pnoise[I_WIDTH][I_WIDTH];
  float mask[I_WIDTH][I_WIDTH];
  int seed = rand();
  int depth_salt = rand() % 2;
  float freq_salt = fmod(((float) rand() / (float) rand()), 0.1);
  memset(mask, 1.0, sizeof(mask));
  generate_mask(mask);
  /* Generate base perlin noise map */
  for (int i = 0; i < I_WIDTH; i++) {
    for (int j = 0; j < I_WIDTH; j++) {
      pnoise[i][j] = perlin(i, j, FREQ + freq_salt, DEPTH + depth_salt, seed)
                     * mask[i][j];
    }
  }
  populate_tiles(island, pnoise);
  merchant_generate(&(island->merchant), island);

  if (island->has_merchant) {
    island->merchant.listings = malloc(sizeof(LISTING) * MAX_MERCHANT_ITEM);
    island->merchant.listings_buf_size = MAX_MERCHANT_ITEM;
    if (island->merchant.listings == NULL) {
      fprintf(stderr,
              "generate_island: unabled to allocate merchant listings buffer");
      return -1;
    }

    // TODO Generate random listings
    island->merchant.num_listings = 12;
    // List of items to be populated in merchant
    ITEM_IDS ids[12] = { CITRUS, RUM, LIFE_POTION, SPEED_POTION, BOW, CLOTH_ARMOR,
                         CROSSBOW, LIGHT_ARMOR, PLATE_ARMOR, GOLD_COIN,
                         SILVER_COIN, COPPER_COIN };
    for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
      if (i < island->merchant.num_listings) {
        island->merchant.listings[i].item_id = ids[i];
        island->merchant.listings[i].quantity = 1;
        if (ids[i] == GOLD_COIN || ids[i] == SILVER_COIN || ids[i] == COPPER_COIN) {
          island->merchant.listings[i].quantity = 1000;
        }
      } else {
        island->merchant.listings[i].item_id = EMPTY;
        island->merchant.listings[i].quantity = 0;
      }
    }
    island->merchant.relationship = 0.0;
    island->merchant.has_trade_route = 0;
  } else {
    island->merchant.listings = NULL;
    island->merchant.num_listings = 0;
    island->merchant.listings_buf_size = 0;
    island->merchant.relationship = 0.0;
    island->merchant.has_trade_route = 0;
  }

  // TODO Create island texture buffer from preloaded tile texture buffers
  unsigned char tile_colors[I_WIDTH * I_WIDTH][3];
  populate_tile_pixel_buffer(island, tile_colors);
  island->texture = texture_from_buffer((unsigned char *) tile_colors,
                                        I_WIDTH, I_WIDTH, GL_RGB);
  return 0;
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
  #ifndef __linux__
  sprintf(time_str, "%lld.%.9ld", tv.tv_sec, tv.tv_nsec);
  #else
  sprintf(time_str, "%ld.%.9ld", tv.tv_sec, tv.tv_nsec);
  #endif

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

void populate_tile_pixel_buffer(ISLAND *island,
                                unsigned char (*tile_colors)[3]) {
  int texel_y = 0;
  int texture_index = 0;
  for (int i = 0; i < I_WIDTH * I_WIDTH; i++) {
    texel_y = (I_WIDTH - 1) - (i / I_WIDTH);
    texture_index = (I_WIDTH * texel_y) + (i % I_WIDTH);
    if (island->tiles[texture_index] == MERCH) {
      #if 1
      tile_colors[i][0] = 255;
      tile_colors[i][1] = 0;
      tile_colors[i][2] = 0;
      #endif
      /* NOTE: Not rendering merchant debug tile */
      #if 0
      tile_colors[i][0] = 4;
      tile_colors[i][1] = 209;
      tile_colors[i][2] = 38;
      #endif
    } else if (island->tiles[texture_index] == ROCK) {
      tile_colors[i][0] = 99;
      tile_colors[i][1] = 87;
      tile_colors[i][2] = 67;
    } else if (island->tiles[texture_index] == GRASS) {
      tile_colors[i][0] = 4;
      tile_colors[i][1] = 209;
      tile_colors[i][2] = 38;
    } else if (island->tiles[texture_index] == SAND) {
      tile_colors[i][0] = 252;
      tile_colors[i][1] = 243;
      tile_colors[i][2] = 162;
    } else if (island->tiles[texture_index] == SHORE) {
      tile_colors[i][0] = 3;
      tile_colors[i][1] = 235;
      tile_colors[i][2] = 252;
    } else if (island->tiles[texture_index] == OCEAN) {
      tile_colors[i][0] = 3;
      tile_colors[i][1] = 157;
      tile_colors[i][2] = 252;
    } else if (island->tiles[texture_index] == HOME) {
      /*
      tile_colors[i][0] = 255;
      tile_colors[i][1] = 0;
      tile_colors[i][2] = 255;
      */
      /* NOTE: Note rendering home debug tile */
      tile_colors[i][0] = 4;
      tile_colors[i][1] = 209;
      tile_colors[i][2] = 38;
    } else if (island->tiles[texture_index] == CHEST) {
      tile_colors[i][0] = 4;
      tile_colors[i][1] = 209;
      tile_colors[i][2] = 38;
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
      glm_ivec2_copy(island->chunk, merchant->chunk);
      merchant->coords[X] = island->coords[X] + xloc_intra_island;
      merchant->coords[Y] = island->coords[Y] + yloc_intra_island;
      island->has_merchant = TRUE;
      island->tiles[tile_location] = MERCH;
      merchant->num_mercenaries = ((unsigned int) rand()) % MAX_MERCENARIES;
      /* Index of the global names array defined in merchant.h */
      merchant->name = ((unsigned int) generate_rand()) % NUM_NAMES;
      found_location = 1;
    }
  }
  return;
}
