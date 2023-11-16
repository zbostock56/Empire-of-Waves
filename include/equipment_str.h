#ifndef __EQUIPMENT_STR_H__
#define __EQUIPMENT_STR_H__

#include <ui_component.h>
#include <player_str.h>

typedef struct equipment {
  WEAPON_T weapon_type;
  ITEM_IDS weapon_equipped;
  ITEM_IDS armor_equipped;
} EQUIPMENT;

#endif
