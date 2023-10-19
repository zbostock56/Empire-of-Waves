#include <enemy.h>
/*
                                    ENEMY.c
Implements the functionality for enemy pathfinding/behavior in combat mode and
exploration mode, and helper functions for common enemy operations such as
respawn, despawn, move, attack, and rotate.
*/

/* Reponsible for spawning enemy in a chunk that is not */
/* at enemy cap nor is outside of the simulation range */

void spawn_enemy() {
  int avail_chunks[CHUNKS_SIMULATED];
  int num_avail = find_avail_chunks(avail_chunks);
  /* Choose a chunk to spawn an enemy in */
  if (num_avail == 0) {
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
  glm_ivec2_add(CHUNK_OFFSETS[chunk_pos], e_player.ship_chunk, chosen_chunk);
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
  CHUNK *chunk = chunk_buffer + player_chunks[chunk_pos];
  generate_chunk_tiles(chunk_tiles, *chunk);
  while (not_found) {
    posx = rand() % C_WIDTH;
    posy = rand() % C_WIDTH;
    TILE tile = chunk_tiles[posx][posy];
    if ((in_player_chunk && e_player.ship_coords[0] != posx &&
         e_player.ship_coords[1] != posy) || !in_player_chunk) {
      if (tile == 1) {
        /* Sets up base enemy */
        unsigned int insert_index = chunk->num_enemies;
        chunk->num_enemies++;

        int status = 0;
        if (chunk->num_enemies == chunk->enemy_buf_size) {
          status = double_buffer((void **) &chunk->enemies,
                                 &chunk->enemy_buf_size,
                                 sizeof(E_ENEMY));
        }

        if (status == 0) {
          E_ENEMY *enemy = &chunk->enemies[insert_index];
          enemy->chunk[0] = chosen_chunk[0];
          enemy->chunk[1] = chosen_chunk[1];
          enemy->coords[0] = posx;
          enemy->coords[1] = posy;
          enemy->direction[0] = 0;
          enemy->direction[1] = 1;
          enemy->speed = 10.0;
          enemy->crew_count = 3;
          not_found = 0;
        }
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
      if (island.tiles[j] != OCEAN && island.tiles[j] != SHORE) {
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
  CHUNK *cur_chunk = NULL;
  for (int i = 0; i < CHUNKS_SIMULATED; i++) {
    cur_chunk = chunk_buffer + player_chunks[i];
    if (cur_chunk->num_enemies < MAX_ENEMIES) {
      avail_chunks[i] = 1;
      num_avail++;
    } else {
      avail_chunks[i] = 0;
    }
  }
  return num_avail;
}

/*
    Updates enemy position based on its direction and speed
*/
void update_enemy_position(E_ENEMY *enemy) {
  vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
  vec2 movement = GLM_VEC2_ZERO_INIT;
  chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
  glm_vec2_scale(enemy->direction, delta_time * enemy->speed * T_WIDTH, movement);
  glm_vec2_add(movement, enemy_world_coords, enemy_world_coords);
  world_to_chunk(enemy_world_coords, enemy->chunk, enemy->coords);
}

void pathfind_enemy(E_ENEMY *enemy) {
  /* If Enemy ship is not in player's current chunk, manually move it to our chunk*/
  if (e_player.ship_chunk[0] != enemy->chunk[0] || e_player.ship_chunk[1] != enemy->chunk[1]) {
    vec2 target_dir = GLM_VEC2_ZERO_INIT;
    /* If the enemy is in the left chunk of the player. */
    if (enemy->chunk[0] == e_player.ship_chunk[0] - 1 && enemy->chunk[1] == e_player.ship_chunk[1]) {
      target_dir[0] = 1.0;
    }
    /* If the enemy is in the upper chunk of the player*/
    else if (enemy->chunk[0] == e_player.ship_chunk[0] && enemy->chunk[1] == e_player.ship_chunk[1] + 1) {
      target_dir[1] = -1.0;
    }
    else if (enemy->chunk[0] == e_player.ship_chunk[0] + 1 && enemy->chunk[1] == e_player.ship_chunk[1]) {
      target_dir[0] = -1.0;
    }
    else if (enemy->chunk[0] == e_player.ship_chunk[0] && enemy->chunk[1] == e_player.ship_chunk[1] - 1) {
      target_dir[1] = 1.0;
    } else {
      /*If the enemy on diagonal*/
    }
    // update_enemy_position(enemy);

    glm_vec2_scale(target_dir, delta_time*3, target_dir);
    glm_vec2_add(target_dir, enemy->direction, enemy->direction);
    if (enemy->direction[0] == 0) {
        enemy->direction[0] = 0.05;
      }
    if (enemy->direction[1] == 0) {
      enemy->direction[1] = 0.05;
    }
    glm_vec2_normalize(enemy->direction);
    
    update_enemy_position(enemy);
    enemy->on_path = false;
    return;
  }

  /* If enemy ship is on path to the goal col, do the search */
  if (enemy->on_path) {
    int goal_col = (int)(e_player.ship_coords[0]);
    int goal_row = (int)(e_player.ship_coords[1]);
    // int goal_col = 110;
    // int goal_row = 80;
    int start_col = (int)(enemy->coords[0]);
    int start_row = (int)(enemy->coords[1]);
    vector *path_list = (vector *)malloc(sizeof(vector));
    if (search(start_col, start_row, goal_col, goal_row, enemy, path_list)) {

      Node *n = ((Node *)vector_get(path_list, vector_total(path_list) - 1));
      int next_x = n->col;
      int next_y = n->row;

      // printf("next_x : %d next_y : %d\n\n", next_x, next_y);
      vec2 next_tile = {next_x, next_y};
      vec2 difference = GLM_VEC2_ZERO_INIT;
      vec2 enemy_coords = {(int)enemy->coords[0], (int)enemy->coords[1]};
      glm_vec2_sub(next_tile, enemy_coords, difference);
      glm_vec2_normalize(difference);
      difference[1] *= -1;
      glm_vec2_scale(difference, delta_time*3, difference);
      glm_vec2_add(difference, enemy->direction, enemy->direction);
      
      if (enemy->direction[0] == 0) {
        enemy->direction[0] = 0.05;
      }
      if (enemy->direction[1] == 0) {
        enemy->direction[1] = 0.05;
      }      
      glm_vec2_normalize(enemy->direction);
      
      int next_col = ((Node *)vector_get(path_list, vector_total(path_list) - 1))->col;
      int next_row = ((Node *)vector_get(path_list, vector_total(path_list) - 1))->row;

      if (next_col == goal_col && next_row == goal_row) {
        enemy->on_path = false;
      }
      vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
      vec2 movement = GLM_VEC2_ZERO_INIT;
      chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
      glm_vec2_scale(enemy->direction, delta_time * enemy->speed * T_WIDTH, movement);
      glm_vec2_add(movement, enemy_world_coords, enemy_world_coords);
      world_to_chunk(enemy_world_coords, enemy->chunk, enemy->coords);
    }
    free(path_list);
  }
}

/*
    A* search algorithm
*/
int search(int start_col, int start_row, int goal_col, int goal_row, E_ENEMY *enemy, vector *path_list) {
  if (start_col >= C_WIDTH || start_col < 0 ||
      start_row >= C_WIDTH || start_row < 0 ||
      goal_col >= C_WIDTH || goal_col < 0 ||
      goal_row >= C_WIDTH || goal_row < 0) {

    return 0;
  }

  /*
      Setting the nodes and arrays
      tiles = returns 1 if the tile is an ocean in the given tile index
      open = returns 1 if the tile is open in the given tile index
      checked = returns 1 if the tile was checked before in the given tile index
  */
  int tiles[C_WIDTH][C_WIDTH] = {
      [0 ... C_WIDTH - 1] = {[0 ... C_WIDTH - 1] = 1}};
  int open[C_WIDTH][C_WIDTH] = {
      [0 ... C_WIDTH - 1] = {[0 ... C_WIDTH - 1] = 0}};
  int checked[C_WIDTH][C_WIDTH] = {
      [0 ... C_WIDTH - 1] = {[0 ... C_WIDTH - 1] = 0}};
  Node nodes[C_WIDTH][C_WIDTH];
  //Node (*nodes)[C_WIDTH] = malloc(sizeof(Node) * C_WIDTH * C_WIDTH);

  /* getting the chunk of the enemy and creating a tilemap to check which objects are able/unable to pass through*/
  ivec2 chunk_offset = GLM_VEC2_ZERO_INIT;
  glm_ivec2_sub(enemy->chunk, e_player.ship_chunk, chunk_offset);
  CHUNK *enemy_chunk;
  int chunk_index = (chunk_offset[0] + 1) + 3 * (1 - chunk_offset[1]);
  enemy_chunk = chunk_buffer + player_chunks[chunk_index];
  int in_player_chunk = chunk_index == CURRENT_CHUNK ? 1 : 0;
  if (!in_player_chunk) {
    printf("Not in player chunk\n\n");
    enemy->on_path = false;
    return 0;
  }
  generate_chunk_tiles(tiles, *enemy_chunk);

  // Node goal_node = nodes[goal_col][goal_row];

  /* Setting up the costs needed for A * algorithm on each node of the tilemap */
  for (int i = 0; i < C_WIDTH; i++) {
    for (int j = 0; j < C_WIDTH; j++) {
      get_cost(&nodes[i][j], i, j, start_col, start_row, goal_col, goal_row);
    }
  }

  Node start_node = nodes[start_col][start_row];
  Node cur_node = start_node;
  vector openList;
  vector_init(&openList);
  vector_add(&openList, (void *)&cur_node);
  vector_init(path_list);
  int goal_reached = 0;
  int step = 0;
  int best_node_idx = 0;
  while (!goal_reached && step < 600) {
    int col = cur_node.col;
    int row = cur_node.row;

    /* check the current node and remove it from the openList */
    checked[col][row] = 1;
    vector_delete(&openList, best_node_idx);

    /* Open the node UP if it's possible, and add to openList */
    if (row - 1 >= 0) {
      open_node(&nodes[col][row - 1], &cur_node, open, checked, tiles, &openList);
    }

    /* Open the node LEFT if it's possible, and add to openList */
    if (col - 1 >= 0) {
      open_node(&nodes[col - 1][row], &cur_node, open, checked, tiles, &openList);
    }

    /* Open the node DOWN if it's possible, and add to openList */
    if (row + 1 < C_WIDTH) {
      open_node(&nodes[col][row + 1], &cur_node, open, checked, tiles, &openList);
    }

    /* Open the node RIGHT if it's possible, and add to openList */
    if (col + 1 < C_WIDTH) {
      open_node(&nodes[col + 1][row], &cur_node, open, checked, tiles, &openList);
    }

    /* For 8-way feature */
    /* OPEN the RIGHT UP if it it's possible, and add to openList*/
    if (col + 1 < C_WIDTH && row - 1 >= 0) {
      open_node(&nodes[col + 1][row - 1], &cur_node, open, checked, tiles, &openList);
    }
    /* Open the right down if it's possible*/
    if (col + 1 < C_WIDTH && row + 1 < C_WIDTH) {
      open_node(&nodes[col + 1][row], &cur_node, open, checked, tiles, &openList);
    }
    /* open the LEFT UP if it's possible*/
    if (col - 1 >= 0 && row - 1 >= 0) {
      open_node(&nodes[col - 1][row - 1], &cur_node, open, checked, tiles, &openList);
    }
    /* open the LEFT down if it's possible*/
    if (col - 1 >= 0 && row + 1 < C_WIDTH) {
      open_node(&nodes[col - 1][row + 1], &cur_node, open, checked, tiles, &openList);
    }

    best_node_idx = 0;
    int best_node_fCost = 1000;
    for (int i = 0; i < vector_total(&openList); i++) {

      /* Check if the openList's node we checking has a smaller F cost than the previous best and update */
      if (((Node *)vector_get(&openList, i))->f_cost < best_node_fCost) {
        best_node_idx = i;
        best_node_fCost = ((Node *)vector_get(&openList, i))->f_cost;
      }
      /* if F cost is equal, check the G cost and update */
      else if (((Node *)vector_get(&openList, i))->f_cost == best_node_fCost) {
        if (((Node *)vector_get(&openList, i))->g_cost < ((Node *)vector_get(&openList, best_node_idx))->g_cost) {
          best_node_idx = i;
        }
      }
    }

    /* If no more node in the openList, end the loop */
    if (vector_total(&openList) == 0) {
      break;
    }

    cur_node = *(Node *)vector_get(&openList, best_node_idx);

    /* If we reached to the goal node, we start tracking the path */
    if (cur_node.col == goal_col && cur_node.row == goal_row) {
      goal_reached = 1;
      track_path(nodes, path_list, start_col, start_row, goal_col, goal_row);
    }
    step++;
  }
  vector_free(&openList);
  //vector_free(path_list);
  return goal_reached;
}

/*
    function for tracking the best path we got from A * search alg. by backtracking
*/
void track_path(Node nodes[C_WIDTH][C_WIDTH], vector *path_list, int start_col, int start_row, int goal_col, int goal_row) {
  Node *current = &nodes[goal_col][goal_row];
  while (current->col != start_col || current->row != start_row) {
    // printf("X %d Y %d\n",current->col, current->row);
    vector_add(path_list, (void *)current);
    current = &nodes[current->parent_col][current->parent_row];
  }
}

/*
    function for getting the cost needed for A * algorithm of the given node
*/
void get_cost(Node *node, int col, int row, int start_col, int start_row, int goal_col, int goal_row) {
  // G cost
  int x_distance = abs(col - start_col);
  int y_distance = abs(row - start_row);
  node->g_cost = x_distance + y_distance;
  // H cost
  x_distance = abs(col - goal_col);
  y_distance = abs(row - goal_row);
  node->h_cost = x_distance + y_distance;
  // F cost
  node->f_cost = node->g_cost + node->h_cost;
  node->row = row;
  node->col = col;
}

/*
    Checks if we can open the given node
    If so, add to the openList
    tiles = 1 mean it is ocean
*/
void open_node(Node *node, Node *cur_node, int open[C_WIDTH][C_WIDTH], int checked[C_WIDTH][C_WIDTH], int tiles[C_WIDTH][C_WIDTH], vector *openList) {
  if (open[node->col][node->row] == 0 && checked[node->col][node->row] == 0 && tiles[node->col][node->row] == 1) {
    open[node->col][node->row] = 1;
    node->parent_col = cur_node->col;
    node->parent_row = cur_node->row;
    vector_add(openList, (void *)node);
    // printf("Opened node %d %d\n", node->col,node->row);
  }
}

/*
  Updates the CHUNK's num_enemies and enemies buffer when changes are made
*/
void update_enemy_chunk(E_ENEMY *cur_enemy, CHUNK *chunk, int i) {
  if (cur_enemy->chunk[0] != chunk->coords[0] || cur_enemy->chunk[1] != chunk->coords[1]) {
    // printf("Enemy chunk changed\n");
    ivec2 chunk_offset = GLM_VEC2_ZERO_INIT;
    glm_ivec2_sub(cur_enemy->chunk, e_player.ship_chunk, chunk_offset);
    CHUNK *new_enemy_chunk;
    int chunk_index = (chunk_offset[0] + 1) + 3 * (1 - chunk_offset[1]);
    new_enemy_chunk = chunk_buffer + player_chunks[chunk_index];
    memcpy(&new_enemy_chunk->enemies[new_enemy_chunk->num_enemies], cur_enemy, sizeof(E_ENEMY));
    new_enemy_chunk->num_enemies++;

    chunk->num_enemies--;
    // Swap last enemy in buffer to replace deleted enemy
    chunk->enemies[i] = chunk->enemies[chunk->num_enemies];

    /* Buffer adjustments if needed*/
    int status = 0;
    if (new_enemy_chunk->num_enemies == new_enemy_chunk->enemy_buf_size) {
      // printf("Buffer adjustments, num_enemies : %d\n\n", new_enemy_chunk->num_enemies);
      status = double_buffer((void **)&new_enemy_chunk->enemies,
                             &new_enemy_chunk->enemy_buf_size,
                             sizeof(E_ENEMY));
    }
    if (status == 0) {
      // printf("Old chunk's num_enemies : %d\n", chunk->num_enemies);
    }
  }
}

// ============================= COMBAT MODE =================================

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
