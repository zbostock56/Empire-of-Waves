#include <collision.h>

/*
                                COLLISION.c
Implements the functionality for detecting and handling collisions across the
various other systems in the application.
*/

/* Checks collision between two bounding boxes */
int aabb_collision(float *p1, float w1, float h1, float *p2, float w2,
                   float h2) {
  float box1_top = p1[Y]+h1/2;
  float box2_top = p2[Y]+h2/2;
  float box1_bot = p1[Y]-h1/2;
  float box2_bot = p2[Y]-h2/2;
  float box1_left = p1[X]-w1/2;
  float box2_left = p2[X]-w2/1;
  float box1_right = p1[X]+w1/2;
  float box2_right = p2[X]+w2/2;

  if (box1_right >= box2_left &&
      box1_left <= box2_right &&
      box1_top >= box2_bot &&
      box1_bot <= box2_top)
  {
    return true;
  }

  return false;
}

int circle_aabb_collision(vec2 a_center, float a_radius, vec2 b_top_left,
                          float b_width, float b_height, vec2 correction) {
  float by_max = b_top_left[Y];
  float by_min = b_top_left[Y] - b_height;
  float bx_max = b_top_left[X] + b_width;
  float bx_min = b_top_left[X];

  // Calculate closest point on aabb to circle
  vec2 closest_aabb_point = {
    fmax(bx_min, fmin(bx_max, a_center[X])), // Clamp x value of circle's
                                             // center between max and min x
                                             // value of aabb
    fmax(by_min, fmin(by_max, a_center[Y])) // Clamp y value of circle's center
                                            // between max and min y value of
                                            // aabb
  };

  float dist_circle_to_closest = glm_vec2_distance(closest_aabb_point,
                                                   a_center);
  // If the distance from the circle's center to the closest point on the aabb
  // is less than the radius of the circle, a collision has occured which can
  // be corrected along the vector from the circle to the closest point
  float correction_distance = a_radius - dist_circle_to_closest;
  if (correction_distance > 0.0) {
    glm_vec2_sub(a_center, closest_aabb_point, correction);
    glm_vec2_scale_as(correction, correction_distance, correction);
    return 1;
  }

  return 0;
}

// Performs collision detection of two circles
int circle_circle_collision(vec2 a_center, float a_radius, vec2 b_center,
                            float b_radius) {
  // If the distance from circle a's center to circle b's center is less than
  // the sum of their radii, then a collision has occured
  float ab_dist = glm_vec2_distance(a_center, b_center);
  if (ab_dist < a_radius + b_radius) {
    return 1;
  }
  return 0;
}

/*
   helper function for getting the island player's currently on
*/
ISLAND *cur_island(CHUNK *chunk, vec2 world_coords, float radius) {
  /* Finds which island the the player is on by checking if they collide */
  for (int i = 0; i < chunk->num_islands; i++) {
    vec2 island_coords = { chunk->islands[i].coords[X],
                           chunk->islands[i].coords[Y] };
    vec2 island_coords_world = GLM_VEC2_ZERO_INIT;
    chunk_to_world(chunk->coords, island_coords, island_coords_world);
    vec2 correction = GLM_VEC2_ZERO_INIT;
    if (circle_aabb_collision(world_coords, radius, island_coords_world,
                              I_WIDTH * T_WIDTH, I_WIDTH * T_WIDTH,
                              correction)) {
      return chunk->islands + i;
    }
  }

  return NULL;
}

/*
    Helper function for checking the tile's type of a given local coordinate

    - coords is given in chunk space
*/
int check_tile(ISLAND *island, vec2 coords) {
  ivec2 tile_coords = { coords[X], coords[Y] };
  glm_ivec2_sub(tile_coords, island->coords, tile_coords);
  int tile_index = tile_coords[Y] * I_WIDTH + tile_coords[X];
  if (tile_index < 0 || tile_index >= I_WIDTH * I_WIDTH) {
    return OCEAN;
  }

  return island->tiles[tile_index];
}

// Exploration mode collision:

