#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)

extern int shore_interaction_enabled;

int holding_equals = 0;
int holding_minus = 0;
int holding_interaction = 0;
int holding_attack = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void exploration_movement(GLFWwindow *);
void combat_movement(GLFWwindow *);
void debug_keys(GLFWwindow *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void detect_context_interaction();

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);

int to_combat_mode(unsigned int);
void from_combat_mode();
