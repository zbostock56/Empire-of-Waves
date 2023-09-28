#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  init_scene();

  while (!glfwWindowShouldClose(window)) {
    keyboard_input(window);

    // Insert all simulation logic here:
    // - collision detection
    // - pathfinding
    // - chunk serialization
    // - etc...

    render_scene(window);
  }

  // Insert all "cleanup" functionality here
  cleanup_scene();

  glfwTerminate();

  return 0;
}
