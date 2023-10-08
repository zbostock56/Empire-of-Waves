#include <player_str.h>
#include <chunk_str.h>
#include <stdio.h>

/* GLOBALS  */
extern CHUNK player_chunks[CHUNKS_SIMULATED];
extern E_PLAYER e_player;

/* DEFINES */
#define CURRENT_CHUNK (4)

/* =================== INTERNALLY DEFINED FUNCTIONS =================== */
void spawn_enemy();
int find_avail_chunks();
void generate_chunk_tiles(int [C_WIDTH][C_WIDTH], CHUNK);

/* =================== EXTERNALLY DEFINED FUNCTIONS =================== */

int double_buffer(void **, unsigned int *, unsigned int);
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);