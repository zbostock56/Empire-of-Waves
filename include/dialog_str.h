/* DEFINES */
#define MAX_NAME_STR_LENGTH (16) // DIALOG :: Max string length of name
#define MAX_CONTENT_STR_LENGTH (256) // DIALOG :: Max string length of content
#define MAX_DIALOGS (16) // DIALOG :: Max number of dialogs
#define TEXT_BUFFER_LEN (50)

typedef enum dialog_type {
  INVALID_DIALOG = 0,
  TALK = 1,
  MERCHANT_OPTION = 2
} T_DIALOG;

typedef struct dialog {
  T_DIALOG type;
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

