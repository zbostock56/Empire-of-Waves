
/* DEFINES  */
#define NUM_COMPONENTS (184) // UI COMPONENT:: Max number of UI component

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
  CONSOLE_ERROR = 50,

  TRADE_BUTTON_MERCHANT_ITEM_0 = 51,
  TRADE_BUTTON_MERCHANT_ITEM_1 = 52,
  TRADE_BUTTON_MERCHANT_ITEM_2 = 53,
  TRADE_BUTTON_MERCHANT_ITEM_3 = 54,
  TRADE_BUTTON_MERCHANT_ITEM_4 = 55,
  TRADE_BUTTON_MERCHANT_ITEM_5 = 56,
  TRADE_BUTTON_MERCHANT_ITEM_6 = 57,
  TRADE_BUTTON_MERCHANT_ITEM_7 = 58,
  TRADE_BUTTON_MERCHANT_ITEM_8 = 59,
  TRADE_BUTTON_PLAYER_ITEM_0 = 60,
  TRADE_BUTTON_PLAYER_ITEM_1 = 61,
  TRADE_BUTTON_PLAYER_ITEM_2 = 62,
  TRADE_BUTTON_PLAYER_ITEM_3 = 63,
  TRADE_BUTTON_PLAYER_ITEM_4 = 64,
  TRADE_BUTTON_PLAYER_ITEM_5 = 65,
  TRADE_BUTTON_PLAYER_ITEM_6 = 66,
  TRADE_BUTTON_PLAYER_ITEM_7 = 67,
  TRADE_BUTTON_PLAYER_ITEM_8 = 68,
  TRADE_BUTTON_MERCHANT_ITEM_PAGE_UP = 69,
  TRADE_BUTTON_MERCHANT_ITEM_PAGE_DOWN = 70,
  TRADE_BUTTON_PLAYER_ITEM_PAGE_UP = 71,
  TRADE_BUTTON_PLAYER_ITEM_PAGE_DOWN = 72,
  TRADE_TEXT_ON_HOVER_ITEM = 73,
  TRADE_TEXT_MERCHANT_VALUE = 74,
  TRADE_TEXT_PLAYER_VALUE = 75,
  TRADE_BUTTON_TRADE = 76,
  TRADE_TEXT_EVENT_PROMPT = 77,

  CONTAINER_1_SLOTS = 78,
  CONTAINER_1_NEXT = 113,
  CONTAINER_1_PREV = 114,
  CONTAINER_1_MOVE = 115,
  CONTAINER_2_SLOTS = 116,
  CONTAINER_2_NEXT = 152,
  CONTAINER_2_PREV = 153,
  CONTAINER_2_MOVE = 154,

  NEW_GAME = 155,
  SAVE = 156,
  LOAD = 157,
  SAVE_CLOSE = 158,
  SAVE_PROMPT = 159,
  SAVE_INPUT = 160,
  SAVE_STATUS = 161,

  GENERAL_PROMPT_ON_SCREEN = 162,

  INVENTORY_BUTTON_PLAYER_ITEM_0 = 163,
  INVENTORY_BUTTON_PLAYER_ITEM_1 = 164,
  INVENTORY_BUTTON_PLAYER_ITEM_2 = 165,
  INVENTORY_BUTTON_PLAYER_ITEM_3 = 166,
  INVENTORY_BUTTON_PLAYER_ITEM_4 = 167,
  INVENTORY_BUTTON_PLAYER_ITEM_5 = 168,
  INVENTORY_BUTTON_PLAYER_ITEM_6 = 169,
  INVENTORY_BUTTON_PLAYER_ITEM_7 = 170,
  INVENTORY_BUTTON_PLAYER_ITEM_8 = 171,
  INVENTORY_BUTTON_PLAYER_ITEM_9 = 172,
  INVENTORY_BUTTON_PLAYER_ITEM_10 = 173,
  INVENTORY_BUTTON_PLAYER_ITEM_11 = 174,
  INVENTORY_BUTTON_PLAYER_ITEM_12 = 175,
  INVENTORY_BUTTON_PLAYER_ITEM_13 = 176,
  INVENTORY_BUTTON_PLAYER_ITEM_14 = 177,
  INVENTORY_BUTTON_PLAYER_ITEM_15 = 178,
  INVENTORY_BUTTON_WEAPON = 179,
  INVENTORY_BUTTON_ARMOR = 180,
  INVENTORY_TEXT_HOVER_ITEM = 181,
  INVENTORY_TEXT_EVENT_PROMPT = 182

  // Populate as more components are added
} UI_ID;

/* GLOBALS */
// Index: item id, Element: Item struct for that item id
extern UI_COMPONENT ui_tab[NUM_COMPONENTS];


/* UTILITIES  */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
void init_menu(vec2 position, void (*on_click)(void *),
               void (*on_hover)(void *), void *on_click_args,
               void *on_hover_args, char *text, int enabled, int textured,
               unsigned int texture, float text_padding, float text_scale,
               float width, float height, PIVOT pivot, TEXT_ANCHOR text_anchor,
               UI_COMPONENT *dest);
