#include <merchant_str.h>

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
  UI_COMPONENT *ui_listing[9];
} TRADE;

extern TRADE * trade;

TRADE * init_trade();
void free_trade();
void open_trade();
void close_trade();
int set_trade(T_TRADE dialog_type, MERCHANT * merchant);

void on_click_ui_listing();
