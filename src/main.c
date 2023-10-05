#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  init_chunks();
  init_scene();

  while (!glfwWindowShouldClose(window)) {
    keyboard_input(window);

    // Insert all simulation logic here:
    // - collision detection
    // - pathfinding
    // - chunk serialization
    // - etc...
    manage_chunks();

    render_scene(window);
    //update_event_timer();
  }

  // Insert all "cleanup" functionality here
  cleanup_scene();

  glfwTerminate();
}
