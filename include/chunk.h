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
#define CHUNK_BUFF_STARTING_LEN (10)

/* GLOBALS */

ivec2 CHUNK_OFFSETS[CHUNKS_SIMULATED] = {
  {-1,  1}, { 0,  1}, { 1,  1},
  {-1,  0}, { 0,  0}, { 1,  0},
  {-1, -1}, { 0, -1}, { 1, -1}
};

float T_WIDTH = 0.1;

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
int generate_rand();
int generate_island(ISLAND *island);
int load_chunk(ivec2, CHUNK *);
void save_chunk(CHUNK *);
void populate_tile_pixel_buffer(ISLAND *, unsigned char (*)[3]);
unsigned int texture_from_buffer(unsigned char *, int, int, int);

// ======================= INTERNALLY DEFINED FUNCTIONS ======================
void place_home(ISLAND *, CHUNK *);

int generate_chunk(CHUNK *chunk);
int chunk_from_coords(ivec2, CHUNK *);
void island_locator(ivec2 *locs, int num_islands);
void bounds_check(ivec2 loc);

void free_chunk(CHUNK *);
int add_chunk(ivec2);
int remove_chunk(unsigned int);

char *index_to_str(int);
int double_buffer(void **, unsigned int *, unsigned int);
