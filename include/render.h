#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <shader_code.h>
#include <model_str.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <ui_component.h>
#include <globals.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)
#define FONT_LEN (95)
#define INVALID_SHADER (0xBEEFBEEF)

typedef struct framebuffer {
  unsigned int FBO;
  unsigned int color_texture;
  unsigned int depth_stencil_rbo;
} FRAMEBUFFER;

// ========================== GLOBAL DECLARATIONS ============================

// Insert declarations of global render elemements:
// - models
// - matrices
// - shaders
// - framebuffers
// - etc...
unsigned int std_shader;
unsigned int color_shader;
unsigned int pixel_shader;
unsigned int text_shader;

FRAMEBUFFER entity_framebuffer;

MODEL *player;
MODEL *enemy;
MODEL *merchant;
MODEL *player_ship;
MODEL *enemy_ship;
MODEL *trade_ship;
MODEL *quad;
CHARACTER font[FONT_LEN];

mat4 persp_proj;
mat4 ortho_proj;
vec3 Z = { 0.0, 0.0, 1.0 };

// TEST ENTITIES
UI_COMPONENT test_menu;
ISLAND test_island;
unsigned int ocean_texture;
// END TEST

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void render_player_ship();
void render_enemy_ship(E_ENEMY *);
void render_trade_ship(TRADE_SHIP *);
void render_player();
void render_unit(C_UNIT *);
void render_merchant(MERCHANT *);
void render_e_npc(MODEL *, ivec2, vec2, vec2, float);
void render_c_npc(MODEL *, vec2, vec2, float);
void render_fbo_entity(MODEL *, mat4, mat4, mat4, mat4, mat4, mat4);
void render_menu(UI_COMPONENT *);
void render_text(char *, mat4);
void render_island(ISLAND *);
void render_chunk(ivec2);
void render_arena();
void render_hitbox(vec2);
//void render_tile(TILE, ivec2, vec2);

unsigned int shader_init(const char *, const char *);
FRAMEBUFFER framebuffer_init();

void calc_rot_mat(vec3, mat4);

void set_mat4(char *, mat4, unsigned int);
void set_mat3(char *, mat3, unsigned int);
void set_vec4(char *, vec4, unsigned int);
void set_vec3(char *, vec3, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

MODEL *load_model(char *, char *);
void load_character(char *, char *, CHARACTER *);
void draw_model(MODEL *, unsigned int);
void free_model(MODEL *);

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);

//void generate_island(ISLAND *);
unsigned int texture_from_buffer(unsigned char *, int, int, int);
