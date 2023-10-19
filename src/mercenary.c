#include <mercenary.h>

/*
Sets up the listing strings to be shown in the ui_list
*/
void open_mercenary_reassignment_menu() {
  /* Freed in close_listings() in ui_list.c */
  char **listing_strings = malloc(sizeof(char *) * num_trade_ships);
  if (!listing_strings) {
    fprintf(stderr, "Mercenary.c: failed to allocate listing strings\n");
    exit(1);
  }
  char basic_string[15] = "'s Trade Ship\0";
  for (int i = 0; i < num_trade_ships; i++) {
    listing_strings[i] = malloc(sizeof(char) * 48);
    for (int j = 0; j < 20; j++) {
      char c = trade_ships[i].desc[j];
      /* If the character is in the normal range of A-Z */
      /* or a-z, then keep that, otherwise break out    */
      if ((c > 96 && c < 123) || (c > 64 && c < 91)) {
        listing_strings[i][j] = c;
      } else {
        for (int k = 0; k < 15; k++) {
          listing_strings[i][j++] = basic_string[k];
        }
        break;
      }
    }
  }
  init_ui_list(&list, 1, 4, num_trade_ships, listing_strings);
  open_listing(&list);
  reassignment_menu_open = 1;
}

void close_mercenary_reassignment_menu() {
  close_listings(&list);
  reassignment_menu_open = 0;
}
