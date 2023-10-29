#include <player.h>
#include <globals.h>

/*
Get player's inventory slot item by a given unsigned int item_number
Return NULL if not such inventory slot number is invalid
Args:
unsigned int item_number
  item number of the inventory, start with zero
*/
I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index) {
  if (item_index > 0 || item_index < MAX_PLAYERT_INV_SIZE) { // Argument validate
    return &e_player.inventory[item_index];
  }
  return NULL;
}

/*
Get player's first empty inventory slot and return a I_SLOT pointer
Return NULL if not found such empty slot
*/
I_SLOT * get_player_first_empty_inventory_slot() {
  for (int i = 0; i < MAX_PLAYERT_INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == 0 || e_player.inventory[i].quantity == 0) {
      return &e_player.inventory[i];
    }
  }
  return NULL;
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
    I_SLOT temp = {slot_a->item_id, slot_a->quantity};
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
  for (int i = 0; i < MAX_PLAYERT_INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == item_id) {
      return &e_player.inventory[i];
    }
  }
  return NULL;
}

/*
Return the gold coin player owns
*/
int get_player_gold() {
  // return e_player.money % 100;
  for (int i = 0; i < MAX_PLAYERT_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == GOLD_COIN) {
      return get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return 0;
}

/*
Return the silver coin player owns
*/
int get_player_silver() {
  // return e_player.money % 10;
  for (int i = 0; i < MAX_PLAYERT_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == SILVER_COIN) {
      return get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return 0;
}

/*
Return the copper coin player owns
*/
int get_player_copper() {
  // return e_player.money;
  for (int i = 0; i < MAX_PLAYERT_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id == COPPER_COIN) {
      return get_player_inventory_slot_by_index(i)->quantity;
    }
  }
  return 0;
}

