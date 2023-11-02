#include <mercenary.h>

/*
Sets up the listing strings to be shown in the ui_list
*/
void open_mercenary_reassignment_menu() {
  /* Freed in close_listings() in ui_list.c */
  mercenary_reassign_listings = malloc(sizeof(char *) * num_trade_ships);
  if (!mercenary_reassign_listings) {
    fprintf(stderr, "Mercenary.c: failed to allocate listing strings\n");
    exit(1);
  }
  for (int i = 0; i < num_trade_ships; i++) {
    mercenary_reassign_listings[i] = NULL;
  }
  reassignment_menu_open = 1;
  /* Show title bar */
  vec2 title_bar_pos = { 0.0, 0.75 };
  init_menu(
    title_bar_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Mercenary Reassignment\0", // text
    1, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.75, // text_scale
    0.0, // width
    0.0, // heights
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(REASSIGN_MENU_TITLE)// dest
  );

  /* Update the listings text before opening the menu */

  /* Below will be called in main to make sure the text */
  /* is updated while menu is open */
  init_ui_list(&list, 1, 4, num_trade_ships, NULL);
  update_mercenary_reassignment();
  open_listing(&list);
}

/*
 Responsible for updating mercenary reassignment
 menu's listings with the number of mercenaries
 that the ships have assigned to them
*/
void update_mercenary_reassignment() {
  if (reassignment_menu_open && mercenary_reassign_listings) {
    char basic_string[14] = "'s Trade Ship";
    char assigned[11] = "assigned)\0";
    for (int i = 0; i < num_trade_ships; i++) {
      if (!mercenary_reassign_listings[i]) {
        mercenary_reassign_listings[i] = malloc(sizeof(char) * 64);
      }
      for (int j = 0; j < 20; j++) {
        char c = trade_ships[i].desc[j];
        /* If the character is in the normal range of A-Z */
        /* or a-z, then keep that, otherwise break out    */
        if ((c > 96 && c < 123) || (c > 64 && c < 91)) {
          mercenary_reassign_listings[i][j] = c;
        } else {
          for (int k = 0; k < 13; k++) {
            mercenary_reassign_listings[i][j++] = basic_string[k];
          }
          /* print to the screen how many are currently */
          /* assigned to the ships */
          mercenary_reassign_listings[i][j++] = ' ';
          mercenary_reassign_listings[i][j++] = '(';
          if (trade_ships[i].num_mercenaries == 10) {
            mercenary_reassign_listings[i][j++] = '1';
            mercenary_reassign_listings[i][j++] = '0';
          } else {
            mercenary_reassign_listings[i][j++] = '0' + trade_ships[i].num_mercenaries;
          }
          mercenary_reassign_listings[i][j++] = ' ';
          for (int l = 0; l < 11; l++) {
            mercenary_reassign_listings[i][j++] = assigned[l];
          }
          break;
        }
      }
    }
    list.listing_strings = mercenary_reassign_listings;
    list.num_components = num_trade_ships;
    open_listing(&list);
  }
}

/*
  Simply closes the reassignment menu
*/
void close_mercenary_reassignment_menu() {
  reassignment_menu_open = 0;
  get_ui_component_by_ID(MERCENARY_AVAIL)->enabled = 0;
  get_ui_component_by_ID(ADD_MERCENARY)->enabled = 0;
  get_ui_component_by_ID(SUB_MERCENARY)->enabled = 0;
  get_ui_component_by_ID(REASSIGN_MENU_TITLE)->enabled = 0;
  get_ui_component_by_ID(MY_SHIP_MERC)->enabled = 0;
  close_listings(&list);
}

/*
  Carries out the purchasing of once mercenary, if
  the relationship value is high enough
*/
void purchase_mercenary_handler() {
  if (dialog.merchant->relationship > 40.0) {
    unsigned int player_money = get_player_money();
    unsigned int price = calc_merc_price(dialog.merchant);
    if (player_money < price) {
      printf("Not enough money\n");
      return;
    }
    if (dialog.merchant->num_mercenaries <= 0) {
      /* Prompt that no mercenaries are available */
      /* for purchase?                            */
      printf("No mercenaries to buy\n");
      return;
    }
    dialog.merchant->num_mercenaries--;
    e_player.total_mercenaries++;
    remove_money(e_player.inventory, MAX_PLAYER_INV_SIZE);
    if (player_money > price) {
      I_SLOT *empty = get_player_first_empty_inventory_slot();
      empty->item_id = COPPER_COIN;
      empty->quantity = player_money - price;
      coalesce_currency(e_player.inventory, MAX_PLAYER_INV_SIZE);
    }
  } else {
    merchant_mercenary_relationship_prompt();
  }
}