void detect_collisions() {
  if (mode == EXPLORATION) {
    // Collision with world
    // Player
    if (e_player.embarked) {
      ship_collisions(player_chunks + PLAYER_CHUNK, e_player.ship_chunk,
                      e_player.ship_coords);

      // Collision with enemy ships
      detect_enemy_ships();
    } else {
      character_collisions(player_chunks + PLAYER_CHUNK, e_player.chunk,
                           e_player.coords);
    }

    for (int i = 0; i < num_trade_ships; i++) {
      ship_collisions(&trade_ships[i].chunk, trade_ships[i].chunk_coords,
                      trade_ships[i].coords);
      trade_ship_collision(trade_ships + i);
    }

    // Disembark / embark contexts
    detect_context_interaction();
  } else {
    unit_collision(c_player.coords);
  }
}

void detect_enemy_ships() {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(e_player.ship_chunk, e_player.ship_coords, world_coords);

  CHUNK *chunk = player_chunks + PLAYER_CHUNK;
  E_ENEMY *cur_enemy = NULL;
  vec2 cur_enemy_world_coords = GLM_VEC2_ZERO_INIT;
  if (chunk->enemies) {
    for (int i = 0; i < chunk->num_enemies; i++) {
      cur_enemy = chunk->enemies + i;
      chunk_to_world(cur_enemy->chunk, cur_enemy->coords,
                     cur_enemy_world_coords);

      if (circle_circle_collision(world_coords,
                                  SHIP_COLLISION_RADIUS * T_WIDTH,
                                  cur_enemy_world_coords,
                                  SHIP_COLLISION_RADIUS * T_WIDTH)) {
        glm_vec2_zero(c_player.coords);
        glm_vec2_zero(c_player.direction);
        c_player.direction[0] = 1.0;
        mode = COMBAT;
      }
    }
  }
}

void detect_context_interaction() {
  // Disembark / embark
  vec2 world_coords_ship = GLM_VEC2_ZERO_INIT;
  vec2 world_coords_char = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   world_coords_ship);
    float radius = T_WIDTH * CHARACTER_COLLISION_RADIUS;
    ISLAND *island = cur_island(player_chunks + PLAYER_CHUNK,
                                world_coords_ship, radius);
    if (island) {
      int tile = check_tile(island, e_player.ship_coords);
      if (tile == SHORE) {
        // Enable disembark prompt
        shore_interaction_enabled = 1;
      } else {
        // Disable disembark prompt
        shore_interaction_enabled = 0;
      }
    }
  } else {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   world_coords_ship);
    chunk_to_world(e_player.chunk, e_player.coords, world_coords_char);

    if (circle_circle_collision(world_coords_ship,
                                INTERACTION_RADIUS * T_WIDTH,
                                world_coords_char,
                                CHARACTER_COLLISION_RADIUS * T_WIDTH)) {
      // Enable embark prompt
      shore_interaction_enabled = 1;
    } else {
      // Disabled embarked prompt
      shore_interaction_enabled = 0;
    }
  }

  // Merchant Interaction
}

/*
    Handles the collision of a character

    - chunk_coords and coords are given in chunk space
*/
void character_collisions(CHUNK *chunk, ivec2 chunk_coords, vec2 coords) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(chunk_coords, coords, world_coords);

  float radius = T_WIDTH * CHARACTER_COLLISION_RADIUS;
  ISLAND *island = cur_island(chunk, world_coords, radius);
  if (!island) {
    return;
  }

  vec2 search_world_coords = GLM_VEC2_ZERO_INIT;
  vec2 search_tile = GLM_VEC2_ZERO_INIT;
  int cur_tile = OCEAN;

  int search_min_x = ((int) coords[X]) - ceil(CHARACTER_COLLISION_RADIUS);
  int search_max_x = ((int) coords[X]) + ceil(CHARACTER_COLLISION_RADIUS);
  int search_min_y = ((int) coords[Y]) - ceil(CHARACTER_COLLISION_RADIUS);
  int search_max_y = ((int) coords[Y]) + ceil(CHARACTER_COLLISION_RADIUS);
  vec2 collision_correction = GLM_VEC2_ZERO_INIT;
  for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
    for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
      search_tile[X] = cur_x;
      search_tile[Y] = cur_y;
      chunk_to_world(chunk_coords, search_tile, search_world_coords);
      cur_tile = check_tile(island, search_tile);

      if ((cur_tile == OCEAN) &&
          circle_aabb_collision(world_coords, radius, search_world_coords,
                                T_WIDTH, T_WIDTH, collision_correction)) {
        glm_vec2_add(world_coords, collision_correction, world_coords);
        world_to_chunk(world_coords, chunk_coords, coords);
      }
    }
  }
}

