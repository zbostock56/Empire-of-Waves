#include <test.h>

int main() {
 CHUNK chunk;
  double merchants = 0.0;
  double num_islands = 0.0;
  for (int i = 0; i < 1; i++) {
    generate_chunk(&chunk);
    for (int i = 0; i < chunk.num_islands; i++) {
      num_islands += 1.0;
      if (chunk.islands[i].has_merchant)
        merchants += 1.0;
    }
  }
  printf("Number of islands %f\n", num_islands);
  printf("Merchant percentage %f\n", merchants / num_islands);
  RGB_data buffer[C_WIDTH][C_WIDTH];
  /* Set all tiles to open water */
  for (int i = 0; i < C_WIDTH; i++) {
    for (int j = 0; j < C_WIDTH; j++) {
      buffer[i][j].r = buffer[i][j].g = 0;
      buffer[i][j].b = 255;
    }
  }
  for (int i = 0; i < (int) num_islands; i++) {
    ISLAND island = chunk.islands[i];
    for (int i = 0; i < I_WIDTH * I_WIDTH; i++) {
      int yloc = (i / I_WIDTH) + island.coords[1];
      int xloc = (i % I_WIDTH) + island.coords[0];
      if (island.tiles[i] == ROCK) {
        // ROCK
        buffer[xloc][yloc].r = 90;
        buffer[xloc][yloc].g = 77;
        buffer[xloc][yloc].b = 65;
      } else if (island.tiles[i] == GRASS) {
        // HIGHLAND GRASS
        buffer[xloc][yloc].r = 124;
        buffer[xloc][yloc].g = 252;
        buffer[xloc][yloc].b = 0;
      } else if (island.tiles[i] == SAND) {
        // SAND
        buffer[xloc][yloc].r = 194;
        buffer[xloc][yloc].g = 178;
        buffer[xloc][yloc].b = 128;
      } else if (island.tiles[i] == SHORE) {
        // SHORE
        buffer[xloc][yloc].r = 0;
        buffer[xloc][yloc].g = 255;
        buffer[xloc][yloc].b = 255;
      } else if (island.tiles[i] == MERCH) {
        // SHORE
        buffer[xloc][yloc].r = 255;
        buffer[xloc][yloc].g = 0;
        buffer[xloc][yloc].b = 0;
      } else {
        // DEEP WATER
        buffer[xloc][yloc].r = 0;
        buffer[xloc][yloc].g = 0;
        buffer[xloc][yloc].b = 255;
      }
    }
  }
  bmp_generator("output.bmp", C_WIDTH, C_WIDTH, (BYTE *) buffer);
  return 0;
}
