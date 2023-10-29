#include <m.h>

int compare_chunk(CHUNK *a, CHUNK *b);

void generate_chunk_tileset(TILE chunk_tiles[C_WIDTH][C_WIDTH], CHUNK *chunk) {
  ISLAND island;
  int chunkx = 0;
  int chunky = 0;
  for (int i = 0; i < chunk->num_islands; i++) {
    island = chunk->islands[i];
    int xoffset = island.coords[0];
    int yoffset = island.coords[1];
    for (int j = 0; j < I_WIDTH * I_WIDTH; j++) {
      chunkx = xoffset + (j % I_WIDTH);
      chunky = yoffset + (j / I_WIDTH);
      chunk_tiles[chunkx][chunky] = island.tiles[j];
    }
  }
}

int main() {
  init_gl();
  srand(glfwGetTime());
  new_game(game_save_name);

  CHUNK before;
  CHUNK after;

  fprintf(stderr, "TESTING CHUNK SERIALIZATION VALIDITY\n");
  manage_chunks();
  for (int i = 0; i < chunk_buff_len; i++) {
    before = chunk_buffer[i];
    fprintf(stderr, "Testing chunk (%d, %d)\n", before.coords[0],
                                                before.coords[1]);
    save_chunk(&before, UNSAVED_DIR_PATH);
    load_chunk(before.coords, &after, UNSAVED_DIR_PATH);
    if (!compare_chunk(&before, &after)) {
      fprintf(stderr, "Chunk passed\n");
    }
  }
  fprintf(stderr, "CHUNK SERIALIZATION VALIDITY DONE\n\n\n");

  fprintf(stderr, "TESTING GAME SAVE SERIALIZATION VALIDITY\n");
  STATE before_state;
  randomize_player();
  randomize_state();
  save_game(game_save_name);
  copy_state(&before_state);
  trade_ships = malloc(sizeof(TRADE_SHIP) * 10);
  trade_ship_buf_size = 10;
  num_trade_ships = 0;
  load_game(game_save_name);
  compare_state(&before_state);
  fprintf(stderr, "GAME SAVE SERIALIZATION VALIDITY DONE\n");
  exit(0);
}

void randomize_player() {
  randomize_ivec2(e_player.chunk, 1000);
  randomize_ivec2(e_player.ship_chunk, 1000);
  randomize_vec2(e_player.coords, C_WIDTH);
  randomize_vec2(e_player.ship_coords, C_WIDTH);
  randomize_vec2(e_player.direction, 100);
  e_player.embarked = rand();
  for (int i = 0; i < INV_SIZE; i++) {
    e_player.inventory[i].item_id = rand();
    e_player.inventory[i].quantity = rand();
  }
  for (int i = 0; i < NUM_RESOURCES; i++) {
    e_player.resources[i] = rand();
  }
  e_player.money = rand();
  e_player.total_mercenaries = rand();
  e_player.max_health = (float) (rand() % 100);
  e_player.health = (float) (rand() % 100);
  e_player.speed = (float) (rand() % 100);
}

void randomize_state() {
  mode = rand();
  randomize_vec2(home_island_coords, 100);
  randomize_vec2(house_tile, 100);
  global_time = (float) rand();
  num_trade_ships = rand() % 10;
  trade_ships = malloc(sizeof(TRADE_SHIP) * num_trade_ships);
  for (int i = 0; i < num_trade_ships; i++) {
    randomize_ts(trade_ships + i);
  }
}

void randomize_ts(TRADE_SHIP *ts) {
  randomize_ivec2(ts->chunk_coords, 1000);
  randomize_vec2(ts->coords, C_WIDTH);
  randomize_vec2(ts->direction, 100);
  ts->cur_chunk_index = 0;
  ts->target_chunk_index = 0;
  ts->updated_chunk_index = 0;
  ts->export_rec = rand();
  ts->import_rec = rand();
  ts->target_island = rand();
  ts->num_mercenaries = rand();
  ts->speed = (float) (rand() % 100);
}

void randomize_vec2(vec2 vec, int bound) {
  vec[0] = (float) (rand() % bound);
  vec[1] = (float) (rand() % bound);
}

void randomize_ivec2(ivec2 vec, int bound) {
  vec[0] = rand() % bound;
  vec[1] = rand() % bound;
}

