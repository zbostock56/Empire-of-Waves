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
  int status = gen_island_texture(island);
  if (status) {
    return -1;
  }
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
  island->num_items = 0;
  init_resource_buffer(island);
  spawn_items(island);
  return 0;
}

void init_resource_buffer(ISLAND *island) {
  for (int i = 0; i < ITEM_BUFFER_SIZE; i++) {
     island->item_tiles[i].type = OCEAN;
     island->item_tiles[i].resource = INVALID_REC;
     island->item_tiles[i].quantity = 0;
     island->item_tiles[i].position[0] = -1.0f;
     island->item_tiles[i].position[1] = -1.0f;
  }
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
      unsigned int tile_location = i * I_WIDTH + j;

      if (pixel > 0.7 && pixel <= 1.0) {
        // ROCK
        island->tiles[tile_location] = ROCK;
      }
      #if 0
      else if (pixel > 0.65 && pixel <= 0.7) {
        // HIGHLAND GRASS (IN THE FUTURE)
        island->tiles[tile_location] = GRASS;
      }
      #endif
      else if (pixel > 0.425 && pixel <= 0.7) {
        // LOWLAND GRASS (IN THE FUTURE)
        island->tiles[tile_location] = GRASS;
      } else if (pixel > 0.35 && pixel <= 0.425) {
        // SAND
        island->tiles[tile_location] = SAND;
      } else if (pixel > 0.0 && pixel <= 0.35) {
        // SHORE (LANDABLE)
        island->tiles[tile_location] = SHORE;
      } else {
        // WATER
        island->tiles[tile_location] = OCEAN;
      }
    }
  }
}

int gen_island_texture(ISLAND *island) {
  unsigned char *texture_buffer = malloc(NUM_TILE_CHANNELS * sizeof(char) *
                                         I_WIDTH * I_WIDTH);
  if (texture_buffer == NULL) {
    fprintf(stderr, "Unable to allocate island texture buffer\n");
    return -1;
  }

  for (int i = 0; i < I_WIDTH; i++) {
    for (int j = 0; j < I_WIDTH; j++) {
      unsigned int tile_location = i * I_WIDTH + j;
      unsigned int tex_location = (((I_WIDTH - 1 - i) * I_WIDTH) + j) *
                                  NUM_TILE_CHANNELS;
      if (island->tiles[tile_location] == ROCK) {
        texture_buffer[tex_location] = 99;
        texture_buffer[tex_location + 1] = 87;
        texture_buffer[tex_location + 2] = 67;
        texture_buffer[tex_location + 3] = 255;
      } else if (island->tiles[tile_location] == GRASS) {
        texture_buffer[tex_location] = 4;
        texture_buffer[tex_location + 1] = 209;
        texture_buffer[tex_location + 2] = 38;
        texture_buffer[tex_location + 3] = 255;
      } else if (island->tiles[tile_location] == SAND) {
        texture_buffer[tex_location] = 252;
        texture_buffer[tex_location + 1] = 243;
        texture_buffer[tex_location + 2] = 162;
        texture_buffer[tex_location + 3] = 255;
      } else if (island->tiles[tile_location] == SHORE) {
        texture_buffer[tex_location] = 3;
        texture_buffer[tex_location + 1] = 234;
        texture_buffer[tex_location + 2] = 252;
        texture_buffer[tex_location + 3] = 255;
      } else {
        texture_buffer[tex_location] = 0;
        texture_buffer[tex_location + 1] = 0;
        texture_buffer[tex_location + 2] = 0;
        texture_buffer[tex_location + 3] = 0;
      }
    }
  }

  island->texture = texture_from_buffer(texture_buffer, I_WIDTH, I_WIDTH,
                                        GL_RGBA);
  free(texture_buffer);
  return 0;
}

