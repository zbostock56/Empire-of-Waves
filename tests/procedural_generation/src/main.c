#include <test.h>

int main() {
  ISLAND island;
  generate_island(&island);
  RGB_data buffer[I_WIDTH][I_WIDTH];
  for (int i = 0; i < I_WIDTH * I_WIDTH; i++) {
    int yloc = i / I_WIDTH;
    int xloc = i % I_WIDTH;
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
    } else {
      // DEEP WATER
      buffer[xloc][yloc].r = 0;
      buffer[xloc][yloc].g = 0;
      buffer[xloc][yloc].b = 255;
    }
  }
  bmp_generator("output.bmp", I_WIDTH, I_WIDTH, (BYTE *) buffer);
  return 0;
}
