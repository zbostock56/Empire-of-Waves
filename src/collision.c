#include <collision.h>

/*
                                COLLISION.c
Implements the functionality for detecting and handling collisions across the
various other systems in the application.
*/

int detect_collisions() {
  // Player
  if (mode == EXPLORATION) {
    CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
    // Collision with world
    if (e_player.embarked) {
      ship_collisions(cur_chunk, e_player.ship_chunk, e_player.ship_coords);

      // Collision with enemy ships
      detect_enemy_ships();
    } else {
      character_collisions(cur_chunk, e_player.chunk, e_player.coords);
    }

    // Enemy ship collision
    E_ENEMY *cur_enemy = NULL;
    for (int i = 0; i < chunk_buff_len; i++) {
      for (int j = 0; j < chunk_buffer[i].num_enemies; j++) {
        cur_enemy = &chunk_buffer[i].enemies[j];
        ship_collisions(chunk_buffer + i, cur_enemy->chunk, cur_enemy->coords);
      }
    }

    // Trade ship collision
    for (int i = 0; i < num_trade_ships; i++) {
      if (trade_ship_active(i)) {
        cur_chunk = chunk_buffer + trade_ships[i].cur_chunk_index;
        ship_collisions(cur_chunk, trade_ships[i].chunk_coords,
                        trade_ships[i].coords);
        trade_ship_collision(trade_ships + i);
      }
    }

    // Home island collision
    detect_island_invasion();

    // Disembark / embark contexts
    detect_context_interaction();
  } else {
    unit_collision(c_player.coords);
    for (int i = 0; i < num_npc_units; i++) {
      unit_collision(npc_units[i].coords);
    }
    attack_collision();
  }

  return 0;
}

// ===================== EXPLORATION MODE COLLISION ==========================

int detect_enemy_ships() {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(e_player.ship_chunk, e_player.ship_coords, world_coords);

  CHUNK *chunk = NULL;
  E_ENEMY *cur_enemy = NULL;
  vec2 cur_enemy_world_coords = GLM_VEC2_ZERO_INIT;
  int status = 0;
  for (int i = CHUNK_UPPER_LEFT; i <= CHUNK_LOWER_RIGHT; i++) {
    chunk = chunk_buffer + player_chunks[i];
    if (chunk->enemies) {
      for (int i = 0; i < chunk->num_enemies; i++) {
        cur_enemy = chunk->enemies + i;
        chunk_to_world(cur_enemy->chunk, cur_enemy->coords,
                      cur_enemy_world_coords);

        if (circle_circle_collision(world_coords,
                                    SHIP_COLLISION_RADIUS *T_WIDTH,
                                    cur_enemy_world_coords,
                                    SHIP_COLLISION_RADIUS *T_WIDTH)) {
          status = to_combat_mode(i);
          if (status) {
            return -1;
          }
        }
      }
    }
  }

  return 0;
}

// Detects collision with shore, player ship, merchants, etc for prompts
void detect_context_interaction() {
  // Disembark / embark
  vec2 world_coords_ship = GLM_VEC2_ZERO_INIT;
  vec2 world_coords_char = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   world_coords_ship);
    CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
    ISLAND *island = cur_island(cur_chunk, world_coords_ship);
    if (island) {
      int tile = check_tile(island, e_player.ship_coords);
      if (tile == SHORE) {
        // Enable disembark prompt
        shore_interaction_enabled = 1;
        get_ui_component_by_ID(EMBARK_PROMPT)->text = "Press 'e' to disembark";
        get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 1;
      } else {
        // Disable disembark prompt
        shore_interaction_enabled = 0;
        get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 0;
      }
    }
    if (trade.ui_button_trade->enabled || dialog.ui_text_name->enabled ||
        container_menu_open || reassignment_menu_open || save_menu_open()) {
      shore_interaction_enabled = 0;
      get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 0;
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
      get_ui_component_by_ID(EMBARK_PROMPT)->text = "Press 'e' to embark";
      get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 1;
    } else {
      // Disabled embarked prompt
      shore_interaction_enabled = 0;
      get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 0;
    }
    if (trade.ui_button_trade->enabled || dialog.ui_text_name->enabled ||
        container_menu_open || reassignment_menu_open || save_menu_open()) {
      shore_interaction_enabled = 0;
      get_ui_component_by_ID(EMBARK_PROMPT)->enabled = 0;
    }

    // Merchant Interaction
    get_ui_component_by_ID(INTERACT_PROMPT)->enabled = 0;
    check_merchant_prompt(world_coords_char);
    // Check mercenary reassignment prompt
    check_mercenary_reassignment_prompt(world_coords_char);
    // Check chest interaction prompt
    check_chest_prompt(world_coords_char);
  }
}

