#include <mercenary.h>

void open_mercenary_reassignment_menu() {
  char **listing_strings = malloc(sizeof(char *) * num_trade_ships);
  if (!listing_strings) {
    fprintf(stderr, "Mercenary.c: failed to allocate listing strings\n");
    exit(1);
  }
  for (int i = 0; i < num_trade_ships; i++) {
    listing_strings[i] = trade_ships[i].desc;
    snprintf(listing_strings[i], 50, "trade ship (%d)", i);
  }
  init_ui_list(&list, 1, 4, num_trade_ships, listing_strings);
  open_listing(&list);
  reassignment_menu_open = 1;
}

void close_mercenary_reassignment_menu() {
  close_listings(&list);
  reassignment_menu_open = 0;
}
