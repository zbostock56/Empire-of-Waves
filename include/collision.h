#include <cglm/vec2.h>
#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define X (0)
#define Y (1)

#define INTERACTION_RADIUS (3.0)
#define SHIP_COLLISION_RADIUS (1.0)
#define CHARACTER_COLLISION_RADIUS (0.5)

int shore_interaction_enabled = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void detect_context_interaction();
ISLAND *cur_island(CHUNK *, vec2, float);
int check_tile(ISLAND *cur_isl, vec2);
void character_collisions(CHUNK *, ivec2, vec2);
void ship_collisions(CHUNK *, ivec2, vec2);
void unit_collision(vec2);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
