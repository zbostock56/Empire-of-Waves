#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <model_str.h>
#include <shader_code.h>

#define INVALID_SHADER (0xBEEFBEEF)

// ========================== GLOBAL DECLARATIONS ============================

// Insert declarations of models, shaders, etc...
unsigned int player_shader;

MODEL *player;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

unsigned int shader_init(const char *, const char *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

MODEL *load_model(char *, char *);
void draw_model(MODEL *, unsigned int);
