#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  int status = 0;
  status = init_chunks();
  if (status) {
    return -1;
  }

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
    status = manage_chunks();
    if (status) {
      return -1;
    }

    update_trade_ships();
    
    status = detect_collisions();
    if (status) {
      return -1;
    }

    render_scene(window);
    update_combat_state();
    update_event_timer();
  }

  // Insert all "cleanup" functionality here
  cleanup_scene();

  glfwTerminate();
}
