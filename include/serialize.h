#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <errno.h>
#include <globals.h>

#define SAVE_DIR_PATH ("saves")
#define SAVE_FILE_EXTENSION (".data")
#define FILENAME_BUF_STARTING_LEN (20)
#define MAX_PATH_LEN (128)
#define MAX_CHUNK_NAME_LEN (32)
#define MAX_CHUNK_COORD (1000000)

// Definition of macros and functions for directory i/o on windows
#ifdef _IWIN32
#include <windows.h>
#define make_dir(dir) (_mkdir(dir))
#define PATH_SEPERATOR ('\\')
#endif

// Definition of macros and functions for directory i/o on linux
#ifdef __linux__
#include <sys/stat.h>
#include <dirent.h>
#define make_dir(dir) (mkdir(dir, S_IRUSR | \
                                  S_IWUSR | \
                                  S_IXUSR | \
                                  S_IRGRP | \
                                  S_IWGRP | \
                                  S_IXGRP | \
                                  S_IROTH))
#define PATH_SEPERATOR ('/')
#endif

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

int save_game(char *);
int load_game(char *);
int load_chunk(ivec2, CHUNK *, char *);
void save_chunk(CHUNK *, char *);

int init_save_dir(char *);
void consolidate_unsaved_chunks();
void clear_unsaved_chunks();
void reset_state();
void save_state(FILE *);
void save_trade_ship(TRADE_SHIP *, FILE *);
void load_player_state(FILE *);
void load_game_state(FILE *);
void load_trade_ship(FILE *);

int chunk_from_disk(char *, CHUNK *);
int load_island(FILE *, ISLAND *);
int load_merchant(FILE *, MERCHANT *);

void chunk_to_disk(char *, CHUNK *);
void save_island(FILE *, ISLAND *);
void save_merchant(FILE *, MERCHANT *);

int create_dir(char *);
void copy_valid_path(const char *, char *, int);
int build_save_path(char *, char *, int);
int build_chunk_path(char *, char *, char *, char *, int);

int list_all_files(char *, char (**)[MAX_CHUNK_NAME_LEN], unsigned int *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void init_chunks();
void clear_chunk_buffer();
int out_of_bounds(ivec2, int, int);
void populate_tile_pixel_buffer(ISLAND *, unsigned char (*)[3]);
unsigned int texture_from_buffer(unsigned char *, int, int, int);
TRADE_SHIP *init_trade_ship(ivec2, unsigned int);
int double_buffer(void **, unsigned int *, unsigned int);