void copy_state(STATE *dest) {
  dest->e_player = e_player;
  dest->mode = mode;
  glm_vec2_copy(home_island_coords, dest->home_island_coords);
  glm_vec2_copy(house_tile, dest->house_tile);
  dest->global_time = global_time;
  dest->num_trade_ships = num_trade_ships;
  dest->trade_ships = trade_ships;
}

void compare_state(STATE *dest) {
  if (!compare_player(&e_player, &dest->e_player)) {
    fprintf(stderr, "PLAYER PASSED\n");
  }
  if (mode != dest->mode) {
    fprintf(stderr, "MODE FAILURE: %d != %d\n", mode, dest->mode);
  } else {
    fprintf(stderr, "MODE PASSED\n");
  }
  if (home_island_coords[0] != dest->home_island_coords[0] ||
      home_island_coords[1] != dest->home_island_coords[1]) {
    fprintf(stderr, "HOME_ISLAND_COORDS ERROR: (%f, %f) != (%f, %f)\n",
            home_island_coords[0], home_island_coords[1],
            dest->home_island_coords[0], dest->home_island_coords[1]);
  } else {
    fprintf(stderr, "HOME_ISLAND_COORDS PASSED\n");
  }
  if (house_tile[0] != dest->house_tile[0] ||
      house_tile[1] != dest->house_tile[1]) {
    fprintf(stderr, "HOUSE_TILE ERROR: (%f, %f) != (%f, %f)\n",
            house_tile[0], house_tile[1],
            dest->house_tile[0], dest->house_tile[1]);
  } else {
    fprintf(stderr, "HOUSE_TILE PASSED\n");
  }
  if (global_time != dest->global_time) {
    fprintf(stderr, "GLOBAL_TIME ERROR: %f != %f\n", global_time,
            dest->global_time);
  } else {
    fprintf(stderr, "GLOBAL_TIME PASSED\n");
  }
  if (num_trade_ships != dest->num_trade_ships) {
    fprintf(stderr, "NUM_TRADE_SHIPS ERROR: %d != %d\n", num_trade_ships,
            dest->num_trade_ships);
  } else {
    for (int i = 0; i < num_trade_ships; i++) {
      if (!compare_trade_ship(trade_ships + i, dest->trade_ships + i)) {
        fprintf(stderr, "TRADE_SHIP[%d] PASSED\n", i);
      }
    }
  }
}

int compare_player(E_PLAYER *a, E_PLAYER *b) {
  int status = 0;
  if (a->chunk[0] != b->chunk[0] || a->chunk[1] != b->chunk[1]) {
    fprintf(stderr, "PLAYER CHUNK ERROR: (%d, %d) != (%d, %d)\n",
            a->chunk[0], a->chunk[1], b->chunk[0], b->chunk[1]);
    status = 1;
  }
  if (a->ship_chunk[0] != b->ship_chunk[0] ||
      a->ship_chunk[1] != b->ship_chunk[1]) {
    fprintf(stderr, "PLAYER SHIP CHUNK ERROR: (%d, %d) != (%d, %d)\n",
            a->ship_chunk[0], a->ship_chunk[1], b->ship_chunk[0],
            b->ship_chunk[1]);
    status = 1;
  }
  if (a->coords[0] != b->coords[0] || a->coords[1] != b->coords[1]) {
    fprintf(stderr, "PLAYER COORDS ERROR: (%f, %f) != (%f, %f)\n",
            a->coords[0], a->coords[1], b->coords[0], b->coords[1]);
    status = 1;
  }
  if (a->ship_coords[0] != b->ship_coords[0] ||
      a->ship_coords[1] != b->ship_coords[1]) {
    fprintf(stderr, "PLAYER SHIP COORDS ERROR: (%f, %f) != (%f, %f)\n",
            a->ship_coords[0], a->ship_coords[1], b->ship_coords[0],
            b->ship_coords[1]);
    status = 1;
  }
  if (a->direction[0] != b->direction[0] ||
      a->direction[1] != b->direction[1]) {
    fprintf(stderr, "PLAYER DIRECTION ERROR: (%f, %f) != (%f, %f)\n",
            a->direction[0], a->direction[1], b->direction[0],
            b->direction[1]);
    status = 1;
  }
  if (a->ship_direction[0] != b->ship_direction[0] ||
      a->ship_direction[1] != b->ship_direction[1]) {
    fprintf(stderr, "PLAYER SHIP DIRECTION ERROR: (%f, %f) != (%f, %f)\n",
            a->ship_direction[0], a->ship_direction[1], b->ship_direction[0],
            b->ship_direction[1]);
    status = 1;
  }
  return status;
}

