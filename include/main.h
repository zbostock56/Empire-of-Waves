#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_scene();
int init_chunks();
void keyboard_input(GLFWwindow *);
int manage_chunks();
void render_scene(GLFWwindow *);
void cleanup_scene();
void init_menus();
void update_event_timer();
void init_lexer();
int detect_collisions();
int init_trade_ship_buffers();
void update_trade_ships();
void update_combat_state();