/*
  Checks if the player is within range of the home
  island home asset to see if the player can open
  the mercenary reassignment menu
*/
void check_mercenary_reassignment_prompt(vec2 coords) {
  CHUNK *chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  /* If the current chunk is not the home chunk, return. */
  if (chunk->coords[0] != 0 || chunk->coords[1] != 0) {
    return;
  }
  ISLAND *island = cur_island(chunk, coords);
  /* Check if on the home island, if not return */
  if (island != chunk->islands + 0) {
    return;
  }
  vec2 house_tile_world = GLM_VEC2_ZERO_INIT;
  chunk_to_world(island->chunk, house_tile, house_tile_world);
  float dist = glm_vec2_distance(house_tile_world, coords);
  UI_COMPONENT *interaction_prompt = get_ui_component_by_ID(INTERACT_PROMPT);
  home_interaction_enabled = 0;
  if (dist <= 3.0 * T_WIDTH && !reassignment_menu_open) {
    // prompt to reassign mercenaries
    interaction_prompt->enabled = 1;
    home_interaction_enabled = 1;
  } else if (dist > 3.0 * T_WIDTH) {
    close_mercenary_reassignment_menu();
    close_ransom_menu();
  }

  if (trade.ui_button_trade->enabled || dialog.ui_text_name->enabled ||
      container_menu_open || reassignment_menu_open || save_menu_open()) {
    interaction_prompt->enabled = 0;
    home_interaction_enabled = 0;
  }
}

void check_chest_prompt(vec2 coords) {
  CHUNK *chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  /* If the current chunk is not the home chunk, return. */
  if (chunk->coords[0] != 0 || chunk->coords[1] != 0) {
    return;
  }
  ISLAND *island = cur_island(chunk, coords);
  /* Check if on the home island, if not return */
  if (island != chunk->islands) {
    return;
  }
  vec2 home_box_world = GLM_VEC2_ZERO_INIT;
  chunk_to_world(island->chunk, home_box_tile, home_box_world);
  float dist = glm_vec2_distance(home_box_world, coords);
  UI_COMPONENT *interaction_prompt = get_ui_component_by_ID(INTERACT_PROMPT);
  container_interaction_enabled = 0;
  if (dist <= 3.0 * T_WIDTH && !container_menu_open) {
    // prompt to reassign mercenaries
    interaction_prompt->enabled = 1;
    container_interaction_enabled = 1;
  } else if (dist > 3.0 * T_WIDTH) {
    close_container();
  }
  if (trade.ui_button_trade->enabled || dialog.ui_text_name->enabled ||
      container_menu_open || reassignment_menu_open || save_menu_open()) {
    interaction_prompt->enabled = 0;
    container_interaction_enabled = 0;
  }
}

