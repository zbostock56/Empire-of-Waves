#ifndef __TRADE_STR_H__
#define __TRADE_STR_H__

#include <ui_component.h>

#define MAX_BUY_SELL_LISTING (9)
#define MAX_ITEMS_PER_PAGE (8) // Max number of items per page for both merchant and player associated with UI Components
#define MAX_MERCHANT_ITEM (16) // Max number of items selected for merchant, currently not infinite
#define MAX_PLAYER_ITEM (16) // Max number of items selected for merchant

typedef enum trade_type {
  INVALID_TRADE = 0,
  BUY = 1,
  SELL = 2,
  WITH_ITEM = 3
} T_TRADE;

typedef struct trade {
  MERCHANT *merchant;
  T_TRADE type;
  UI_COMPONENT *ui_listing[MAX_BUY_SELL_LISTING];
  UI_COMPONENT *ui_merchant_items[MAX_ITEMS_PER_PAGE];
  UI_COMPONENT *ui_player_items[MAX_ITEMS_PER_PAGE];
  UI_COMPONENT *ui_button_merchant_item_page_up;
  UI_COMPONENT *ui_button_merchant_item_page_down;
  UI_COMPONENT *ui_button_player_item_page_up;
  UI_COMPONENT *ui_button_player_item_page_down;
  UI_COMPONENT *ui_text_on_hover_item;
  UI_COMPONENT *ui_text_merchant_value;
  UI_COMPONENT *ui_text_player_value;
  UI_COMPONENT *ui_button_trade;
  UI_COMPONENT *ui_text_event_prompt;
  int merchant_page;
  int player_page;
  int merchant_value;
  int player_value;
  /* merchant item index at [i] is selected or not, 0 = unselected and 1 = selected */
  int merchant_item_selected[MAX_MERCHANT_ITEM];
  /* player item index at [i] is selected or not, 0 = unselected and 1 = selected */
  int player_item_selected[MAX_MERCHANT_ITEM];
} TRADE;

#endif
