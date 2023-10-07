#include <string.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <menu.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)
#define X_MIN (0)
#define Y_MIN (1)
#define X_MAX (2)
#define Y_MAX (3)

// Mouse position in screen space:
// X: [-1, 1], Y: [-1, 1]
vec2 mouse_position = GLM_VEC2_ZERO_INIT;

int holding_left_click = 0;
extern int shore_interaction_enabled;

int holding_equals = 0;
int holding_minus = 0;
int holding_interaction = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void exploration_movement(GLFWwindow *);
void combat_movement(GLFWwindow *);
void debug_keys(GLFWwindow *);
void ui_click_listener(double, double);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void detect_context_interaction();

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
void get_ui_min_max(UI_COMPONENT *, vec4);
