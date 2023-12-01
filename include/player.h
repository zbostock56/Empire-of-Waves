#include <stdio.h>
#include <items.h>
#include <globals.h>
#include <equipment_str.h>


#define PLAYER_BASE_HEALTH (100.0)
#define PLAYER_BASE_HUNGER (100.0)

EQUIPMENT equipment;
extern TEMP_ITEM t;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index);
I_SLOT * get_player_first_empty_inventory_slot();
I_SLOT *get_requested_inventory_slot(ITEM_IDS);
void swap_inventory_slot(I_SLOT * slot_a, I_SLOT * slot_b);
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
int get_player_gold();
int get_player_silver();
int get_player_copper();
void give_player_copper(unsigned int);
unsigned int get_num_empty_slots(I_SLOT *, unsigned int);
I_SLOT *get_requested_inventory_slot(ITEM_IDS);
int are_inventory_slots_open();
void set_hunger_level(float);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

int add_chunk(ivec2);
int remove_chunk(unsigned int);
void find_shore_tile(ISLAND *, vec2);
void delete_trade_ship(unsigned int);
int are_inventory_slots_open();
void reset_buff_list();
