#include <chunk.h>
/*
                                    CHUNK.c
Implements the functionality for chunk loading and unloading, random chunk
generation, and detection of which chunks to load/unload.
*/

int init_chunks() {
  chunk_buffer = malloc(sizeof(CHUNK) * CHUNK_BUFF_STARTING_LEN);
  if (chunk_buffer == NULL) {
    return -1;
  }
  chunk_buff_size = CHUNK_BUFF_STARTING_LEN;
  chunk_buff_len = 0;

  ivec2 player_chunk = { 0, 0 };
  if (e_player.embarked) {
    glm_ivec2_copy(e_player.ship_chunk, player_chunk);
  } else {
    glm_ivec2_copy(e_player.chunk, player_chunk);
  }

  int status = 0;
  ivec2 chunk_coords = { 0, 0 };
  for (int i = CHUNK_UPPER_LEFT; i <= CHUNK_LOWER_RIGHT; i++) {
    glm_ivec2_add(player_chunk, CHUNK_OFFSETS[i], chunk_coords);
    status = add_chunk(chunk_coords);
    if (status == -1) {
      return -1;
    }

    player_chunks[i] = status;
  }
  return 0;
}

void print_refs() {
  printf("buff_len: %d\n", chunk_buff_len);
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    printf("coords: (%d, %d), ref_count: %d\n",
           chunk_buffer[player_chunks[i]].coords[0],
           chunk_buffer[player_chunks[i]].coords[1],
           chunk_buffer[player_chunks[i]].ref_count);
  }
  fflush(stdout);
  return;
}

void place_home(ISLAND *island, CHUNK *home_chunk) {
  for (int i = (I_WIDTH * I_WIDTH) / 2 + (I_WIDTH / 2); i < (I_WIDTH * I_WIDTH); i++) {
    if (home_chunk->islands[0].tiles[i] == GRASS) {
      home_chunk->islands[0].tiles[i] = HOME;
      house_tile[0] = (i % I_WIDTH) + island->coords[0];
      house_tile[1] = (i / I_WIDTH) + island->coords[1];
      break;
    }
  }
  unsigned char tile_colors[I_WIDTH * I_WIDTH][3];
  populate_tile_pixel_buffer(&home_chunk->islands[0], tile_colors);
  home_chunk->islands[0].texture = texture_from_buffer((unsigned char *) tile_colors,
                                                      I_WIDTH, I_WIDTH, GL_RGB);
  island->has_merchant = 0;
}

int manage_chunks() {
  if (mode != EXPLORATION) {
    return 0;
  }

  ivec2 player_chunk = { 0, 0 };
  if (e_player.embarked) {
    glm_ivec2_copy(e_player.ship_chunk, player_chunk);
  } else {
    glm_ivec2_copy(e_player.chunk, player_chunk);
  }

  ivec2 new_chunk_coords = { 0, 0 };
  // Detect updated player chunks
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    // Get chunk coordinates of chunk to load in index i
    glm_ivec2_add(player_chunk, CHUNK_OFFSETS[i], new_chunk_coords);
    int new_chunk = add_chunk(new_chunk_coords);
    if (new_chunk == -1) {
      return -1;
    }
    updated_chunks[i] = new_chunk;
  }

  // Write updated chunks to tracked player chunks
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    remove_chunk(player_chunks[i]);
    player_chunks[i] = updated_chunks[i];
  }

  TRADE_SHIP *trade_ship = NULL;
  // Detect updated trade ship chunks
  for (int i = 0; i < num_trade_ships; i++) {
    trade_ship = trade_ships + i;
    int new_chunk = add_chunk(trade_ship->chunk_coords);
    if (new_chunk == -1) {
      return -1;
    }

    trade_ship->updated_chunk_index = new_chunk;
  }

  // Write updated chunks to trade ship chunks
  for (int i = 0; i < num_trade_ships; i++) {
    remove_chunk(trade_ships[i].cur_chunk_index);
    trade_ships[i].cur_chunk_index = trade_ships[i].updated_chunk_index;
  }

  if (!house_tile[0] && !house_tile[1]) {
    /* Home asset rendering */
    /* Check if rendering the home chunk */
    for (int j = 0; j < CHUNKS_SIMULATED; j++) {
      CHUNK *cur_chunk = chunk_buffer + player_chunks[j];
      if (cur_chunk->coords[0] == 0 &&
          cur_chunk->coords[1] == 0) {
        ISLAND *island = &cur_chunk->islands[0];
        for (int i = 0; i < I_WIDTH * I_WIDTH; i++) {
          if (island->tiles[i] == HOME) {
            house_tile[0] = (i % I_WIDTH) + island->coords[0];
            house_tile[1] = (i / I_WIDTH) + island->coords[1];
          }
        }
      }

    }
  }
  return 0;
}