// Exploration mode collision:
void check_merchant_prompt(vec2 world_player_coords) {
  CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];

  ISLAND *cur_island = NULL;
  vec2 world_merchant_coords = GLM_VEC2_ZERO_INIT;
  float dist_to_merchant = 0.0;
  MERCHANT *cur_merchant = NULL;
  for (int i = 0; i < cur_chunk->num_islands && !cur_merchant; i++) {
    cur_island = cur_chunk->islands + i;
    if (cur_island->has_merchant) {
      chunk_to_world(cur_island->merchant.chunk, cur_island->merchant.coords,
                     world_merchant_coords);
      dist_to_merchant = glm_vec2_distance(world_merchant_coords,
                                           world_player_coords);
      if (dist_to_merchant <= INTERACTION_RADIUS * T_WIDTH) {
        get_ui_component_by_ID(INTERACT_PROMPT)->enabled = 1;
        cur_merchant = &cur_island->merchant;
        size_t island_index = i;
        UI_COMPONENT *tr_btn = dialog.ui_button_trade_route;
        tr_btn->on_click_args = (void *) island_index;
      }
    }
  }
  if (trade.ui_button_trade->enabled || dialog.ui_text_name->enabled ||
      container_menu_open || reassignment_menu_open || save_menu_open()) {
    get_ui_component_by_ID(INTERACT_PROMPT)->enabled = 0;
  }
  if (!cur_merchant) {
    close_dialog();
    close_trade();
  }
  close_merchant = cur_merchant;
}

/*
    Handles the collision of a character with ocean tiles

    - chunk_coords and coords are given in chunk space
*/
void character_collisions(CHUNK *chunk, ivec2 chunk_coords, vec2 coords) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(chunk_coords, coords, world_coords);

  float radius = T_WIDTH * CHARACTER_COLLISION_RADIUS;
  ISLAND *island = cur_island(chunk, world_coords);
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
    Handles the collision of a ship with land tiles

    - chunk_coords and coords are given in chunk space
*/
void ship_collisions(CHUNK *chunk, ivec2 chunk_coords, vec2 coords) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(chunk_coords, coords, world_coords);

  float radius = T_WIDTH * SHIP_COLLISION_RADIUS;
  ISLAND *island = cur_island(chunk, world_coords);
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

// Detect enemies and steer away to avoid
void trade_ship_detect_enemies(TRADE_SHIP *trade_ship, vec2 ship_dir,
                              CHUNK *trade_ship_chunk) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(trade_ship->chunk_coords, trade_ship->coords, world_coords);
  E_ENEMY *cur_enemy = NULL;
  vec2 cur_enemy_world_coords = GLM_VEC2_ZERO_INIT;
  // int status = 0;
  /* AVOID Steering behavior added across chunks if tradeship is in player_chunk[9]
    Assumption: e_player.ship_chunk corresponds to player_chunks[4]
  */
  vec2 target_dir = GLM_VEC2_ZERO_INIT;
  for (int i = 0; i < chunk_buff_len; i++) {
    for (int j = 0; j < chunk_buffer[i].num_enemies; j++) {
      cur_enemy = chunk_buffer[i].enemies+j;
      chunk_to_world(cur_enemy->chunk, cur_enemy->coords,
                     cur_enemy_world_coords);
      if (circle_circle_collision(world_coords,
                            SHIP_COLLISION_RADIUS*SHIP_CHASE_RADIUS*T_WIDTH,
                            cur_enemy_world_coords,
                            SHIP_COLLISION_RADIUS*SHIP_CHASE_RADIUS*T_WIDTH)) {
        glm_vec2_sub(world_coords, cur_enemy_world_coords, target_dir);
        glm_vec2_normalize(target_dir);
        glm_vec2_add(ship_dir, target_dir, ship_dir);
        glm_vec2_normalize(ship_dir);
      }
    }
  }
}

