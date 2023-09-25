#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <model_str.h>
#include <shader_code.h>

// ========================== GLOBAL DECLARATIONS ============================

// Insert declarations of models, shaders, etc...

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

unsigned int shader_init(const char *, const char *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

MODEL *load_model(char *);
void draw_model(MODEL *);
