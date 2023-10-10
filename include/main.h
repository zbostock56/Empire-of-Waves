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
void update_event_timer();
int detect_collisions();
void update_combat_state();
