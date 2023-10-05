#include <chunk.h>
/*
                                    CHUNK.c
Implements the functionality for chunk loading and unloading, random chunk
generation, and detection of which chunks to load/unload.
*/

void init_chunks() {
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
    printf("loading new: %s (%d) at (%d, %d)\n", index_to_str(i), i,
           chunk_coords[X], chunk_coords[Y]);
    fflush(stdout);
    status = load_chunk(chunk_coords, player_chunks + i);
    if (status) {
      glm_ivec2_copy(chunk_coords, player_chunks[i].coords);
      generate_chunk(player_chunks + i);
    }
  }
}

void manage_chunks() {
  ivec2 player_chunk = { 0, 0 };
  if (e_player.embarked) {
    glm_ivec2_copy(e_player.ship_chunk, player_chunk);
  } else {
    glm_ivec2_copy(e_player.chunk, player_chunk);
  }

  // Updated list of chunks
  CHUNK updated_chunks[9];
  // Array specifying which of the current chunks are no longer in use, and
  // thus must be saved to disk
  unsigned int to_serialize[9] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
  };

  int status = 0;
  ivec2 new_chunk_coords = { 0, 0 };
  ivec2 new_chunk_offset = { 0, 0 };
  for (int i = CHUNK_UPPER_LEFT; i <= CHUNK_LOWER_RIGHT; i++) {
    // Get chunk coordinates of chunk to load in index i
    glm_ivec2_add(player_chunk, CHUNK_OFFSETS[i], new_chunk_coords);

    // Check if the chunk to load is already loaded in the needed spot
    if (new_chunk_coords[X] != player_chunks[i].coords[X] ||
        new_chunk_coords[Y] != player_chunks[i].coords[Y]) {
      glm_ivec2_sub(new_chunk_coords, player_chunks[PLAYER_CHUNK].coords,
                    new_chunk_offset);
      // Check if the new chunk to load already exists in memory
      if (out_of_bounds(new_chunk_offset, 1, 1)) {
        // New chunk does not currently exist in memory, load it up
        printf("loading new: %s (%d) at (%d, %d)\n", index_to_str(i), i,
               new_chunk_coords[X], new_chunk_coords[Y]);
        fflush(stdout);
        status = load_chunk(new_chunk_coords, updated_chunks + i);
        if (status) {
          // Chunk also does not exist on disk, so generate it
          glm_ivec2_copy(new_chunk_coords, updated_chunks[i].coords);
          generate_chunk(updated_chunks + i);
        }
      } else {
        int new_chunk_index = get_index(new_chunk_offset);
        // New chunk does exist in memory, relocate it to the correct index
        updated_chunks[i] = player_chunks[new_chunk_index];
        to_serialize[new_chunk_index] = 0;
      }
    } else {
      updated_chunks[i] = player_chunks[i];
      to_serialize[i] = 0;
    }
  }

  // Update global chunk state with newly loaded/ordered chunks, and save all
  // unused chunks to disk
  for (int i = CHUNK_UPPER_LEFT; i <= CHUNK_LOWER_RIGHT; i++) {
    if (to_serialize[i]) {
      printf("serializing: %s (%d)\n", index_to_str(i), i);
      fflush(stdout);
      save_chunk(player_chunks + i);
      free_chunk(player_chunks + i);
    }

    player_chunks[i] = updated_chunks[i];
  }
}

void free_chunk(CHUNK *chunk) {
  for (int i = 0; i < chunk->num_islands; i++) {
    free(chunk->islands[i].merchant.listings);
    glDeleteTextures(1, &chunk->islands[i].texture);
  }
  free(chunk->enemies);
}

int load_chunk(ivec2 coords, CHUNK *dest) {
  // Must have some sort of max chunk to guarentee chunk file path is limited
  if (out_of_bounds(coords, MAX_CHUNK_COORD, MAX_CHUNK_COORD)) {
    fprintf(stderr, "chunk.c: Chunk exceeds max chunk coordinates\n");
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
    dest->enemies = malloc(sizeof(E_ENEMY) * dest->num_enemies);
    if (dest->enemies == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate chunk enemy buffer\n");
      return -1;
    }

    fread(dest->enemies, sizeof(E_ENEMY), dest->num_enemies, chunk_file);
  } else {
    dest->enemies = NULL;
  }
  for (unsigned int i = 0; i < dest->num_islands; i++) {
    int status = load_island(chunk_file, dest->islands + i);
    if (status != 0) {
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
  if (dest->has_merchant) {
    return load_merchant(file, &dest->merchant);
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
  if (dest->num_listings) {
    dest->listings = malloc(sizeof(LISTING) * dest->num_listings);
    if (dest->listings == NULL) {
      fprintf(stderr, "chunk.c: Unable to allocate merchant listing buffer\n");
      return -1;
    }

    fread(dest->listings, sizeof(LISTING), dest->num_listings, file);
  } else {
    dest->listings = NULL;
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
    glm_ivec2_copy(chunk->coords, chunk->islands[i].chunk);
    glm_ivec2_copy(isl_locations[i], chunk->islands[i].coords);
    generate_island(&chunk->islands[i]);
  }

  // TODO initialize enemy buffer
  chunk->num_enemies = 0;
  chunk->enemies = NULL;

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

int get_index(ivec2 offset) {
  int x = offset[X] + 1;
  int y = 1 - offset[Y];
  return x + (3 * y);
}

int out_of_bounds(ivec2 coords, int max_x, int max_y) {
  return (
    (coords[X] > 0 && coords[X] > max_x) ||
    (coords[Y] > 0 && coords[Y] > max_y) ||
    (coords[X] < 0 && coords[X] < -max_x) ||
    (coords[Y] < 0 && coords[Y] < -max_x)
  );
}
