#include <player.h>

void init_player() {
  glm_vec2_zero(e_player.coords);
  glm_ivec2_zero(e_player.chunk);
  glm_vec2_zero(e_player.ship_coords);
  glm_ivec2_zero(e_player.ship_chunk);
  glm_vec2_zero(e_player.direction);
  glm_vec2_zero(e_player.ship_direction);
  e_player.hunger = PLAYER_BASE_HUNGER;
  e_player.max_health = PLAYER_BASE_HEALTH;
  e_player.health = PLAYER_BASE_HEALTH;
  e_player.speed = 1.0;
  e_player.direction[1] = 1.0;
  e_player.ship_direction[1] = 1.0;
  e_player.embarked = 1;
  e_player.moving = 0;
  e_player.reputation = 0.0;
  e_player.inventory[0].item_id = RUM;
  e_player.inventory[0].quantity = 2;
  e_player.inventory[1].item_id = KNIFE;
  e_player.inventory[1].quantity = 1;
  e_player.inventory[2].item_id = GOLD_COIN;
  e_player.inventory[2].quantity = COIN_STACK_LIMIT;
  e_player.inventory[3].item_id = SILVER_COIN;
  e_player.inventory[3].quantity = 1;
  e_player.inventory[4].item_id = COPPER_COIN;
  e_player.inventory[4].quantity = 50;
  e_player.inventory[5].item_id = CROSSBOW;
  e_player.inventory[5].quantity = 1;
  e_player.inventory[6].item_id = COPPER_COIN;
  e_player.inventory[6].quantity = 1;
  e_player.inventory[7].item_id = HEAVY_ARMOR;
  e_player.inventory[7].quantity = 1;
  e_player.inventory[8].item_id = PLATE_ARMOR;
  e_player.inventory[8].quantity = 1;
  e_player.inventory[9].item_id = FIRERATE_POTION;
  e_player.inventory[9].quantity = 1;
  e_player.inventory[10].item_id = LIFE_POTION;
  e_player.inventory[10].quantity = 5;
  e_player.inventory[11].item_id = RUM;
  e_player.inventory[11].quantity = STACK_LIMIT;
  e_player.inventory[12].item_id = GOLD_COIN;
  e_player.inventory[12].quantity = 50;

  t.slot.item_id = EMPTY;
  t.slot.quantity = -1;

  equipment.weapon_type = MELEE;
  equipment.weapon_equipped = EMPTY;
  equipment.armor_equipped = EMPTY;

  /* combat mode initilization */
  c_player.max_health = PLAYER_BASE_HEALTH;
  c_player.health = PLAYER_BASE_HEALTH;
  e_player.hunger = PLAYER_BASE_HUNGER;
  c_player.fire_rate = 0.5;
  c_player.speed = 15.0;
}

// Reset player inventory, ship merchants, and position back to the home island
void respawn_player() {
  glm_ivec2_zero(e_player.chunk);
  glm_ivec2_zero(e_player.ship_chunk);

  int home_chunk = add_chunk((ivec2) {0, 0});
  find_shore_tile(chunk_buffer[home_chunk].islands, e_player.ship_coords);
  glm_vec2_copy(e_player.ship_coords, e_player.coords);
  remove_chunk(home_chunk);

  unsigned int num_ts = num_trade_ships;
  for (unsigned int i = 0; i < num_ts; i++) {
    delete_trade_ship(0);
  }

  e_player.embarked = 1;
  e_player.ship_mercenaries = 0;
  e_player.total_mercenaries = 0;
  for (unsigned int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    e_player.inventory[i].item_id = EMPTY;
    e_player.inventory[i].quantity = 0;
  }
  equipment.weapon_type = MELEE;
  equipment.weapon_equipped = EMPTY;
  equipment.armor_equipped = EMPTY;

  reset_buff_list();

  c_player.max_health = PLAYER_BASE_HEALTH;
  c_player.health = PLAYER_BASE_HEALTH;
  e_player.hunger = PLAYER_BASE_HUNGER;
  c_player.health = 100;
  c_player.fire_rate = 0.5;
  c_player.speed = 15.0;
}

/*
Get player's inventory slot item by a given unsigned int item_number
Return NULL if not such inventory slot number is invalid
Args:
unsigned int item_number
  item number of the inventory, start with zero
*/
I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index) {
  if (item_index > 0 || item_index < MAX_PLAYER_INV_SIZE) { // Argument validate
    return &e_player.inventory[item_index];
  }
  return NULL;
}

/*
Get player's first empty inventory slot and return a I_SLOT pointer
Return NULL if not found such empty slot
*/
I_SLOT * get_player_first_empty_inventory_slot() {
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if ((e_player.inventory[i].item_id == 0) ||
         e_player.inventory[i].quantity == 0) {
      e_player.inventory[i].quantity = 0;
      return &e_player.inventory[i];
    }
  }
  return NULL;
}

