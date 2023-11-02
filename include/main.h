#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_player();
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
void update_enemy_ships();
void update_combat_state();
void update_projectiles();
void init_dialog();
void init_trade();
void init_status_bar();
void open_status_bar();
int init_save_menu();
void update_save_interval();
void init_ui_lists();
void update_dialog_buffers();
void update_available_mercenaries();
void update_mercenary_reassignment();
void update_console_prompt();
void update_timers();
void print_coords();
int init_containers();
void init_container_ui();
void init_game(char *);
void free_game();
