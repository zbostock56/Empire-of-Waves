#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>

/*
                                    GLOBALS.c
Defines global state variables, which tracks information such as player, chunk,
game mode, and hit box state. globals.h defines external references to these
defined variables and can be included by any file which manipulates/reads
global information.

NEVER INCLUDE THIS FILE. IF YOU NEED TO ACCESS ANY OF THE GLOBAL VARIABLES,
INCLUDE globals.h INSTEAD
*/
// GLOBAL GAME STATE
GAME_MODE mode = EXPLORATION;
float delta_time = 0.0;
float last_frame = 0.0;

// EXPLORATION MODE STATE
CHUNK *chunk_buffer = NULL;
unsigned int chunk_buff_size = 0;
unsigned int chunk_buff_len = 0;

E_PLAYER e_player;
unsigned int player_chunks[9];
unsigned int updated_chunks[9];
TRADE_SHIP *trade_ships = NULL;
vec2 home_island_coords = GLM_VEC2_ZERO_INIT;
vec2 house_tile = GLM_VEC2_ZERO_INIT;
unsigned int num_trade_ships = 0;
unsigned int trade_ship_buf_size = 0;
float next_event;
MERCHANT *cur_merchant = NULL;

// COMBAT MODE STATE
C_PLAYER c_player;
C_UNIT *npc_units = NULL;
unsigned int num_npc_units = 0;
PROJ *projectiles = NULL;
unsigned int num_projectiles = 0;
unsigned int proj_buf_size = 0;
unsigned int e_enemy_index;
ivec2 arena_dimensions = { 50, 20 };
