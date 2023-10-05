#include <cglm/vec2.h>
#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define X (0)
#define Y (1)
#define PLAYER_WIDTH (3.0)
#define PLAYER_HEIGHT (1.0)

// ======================= INTERNALLY DEFINED FUNCTIONS ======================
int check_collision(float*, float, float, float*, float, float);
int cur_island();
int check_tile(int cur_isl, vec2);
int player_collisions(vec2);
int ship_collisions(vec2);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);