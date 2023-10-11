#include <items.h>
#include <stdio.h>
#include <string.h>
/*
                                     ITEMS.c
Defines a global table of items and resources. items.h defines the item and
resource structs, as well as external references to the global item and
resource table, and may be included in any files which reference item/resource
information.

DO NOT INCLUDE THIS FILE ANYWHERE. IF YOU NEED TO REFERENCE item_tab OR
resource_tab, INCLUDE items.h.
*/

// Item table definition
ITEM item_tab[NUM_ITEMS] = {
  /*
  {
    value (int),
    edible (int),
    equippable (int),
    health_mod (float),
    firerate_mod (float),
    speed_mod (float),
    max_heath_mod (float)
  }
  */
  // INVALID_ITEM
  {0, 0, 0, 0.0, 0.0, 0.0, 0.0},

  // COIN
  {1, 0, 0, 0.0, 0.0, 0.0, 0.0},
  
  // CITRUS
  {10, 1, 0, 20.0, 0.0, 0.0, 0.0},
  
  // RUM
  {15, 1, 0, 50.0, -0.2, -0.2, 0.0},
  
  // LIFE_POTION
  {50, 1, 0, 100.0, 0.0, 0.0, 0.0},
  
  // FIRERATE_POTION
  {50, 1, 0, 0.0, 1.0, 0.0, 0.0},
  
  // SPEED_POTION
  {50, 1, 0, 0.0, 0.0, 1.0, 0.0},
  
  // KNIVE
  {80, 0, 1, 0.0, 0.8, 0.0, 0.0},
  
  // SWORD
  {100, 0, 1, 0.0, 0.5, 0.0, 0.0},
  
  // AXE
  {120, 0, 1, 0.0, 0.2, -0.5, 0.0},
  
  // BOW
  {100, 0, 1, 0.0, 0.5, 0.0, 0.0},
  
  // CROSSBOW
  {120, 0, 1, 0.0, 0.2, -0.5, 0.0},
  
  // FLINTLOCK
  {150, 0, 1, 0.0, 0.8, -0.8, 0.0},
  
  // CLOTH_ARMOR
  {50, 0, 1, 0.0, 0.5, 0.5, 50.0},
  
  // LEATHER_ARMOR
  {80, 0, 1, 0.0, 0.2, 0.2, 80.0},
  
  // LIGHT_ARMOR
  {100, 0, 1, 0.0, 0.2, 0.0, 100.0},
  
  // MIDIUM_ARMOR
  {120, 0, 1, 0.0, 0.0, -0.2, 120.0},
  
  // HEAVY_ARMOR
  {150, 0, 1, 0.0, -0.2, -0.2, 150.0},
  
  // PLATE_ARMOR
  {180, 0, 1, 0.0, -0.2, -0.5, 200.0},
};

// Resource table definition
unsigned int resource_tab[NUM_RESOURCES] = {
  0,  // INVALID_REC
  5,  // GRAIN
  10, // SUGAR
  20, // WINE
  15, // CHEESE
  25, // SPICE
  30, // TEA
  8,  // WOOL
  12, // LEATHER
  6,  // COTTON
  40, // SILK
  8,  // DYES
  50, // GOLD_ORE
  30, // SILVER_ORE
  20, // COPPER_ORE
  18, // IRON_ORE
  45, // PEAL
  35, // PORCELAIN
  60, // AMBERGRIS
  28, // HERB
  55  // SAFFRON
};

unsigned int get_resource_value_by_ID(REC_IDS resource_id) {
  // Check for invalid resource_id
  if (resource_id < GRAIN || resource_id > SAFFRON) {
    return 0;
  }
  return resource_tab[resource_id + 1]; // +1 to account for INVALID_REC being -1
}

ITEM get_item_info_by_ID(ITEM_IDS item_id) {
  // Check for invalid resource_id
  if (item_id < COIN || item_id > PLATE_ARMOR) {
    return item_tab[0];
  }
  return item_tab[item_id + 1]; // +1 to account for INVALID_ITEM being -1
}

char * get_item_name_by_ID(unsigned int item_id) {
  if (item_id < 0 || item_id > 17) {
    return NULL;
  }
  switch (item_id) {
    case 0: return "COIN";
    case 1: return "CITRUS";
    case 2: return "RUM";
    case 3: return "LIFE_POTION";
    case 4: return "FIRERATE_POTION";
    case 5: return "SPEED_POTION";
    case 6: return "KNIVE";
    case 7: return "SWROD";
    case 8: return "AXE";
    case 9: return "BOW";
    case 10: return "CROSSBOW";
    case 11: return "FLINTLOCK";
    case 12: return "CLOTH_ARMOR";
    case 13: return "LEATHER_ARMOR";
    case 14: return "LIGHT_ARMOR";
    case 15: return "MIDIUM_ARMOR";
    case 16: return "HEAVY_ARMOR";
    case 17: return "PLATE_ARMOR";
    default: return "INVALID_ITEM";
  }
}

ITEM get_item_info_by_name(char * item_name) {
  if (item_name) {
    if (strcmp(item_name, "COIN") == 0) return get_item_info_by_ID(COIN);
    else if (strcmp(item_name, "CITRUS") == 0) return get_item_info_by_ID(CITRUS);
    else if (strcmp(item_name, "RUM") == 0) return get_item_info_by_ID(RUM);
    else if (strcmp(item_name, "LIFE_POTION") == 0) return get_item_info_by_ID(LIFE_POTION);
    else if (strcmp(item_name, "FIRERATE_POTION") == 0) return get_item_info_by_ID(FIRERATE_POTION);
    else if (strcmp(item_name, "SPEED_POTION") == 0) return get_item_info_by_ID(SPEED_POTION);
    else if (strcmp(item_name, "KNIVE") == 0) return get_item_info_by_ID(KNIVE);
    else if (strcmp(item_name, "SWROD") == 0) return get_item_info_by_ID(SWROD);
    else if (strcmp(item_name, "AXE") == 0) return get_item_info_by_ID(AXE);
    else if (strcmp(item_name, "BOW") == 0) return get_item_info_by_ID(BOW);
    else if (strcmp(item_name, "CROSSBOW") == 0) return get_item_info_by_ID(CROSSBOW);
    else if (strcmp(item_name, "FLINTLOCK") == 0) return get_item_info_by_ID(FLINTLOCK);
    else if (strcmp(item_name, "CLOTH_ARMOR") == 0) return get_item_info_by_ID(CLOTH_ARMOR);
    else if (strcmp(item_name, "LEATHER_ARMOR") == 0) return get_item_info_by_ID(LEATHER_ARMOR);
    else if (strcmp(item_name, "LIGHT_ARMOR") == 0) return get_item_info_by_ID(LIGHT_ARMOR);
    else if (strcmp(item_name, "MIDIUM_ARMOR") == 0) return get_item_info_by_ID(MIDIUM_ARMOR);
    else if (strcmp(item_name, "HEAVY_ARMOR") == 0) return get_item_info_by_ID(HEAVY_ARMOR);
    else if (strcmp(item_name, "PLATE_ARMOR") == 0) return get_item_info_by_ID(PLATE_ARMOR);
  }
  return get_item_info_by_ID(INVALID_ITEM);
}
