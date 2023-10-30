#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  init_game("eow");
  init_scene();

  int status = 0;
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
    update_enemy_ships();
    update_projectiles();

    status = detect_collisions();
    if (status) {
      return -1;
    }

    update_timers();
    print_coords();
    update_console_prompt();
    update_dialog_buffers();
    update_available_mercenaries();
    update_mercenary_reassignment();
    render_scene(window);
    update_combat_state();
    update_event_timer();
    update_save_interval();
  }

  // Insert all "cleanup" functionality here
  free_game();
  cleanup_scene();

  glfwTerminate();
}
