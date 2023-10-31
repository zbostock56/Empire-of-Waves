#include <serialize.h>

/*
                                  SERIALIZE.C
  Implements the functionality for saving and loading game state from disk.
*/

int new_game(char *save_name) {
  if (strlen(save_name) >= MAX_SAVE_NAME_LEN) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  } else if (!strlen(save_name)) {
    fprintf(stderr, "serialize.c: Non-existant save name\n");
    return -1;
  }

  char save_path[MAX_PATH_LEN];
  // Generate path to save file
  int status = build_save_path(save_name, save_path, MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  }

  // Reset global state
  free_game();
  clear_unsaved_chunks();
  init_game(save_name);

  status = save_game(game_save_name);
  if (status) {
    return -1;
  }

  return 0;
}

/*
  Saves game state to disk. If the game save directory for the game save is not
  created, it will be automatically created
  Args:
  - char *save_name: Name of game save. If not already existing, will create a
                     directory of the same name under the game saves directory,
                     where the chunks and save data for the game save will be
                     stored
*/
int save_game(char *save_name) {
  if (strlen(save_name) >= MAX_SAVE_NAME_LEN) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  } else if (!strlen(save_name)) {
    fprintf(stderr, "serialize.c: Non-existant save name\n");
    return -1;
  }

  char save_path[MAX_PATH_LEN];
  // Generate path to save file
  int status = build_save_path(save_name, save_path, MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  }

  // Create save directory if it does not already exist
  status = init_save_dir(save_name);
  if (status) {
    return -1;
  }

  // Save player data, trade ships, etc...
  FILE *save_file = fopen(save_path, "wb");
  if (save_file == NULL) {
    fprintf(stderr, "serialize.c: Unable to open save file %s for writing\n",
            save_path);
    return -1;
  }
  save_state(save_file);
  fclose(save_file);

  copy_valid_path(save_name, game_save_name, strlen(save_name));
  game_save_name[strlen(save_name)] = '\0';

  consolidate_unsaved_chunks();

  for (unsigned int i = 0; i < chunk_buff_len; i++) {
    save_chunk(chunk_buffer + i, CHUNK_DIR_PATH);
  }

  return 0;
}

/*
  Loads game state from disk. If the game save directory for the game save is
  not created, the function will fail
  Args:
  - char *save_name: Name of game save
*/
int load_game(char *save_name) {
  if (strlen(save_name) >= MAX_SAVE_NAME_LEN) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  } else if (!strlen(save_name)) {
    fprintf(stderr, "serialize.c: Non-existant save name\n");
    return -1;
  }

  char save_path[MAX_PATH_LEN];
  // Generate path to save file
  int status = build_save_path(save_name, save_path, MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialize.c: Save name too long\n");
    return -1;
  }

  // read player data, trade ships, etc...
  FILE *save_file = fopen(save_path, "rb");
  if (save_file == NULL) {
    fprintf(stderr, "serialize.c: Unable to open save file %s for reading\n",
            save_path);
    return -1;
  }

  // Clear chunk buffer of possibly stale chunks with hanging references
  free_game();
  clear_unsaved_chunks();
  init_game(save_name);

  // Load player data before reinitializing chunks in order to properly load
  // player's 9 chunks
  load_player_state(save_file);

  // Load rest of the data now that the initial chunk buffer is set up
  status = load_game_state(save_file);
  if (status) {
    return -1;
  }
  fclose(save_file);

  return 0;
}

