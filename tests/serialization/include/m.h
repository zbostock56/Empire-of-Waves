#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <player_str.h>
#include <globals.h>


// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_scene();
void init_chunks();
void keyboard_input(GLFWwindow *);
void manage_chunks();
void render_scene(GLFWwindow *);
void cleanup_scene();
void update_event_timer();
int load_chunk(ivec2, CHUNK *);
void save_chunk(CHUNK *);
