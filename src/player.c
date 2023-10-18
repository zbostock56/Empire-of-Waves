#include <player.h>

I_SLOT * get_player_inventory_slot_by_number(unsigned int item_number) {
  if (item_number > 0 || item_number < INV_SIZE) { // Argument validate
    return &e_player.inventory[item_number - 1];
  }
  return NULL;
}

I_SLOT * get_player_first_empty_inventory_slot() {
  for (int i = 0; i < INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == 0 || e_player.inventory[i].quantity == 0) {
      return &e_player.inventory[i];
    }
  }
  return NULL;
}

void swap_inventory_slot(I_SLOT * slot_a, I_SLOT * slot_b) {
  if (slot_a && slot_b) {
    I_SLOT temp = {slot_a->item_id, slot_a->quantity};
    slot_a->item_id = slot_b->item_id;
    slot_a->quantity = slot_b->quantity;
    slot_b->item_id = temp.item_id;
    slot_b->quantity = temp.quantity;
  }
}

I_SLOT * search_player_inventory_with_ID(ITEM_IDS item_id) {
  for (int i = 0; i < INV_SIZE; i++) {
    if (e_player.inventory[i].item_id == item_id) {
      return &e_player.inventory[i];
    }
  }
  return NULL;
}
