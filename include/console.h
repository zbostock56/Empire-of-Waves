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

/* DEFINES */
#define MAX_TOKENS (10)
#define GIVE ("give")
#define TELEPORT ("teleport")
#define SPAWN_ENEMY ("spawn_enemy")
#define SPAWN_TRADE_SHIP ("spawn_trade_ship")
#define SET_SPEED ("set_speed")

/* PROTOTYPES */
void teleport(ivec2);
void set_speed(float speed);

/* EXTERNAL PROTOTYPES */
void world_to_chunk(vec2, ivec2, vec2);
