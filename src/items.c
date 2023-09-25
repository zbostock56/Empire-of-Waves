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
    equippable (int),
    firerate_mod (float),
    speed_mod (float)
  }
  */
};

// Resource table definition
unsigned int resource_tab[NUM_RESOURCES] = {
  // resource value (unsigned int)
};
