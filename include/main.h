#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_scene();
void init_chunks();
void keyboard_input(GLFWwindow *);
void manage_chunks();
void render_scene(GLFWwindow *);
void cleanup_scene();
void init_menus();
void update_event_timer();