/*
  Saves a chunk to disk
  Args:
  - CHUNK *chunk: pointer to chunk to save
  - char *chunk_dir: Directory to save chunk to in save folder
*/
void save_chunk(CHUNK *chunk, char *chunk_dir) {
  // Must have some sort of max chunk to guarentee chunk file path is limited
  if (out_of_bounds(chunk->coords, MAX_CHUNK_COORD, MAX_CHUNK_COORD)) {
    fprintf(stderr, "chunk.c: Chunk exceeds max chunk coordinates\n");
    return;
  }

  if (!strlen(game_save_name)) {
    fprintf(stderr, "chunk.c: No save loaded to serialize chunk\n");
    return;
  }

  // Ensure game save directories exist
  int status = init_save_dir(game_save_name);
  if (status) {
    fprintf(stderr, "serialize.c: Failed to initialize game save directory\n");
    return;
  }

  // Generate chunk file name
  char file_name[MAX_CHUNK_NAME_LEN];
  snprintf(file_name, MAX_CHUNK_NAME_LEN, "c_%d_%d.chunk",
           chunk->coords[0], chunk->coords[1]);
  file_name[MAX_CHUNK_NAME_LEN - 1] = '\0';

  // Build path to chunk file
  char chunk_path[MAX_PATH_LEN];
  status = build_chunk_path(file_name, game_save_name, chunk_dir, chunk_path,
                            MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialize.c: Chunk filename too long\n");
    return;
  }

  chunk_to_disk(chunk_path, chunk);
}

/*
  Loads a chunk from disk
  Args:
  - ivec2 coords: coordinates of desired chunk
  - CHUNK *dest: pointer to CHUNK struct to populate with chunk data
  - char *chunk_dir: Directory to load chunk from in save directory
*/
int load_chunk(ivec2 coords, CHUNK *dest, char *chunk_dir) {
  // Must have some sort of max chunk to guarentee chunk file path is limited
  if (out_of_bounds(coords, MAX_CHUNK_COORD, MAX_CHUNK_COORD)) {
    fprintf(stderr, "serialize.c: Chunk (%d, %d) exceeds max chunk coordinates\n",
            coords[0], coords[1]);
    return -1;
  }

  if (!strlen(game_save_name)) {
    fprintf(stderr, "serialize.c: No save loaded to load chunk from\n");
    return -1;
  }

  // Ensure game save directories exist
  int status = init_save_dir(game_save_name);
  if (status) {
    fprintf(stderr, "serialize.c: Failed to initialize game save directory\n");
    return -1;
  }

  // Generate chunk file name
  char file_name[MAX_CHUNK_NAME_LEN];
  snprintf(file_name, MAX_CHUNK_NAME_LEN, "c_%d_%d.chunk",
           coords[0], coords[1]);
  file_name[MAX_CHUNK_NAME_LEN - 1] = '\0';

  // Build chunk file path
  char chunk_path[MAX_PATH_LEN];
  status = build_chunk_path(file_name, game_save_name, chunk_dir, chunk_path,
                            MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialze.c: Chunk filename too long\n");
    return -1;
  }

  status = chunk_from_disk(chunk_path, dest);
  if (status) {
    return -1;
  }

  return 0;
}

// =============================== HELPERS ===================================

