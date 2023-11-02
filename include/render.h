#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <model_str.h>
#include <player_str.h>
#include <chunk_str.h>
#include <ui_component.h>
#include <dialog_str.h>
#include <trade_str.h>
#include <menu.h>
#include <items.h>
#include <trade_ship_str.h>
#include <globals.h>

#define PI (3.14159265)
#define WAVE_PERIOD (4.0)
#define MIN_TEXT_SCALE (1.25)
#define BASE_RES_X (640.0)
#define BASE_RES_Y (640.0)
#define FBO_QUAD_WIDTH (18.0)
#define RIPPLE_WIDTH (20)
#define FONT_LEN (95)
#define INVALID_SHADER (0xBEEFBEEF)
#define X_MIN (0)
#define Y_MIN (1)
#define X_MAX (2)
#define Y_MAX (3)

// Constants for ocean wave effect
#define A (3.0)
#define B (0.001)
#define C (2.0)
#define K (1.0)
#define H (0.05)
#include <shader_code.h>

#define WORLD_DEPTH (-0.7)
#define OBSTACLE_DEPTH (-0.6)
#define EFFECT_DEPTH (-0.5)
#define SHIP_DEPTH (-0.4)
#define AVATAR_DEPTH (-0.3)
#define UI_DEPTH (-0.2)
#define TEXT_DEPTH (-0.1)

typedef struct framebuffer {
  unsigned int FBO;
  unsigned int color_texture;
  unsigned int depth_stencil_rbo;
} FRAMEBUFFER;

vec3 UI_PIVOT_OFFSETS[9] = {
  { 0.0,  0.0, 0.0}, // CENTER
  { 0.0, -1.0, 0.0}, // TOP
  { 0.0,  1.0, 0.0}, // BOTTOM
  { 1.0,  0.0, 0.0}, // LEFT
  {-1.0,  0.0, 0.0}, // RIGHT
  { 1.0, -1.0, 0.0}, // TOP LEFT
  {-1.0, -1.0, 0.0}, // TOP RIGHT
  { 1.0,  1.0, 0.0}, // BOTTOM LEFT
  {-1.0,  1.0, 0.0}  // BOTTOM RIGHT
};


// ========================== GLOBAL DECLARATIONS ============================

extern DIALOG dialog;
extern float time_schdule_trade_toute_prompt;
extern TRADE trade;
extern float time_trade_event_prompt;

// Insert declarations of global render elemements:
// - models
// - matrices
// - shaders
// - framebuffers
// - etc...
int RES_X = 640;
int RES_Y = 640;
int incremented_1 = 0;
int incremented_2 = 0;
float wave_offset_1 = 0.0;
float wave_offset_2 = 0.0;

unsigned int std_shader;
unsigned int color_shader;
unsigned int pixel_shader;
unsigned int text_shader;
unsigned int ripple_shader;
unsigned int menu_shader;
unsigned int chunk_shader;
unsigned int island_shader;

unsigned int item_textures[NUM_ITEMS];

FRAMEBUFFER entity_framebuffer;

MODEL *player;
MODEL *enemy;
MODEL *merchant;
MODEL *mercenary;
MODEL *player_ship;
MODEL *enemy_ship;
MODEL *trade_ship;
MODEL *house;
MODEL *chest;
MODEL *quad;
MODEL *circle;
CHARACTER font[FONT_LEN];

mat4 persp_proj;
mat4 ortho_proj;
vec2 screen_scale;
vec3 Z = { 0.0, 0.0, 1.0 };

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void render_player_ship();
void render_enemy_ship(E_ENEMY *, unsigned int);
void render_trade_ship(TRADE_SHIP *, unsigned int);
void render_player();
void render_unit(C_UNIT *);
void render_merchant(MERCHANT *);
void render_e_npc(MODEL *, ivec2, vec2, vec2, float);
void render_c_npc(MODEL *, vec2, vec2, float);
void render_obstacle(MODEL *, ivec2, vec2, float);
void render_fbo_entity(MODEL *, mat4, mat4, mat4, mat4, mat4, mat4);
void render_ui(UI_COMPONENT *);
void render_text(char *, int, mat4);
void render_ripple(unsigned int, vec2, vec2, int);
void render_island(ISLAND *);
void render_chunk(ivec2);
void render_arena();
void render_hitbox(vec2, float);
void render_health_bar_background(vec2);
void render_health_bar_filled(vec2, float, float);

void calc_screen_scale();

unsigned int shader_init(const char *, const char *);
FRAMEBUFFER framebuffer_init();
void refresh_framebuffers();

void calc_rot_mat(vec3, mat4);

float get_text_width(char *, int);
float get_screen_text_scale();

void set_mat4(char *, mat4, unsigned int);
void set_mat3(char *, mat3, unsigned int);
void set_vec4(char *, vec4, unsigned int);
void set_vec3(char *, vec3, unsigned int);
void set_vec2(char *, vec2, unsigned int);
void set_float(char *, float, unsigned int);
void set_int(char *, int, unsigned int);
void set_uint(char *, unsigned int, unsigned int);
void set_iarr(char *, int *, unsigned int, unsigned int);
void set_texture(char *, unsigned int, unsigned int, int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

MODEL *load_model(char *, char *);
void load_character(char *, char *, CHARACTER *);
void draw_model(MODEL *, unsigned int);
void free_model(MODEL *);
unsigned int gen_texture(char *);

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
int get_tile(unsigned int, vec2);

unsigned int texture_from_buffer(unsigned char *, int, int, int);
void update_status_bar();
