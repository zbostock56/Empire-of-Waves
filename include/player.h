#include <player_str.h>
#include <items.h>

I_SLOT * get_player_inventory_slot_by_number(unsigned int item_number);
I_SLOT * get_player_first_empty_inventory_slot();
void swap_inventory_slot(I_SLOT * slot_a, I_SLOT * slot_b);
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