void free_chunk(CHUNK *chunk) {
  for (int i = 0; i < chunk->num_islands; i++) {
    if (chunk->islands[i].has_merchant) {
      free(chunk->islands[i].merchant.listings);
    }
    glDeleteTextures(1, &chunk->islands[i].texture);
  }
  free(chunk->enemies);
  chunk->coords[0] = 0xBAADF00D;
  chunk->coords[1] = 0xBAADF00D;
  chunk->ref_count = 0xBAADF00D;
}

int chunk_from_coords(ivec2 coords, CHUNK *dest) {
  // Attempt to load chunk from disk
  int status = load_chunk(coords, dest);
  if (status) {
    // Chunk also does not exist on disk, so generate it
    glm_ivec2_copy(coords, dest->coords);
    status = generate_chunk(dest);
    if (status) {
      return -1;
    }

    if (coords[0] == 0 && coords[1] == 0) {
      dest->islands[0].coords[0] = 0;
      dest->islands[0].coords[1] = 0;
      dest->islands[0].chunk[0] = 0;
      dest->islands[0].chunk[1] = 0;
      generate_island(&dest->islands[0]);
      place_home(&dest->islands[0], dest);
      vec2 home_coords = {
        dest->islands[0].coords[X],
        dest->islands[0].coords[Y]
      };
      glm_vec2_copy(home_coords, home_island_coords);
      if (!dest->num_islands) {
        dest->num_islands++;
      }
    }
  }

  return 0;
}

int add_chunk (ivec2 coords) {
  int chunk_exists = -1;
  for (unsigned int i = 0; i < chunk_buff_len; i++) {
    if (chunk_buffer[i].coords[0] == coords[0] &&
        chunk_buffer[i].coords[1] == coords[1]) {
      chunk_exists = i;
      break;
    }
  }

  // Chunk already exists in buffer, return its index
  if (chunk_exists != -1) {
    chunk_buffer[chunk_exists].ref_count++;
    return chunk_exists;
  }

  // Chunk does not exist in buffer, generate it / read it from disk
  int status = chunk_from_coords(coords, chunk_buffer + chunk_buff_len);
  if (status) {
    return -1;
  }
  int new_index = chunk_buff_len;

  chunk_buff_len++;
  if (chunk_buff_len == chunk_buff_size) {
    status = double_buffer((void **) &chunk_buffer, &chunk_buff_size,
                           sizeof(CHUNK));
    if (status) {
      return -1;
    }
  }

  chunk_buffer[new_index].ref_count = 1;
  return new_index;
}

int remove_chunk(unsigned int index) {
  if (index >= chunk_buff_len) {
    return -1;
  }

  // Decrement chunk ref count
  chunk_buffer[index].ref_count--;

  if (chunk_buffer[index].ref_count) {
    return 0;
  }

  // Remove chunk from memory if it's ref count is zero
  save_chunk(chunk_buffer + index);
  free_chunk(chunk_buffer + index);
  chunk_buff_len--;

  // Swap last chunk in buffer to replace spot of deleted chunk
  chunk_buffer[index] = chunk_buffer[chunk_buff_len];

  // Update chunk index for player and trade ships referring to the swapped
  // chunk
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    if (player_chunks[i] == chunk_buff_len) {
      player_chunks[i] = index;
    }
  }

  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    if (updated_chunks[i] == chunk_buff_len) {
      updated_chunks[i] = index;
    }
  }

  // Update ref for trade ship chunk
  for (int i = 0; i < num_trade_ships; i++) {
    if (trade_ships[i].cur_chunk_index == chunk_buff_len) {
      trade_ships[i].cur_chunk_index = index;
    }
    if (trade_ships[i].target_chunk_index == chunk_buff_len) {
      trade_ships[i].target_chunk_index = index;
    }
    if (trade_ships[i].updated_chunk_index == chunk_buff_len) {
      trade_ships[i].updated_chunk_index = index;
    }
  }

  return 0;
}

