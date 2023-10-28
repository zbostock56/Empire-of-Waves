#include <helpers.h>
/*
                        HELPERS.C
  Gives helper functions for testing out different modules
*/

void print_islands(CHUNK chunk) {
  ISLAND island;
  printf("ISLANDS FOR: chunk %d | %d\n", chunk.coords[0], chunk.coords[1]);
  printf("Number of islands: %d\n", chunk.num_islands);
  for (int i = 0; i < chunk.num_islands; i++) {
    island = chunk.islands[i];
    char *tf = island.has_merchant ? "TRUE" : "FALSE";
    printf("ISLAND %d\nCoords: %d | %d\nmerchant? %s\n",
            i, island.coords[0], island.coords[1], tf);
  }
}
