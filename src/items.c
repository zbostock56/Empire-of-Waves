#include <items.h>
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
