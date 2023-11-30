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
  // EMPTY
  {0, 0, 0, 0.0, 0.0, 0.0, 0.0},
  // CITRUS
  {10, 1, 0, 20.0, 0.0, 0.0, 0.0},
  // RUM
  {15, 1, 0, 50.0, -0.2, -0.2, 0.0},
  // LIFE_POTION
  {50, 1, 0, 100.0, 0.0, 0.0, 0.0},
  // FIRERATE_POTION
  {50, 1, 0, 0.0, 1.0, 0.0, 0.0},
  // SPEED_POTION
  {50, 1, 0, 0.0, 0.0, 10.0, 0.0},
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
  // GOLD_COIN
  {100, 0, 0, 0.0, 0.0, 0.0, 0.0},
  // SILVER_COIN
  {10, 0, 0, 0.0, 0.0, 0.0, 0.0},
  // COPPER_COIN
  {1, 0, 0, 0.0, 0.0, 0.0, 0.0},
  
  /* GRAIN */
  {5, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* COTTON */
  {6, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* WOOL */
  {8, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* DYES */
  {8, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* SUGAR */
  {10, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* LEATHER */
  {15, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* CHEESE */
  {15, 1, 0, 0.0, 0.0, 0.0, 0.0},
  /* IRON_ORE */
  {18, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* WINE */
  {20, 1, 0, 0.0, 0.0, 0.0, 0.0},
  /* COPPER_ORE */
  {20, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* SPICE */  
  {25, 1, 0, 0.0, 0.0, 0.0, 0.0},
  /* HERB */
  {28, 1, 0, 0.0, 0.0, 0.0, 0.0},
  /* TEA */
  {30, 1, 0, 0.0, 0.0, 0.0, 0.0},
  /* SILVER_ORE */
  {30, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* PORCELAIN */
  {35, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* SILK */
  {40, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* PEARL */
  {45, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* GOLD_ORE */
  {50, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* SAFFRON */
  {55, 0, 0, 0.0, 0.0, 0.0, 0.0},
  /* AMBERGRIS */
  {60, 0, 0, 0.0, 0.0, 0.0, 0.0}
};

// Resource table definition
unsigned int resource_tab[NUM_RESOURCES] = {
  0,  // INVALID_REC
  5,  // GRAIN
  6,  // COTTON
  8,  // WOOL
  8,  // DYES
  10, // SUGAR
  12, // LEATHER
  15, // CHEESE
  18, // IRON_ORE
  20, // WINE
  20, // COPPER_ORE
  25, // SPICE
  28, // HERB
  30, // TEA
  30, // SILVER_ORE
  35, // PORCELAIN
  40, // SILK
  45, // PEARL
  50, // GOLD_ORE
  55, // SAFFRON
  60  // AMBERGRIS
};

/*
Get the value of a resources by provide a enum REC_IDS
Args:
REC_IDS resources_id
  the resource ID of the resource value needs to get
*/
unsigned int get_resource_value_by_ID(REC_IDS resource_id) {
  // Check for invalid resource_id
  if (resource_id < 0 || resource_id >= NUM_RESOURCES) {
    return 0;
  }
  return resource_tab[resource_id + 1]; // +1 to account for INVALID_REC being -1
}

/*
Get a item's item_tab reference by provide a enum REC_IDS
Args:
ITEM_IDS item_id
  the item ID of the item info needs to get
*/
ITEM get_item_info_by_ID(ITEM_IDS item_id) {
  // Check for invalid resource_id
  if (item_id < 0 || item_id >= NUM_ITEMS) {
    return item_tab[0];
  }
  return item_tab[item_id + 1]; // +1 to account for INVALID_ITEM being -1
}

/*
Get a item's name string by provide a enum REC_IDS
Args:
ITEM_IDS item_id
  the item ID of the item info needs to from listing::item_id
*/
char * get_item_name_by_ID(int item_id) {
  if (item_id < 0 || item_id >= NUM_ITEMS) {
    return "INVALID_ITEM";
  }
  switch (item_id) {
    case 0: return "EMPTY";
    case 1: return "CITRUS";
    case 2: return "RUM";
    case 3: return "LIFE_POTION";
    case 4: return "FIRERATE_POTION";
    case 5: return "SPEED_POTION";
    case 6: return "KNIFE";
    case 7: return "SWORD";
    case 8: return "AXE";
    case 9: return "BOW";
    case 10: return "CROSSBOW";
    case 11: return "FLINTLOCK";
    case 12: return "CLOTH_ARMOR";
    case 13: return "LEATHER_ARMOR";
    case 14: return "LIGHT_ARMOR";
    case 15: return "MEDIUM_ARMOR";
    case 16: return "HEAVY_ARMOR";
    case 17: return "PLATE_ARMOR";
    case 18: return "GOLD_COIN";
    case 19: return "SILVER_COIN";
    case 20: return "COPPER_COIN";
    case 21: return "GRAIN";
    case 22: return "COTTON";
    case 23: return "WOOL";
    case 24: return "DYES";
    case 25: return "SUGAR";
    case 26: return "LEATHER";
    case 27: return "CHEESE";
    case 28: return "IRON_ORE";
    case 29: return "WINE";
    case 30: return "COPPER_ORE";
    case 31: return "SPICE";
    case 32: return "HERB";
    case 33: return "TEA";
    case 34: return "SILVER_ORE";
    case 35: return "PORCELAIN";
    case 36: return "SILK";
    case 37: return "PEARL";
    case 38: return "GOLD_ORE";
    case 39: return "SAFFRON";
    case 40: return "AMBERGRIS";
    default: return "INVALID_ITEM";
  }
}

/*
Get a item's item_tab reference string by provide a enum REC_IDS
Args:
ITEM_IDS item_id
  the item ID of the item info needs to from listing::item_id
*/
ITEM get_item_info_by_name(char * item_name) {
  if (item_name) {
    if (strcmp(item_name, "EMPTY") == 0) return get_item_info_by_ID(EMPTY);
    else if (strcmp(item_name, "CITRUS") == 0) return get_item_info_by_ID(CITRUS);
    else if (strcmp(item_name, "RUM") == 0) return get_item_info_by_ID(RUM);
    else if (strcmp(item_name, "LIFE_POTION") == 0) return get_item_info_by_ID(LIFE_POTION);
    else if (strcmp(item_name, "FIRERATE_POTION") == 0) return get_item_info_by_ID(FIRERATE_POTION);
    else if (strcmp(item_name, "SPEED_POTION") == 0) return get_item_info_by_ID(SPEED_POTION);
    else if (strcmp(item_name, "KNIFE") == 0) return get_item_info_by_ID(KNIFE);
    else if (strcmp(item_name, "SWORD") == 0) return get_item_info_by_ID(SWORD);
    else if (strcmp(item_name, "AXE") == 0) return get_item_info_by_ID(AXE);
    else if (strcmp(item_name, "BOW") == 0) return get_item_info_by_ID(BOW);
    else if (strcmp(item_name, "CROSSBOW") == 0) return get_item_info_by_ID(CROSSBOW);
    else if (strcmp(item_name, "FLINTLOCK") == 0) return get_item_info_by_ID(FLINTLOCK);
    else if (strcmp(item_name, "CLOTH_ARMOR") == 0) return get_item_info_by_ID(CLOTH_ARMOR);
    else if (strcmp(item_name, "LEATHER_ARMOR") == 0) return get_item_info_by_ID(LEATHER_ARMOR);
    else if (strcmp(item_name, "LIGHT_ARMOR") == 0) return get_item_info_by_ID(LIGHT_ARMOR);
    else if (strcmp(item_name, "MEDIUM_ARMOR") == 0) return get_item_info_by_ID(MEDIUM_ARMOR);
    else if (strcmp(item_name, "HEAVY_ARMOR") == 0) return get_item_info_by_ID(HEAVY_ARMOR);
    else if (strcmp(item_name, "PLATE_ARMOR") == 0) return get_item_info_by_ID(PLATE_ARMOR);
    else if (strcmp(item_name, "GOLD_COIN") == 0) return get_item_info_by_ID(GOLD_COIN);
    else if (strcmp(item_name, "SILVER_COIN") == 0) return get_item_info_by_ID(SILVER_COIN);
    else if (strcmp(item_name, "COPPER_COIN") == 0) return get_item_info_by_ID(COPPER_COIN);
    /* Begin resources converted to items  */
    else if (strcmp(item_name, "GRAIN") == 0) return get_item_info_by_ID(ITEM_GRAIN);
    else if (strcmp(item_name, "COTTON") == 0) return get_item_info_by_ID(ITEM_COTTON);
    else if (strcmp(item_name, "WOOL") == 0) return get_item_info_by_ID(ITEM_WOOL);
    else if (strcmp(item_name, "DYES") == 0) return get_item_info_by_ID(ITEM_DYES);
    else if (strcmp(item_name, "SUGAR") == 0) return get_item_info_by_ID(ITEM_SUGAR);
    else if (strcmp(item_name, "LEATHER") == 0) return get_item_info_by_ID(ITEM_LEATHER);
    else if (strcmp(item_name, "CHEESE") == 0) return get_item_info_by_ID(ITEM_CHEESE);
    else if (strcmp(item_name, "IRON_ORE") == 0) return get_item_info_by_ID(ITEM_IRON_ORE);
    else if (strcmp(item_name, "WINE") == 0) return get_item_info_by_ID(ITEM_WINE);
    else if (strcmp(item_name, "COPPER_ORE") == 0) return get_item_info_by_ID(ITEM_COPPER_ORE);
    else if (strcmp(item_name, "SPICE") == 0) return get_item_info_by_ID(ITEM_SPICE);
    else if (strcmp(item_name, "HERB") == 0) return get_item_info_by_ID(ITEM_HERB);
    else if (strcmp(item_name, "TEA") == 0) return get_item_info_by_ID(ITEM_TEA);
    else if (strcmp(item_name, "SILVER_ORE") == 0) return get_item_info_by_ID(ITEM_SILVER_ORE);
    else if (strcmp(item_name, "PORCELAIN") == 0) return get_item_info_by_ID(ITEM_PORCELAIN);
    else if (strcmp(item_name, "SILK") == 0) return get_item_info_by_ID(ITEM_SILK);
    else if (strcmp(item_name, "PEARL") == 0) return get_item_info_by_ID(ITEM_PEARL);
    else if (strcmp(item_name, "GOLD_ORE") == 0) return get_item_info_by_ID(ITEM_GOLD_ORE);
    else if (strcmp(item_name, "SAFFRON") == 0) return get_item_info_by_ID(ITEM_SAFFRON);
    else if (strcmp(item_name, "AMBERGRIS") == 0) return get_item_info_by_ID(ITEM_AMBERGRIS);
  }
  return get_item_info_by_ID(INVALID_ITEM);
}

/*
Helper function
Return 1 if it is
Return 0 it it is not
*/
int item_isWeapon(ITEM_IDS item_id) {
  if (item_id == KNIFE || item_id == SWORD || item_id == AXE || item_id == BOW || item_id == CROSSBOW || item_id == FLINTLOCK) {
    return 1;
  } else {
    return 0;
  }
}

/*
Helper function
Return 1 if it is
Return 0 it it is not
*/
int item_isMeleeWeapon(ITEM_IDS item_id) {
  if (item_id == KNIFE || item_id == SWORD || item_id == AXE) {
    return 1;
  } else {
    return 0;
  }
}


/*
Helper function
Return 1 if it is
Return 0 it it is not
*/
int item_isRangedWeapon(ITEM_IDS item_id) {
  if (item_id == BOW || item_id == CROSSBOW || item_id == FLINTLOCK) {
    return 1;
  } else {
    return 0;
  }
}

/*
Helper function
Return 1 if it is
Return 0 it it is not
*/
int item_isArmor(ITEM_IDS item_id) {
  if (item_id == CLOTH_ARMOR || item_id == LIGHT_ARMOR || item_id == MEDIUM_ARMOR || item_id == HEAVY_ARMOR || item_id == LEATHER_ARMOR || item_id == PLATE_ARMOR) {
    return 1;
  } else {
    return 0;
  }
}

/*
Helper function to determine if an item is edible
to increase the hunger stat and how much it is worth
in terms of hunger
*/
float item_food_value(ITEM_IDS item_id) {
  if (item_id == CITRUS) {
    return 30.0;
  } else if (item_id == RUM) {
    return 10.0;
  } else if (item_id == ITEM_CHEESE) {
    return 40.0;
  } else if (item_id == ITEM_SPICE) {
    return 5.0;
  } else if (item_id == ITEM_HERB) {
    return 15.0;
  } else if (item_id == ITEM_TEA) {
    return 10.0;
  } else if (item_id == ITEM_WINE) {
    return 10.0;
  }
  return -1.0;
}

int item_isPotion(ITEM_IDS item_id) {
  if (item_id == SPEED_POTION || item_id == FIRERATE_POTION ) {
    return 1;
  } else {
    return 0;
  }
}