#include <serialize.h>

/*
                                  SERIALIZE.C
  Implements the functionality for saving and loading game state from disk.
*/

int save_game(char *save_name) {
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

  printf("Save path: %s\n", save_path);
  fflush(stdout);

  FILE *save_file = fopen(save_path, "wb");
  // Save player data, trade ships, etc...
  fclose(save_file);

  return 0;
}

/*
void load_game() {
}
*/

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
  int save_dir_path_len = strlen(SAVE_DIR_PATH);
  if (chunk_dir_path_len + save_dir_path_len >= MAX_PATH_LEN - 3) {
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
  path[path_len] = '/';
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
  path[path_len] = '/';
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
  memcpy(dest, SAVE_DIR_PATH, strlen(SAVE_DIR_PATH));
  cur_len += strlen(SAVE_DIR_PATH);
  dest[cur_len] = '/';
  cur_len++;
  memcpy(dest + cur_len, save_name, strlen(save_name));
  cur_len += strlen(save_name);
  dest[cur_len] = '/';
  cur_len++;
  memcpy(dest + cur_len, save_name, strlen(save_name));
  cur_len += strlen(save_name);
  memcpy(dest + cur_len, SAVE_FILE_EXTENSION,
         strlen(SAVE_FILE_EXTENSION));
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
int build_chunk_path(char *chunk_name, char *dest, int dest_buf_size) {
  // Calculates length of path to chunk file:
  // [SAVE_DIR_PATH]/[CHUNK_DIR_PATH]/[chunk_name]
  // +2 to account for the two '/'s
  int chunk_path_len = strlen(SAVE_DIR_PATH) + strlen(CHUNK_DIR_PATH) +
                       strlen(chunk_name) + 2;
  if (chunk_path_len >= dest_buf_size) {
    return -1;
  }

  // Build path of save file. Ex:
  // 'saves/chunks/c_0_0.chunk'
  int cur_len = 0;
  memcpy(dest, SAVE_DIR_PATH, strlen(SAVE_DIR_PATH));
  cur_len += strlen(SAVE_DIR_PATH);
  dest[cur_len] = '/';
  cur_len++;
  memcpy(dest + cur_len, CHUNK_DIR_PATH, strlen(CHUNK_DIR_PATH));
  cur_len += strlen(CHUNK_DIR_PATH);
  dest[cur_len] = '/';
  cur_len++;
  memcpy(dest + cur_len, chunk_name, strlen(chunk_name));
  cur_len += strlen(chunk_name);
  dest[cur_len] = '\0';

  return 0;

}