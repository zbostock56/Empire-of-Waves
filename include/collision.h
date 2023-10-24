#include <cglm/vec2.h>
#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <dialog_str.h>
#include <trade_str.h>
#include <menu.h>

#define X (0)
#define Y (1)

#define INTERACTION_RADIUS (3.0)
#define SHIP_COLLISION_RADIUS (1.0)
#define SHIP_COMPLETION_RADIUS (5.0)
#define SHIP_PATHFIND_RADIUS (40.0)
#define CHARACTER_COLLISION_RADIUS (0.5)
#define SHIP_CHASE_RADIUS (20)

extern float T_WIDTH;
extern DIALOG dialog;
extern TRADE trade;
extern char merchant_name_list[][20];

int shore_interaction_enabled = 0;
int home_interaction_enabled = 0;
extern int reassignment_menu_open;

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
void check_mercenary_reassignment_prompt(vec2);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
void update_enemy_position(E_ENEMY *);
void pathfind_enemy(E_ENEMY *);
int double_buffer(void **, unsigned int *, unsigned int);
void update_enemy_chunk(E_ENEMY*, CHUNK*, int);

int to_combat_mode(unsigned int);
void from_combat_mode();
void close_dialog();
void close_trade();
void close_mercenary_reassignment_menu();
