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
int detect_collisions();
int init_trade_ship_buffers();
void update_trade_ships();
void update_combat_state();
void init_dialog();
void init_trade();
void init_status_bar();
void open_status_bar();
void init_ui_lists();
void update_dialog_buffers();
void update_available_mercenaries();
void update_mercenary_reassignment();
