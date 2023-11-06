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

  spawn_items(island);
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
  for (int i = 0; i < I_WIDTH; i++) {
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
  Items have a 0.1% chance on spawning on each tile, given
  that the tile is a land tile
*/
void spawn_items(ISLAND *island) {
  int num_potential_items = (int) ((float) I_WIDTH * (float) I_WIDTH * ITEM_SPAWN_CHANCE);
  /* Choose random tiles for the number of potential items that can spawn */
  int rand_tile;
  int retry = 0;
  for (int i = 0; i < num_potential_items; i++) {
    rand_tile = rand() % I_WIDTH;
    if ((island->tiles[rand_tile] == SAND ||
        island->tiles[rand_tile] == GRASS ||
        island->tiles[rand_tile] == ROCK) &&
        retry < 3 &&
        island->item_tiles[island->num_items].quantity == 0) {
      /* Found open land tile */
      island->item_tiles[island->num_items].position[0] = (float) (rand_tile % I_WIDTH);
      island->item_tiles[island->num_items].position[1] = (float) (rand_tile / I_WIDTH);
      island->item_tiles[island->num_items].type = island->tiles[rand_tile];
      island->item_tiles[island->num_items].quantity++;
      item_rng(island->item_tiles + island->num_items);
      island->num_items++;
    } else if ((island->tiles[rand_tile] == SAND ||
        island->tiles[rand_tile] == GRASS ||
        island->tiles[rand_tile] == ROCK) &&
        retry < 3 &&
        island->item_tiles[island->num_items].quantity > 0) {
      /* Tile already has something there */
      /* Increment the item that is stored there */
      /* to indicate that another item has spawned there */
      island->item_tiles[island->num_items].quantity++;
    } else if (retry >= 3) {
      /* Retries exhausted */
      retry = 0;
    } else {
      /* Retry item placement if did not find land tile */
      retry++;
      i--;
    }
  }
}

/*
  Determines which item should be placed on the tile

  RESOURCE RNG TABLE:

  Name       | Value  |  Spawn Chance
  -----------|--------|----------------
  Grain      |   5    |      30%
  Cotton     |   6    |       ^
  Wool       |   8    |       ^
  Dyes       |   8    |       ^
  Sugar      |   10   |       ^
  Leather    |   12   |      25%
  Cheese     |   15   |       ^
  Iron Ore   |   18   |       ^
  Wine       |   20   |      15%
  Copper Ore |   20   |       ^
  Spice      |   25   |       ^
  Herb       |   28   |      10%
  Tea        |   30   |       ^
  Silver Ore |   30   |      7%
  Porcelain  |   35   |      5%
  Silk       |   40   |      3%
  Pearl      |   45   |      2%
  Gold Ore   |   50   |      1.5%
  Saffron    |   55   |      1%
  Ambergris  |   60   |      0.5%
*/
void item_rng(ITEM_TILES *tile) {
  int spawn_chances[12] = {
    300, 250, 150, 100,
    70, 50, 30, 20, 10,
    15, 10, 5
  };
  int rand_number = rand() % 1000;
  int category = 0;
  for (int i = 0; i < 12; i++) {
    if (rand_number < spawn_chances[i]) {
      rand_number -= spawn_chances[i];
    } else {
      category = i;
      break;
    }
  }
  switch (category) {
    /* Grain, Cotton, Wool, Dyes, Sugar */
    case 0:
    tile->resource = (rand() % 5);
    break;
    /* Leather, Cheese, Iron Ore */
    case 1:
    /* Add offset to account for REC_IDS not starting at 0 */
    tile->resource = (rand() % 3) + 5;
    break;
    /* Wine, Copper Ore, Spice */
    case 2:
    tile->resource = (rand() % 3) + 8;
    break;
    /* Herb, Tea */
    case 3:
    tile->resource = (rand() % 2) + 11;
    break;
    /* Silver Ore  */
    case 4:
    tile->resource = 13;
    break;
    /* Porcelain */
    case 5:
    tile->resource = 14;
    break;
    /* Silk */
    case 6:
    tile->resource = 15;
    break;
    /* Pearl */
    case 7:
    tile->resource = 16;
    break;
    /* Gold Ore */
    case 8:
    tile->resource = 17;
    break;
    /* Saffron */
    case 9:
    tile->resource = 18;
    break;
    /* Ambergris */
    case 10:
    tile->resource = 19;
    break;
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