/*
  Creates the directory in game files where all files related to game state are
  stored for each game save.
  File structure:

  saves
  |
  |__save_1
  |  |
  |  |__save_1.data
  |  |
  |  |__chunks
  |     |
  |     |__c_0_0.chunk
  |     |
  |     |__c_0_1.chunk
  |     |
  |     |__...
  |
  |__save_2
     |
     |__...

  Args:
  - char *save_name: name of save file
*/
int init_save_dir(char *save_name) {
  int status = create_dir(SAVE_DIR_PATH);
  if (status == -1) {
    fprintf(stderr, "serialize.c: Unable to create game save directory.\n");
    return -1;
  }

  int chunk_dir_path_len = strlen(CHUNK_DIR_PATH);
  int unsaved_dir_path_len = strlen(UNSAVED_DIR_PATH);
  int save_dir_path_len = strlen(SAVE_DIR_PATH);
  if (chunk_dir_path_len + save_dir_path_len >= MAX_PATH_LEN - 3 ||
      unsaved_dir_path_len + save_dir_path_len >= MAX_PATH_LEN - 3) {
    fprintf(stderr, "serialize.c: Invalid chunk and save directory names\n");
    return -1;
  }

  // Subtract by 3 to account for null byte and two '/'s
  int name_len = MAX_PATH_LEN - chunk_dir_path_len - save_dir_path_len - 3;
  if (strlen(save_name) < name_len) {
    name_len = strlen(save_name);
  };

  char path[MAX_PATH_LEN];
  int path_len = 0;

  // 'saves'
  copy_valid_path(SAVE_DIR_PATH, path, save_dir_path_len);
  path_len += save_dir_path_len;

  // 'saves/'
  path[path_len] = PATH_SEPERATOR;
  path_len++;

  // 'saves/player1'
  copy_valid_path(save_name, path + path_len, name_len);
  path_len += name_len;
  path[path_len] = '\0';

  // Create save dir
  status = create_dir(path);
  if (status == -1) {
    fprintf(stderr, "serialize.c: Unable to create save folder: %s\n", path);
    return -1;
  }

  // 'saves/player1/'
  path[path_len] = PATH_SEPERATOR;
  path_len++;

  // 'saves/player1/chunks
  copy_valid_path(CHUNK_DIR_PATH, path + path_len, chunk_dir_path_len);
  path_len += chunk_dir_path_len;
  path[path_len] = '\0';

  // Create chunk dir for the save
  status = create_dir(path);
  if (status == -1) {
    fprintf(stderr, "serialize.c: Unable to create chunk dir for save: %s\n",
            save_name);
    return -1;
  }

  path_len -= chunk_dir_path_len;
  copy_valid_path(UNSAVED_DIR_PATH, path + path_len, unsaved_dir_path_len);
  path_len += unsaved_dir_path_len;
  path[path_len] = '\0';

  status = create_dir(path);
  if (status == -1) {
    fprintf(stderr, "serialize.c: Unable to create unsaved dir for save: %s\n",
            save_name);
  }

  return 0;
}

/*
  Update the tracked chunks for the current game save by moving all chunk files
  in the 'unsaved' directory to the 'chunks' directory of the current game
  save
*/
void consolidate_unsaved_chunks() {
  char chunk_path[MAX_PATH_LEN];
  int status = build_chunk_path("", game_save_name, UNSAVED_DIR_PATH,
                                chunk_path, MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialze.c: Chunk filename too long\n");
    return;
  }
  // Remove stray '/' from end of chunk_path
  chunk_path[strlen(chunk_path) - 1] = '\0';

  char (*unsaved_chunks)[MAX_CHUNK_NAME_LEN] = NULL;
  unsigned int num_unsaved = 0;
  status = list_all_files(chunk_path, &unsaved_chunks, &num_unsaved);
  if (status) {
    fprintf(stderr, "serialize.c: Failed to consolidate unsaved chunks\n");
    return;
  }

  CHUNK temp_chunk;
  for (int i = 0; i < num_unsaved; i++) {
    status = build_chunk_path(unsaved_chunks[i], game_save_name,
                              UNSAVED_DIR_PATH, chunk_path, MAX_PATH_LEN);
    if (status) {
      continue;
    }

    // Load chunk from 'unsaved' directory
    status = chunk_from_disk(chunk_path, &temp_chunk);
    if (status) {
      continue;
    }
    // Remove chunk from 'unsaved' directory
    remove(chunk_path);

    status = build_chunk_path(unsaved_chunks[i], game_save_name,
                              CHUNK_DIR_PATH, chunk_path, MAX_PATH_LEN);
    if (status) {
      continue;
    }

    // Save chunk to 'chunks' directory
    chunk_to_disk(chunk_path, &temp_chunk);
    free_chunk(&temp_chunk);
  }
  free(unsaved_chunks);
}

void clear_unsaved_chunks() {
  char chunk_path[MAX_PATH_LEN];
  int status = build_chunk_path("", game_save_name, UNSAVED_DIR_PATH,
                                chunk_path, MAX_PATH_LEN);
  if (status) {
    fprintf(stderr, "serialze.c: Chunk filename too long\n");
    return;
  }
  // Remove stray '/' from end of chunk_path
  chunk_path[strlen(chunk_path) - 1] = '\0';

  char (*unsaved_chunks)[MAX_CHUNK_NAME_LEN] = NULL;
  unsigned int num_unsaved = 0;
  status = list_all_files(chunk_path, &unsaved_chunks, &num_unsaved);
  if (status) {
    return;
  }

  for (int i = 0; i < num_unsaved; i++) {
    status = build_chunk_path(unsaved_chunks[i], game_save_name,
                              UNSAVED_DIR_PATH, chunk_path, MAX_PATH_LEN);
    if (status) {
      continue;
    }

    // Remove chunk from 'unsaved' directory
    remove(chunk_path);
  }
  free(unsaved_chunks);
}

