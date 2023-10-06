#include <controls.h>
/*
                                  CONTROLS.c
Implements the functionality for detecting and handling keyboard and mouse
events.
*/

void keyboard_input(GLFWwindow *window) {
  if (mode == EXPLORATION) {
    // Exploration mode keyboard handlers here
    exploration_movement(window);
  } else {
    // Combat mode keyboard handlers here
    combat_movement(window);
  }
  debug_keys(window);
}

void mouse_pos(GLFWwindow *window, double x_pos, double y_pos) {
  double x = 2.0 * ((x_pos / RES_X) - 0.5);
  double y = 2.0 * (((RES_Y - y_pos) / RES_Y) - 0.5);

  if (mode == EXPLORATION) {
    if (e_player.embarked) {
      e_player.ship_direction[0] = x;
      e_player.ship_direction[1] = y;
      glm_vec2_normalize(e_player.ship_direction);
    } else {
      e_player.direction[0] = x;
      e_player.direction[1] = y;
      glm_vec2_normalize(e_player.direction);
    }
  } else {
    c_player.direction[0] = x;
    c_player.direction[1] = y;
    glm_vec2_normalize(c_player.direction);
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
  glViewport(0, 0, width, height);
}

void exploration_movement(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec2 movement = GLM_VEC2_ZERO_INIT;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;
    vec2 predicted_coords = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                     world_coords);

      glm_vec2_scale(e_player.ship_direction, delta_time, movement);
      glm_vec2_add(movement, world_coords, predicted_coords);
      world_to_chunk(predicted_coords, e_player.ship_chunk, predicted_coords);

      /*
        Only move the ship if the predicted tile is an Ocean or Shore tile
      */
      if (!ship_collisions(predicted_coords)) {
        glm_vec2_add(movement, world_coords, world_coords);
        world_to_chunk(world_coords, e_player.ship_chunk,
                    e_player.ship_coords); 
      }
      
      /*
        Reverse the direction if the ship collided with obstructive tile.
      */
      // else {
      //   glm_vec2_scale(e_player.ship_direction, -1.0, e_player.ship_direction);
      // }
    } else {
      
        chunk_to_world(e_player.chunk, e_player.coords,
                      world_coords);

        glm_vec2_scale(e_player.direction, delta_time, movement);
        glm_vec2_add(movement, world_coords, predicted_coords);
        world_to_chunk(predicted_coords, e_player.chunk, predicted_coords);

        /*
          Only move the player if the predicted tile is not an 'obstructive' tile
        */
        if (!player_collisions(predicted_coords)) {
          glm_vec2_add(movement, world_coords, world_coords);
          world_to_chunk(world_coords, e_player.chunk,
                        e_player.coords);
        }
    }
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec2 movement = GLM_VEC2_ZERO_INIT;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;;
    if (e_player.embarked) {
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                     world_coords);

      glm_vec2_scale(e_player.ship_direction, delta_time, movement);
      glm_vec2_sub(world_coords, movement, world_coords);
      world_to_chunk(world_coords, e_player.ship_chunk,
                     e_player.ship_coords);
    } else {
      chunk_to_world(e_player.chunk, e_player.coords,
                     world_coords);

      glm_vec2_scale(e_player.direction, delta_time, movement);
      glm_vec2_sub(world_coords, movement, world_coords);
      world_to_chunk(world_coords, e_player.chunk,
                     e_player.coords);
    }
  }
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && !holding_equals) {
    printf("Pressing \n");
    vec2 movement = GLM_VEC2_ZERO_INIT;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;
    vec2 predicted_coords = GLM_VEC2_ZERO_INIT;
    
    chunk_to_world(trade_ships[0].chunk, trade_ships[0].coords,
                    world_coords);

    glm_vec2_scale(trade_ships[0].direction, delta_time, movement);
    glm_vec2_add(world_coords, movement, predicted_coords);
    world_to_chunk(predicted_coords, trade_ships[0].chunk, predicted_coords);
    if (!ship_collisions(predicted_coords)) {
      glm_vec2_add(world_coords, movement, world_coords);
      world_to_chunk(world_coords, trade_ships[0].chunk,
                     trade_ships[0].coords);
      
    } else {
      glm_vec2_rotate(trade_ships[0].direction,  90.0, trade_ships[0].direction);
    }
    
    holding_equals = 1;
  } else if (glfwGetKey(window, GLFW_KEY_0) != GLFW_PRESS) {
    holding_equals = 0;
  }

  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && !holding_equals) {
    printf("Pressing \n");
    vec2 movement = GLM_VEC2_ZERO_INIT;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;
    vec2 predicted_coords = GLM_VEC2_ZERO_INIT;
    
    chunk_to_world(player_chunks[4].enemies[0].chunk, player_chunks[4].enemies[0].coords,
                    world_coords);

    glm_vec2_scale(player_chunks[4].enemies[0].direction, delta_time, movement);
    glm_vec2_add(world_coords, movement, predicted_coords);
    world_to_chunk(predicted_coords, player_chunks[4].enemies[0].chunk, predicted_coords);
    if (!ship_collisions(predicted_coords)) {
      glm_vec2_add(world_coords, movement, world_coords);
      world_to_chunk(world_coords, player_chunks[4].enemies[0].chunk,
                     player_chunks[4].enemies[0].coords);
      
    } else {
      glm_vec2_rotate(player_chunks[4].enemies[0].direction,  90.0, player_chunks[4].enemies[0].direction);
    }
    
    holding_equals = 1;
  } else if (glfwGetKey(window, GLFW_KEY_0) != GLFW_PRESS) {
    holding_equals = 0;
  }
  
}

void combat_movement(GLFWwindow *window) {
  vec2 movement = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(c_player.direction, delta_time, movement);
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
      glm_vec2_zero(c_player.coords);
      glm_vec2_zero(c_player.direction);
      c_player.direction[0] = 1.0;
      mode = COMBAT;
    } else {
      mode = EXPLORATION;
    }
    holding_minus = 1;
  } else if (glfwGetKey(window, GLFW_KEY_MINUS) != GLFW_PRESS) {
    holding_minus = 0;
  }
}