int compare_trade_ship(TRADE_SHIP *a, TRADE_SHIP *b) {
  int status = 0;
  if (a->chunk_coords[0] != b->chunk_coords[0] ||
      a->chunk_coords[1] != b->chunk_coords[1]) {
    fprintf(stderr, "TRADE CHUNK ERROR: (%d, %d) != (%d, %d)\n",
            a->chunk_coords[0], a->chunk_coords[1], b->chunk_coords[0],
            b->chunk_coords[1]);
    status = 1;
  }
  if (a->coords[0] != b->coords[0] || a->coords[1] != b->coords[1]) {
    fprintf(stderr, "TRADE COORDS ERROR: (%f, %f) != (%f, %f)\n",
            a->coords[0], a->coords[1], b->coords[0], b->coords[1]);
    status = 1;
  }
  if (a->direction[0] != b->direction[0] ||
      a->direction[1] != b->direction[1]) {
    fprintf(stderr, "TRADE DIRECTION ERROR: (%f, %f) != (%f, %f)\n",
            a->direction[0], a->direction[1], b->direction[0],
            b->direction[1]);
    status = 1;
  }
  if (a->export_rec != b->export_rec) {
    fprintf(stderr, "TRADE EXPORT ERROR: %d != %d\n", a->export_rec,
            b->export_rec);
    status = 1;
  }
  if (a->import_rec != b->import_rec) {
    fprintf(stderr, "TRADE IMPORT ERROR: %d != %d\n", a->import_rec,
            b->import_rec);
    status = 1;
  }
  if (a->target_island != b->target_island) {
    fprintf(stderr, "TRADE TARGET ISLAND ERROR: %d != %d\n", a->target_island,
            b->target_island);
    status = 1;
  }
  if (a->num_mercenaries != b->num_mercenaries) {
    fprintf(stderr, "TRADE NUM MERCENARIES ERROR: %d != %d\n",
            a->num_mercenaries, b->num_mercenaries);
    status = 1;
  }
  if (a->speed != b->speed) {
    fprintf(stderr, "TRADE speed ERROR: %f != %f\n", a->speed, b->speed);
    status = 1;
  }

  return status;
}

int compare_chunk(CHUNK *a, CHUNK *b) {
  int status = 0;
  if (a->coords[0] != b->coords[0] ||
      a->coords[1] != b->coords[1]) {
    fprintf(stderr, "COORD FAILURE: (%d, %d) != (%d, %d)\n", a->coords[0],
            a->coords[1],
            b->coords[0], b->coords[1]);
    status = 1;
  }
  if (a->num_islands != b->num_islands) {
    fprintf(stderr, "NUM_ISLANDS FAILURE: %d != %d\n", a->num_islands,
            b->num_islands);
    status = 1;
  }
  if (a->num_enemies != b->num_enemies) {
    fprintf(stderr, "NUM_ENEMIES FAILURE: %d != %d\n", a->num_enemies,
            b->num_enemies);
    status = 1;
  }

  TILE chunk_tiles_a[C_WIDTH][C_WIDTH] = {
    [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = OCEAN }
  };
  TILE chunk_tiles_b[C_WIDTH][C_WIDTH] = {
    [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = OCEAN }
  };
  generate_chunk_tileset(chunk_tiles_a, a);
  generate_chunk_tileset(chunk_tiles_b, b);
  for (int j = 0; j < C_WIDTH; j++) {
    for (int k = 0; k < C_WIDTH; k++) {
      if (chunk_tiles_a[j][k] != chunk_tiles_b[j][k]) {
        fprintf(stderr, "TILE FAILURE: (%d, %d) %d != %d\n", j, k,
                chunk_tiles_a[j][k], chunk_tiles_b[j][k]);
        status = 1;
      }
    }
  }

  return status;
}