void save_state(FILE *file) {
  fwrite(&e_player, sizeof(E_PLAYER), 1, file);
  fwrite(&mode, sizeof(GAME_MODE), 1, file);
  fwrite(home_island_coords, sizeof(float), 2, file);
  fwrite(home_box_tile, sizeof(float), 2, file);
  fwrite(house_tile, sizeof(float), 2, file);
  fwrite(&global_time, sizeof(float), 1, file);

  fwrite(&num_trade_ships, sizeof(unsigned int), 1, file);
  for (int i = 0; i < num_trade_ships; i++) {
    save_trade_ship(trade_ships + i, file);
  }

  fwrite(&home_box.capacity, sizeof(unsigned int), 1, file);
  for (int i = 0; i < home_box.capacity; i++) {
    fwrite(home_box.items + i, sizeof(I_SLOT), 1, file);
  }
}

void save_trade_ship(TRADE_SHIP *ship, FILE *file) {
  CHUNK *target_chunk = chunk_buffer + ship->target_chunk_index;
  fwrite(target_chunk->coords, sizeof(int), 2, file);
  fwrite(&ship->target_island, sizeof(unsigned int), 1, file);
  fwrite(ship->chunk_coords, sizeof(int), 2, file);
  fwrite(ship->coords, sizeof(float), 2, file);
  fwrite(ship->direction, sizeof(float), 2, file);
  fwrite(&ship->export_rec, sizeof(unsigned int), 1, file);
  fwrite(&ship->import_rec, sizeof(unsigned int), 1, file);
  fwrite(&ship->num_mercenaries, sizeof(unsigned int), 1, file);
  fwrite(ship->desc, sizeof(char), MAX_TRADE_SHIP_DESC, file);
  fwrite(&ship->speed, sizeof(float), 1, file);
}

void load_player_state(FILE *file) {
  fread(&e_player, sizeof(E_PLAYER), 1, file);
}

int load_game_state(FILE *file) {
  num_trade_ships = 0;

  fread(&mode, sizeof(GAME_MODE), 1, file);
  fread(home_island_coords, sizeof(float), 2, file);
  fread(home_box_tile, sizeof(float), 2, file);
  fread(house_tile, sizeof(float), 2, file);
  fread(&global_time, sizeof(float), 1, file);

  unsigned int to_read = 0;
  fread(&to_read, sizeof(unsigned int), 1, file);
  for (unsigned int i = 0; i < to_read; i++) {
    load_trade_ship(file);
  }

  fread(&home_box.capacity, sizeof(unsigned int), 1, file);
  for (int i = 0; i < home_box.capacity; i++) {
    fread(home_box.items + i, sizeof(I_SLOT), 1, file);
  }
  return 0;
}

void load_trade_ship(FILE *file) {
  ivec2 target_chunk_coords = { 0, 0 };
  unsigned int target_island = 0;
  fread(target_chunk_coords, sizeof(int), 2, file);
  fread(&target_island, sizeof(unsigned int), 1, file);
  TRADE_SHIP *cur_ship = init_trade_ship("", target_chunk_coords,
                                         target_island);

  fread(cur_ship->chunk_coords, sizeof(float), 2, file);
  fread(cur_ship->coords, sizeof(float), 2, file);
  fread(cur_ship->direction, sizeof(float), 2, file);
  fread(&cur_ship->export_rec, sizeof(unsigned int), 1, file);
  fread(&cur_ship->import_rec, sizeof(unsigned int), 1, file);
  fread(&cur_ship->num_mercenaries, sizeof(unsigned int), 1, file);
  fread(cur_ship->desc, sizeof(char), MAX_TRADE_SHIP_DESC, file);
  fread(&cur_ship->speed, sizeof(float), 1, file);
}

