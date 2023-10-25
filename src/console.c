#include <console.h>

void command_not_found() {
  printf("Command not found\n");
  fflush(stdout);
}

/*
Teleports the player to the nearest island within the chunks that
are currently being rendered
*/
void teleport_nearest_island() {
  if (mode != EXPLORATION) {
    printf("ERROR: Not in exploration mode!\n");
    return;
  } else if (!e_player.embarked) {
    printf("ERROR: Not embarked on ship!\n");
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
  vec2 player_coords = { 0.0, 0.0 };
  for (int i = 0; i < MAX_CHUNKS; i++) {
    chunk = chunk_buffer[player_chunks[i]];
    for (int j = 0; j < chunk.num_islands; j++) {
      dist[tot_num_islands].island = chunk.islands[j];
      glm_ivec2_copy(dist[tot_num_islands].island.chunk, chunk_coords);
      glm_ivec2_copy(dist[tot_num_islands].island.coords, intra_chunk_int);
      intra_chunk[0] = (float) intra_chunk_int[0];
      intra_chunk[1] = (float) intra_chunk_int[1];
      /* Find the coordinates of the island in world space */
      chunk_to_world(chunk_coords, intra_chunk, world_coords);
      chunk_to_world(e_player.ship_chunk, e_player.ship_coords, player_coords);
      dist[tot_num_islands++].distance = glm_vec2_distance(player_coords, world_coords);
    }
  }

  if (!tot_num_islands) {
    printf("ERROR: No islands in simulated chunks\n");
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
  e_player.ship_coords[0] = island_coords_float[0];
  e_player.ship_coords[1] = island_coords_float[1];
  e_player.coords[0] = island_coords_float[0];
  e_player.coords[1] = island_coords_float[1];
  printf("\nNEW CHUNK: %d | %d\nNEW WORLD COORDINATES %f | %f\n",
          dist[shortest].island.chunk[0], dist[shortest].island.chunk[1],
          island_coords_float[0], island_coords_float[1]);
  fprintf(stderr, "Operation completed successfully\n");
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
  fprintf(stderr, "Operation completed successfully\n");
}

/* Give or remove mercenaries */
void give_mercenaries(int change) {
  if (change == 0) {
    return;
  }
  e_player.total_mercenaries += change;
  if (e_player.total_mercenaries < 0) {
    e_player.total_mercenaries = 0;
  }
  fprintf(stderr, "Operation completed successfully\n");
}

/* Change the speed of the player  */
void set_speed(float speed) {
  if (speed <= 0.0) {
    return;
  }
  if (mode == EXPLORATION) {
    e_player.speed = speed;
  } else if (mode == COMBAT) {
    c_player.speed = speed;
  }
  fprintf(stderr, "Operation completed successfully\n");
}

/* Updates the buffer of what is in the console on the screen */
/* for every frame */
void update_console_prompt() {
  if (console_enabled) {
    vec2 console_pos = { 0.0, -1.0 };
    init_menu(
      console_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      cons_cmd, // text
      1, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      2.0, // width
      0.0, // height
      PIVOT_BOTTOM, // pivot
      T_LEFT, // text_anchor
      get_ui_component_by_ID(CONSOLE) // dest
    );
    vec2 cursor_pos = GLM_VEC2_ZERO_INIT;
    calc_cursor_pos(cursor_pos);
    init_menu(
      cursor_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      "_", // text
      cursor_enabled, // enabled
      0, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_BOTTOM_LEFT, // pivot
      T_LEFT, // text_anchor
      get_ui_component_by_ID(CONSOLE_CURSOR) // dest
    );
  }
}

/* Resets the console buffer and closes the prompt  */
void close_console_prompt() {
  get_ui_component_by_ID(CONSOLE)->enabled = 0;
  get_ui_component_by_ID(CONSOLE_CURSOR)->enabled = 0;
  for (int i = 0; i < 100; i++) {
    cons_cmd[i] = '\0';
  }
}

void calc_cursor_pos(vec2 dest) {
  float screen_text_scale = get_screen_text_scale();
  UI_COMPONENT *console = get_ui_component_by_ID(CONSOLE);
  float text_width = get_text_width(cons_cmd, strlen(cons_cmd))
                     * screen_text_scale / screen_scale[0];
  //text_width += console->text_padding;
  dest[1] = console->position[1];
  dest[0] = (text_width) - (0.5 * console->width);
}