/*
  Gets player's first inventory slot that matches with the requested
  item type that already has items in the slot
*/
I_SLOT *get_requested_inventory_slot_type(ITEM_IDS item) {
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == item &&
        e_player.inventory[i].quantity > 0) {
      if ((item == GOLD_COIN || item == SILVER_COIN ||
          item == COPPER_COIN) &&
          e_player.inventory[i].quantity < COIN_STACK_LIMIT) {
          return e_player.inventory + i;
      } else {
        if (e_player.inventory[i].quantity < STACK_LIMIT) {
          return e_player.inventory + i;
        }
      }
    }
  }
  return NULL;
}

int are_inventory_slots_open() {
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == EMPTY) {
      return 1;
    }
  }
  return 0;
}

/*
Swap two slot
Args:
I_SLOT * slot_a
I_SLOT * slot_b
  two slot being swapped
*/
void swap_inventory_slot(I_SLOT * slot_a, I_SLOT * slot_b) {
  if (slot_a && slot_b) {
    I_SLOT temp = {
      slot_a->item_id,
      slot_a->quantity
    };
    slot_a->item_id = slot_b->item_id;
    slot_a->quantity = slot_b->quantity;
    slot_b->item_id = temp.item_id;
    slot_b->quantity = temp.quantity;
  }
}

/*
Find first item slot that is item_id of the player's inventory
Return NULL when not find
Args:
ITEM_IDS item_id
  the item that you want to search
*/
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id) {
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == item_id) {
      return &e_player.inventory[i];
    }
  }
  return NULL;
}

void remove_money(I_SLOT *items, unsigned int num_items) {
  I_SLOT *cur_item = NULL;
  for (int i = 0; i < num_items; i++) {
    cur_item = items + i;
    if (cur_item->item_id == GOLD_COIN || cur_item->item_id == SILVER_COIN ||
        cur_item->item_id == COPPER_COIN) {
      cur_item->item_id = EMPTY;
      cur_item->quantity = 0;
    }
  }
}

/*
Return the gold coin player owns
*/
int get_player_gold() {
  int num_gold = 0;
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == GOLD_COIN) {
      num_gold += get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return num_gold;
}

/*
Return the silver coin player owns
*/
int get_player_silver() {
  int num_silver = 0;
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == SILVER_COIN) {
      num_silver += get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return num_silver;
}

/*
Return the copper coin player owns
*/
int get_player_copper() {
  int num_copper = 0;
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == COPPER_COIN) {
      num_copper += get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return num_copper;
}

/*
  General function to give the player money,
  mostly used when a trade ship finishes its route
  to reward the player
*/
void give_player_copper(unsigned int amount) {
  I_SLOT *slot;
  /* Check if the player already has copper coins */
  if (!get_player_copper()) {
    /* Player has no copper, make an inventory slot */
    /* and give the player the requested amount     */
    slot = get_player_first_empty_inventory_slot();
    slot->item_id = COPPER_COIN;
    slot->quantity = amount;
  } else {
    /* Find the slot which corresponds to the copper */
    /* coin and add the amount requested             */
    slot = search_player_inventory_by_ID(COPPER_COIN);
    if (slot) {
      slot->quantity += amount;
    } else {
      fprintf(stderr, "player.c (give_player_copper): MISSING COPPER IN INVENTORY\n");
      exit(1);
    }
  }
}

unsigned int get_player_money() {
  unsigned int money = 0;
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    I_SLOT *slot = get_player_inventory_slot_by_index(i);
    if (slot->item_id == GOLD_COIN) {
      money += (SILVER_PER_GOLD * COPPER_PER_SILVER * slot->quantity);
    }
    if (slot->item_id == SILVER_COIN) {
      money += (COPPER_PER_SILVER * slot->quantity);
    }
    if (slot->item_id == COPPER_COIN) {
      money += slot->quantity;
    }
  }
  return money;
}

