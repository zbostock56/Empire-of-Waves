#include <ui_component.h>

#define NUM_COMPONENTS (1)

/*
  Enum mapping human-readable component names to the index of the ui component
  in ui_tab
*/
typedef enum component_ids {
  INVALID_MENU = -1
  // Populate as more components are added
} UI_ID;

// Index: item id, Element: Item struct for that item id
extern UI_COMPONENT ui_tab[NUM_COMPONENTS];