// Steer ship away from obstructive tiles of non-target islands
void ship_steering(ivec2 ship_chunk, vec2 ship_coords, vec2 target_dir,
                   CHUNK *cur_chunk, CHUNK *target_chunk,
                   unsigned int target_isl_index) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(ship_chunk, ship_coords, world_coords);

  ISLAND *target_island = target_chunk->islands + target_isl_index;
  ISLAND *island = cur_island(cur_chunk, world_coords);
  if (!island) {
    return;
  }

  vec2 search_world_coords = GLM_VEC2_ZERO_INIT;
  vec2 search_tile = GLM_VEC2_ZERO_INIT;
  vec2 from_tile = GLM_VEC2_ZERO_INIT;
  float tile_dist = 0.0;
  int cur_tile = OCEAN;

  int search_min_x = ((int) ship_coords[X]) -
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_max_x = ((int) ship_coords[X]) +
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_min_y = ((int) ship_coords[Y]) -
                     ceil(SHIP_PATHFIND_RADIUS);
  int search_max_y = ((int) ship_coords[Y]) +
                     ceil(SHIP_PATHFIND_RADIUS);
  for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
    for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
      search_tile[X] = cur_x;
      search_tile[Y] = cur_y;
      chunk_to_world(ship_chunk, search_tile, search_world_coords);
      tile_dist = glm_vec2_distance(world_coords, search_world_coords);
      glm_vec2_sub(world_coords, search_world_coords, from_tile);
      glm_vec2_normalize(from_tile);
      float magnitude = SHIP_PATHFIND_RADIUS;
      if (tile_dist) {
        magnitude /= tile_dist;
      }
      glm_vec2_scale(from_tile, magnitude, from_tile);
      cur_tile = check_tile(island, search_tile);

      if (island == target_island) {
        if (cur_tile != SHORE) {
          glm_vec2_add(from_tile, target_dir, target_dir);
        } else {
          glm_vec2_sub(target_dir, from_tile, target_dir);
        }
      } else {
        if (cur_tile != OCEAN) {
          glm_vec2_add(from_tile, target_dir, target_dir);
        } else {
          glm_vec2_sub(target_dir, from_tile, target_dir);
        }
      }
    }
  }

  glm_vec2_normalize(target_dir);
}

// Detect if trade ship completed its route or plundered
void trade_ship_collision(TRADE_SHIP *trade_ship) {
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(trade_ship->chunk_coords, trade_ship->coords, world_coords);

  CHUNK *cur_chunk = chunk_buffer + trade_ship->cur_chunk_index;
  CHUNK *target_chunk = chunk_buffer + trade_ship->target_chunk_index;
  ISLAND *target_island = target_chunk->islands + trade_ship->target_island;
  ISLAND *island = cur_island(cur_chunk, world_coords);
  if (island && target_island == island) {
    vec2 search_tile = GLM_VEC2_ZERO_INIT;
    int cur_tile = OCEAN;

    int search_min_x = ((int) trade_ship->coords[X]) -
                       ceil(SHIP_COMPLETION_RADIUS);
    int search_max_x = ((int) trade_ship->coords[X]) +
                       ceil(SHIP_COMPLETION_RADIUS);
    int search_min_y = ((int) trade_ship->coords[Y]) -
                       ceil(SHIP_COMPLETION_RADIUS);
    int search_max_y = ((int) trade_ship->coords[Y]) +
                       ceil(SHIP_COMPLETION_RADIUS);
    for (int cur_y = search_min_y; cur_y <= search_max_y; cur_y++) {
      for (int cur_x = search_min_x; cur_x <= search_max_x; cur_x++) {
        search_tile[X] = cur_x;
        search_tile[Y] = cur_y;
        cur_tile = check_tile(island, search_tile);

        if (cur_tile == SHORE) {
          // Finished route
          update_relationship(&target_island->merchant, 10.0);
          /* Give player copper reward */
          give_player_copper(10);

          glm_ivec2_zero(trade_ship->chunk_coords);
          glm_vec2_copy(home_island_coords, trade_ship->coords);
          return;
        }
      }
    }
  }

  vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
  E_ENEMY *cur_enemy = NULL;
  for (unsigned int i = 0; i < cur_chunk->num_enemies; i++) {
    cur_enemy = cur_chunk->enemies + i;
    chunk_to_world(cur_enemy->chunk, cur_enemy->coords, enemy_world_coords);

    if (circle_circle_collision(world_coords,
                                SHIP_COLLISION_RADIUS*T_WIDTH,
                                enemy_world_coords,
                                SHIP_COLLISION_RADIUS*T_WIDTH)) {
      int roll = rand() % 100;
      int upper_bound = 5 * trade_ship->num_mercenaries;
      if (roll > upper_bound) {
        trade_ship->death_animation = 1.0;
        return;
      } else {
        unsigned int last_index = --cur_chunk->num_enemies;
        cur_chunk->enemies[i] = cur_chunk->enemies[last_index];
        i--;
      }
    }
  }
}

