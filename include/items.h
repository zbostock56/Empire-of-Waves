#ifndef __ITEMS_H__
#define __ITEMS_H__

#define NUM_ITEMS (42)
#define NUM_RESOURCES (42)
#define COPPER_PER_SILVER (10)
#define SILVER_PER_GOLD (10)

/*
  Enum mapping human-readable resource names to the index of the resource in
  resource_tab
*/
typedef enum resource_ids {
  INVALID_REC = -1,
  // items below
  REC_EMPTY = 0,
  REC_CITRUS = 1,
  REC_RUM = 2,
  REC_LIFE_POTION = 3,
  REC_FIRERATE_POTION = 4,
  REC_SPEED_POTION = 5,
  REC_KNIFE = 6,
  REC_SWORD = 7,
  REC_AXE = 8,
  REC_BOW = 9,
  REC_CROSSBOW = 10,
  REC_FLINTLOCK = 11,
  REC_CLOTH_ARMOR = 12,
  REC_LEATHER_ARMOR = 13,
  REC_LIGHT_ARMOR = 14,
  REC_MEDIUM_ARMOR = 15,
  REC_HEAVY_ARMOR = 16,
  REC_PLATE_ARMOR = 17,
  REC_GOLD_COIN = 18,
  REC_SILVER_COIN = 19,
  REC_COPPER_COIN = 20,
  // recourses below
  GRAIN =        21,
  COTTON =       22,
  WOOL =         23,
  DYES =         24,
  SUGAR =        25,
  LEATHER =      26,
  CHEESE =       27,
  IRON_ORE =     28,
  WINE =         29,
  COPPER_ORE =   30,
  SPICE =        31,
  HERB =         32,
  TEA =          33,
  SILVER_ORE =   34,
  PORCELAIN =    35,
  SILK =         36,
  PEARL =        37,
  GOLD_ORE =     38,
  SAFFRON =      39,
  AMBERGRIS =    40
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
  KNIFE = 6,
  SWORD = 7,
  AXE = 8,
  // Ranged Weapons
  BOW = 9,
  CROSSBOW = 10,
  FLINTLOCK = 11,
  // Armor
  CLOTH_ARMOR = 12,
  LEATHER_ARMOR = 13,
  LIGHT_ARMOR = 14,
  MEDIUM_ARMOR = 15,
  HEAVY_ARMOR = 16,
  PLATE_ARMOR = 17,
  // Currency 1 GOLD = 10 SILVER = 100 COPPER
  GOLD_COIN = 18,
  SILVER_COIN = 19,
  COPPER_COIN = 20,
  ITEM_GRAIN =       21,
  ITEM_COTTON =      22,
  ITEM_WOOL =        23,
  ITEM_DYES =        24,
  ITEM_SUGAR =       25,
  ITEM_LEATHER =     26,
  ITEM_CHEESE =      27,
  ITEM_IRON_ORE =    28,
  ITEM_WINE =        29,
  ITEM_COPPER_ORE =  30,
  ITEM_SPICE =       31,
  ITEM_HERB =        32,
  ITEM_TEA =         33,
  ITEM_SILVER_ORE =  34,
  ITEM_PORCELAIN =   35,
  ITEM_SILK =        36,
  ITEM_PEARL =       37,
  ITEM_GOLD_ORE =    38,
  ITEM_SAFFRON =     39,
  ITEM_AMBERGRIS =   40
} ITEM_IDS;

/*
 Struct representing an item in the game
*/
typedef struct item {
  int value; // Sell/buy price
  int edible; // Can be eat
  int equippable; // Can be equipped by player
  float health_mod; // When eat, how it modifies player health:
                    // heath = current_health + health_mod
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

int item_isWeapon(ITEM_IDS item_id);
int item_isMeleeWeapon(ITEM_IDS item_id);
int item_isRangedWeapon(ITEM_IDS item_id);
int item_isArmor(ITEM_IDS item_id);
float item_food_value(ITEM_IDS);

#endif
