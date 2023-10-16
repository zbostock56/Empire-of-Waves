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
