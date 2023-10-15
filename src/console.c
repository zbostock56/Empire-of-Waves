#include <console.h>

void command_not_found() {
  printf("Command not found\nEOW-CONSOLE $ ");
  fflush(stdout);
}

/*
Teleports the player to the nearest island within the chunks that
are currently being rendered
*/
void teleport_nearest_island() {
#if 0
  if (mode != EXPLORATION) {
    printf("ERROR: Not in exploration mode!\nEOW-CONSOLE $ ");
    return;
  } else if (!e_player.embarked) {
    printf("ERROR: Not embarked on ship!\nEOW-CONSOLE $ ");
    return;
  }
  ISLAND_DIST dist[MAX_ISLANDS_SIM_CHUNKS];
  int tot_num_islands = 0;
  CHUNK chunk;
  /* Actual chunk coordinates relative to other chunks */
  ivec2 chunk_coords = { 0, 0 };
  /* Coordinates of the island relative to the chunk */
  /* (top left of chunk is 0, 0) */
  vec2 intra_chunk = { 0.0, 0.0 };
  ivec2 intra_chunk_int = { 0, 0 };
  /* Actual coordinates in relative to the origin of the world */
  vec2 world_coords = { 0.0, 0.0 };
  vec2 player_coords = { 0.0, 0.0};
  for (int i = 0; i < MAX_CHUNKS; i++) {
    chunk = player_chunks[i];
    for (int j = 0; j < chunk.num_islands; j++) {
      dist[tot_num_islands].island = chunk.islands[j];
      glm_ivec2_copy(dist[tot_num_islands].island.chunk, chunk_coords);
      glm_ivec2_copy(dist[tot_num_islands].island.coords, intra_chunk_int);
      intra_chunk[0] = (float) intra_chunk_int[0];
      intra_chunk[1] = (float) intra_chunk_int[1];
      /* Find the coordinates of the island in world space */
      chunk_to_world(chunk_coords, intra_chunk, world_coords);
      chunk_to_world(e_player.chunk, e_player.coords, player_coords);
      dist[tot_num_islands].distance = sqrt(
        ((player_coords[0] - world_coords[0]) * (player_coords[0] - world_coords[0]))
        +
        ((player_coords[1] - world_coords[1]) * (player_coords[1] - world_coords[1]))
      );
      tot_num_islands++;
    }
  }

  if (!tot_num_islands) {
    printf("ERROR: No islands in simulated chunks\nEOW-CONSOLE $ ");
    return;
  }

  /* Find which distance is the shortest and teleport */
  int shortest = 0;
  float shortest_distance = FLT_MAX;
  for (int i = 0; i < tot_num_islands; i++) {
     if (dist[i].distance < shortest_distance) {
        shortest_distance = dist[i].distance;
        shortest = i;
     }
  }
  vec2 island_coords_float = { 0.0, 0.0 };
  island_coords_float[0] = (float) dist[shortest].island.coords[0];
  island_coords_float[1] = (float) dist[shortest].island.coords[1];

  /* Move player and ship to new chunk */
  e_player.ship_chunk[0] = dist[shortest].island.chunk[0];
  e_player.ship_chunk[1] = dist[shortest].island.chunk[1];
  e_player.chunk[0] = dist[shortest].island.chunk[0];
  e_player.chunk[1] = dist[shortest].island.chunk[1];

  /* Move player and ship to new coordinates within chunk */
  //chunk_to_world(dist[shortest].island.chunk,
  //               island_coords_float, destination);
  e_player.ship_coords[0] = island_coords_float[0];
  e_player.ship_coords[1] = island_coords_float[1];
  e_player.coords[0] = island_coords_float[0];
  e_player.coords[1] = island_coords_float[1];
  printf("\nNEW CHUNK: %d | %d\nNEW WORLD COORDINATES %f | %f\nEOW-CONSOLE $ ",
          dist[shortest].island.chunk[0], dist[shortest].island.chunk[1],
          island_coords_float[0], island_coords_float[1]);
  #endif
}

void teleport(ivec2 pos) {
  vec2 coords = { pos[0], pos[1] };
  if (mode == EXPLORATION) {
    if (e_player.embarked) {
      world_to_chunk(coords, e_player.ship_chunk, e_player.ship_coords);
    } else {
      world_to_chunk(coords, e_player.chunk, e_player.coords);
    }
  } else {
    glm_vec2_copy(coords, c_player.coords);
  }
}

void set_speed(float speed) {
  if (mode == EXPLORATION) {
    e_player.speed = speed;
  } else if (mode == COMBAT) {
    c_player.speed = speed;
  }
}