int load_chunk(ivec2 coords, CHUNK *dest) {
  // Must have some sort of max chunk to guarentee chunk file path is limited
  if (out_of_bounds(coords, MAX_CHUNK_COORD, MAX_CHUNK_COORD)) {
    fprintf(stderr, "chunk.c: Chunk (%d, %d) exceeds max chunk coordinates\n",
            coords[0], coords[1]);
    return -1;
  }

  char file_path[MAX_CHUNK_PATH_LEN];
  snprintf(file_path, MAX_CHUNK_PATH_LEN, "chunks/c_%d_%d.chunk",
           coords[0], coords[1]);
  FILE *chunk_file = fopen(file_path, "rb");
  if (chunk_file == NULL) {
    //fprintf(stderr, "chunk.c: Unable to open chunk file for reading\n");
    return -1;
  }

  fread(dest->coords, sizeof(int), 2, chunk_file);
  fread(&dest->num_islands, sizeof(unsigned int), 1, chunk_file);
  fread(&dest->num_enemies, sizeof(unsigned int), 1, chunk_file);
  if (dest->num_enemies) {
    dest->enemies = malloc(sizeof(E_ENEMY) * dest->num_enemies * 2);
    if (dest->enemies == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate chunk enemy buffer\n");
      fclose(chunk_file);
      return -1;
    }
    dest->enemy_buf_size = dest->num_enemies * 2;

    fread(dest->enemies, sizeof(E_ENEMY), dest->num_enemies, chunk_file);
  } else {
    dest->enemies = malloc(sizeof(E_ENEMY) * STARTING_BUFF_SIZE);
    if (dest->enemies == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate chunk enemy buffer\n");
      fclose(chunk_file);
      return -1;
    }
    dest->enemy_buf_size = STARTING_BUFF_SIZE;
  }

  for (unsigned int i = 0; i < dest->num_islands; i++) {
    int status = load_island(chunk_file, dest->islands + i);
    if (status) {
      free(dest->enemies);
      for (unsigned int j = 0; j < i; j++) {
        if (dest->islands[j].has_merchant) {
          free(dest->islands[j].merchant.listings);
        }
        glDeleteTextures(1, &dest->islands[j].texture);
      }
      fclose(chunk_file);
      return -1;
    }
  }

  fclose(chunk_file);

  return 0;
}

int load_island(FILE *file, ISLAND *dest) {
  fread(dest->chunk, sizeof(int), 2, file);
  fread(dest->coords, sizeof(int), 2, file);
  fread(dest->tiles, sizeof(TILE), I_WIDTH * I_WIDTH, file);
  fread(&dest->has_merchant, sizeof(int), 1, file);
  int status  = 0;
  if (dest->has_merchant) {
    status = load_merchant(file, &dest->merchant);
    if (status) {
      return -1;
    }
  }

  // TODO Create island texture buffer from preloaded tile texture buffers
  unsigned char tile_colors[I_WIDTH * I_WIDTH][3];
  populate_tile_pixel_buffer(dest, tile_colors);
  dest->texture = texture_from_buffer((unsigned char *) tile_colors, I_WIDTH,
                                      I_WIDTH, GL_RGB);

  return 0;
}

int load_merchant(FILE *file, MERCHANT *dest) {
  fread(dest->chunk, sizeof(int), 2, file);
  fread(dest->coords, sizeof(float), 2, file);
  fread(&dest->num_listings, sizeof(unsigned int), 1, file);
  fread(&dest->relationship, sizeof(float), 1, file);

  dest->listings = NULL;
  if (dest->num_listings) {
    dest->listings = malloc(sizeof(LISTING) * dest->num_listings * 2);
    dest->listings_buf_size = dest->num_listings * 2;
    if (dest->listings == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate merchant listing buffer\n");
      return -1;
    }

    fread(dest->listings, sizeof(LISTING), dest->num_listings, file);
  } else {
    dest->listings = malloc(sizeof(LISTING) * STARTING_BUFF_SIZE);
    dest->listings_buf_size = STARTING_BUFF_SIZE;
    if (dest->listings == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate merchant listing buffer\n");
      return -1;
    }
  }

  return 0;
}

void save_chunk(CHUNK *chunk) {
  // Must have some sort of max chunk to guarentee chunk file path is limited
  if (out_of_bounds(chunk->coords, MAX_CHUNK_COORD, MAX_CHUNK_COORD)) {
    fprintf(stderr, "chunk.c: Chunk exceeds max chunk coordinates\n");
    return;
  }

  char file_path[MAX_CHUNK_PATH_LEN];
  snprintf(file_path, MAX_CHUNK_PATH_LEN, "chunks/c_%d_%d.chunk",
           chunk->coords[0], chunk->coords[1]);
  file_path[MAX_CHUNK_PATH_LEN - 1] = '\0';
  FILE *chunk_file = fopen(file_path, "wb");
  if (chunk_file == NULL) {
    fprintf(stderr, "chunk.c: Unable to open chunk file for writing: %s\n",
            file_path);
    return;
  }

  fwrite(chunk->coords, sizeof(int), 2, chunk_file);
  fwrite(&chunk->num_islands, sizeof(unsigned int), 1, chunk_file);
  fwrite(&chunk->num_enemies, sizeof(unsigned int), 1, chunk_file);
  if (chunk->num_enemies) {
    fwrite(chunk->enemies, sizeof(E_ENEMY), chunk->num_enemies, chunk_file);
  }
  for (unsigned int i = 0; i < chunk->num_islands; i++) {
    save_island(chunk_file, chunk->islands + i);
  }

  fclose(chunk_file);
}

