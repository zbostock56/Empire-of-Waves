#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <menu.h>

#define X (0)
#define Y (1)
#define MELEE_CUTOFF (75)
#define PROJ_BUF_START_LEN (10)

extern int container_menu_open;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

float decrement_timer(float);
void update_projectiles();
int spawn_projectile(vec2, vec2, float, UNIT_T);
void knockback(C_UNIT *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void c_enemy_pathfind(C_UNIT *, vec2);
int double_buffer(void **, unsigned int *, unsigned int);
void free_container(CONTAINER *);
void init_container(CONTAINER *, unsigned int);
void respawn_player();
void close_container();
