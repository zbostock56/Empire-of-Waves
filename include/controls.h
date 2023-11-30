#include <string.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <lexer_str.h>
#include <globals.h>
#include <ui_component.h>
#include <dialog_str.h>
#include <trade_str.h>
#include <menu.h>
#include <event_str.h>

#define INPUT_BUFFER_SIZE (16)
#define MAX_CMD_LEN (100)
#define X_MIN (0)
#define Y_MIN (1)
#define X_MAX (2)
#define Y_MAX (3)

// Mouse position in screen space:
// X: [-1, 1], Y: [-1, 1]
vec2 mouse_position = GLM_VEC2_ZERO_INIT;

extern int save_input_enabled;
extern int console_input_enabled;
extern int shore_interaction_enabled;
extern int home_interaction_enabled;
extern int container_interaction_enabled;
extern int item_interaction_enabled;
extern int reassignment_menu_open;
extern UI_ID open_prompt;
extern MERCHANT *close_merchant;
extern TRADE trade;
extern DIALOG dialog;
extern int RES_X;
extern int RES_Y;
extern float save_status_interval;
extern char cons_cmd[MAX_CMD_LEN];
extern unsigned int cons_cmd_len;
extern char *save_input_buffer;
extern unsigned int save_input_len;
extern int console_input_enabled;
extern int container_menu_open;
extern unsigned int cur_lootable;
extern int status_menu_open;

int holding_left_click = 0;
int holding_equals = 0;
int holding_left_bracket = 0;
int holding_tilde = 0;
int holding_alpha[26];
int holding_num[10];
int holding_space = 0;
int holding_shift = 0;
int holding_ctrl = 0;
int holding_alt = 0;
int holding_enter = 0;
int holding_backspace = 0;
int holding_underscore = 0;
int holding_dot = 0;
int holding_minus = 0;
int holding_interaction = 0;
int holding_attack = 0;
int holding_save = 0;
int holding_load = 0;
int holding_esc = 0;
int holding_leave = 0;
int holding_i = 0;
int holding_f = 0;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void exploration_movement(GLFWwindow *);
void combat_movement(GLFWwindow *);
void debug_keys(GLFWwindow *);
void input_keys(GLFWwindow *);
void ui_click_listener(double, double);
void ui_hover_listener(double, double);
void close_merchant_menu(GLFWwindow *window);
void load_keys(GLFWwindow *);
void combat_mode_attack(int);
void modifier_keys(GLFWwindow *);
void surrender_listener(GLFWwindow *);
void open_stats(GLFWwindow *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================
void detect_context_interaction();
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
void handle_command(char *);
void tokenize(char *, int);
void get_ui_min_max(UI_COMPONENT *, vec4);
int to_combat_mode(unsigned int);
void from_combat_mode();
void open_dialog();
int set_dialog(MERCHANT *, T_DIALOG, char *, char *);
void close_dialog();
void close_trade();
void open_save_menu();
void close_save_menu();
void open_save_status(char *);
void close_save_status();
int save_menu_opened();
void refresh_framebuffers();
int new_game(char *);
int save_game(char *);
int load_game(char *);
int spawn_projectile(vec2, vec2, float, UNIT_T);
void open_container(CONTAINER, CONTAINER);
void close_container();
char *get_merchant_name(short);
void leave_combat();
void set_prompt(const char *);

void open_mercenary_reassignment_menu();
void close_mercenary_reassignment_menu();
void close_console_prompt();
void open_ransom_menu();
void close_ransom_menu();

void open_status_menu();
void close_status_menu();

void inventory_open_listner();
void open_inventory_ui();
void close_inventory_ui();

int pickup_resource();

void perform_surrender();