/*
  Perform calculation to automatically coalesce coins into higher
  denomnations given there is enought of them.
  Ex: 10 copper becomes 1 silver
*/
void coalesce_currency(I_SLOT *items, unsigned int num_items) {
  unsigned int num_copper = 0;
  unsigned int num_silver = 0;
  unsigned int num_gold = 0;
  for (int i = 0; i < num_items; i++) {
    I_SLOT *slot = items + i;
    if (slot->item_id == GOLD_COIN) {
      num_gold += slot->quantity;
      slot->item_id = EMPTY;
      slot->quantity = 0;
    } else if (slot->item_id == SILVER_COIN) {
      num_silver += slot->quantity;
      slot->item_id = EMPTY;
      slot->quantity = 0;
    } else if (slot->item_id == COPPER_COIN) {
      num_copper += slot->quantity;
      slot->item_id = EMPTY;
      slot->quantity = 0;
    }
  }

  if (!num_copper && !num_silver && !num_gold) {
    return;
  }

  num_silver += num_copper / COPPER_PER_SILVER;
  num_copper = num_copper % COPPER_PER_SILVER;
  num_gold += num_silver / SILVER_PER_GOLD;
  num_silver = num_silver % SILVER_PER_GOLD;

  unsigned int num_stacks_gold = num_gold / COIN_STACK_LIMIT;
  if (num_gold % COIN_STACK_LIMIT) {
    num_stacks_gold++;
  }
  unsigned int num_stacks_silver = num_silver / COIN_STACK_LIMIT;
  if (num_silver % COIN_STACK_LIMIT) {
    num_stacks_silver++;
  }
  unsigned int num_stacks_copper = num_copper / COIN_STACK_LIMIT;
  if (num_copper % COIN_STACK_LIMIT) {
    num_stacks_copper++;
  }

  while (num_stacks_gold + num_stacks_silver + num_stacks_copper) {
    I_SLOT *slot = get_player_first_empty_inventory_slot();
    if (!slot) {
      break;
    }

    if (num_stacks_gold) {
      slot->item_id = GOLD_COIN;
      if (num_gold >= COIN_STACK_LIMIT) {
        slot->quantity = COIN_STACK_LIMIT;
      } else {
        slot->quantity = num_gold;
      }
      num_gold -= slot->quantity;
      num_stacks_gold--;
    } else if (num_stacks_silver) {
      slot->item_id = SILVER_COIN;
      if (num_silver >= COIN_STACK_LIMIT) {
        slot->quantity = COIN_STACK_LIMIT;
      } else {
        slot->quantity = num_silver;
      }
      num_silver -= slot->quantity;
      num_stacks_silver--;
    } else if (num_stacks_copper) {
      slot->item_id = COPPER_COIN;
      if (num_copper >= COIN_STACK_LIMIT) {
        slot->quantity = COIN_STACK_LIMIT;
      } else {
        slot->quantity = num_copper;
      }
      num_copper -= slot->quantity;
      num_stacks_copper--;
    } else {
      break;
    }
  }
}

unsigned int get_num_empty_slots(I_SLOT *items, unsigned int num_items) {
  unsigned int count = 0;
  for (int i = 0; i < num_items; i++) {
    if (items[i].item_id == EMPTY) {
      count++;
    }
  }
  return count;
}

/*
  Returns the number of slots in a list which are currency items
  (GOLD_COIN, SILVER_COIN, COPPER_COIN)
*/
unsigned int num_currency_slots(I_SLOT *items, unsigned int num_items) {
  unsigned int count = 0;
  for (int i = 0; i < num_items; i++) {
    if (items[i].item_id == GOLD_COIN ||
        items[i].item_id == SILVER_COIN ||
        items[i].item_id == COPPER_COIN) {
      count++;
    }
  }
  return count;
}

/*
  Returns the number of items slots the currency in an item list would take up
  if the currency were coalesced
  Ex:
    "items" has 111 copper coins
    Returns value would be 3:
    - 1 gold coin   (100) +
    - 1 silver coin  (10) +
    - 1 copper coin   (1)
*/
unsigned int num_currency_slots_coalesced(I_SLOT *items,
                                          unsigned int num_items) {
  unsigned int num_copper = 0;
  unsigned int num_silver = 0;
  unsigned int num_gold = 0;
  for (int i = 0; i < num_items; i++) {
    I_SLOT *slot = items + i;
    if (slot->item_id == GOLD_COIN) {
      num_gold += slot->quantity;
    } else if (slot->item_id == SILVER_COIN) {
      num_silver += slot->quantity;
    } else if (slot->item_id == COPPER_COIN) {
      num_copper += slot->quantity;
    }
  }

  num_silver += num_copper / COPPER_PER_SILVER;
  num_copper = num_copper % COPPER_PER_SILVER;
  num_gold += num_silver / SILVER_PER_GOLD;
  num_silver = num_silver % SILVER_PER_GOLD;

  unsigned int num_coins = 0;
  if (num_gold) {
    num_coins++;
  }
  if (num_silver) {
    num_coins++;
  }
  if (num_copper) {
    num_coins++;
  }
  return num_coins;
}

unsigned int check_fit(LISTING *from, int *from_selected,
                       unsigned int from_len, I_SLOT *to, int *to_selected,
                       unsigned int to_len) {
  unsigned int num_new = 0;
  LISTING *cur_listing = 0;
  I_SLOT *cur_slot = 0;
  for (int i = 0; i < from_len; i++) {
    cur_listing = from + i;
    if (from_selected[i]) {
      num_new++;
      for (int j = 0; j < to_len; j++) {
        cur_slot = to + j;
        if (cur_listing->item_id == cur_slot->item_id) {
          num_new--;
          break;
        }
      }
    }
  }

  unsigned int num_empty = get_num_empty_slots(to, to_len);
  if (num_new <= num_empty) {
    return 1;
  }
  return 0;
}

void set_hunger_level(float level) {
  e_player.hunger = level;
}

void set_health(float health) {
  e_player.health = health;
  c_player.health = health;
}
