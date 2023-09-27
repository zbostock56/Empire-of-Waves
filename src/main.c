#include <main.h>

int main() {
  CHUNK chunk;
  double merchants = 0.0;
  double num_islands = 0.0;
  for (int i = 0; i < 9; i++) {
    generate_chunk(&chunk);
    for (int i = 0; i < chunk.num_islands; i++) {
      num_islands += 1.0;
      if (chunk.islands[i].has_merchant)
        merchants += 1.0;
    }
  }
  printf("Merchant percentage %f\n", merchants / num_islands);
  return 0;
}
