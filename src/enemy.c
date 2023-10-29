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
          enemy->speed = 15.0;
          //randomize (1-5)
          enemy->crew_count = rand()%5+1;
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
  glm_vec2_scale(enemy->direction, delta_time * enemy->speed * T_WIDTH,
                 movement);
  glm_vec2_add(movement, enemy_world_coords, enemy_world_coords);
  world_to_chunk(enemy_world_coords, enemy->chunk, enemy->coords);
}

/*
  Searches the given enemy's boundary and selects the target, pathfind towards it.
  Currently, I will make enemy ships to halt to follow the trade ship if it's in a diff chunk
*/
void pathfind_enemy(E_ENEMY *enemy, unsigned int enemy_chunk) {
  vec2 player_ship_world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                 player_ship_world_coords);
  vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
  float min_distance = FLT_MAX;
  float distance = 0.0;
  bool prioritize_player = false;
  TRADE_SHIP *target_tradeship = NULL;
  int goal_col;
  int goal_row;
  // Adjust this value to control the smoothness of the transition.
  float lerp_factor = 0.1;
  /*
    the enemy finds all possible targets in range and
    sets the closet ship (either tradeship or player_ship) as the target
  */
  float ship_search_radius = SHIP_COLLISION_RADIUS * SHIP_CHASE_RADIUS *
                             T_WIDTH;
  if (circle_circle_collision(player_ship_world_coords, ship_search_radius,
                              enemy_world_coords, ship_search_radius)) {
    /* player ship Detected by enemy */
    enemy->on_path = true;
    min_distance = glm_vec2_distance(player_ship_world_coords,
                                     enemy_world_coords);
    prioritize_player = true;
  }
  for (int i = 0; i < num_trade_ships; i++) {
    vec2 trade_world_coords = GLM_VEC2_ZERO_INIT;
    chunk_to_world((trade_ships+i)->chunk_coords, (trade_ships+i)->coords,
                   trade_world_coords);
    if (circle_circle_collision(trade_world_coords, ship_search_radius,
                                enemy_world_coords, ship_search_radius)) {
      /* trade ship detected by enemy */
      enemy->on_path = true;
      distance = glm_vec2_distance(enemy_world_coords, trade_world_coords);
      if (distance < min_distance) {
        min_distance = distance;
        prioritize_player = false;
        target_tradeship = trade_ships+i;
      }
    }
  }

  /* Return if there was no target found */
  if (min_distance == FLT_MAX) {
    enemy->on_path = false;
    return;
  }

  /* if target is player_ship */
  if (prioritize_player) {
     /* if player is not on board, don't chase */
    if (!e_player.embarked) {
      enemy->on_path = false;
      return;
    }

    /* If Enemy ship is not in player's current chunk, manually move it to our
       chunk instead of targetng */
    if (e_player.ship_chunk[0] != enemy->chunk[0] ||
        e_player.ship_chunk[1] != enemy->chunk[1]) {
      vec2 target_dir = GLM_VEC2_ZERO_INIT;
      if (enemy->chunk[0] == e_player.ship_chunk[0] - 1 &&
          enemy->chunk[1] == e_player.ship_chunk[1]) {
        /* If the enemy is in the left chunk of the player. */
        target_dir[0] = 1.0;
      } else if (enemy->chunk[0] == e_player.ship_chunk[0] &&
                 enemy->chunk[1] == e_player.ship_chunk[1] + 1) {
        /* If the enemy is in the upper chunk of the player*/
        target_dir[1] = -1.0;
      } else if (enemy->chunk[0] == e_player.ship_chunk[0] + 1 &&
                 enemy->chunk[1] == e_player.ship_chunk[1]) {
        target_dir[0] = -1.0;
      } else if (enemy->chunk[0] == e_player.ship_chunk[0] &&
                 enemy->chunk[1] == e_player.ship_chunk[1] - 1) {
        target_dir[1] = 1.0;
      } else {
        /*If the enemy on diagonal. Currently doesn't manually move it*/
      }

      vec2 smoothed_direction = GLM_VEC2_ZERO_INIT;
      glm_vec2_lerp(enemy->direction, target_dir, lerp_factor,
                    smoothed_direction);
      if (smoothed_direction[0] == 0) {
        smoothed_direction[0] = 0.05;
      }
      if (smoothed_direction[1] == 0) {
        smoothed_direction[1] = 0.05;
      }
      glm_vec2_normalize(smoothed_direction);
      glm_vec2_copy(smoothed_direction, enemy->direction);

      update_enemy_position(enemy);
      enemy->on_path = false;
      return;
    } else {
      goal_col = (int)(e_player.ship_coords[0]);
      goal_row = (int)(e_player.ship_coords[1]);
    }
  } else {
    /* If target is a trade_ship. */
    /* If Enemy ship is not in tradeship's current chunk, manually move it to
       its chunk instead of targetng */
    if (target_tradeship->chunk_coords[0] != enemy->chunk[0] ||
        target_tradeship->chunk_coords[1] != enemy->chunk[1]) {
      vec2 target_dir = GLM_VEC2_ZERO_INIT;
      /* If the enemy is in the left chunk of the player. */
      if (enemy->chunk[0] == target_tradeship->chunk_coords[0] - 1 &&
          enemy->chunk[1] == target_tradeship->chunk_coords[1]) {
        target_dir[0] = 1.0;
      } else if (enemy->chunk[0] == target_tradeship->chunk_coords[0] &&
                 enemy->chunk[1] == target_tradeship->chunk_coords[1] + 1) {
        /* If the enemy is in the upper chunk of the player*/
        target_dir[1] = -1.0;
      } else if (enemy->chunk[0] == target_tradeship->chunk_coords[0] + 1 &&
                 enemy->chunk[1] == target_tradeship->chunk_coords[1]) {
        target_dir[0] = -1.0;
      } else if (enemy->chunk[0] == target_tradeship->chunk_coords[0] &&
                 enemy->chunk[1] == target_tradeship->chunk_coords[1] - 1) {
        target_dir[1] = 1.0;
      } else {
        /*If the enemy on diagonal. Currently doesn't manually move it*/
      }

      vec2 smoothed_direction = GLM_VEC2_ZERO_INIT;
      glm_vec2_lerp(enemy->direction, target_dir, lerp_factor,
                    smoothed_direction);
      if (smoothed_direction[0] == 0) {
        smoothed_direction[0] = 0.05;
      }
      if (smoothed_direction[1] == 0) {
        smoothed_direction[1] = 0.05;
      }
      glm_vec2_normalize(smoothed_direction);
      glm_vec2_copy(smoothed_direction, enemy->direction);

      update_enemy_position(enemy);
      enemy->on_path = false;
      return;
    }
    else {
      goal_col = (int)(target_tradeship->coords[0]);
      goal_row = (int)(target_tradeship->coords[1]);
    }
  }

  /* If enemy ship is on path to the goal col, row, do the search */
  if (enemy->on_path) {
    int start_col = (int)(enemy->coords[0]);
    int start_row = (int)(enemy->coords[1]);
    Node *path_list = NULL;
    unsigned int path_list_len = 0;
    unsigned int path_list_size = 0;

    if (search(enemy->coords, start_col, start_row, goal_col, goal_row, enemy,
               enemy_chunk, &path_list, &path_list_len, &path_list_size)) {
      int min_x = start_col - SHIP_CHASE_RADIUS*2;
      int max_x = start_col + SHIP_CHASE_RADIUS*2;
      int min_y = start_row - SHIP_CHASE_RADIUS*2;
      int max_y = start_row + SHIP_CHASE_RADIUS*2;
      min_x = MAX(min_x, 0);
      max_x = MIN(max_x, C_WIDTH - 1);
      min_y = MAX(min_y, 0);
      max_y = MIN(max_y, C_WIDTH - 1);
      Node n = path_list[path_list_len - 1];
      int next_x = n.col+min_x;
      int next_y = n.row+min_y;

      vec2 next_tile = {next_x, next_y};
      vec2 difference = GLM_VEC2_ZERO_INIT;
      vec2 enemy_coords = {(int)enemy->coords[0], (int)enemy->coords[1]};
      glm_vec2_sub(next_tile, enemy_coords, difference);
      glm_vec2_normalize(difference);
      difference[1] *= -1;

      vec2 smoothed_direction = GLM_VEC2_ZERO_INIT;
      glm_vec2_lerp(enemy->direction, difference, lerp_factor,
                    smoothed_direction);
      if (smoothed_direction[0] == 0) {
        smoothed_direction[0] = 0.05;
      }
      if (smoothed_direction[1] == 0) {
        smoothed_direction[1] = 0.05;
      }
      glm_vec2_normalize(smoothed_direction);
      glm_vec2_copy(smoothed_direction, enemy->direction);
      int next_col = path_list[path_list_len - 1].col + min_x;
      int next_row = path_list[path_list_len - 1].row + min_y;

      if (next_col == goal_col && next_row == goal_row) {
        enemy->on_path = false;
      }
      vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
      vec2 movement = GLM_VEC2_ZERO_INIT;
      chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
      glm_vec2_scale(enemy->direction, delta_time * enemy->speed * T_WIDTH,
                     movement);
      glm_vec2_add(movement, enemy_world_coords, enemy_world_coords);
      world_to_chunk(enemy_world_coords, enemy->chunk, enemy->coords);
    }
    free(path_list);
  }
}

