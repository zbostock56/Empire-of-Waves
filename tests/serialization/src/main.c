#include <m.h>

void generate_chunk_tileset(TILE chunk_tiles[C_WIDTH][C_WIDTH], CHUNK chunk) {
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
      chunk_tiles[chunkx][chunky] = island.tiles[j];
    }
  }
}

int main() {
  GLFWwindow *window = init_gl();

  init_chunks();
  init_scene();
  int ran_once = 0;

  while (!glfwWindowShouldClose(window)) {
    keyboard_input(window);

    // Insert all simulation logic here:
    // - collision detection
    // - pathfinding
    // - chunk serialization
    // - etc...
    if (!ran_once) {
      manage_chunks();
      ran_once = 1;
    } else {
      TILE chunk_tiles_before[C_WIDTH][C_WIDTH] = {
        [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = OCEAN }
      };

      TILE chunk_tiles_after[C_WIDTH][C_WIDTH] = {
        [0 ... C_WIDTH - 1] = { [0 ... C_WIDTH - 1] = OCEAN }
      };

      CHUNK chunk;
      CHUNK temp;
      printf("STARTING TEST\n");

      for (int i = 0; i < 9; i++) {
        fprintf(stderr, "Testing chunk %d\n", i);
        chunk = player_chunks[i];
        generate_chunk_tileset(chunk_tiles_before, chunk);
        save_chunk(&chunk);
        load_chunk(chunk.coords, &temp);
        generate_chunk_tileset(chunk_tiles_after, temp);
        for (int j = 0; j < C_WIDTH; j++) {
          for (int k = 0; k < C_WIDTH; k++) {
            if (chunk_tiles_before[j][k] != chunk_tiles_after[j][k]) {
              fprintf(stderr, "FAIL\n");
              exit(1);
            }
          }
        }
        fprintf(stderr, "Chunk %d passed\n", i);
      }
      printf("TEST CASES PASSED\n");
      exit(0);
    }

    render_scene(window);
    //update_event_timer();
  }

  // Insert all "cleanup" functionality here
  cleanup_scene();

  glfwTerminate();
}