void chunk_to_disk(char *chunk_path, CHUNK *chunk) {
  FILE *file = fopen(chunk_path, "wb");
  if (file == NULL) {
    fprintf(stderr, "serialize.c: Unable to open chunk file for writing: %s\n",
            chunk_path);
    return;
  }

  fwrite(chunk->coords, sizeof(int), 2, file);
  fwrite(&chunk->num_islands, sizeof(unsigned int), 1, file);
  fwrite(&chunk->num_enemies, sizeof(unsigned int), 1, file);
  if (chunk->num_enemies) {
    fwrite(chunk->enemies, sizeof(E_ENEMY), chunk->num_enemies, file);
  }
  for (unsigned int i = 0; i < chunk->num_islands; i++) {
    save_island(file, chunk->islands + i);
  }

  fclose(file);
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
  fwrite(&merchant->has_trade_route, sizeof(unsigned int), 1, file);
  fwrite(&merchant->num_mercenaries, sizeof(unsigned int), 1, file);
  fwrite(&merchant->num_listings, sizeof(unsigned int), 1, file);
  fwrite(&merchant->relationship, sizeof(float), 1, file);
  fwrite(&merchant->name, sizeof(short), 1, file);
  if (merchant->num_listings) {
    fwrite(merchant->listings, sizeof(LISTING), merchant->num_listings, file);
  }
}

int chunk_from_disk(char *chunk_path, CHUNK *dest) {
  FILE *file = fopen(chunk_path, "rb");
  if (file == NULL) {
    return -1;
  }

  fread(dest->coords, sizeof(int), 2, file);
  fread(&dest->num_islands, sizeof(unsigned int), 1, file);
  fread(&dest->num_enemies, sizeof(unsigned int), 1, file);
  if (dest->num_enemies) {
    dest->enemies = malloc(sizeof(E_ENEMY) * dest->num_enemies * 2);
    if (dest->enemies == NULL) {
      fprintf(stderr, "serialize.c: Unable to allocate chunk enemy buffer\n");
      fclose(file);
      return -1;
    }
    dest->enemy_buf_size = dest->num_enemies * 2;

    fread(dest->enemies, sizeof(E_ENEMY), dest->num_enemies, file);
  } else {
    dest->enemies = malloc(sizeof(E_ENEMY) * STARTING_BUFF_SIZE);
    if (dest->enemies == NULL) {
      fprintf(stderr, "serialze.c: Unable to allocate chunk enemy buffer\n");
      fclose(file);
      return -1;
    }
    dest->enemy_buf_size = STARTING_BUFF_SIZE;
  }

  for (unsigned int i = 0; i < dest->num_islands; i++) {
    int status = load_island(file, dest->islands + i);
    if (status) {
      free(dest->enemies);
      for (unsigned int j = 0; j < i; j++) {
        if (dest->islands[j].has_merchant) {
          free(dest->islands[j].merchant.listings);
        }
        glDeleteTextures(1, &dest->islands[j].texture);
      }
      fclose(file);
      return -1;
    }
  }

  fclose(file);
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
  fread(&dest->has_trade_route, sizeof(unsigned int), 1, file);
  fread(&dest->num_mercenaries, sizeof(unsigned int), 1, file);
  fread(&dest->num_listings, sizeof(unsigned int), 1, file);
  fread(&dest->relationship, sizeof(float), 1, file);
  fread(&dest->name, sizeof(short), 1, file);

  dest->listings = NULL;
  if (dest->num_listings) {
    dest->listings = malloc(sizeof(LISTING) * dest->num_listings * 2);
    dest->listings_buf_size = dest->num_listings * 2;
    if (dest->listings == NULL) {
      fprintf(stderr, "serialize.c: Unable to allocate merchant listing buffer\n");
      return -1;
    }

    fread(dest->listings, sizeof(LISTING), dest->num_listings, file);
  } else {
    dest->listings = malloc(sizeof(LISTING) * STARTING_BUFF_SIZE);
    dest->listings_buf_size = STARTING_BUFF_SIZE;
    if (dest->listings == NULL) {
      fprintf(stderr, "serialize.c: Unable to allocate merchant listing buffer\n");
      return -1;
    }
  }

  return 0;
}

