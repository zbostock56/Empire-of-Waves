#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <errno.h>
#include <globals.h>

#define CHUNK_DIR_PATH ("chunks")
#define SAVE_DIR_PATH ("saves")
#define SAVE_FILE_EXTENSION (".data")
#define MAX_PATH_LEN (128)
#define MAX_CHUNK_NAME_LEN (32)
#define MAX_CHUNK_COORD (1000000)

// Definition of macro for making directories on windows
#ifdef _IWIN32
#include <windows.h>
#define make_dir(dir) (_mkdir(dir))
#endif

// Definition of macro for making directories on linux
#ifdef __linux__
#include <sys/stat.h>
#define make_dir(dir) (mkdir(dir, S_IRUSR | \
                                  S_IWUSR | \
                                  S_IXUSR | \
                                  S_IRGRP | \
                                  S_IWGRP | \
                                  S_IXGRP | \
                                  S_IROTH))
#endif

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

int save_game(char *);
int load_game(char *);

int init_save_dir(char *);
void save_game_state(FILE *);
void load_game_state(FILE *);

int load_island(FILE *, ISLAND *);
int load_merchant(FILE *, MERCHANT *);

void save_island(FILE *, ISLAND *);
void save_merchant(FILE *, MERCHANT *);

int create_dir(char *);
void copy_valid_path(const char *, char *, int);
int build_save_path(char *, char *, int);
int build_chunk_path(char *, char *, char *, int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void init_chunks();
void clear_chunk_buffer();
int out_of_bounds(ivec2, int, int);
void populate_tile_pixel_buffer(ISLAND *, unsigned char (*)[3]);
unsigned int texture_from_buffer(unsigned char *, int, int, int);
