

#include <merchant.h>

/*
                                   MERCHANT.c
Implements the functionality for enabling/disabling merchant prompts/menus,
modifying merchant relationships, and conducting trade.
*/


// #23 - As a user I would like to talk to merchants

MERCHANT * get_closest_merchant(E_PLAYER e_player) {
  float closest_merchant_distance = 1e9+7;
  MERCHANT * closest_merchant = NULL;
  if (e_player.embarked == 0) {
    for (int i = 0; i < 9; i++) {
      CHUNK *cur_chunk = chunk_buffer + player_chunks[i];
      for (int j = 0; j < cur_chunk->num_islands; j++) { // For each island inside player's chunk
        if (cur_chunk->islands[j].has_merchant) { // Check if island has merchant
          // YES - island has merchant
          if (abs(glm_vec2_norm(cur_chunk->islands[j].merchant.coords)) -
              abs(glm_vec2_norm(e_player.coords)) < closest_merchant_distance) { // Check if the merchant is closer
            // YES - the merchant is closer
            closest_merchant_distance = abs(cur_chunk->islands[j].merchant.coords - e_player.coords); // Update closest distance
            closest_merchant = &cur_chunk->islands[j].merchant; // Update closest merchant
          }
        }
      }
    }
  }
  return closest_merchant;
}

LISTING *get_merchant_listing_item_by_number(MERCHANT * merchant, unsigned int item_number) {
  if (merchant) { // Argument validate
    if (item_number > 0 || item_number < merchant->num_listings) { // Argument validate
      if (merchant->listings) {
        return &merchant->listings[item_number - 1];
      }
    }
  }
  return NULL;
}