void detect_island_invasion() {
  CHUNK *home_chunk = chunk_buffer + home_chunk_index;
  ISLAND *home_island = home_chunk->islands + HOME_ISLAND_INDEX;
  vec2 island_coords = { home_island->coords[X],
                         home_island->coords[Y] };
  vec2 island_coords_world = GLM_VEC2_ZERO_INIT;
  chunk_to_world(home_chunk->coords, island_coords, island_coords_world);

  unsigned int num_invading = 0;
  for (unsigned int i = 0; i < home_chunk->num_enemies; i++) {
    E_ENEMY *cur_enemy = home_chunk->enemies + i;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;
    chunk_to_world(home_chunk->coords, cur_enemy->coords, world_coords);
    if (world_coords[X] <= island_coords_world[X] + (I_WIDTH * T_WIDTH) &&
        world_coords[X] >= island_coords_world[X] &&
        world_coords[Y] <= island_coords_world[Y] &&
        world_coords[Y] >= island_coords_world[Y] - (I_WIDTH * T_WIDTH)) {
      num_invading++;
    }
  }

  update_invading_enemies(num_invading);
}

// ======================== COMBAT MODE COLLISIONS ===========================

/*
  Performs collision detection and resolution for combat units
  - coords are given as tile coordinates
*/
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

void attack_collision() {
  float hurt_radius = CHARACTER_HURT_RADIUS * T_WIDTH;

  vec2 player_coords = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  player_coords[Y] += T_WIDTH;

  vec2 player_attack_pos = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale_as(c_player.direction, T_WIDTH, player_attack_pos);
  glm_vec2_add(player_attack_pos, player_coords, player_attack_pos);

  // Because circle_aabb_collision utilizes the top left corner of the aabb,
  // we must calculate the top left corner of the hitbox, since
  // player_attack_pos currently contains the middle of the hitbox
  vec2 to_top_left = { -0.5 * T_WIDTH, 0.5 * T_WIDTH };
  glm_vec2_add(to_top_left, player_attack_pos, player_attack_pos);

  C_UNIT *unit = NULL;
  C_UNIT *target = NULL;
  vec2 unit_coords = GLM_VEC2_ZERO_INIT;
  vec2 target_coords = GLM_VEC2_ZERO_INIT;
  vec2 unit_attack_pos = GLM_VEC2_ZERO_INIT;
  // Check melee collisions
  for (unsigned int i = 0; i < num_npc_units; i++) {
    unit = npc_units + i;
    glm_vec2_scale(unit->coords, T_WIDTH, unit_coords);
    unit_coords[Y] += T_WIDTH;
    glm_vec2_scale_as(unit->direction, T_WIDTH, unit_attack_pos);
    glm_vec2_add(unit_attack_pos, unit_coords, unit_attack_pos);
    if (unit->type == ENEMY && unit->death_animation == -1.0) {
      // Check player collision with enemy hitbox
      if (c_player.invuln_timer == 0.0 && unit->attack_active &&
          circle_circle_collision(player_coords, hurt_radius, unit_attack_pos,
                                  T_WIDTH)) {
        c_player.health -= 10.0;
        c_player.invuln_timer = 0.5;
      }

      // Check enemy collision with player hitbox
      if (unit->invuln_timer == 0.0 && c_player.attack_active &&
          circle_circle_collision(unit_coords, hurt_radius, player_attack_pos,
                                  T_WIDTH)) {
        unit->life-= 5.0;
        unit->invuln_timer = 0.5;
        if (unit->life <= 0.0) {
          unit->death_animation = 1.0;
        }
        unit->knockback_counter = 0.15;
      }
    }

    if (unit->attack_active) {
      // Check collision with other npcs
      if (unit->death_animation == -1.0 && unit->attack_active) {
        for (unsigned int j = 0; j < num_npc_units; j++) {
          target = npc_units + j;
          glm_vec2_scale(target->coords, T_WIDTH, target_coords);
          target_coords[Y] += T_WIDTH;
          if (unit->type == target->type || target->death_animation != -1.0) {
            continue;
          }
          if (target->invuln_timer == 0.0 &&
              circle_circle_collision(target_coords, hurt_radius,
                                      unit_attack_pos, T_WIDTH)) {
            target->life-= 5.0;
            target->invuln_timer = 0.5;
            if (target->life <= 0.0) {
              target->death_animation = 1.0;
            }
            target->knockback_counter = 0.15;
          }
        }
      }
    }
  }

  PROJ *cur_proj = NULL;
  int to_despawn = 0;
  // Check ranged collisions
  for (unsigned int i = 0; i < num_projectiles; i++) {
    to_despawn = 0;
    cur_proj = projectiles + i;
    if (cur_proj->pos[X] <= -0.5 * arena_dimensions[X] * T_WIDTH ||
        cur_proj->pos[X] >=  0.5 * arena_dimensions[X] * T_WIDTH ||
        cur_proj->pos[Y] <= -0.5 * arena_dimensions[Y] * T_WIDTH ||
        cur_proj->pos[Y] >=  0.5 * arena_dimensions[Y] * T_WIDTH) {
      to_despawn = 1;
    }

    for (unsigned int j = 0; j < num_npc_units && !to_despawn; j++) {
      unit = npc_units + j;
      glm_vec2_scale(unit->coords, T_WIDTH, unit_coords);
      unit_coords[Y] += T_WIDTH;
      if (((cur_proj->target == ENEMY && unit->type == ENEMY) ||
           (cur_proj->target == ALLY && unit->type == ALLY)) &&
          unit->death_animation == -1.0 &&
          circle_circle_collision(unit_coords, hurt_radius, cur_proj->pos,
                                  PROJ_RAD * T_WIDTH)) {
        unit->life-= 5.0;
        unit->invuln_timer = 0.5;
        if (unit->life <= 0.0) {
          unit->death_animation = 1.0;
        }
        unit->knockback_counter = 0.15;
        to_despawn = 1;
      }
    }

    if (cur_proj->target == ALLY && c_player.invuln_timer == 0.0 &&
        !to_despawn && circle_circle_collision(player_coords, hurt_radius,
                                               cur_proj->pos,
                                               PROJ_RAD * T_WIDTH)) {
      c_player.health -= 10.0;
      c_player.invuln_timer = 0.5;
      to_despawn = 1;
    }

    if (to_despawn) {
      despawn_projectile(i);
      i--;
    }
  }
}

