#define NUM_ITEMS (1)
#ifndef NUM_RESOURCES
#define NUM_RESOURCES (1) // Also defined in items.h
#endif

/*
  Enum mapping human-readable resource names to the index of the resource in
  resource_tab
*/
typedef enum resource_ids {
 INVALID_REC = -1
 // Populate as more resources are added
} REC_IDS;

/*
  Enum mapping human-readable item names to the index of the item in item_tab
*/
typedef enum item_ids {
  INVALID_ITEM = -1
  // Populate as more items are added
} ITEM_IDS;

/*
 Struct representing an item in the game
*/
typedef struct item {
  int value; // Sell/buy price
  int equippable; // Can be equipped by player
  float firerate_mod; // When equipped, how it modifies the player firerate:
                      // firerate = base_firerate + firerate_mod
  float speed_mod; // When equipped, how it modifies player speed:
                   // speed = base_speed + speed_mod
} ITEM;

// Index: item id, Element: Item struct for that item id
extern ITEM item_tab[NUM_ITEMS];
// Index: resource id, Element: Value of resource
extern unsigned int resource_tab[NUM_RESOURCES];
