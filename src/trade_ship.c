#include <trade_ship.h>
/*
                                  TRADE_SHIP.c
Implements the functionality for trade ship pathfinding, and helper functions
for common trade ship operations such as respawn, despawn, move, and rotate.
*/

int init_trade_ship_buffers() {
  trade_ships = malloc(sizeof(TRADE_SHIP) * TRADE_SHIP_BUF_START_LEN);
  if (trade_ships == NULL) {
    free(trade_ships);
    fprintf(stderr, "trade_ship.c: unabled to allocate trade ship buffers\n");
    return -1;
  }

  trade_ship_buf_size = TRADE_SHIP_BUF_START_LEN;
  num_trade_ships = 0;

  return 0;
}

void free_trade_ship_buffers() {
  free(trade_ships);
  num_trade_ships = 0;
  trade_ship_buf_size = 0;
}

/*
  Initializes a trade ship located at the home island, targeting an island
  at a given chunk
  Returns:
  A pointer to the newly created trade ship
*/
TRADE_SHIP *init_trade_ship(char *merch_name, ivec2 target_chunk,
                            unsigned int target_island) {
  TRADE_SHIP *trade_ship = trade_ships + num_trade_ships;

  trade_ship->target_chunk_index = add_chunk(target_chunk);
  ivec2 cur_chunk_coords = { 0, 0 };
  trade_ship->cur_chunk_index = add_chunk(cur_chunk_coords);

  glm_vec2_copy(home_island_coords, trade_ship->coords);
  glm_ivec2_copy(cur_chunk_coords, trade_ship->chunk_coords);
  glm_vec2_zero(trade_ships[num_trade_ships].direction);
  trade_ships[num_trade_ships].direction[0] = 1.0;
  trade_ships[num_trade_ships].export_rec = 0;
  trade_ships[num_trade_ships].import_rec = 0;
  trade_ships[num_trade_ships].target_island = target_island;
  trade_ships[num_trade_ships].num_mercenaries = 0;
  trade_ships[num_trade_ships].speed = 10.0;
  trade_ships[num_trade_ships].death_animation = -1.0;
  trade_ships[num_trade_ships].moving = 0;
  for (int i = 0; i < 20; i++) {
    trade_ships[num_trade_ships].desc[i] = merch_name[i];
  }
  num_trade_ships++;

  if (num_trade_ships == trade_ship_buf_size) {
    double_buffer((void **) &trade_ships, &trade_ship_buf_size,
                  sizeof(TRADE_SHIP));
  }

  return trade_ships + num_trade_ships - 1;
}

void delete_trade_ship(ivec2 target_chunk, unsigned int target_island) {
  TRADE_SHIP *cur_ship = NULL;
  CHUNK *cur_chunk = NULL;
  for (int i = 0; i < num_trade_ships; i++) {
    cur_ship = trade_ships + i;
    cur_chunk = chunk_buffer + cur_ship->target_chunk_index;
    if (target_chunk[0] == cur_chunk->coords[0] &&
        target_chunk[1] == cur_chunk->coords[1] &&
        target_island == cur_ship->target_island) {
      num_trade_ships--;
      trade_ships[i] = trade_ships[num_trade_ships];
      return;
    }
  }
}

void restore_trade_ship_mercs(ivec2 target_chunk, unsigned int target_island) {
  TRADE_SHIP *cur_ship = NULL;
  CHUNK *cur_chunk = NULL;
  for (int i = 0; i < num_trade_ships; i++) {
    cur_ship = trade_ships + i;
    cur_chunk = chunk_buffer + cur_ship->target_chunk_index;
    if (target_chunk[0] == cur_chunk->coords[0] &&
        target_chunk[1] == cur_chunk->coords[1] &&
        target_island == cur_ship->target_island) {
      e_player.total_mercenaries += cur_ship->num_mercenaries;
      cur_ship->num_mercenaries = 0;
      return;
    }
  }
}