/*
    A* search algorithm
*/
int search(
    vec2 start_coords, int start_col, int start_row,
    int goal_col, int goal_row,
    E_ENEMY *enemy, unsigned int enemy_chunk,
    Node **path_list, unsigned int *path_list_len, unsigned int *path_list_size
) {
  if (start_col >= C_WIDTH || start_col < 0 ||
      start_row >= C_WIDTH || start_row < 0 ||
      goal_col >= C_WIDTH || goal_col < 0 ||
      goal_row >= C_WIDTH || goal_row < 0) {
    return 0;
  }

  Node *openList = malloc(sizeof(Node) * NODE_BUF_START_LEN);
  if (openList == NULL) {
    // error check
  }
  unsigned int open_list_len = 0;
  unsigned int open_list_size = NODE_BUF_START_LEN;

  *path_list = malloc(sizeof(Node) * NODE_BUF_START_LEN);
  if (*path_list == NULL) {
    // error check
  }
  *path_list_len = 0;
  *path_list_size = NODE_BUF_START_LEN;

  int min_x = start_col - SHIP_CHASE_RADIUS*2;
  int max_x = start_col + SHIP_CHASE_RADIUS*2;
  int min_y = start_row - SHIP_CHASE_RADIUS*2;
  int max_y = start_row + SHIP_CHASE_RADIUS*2;
  min_x = MAX(min_x, 0);
  max_x = MIN(max_x, C_WIDTH - 1);
  min_y = MAX(min_y, 0);
  max_y = MIN(max_y, C_WIDTH - 1);
  int localized_goal_col = goal_col - min_x;
  int localized_goal_row = goal_row - min_y;
  int localized_start_col = start_col-min_x;
  int localized_start_row = start_row-min_y;
  int X_WIDTH = max_x - min_x + 1;
  int Y_WIDTH = max_y - min_y + 1;
  Node nodes[X_WIDTH][Y_WIDTH];

  /*
      Setting the nodes, arrays and costs for A*
      valid_tile = returns 1 if the tile is an ocean in the given tile index
      open = returns 1 if the tile is open in the given tile index
      checked = returns 1 if the tile was checked before in the given tile index
      Array Size :
      It is maximum SHIP_CHASE_RADIUS * 2 + SHIP_CHASE_RADIUS * 2 + 1
      Can shrink if it's right next to the borderline of chunks.
  */
  for (int i = 0; i < X_WIDTH; i++) {
    for (int j = 0; j < Y_WIDTH; j++) {
      nodes[i][j].open = 0;
      nodes[i][j].checked = 0;
      ivec2 coords = { i, j };
      ivec2 s_coords = { start_coords[0]-min_x, start_coords[1]-min_y };
      ivec2 g_coords = { localized_goal_col, localized_goal_row };
      get_cost(&nodes[i][j], coords, s_coords, g_coords);

      vec2 cur_coords = { i + min_x, j + min_y };
      TILE cur_tile = get_tile(enemy_chunk, cur_coords);
      nodes[i][j].valid_tile = cur_tile == OCEAN || cur_tile == SHORE ? 1 : 0;
    }
  }
  Node start_node = nodes[localized_start_col][localized_start_row];
  Node cur_node = start_node;

  add_node(&openList, &open_list_len, &open_list_size, &cur_node);

  int goal_reached = 0;
  int step = 0;
  int best_node_idx = 0;
  while (!goal_reached && step < 600) {
    int col = cur_node.col;
    int row = cur_node.row;

    /* check the current node and remove it from the openList */
    nodes[col][row].checked = 1;
    delete_node(openList, &open_list_len, best_node_idx);
    cur_node.checked = 0;

    /* Open the node UP if it's possible, and add to openList */
    if (row - 1 >= 0) {
      open_node(&nodes[col][row - 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }

    /* Open the node LEFT if it's possible, and add to openList */
    if (col - 1 >= 0) {
      open_node(&nodes[col - 1][row], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }

    /* Open the node DOWN if it's possible, and add to openList */
    if (row + 1 < Y_WIDTH) {
      open_node(&nodes[col][row + 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }

    /* Open the node RIGHT if it's possible, and add to openList */
    if (col+ 1 < X_WIDTH) {
      open_node(&nodes[col + 1][row], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }

    /* For 8-way feature */
    /* OPEN the RIGHT UP if it it's possible, and add to openList*/
    if (col + 1 < X_WIDTH && row - 1 >= 0) {
      open_node(&nodes[col + 1][row - 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }
    /* Open the right down if it's possible*/
    if (col + 1 < X_WIDTH && row + 1 < Y_WIDTH) {
      open_node(&nodes[col + 1][row + 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }
    /* open the LEFT UP if it's possible*/
    if (col - 1 >= 0 && row - 1 >= 0) {
      open_node(&nodes[col - 1][row - 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }
    /* open the LEFT down if it's possible*/
    if (col - 1 >= 0 && row + 1 < Y_WIDTH) {
      open_node(&nodes[col - 1][row + 1], &cur_node, &openList, &open_list_len,
                &open_list_size);
    }

    best_node_idx = 0;
    float best_node_fCost = 1000;
    for (int i = 0; i < open_list_len; i++) {
      /* Check if the openList's node we checking has a smaller F cost than the
         previous best and update */
      if (openList[i].f_cost < best_node_fCost) {
        best_node_idx = i;
        best_node_fCost = openList[i].f_cost;
      }
      /* if F cost is equal, check the G cost and update */
      else if (openList[i].f_cost == best_node_fCost &&
               openList[i].g_cost < openList[best_node_idx].g_cost) {
        best_node_idx = i;
      }
    }

    /* If no more node in the openList, end the loop */
    if (open_list_len == 0) {
      break;
    }

    cur_node = openList[best_node_idx];

    /* If we reached to the goal node, we start tracking the path */
    if (cur_node.col == localized_goal_col &&
        cur_node.row == localized_goal_row) {
      goal_reached = 1;
      track_path(X_WIDTH, Y_WIDTH, nodes, path_list, path_list_len,
                 path_list_size, localized_start_col, localized_start_row,
                 localized_goal_col, localized_goal_row);
    }
    step++;
  }
  free(openList);
  return goal_reached;
}

/*
    function for tracking the best path we got from A * search alg. by
    backtracking
*/
void track_path(int arr_size_x, int arr_size_y,
                Node nodes[arr_size_x][arr_size_y],
                Node **path_list, unsigned int *path_list_len,
                unsigned int *path_list_size, int start_col, int start_row,
                int goal_col, int goal_row) {
  Node *current = &nodes[goal_col][goal_row];
  while (current->col != start_col || current->row != start_row) {
    // printf("X %d Y %d\n",current->col, current->row);
    add_node(path_list, path_list_len, path_list_size, current);
    current = &nodes[current->parent_col][current->parent_row];
  }
}

/*
    function for getting the cost needed for A * algorithm of the given node
*/
void get_cost(Node *node, ivec2 coords, ivec2 start_coords,
              ivec2 goal_coords) {
  // G cost
  node->g_cost = glm_ivec2_distance(coords, start_coords);
  // H cost
  node->h_cost = glm_ivec2_distance(coords, goal_coords);
  // F cost
  node->f_cost = node->g_cost + node->h_cost;
  node->row = coords[Y];
  node->col = coords[X];
}

/*
    Checks if we can open the given node
    If so, add to the openList
    tiles = 1 mean it is ocean
*/
void open_node(Node *node, Node *cur_node,
               Node **open_list, unsigned int *open_list_len,
               unsigned int *open_list_size) {
  if (!node->open && !node->checked && node->valid_tile) {
    node->open = 1;
    node->parent_col = cur_node->col;
    node->parent_row = cur_node->row;
    add_node(open_list, open_list_len, open_list_size, node);
    // printf("Opened node %d %d\n", node->col,node->row);
  }
}

int add_node(Node **list, unsigned int *list_len, unsigned int *list_size,
             Node *node) {
  (*list)[*list_len] = *node;
  (*list_len)++;
  if (*list_len == *list_size) {
    return double_buffer((void **) list, list_size, sizeof(Node));
  }
  return 0;
}

void delete_node (Node *list, unsigned int *list_len, unsigned int index) {
  (*list_len)--;
  list[index] = list[*list_len];
}

/*
  Updates the CHUNK's num_enemies and enemies buffer when changes are made
*/
void update_enemy_chunk(E_ENEMY *cur_enemy, CHUNK *chunk, int i) {
  if (cur_enemy->chunk[0] != chunk->coords[0] ||
      cur_enemy->chunk[1] != chunk->coords[1]) {
    // printf("Enemy chunk changed\n");
    ivec2 chunk_offset = GLM_VEC2_ZERO_INIT;
    glm_ivec2_sub(cur_enemy->chunk, e_player.ship_chunk, chunk_offset);
    CHUNK *new_enemy_chunk;
    int chunk_index = (chunk_offset[0] + 1) + 3 * (1 - chunk_offset[1]);
    new_enemy_chunk = chunk_buffer + player_chunks[chunk_index];
    memcpy(&new_enemy_chunk->enemies[new_enemy_chunk->num_enemies], cur_enemy,
           sizeof(E_ENEMY));
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
  if (enemy->weapon_type == RANGED) {
    if (enemy->attack_cooldown == 0.0) {
      npc_ranged_attack(enemy);
    }
  
  } else {
    if (target_dist <= 1.5 && enemy->attack_cooldown == 0.0) {
      npc_melee_attack(enemy);
    }
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