// ========================== COLLISION HELPERS ==============================

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
ISLAND *cur_island(CHUNK *chunk, vec2 world_coords) {
  /* Finds which island the the player is on by checking if they collide */
  for (int i = 0; i < chunk->num_islands; i++) {
    vec2 island_coords = { chunk->islands[i].coords[X],
                           chunk->islands[i].coords[Y] };
    vec2 island_coords_world = GLM_VEC2_ZERO_INIT;
    chunk_to_world(chunk->coords, island_coords, island_coords_world);
    if (world_coords[0] <= island_coords_world[0] + (I_WIDTH * T_WIDTH) &&
        world_coords[0] >= island_coords_world[0] &&
        world_coords[1] <= island_coords_world[1] &&
        world_coords[1] >= island_coords_world[1] - (I_WIDTH * T_WIDTH)) {
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

/*
  Helper function to retrieve the type of a tile given its coordinates
  Args:
  - unsinged int chunk: index of chunk in "chunk_buffer" to which the tile
                        belongs
  - vec2 coords: tile coordinates of the tile within the chunk
*/
int get_tile(unsigned int chunk, vec2 coords) {
  if (chunk >= chunk_buff_len) {
    return OCEAN;
 }
 CHUNK *target_chunk = chunk_buffer + chunk;
 vec2 world_coords = GLM_VEC2_ZERO_INIT;
 chunk_to_world(target_chunk->coords, coords, world_coords);
 ISLAND *island = cur_island(target_chunk, world_coords);
 if (island == NULL) {
   return OCEAN;
 }
 return check_tile(island, coords);
}
