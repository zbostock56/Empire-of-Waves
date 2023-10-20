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
  reassignment_menu_open = 1;
  open_listing(&list);
}

void close_mercenary_reassignment_menu() {
  reassignment_menu_open = 0;
  get_ui_component_by_ID(MERCENARY_AVAIL)->enabled = 0;
  close_listings(&list);
}

void purchase_mercenary_handler() {
  if (cur_merchant->num_mercenaries <= 0) {
    /* Prompt that no mercenaries are available */
    /* for purchase?                            */
    printf("No mercenaries to buy\n");
    return;
  }
  cur_merchant->num_mercenaries--;
  e_player.total_mercenaries++;
}

void update_available_mercenaries() {
  snprintf(avail_mercenaries, AVAIL_MERC_BUFF_LEN,
             "Available: %d", e_player.total_mercenaries);
}

void check_available_mercenaries() {
  if (reassignment_menu_open) {
    vec2 avail_merc_pos = { 0.0, -0.75 };
    init_menu(
      avail_merc_pos, // position
      NULL, // on_click
      NULL, // on_hover
      (void *) 0xBAADF00D, // on_click_args
      (void *) 0xBAADF00D, // on_hover_args
      avail_mercenaries, // text
      1, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.0, // width
      0.0, // heights
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      get_ui_component_by_ID(MERCENARY_AVAIL)// dest
    );
  }
}
