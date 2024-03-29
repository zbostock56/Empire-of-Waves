#include <en.h>
/*
                                    ENEMY.c
Implements the functionality for enemy pathfinding/behavior in combat mode and
exploration mode, and helper functions for common enemy operations such as
respawn, despawn, move, attack, and rotate.
*/

/*

TESTING COPY, CHANGED WITH OUPUTS AND OTHER CHECKS

*/

/* Reponsible for spawning enemy in a chunk that is not */
/* at enemy cap nor is outside of the simulation range */
void spawn_enemy() {
  /*
    Check the number of enemies in the 3 x 3 chunks
  */
  int number_enemies = 0;
  for (int i = 0; i < 9; i++) {
    number_enemies += player_chunks[i].num_enemies;
    printf("Number Enemies chunk %d | %d\n", i, player_chunks[i].num_enemies);
  }
  printf("NUMBER OF ENEMIES: %d\n", number_enemies);
  int avail_chunks[CHUNKS_SIMULATED];
  int num_avail = find_avail_chunks(avail_chunks);
  /* Choose a chunk to spawn an enemy in */
  if (num_avail == 0) {
    printf("NO CHUNKS AVAILABLE\n");
    check_enemy_positions();
    /* No chunks available */
    return;
  }
  int not_found = 1;
  int chunk_pos = 0;
  ivec2 chosen_chunk = { 0, 0 };
  while (not_found) {
    chunk_pos = rand() % CHUNKS_SIMULATED;
    if (avail_chunks[chunk_pos]) {
      not_found = 0;
    }
  }
  /* chunk_pos is now set to the chosen chunk */
  /* Macro defined in enemy.h */
  convert_chunk_coords(chunk_pos, chosen_chunk,
                       e_player.chunk);
  /* Determine where to put the enemy within the chunk */
  int in_player_chunk = chunk_pos == CURRENT_CHUNK ? 1 : 0;
  int posx = 0;
  int posy = 0;
  not_found = 1;
  /* GCC trick to initialize array with all zeros on the stack */
  int chunk_tiles[C_WIDTH][C_WIDTH] = {
    [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = 1 }
  };
  /* Set up chunk tileset */
  generate_chunk_tiles(chunk_tiles, player_chunks[chunk_pos]);
  while (not_found) {
    posx = rand() % C_WIDTH;
    posy = rand() % C_WIDTH;
    TILE tile = chunk_tiles[posx][posy];
    if ((in_player_chunk && e_player.coords[0] != posx && e_player.coords[1] != posy)
         || !in_player_chunk) {
      if (tile == 1) {
        /* Sets up base enemy */
        unsigned int num_e = player_chunks[chunk_pos].num_enemies;
        E_ENEMY *enemy = &player_chunks[chunk_pos].enemies[num_e];
        enemy->chunk[0] = chosen_chunk[0];
        enemy->chunk[1] = chosen_chunk[1];
        enemy->coords[0] = posx;
        enemy->coords[1] = posy;
        enemy->direction[0] = 0;
        enemy->direction[1] = 1;
        enemy->speed = 1.0;
        enemy->crew_count = 1;
        player_chunks[chunk_pos].num_enemies++;
        not_found = 0;
        printf("ENEMY SPAWNED:\nchunk: %d | %d\npos: %f | %f\n", enemy->chunk[0], enemy->chunk[1], enemy->coords[0], enemy->coords[1]);
        /* TODO: Account for resize enemy buffer */
      }
    }
  }
}

/* Generates a 2D Array of the tiles in the passed chunk that are  */
/* ocean and are not. */
void generate_chunk_tiles(int chunk_tiles[C_WIDTH][C_WIDTH], CHUNK chunk) {
  ISLAND island;
  int chunkx = 0;
  int chunky = 0;
  for (int i = 0; i < chunk.num_islands; i++) {
    island = chunk.islands[i];
    int xoffset = island.coords[0];
    int yoffset = island.coords[1];
    for (int j = 0; j < I_WIDTH * I_WIDTH; j++) {
      chunkx = xoffset + (j % I_WIDTH);
      chunky = yoffset + (j / I_WIDTH);
      if (island.tiles[j] != OCEAN) {
        chunk_tiles[chunkx][chunky] = 0;
      }
    }
  }
}

