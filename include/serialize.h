#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <errno.h>
#include <chunk_str.h>

#define CHUNK_DIR_PATH ("chunks")
#define SAVE_DIR_PATH ("saves")
#define SAVE_FILE_EXTENSION (".data")
#define MAX_PATH_LEN (64)
#define MAX_CHUNK_PATH_LEN (100)
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

int init_save_dir(char *);
int create_dir(char *);
void copy_valid_path(const char *, char *, int);
int build_save_path(char *, char *, int);
int build_chunk_path(char *, char *, int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

int load_island(FILE *, ISLAND *);
int load_merchant(FILE *, MERCHANT *);

void save_island(FILE *, ISLAND *);
void save_merchant(FILE *, MERCHANT *);

int out_of_bounds(ivec2, int, int);
void populate_tile_pixel_buffer(ISLAND *, unsigned char (*)[3]);
unsigned int texture_from_buffer(unsigned char *, int, int, int);
