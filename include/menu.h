#include <ui_component.h>
#include <merchant_str.h>

#define NUM_COMPONENTS (16)
#define MAX_NAME (16) // DIALOG :: Max string length of name
#define MAX_CONTENT (256) // DIALOG :: Max string length of content
#define MAX_DIALOGS (16) // DIALOG :: Max number of dialogs

/*
  Enum mapping human-readable component names to the index of the ui component
  in ui_tab
*/
typedef enum component_ids {
  INVALID_MENU = -1,
  TEST_MENU = 0,
  DIALOG_NAME = 1,
  DIALOG_CONTENT = 2,
  DIALOG_BUTTON_BUY = 3,
  DIALOG_BUTTON_SELL = 4,
  DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE = 5,
  TRADE_BUTTON_LISTING_0 = 6,
  TRADE_BUTTON_LISTING_1 = 7,
  TRADE_BUTTON_LISTING_2 = 8,
  TRADE_BUTTON_LISTING_3 = 9,
  TRADE_BUTTON_LISTING_4 = 10,
  TRADE_BUTTON_LISTING_5 = 11,
  TRADE_BUTTON_LISTING_6 = 12,
  TRADE_BUTTON_LISTING_7 = 13,
  TRADE_BUTTON_LISTING_8 = 14
} UI_ID;

// Index: item id, Element: Item struct for that item id
extern UI_COMPONENT ui_tab[NUM_COMPONENTS];

UI_COMPONENT * get_ui_component_by_ID(UI_ID ui_id);

/*
                                   DIALOG
Implements the functionality for opening and closing a dialog menu. Could be 
used for conversation with merchants or other places that need a dialog box.
*/

typedef enum dialog_type {
  INVALID_DIALOG = 0,
  TALK = 1,
  MERCHANT_OPTION = 2
} T_DIALOG;

typedef struct dialog {
  T_DIALOG type;
  char *name;
  char *content;
  UI_COMPONENT *ui_text_name;
  UI_COMPONENT *ui_text_content;
  UI_COMPONENT *ui_button_buy;
  UI_COMPONENT *ui_button_sell;
  UI_COMPONENT *ui_button_establish_trade_route;
} DIALOG;

extern DIALOG * dialog;

DIALOG * init_dialog();
void free_dialog();
void open_dialog();
void close_dialog();
int set_dialog(T_DIALOG dialog_type, char *name, char *content);

void open_buy();
void close_buy();

void open_sell();
void close_sell();

void open_establish_trade_route();
void close_establish_trade_route();

/*
                                   TRADING
Implements the functionality for buying and selling. Could be used for 
interaction with merchants.
*/

typedef enum trade_type {
  INVALID_TRADE = 0,
  BUY = 1,
  SELL = 2
} T_TRADE;

typedef struct trade {
  T_TRADE type;
  MERCHANT * merchant;
  UI_COMPONENT *ui_listing_0;
  UI_COMPONENT *ui_listing_1;
  UI_COMPONENT *ui_listing_2;
  UI_COMPONENT *ui_listing_3;
  UI_COMPONENT *ui_listing_4;
  UI_COMPONENT *ui_listing_5;
  UI_COMPONENT *ui_listing_6;
  UI_COMPONENT *ui_listing_7;
  UI_COMPONENT *ui_listing_8;
} TRADE;

extern TRADE * trade;

TRADE * init_trade();
void free_trade();
void open_trade();
void close_trade();
int set_trade(T_TRADE dialog_type, MERCHANT * merchant);

void on_click_ui_listing_0();
void on_click_ui_listing_1();
void on_click_ui_listing_2();
void on_click_ui_listing_3();
void on_click_ui_listing_4();
void on_click_ui_listing_5();
void on_click_ui_listing_6();
void on_click_ui_listing_7();
void on_click_ui_listing_8();
