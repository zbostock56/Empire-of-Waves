#include <controls.h>
/*
                                  CONTROLS.c
Implements the functionality for detecting and handling keyboard and mouse
events.
*/

void keyboard_input(GLFWwindow *window) {
  if (mode == EXPLORATION) {
    // Exploration mode keyboard handlers here
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      vec2 movement = GLM_VEC2_ZERO_INIT;
      vec2 world_coords = GLM_VEC2_ZERO_INIT;;
      if (e_player.embarked) {
        chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                       world_coords);

        glm_vec2_scale(e_player.ship_direction, delta_time, movement);
        glm_vec2_add(movement, world_coords, world_coords);
        world_to_chunk(world_coords, e_player.ship_chunk,
                       e_player.ship_coords);
      }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    }
  } else {
    // Combat mode keyboard handlers here
  }
}

void mouse_pos(GLFWwindow *window, double x_pos, double y_pos) {
  double x = 2.0 * ((x_pos / RES_X) - 0.5);
  double y = 2.0 * (((RES_Y - y_pos) / RES_Y) - 0.5);

  if (mode == EXPLORATION) {
    e_player.ship_direction[0] = x;
    e_player.ship_direction[1] = y;
    glm_vec2_normalize(e_player.ship_direction);
  } else {
    c_player.direction[0] = x;
    c_player.direction[1] = y;
    glm_vec2_normalize(c_player.direction);
  }
}

void mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
  // Mouse scroll handler here
}

void fb_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
