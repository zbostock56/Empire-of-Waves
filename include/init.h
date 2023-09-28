#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define RES_X (640.0f)
#define RES_Y (640.0f)
#define PROJECT_TITLE ("Empire of Waves")

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void mouse_pos(GLFWwindow *, double, double);
void mouse_scroll(GLFWwindow *, double, double);
void fb_size_callback(GLFWwindow *, int, int);
