#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  init_menus();
  init_dialog();
  init_trade();
  init_status_bar();
  init_ui_lists();
  init_container_ui();
  open_status_bar();

  int status = 0;
  status = init_save_menu();
  if (status) {
    return -1;
  }

  status = init_chunks();
  if (status) {
    return -1;
  }

  status = init_containers();
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
  cleanup_scene();

  glfwTerminate();
}
