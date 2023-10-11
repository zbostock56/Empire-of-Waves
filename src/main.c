#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  int status = 0;
  init_chunks();
  init_scene();
  status = init_trade_ship_buffers();
  if (status) {
    return -1;
  }

  while (!glfwWindowShouldClose(window)) {
    keyboard_input(window);

    // Insert all simulation logic here:
    // - collision detection
    // - pathfinding
    // - chunk serialization
    // - etc...
    manage_chunks();
    update_trade_ships();
    detect_collisions();

    render_scene(window);
    update_event_timer();
  }

  // Insert all "cleanup" functionality here
  cleanup_scene();

  glfwTerminate();
}