/*
  Spawns a new item for every island that
  is currently being simulated. If an island
  already has its item_tile's buffer full,
  an item is replaced out of the buffer randomly

  tile_info structure:
  [0]: Duplicate (0 == FALSE, 1 == TRUE)
  [1]: X location of tile
  [2]: Y location of tile
  [3]: item_tile number
*/
void spawn_new_items() {
  CHUNK *chunk;
  ISLAND *island;
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    chunk = chunk_buffer + player_chunks[i];
    for (int j = 0; j < chunk->num_islands; j++) {
      island = chunk->islands + j;
      for (int k = 0; k < island->num_items; k++) {
        /* If item has an item_tile that is not populated */
        /* then populate it. Otherwise, discard item_tile */
        /* randomly and replace with new resource         */
        if (island->item_tiles[k].quantity == 0) {
          int *tile_info = find_rand_tile(island, RETRIES_NEW_ITEM);
          if (tile_info[0] == 0) {
            /* Not duplicate location */
            island->item_tiles[k].position[0] = (float) tile_info[1];
            island->item_tiles[k].position[1] = (float) tile_info[2];
            island->item_tiles[k].quantity = 1;
            item_rng(island->item_tiles + k);
            if (island->item_tiles[k].resource == INVALID_REC) {
              fprintf(stderr, "island.c: new item respawn found to be invalid resource\n");
            }
          } else if (tile_info[0] == 1) {
            /* Duplicate location */
            island->item_tiles[tile_info[3]].quantity++;
          }
          free(tile_info);
          break;
        } else if (k == island->num_items - 1) {
          /* Unable to find open item_tile */
          int replacement = rand() % island->num_items;
          ITEM_TILES *tile = island->item_tiles + replacement;
          if (tile->quantity < 1) {
            fprintf(stderr, "island.c: Item respawning found item tile without existing item\n");
            exit(1);
          }
          tile->quantity = 1;
          item_rng(tile);
          if (tile->resource == INVALID_REC) {
            fprintf(stderr, "island.c: item replacement found to be invalid resource\n");
            exit(1);
          }
          break;
        }
      }
    }
  }
}

/*
  Helper routine to find an open tile, figure out
  if it has duplicates with other tiles, and
  return the location of the tile to modify
  and if it is a duplicate

  Return buffer structure:
  [0]: Duplicate (0 == FALSE, 1 == TRUE)
  [1]: X location of tile
  [2]: Y location of tile
  [3]: item_tile number
*/
int *find_rand_tile(ISLAND *island, int bound) {
  /* Freed in respective other helper functions  */
  int *info = (int *) malloc(sizeof(int) * 4);
  int rand_tile;
  /* Set defaults of return */
  info[0] = -1;
  info[1] = -1;
  info[2] = -1;
  info[3] = -1;
  for (int i = 0; i < bound; i++) {
    rand_tile = rand() % (I_WIDTH * I_WIDTH);
    if (island->tiles[rand_tile] == SAND ||
        island->tiles[rand_tile] == GRASS ||
        island->tiles[rand_tile] == ROCK) {
      float xloc = (float) (rand_tile % I_WIDTH);
      float yloc = (float) (rand_tile / I_WIDTH);
      int j = 0;
      while (j < island->num_items) {
        if (island->item_tiles[j].position[0] == xloc &&
            island->item_tiles[j].position[1] == yloc) {
          /* Found position is duplicate */
          info[0] = 1;
          info[3] = j;
          return info;
        }
        j++;
      }
      /* Not a duplicate location */
      info[0] = 0;
      info[1] = (int) xloc;
      info[2] = (int) yloc;
      return info;
    }
  }
  return info;
}

/*
  Items have a 0.1% chance on spawning on each tile, given
  that the tile is a land tile

  tile_info structure:
  [0]: Duplicate (0 == FALSE, 1 == TRUE)
  [1]: X location of tile
  [2]: Y location of tile
  [3]: item_tile number
*/
void spawn_items(ISLAND *island) {
  int num_potential_items = (int) ((float) I_WIDTH * (float) I_WIDTH * ITEM_SPAWN_CHANCE);
  /* Choose random tiles for the number of potential items that can spawn */
  for (int i = 0; i < num_potential_items; i++) {
    int *tile_info = find_rand_tile(island, RETRIES);
    if (tile_info[0] == 0) {
      /* Not duplicate location */
      island->item_tiles[island->num_items].position[0] = (float) tile_info[1];
      island->item_tiles[island->num_items].position[1] = (float) tile_info[2];
      island->item_tiles[island->num_items].quantity = 1;
      item_rng(island->item_tiles + island->num_items);
      if (island->item_tiles[island->num_items].resource == INVALID_REC) {
        fprintf(stderr, "island.c: initial item spawned found to be invalid resource\n");
      }
      island->num_items++;
    } else if (tile_info[0] == 1) {
      /* Duplicate location */
      island->item_tiles[tile_info[3]].quantity++;
    }
    free(tile_info);
  }
  if (island->num_items < num_potential_items) {
      island->num_items = num_potential_items;
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
    if (rand_number > spawn_chances[i]) {
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
    /* Add offset to account for REC_IDS not starting at 0 */
    tile->resource = (rand() % 3) + 8;
    break;
    /* Herb, Tea */
    case 3:
    /* Add offset to account for REC_IDS not starting at 0 */
    tile->resource = (rand() % 2) + 11;
    break;
    /* Silver Ore, Porcelain, Silk, Pearl */
    /* Gold Ore, Saffron, Amergris */
    case 4: case 5: case 6: case 7:
    case 8: case 9: case 10:
    tile->resource = category + 9;
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
      merchant->name = ((unsigned int) rand()) % NUM_NAMES;
      found_location = 1;
    }
  }
  return;
}

