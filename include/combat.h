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
#define PROMPT_BUFFER_MAX (30)

// ================================ GLOBALS ================================
char lost_item_prompt_buffer[PROMPT_BUFFER_MAX];

extern int container_menu_open;
int surrender_menu_open = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

float decrement_timer(float);
void update_projectiles();
int spawn_projectile(vec2, vec2, float, UNIT_T);
void knockback(C_UNIT *);
void perform_surrender();
void init_surrender_ui();
void open_surrender_prompt();
void close_surrender_prompt();

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void c_enemy_pathfind(C_UNIT *, vec2);
int double_buffer(void **, unsigned int *, unsigned int);
void free_container(CONTAINER *);
void init_container(CONTAINER *, unsigned int);
void respawn_player();
void close_container();
I_SLOT *get_player_inventory_slot_by_index(unsigned int);
void set_prompt(const char *buffer);
void close_all_menus();
