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
  generate_chunk_tiles(chunk_tiles, player_chunks[chunk_pos]);
  while (not_found) {
    posx = rand() % C_WIDTH;
    posy = rand() % C_WIDTH;
    TILE tile = chunk_tiles[posx][posy];
    if ((in_player_chunk && e_player.ship_coords[0] != posx &&
         e_player.ship_coords[1] != posy) || !in_player_chunk) {
      if (tile == 1) {
        /* Sets up base enemy */
        unsigned int insert_index = player_chunks[chunk_pos].num_enemies;
        player_chunks[chunk_pos].num_enemies++;

        int status = 0;
        if (player_chunks[chunk_pos].num_enemies ==
            player_chunks[chunk_pos].enemy_buf_size) {
          status = double_buffer((void **) &player_chunks[chunk_pos].enemies,
                                 &player_chunks[chunk_pos].enemy_buf_size,
                                 sizeof(E_ENEMY));
        }

        if (status == 0) {
          E_ENEMY *enemy = &player_chunks[chunk_pos].enemies[insert_index];
          enemy->chunk[0] = chosen_chunk[0];
          enemy->chunk[1] = chosen_chunk[1];
          enemy->coords[0] = posx;
          enemy->coords[1] = posy;
          enemy->direction[0] = 0;
          enemy->direction[1] = 1;
          enemy->speed = 0.01;
          enemy->crew_count = 1;
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

/*
    Updates enemy position based on its direction and speed
*/
void update_enemy_position(E_ENEMY * enemy) {
    vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
    vec2 movement = GLM_VEC2_ZERO_INIT;
    printf("Enemy coords: X %f Y %f\n", enemy->coords[0], enemy->coords[1]);
    printf("Enemy chunk coords: X %d y %d\n", enemy->chunk[0], enemy->chunk[1]);
    chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
    glm_vec2_scale(enemy->direction, enemy->speed, movement);
    printf("movement : %f %f\n", movement[0], movement[1]);
    glm_vec2_add(movement, enemy_world_coords, enemy_world_coords);
    world_to_chunk(enemy_world_coords, enemy->chunk, enemy->coords);
    // printf("My coords : X %f Y %f\n", e_player.ship_coords[0], e_player.ship_coords[1]);
    printf("Updated Enemy coords: X %f Y %f\n\n", enemy->coords[0], enemy->coords[1]);
    
}

void pathfind_enemy(E_ENEMY * enemy) {
    
    if (enemy->on_path) {
        int goal_col = (int) (e_player.ship_coords[0]);
        int goal_row = (int) (e_player.ship_coords[1]);
        // int goal_col = 110;
        // int goal_row = 80;
        int start_col = (int) (enemy->coords[0]);
        int start_row = (int) (enemy->coords[1]);
        printf("start col : %d start row: %d\ngoal col : %d goal row :%d\n\n", start_col, start_row, goal_col, goal_row);
        vector *path_list = (vector*) malloc(sizeof(vector));
        if (search(start_col, start_row, goal_col, goal_row, enemy, path_list)) {
            int next_x = ((Node *) vector_get(path_list, vector_total(path_list)-1))->col;
            int next_y = ((Node *) vector_get(path_list, vector_total(path_list)-1))->row;
            
            printf("next_x : %d next_y : %d\n\n", next_x, next_y);
            vec2 next_tile = {next_x, next_y};
            vec2 difference = GLM_VEC2_ZERO_INIT;
            vec2 enemy_coords = {(int) enemy->coords[0], (int)enemy->coords[1]};
            glm_vec2_sub(next_tile, enemy_coords, difference);
            printf("difference X %f Y %f\n", difference[0], difference[1]);
            glm_vec2_normalize(difference);
            printf("normalized X %f Y %f\n", difference[0], difference[1]);
            enemy->direction[0] = difference[0];
            enemy->direction[1] = -difference[1];
            int next_col =  ((Node *) vector_get(path_list, vector_total(path_list)-1))->col;
            int next_row = ((Node *) vector_get(path_list, vector_total(path_list)-1))->row;
            
            if (next_col == goal_col && next_row == goal_row) {
                enemy->on_path = false;
            }
        }
        free(path_list);
        
    }
    // vec2 player_world_coords = GLM_VEC2_ZERO_INIT;
    // vec2 enemy_world_coords = GLM_VEC2_ZERO_INIT;
    // chunk_to_world(enemy->chunk, enemy->coords, enemy_world_coords);
    // chunk_to_world(e_player.ship_chunk, e_player.ship_coords, player_world_coords);
    // vec2 difference = GLM_VEC2_ZERO_INIT;
    // glm_vec2_sub(enemy_world_coords, player_world_coords, difference);
    // printf("position difference : X %f Y %f", difference[0], difference[1]);
    // glm_vec2_scale_as(difference, -1.0, difference);
    // printf("new direction : X %f Y %f", difference[0], difference[1]);
    // enemy->direction[0] = difference[0];
    // enemy->direction[1] = difference[1];


}


int search (int start_col, int start_row, int goal_col, int goal_row, E_ENEMY * enemy, vector* path_list) {
    if (start_col >= C_WIDTH || start_col < 0 ||
        start_row >= C_WIDTH || start_row < 0 ||
        goal_col >= C_WIDTH || goal_col < 0 ||
        goal_row >= C_WIDTH || goal_row < 0) {
        
        return 0;
    }

    /*
        Setting the nodes
    */
    int tiles[C_WIDTH][C_WIDTH] = {
        [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = 1 }
    };
    int open[C_WIDTH][C_WIDTH]= {
        [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = 0 }
    };
    int checked[C_WIDTH][C_WIDTH] = {
        [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = 0 }
    };
    Node nodes[C_WIDTH][C_WIDTH];

    /* getting the chunk of the enemy and creating a tilemap to check which objects are able/unable to pass through*/
    ivec2 chunk_offset = GLM_VEC2_ZERO_INIT;
    glm_ivec2_sub(enemy->chunk, e_player.ship_chunk, chunk_offset);
    CHUNK * enemy_chunk;
    int chunk_index = (chunk_offset[0] + 1) + 3 * (1 - chunk_offset[1]);
    enemy_chunk = &player_chunks[chunk_index];
    int in_player_chunk = chunk_index == CURRENT_CHUNK ? 1 : 0;
    if (!in_player_chunk) {
      printf("Not in player chunk\n\n");
      enemy->on_path = false;
      return 0;
    }
    printf("enemy_chunk coords check : %d %d\n",enemy_chunk->coords[0], enemy_chunk->coords[1]);
    generate_chunk_tiles(tiles, *enemy_chunk);

    
    //Node goal_node = nodes[goal_col][goal_row];

    for (int i = 0; i < C_WIDTH; i++) {
        for (int j = 0; j < C_WIDTH; j++) {
            get_cost(&nodes[i][j], i, j, start_col, start_row, goal_col, goal_row);
        }
    }
    Node start_node = nodes[start_col][start_row];
    Node cur_node = start_node;
    vector openList;
    vector_init(&openList);
    vector_add(&openList, (void*) &cur_node);
    vector_init(path_list);
    int goal_reached = 0;
    int step = 0;
    int best_node_idx = 0;
    while (!goal_reached) {
        // printf("iteration %d\n", step);
        int col = cur_node.col;
        int row = cur_node.row;
        //printf ("cur node: X %d Y %d\n", col, row);
        // check the current node
        checked[col][row] = 1;
        // Acts same as removing the current checking node from the openList
        vector_delete(&openList, best_node_idx);

        if (row - 1 >= 0) {
            open_node(&nodes[col][row-1], &cur_node, open, checked, tiles, &openList);
        }

        if (col - 1 >= 0) {
            open_node(&nodes[col-1][row], &cur_node, open, checked, tiles, &openList);
        }
        
        if (row + 1 < C_WIDTH) {
            open_node(&nodes[col][row+1], &cur_node, open, checked, tiles, &openList);
        }

        if (col + 1 < C_WIDTH) {
            open_node(&nodes[col+1][row], &cur_node, open, checked, tiles, &openList);
        }

        best_node_idx = 0;
        int best_node_fCost = 1000;
        for (int i = 0; i < vector_total(&openList); i++) {
            // printf("Node we got in the open list[%d]: X %d Y %d gcost = %d fcost = %d\n\n", i, ((Node*)vector_get(&openList, i))->col, ((Node *)vector_get(&openList, i))->row,
            // ((Node*)vector_get(&openList, i))->g_cost, ((Node*)vector_get(&openList, i))->f_cost);
            // printf("Node we got in the open list[%d]: X %d Y %d fCost = %d\n\n", i, ((Node*)vector_get(&openList, i))->col, ((Node *)vector_get(&openList, i))->row, ((Node*)vector_get(&openList, i))->f_cost);
            if (((Node *)vector_get(&openList, i))->f_cost < best_node_fCost) {
                best_node_idx = i;
                best_node_fCost = ((Node *)vector_get(&openList, i))->f_cost;
            }
            // if F cost is equal, check the G cost
            else if (((Node *)vector_get(&openList, i))->f_cost == best_node_fCost) {
                if (((Node *)vector_get(&openList, i))->g_cost < ((Node *)vector_get(&openList, best_node_idx))->g_cost) {
                    best_node_idx = i;
                }
            }
        }

        // If no more node, end the loop
        if (vector_total(&openList) == 0) {
            break;
        }
        
        printf("best_fCost : %d best_node_idx : %d\n\n", best_node_fCost, best_node_idx);
        cur_node = *(Node *) vector_get(&openList, best_node_idx);

        if (cur_node.col == goal_col && cur_node.row == goal_row) {
            goal_reached = 1;
            printf("goal_reached\n");
            track_path(nodes, path_list, start_col, start_row, goal_col, goal_row);
        }
        step++;
    }
    printf("step: %d\n",step);
    printf("-----------------PATH---------------\n\n");
    for (int i = 0; i < vector_total(path_list); i++) {
        printf("X %d Y %d\n",((Node*) vector_get(path_list, i))->col, ((Node*) vector_get(path_list, i))->row);
    }
    vector_free(&openList);
    vector_free(path_list);
    return goal_reached;
}

void track_path(Node nodes[C_WIDTH][C_WIDTH], vector *path_list, int start_col, int start_row, int goal_col, int goal_row) {
     Node *current = &nodes[goal_col][goal_row];
     while (current->col != start_col || current->row != start_row) {
        // printf("X %d Y %d\n",current->col, current->row);
        vector_add(path_list, (void *) current);
        current = &nodes[current->parent_col][current->parent_row];
     }
}

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

// tiles = 1 mean it is ocean
void open_node(Node *node, Node *cur_node, int open[C_WIDTH][C_WIDTH], int checked[C_WIDTH][C_WIDTH], int tiles[C_WIDTH][C_WIDTH], vector *openList) {
    printf("open[%d][%d] : %d\nchecked[%d][%d]: %d\ntiles[%d][%d] : %d\n\n", 
            node->col, node->row, open[node->col][node->row], node->col, node->row, checked[node->col][node->row],
            node->col, node->row, tiles[node->col][node->row]);
    if (open[node->col][node->row] == 0 && checked[node->col][node->row] == 0 && tiles[node->col][node->row] == 1) {
        open[node->col][node->row] = 1;
        node->parent_col = cur_node->col;
        node->parent_row = cur_node->row;
        vector_add(openList, (void*) node);
        //printf("Opened node %d %d\n", node->col,node->row);
    }  
}