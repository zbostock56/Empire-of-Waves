#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)

int holding_equals = 0;

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
