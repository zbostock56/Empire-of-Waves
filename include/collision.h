#include <cglm/vec2.h>
#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <menu.h>

#define X (0)
#define Y (1)

#define INTERACTION_RADIUS (3.0)
#define SHIP_COLLISION_RADIUS (1.0)
#define SHIP_PATHFIND_RADIUS (10.0)
#define CHARACTER_COLLISION_RADIUS (0.5)

extern float T_WIDTH;

int shore_interaction_enabled = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

ISLAND *cur_island(CHUNK *, vec2, float);
int check_tile(ISLAND *cur_isl, vec2);

void detect_context_interaction();
void character_collisions(CHUNK *, ivec2, vec2);
void ship_collisions(CHUNK *, ivec2, vec2);
void unit_collision(vec2);
void check_merchant_prompt(vec2);
int detect_enemy_ships();
void trade_ship_collision(TRADE_SHIP *);
void attack_collision();

int aabb_collision(float *, float, float, float *, float, float);
int circle_aabb_collision(vec2, float, vec2, float, float, vec2);
int circle_circle_collision(vec2, float, vec2, float);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);

int to_combat_mode(unsigned int);
void from_combat_mode();
