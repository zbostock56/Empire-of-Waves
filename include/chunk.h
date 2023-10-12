/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

/* DEFINES */
#define X (0)
#define Y (1)
#define FIRST  (0)
#define SECOND (1)
#define THIRD  (2)
#define MAX_CHUNK_PATH_LEN (100)
#define MAX_CHUNK_COORD (1000000)

/* GLOBALS */
extern CHUNK player_chunks[CHUNKS_SIMULATED];

ivec2 CHUNK_OFFSETS[CHUNKS_SIMULATED] = {
  {-1,  1}, { 0,  1}, { 1,  1},
  {-1,  0}, { 0,  0}, { 1,  0},
  {-1, -1}, { 0, -1}, { 1, -1}
};

float T_WIDTH = 0.1;

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
/* ISLAND.C */
int generate_rand();
int generate_island(ISLAND *island);
void populate_tile_pixel_buffer(ISLAND *, unsigned char (*)[3]);
unsigned int texture_from_buffer(unsigned char *, int, int, int);

// ======================= INTERNALLY DEFINED FUNCTIONS ======================
void place_home(ISLAND *);
int chunk_loaded_by_player(ivec2);

int generate_chunk(CHUNK *chunk);
int chunk_from_coords(ivec2, CHUNK *);
void island_locator(ivec2 *locs, int num_islands);
void bounds_check(ivec2 loc);

void free_chunk(CHUNK *);
int copy_chunk(CHUNK *, CHUNK *);

int load_chunk(ivec2, CHUNK *);
int load_island(FILE *, ISLAND *);
int load_merchant(FILE *, MERCHANT *);

void save_chunk(CHUNK *);
void save_island(FILE *, ISLAND *);
void save_merchant(FILE *, MERCHANT *);

char *index_to_str(int);
int get_index(ivec2);
int out_of_bounds(ivec2, int, int);
