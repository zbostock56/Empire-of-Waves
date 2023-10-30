#ifndef __ITEMS_H__
#define __ITEMS_H__

#define NUM_ITEMS (22)
#define NUM_RESOURCES (21)
#define COPPER_PER_SILVER (10)
#define SILVER_PER_GOLD (10)

/*
  Enum mapping human-readable resource names to the index of the resource in
  resource_tab
*/
typedef enum resource_ids {
  INVALID_REC = -1,
  GRAIN = 0,
  SUGAR = 1,
  WINE = 2,
  CHEESE = 3,
  SPICE = 4,
  TEA = 5,
  WOOL = 6,
  LEATHER = 7,
  COTTON = 8,
  SILK = 9,
  DYES = 10,
  GOLD_ORE = 11,
  SILVER_ORE = 12,
  COPPER_ORE = 13,
  IRON_ORE = 14,
  PEAL = 15,
  PORCELAIN = 16,
  AMBERGRIS = 17,
  HERB = 18,
  SAFFRON = 19
} REC_IDS;

/*
  Enum mapping human-readable item names to the index of the item in item_tab
*/
typedef enum item_ids {
  INVALID_ITEM = -1,
  // Empty
  EMPTY = 0,
  // Foods and Drinks
  CITRUS = 1,
  RUM = 2,
  // Potion
  LIFE_POTION = 3,
  FIRERATE_POTION = 4,
  SPEED_POTION = 5,
  // Melee Weapons
  KNIVE = 6,
  SWROD = 7,
  AXE = 8,
  // Ranged Weapons
  BOW = 9,
  CROSSBOW = 10,
  FLINTLOCK = 11,
  // Armor
  CLOTH_ARMOR = 12,
  LEATHER_ARMOR = 13,
  LIGHT_ARMOR = 14,
  MIDIUM_ARMOR = 15,
  HEAVY_ARMOR = 16,
  PLATE_ARMOR = 17,
  // Currency 1 GOLD = 10 SILVER = 100 COPPER
  GOLD_COIN = 18,
  SILVER_COIN = 19,
  COPPER_COIN = 20
} ITEM_IDS;

/*
 Struct representing an item in the game
*/
typedef struct item {
  int value; // Sell/buy price
  int edible; // Can be eat
  float health_mod; // When eat, how it modifies player health:
                    // heath = current_health + health_mod
  int equippable; // Can be equipped by player
  float firerate_mod; // When equipped, how it modifies the player firerate:
                      // firerate = base_firerate + firerate_mod
  float speed_mod; // When equipped, how it modifies player speed:
                   // speed = base_speed + speed_mod
  float max_heath_mod; // When equipped, how it modifies player max_health:
                       // max_heath = max_health + max_health_mod
} ITEM;

// Index: item id, Element: Item struct for that item id
extern ITEM item_tab[NUM_ITEMS];
// Index: resource id, Element: Value of resource
extern unsigned int resource_tab[NUM_RESOURCES];

unsigned int get_resource_value_by_ID(REC_IDS resource_id);
ITEM get_item_info_by_ID(ITEM_IDS item_id);
char * get_item_name_by_ID(int item_id);
ITEM get_item_info_by_name(char *item_name);

#endif
