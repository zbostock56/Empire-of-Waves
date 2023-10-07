#include <cglm/vec2.h>
#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define X (0)
#define Y (1)
#define SHIP_COLLISION_RADIUS (1.0)
#define CHARACTER_COLLISION_RADIUS (0.5)

// ======================= INTERNALLY DEFINED FUNCTIONS ======================
int check_collision(float*, float, float, float*, float, float);
ISLAND *cur_island(CHUNK *, vec2, float);
int check_tile(ISLAND *cur_isl, vec2);
void character_collisions(CHUNK *, ivec2, vec2);
void ship_collisions(CHUNK *, ivec2, vec2);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
