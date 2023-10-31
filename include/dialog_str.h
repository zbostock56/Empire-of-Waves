#ifndef __DIALOG_STR_H__
#define __DIALOG_STR_H__

/* DEFINES */
#define MAX_NAME_STR_LENGTH (20) // DIALOG :: Max string length of name
#define MAX_CONTENT_STR_LENGTH (64) // DIALOG :: Max string length of content
#define MAX_DIALOGS (16) // DIALOG :: Max number of dialogs
#define TEXT_BUFFER_LEN (50)

typedef enum dialog_type {
  INVALID_DIALOG = 0,
  TALK = 1,
  MERCHANT_OPTION = 2
} T_DIALOG;

typedef struct dialog {
  T_DIALOG type;
  MERCHANT *merchant;
  char *name;
  char *content;
  char *relationship;
  char *mercenary_count;
  UI_COMPONENT *ui_text_name;
  UI_COMPONENT *ui_text_relationship;
  UI_COMPONENT *ui_text_content;
  UI_COMPONENT *ui_button_buy;
  UI_COMPONENT *ui_button_sell;
  UI_COMPONENT *ui_button_establish_trade_route;
  UI_COMPONENT *ui_text_schedule_trade_route_prompt;
  UI_COMPONENT *ui_mercenary_buy;
} DIALOG;

#endif
