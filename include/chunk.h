/* INCLUDES */
#include <stdio.h>
#include <chunk_str.h>

/* DEFINES */
#define X (0)
#define Y (1)
#define FIRST  (0)
#define SECOND (1)
#define THIRD  (2)
#define CHUNKS_SIMULATED (9)

/* GLOBALS */
extern CHUNK player_chunks[CHUNKS_SIMULATED];

/* PROTOTYPES */
/* ISLAND.C */
int generate_rand();
void generate_island(ISLAND *island);

/* NATIVE */
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
void generate_chunk(CHUNK *chunk);
void island_locator(ivec2 *locs, int num_islands);
void bounds_check(ivec2 loc);
