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

/* DEFINES */
#define MAX_TOKENS (10)
#define MAX_CHUNKS (9)
#define MAX_ISLANDS_SIM_CHUNKS (MAX_ISLANDS * 9)
#define CONSOLE_BUFFER_MAX (100)

/* ------------- GLOBALS ------------ */
/* Timers */
float console_cursor_interval = 0.0;
float console_error_interval = 0.0;
/* Event Flags */
int console_input_enabled;
int cursor_enabled = 0;
int coords_enabled = 0;
int console_error = 0;
/* Buffers */
char console_world_coords[CONSOLE_BUFFER_MAX];
char console_intra_chunk[CONSOLE_BUFFER_MAX];
char console_chunk_coords[CONSOLE_BUFFER_MAX];
char console_error_buffer[CONSOLE_BUFFER_MAX];
extern vec2 screen_scale;
char cons_cmd[CONSOLE_BUFFER_MAX];
unsigned int cons_cmd_len = 0;

/* ---------------------------------  */

/* STRUCTS */
typedef struct island_dist {
  ISLAND island;
  float distance;
} ISLAND_DIST;

/* ---------------- INTERNALLY DEFINED FUNCTIONS -------------- */
void teleport(ivec2);
void set_speed(float);
void teleport_nearest_island();
void command_not_found();
void update_console_prompt();
void give_mercenaries(int);
void calc_cursor_pos(vec2);
void teleport_nearest_merchant();
void reset_console_error();
void set_console_error(const char *);
void console_error_init();

/* ---------------- EXTERNALLY DEFINED FUNCTIONS -------------- */
void world_to_chunk(vec2, ivec2, vec2);
void chunk_to_world(ivec2, vec2, vec2);
void spawn_enemy();
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
float get_text_width(char *, int);
float get_screen_text_scale();
void print_parse_table();
