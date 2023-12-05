#include <merchant.h>

/*
                                   MERCHANT.c
Implements the functionality for enabling/disabling merchant prompts/menus,
modifying merchant relationships, and conducting trade.
*/

/*
Get the merchant listing item by given int number
Args:
MERCHANT * merchant
  The merchant that you need a listing
unsigned int item_number
  The item_number in the listing array, start with index zero
*/
LISTING *get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int listing_index) {
  if (merchant) {
    if (listing_index >= 0 && listing_index < merchant->num_listings) {
      if (merchant->listings) {
        return &merchant->listings[listing_index];
      }
    }
  }
  return NULL;
}

/*
Find first listing that is item_id of the merchant's listing
Return NULL when not find
Args:
MERCHANT * merchant
  pointer to the merchant
ITEM_IDS item_id
  the item that you want to search
*/
LISTING * search_merchant_listing_by_ID(MERCHANT * merchant, ITEM_IDS item_id) {
  for (int i = 0; i < merchant->num_listings; i++) {
    if (merchant->listings[i].item_id == item_id && merchant->listings[i].quantity > 0) {
      return &merchant->listings[i];
    }
  }
  return NULL;
}

/*
Get merchant's first empty listing and return a LISTING pointer
Return NULL if not found such empty listing
Args:
MERCHANT * merchant
  pointer to the merchant
*/
LISTING * get_merchant_first_empty_listing(MERCHANT * merchant) {
  for (int i = 0; i < merchant->listings_buf_size; i++) {
    if (merchant->listings[i].item_id < 1) {
      return &merchant->listings[i];
    }
  }
  return NULL;
}

char *get_merchant_name(short index) {
  return merchant_name_list[index];
}

/*
  Helper function for resizing merchant listings
*/
int resize_listings(MERCHANT *merchant) {
  unsigned int old_size = merchant->listings_buf_size;
  int status = double_buffer((void **) &merchant->listings,
                             &merchant->listings_buf_size,
                             sizeof(LISTING));
  if (status) {
    return -1;
  }

  for (int i = old_size; i < merchant->listings_buf_size; i++) {
    merchant->listings[i].item_id = EMPTY;
    merchant->listings[i].quantity = 0;
  }

  return 0;
}

void add_listing(MERCHANT *merchant, int **selected_buffer, ITEM_IDS item,
                 unsigned int quantity) {
  LISTING *exists = search_merchant_listing_by_ID(merchant, item);
  if (exists) {
    exists->quantity += quantity;
    return;
  } else {
    merchant->listings[merchant->num_listings].item_id = item;
    merchant->listings[merchant->num_listings].quantity = quantity;
    merchant->num_listings++;

    if (merchant->num_listings == merchant->listings_buf_size) {
      unsigned int selected_buf_size = merchant->listings_buf_size;
      int status = double_buffer((void **) merchant->listings,
                                 &merchant->listings_buf_size,
                                 sizeof(LISTING));
      if (status) {
        fprintf(stderr, "merchant.c: Unable to resize listings buffer\n");
        exit(1);
      }
      status = double_buffer((void **) selected_buffer, &selected_buf_size,
                             sizeof(int));
      if (status) {
        fprintf(stderr, "merchant.c: Unable to resize selected buffer\n");
        exit(1);
      }
    }
  }
}

/*
  Clears a given listing index to EMPTY and replaces it with the last item
  in the listing buffer
*/
void clear_listing(MERCHANT *merchant, unsigned int index) {
  merchant->num_listings--;
  merchant->listings[index] = merchant->listings[merchant->num_listings];
  merchant->listings[merchant->num_listings].item_id = EMPTY;
  merchant->listings[merchant->num_listings].quantity = 0;
}

unsigned int calc_merc_price(MERCHANT *merchant) {
  return ((MAX_MERCENARIES - merchant->num_mercenaries) * 0.25 *
           MERC_BASE_PRICE) + MERC_BASE_PRICE;
}

void update_relationship(MERCHANT *merchant, float delta) {
  merchant->relationship += delta;
  if (merchant->relationship > REL_MAX) {
    merchant->relationship = REL_MAX;
  }
  if (merchant->relationship < REL_MIN) {
    merchant->relationship = REL_MIN;
  }

  e_player.reputation += delta;
  if (e_player.reputation > REP_MAX) {
    e_player.reputation = REP_MAX;
  }
  if (e_player.reputation < REP_MIN) {
    e_player.reputation = REP_MIN;
  }
}

// Spawn 3 enemy ships around the player's home island
int invade_home_island() {
  if (e_player.reputation > INVASION_REP) {
    return 0;
  }

  CHUNK *home_chunk = chunk_buffer + home_chunk_index;
  ISLAND *home_island = home_chunk->islands + HOME_ISLAND_INDEX;

  vec2 spawn_tile = GLM_VEC2_ZERO_INIT;
  unsigned int island_side = 0;
  unsigned int side_tile = 0;
  for (int i = 0; i < 3; i++) {
    // Determine which side of the island the enemy will spawn on
    island_side = rand() % 4;
    // Determine which tile on the selected island side the enemy will spawn on
    side_tile = rand() % I_WIDTH;
    spawn_tile[0] = home_island->coords[0];
    spawn_tile[1] = home_island->coords[1];

    if (island_side == TOP) {
      spawn_tile[0] += side_tile;
    } else if (island_side == BOTTOM) {
      spawn_tile[0] += side_tile;
      spawn_tile[1] += I_WIDTH - 1;
    } else if (island_side == LEFT) {
      spawn_tile[1] += side_tile;
    } else {
      spawn_tile[0] += I_WIDTH - 1;
      spawn_tile[1] += side_tile;
    }

    init_enemy(home_chunk->enemies + home_chunk->num_enemies,
               home_chunk->coords, spawn_tile);
    home_chunk->num_enemies++;
    if (home_chunk->num_enemies == home_chunk->enemy_buf_size) {
      int status = double_buffer((void **) &home_chunk->enemies,
                                 &home_chunk->enemy_buf_size, sizeof(E_ENEMY));
      if (status) {
        fprintf(stderr, "merchant.c: Failed to reallocate enemy buffer\n");
        return -1;
      }
    }
  }
  return 0;
}