void update_trade_ships() {
  if (mode != EXPLORATION) {
    return;
  }

  TRADE_SHIP *cur_ship = NULL;
  for (unsigned int i = 0; i < num_trade_ships; i++) {
    cur_ship = trade_ships + i;
    if (cur_ship->death_animation == -1.0) {
      trade_ship_pathfind(trade_ships + i);
    } else if (cur_ship->death_animation == 0.0) {
      CHUNK *target_chunk = chunk_buffer + cur_ship->target_chunk_index;
      ISLAND *target_island = target_chunk->islands +
                              cur_ship->target_island;
      target_island->merchant.has_trade_route = 0;
      num_trade_ships--;
      trade_ships[i] = trade_ships[num_trade_ships];

      // Spawn prompt to show that trade ship was plundered
      prompt_plundered_trade_ship();
      update_relationship(&target_island->merchant, -10.0);
      i--;
    }
    /*
    vec2 c = GLM_VEC2_ZERO_INIT;
    chunk_to_world(trade_ships[i].chunk_coords, trade_ships[i].coords, c);
    printf("TRADE SHIP: %d with chunk (%d, %d), and world coords (%f, %f)\n",
            i, trade_ships[i].chunk_coords[0], trade_ships[i].chunk_coords[1],
            c[0], c[1]);
    fflush(stdout);
    */
  }
}

void trade_ship_pathfind(TRADE_SHIP *ship) {
  CHUNK *target_chunk = chunk_buffer + ship->target_chunk_index;
  CHUNK *cur_chunk = chunk_buffer + ship->cur_chunk_index;
  ISLAND *target_island = target_chunk->islands + ship->target_island;
  vec2 target_world = { target_island->coords[0], target_island->coords[1] };
  vec2 to_center_target = {
    T_WIDTH * I_WIDTH * 0.5,
    -T_WIDTH * I_WIDTH * 0.5
  };
  vec2 ship_world = GLM_VEC2_ZERO_INIT;

  // Steer toward target island
  vec2 to_target = GLM_VEC2_ZERO_INIT;
  chunk_to_world(target_chunk->coords, target_world, target_world);
  glm_vec2_add(to_center_target, target_world, target_world);
  chunk_to_world(cur_chunk->coords, ship->coords, ship_world);
  glm_vec2_sub(target_world, ship_world, to_target);
  glm_vec2_normalize(to_target);

  // Steer away from non-target island tiles
  vec2 from_obstacles = GLM_VEC2_ZERO_INIT;
  ship_steering(ship->chunk_coords, ship->coords, from_obstacles,
                cur_chunk, target_chunk, ship->target_island);

  // Steer away from enemy ships
  vec2 from_enemies = GLM_VEC2_ZERO_INIT;
  trade_ship_detect_enemies(ship, from_enemies, cur_chunk);

  // Combine steering calculations
  glm_vec2_add(to_target, from_enemies, to_target);
  glm_vec2_add(to_target, from_obstacles, to_target);
  glm_vec2_normalize(to_target);

  // Linearly interpolate current ship direction with target direction
  vec2 smoothed_direction = GLM_VEC2_ZERO_INIT;
  glm_vec2_lerp(ship->direction, to_target, 0.1, smoothed_direction);
  if (smoothed_direction[0] == 0) {
    smoothed_direction[0] = 0.05;
  }
  if (smoothed_direction[1] == 0) {
    smoothed_direction[1] = 0.05;
  }
  glm_vec2_normalize(smoothed_direction);
  glm_vec2_copy(smoothed_direction, ship->direction);

  // Update ship position
  vec2 movement = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(ship->direction, delta_time * ship->speed * T_WIDTH,
                 movement);
  glm_vec2_add(movement, ship_world, ship_world);
  world_to_chunk(ship_world, ship->chunk_coords, ship->coords);
  ship->moving = 1;
}

int trade_ship_active(unsigned int index) {
  return trade_ships[index].death_animation == -1.0;
}