/* Determine which chunks around the player have the */
/* ability to spawn an enemy */
/*
Simulation Radius Diagram:

|-----------------------|
|       |       |       |
|   0   |   1   |   2   |
|-----------------------|
|       |Curr   |       |
|   3   | chunk |   5   |
|-----------------------|
|       |       |       |
|   6   |   7   |   8   |
|-----------------------|

With the current simulation strategy,
the chunks will be rendered in blocks of
3 x 3. The above numbers represent the
way that the chunks will be checked in,
and the way that there result will be stored
in the input array.

Returns: Number of chunks which can have enemies spawned
in them
*/
int find_avail_chunks(int avail_chunks[CHUNKS_SIMULATED]) {
  int num_avail = 0;
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    if (player_chunks[i].num_enemies < MAX_ENEMIES) {
      avail_chunks[i] = 1;
      num_avail++;
    } else {
      avail_chunks[i] = 0;
    }
  }
  return num_avail;
}


/* TEST CASE */
/* Proves that all enemies are being spawned on ocean tiles  */
void check_enemy_positions() {
  /* GCC trick to initialize array with all zeros on the stack */
  CHUNK chunk;
  for (int i = 0; i < 9; i++) {
    int chunk_tiles[C_WIDTH][C_WIDTH] = {
      [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = 1 }
    };
    chunk = player_chunks[i];
    generate_chunk_tiles(chunk_tiles, chunk);
    for (int j = 0; j < 3; j++) {
      int posx = chunk.enemies[j].coords[0];
      int posy = chunk.enemies[j].coords[1];
      if (chunk_tiles[posx][posy] == 0) {
        printf("FAILURE\n");
        printf("Chunk info:\nx: %d\ny: %d\nposx: %d\nposy: %d\n",
                chunk.coords[0], chunk.coords[1], posx, posy);
        exit(1);
      }
    }
  }
  printf("Tests Passed\n");
  exit(0);
}


void c_enemy_pathfind(C_UNIT *enemy, vec2 target_coords) {
  float target_dist = glm_vec2_distance(target_coords, enemy->coords);
  if (target_dist <= 1.5 && enemy->attack_cooldown == 0.0) {
    enemy->attack_cooldown = enemy->fire_rate;
    enemy->attack_active = 0.1;
  }

  vec2 movement = GLM_VEC2_ZERO_INIT;
  int move = 0;
  if (target_dist >= 1.5) {
    glm_vec2_sub(target_coords, enemy->coords, enemy->direction);
    glm_vec2_normalize(enemy->direction);
    move = 1;
  }

  // Distance from enemy to its allies
  float ally_dist = 0.0;
  vec2 steer_direction = GLM_VEC2_ZERO_INIT;
  // Move enemy away from allies so they don't cluster together
  for (unsigned int i = 0; i < num_npc_units; i++) {
    if (npc_units + i != enemy) {
      ally_dist = glm_vec2_distance(enemy->coords, npc_units[i].coords);
      if (ally_dist <= 2.0) {
        glm_vec2_sub(enemy->coords, npc_units[i].coords, steer_direction);
        glm_vec2_normalize(steer_direction);
        glm_vec2_scale(steer_direction, 0.5, steer_direction);
        glm_vec2_add(steer_direction, enemy->direction, enemy->direction);
        glm_vec2_normalize(enemy->direction);

        move = 1;
      }
    }
  }

  if (move) {
    glm_vec2_scale(enemy->direction, (delta_time * enemy->speed) / T_WIDTH,
                   movement);
    glm_vec2_add(movement, enemy->coords, enemy->coords);
  }
}
