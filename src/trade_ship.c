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

void update_trade_ships() {
  for (unsigned int i = 0; i < num_trade_ships; i++) {
    trade_ship_pathfind(trade_ships + i);
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
  vec2 to_target = GLM_VEC2_ZERO_INIT;

  // Steer toward target island
  chunk_to_world(target_chunk->coords, target_world, target_world);
  glm_vec2_add(to_center_target, target_world, target_world);
  chunk_to_world(cur_chunk->coords, ship->coords, ship_world);
  glm_vec2_sub(target_world, ship_world, to_target);
  glm_vec2_normalize(to_target);
  glm_vec2_scale(to_target, delta_time, to_target);
  glm_vec2_add(to_target, ship->direction, ship->direction);


  // Steer away from non-target island tiles
  trade_ship_steering(ship, ship->direction);
  glm_vec2_normalize(ship->direction);

  vec2 movement = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(ship->direction, delta_time * ship->speed * T_WIDTH,
                 movement);
  glm_vec2_add(movement, ship_world, ship_world);
  world_to_chunk(ship_world, ship->chunk_coords, ship->coords);
}
