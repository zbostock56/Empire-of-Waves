#include <stdio.h>
#include <chunk_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define TRADE_SHIP_BUF_START_LEN (5)

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void trade_ship_pathfind(TRADE_SHIP *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

int chunk_from_coords(ivec2, CHUNK *);

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);

void trade_ship_steering(TRADE_SHIP *, vec2);
void prompt_plundered_trade_ship();

int add_chunk(ivec2);
int double_buffer(void **, unsigned int *, unsigned int);
