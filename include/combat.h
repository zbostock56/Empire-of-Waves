#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <menu.h>

#define X (0)
#define Y (1)

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

float decrement_timer(float);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void c_enemy_pathfind(C_UNIT *, vec2);
