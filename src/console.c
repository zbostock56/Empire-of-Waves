#include <console.h>

void command_not_found() {
  set_console_error("Command not found");
  print_parse_table();
}

/*
Teleports the player to the nearest island within the chunks that
are currently being rendered
*/
void teleport_nearest_island() {
  if (mode != EXPLORATION) {
    set_console_error("ERR: Not in exploration mode");
    return;
  } else if (!e_player.embarked) {
    set_console_error("ERR: Not embarked on ship");
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
    set_console_error("ERR: No islands in simulated chunks");
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
}

/* Finds which merchant is closest and teleports */
void teleport_nearest_merchant() {
  if (mode != EXPLORATION) {
    set_console_error("ERR: Not in exploration mode");
    return;
  } else if (!e_player.embarked) {
    set_console_error("ERR: Not emarked on ship");
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
      if (chunk.islands[j].has_merchant) {
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
  }

  if (!tot_num_islands) {
    set_console_error("ERR: No merchants in simulated chunks");
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
}

#if 0
/* Establishes trade route with nearest merchant */
void establish_nearest_traderoute() {
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
      if (chunk.islands[j].has_merchant) {
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
  }

  if (!tot_num_islands) {
    set_console_error("ERR: No merchants in simulated chunks");
    return;
  }

  /* Find which distance is the shortest*/
  int shortest = 0;
  float shortest_distance = FLT_MAX;
  for (int i = 0; i < tot_num_islands; i++) {
     if (dist[i].distance < shortest_distance) {
        shortest_distance = dist[i].distance;
        shortest = i;
     }
  }
  cur_merchant = &dist[shortest].island.merchant;
  open_establish_trade_route();
  cur_merchant = NULL;
}
#endif

void teleport(ivec2 pos) {
  ivec2 chunk_coords = { pos[0] / C_WIDTH , pos[1] / C_WIDTH };
  vec2 intra_chunk = { (float) (pos[0] % C_WIDTH), (float) (pos[1] % C_WIDTH) };
  if (mode == EXPLORATION) {
    glm_ivec2_copy(chunk_coords, e_player.ship_chunk);
    glm_vec2_copy(intra_chunk, e_player.ship_coords);
  }
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
}

void teleport_home() {
  ivec2 home_chunk = { 0, 0 };
  if (mode == EXPLORATION) {
    glm_vec2_copy(home_island_coords, e_player.coords);
    glm_vec2_copy(home_island_coords, e_player.ship_coords);
    glm_ivec2_copy(home_chunk, e_player.chunk);
    glm_ivec2_copy(home_chunk, e_player.ship_chunk);
  }
}

/* Updates the buffer of what is in the console on the screen */
/* for every frame */
void update_console_prompt() {
  if (console_input_enabled) {
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
      event_flags[CONS_CURSOR],
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

/*
  Prints current coordinates to the screen using the command "coords"
*/
void print_coords() {
  if (coords_enabled) {
    //vec2 world_tile_coords = GLM_VEC2_ZERO_INIT;
    if (e_player.embarked) {
    /*
      world_tile_coords[0] = C_WIDTH * e_player.ship_chunk[0];
      world_tile_coords[1] = C_WIDTH * e_player.ship_chunk[1];

      if (world_tile_coords[0] < 0.0) {
        world_tile_coords[0] -= e_player.ship_coords[0];
        printf("%f\n", world_tile_coords[0]);
      } else {
        world_tile_coords[0] += e_player.ship_coords[0];
      }

      if (world_tile_coords[1] < 0) {
        world_tile_coords[1] -= C_WIDTH + e_player.ship_coords[1];
      } else {
        world_tile_coords[1] += e_player.ship_coords[1];
      }
    */
    snprintf(console_intra_chunk, 100, "Chunk Coords: (%.3f, %.3f)",
             e_player.ship_coords[0], e_player.ship_coords[1]);
    snprintf(console_chunk_coords, 100, "Global Chunk: (%d, %d)",
             e_player.ship_chunk[0], e_player.ship_chunk[1]);
    } else {
      /*
      world_tile_coords[0] = C_WIDTH * e_player.chunk[0];
      world_tile_coords[1] = C_WIDTH * e_player.chunk[0];

      if (world_tile_coords[0] < 0.0) {
        world_tile_coords[0] -= C_WIDTH - e_player.coords[0];
      } else {
        world_tile_coords[0] += e_player.coords[0];
      }

      if (world_tile_coords[1] < 0) {
        world_tile_coords[1] -= C_WIDTH - e_player.coords[1];
      } else {
        world_tile_coords[1] += e_player.coords[1];
      }
      */
      snprintf(console_intra_chunk, 100, "Chunk Coords: (%.3f, %.3f)",
               e_player.ship_coords[0], e_player.ship_coords[1]);
      snprintf(console_chunk_coords, 100, "Global Chunk: (%d, %d)",
               e_player.ship_chunk[0], e_player.ship_chunk[1]);
    }
    /*
    snprintf(console_world_coords, 100, "World Coords: (%.3f, %.3f)",
             world_tile_coords[0], world_tile_coords[1]);
    */
    /*
    vec2 world_coords_pos = { 1.0, 1.0};
    init_menu(
      world_coords_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      console_world_coords, // text
      coords_enabled, // enabled
      0, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_TOP_RIGHT, // pivot
      T_LEFT, // text_anchor
      get_ui_component_by_ID(CONSOLE_WORLD_COORDS) // dest
    );
    */
    vec2 intra_chunk_pos = { 1.0, 0.9 };
    init_menu(
      intra_chunk_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      console_intra_chunk, // text
      coords_enabled, // enabled
      0, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_TOP_RIGHT, // pivot
      T_LEFT, // text_anchor
      get_ui_component_by_ID(CONSOLE_INTRA_CHUNK_COORDS) // dest
    );
    vec2 chunk_coords_pos = { 1.0, 0.8 };
    init_menu(
      chunk_coords_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      console_chunk_coords, // text
      coords_enabled, // enabled
      0, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_TOP_RIGHT, // pivot
      T_LEFT, // text_anchor
      get_ui_component_by_ID(CONSOLE_CHUNK_COORDS) // dest
    );
  }
}

/*
  Closes all parts related to the coords
*/
void close_coords() {
  get_ui_component_by_ID(CONSOLE_WORLD_COORDS)->enabled = 0;
  get_ui_component_by_ID(CONSOLE_INTRA_CHUNK_COORDS)->enabled = 0;
  get_ui_component_by_ID(CONSOLE_CHUNK_COORDS)->enabled = 0;
}

/*
  Resets the console buffer and closes the prompt
*/
void close_console_prompt() {
  get_ui_component_by_ID(CONSOLE)->enabled = 0;
  get_ui_component_by_ID(CONSOLE_CURSOR)->enabled = 0;
  for (int i = 0; i < 100; i++) {
    cons_cmd[i] = '\0';
  }
}

/*
  Find the position in the console box to put the cursor
*/
void calc_cursor_pos(vec2 dest) {
  float screen_text_scale = get_screen_text_scale();
  UI_COMPONENT *console = get_ui_component_by_ID(CONSOLE);
  float text_width = get_text_width(cons_cmd, strlen(cons_cmd))
                     * screen_text_scale / screen_scale[0];
  dest[1] = console->position[1];
  dest[0] = (text_width) - (0.5 * console->width);
}

/*
  Open the error menu
*/
void console_error_init() {
  vec2 console_error_pos = { 0.0, 0.0 };
  init_menu(
      console_error_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      console_error_buffer, // text
      event_flags[CONS_ERROR],
      1, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_TOP, // pivot
      T_CENTER, // text_anchor
      get_ui_component_by_ID(CONSOLE_ERROR) // dest
  );
}

/*
Helper function to raise errors when they occur
*/
void set_console_error(const char *error) {
  memcpy(console_error_buffer, error, strlen(error));
  timers[CONS_ERROR] = 1.5;
  event_flags[CONS_ERROR] = 1;
  console_error_init();
}

/* Resets the buffer for the next error */
void reset_console_error() {
  get_ui_component_by_ID(CONSOLE_ERROR)->enabled = 0;
  memset(console_error_buffer, '\0', CONSOLE_BUFFER_MAX);
}

void set_weather(int w) {
  if (w == CLEAR) {
    weather = CLEAR;
    event_flags[WEATHER_TIMER] = 0;
    timers[WEATHER_TIMER] = WEATHER_TIME;
  } else if (w == FOG) {
    weather = FOG;
    event_flags[WEATHER_TIMER] = 1;
    timers[WEATHER_TIMER] = WEATHER_TIME;
  }
}

void spawn_invasion() {
  float temp = e_player.reputation;
  e_player.reputation = REP_MIN;
  invade_home_island();
  e_player.reputation = temp;
}
