/* INCLUDES */
#include <stdio.h>
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

// Macros specifying index names for player_chunks
#define CHUNK_UPPER_LEFT (0)
#define CHUNK_UP (1)
#define CHUNK_UPPER_RIGHT (2)
#define CHUNK_LEFT (3)
#define PLAYER_CHUNK (4)
#define CHUNK_RIGHT (5)
#define CHUNK_LOWER_LEFT (6)
#define CHUNK_DOWN (7)
#define CHUNK_LOWER_RIGHT (8)

// Offsets of each chunk in player_chunks from player_chunks[4]
// Each index correponds to the same chunk pointer in player_chunks
ivec2 CHUNK_OFFSETS[9] = {
  {-1,  1}, { 0,  1}, { 1,  1},
  {-1,  0}, { 0,  0}, { 1,  0},
  {-1, -1}, { 0, -1}, { 1, -1}
};

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
/* ISLAND.C */
int generate_rand();
void generate_island(ISLAND *island);

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void generate_chunk(CHUNK *chunk);
void island_locator(ivec2 *locs, int num_islands);
void bounds_check(ivec2 loc);

void free_chunk(CHUNK *);

int load_chunk(ivec2, CHUNK *);
int load_island(FILE *, ISLAND *);
int load_merchant(FILE *, MERCHANT *);

void save_chunk(CHUNK *);
void save_island(FILE *, ISLAND *);
void save_merchant(FILE *, MERCHANT *);

char *index_to_str(int);
int get_index(ivec2);
int out_of_bounds(ivec2, int, int);
