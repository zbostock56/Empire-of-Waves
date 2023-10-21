#include <controls.h>

/*
                                  CONTROLS.c
Implements the functionality for detecting and handling keyboard and mouse
events.
*/

void keyboard_input(GLFWwindow *window) {
  if (mode == EXPLORATION) {
    if (get_ui_component_by_ID(SAVE_INPUT)->enabled) {
      load_keys(window);
    } else {
      // Exploration mode keyboard handlers here
      exploration_movement(window);
      close_merchant_menu(window);
    }
  } else {
    // Combat mode keyboard handlers here
    combat_movement(window);
  }
  debug_keys(window);
}

void mouse_pos(GLFWwindow *window, double x_pos, double y_pos) {
  mouse_position[0] = 2.0 * ((x_pos / RES_X) - 0.5);
  mouse_position[1] = 2.0 * (((RES_Y - y_pos) / RES_Y) - 0.5);

  if (mode == EXPLORATION) {
    if (e_player.embarked) {
      e_player.ship_direction[0] = mouse_position[0];
      e_player.ship_direction[1] = mouse_position[1];
      glm_vec2_normalize(e_player.ship_direction);
    } else {
      e_player.direction[0] = mouse_position[0];
      e_player.direction[1] = mouse_position[1];
      glm_vec2_normalize(e_player.direction);
    }
  } else {
    c_player.direction[0] = mouse_position[0];
    c_player.direction[1] = mouse_position[1];
    glm_vec2_normalize(c_player.direction);
  }

  ui_hover_listener(mouse_position[0], mouse_position[1]);
}

void mouse_click(GLFWwindow *window, int button, int action, int mods) {
  if (mode == COMBAT) {
    if (action == GLFW_PRESS && c_player.attack_cooldown == 0.0) {
      c_player.speed = 0.5;
    } else if (action != GLFW_PRESS && c_player.attack_cooldown == 0.0) {
      c_player.attack_cooldown = c_player.fire_rate;
      c_player.attack_active = 0.1;
      c_player.speed = 1.0;
    }
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    ui_click_listener(mouse_position[0], mouse_position[1]);
  }
}

void mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
  // Mouse scroll handler here
  float modifier = 0.01 * y_offset;
  if ((y_offset < 0.0 && T_WIDTH + modifier > 0.001) ||
      (y_offset > 0.0 && T_WIDTH + modifier <= 1.0)) {
    T_WIDTH += modifier;
  }

  if (T_WIDTH <= 0.0) {
    T_WIDTH = 0.001;
  } else if (T_WIDTH > 1.0) {
    T_WIDTH = 1.0;
  }
}

void fb_size_callback(GLFWwindow *window, int width, int height) {
  RES_X = width;
  RES_Y = height;
  glViewport(0, 0, RES_X, RES_Y);
  refresh_framebuffers();
}

void exploration_movement(GLFWwindow *window) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords,
                  world_coords);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec2 movement = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
      glm_vec2_scale(e_player.ship_direction, delta_time, movement);
      glm_vec2_add(movement, world_coords, world_coords);
      world_to_chunk(world_coords, e_player.ship_chunk,
                     e_player.ship_coords);
    } else {
        glm_vec2_scale(e_player.direction, delta_time, movement);
        glm_vec2_add(movement, world_coords, world_coords);
        world_to_chunk(world_coords, e_player.chunk,
                      e_player.coords);
    }
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec2 movement = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
      glm_vec2_scale(e_player.ship_direction, delta_time, movement);
      glm_vec2_sub(world_coords, movement, world_coords);
      world_to_chunk(world_coords, e_player.ship_chunk,
                     e_player.ship_coords);
    } else {
      glm_vec2_scale(e_player.direction, delta_time, movement);
      glm_vec2_sub(world_coords, movement, world_coords);
      world_to_chunk(world_coords, e_player.chunk,
                     e_player.coords);
    }
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !holding_interaction) {
    if (shore_interaction_enabled) {
      if (e_player.embarked) {
        e_player.embarked = 0;
        glm_ivec2_copy(e_player.ship_chunk, e_player.chunk);
        glm_vec2_copy(e_player.ship_coords, e_player.coords);
        glm_vec2_copy(e_player.ship_direction, e_player.direction);
      } else {
        e_player.embarked = 1;
      }
    }
    if (!e_player.embarked && dialog.merchant &&
        !trade.ui_listing[0]->enabled) {
      if (set_dialog(MERCHANT_OPTION, "Merchant",
                     "Hail, Captain! What brings you to my humble stall")) {
        open_dialog();
      }
      get_ui_component_by_ID(INTERACT_PROMPT)->enabled = 0;
      dialog.merchant = NULL;
    }
    holding_interaction = 1;
  } else if (glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS) {
    holding_interaction = 0;
  }
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !holding_esc) {
    if (!trade.ui_listing[0]->enabled && !dialog.ui_text_name->enabled) {
      if (save_menu_opened()) {
        close_save_menu();
      } else {
        open_save_menu();
      }
    }
    holding_esc = 1;
  } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
    holding_esc = 0;
  }
}

void combat_movement(GLFWwindow *window) {
  vec2 movement = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(c_player.direction, (delta_time * c_player.speed) / T_WIDTH,
                 movement);
  // Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    glm_vec2_add(movement, c_player.coords, c_player.coords);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    glm_vec2_sub(c_player.coords, movement, c_player.coords);
  }
}

