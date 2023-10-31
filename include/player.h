#include <items.h>
#include <globals.h>

I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index);
I_SLOT * get_player_first_empty_inventory_slot();
void swap_inventory_slot(I_SLOT * slot_a, I_SLOT * slot_b);
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
int get_player_gold();
int get_player_silver();
int get_player_copper();
unsigned int get_num_empty_slots(I_SLOT *, unsigned int);
