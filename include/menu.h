#include <ui_component.h>

#define NUM_COMPONENTS (7)
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
  DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE = 5
} UI_ID;

// Index: item id, Element: Item struct for that item id
extern UI_COMPONENT ui_tab[NUM_COMPONENTS];

UI_COMPONENT * get_ui_component_by_ID(UI_ID ui_id);

/*
                                   dialog
Implements the functionality for opening and closing a dialog box. Could be 
used for conversation with merchants or other places that need a dialog box.
*/

typedef enum dialog_type {
  DEFAULT = 0,
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

DIALOG * new_dialog(char *name, char *content);
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
