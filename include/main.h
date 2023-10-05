#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_scene();
void keyboard_input(GLFWwindow *);
void render_scene(GLFWwindow *);
void cleanup_scene();
void init_menus();
