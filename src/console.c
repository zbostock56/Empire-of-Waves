#include <console.h>


/*
Command List:
-> give
-> teleport
-> spawn_enemy
-> spawn_trade_ship

Assumptions:
  -> cmd is delimited by null terminator ('\0')
*/
void handle_command(char *cmd) {
  char *buffer[MAX_TOKENS];
  int cmd_len = strlen(cmd);
  char *cur_tok = cmd;
  int num_tokens = 0;
  for (int i = 0; i < cmd_len && num_tokens < MAX_TOKENS; i++) {
    if (cmd[i] == ' ') {
      cmd[i] = '\0';
      buffer[num_tokens] = cur_tok;
      cur_tok = cmd + i + 1;
      num_tokens++;
    } else if (i == cmd_len - 1) {
      buffer[num_tokens] = cur_tok;
      num_tokens++;
    }
  }
  if (num_tokens >= 1) {
    if (strncmp(buffer[0], GIVE, strlen(GIVE) + 1) == 0) {
      printf("GIVE\n");
    } else if (strncmp(buffer[0], TELEPORT, strlen(TELEPORT) + 1) == 0 && num_tokens == 3) {
      ivec2 pos = { atoi(buffer[1]), atoi(buffer[2]) };
      teleport(pos);
    } else if (strncmp(buffer[0], SPAWN_ENEMY, strlen(SPAWN_ENEMY) + 1) == 0) {
      printf("SPAWN_ENEMY\n");
      //spawn_enemy();
    } else if (strncmp(buffer[0], SPAWN_TRADE_SHIP, strlen(SPAWN_TRADE_SHIP) + 1) == 0) {
      printf("SPAWN_TRADE_SHIP\n");
    } else if (strncmp(buffer[0], SET_SPEED, strlen(SET_SPEED) + 1) == 0 && num_tokens == 2) {
      float speed = atof(buffer[1]);
      set_speed(speed);
    } else if (strncmp(buffer[0], TELEPORT_NEAREST_ISLAND, strlen(TELEPORT_NEAREST_ISLAND) + 1) == 0
               && num_tokens == 1) {
      teleport_nearest_island();
    } else {
      printf("%s: command not found\nEOW CONSOLE $ ", cmd);
    }
    fflush(stdout);
  }
}

/*
Teleports the player to the nearest island within the chunks that
are currently being rendered
*/
void teleport_nearest_island() {
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
      dist[tot_num_islands].distance = sqrt(
        ((e_player.coords[0] - world_coords[0]) * (e_player.coords[0] - world_coords[0]))
        +
        ((e_player.coords[1] - world_coords[1]) * (e_player.coords[1] - world_coords[1]))
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
  vec2 destination = { 0.0, 0.0 };
  vec2 island_coords_float = { 0.0, 0.0 };
  island_coords_float[0] = (float) dist[shortest].island.coords[0];
  island_coords_float[1] = (float) dist[shortest].island.coords[1];

  /* Move player and ship to new chunk */
  e_player.ship_chunk[0] = dist[shortest].island.chunk[0];
  e_player.ship_chunk[1] = dist[shortest].island.chunk[1];
  e_player.chunk[0] = dist[shortest].island.chunk[0];
  e_player.chunk[1] = dist[shortest].island.chunk[1];

  /* Move player and ship to new coordinates within chunk */
  chunk_to_world(dist[shortest].island.chunk,
                 island_coords_float, destination);
  e_player.ship_coords[0] = destination[0];
  e_player.ship_coords[1] = destination[1];
  e_player.coords[0] = destination[0];
  e_player.coords[1] = destination[1];
  printf("\nNEW CHUNK: %d | %d\nNEW WORLD COORDINATES %f | %f\nEOW-CONSOLE $ ",
          dist[shortest].island.chunk[0], dist[shortest].island.chunk[1],
          destination[0], destination[1]);
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