void debug_keys(GLFWwindow *window) {
  if (mode == EXPLORATION) {
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && !holding_equals) {
      if (e_player.embarked) {
        e_player.embarked = 0;
        glm_ivec2_copy(e_player.ship_chunk, e_player.chunk);
        glm_vec2_copy(e_player.ship_coords, e_player.coords);
        glm_vec2_copy(e_player.ship_direction, e_player.direction);
      } else {
        e_player.embarked = 1;
      }
      holding_equals = 1;
    } else if (glfwGetKey(window, GLFW_KEY_EQUAL) != GLFW_PRESS) {
      holding_equals = 0;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !holding_minus) {
    if (mode == EXPLORATION) {
      CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
      // Make a dummy enemy ship to fight
      unsigned int new_enemy_index = cur_chunk->num_enemies;
      cur_chunk->enemies[new_enemy_index].crew_count = 3;
      cur_chunk->num_enemies++;
      to_combat_mode(new_enemy_index);
    } else {
      from_combat_mode();
    }
    holding_minus = 1;
  } else if (glfwGetKey(window, GLFW_KEY_MINUS) != GLFW_PRESS) {
    holding_minus = 0;
  }
}

// =============================== HELPERS ===================================

void close_merchant_menu(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    if (dialog.ui_text_name->enabled) {
      close_dialog();
    }
    if (trade.ui_listing[0]->enabled) {
      close_trade();
    }
  }
}

void ui_click_listener(double x_pos, double y_pos) {
  for (int i = 0; i < NUM_COMPONENTS; i++) {
    if (!ui_tab[i].enabled || ui_tab[i].on_click == NULL) {
      continue;
    }

    // { x_min, y_min, x_max, y_max }
    vec4 min_max = GLM_VEC4_ZERO_INIT;
    get_ui_min_max(ui_tab + i, min_max);

    if (x_pos >= min_max[X_MIN] && x_pos <= min_max[X_MAX] &&
        y_pos >= min_max[Y_MIN] && y_pos <= min_max[Y_MAX]) {
      ui_tab[i].on_click(ui_tab[i].on_click_args);
    }
  }
}

void ui_hover_listener(double x_pos, double y_pos) {
  for (int i = 0; i < NUM_COMPONENTS; i++) {
    if (!ui_tab[i].enabled || ui_tab[i].on_hover == NULL) {
      continue;
    }

    // { x_min, y_min, x_max, y_max }
    vec4 min_max = GLM_VEC4_ZERO_INIT;
    get_ui_min_max(ui_tab + i, min_max);

    if (x_pos >= min_max[X_MIN] && x_pos <= min_max[X_MAX] &&
        y_pos >= min_max[Y_MIN] && y_pos <= min_max[Y_MAX]) {
      ui_tab[i].on_hover(ui_tab[i].on_hover_args);
    }
  }
}

void load_keys(GLFWwindow *window) {
  char *input_buffer = get_ui_component_by_ID(SAVE_INPUT)->text;
  // LETTERS
  for (int i = GLFW_KEY_A; i <= GLFW_KEY_Z; i++) {
    if (glfwGetKey(window, i) == GLFW_PRESS && !holding_alpha[i - GLFW_KEY_A]) {
      holding_alpha[i - GLFW_KEY_A] = 1;
      if (load_input_len < INPUT_BUFFER_SIZE - 1) {
        input_buffer[load_input_len] = i + 32;
        input_buffer[++load_input_len] = '\0';
      }
    } else if (glfwGetKey(window, i) != GLFW_PRESS) {
      holding_alpha[i - GLFW_KEY_A] = 0;
    }
  }
  // NUMBERS
  for (int i = GLFW_KEY_0; i < GLFW_KEY_9; i++) {
    if (glfwGetKey(window, i) == GLFW_PRESS && !holding_num[i - GLFW_KEY_0]) {
      holding_num[i - GLFW_KEY_0] = 1;
      if (load_input_len < INPUT_BUFFER_SIZE - 1) {
        input_buffer[load_input_len] = i;
        input_buffer[++load_input_len] = '\0';
      }
    } else if (glfwGetKey(window, i) != GLFW_PRESS) {
      holding_num[i - GLFW_KEY_0] = 0;
    }
  }

  // SPACE
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !holding_space) {
    holding_space = 1;
    if (load_input_len < INPUT_BUFFER_SIZE - 1) {
      input_buffer[load_input_len] = ' ';
      input_buffer[++load_input_len] = '\0';
    }
  } else if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS) {
    holding_space = 0;
  }

  // UNDERSCORE
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !holding_underscore) {
    holding_underscore = 1;
    if (load_input_len < INPUT_BUFFER_SIZE - 1) {
      input_buffer[load_input_len] = '_';
      input_buffer[++load_input_len] = '\0';
    }
  } else if (glfwGetKey(window, GLFW_KEY_MINUS) != GLFW_PRESS) {
    holding_underscore = 0;
  }

  // ENTER
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !holding_enter) {
    holding_enter = 1;

    if (open_prompt == LOAD) {
      load_game(input_buffer);
    } else if (open_prompt == NEW_GAME) {
      new_game(input_buffer);
    }
    close_save_menu();

    load_input_len = 0;
    input_buffer[load_input_len] = '\0';
  } else if (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS) {
    holding_enter = 0;
  }

  // BACKSPACE
  if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && !holding_bs) {
    if (load_input_len) {
      load_input_len--;
      input_buffer[load_input_len] = '\0';
    }
    holding_bs = 1;
  } else if (glfwGetKey(window, GLFW_KEY_BACKSPACE) != GLFW_PRESS) {
    holding_bs = 0;
  }
}
