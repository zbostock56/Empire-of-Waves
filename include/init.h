#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>


#define RES_X (640.0f)
#define RES_Y (640.0f)
#define PROJECT_TITLE ("Empire of Waves")

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void mouse_click(GLFWwindow *, int, int, int);
void mouse_pos(GLFWwindow *, double, double);
void mouse_scroll(GLFWwindow *, double, double);
void fb_size_callback(GLFWwindow *, int, int);
