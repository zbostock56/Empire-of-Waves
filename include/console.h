/*
                      CONSOLE.h
  Responsible for handling console prototypes and functionality
*/
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <ui_component.h>
#include <menu.h>

/* GLOBALS */
extern int console_enabled;
extern char cons_cmd[100];

/* DEFINES */
#define MAX_TOKENS (10)
#define MAX_CHUNKS (9)
#define MAX_ISLANDS_SIM_CHUNKS (MAX_ISLANDS * 9)

/* STRUCTS */
typedef struct island_dist {
  ISLAND island;
  float distance;
} ISLAND_DIST;

/* PROTOTYPES */
void teleport(ivec2);
void set_speed(float);
void teleport_nearest_island();
void command_not_found();
void update_console_prompt();

/* EXTERNAL PROTOTYPES */
void world_to_chunk(vec2, ivec2, vec2);
void chunk_to_world(ivec2, vec2, vec2);
void spawn_enemy();
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