void save_island(FILE *file, ISLAND *island) {
  fwrite(island->chunk, sizeof(int), 2, file);
  fwrite(island->coords, sizeof(int), 2, file);
  fwrite(island->tiles, sizeof(TILE), I_WIDTH * I_WIDTH, file);
  fwrite(&island->has_merchant, sizeof(int), 1, file);
  if (island->has_merchant) {
    save_merchant(file, &island->merchant);
  }
}

void save_merchant(FILE *file, MERCHANT *merchant) {
  fwrite(merchant->chunk, sizeof(int), 2, file);
  fwrite(merchant->coords, sizeof(float), 2, file);
  fwrite(&merchant->num_listings, sizeof(unsigned int), 1, file);
  fwrite(&merchant->relationship, sizeof(float), 1, file);
  if (merchant->num_listings) {
    fwrite(merchant->listings, sizeof(LISTING), merchant->num_listings, file);
  }
}

/*
  Responsible for populating the chunk struct, which in turn fills
  the up the islands in the chunk, handled in island.c. Enemies will
  also be populated in the chunk in this function.

Assumptions:
  -> Chunk struct is fully allocated BEFORE function is called
*/
int generate_chunk(CHUNK *chunk) {
  srand((unsigned int) generate_rand());
  ivec2 isl_locations[MAX_ISLANDS];
  /* Generate random number to find how many islands will */
  /* be in a chunk */
  int num_islands = generate_rand() % MAX_ISLANDS;
  chunk->num_islands = num_islands;
  island_locator(isl_locations, num_islands);

  chunk->num_enemies = 0;
  chunk->enemies = malloc(sizeof(E_ENEMY) * STARTING_BUFF_SIZE);
  if (chunk->enemies == NULL) {
    fprintf(stderr, "chunk.c: failed to allocate chunk enemies buffer\n");
    return -1;
  }

  /* Set the seed for the merchant generation */
  /* Set the coords of the islands and generate subsequently */
  int status = 0;
  chunk->enemy_buf_size = STARTING_BUFF_SIZE;
  for (int i = 0; i < num_islands; i++) {
    glm_ivec2_copy(chunk->coords, chunk->islands[i].chunk);
    glm_ivec2_copy(isl_locations[i], chunk->islands[i].coords);
    status = generate_island(&chunk->islands[i]);
    if (status) {
      for (int j = 0; j < i; j++) {
        if (chunk->islands[j].has_merchant) {
          free(chunk->islands[j].merchant.listings);
        }
        glDeleteTextures(1, &chunk->islands[j].texture);
      }
      free(chunk->enemies);
      return -1;
    }
  }

  return 0;
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
  ivec2 xymax[MAX_ISLANDS];
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

char *index_to_str(int index) {
  if (index == CHUNK_UPPER_LEFT) {
    return "UPPER_LEFT";
  } else if (index == CHUNK_UP) {
    return "UP";
  } else if (index == CHUNK_UPPER_RIGHT) {
    return "UPPER_RIGHT";
  } else if (index == CHUNK_LEFT) {
    return "LEFT";
  } else if (index == PLAYER_CHUNK) {
    return "PLAYER";
  } else if (index == CHUNK_RIGHT) {
    return "RIGHT";
  } else if (index == CHUNK_LOWER_LEFT) {
    return "LOWER_LEFT";
  } else if (index == CHUNK_DOWN) {
    return "DOWN";
  } else if (index == CHUNK_LOWER_RIGHT) {
    return "LOWER_RIGHT";
  } else {
    return "INVALID";
  }
}

int out_of_bounds(ivec2 coords, int max_x, int max_y) {
  return (
    (coords[X] > 0 && coords[X] > max_x) ||
    (coords[Y] > 0 && coords[Y] > max_y) ||
    (coords[X] < 0 && coords[X] < -max_x) ||
    (coords[Y] < 0 && coords[Y] < -max_x)
  );
}

int double_buffer(void **buffer, unsigned int *buff_size,
                  unsigned int unit_size) {
  void *new_buff = realloc(*buffer, 2 * (*buff_size) * unit_size);
  if (new_buff == NULL) {
    return -1;
  }
  (*buffer) = new_buff;
  *buff_size = 2 * *buff_size;
  return 0;
}
