#include <string.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <dialog_str.h>
#include <trade_str.h>
#include <menu.h>

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
int holding_attack = 0;

extern DIALOG dialog;
extern TRADE trade;
extern int RES_X;
extern int RES_Y;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void exploration_movement(GLFWwindow *);
void combat_movement(GLFWwindow *);
void debug_keys(GLFWwindow *);
void ui_click_listener(double, double);
void ui_hover_listener(double, double);
void close_merchant_menu(GLFWwindow *window);
void combat_mode_attack(int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
void detect_context_interaction();
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
void get_ui_min_max(UI_COMPONENT *, vec4);
int to_combat_mode(unsigned int);
void from_combat_mode();
void open_dialog();
int set_dialog(T_DIALOG, char *, char *);
void close_dialog();
void close_trade();
void refresh_framebuffers();
int spawn_projectile(vec2, vec2, float, UNIT_T);

