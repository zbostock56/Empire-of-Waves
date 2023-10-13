#include <ui_component.h>
#include <trade.h>

#define NUM_COMPONENTS (22) // UI COMPONENT:: Max number of UI component
#define MAX_NAME_STR_LENGTH (16) // DIALOG :: Max string length of name
#define MAX_CONTENT_STR_LENGTH (256) // DIALOG :: Max string length of content
#define MAX_DIALOGS (16) // DIALOG :: Max number of dialogs
#define MAX_LISTING_STR_LENGTH (16) // TRADE :: Max length of listing string
#define MAX_STATUS_STR_LENGTH (16) // STATUS :: Max length of status string
#define TEXT_BUFFER_LEN (50)

/*
  Enum mapping human-readable component names to the index of the ui component
  in ui_tab
*/
typedef enum component_ids {
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
  STATUS_MONEY = 20
  // Populate as more components are added
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
  MERCHANT *merchant;
  char *name;
  char *relationship;
  char *content;
  UI_COMPONENT *ui_text_name;
  UI_COMPONENT *ui_text_relationship;
  UI_COMPONENT *ui_text_content;
  UI_COMPONENT *ui_button_buy;
  UI_COMPONENT *ui_button_sell;
  UI_COMPONENT *ui_button_establish_trade_route;
  UI_COMPONENT *ui_text_schedule_trade_route_prompt;
} DIALOG;

extern DIALOG * dialog;
extern float time_schdule_trade_toute_prompt;

DIALOG * init_dialog();
void free_dialog();
void open_dialog();
void close_dialog();
int set_dialog(T_DIALOG, MERCHANT *, char *, char *);

void open_buy();
void close_buy();

void open_sell();
void close_sell();

void open_establish_trade_route();
void close_establish_trade_route();

/*
                                   STATUS
Implements the functionality for shows player status.
*/

typedef struct status {
  UI_COMPONENT *ui_health_status;
  UI_COMPONENT *ui_money_status;
} STATUS;

extern STATUS * status;

STATUS * init_status_bar();
void free_status();
void update_status_bar();
void open_status_bar();
void close_status_bar();
