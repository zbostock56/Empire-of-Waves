#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <shader_code.h>
#include <model_str.h>
#include <player_str.h>
#include <trade_ship_str.h>
#include <chunk_str.h>
#include <globals.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)
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
unsigned int model_shader;
unsigned int pvm_shader;
FRAMEBUFFER entity_framebuffer;
MODEL *player;
MODEL *enemy;
MODEL *merchant;
MODEL *player_ship;
MODEL *enemy_ship;
MODEL *trade_ship;
MODEL *quad;
mat4 persp_proj;
mat4 ortho_proj;
vec3 Z = { 0.0, 0.0, 1.0 };

// TEST ENTITIES
E_ENEMY test_enemy;
TRADE_SHIP test_ts;
// END TEST

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

void render_player_ship();
void render_enemy_ship(E_ENEMY *);
void render_trade_ship(TRADE_SHIP *);
void render_fbo_entity(MODEL *, mat4, mat4, mat4, mat4, mat4, mat4);

unsigned int shader_init(const char *, const char *);
FRAMEBUFFER framebuffer_init();

void calc_rot_mat(vec3, mat4);

void set_mat4(char *, mat4, unsigned int);
void set_mat3(char *, mat3, unsigned int);
void set_vec4(char *, vec4, unsigned int);
void set_vec3(char *, vec3, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

MODEL *load_model(char *, char *);
void draw_model(MODEL *, unsigned int);

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
