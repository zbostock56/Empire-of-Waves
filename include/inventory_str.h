#ifndef __INVENTORY_STR_H__
#define __INVENTORY_STR_H__

#include <ui_component.h>
#include <player_str.h>

typedef struct inventory {
  UI_COMPONENT *ui_inventory_items[MAX_PLAYER_INV_SIZE];
  UI_COMPONENT *ui_equipment_weapon;
  UI_COMPONENT *ui_equipment_armor;
  UI_COMPONENT *ui_text_on_hover_item;
  UI_COMPONENT *ui_text_event_prompt;
  UI_COMPONENT *ui_drop;
} INVENTORY;

#endif
