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
int is_dropping = 0;
int inventory_open = 0;
extern int holding_drop;
extern int holding_shift;
TEMP_ITEM t;

#define place_item(s) {\
  s->item_id = t.slot.item_id; \
  s->quantity += t.slot.quantity; \
  t.slot.item_id = EMPTY; \
  t.slot.quantity = -1; \
}

#define grab_item(s) {\
  t.slot.item_id = s->item_id; \
  t.slot.quantity = s->quantity; \
  s->item_id = EMPTY; \
  s->quantity = 0; \
}

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
int drop_item(ITEM_IDS item_ids);

/* ==================== EXTERNALLY DEFINED FUNCTIONS ================== */
void increment_buff(void*, void*);
I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index);
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
int num_items_on_island(ISLAND *);
int find_first_avail_item_slot(ISLAND *);
int find_item_slot_specific_loc(ISLAND *, vec2);
void set_prompt(const char *);