/*
    Handles the collision of a ship

    - chunk_coords and coords are given in chunk space
*/
void ship_collisions(CHUNK *chunk, ivec2 chunk_coords, vec2 coords) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(chunk_coords, coords, world_coords);

  float radius = T_WIDTH * SHIP_COLLISION_RADIUS;
  ISLAND *island = cur_island(chunk, world_coords, radius);
  if (!island) {
    return;
  }

  vec2 search_world_coords = GLM_VEC2_ZERO_INIT;
  vec2 search_tile = GLM_VEC2_ZERO_INIT;
  int cur_tile = OCEAN;

  int search_min_x = ((int) coords[X]) - ceil(SHIP_COLLISION_RADIUS);
  int search_max_x = ((int) coords[X]) + ceil(SHIP_COLLISION_RADIUS);
  int search_min_y = ((int) coords[Y]) - ceil(SHIP_COLLISION_RADIUS);
  int search_max_y = ((int) coords[Y]) + ceil(SHIP_COLLISION_RADIUS);
  vec2 collision_correction = GLM_VEC2_ZERO_INIT;
  for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
    for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
      search_tile[X] = cur_x;
      search_tile[Y] = cur_y;
      chunk_to_world(chunk_coords, search_tile, search_world_coords);
      cur_tile = check_tile(island, search_tile);

      if ((cur_tile != OCEAN && cur_tile != SHORE) &&
          circle_aabb_collision(world_coords, radius, search_world_coords,
                                T_WIDTH, T_WIDTH, collision_correction)) {
        glm_vec2_add(world_coords, collision_correction, world_coords);
        world_to_chunk(world_coords, chunk_coords, coords);
      }
    }
  }
}

// Steer trade ship away from obstructive tiles of non-target islands
void trade_ship_steering(TRADE_SHIP *trade_ship, vec2 direction) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(trade_ship->chunk_coords, trade_ship->coords, world_coords);

  float radius = T_WIDTH * SHIP_COLLISION_RADIUS;
  ISLAND *target_island = trade_ship->target_chunk.islands +
                          trade_ship->target_island;
  ISLAND *island = cur_island(&trade_ship->chunk, world_coords, radius);
  if (!island) {
    return;
  }

  vec2 search_world_coords = GLM_VEC2_ZERO_INIT;
  vec2 search_tile = GLM_VEC2_ZERO_INIT;
  vec2 from_tile = GLM_VEC2_ZERO_INIT;
  float tile_dist = 0.0;
  vec2 steer_dir = GLM_VEC2_ZERO_INIT;
  int cur_tile = OCEAN;

  int search_min_x = ((int) trade_ship->coords[X]) -
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_max_x = ((int) trade_ship->coords[X]) +
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_min_y = ((int) trade_ship->coords[Y]) -
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_max_y = ((int) trade_ship->coords[Y]) +
                     ceil(SHIP_PATHFIND_RADIUS);
  for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
    for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
      search_tile[X] = cur_x;
      search_tile[Y] = cur_y;
      chunk_to_world(trade_ship->chunk_coords, search_tile,
                     search_world_coords);
      tile_dist = glm_vec2_distance(world_coords, search_world_coords);
      glm_vec2_sub(world_coords, search_world_coords, from_tile);
      glm_vec2_normalize(from_tile);
      glm_vec2_scale(from_tile, tile_dist / SHIP_PATHFIND_RADIUS, from_tile);
      cur_tile = check_tile(island, search_tile);

      if (island == target_island) {
        if (cur_tile != SHORE) {
          glm_vec2_add(from_tile, steer_dir, steer_dir);
        } else {
          glm_vec2_sub(steer_dir, from_tile, steer_dir);
        }
      } else {
        if (cur_tile != OCEAN) {
          glm_vec2_add(from_tile, steer_dir, steer_dir);
        } else {
          glm_vec2_sub(steer_dir, from_tile, steer_dir);
        }
      }
    }
  }

  glm_vec2_normalize(steer_dir);
  //glm_vec2_scale(steer_dir, delta_time, steer_dir);
  glm_vec2_add(steer_dir, direction, direction);
}

