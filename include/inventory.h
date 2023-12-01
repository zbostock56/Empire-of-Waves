#include <inventory_str.h>
#include <ui_component.h>
#include <player_str.h>
#include <equipment_str.h>
#include <menu.h>
#include <stdio.h>
#include <globals.h>
#include <island.h>

/*
                                   INVENTORY
Implements the functionality for player inventory.
*/

/* GLOBALS */
INVENTORY inventory;
extern EQUIPMENT equipment;
float time_inventory_event_prompt;
int isDropping;
int inventory_open = 0;

/* ==================== INTERNALLY DEFINED FUNCITONS ================== */
int init_inventory_ui();
void free_inventory_ui();
void open_inventory_ui();
void close_inventory_ui();
void update_inventory_ui();
void on_click_inventory_item(void *inventory_item_index);
void on_hover_inventory_item(void *inventory_item_index);
void on_click_weapon();
void on_hover_weapon();
void on_click_armor();
void on_hover_armor();
void taken_off_weapon();
void taken_off_armor();
void equip_slot_item(I_SLOT *i_slot);
void on_click_drop();
void drop_item(ITEM_IDS item_ids);

/* ==================== EXTERNALLY DEFINED FUNCTIONS ================== */
void increment_buff(void*, void*);
I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index);
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