/*
  Informs the player that their relationship
  value is not high enough to purchase a mercenary
*/
void merchant_mercenary_relationship_prompt() {
  set_prompt("Relationship too low to purchase mercenary");
}

/*
  Updates the bottom component which shows how many
  mercenaries are available to use on the ships. Function
  is called in main
*/
void update_available_mercenaries() {
  if (reassignment_menu_open) {
    snprintf(avail_mercenaries, AVAIL_MERC_BUFF_LEN,
               "Available: %d", e_player.total_mercenaries);
    snprintf(my_ship_mercenaries, MY_SHIP_MERC_LEN,
               "My Ship (%d assigned)", e_player.ship_mercenaries);
  }
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

    /* Add and subtract buttons */

    vec2 add_merc_pos = { 0.15, -0.75 };
    init_menu(
      add_merc_pos, // position
      mercenary_reassign, // on_click
      NULL, // on_hover
      (void *)((long) 1), // on_click_args
      (void *) 0xBAADF00D, // on_hover_args
      "+", // text
      1, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.0, // width
      0.0, // heights
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      get_ui_component_by_ID(ADD_MERCENARY)// dest
    );

    vec2 sub_merc_pos = { -0.15, -0.75 };
    init_menu(
      sub_merc_pos, // position
      mercenary_reassign, // on_click
      NULL, // on_hover
      (void *)((long) -1), // on_click_args
      (void *) 0xBAADF00D, // on_hover_args
      "-", // text
      1, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.0, // width
      0.0, // heights
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      get_ui_component_by_ID(SUB_MERCENARY)// dest
    );

    vec2 my_ship_pos = { 0.0, 0.50 };
    init_menu(
      my_ship_pos,
      (void (*)(void *)) select_listing_dispatcher,
      NULL,
      (void *) ((long) 16),
      NULL,
      my_ship_mercenaries,
      1,
      1,
      0,
      0.05,
      0.75,
      1.0,
      0.2,
      PIVOT_CENTER,
      T_CENTER,
      get_ui_component_by_ID(MY_SHIP_MERC)
    );
  }
}

/*
  Tells the assignment window what selection out of
  the currently listed selections in the reassignment
  window was clicked on
*/
void mercenary_reassignment_selection(int listing_selected) {
  /* Reset all of other listing selections to disabled */
  memset(mercenary_listing_selected, 0,  TOT_LISTING_NUM * sizeof(int));
  mercenary_listing_selected[listing_selected] = 1;
}

/*
  Actually performs the action of reassigning a mercenary
  to a trade ship or the player's ship
*/
void mercenary_reassign(void *ch) {
  size_t change = (size_t) ch;
  if ((e_player.total_mercenaries && change > 0) ||
       change < 0) {
    int is_selected = -1;
    for (int i = 0; i < TOT_LISTING_NUM; i++) {
      if (mercenary_listing_selected[i]) {
        is_selected = i;
        break;
      }
      /* Check if at the end of all available */
      /* listings that could be displayed on the screen */
      if (i == TOT_LISTING_NUM - 1) {
        /* Prompt to select a listing */
        return;
      }
    }
    /* Adding or subtracting from player ship */
    if (is_selected == (TOT_LISTING_NUM - 1)) {
      mercenary_reassign_my_ship(change);
      return;
    }
    /* Calculate what listing this equates to in the trade */
    /* ship array */
    int actual_item = ((list.page - 1) * list.comp_per_page) + is_selected;

    /* Add or subtract the mercenary from the player's */
    /* total allotment of mercenaries available */
    if (change < 0) {
      if (trade_ships[actual_item].num_mercenaries) {
        e_player.total_mercenaries++;
      } else {
        /* No mercenaries left to remove from trade ship or */
        /* your own ship */
        printf("No more mercenaries are left on this ship to remove\n");
        return;
      }
    } else if (change > 0) {
      if (trade_ships[actual_item].num_mercenaries < 10) {
        e_player.total_mercenaries--;
      } else {
        /* No more mercenaries can be added to this ship */
        printf("No more mercenaries can be added to this ship\n");
        return;
      }
    }
    (trade_ships + actual_item)->num_mercenaries += change;
  }
}

/* Handles reassigning mercenaries only to the player's ship */
void mercenary_reassign_my_ship(long change) {
  if (change < 0) {
    if (e_player.ship_mercenaries) {
      e_player.total_mercenaries++;
    } else {
      printf("No more mercenaries are left on your ship to remove\n");
      return;
    }
  } else if (change > 0) {
    if (e_player.ship_mercenaries < 10) {
      e_player.total_mercenaries--;
    } else {
      printf("No more mercenaries can be added to your ship\n");
      return;
    }
  }
  e_player.ship_mercenaries += change;
}