/*
  Platform independent wrapper function for creating a directory on windows
  and linux. Utilizes the "make_dir" macro, which was defined for each
  platform.

  Returns:
  - 0 if directory is successfully created or already exists
  - -1 if directory was unable to be created
*/
int create_dir(char *path) {
  int status = make_dir(path);
  if (status == -1 && errno != EEXIST) {
    return -1;
  }

  return 0;
}

/*
  Copys a given path into a target buffer given some length. Additionally,
  if a character in path is not alpha numeric, it is replaced with '_'
*/
void copy_valid_path(const char *path, char *dest, int len) {
  char cur = ' ';
  for (int i = 0; i < len; i++) {
    cur = path[i];
    if ((cur < '0' || cur > '9') &&
        (cur < 'A' || cur > 'Z') &&
        (cur < 'a' || cur > 'z')) {
      // If character in path is not alpha-numeric, replace it with '_'
      cur = '_';
    }
    dest[i] = cur;
  }
}

/*
  Generates the path to a save file given the name of the save file
  Returns:
  - 0 if successful
  - -1 if the name of the save will cause the path to exceed the size of the
    destination buffer
*/
int build_save_path(char *save_name, char *dest, int dest_buf_size) {
  // Calculates length of path to save file:
  // [SAVE_DIR_PATH]/[save_name]/[save_name][SAVE_FILE_EXTENSION]
  // +2 to account for the two '/'s
  int save_path_len = strlen(SAVE_DIR_PATH) + (2 * strlen(save_name)) +
                      strlen(SAVE_FILE_EXTENSION) + 2;
  if (save_path_len >= dest_buf_size) {
    return -1;
  }

  // Build path of save file. Ex:
  // 'saves/save_1/save_1.data'
  int cur_len = 0;
  copy_valid_path(SAVE_DIR_PATH, dest, strlen(SAVE_DIR_PATH));
  cur_len += strlen(SAVE_DIR_PATH);
  dest[cur_len] = PATH_SEPERATOR;
  cur_len++;
  copy_valid_path(save_name, dest + cur_len, strlen(save_name));
  cur_len += strlen(save_name);
  dest[cur_len] = PATH_SEPERATOR;
  cur_len++;
  copy_valid_path(save_name, dest + cur_len, strlen(save_name));
  cur_len += strlen(save_name);
  memcpy(dest + cur_len, SAVE_FILE_EXTENSION, strlen(SAVE_FILE_EXTENSION));
  cur_len += strlen(SAVE_FILE_EXTENSION);
  dest[cur_len] = '\0';

  return 0;
}

/*
  Generates the path to a chunk file given the name of the chunk file
  Returns:
  - 0 if successful
  - -1 if the name of the chunk will cause the path to exceed the size of the
    destination buffer
*/
int build_chunk_path(char *chunk_name, char *save_name, char *chunk_dir,
                     char *dest, int dest_buf_size) {
  // Calculates length of path to chunk file:
  // [SAVE_DIR_PATH]/[save_name]/[CHUNK_DIR_PATH]/[chunk_name]
  // +3 to account for the three '/'s
  int chunk_path_len = strlen(SAVE_DIR_PATH) + strlen(save_name) +
                       strlen(CHUNK_DIR_PATH) + strlen(chunk_name) + 3;
  if (chunk_path_len >= dest_buf_size) {
    return -1;
  }

  // Build path of save file. Ex:
  // 'saves/[SAVE_NAME]/chunks/c_0_0.chunk'
  int cur_len = 0;
  copy_valid_path(SAVE_DIR_PATH, dest, strlen(SAVE_DIR_PATH));
  cur_len += strlen(SAVE_DIR_PATH);
  dest[cur_len] = PATH_SEPERATOR;
  cur_len++;
  copy_valid_path(save_name, dest + cur_len, strlen(save_name));
  cur_len += strlen(save_name);
  dest[cur_len] = PATH_SEPERATOR;
  cur_len++;
  copy_valid_path(chunk_dir, dest + cur_len, strlen(chunk_dir));
  cur_len += strlen(chunk_dir);
  dest[cur_len] = PATH_SEPERATOR;
  cur_len++;
  memcpy(dest + cur_len, chunk_name, strlen(chunk_name));
  cur_len += strlen(chunk_name);
  dest[cur_len] = '\0';

  return 0;
}

