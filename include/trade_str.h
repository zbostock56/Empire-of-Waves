typedef enum trade_type {
  INVALID_TRADE = 0,
  BUY = 1,
  SELL = 2
} T_TRADE;

typedef struct trade {
  MERCHANT *merchant;
  T_TRADE type;
  UI_COMPONENT *ui_listing[9];
} TRADE;