// Detect if trade ship completed its route
void trade_ship_collision(TRADE_SHIP *trade_ship) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(trade_ship->chunk_coords, trade_ship->coords, world_coords);

  float radius = T_WIDTH * SHIP_COLLISION_RADIUS;
  ISLAND *target_island = trade_ship->target_chunk.islands +
                          trade_ship->target_island;
  ISLAND *island = cur_island(&trade_ship->chunk, world_coords, radius);
  if (
    !island ||
    target_island->chunk[X] != island->chunk[X] ||
    target_island->chunk[Y] != island->chunk[Y] ||
    target_island->coords[X] != island->coords[X] ||
    target_island->coords[Y] != island->coords[Y]
  ) {
    return;
  }

  vec2 search_tile = GLM_VEC2_ZERO_INIT;
  int cur_tile = OCEAN;

  int search_min_x = ((int) trade_ship->coords[X]) -
                     ceil(SHIP_COLLISION_RADIUS);
  int search_max_x = ((int) trade_ship->coords[X]) +
                     ceil(SHIP_COLLISION_RADIUS);
  int search_min_y = ((int) trade_ship->coords[Y]) -
                     ceil(SHIP_COLLISION_RADIUS);
  int search_max_y = ((int) trade_ship->coords[Y]) +
                     ceil(SHIP_COLLISION_RADIUS);
  for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
    for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
      search_tile[X] = cur_x;
      search_tile[Y] = cur_y;
      cur_tile = check_tile(island, search_tile);

      if (cur_tile == SHORE) {
        // Finished route
        glm_ivec2_zero(trade_ship->chunk_coords);
        glm_vec2_copy(home_island_coords, trade_ship->coords);
      }
    }
  }

}

// Combat mode collision:
// Performs collision detection and resolution for combat units
// - coords are given as tile coordinates
void unit_collision(vec2 coords) {
  float radius = CHARACTER_COLLISION_RADIUS * T_WIDTH;
  vec2 collision_correction = GLM_VEC2_ZERO_INIT;

  // Convert tile coordinates to world coordinate
  vec2 player_world_coords = {
    coords[X] * T_WIDTH,
    coords[Y] * T_WIDTH
  };

  vec2 left_wall = {
    -arena_dimensions[X] * T_WIDTH,
    arena_dimensions[Y] * 0.5 * T_WIDTH
  };
  if (circle_aabb_collision(player_world_coords, radius, left_wall,
                            0.5 * arena_dimensions[X] * T_WIDTH,
                            arena_dimensions[Y] * T_WIDTH,
                            collision_correction)) {
    // Convert world coords to "tile" coords
    collision_correction[X] /= T_WIDTH;
    collision_correction[Y] /= T_WIDTH;
    glm_vec2_add(coords, collision_correction, coords);
  }

  vec2 right_wall = {
    arena_dimensions[X] * 0.5 * T_WIDTH,
    arena_dimensions[Y] * 0.5 * T_WIDTH
  };
  if (circle_aabb_collision(player_world_coords, radius, right_wall,
                            0.5 * arena_dimensions[X] * T_WIDTH,
                            arena_dimensions[Y] * T_WIDTH,
                            collision_correction)) {
    // Convert world coords to "tile" coords
    collision_correction[X] /= T_WIDTH;
    collision_correction[Y] /= T_WIDTH;
    glm_vec2_add(coords, collision_correction, coords);
  }

  vec2 top_wall = {
    -arena_dimensions[X] * 0.5 * T_WIDTH,
    arena_dimensions[Y] * T_WIDTH
  };
  if (circle_aabb_collision(player_world_coords, radius, top_wall,
                            arena_dimensions[X] * T_WIDTH,
                            0.5 * arena_dimensions[Y] * T_WIDTH,
                            collision_correction)) {
    // Convert world coords to "tile" coords
    collision_correction[X] /= T_WIDTH;
    collision_correction[Y] /= T_WIDTH;
    glm_vec2_add(coords, collision_correction, coords);
  }

  vec2 bottom_wall = {
    -arena_dimensions[X] * 0.5 * T_WIDTH,
    -arena_dimensions[Y] * 0.5 * T_WIDTH
  };
  if (circle_aabb_collision(player_world_coords, radius, bottom_wall,
                            arena_dimensions[X] * T_WIDTH,
                            0.5 * arena_dimensions[Y] * T_WIDTH,
                            collision_correction)) {
    // Convert world coords to "tile" coords
    collision_correction[X] /= T_WIDTH;
    collision_correction[Y] /= T_WIDTH;
    glm_vec2_add(coords, collision_correction, coords);
  }
}
