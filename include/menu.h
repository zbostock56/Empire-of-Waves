
/* DEFINES  */
#define NUM_COMPONENTS (51) // UI COMPONENT:: Max number of UI component

/*
  Enum mapping human-readable component names to the index of the ui component
  in ui_tab
*/
typedef enum component_ids {
  /*
    WARNING:
    Zero indexed. Make sure to update above define statment
    when adding more components.
  */
  INVALID_MENU = -1,
  TEST_MENU = 0,
  DIALOG_NAME = 1,
  DIALOG_RELATION = 2,
  DIALOG_CONTENT = 3,
  DIALOG_BUTTON_BUY = 4,
  DIALOG_BUTTON_SELL = 5,
  DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE = 6,
  TRADE_BUTTON_LISTING_0 = 7,
  TRADE_BUTTON_LISTING_1 = 8,
  TRADE_BUTTON_LISTING_2 = 9,
  TRADE_BUTTON_LISTING_3 = 10,
  TRADE_BUTTON_LISTING_4 = 11,
  TRADE_BUTTON_LISTING_5 = 12,
  TRADE_BUTTON_LISTING_6 = 13,
  TRADE_BUTTON_LISTING_7 = 14,
  TRADE_BUTTON_LISTING_8 = 15,
  EMBARK_PROMPT = 16,
  INTERACT_PROMPT = 17,
  SCHEDULED_TRADE_ROUTE_PROMPT = 18,
  STATUS_HEALTH = 19,
  STATUS_MONEY = 20,
  LIST_BUTTON_RIGHT = 21,
  LIST_BUTTON_LEFT = 22,
  LIST_ITEM = 23,
  MERCENARY_BUY_PROMPT = 39,
  MERCENARY_AVAIL = 40,
  ADD_MERCENARY = 41,
  SUB_MERCENARY = 42,
  REASSIGN_MENU_TITLE = 43,
  MY_SHIP_MERC = 44,
  CONSOLE = 45,
  CONSOLE_CURSOR = 46,
  CONSOLE_WORLD_COORDS = 47,
  CONSOLE_INTRA_CHUNK_COORDS = 48,
  CONSOLE_CHUNK_COORDS = 49,
  CONSOLE_ERROR = 50
  // Populate as more components are added
} UI_ID;

/* GLOBALS */
// Index: item id, Element: Item struct for that item id
extern UI_COMPONENT ui_tab[NUM_COMPONENTS];


/* UTILITIES  */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
