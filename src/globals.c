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
E_PLAYER e_player;
CHUNK player_chunks[9];
TRADE_SHIP *trade_ships;
CHUNK *trade_ship_chunks;
unsigned int num_trade_ships;
float next_event;

// COMBAT MODE STATE
C_PLAYER c_player;
C_UNIT *npc_units;
unsigned int e_enemy_index;
unsigned int num_npc_units;
ivec2 arena_dimensions = { 50, 20 };