/*
  Reads all files in a directory and populates dest_list with a dynamically
  allocated list of the filenames
  Args:
  - char *dir: Path of directory to read
  - char (**dest_list)[]: Pointer to filename buffer. Will be dynamically
    allocated. Callers responsibility to free.
  - unsigned int *dest_len: Pointer to length of *dest_list. Will be populated
    upon filling *dest_list with names
  Returns:
  - 0 if successful. *dest_list must be freed
  - -1 if unsuccessful. *dest_list does not have to be freed
*/
int list_all_files(char *dir, char (**dest_list)[MAX_CHUNK_NAME_LEN],
                   unsigned int *dest_len) {
  *dest_list = malloc(sizeof(char) * MAX_CHUNK_NAME_LEN *
                      FILENAME_BUF_STARTING_LEN);
  unsigned int dest_list_size = FILENAME_BUF_STARTING_LEN;
  (*dest_len) = 0;
  if ((*dest_list) == NULL) {
    return -1;
  }

  int status = 0;
  // Linux directory i/o
#ifdef __linux__
  DIR *cur_dir = opendir(dir);
  if (!cur_dir) {
    free(*dest_list);
    return -1;
  }

  struct dirent *cur_file = readdir(cur_dir);
  int cur_filename_len = 0;
  while (cur_file) {
    cur_filename_len = strlen(cur_file->d_name);
    if (cur_file->d_type != DT_REG || cur_filename_len >= MAX_CHUNK_NAME_LEN) {
      cur_file = readdir(cur_dir);
      continue;
    }

    strncpy((*dest_list)[*dest_len], cur_file->d_name, MAX_CHUNK_NAME_LEN);
    (*dest_len)++;
    if (*dest_len == dest_list_size) {
      status = double_buffer((void **) dest_list, &dest_list_size,
                             MAX_CHUNK_NAME_LEN * sizeof(char));
      if (status) {
        closedir(cur_dir);
        return -1;
      }
    }

    cur_file = readdir(cur_dir);
  }
  closedir(cur_dir);
#endif

  // Windows directory i/o
#ifdef _WIN32
  char dir_path[MAX_PATH_LEN];
  // dir_path will have '\*' appended to the end of 'dir', so ensure there is
  // enough space in dir_path
  if (strlen(dir) >= MAX_PATH_LEN - 3) {
    free(dest_len);
    return -1;
  }
  snprintf(dir_path, MAX_PATH_LEN, "%s\\*", dir);

  WIN32_FIND_DATA cur_file;
  HANDLE cur_dir = FindFirstFile(dir_path, &cur_file);
  if (cur_dir == INVALID_HANDLE_VALUE) {
    free(*dest_list);
    return -1;
  }

  int reading = 1;
  int cur_filename_len = strlen(cur_file.cFileName);
  while (reading) {
    if (cur_filename_len >= MAX_CHUNK_NAME_LEN) {
      reading = FindNextFile(cur_dir, &cur_file);
      continue;
    }

    strncpy((*dest_list)[*dest_len], cur_file.cFileName, MAX_CHUNK_NAME_LEN);
    (*dest_len)++;
    if (*dest_len == dest_list_size) {
      status = double_buffer((void **) dest_list, &dest_list_size,
                             MAX_CHUNK_NAME_LEN * sizeof(char));
      if (status) {
        FindClose(cur_dir);
        return -1;
      }
    }

    reading = FindNextFile(cur_dir, &cur_file);
  }
  FindClose(cur_dir);
#endif
  return 0;
}
