#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <globals.h>


#define RES_X (640.0f)
#define RES_Y (640.0f)
#define PROJECT_TITLE ("Empire of Waves")

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void mouse_click(GLFWwindow *, int, int, int);
void mouse_pos(GLFWwindow *, double, double);
void mouse_scroll(GLFWwindow *, double, double);
void fb_size_callback(GLFWwindow *, int, int);

void init_player();
void init_menus();
int init_dialog();
int init_trade();
int init_status_bar();
void init_ui_lists();
void init_container_ui();
int init_save_menu();
int init_chunks();
int init_containers();
int init_trade_ship_buffers();
void init_timers();
void open_status_bar();

void free_dialog();
void free_trade();
void free_status_bar();
void clear_chunk_buffer();
void free_containers();
void free_trade_ship_buffers();

void copy_valid_path(const char *, char *, int);
