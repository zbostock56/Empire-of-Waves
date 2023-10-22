#include <trade.h>

/* Init trade menu */
void init_trade() {
  trade.type = INVALID_TRADE;

  // Init buy & sell listing UI
  for (int i = 0; i < 9; i++) {
    trade.ui_listing[i] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_0 + i);
  }

  // Init trade with items UI (8 merchant item and 8 player item)
  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i] = get_ui_component_by_ID(TRADE_BUTTON_MERCHANT_ITEM_0 + i);
    trade.ui_player_items[i] = get_ui_component_by_ID(TRADE_BUTTON_PLAYER_ITEM_0 + i);
  }

  // Init buttons and texts ui
  UI_COMPONENT *ui_button_merchant_item_page_up = get_ui_component_by_ID(TRADE_BUTTON_MERCHANT_ITEM_PAGE_UP);
  UI_COMPONENT *ui_button_merchant_item_page_down = get_ui_component_by_ID(TRADE_BUTTON_MERCHANT_ITEM_PAGE_DOWN);
  UI_COMPONENT *ui_button_player_item_page_up = get_ui_component_by_ID(TRADE_BUTTON_PLAYER_ITEM_PAGE_UP);
  UI_COMPONENT *ui_button_player_item_page_down = get_ui_component_by_ID(TRADE_BUTTON_PLAYER_ITEM_PAGE_DOWN);
  UI_COMPONENT *ui_text_on_hover_item = get_ui_component_by_ID(TRADE_TEXT_ON_HOVER_ITEM);
  UI_COMPONENT *ui_text_merchant_value = get_ui_component_by_ID(TRADE_TEXT_MERCHANT_VALUE);
  UI_COMPONENT *ui_text_player_value = get_ui_component_by_ID(TRADE_TEXT_PLAYER_VALUE);
  UI_COMPONENT *ui_button_trade = get_ui_component_by_ID(TRADE_BUTTON_TRADE);
  UI_COMPONENT *ui_text_event_prompt = get_ui_component_by_ID(TRADE_TEXT_EVENT_PROMPT);

  for (int i = 0; i < MAX_MERCHANT_ITEM_SELECTED; i++) {
    trade.merchant_item_selected[i] = 0;
  }
  for (int i = 0; i < MAX_PLAYER_ITEM_SELECTED; i++) {
    trade.player_item_selected[i] = 0;
  }

  // Init listings
  vec2 listing_0_position = { -0.5, 0.5 };
  init_menu(
    listing_0_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 0, // on_click_args
    NULL, // on_hover_args
    "UI_L_1", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[0] // dest
  );

  vec2 listing_1_position = { 0.0, 0.5 };
  init_menu(
    listing_1_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 1, // on_click_args
    NULL, // on_hover_args
    "UI_L_2", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[1] // dest
  );

  vec2 listing_2_position = { 0.5, 0.5 };
  init_menu(
    listing_2_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 2, // on_click_args
    NULL, // on_hover_args
    "UI_L_3", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[2]// dest
  );

  vec2 listing_3_position = { -0.5, 0.0 };
  init_menu(
    listing_3_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 3, // on_click_args
    NULL, // on_hover_args
    "UI_L_4", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[3] // dest
  );

  vec2 listing_4_position = { 0.0, 0.0 };
  init_menu(
    listing_4_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 4, // on_click_args
    NULL, // on_hover_args
    "UI_L_5", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[4] // dest
  );

  vec2 listing_5_position = { 0.5, 0.0 };
  init_menu(
    listing_5_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 5, // on_click_args
    NULL, // on_hover_args
    "UI_L_6", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[5] // dest
  );

  vec2 listing_6_position = { -0.5, -0.5 };
  init_menu(
    listing_6_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 6, // on_click_args
    NULL, // on_hover_args
    "UI_L_7", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[6] // dest
  );

  vec2 listing_7_position = { 0.0, -0.5 };
  init_menu(
    listing_7_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 7, // on_click_args
    NULL, // on_hover_args
    "UI_L_8", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[7] // dest
  );

  vec2 listing_8_position = { 0.5, -0.5 };
  init_menu(
    listing_8_position, // position
    on_click_ui_listing, // on_click
    NULL, // on_hover
    (void *) 8, // on_click_args
    NULL, // on_hover_args
    "UI_L_9", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[8] // dest
  );

  // Init merchant items
  for (uintptr_t i = 0; i < MAX_ITEMS_PER_PAGE; i++) {
    init_menu(
      (vec2) { 0.2, -0.2 }, // position
      on_click_ui_item, // on_click
      on_hover_ui_item, // on_hover
      (void *) i, // on_click_args
      (void *) i, // on_hover_args
      "UI_M", // text
      0, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.5, // width
      0.5, // height
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      trade.ui_merchant_items[i] // dest
    );
  }

  // Init player items
  for (uintptr_t i = 0; i < MAX_ITEMS_PER_PAGE; i++) {
    init_menu(
      (vec2) { 0.2, -0.2 }, // position
      on_click_ui_item, // on_click
      on_hover_ui_item, // on_hover
      (void *) i, // on_click_args
      (void *) i,// on_hover_args
      "UI_P", // text
      0, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.5, // width
      0.5, // height
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      trade.ui_merchant_items[i] // dest
    );
  }

  // Init page up and down
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    on_click_page_up, // on_click
    NULL, // on_hover
    (void *) 1, // on_click_args
    NULL, // on_hover_args
    "PG_UP", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_merchant_item_page_up // dest
  );

  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    on_click_page_down, // on_click
    NULL, // on_hover
    (void *) 1, // on_click_args
    NULL, // on_hover_args
    "PG_DN", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_merchant_item_page_down // dest
  );

  // Init page up and down
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    on_click_page_up, // on_click
    NULL, // on_hover
    (void *) 1, // on_click_args
    NULL, // on_hover_args
    "PG_UP", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_player_item_page_up // dest
  );

  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    on_click_page_down, // on_click
    NULL, // on_hover
    (void *) 1, // on_click_args
    NULL, // on_hover_args
    "PG_DN", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_player_item_page_down // dest
  );

  // Init on-hover prompt
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "ON_HOVER", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_on_hover_item // dest
  );

  // Init merchant and player value text
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "M_VAL", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_merchant_value // dest
  );

  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "P_VAL", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_player_value // dest
  );

  // Init trade button
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    on_click_trade, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "TRADE", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_trade // dest
  );

  // Init trade event prompt
  init_menu(
    (vec2) { 0.2, -0.2 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "EVENT PROMPT", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_event_prompt // dest
  );


  // Test
  open_trade();
}

/* Render trade menu in frontend */
void open_trade() {
  int is_ui_buy_sell_enabled = 0;
  int is_ui_trade_with_items_enabled = 0;
  switch (trade.type) {
    case BUY: {
      is_ui_buy_sell_enabled = 1;
      is_ui_trade_with_items_enabled = 0;
      break;
    }
    case SELL: {
      is_ui_buy_sell_enabled = 1;
      is_ui_trade_with_items_enabled = 0;
      break;
    }
    case WITH_ITEM: {
    }
    default: {
      is_ui_buy_sell_enabled = 0;
      is_ui_trade_with_items_enabled = 1;
      break;
    }
  }
  for (int i = 0; i < 9; i++) {
    trade.ui_listing[i]->enabled = is_ui_buy_sell_enabled;
  }
  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i]->enabled = is_ui_trade_with_items_enabled;
    trade.ui_player_items[i]->enabled = is_ui_trade_with_items_enabled;
  }
  trade.ui_button_merchant_item_page_up->enabled = is_ui_trade_with_items_enabled;
  trade.ui_button_merchant_item_page_down->enabled = is_ui_trade_with_items_enabled;
  trade.ui_button_player_item_page_up->enabled = is_ui_trade_with_items_enabled;
  trade.ui_button_player_item_page_down->enabled = is_ui_trade_with_items_enabled;
  trade.ui_text_on_hover_item->enabled = is_ui_trade_with_items_enabled;
  trade.ui_text_merchant_value->enabled = is_ui_trade_with_items_enabled;
  trade.ui_text_player_value->enabled = is_ui_trade_with_items_enabled;
  trade.ui_button_trade->enabled = is_ui_trade_with_items_enabled;
  trade.ui_text_event_prompt->enabled = is_ui_trade_with_items_enabled;
}

/* Derender trade menu */
void close_trade() {
  for (int i = 0; i < 9; i++) {
    trade.ui_listing[i]->enabled = 0;
  }
  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i]->enabled = 0;
    trade.ui_player_items[i]->enabled = 0;
  }
}

/*
Set trade menu with given trade type
Args:
T_TRADE trade_type
  type of trade, e.g. BUY
*/
int set_trade(T_TRADE trade_type) {
  if (cur_merchant) {
    trade.type = trade_type;
    // Set trade UI components with given trade type
    switch (trade_type) {
      // BUY: Set each listing slots with merchant listings
      case BUY: {
        for (int i = 0; i < 9; i++) {
          trade.ui_listing[i]->text = get_item_name_by_ID(
                                        get_merchant_listing_item_by_index(cur_merchant, i)->item_id);
        }
        return 1;
      }
      // SELL: Set each listing slots with invotory slots
      case SELL: {
        for (int i = 0; i < 9; i++) {
          trade.ui_listing[i]->text = get_item_name_by_ID(
                                        get_player_inventory_slot_by_index(i)->item_id);
        }
        return 1;
      }
      default: {
        for (int i = 0; i < 9; i++) {
          trade.ui_listing[i]->text = "default";
        }
        return 1;
      }
    }
  }
  return 0;
}

/*
Click listener of trade menu ui listings
Args:
unsigned int listing_ui_index
  listing item index, from 0 to 8
*/
void on_click_ui_listing(void *listing_ui_index) {
  uintptr_t listing_index = (uintptr_t)listing_ui_index;
  LISTING *listing_slot;
  I_SLOT *slot;
  ITEM item;
  if (trade.type == BUY) {
    item = get_item_info_by_name(trade.ui_listing[listing_index]->text);
    listing_slot = get_merchant_listing_item_by_index(cur_merchant, listing_index);
    // If player have enough money to buy the listing item
    // Decrease the quatity of merchant listing slot
    if (e_player.money >= item.value && listing_slot->quantity > 0) {
      listing_slot->quantity -= 1;
      e_player.money -= item.value;
      // Add listing item to player inventory
      // If player already have this item, increase its quatity
      // Other wise, add the item to the first empty slot
      slot = search_player_inventory_with_ID(
               get_merchant_listing_item_by_index(
               cur_merchant, listing_index)->item_id);
      if (slot) {
        slot->quantity += 1;
      } else if (get_player_first_empty_inventory_slot()) {
        I_SLOT *empty_inventory_slot = get_player_first_empty_inventory_slot();
        empty_inventory_slot->item_id = get_merchant_listing_item_by_index(cur_merchant, listing_index)->item_id;
        empty_inventory_slot->quantity = 1;
      } else {
        listing_slot->quantity += 1;
        e_player.money += item.value;
        // TODO: popup when player's inventory is full already
        return;
      }

      // When merchant item less than 1, show slot empty
      if (listing_slot->quantity <= 0) {
        listing_slot->item_id = 0;
        trade.ui_listing[listing_index]->text = "EMPTY";
      }

      // Increase the merchant relationship, maximum 100.0
      cur_merchant->relationship += 10.0;
      if (cur_merchant->relationship > 100.0) {
        cur_merchant->relationship = 100.0;
      }

#ifndef __linux__
      printf("**** [SLOT %lld] [ITEM \"%s\"] [QUATITY %d] ****\n",
             listing_index + 1, trade.ui_listing[listing_index]->text,
             get_merchant_listing_item_by_index(cur_merchant,
                                                listing_index)->quantity);
#else
      printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
             listing_index + 1, trade.ui_listing[listing_index]->text,
             get_merchant_listing_item_by_index(cur_merchant,
                                                listing_index)->quantity);
#endif
    }
  } else if (trade.type == SELL && get_player_inventory_slot_by_index(listing_index)->quantity > 0) {
    // If player trying to sell a item
    // Decrease the item quatity in that slot
    // Get the infomation of that item
    // Add money as the item price
    slot = get_player_inventory_slot_by_index(listing_index);
    slot->quantity -= 1;
    int sold_item_id = slot->item_id;
    item = get_item_info_by_name(trade.ui_listing[listing_index]->text);
    e_player.money += item.value;

    // When item's quatity in inventory less than 1, show slot empty
    if (slot->quantity <= 0) {
      slot->item_id = 0;
      trade.ui_listing[listing_index]->text = "EMPTY";
    }

    // Add solded item to merchant listing
    // If merchant already have this item, increase its quatity
    // Other wise, add the item to the first empty slot
    for (int i = 0; i < 9; i++) {
      listing_slot = get_merchant_listing_item_by_index(cur_merchant, i);
      if (listing_slot->item_id == sold_item_id) {
        listing_slot->quantity += 1;
        break;
      } else if (listing_slot->quantity == 0) {
        listing_slot->quantity += 1;
        listing_slot->item_id = sold_item_id;
        break;
      }
    }

    // Increase the merchant relationship, maximum 100.0
    cur_merchant->relationship += 10.0;
    if (cur_merchant->relationship > 100.0) {
      cur_merchant->relationship = 100.0;
    }

#ifndef __linux__
    printf("**** [SLOT %lld] [ITEM \"%s\"] [QUATITY %d] ****\n",
           listing_index + 1, trade.ui_listing[listing_index]->text,
           get_player_inventory_slot_by_index(listing_index)->quantity);
#else
    printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
           listing_index + 1, trade.ui_listing[listing_index]->text,
           get_player_inventory_slot_by_index(listing_index)->quantity);
#endif
  }
}

/* Render buy menu, called by dialog "buy" button */
void open_buy() {
  close_dialog();
  if (set_trade(BUY)) {
    open_trade();
  }
}

/* Render sell menu, called by dialog "sell" button */
void open_sell() {
  close_dialog();
  if (set_trade(SELL)) {
    open_trade();
  }
}

/* Click lisnter of establish trade route button */
void open_establish_trade_route() {
  MERCHANT *target_merch = cur_merchant;
  CHUNK *target_chunk = NULL;
  // Check if player already have a trade route to this island
  for (int i = 0; i < num_trade_ships; i++) {
    target_chunk = chunk_buffer + trade_ships[i].target_chunk_index;
    if (target_chunk->coords[0] == target_merch->chunk[0] &&
        target_chunk->coords[1] == target_merch->chunk[1]) {
      dialog.ui_text_schedule_trade_route_prompt->text = "Unable to Schedule Duplicate Trade Route";
      dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
      return;
    }
  }
  // Establish trade route and popup the prompt shows successful
  dialog.ui_text_schedule_trade_route_prompt->text = "Trade Route Established";
  dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
  time_schdule_trade_toute_prompt = 2.0;

  TRADE_SHIP *trade_ship = trade_ships + num_trade_ships;

  ivec2 target_chunk_coords = { 0, 0 };
  glm_ivec2_copy(cur_merchant->chunk, target_chunk_coords);
  trade_ship->target_chunk_index = add_chunk(target_chunk_coords);

  ivec2 cur_chunk_coords = { 0, 0 };
  trade_ship->cur_chunk_index = add_chunk(cur_chunk_coords);

  glm_vec2_copy(home_island_coords, trade_ship->coords);
  glm_ivec2_copy(cur_chunk_coords, trade_ship->chunk_coords);
  glm_vec2_zero(trade_ships[num_trade_ships].direction);
  trade_ships[num_trade_ships].direction[0] = 1.0;
  trade_ships[num_trade_ships].export_rec = 0;
  trade_ships[num_trade_ships].import_rec = 0;
  trade_ships[num_trade_ships].target_island = 0;
  trade_ships[num_trade_ships].num_mercenaries = 0;
  trade_ships[num_trade_ships].speed = 10.0;
  num_trade_ships++;

  if (num_trade_ships == trade_ship_buf_size) {
    double_buffer((void **) &trade_ships, &trade_ship_buf_size,
                  sizeof(TRADE_SHIP));
  }
}

/*
Click listener of trade with item menu merchant items
Args:
unsigned int merchant_ui_index
  merchant item index, from 0 to 7
*/
void on_click_ui_item(void *item_ui_index) {
  uintptr_t arg = (uintptr_t)item_ui_index;
  printf("Click Detected\n");
}


/*
Hover listener of trade with item menu merchant items
Args:
unsigned int merchant_ui_index
  merchant item index, from 0 to 7

*/
void on_hover_ui_item(void *item_ui_index) {
  uintptr_t arg = (uintptr_t)item_ui_index;
  printf("Hover Detected\n");
}

/*
Click listener of merchant items page up
Args:
int isMerchant
  is clicked page up button belongs to merchant or player
*/
void on_click_page_up(void *isMerchant) {
  uintptr_t arg = (uintptr_t)isMerchant;
  printf("Click Detected\n");
}

/*
Click listener of merchant items page up
Args:
int isMerchant
  is clicked page up button belongs to merchant or player
*/
void on_click_page_down(void *isMerchant) {
  uintptr_t arg = (uintptr_t)isMerchant;
  printf("Click Detected\n");
}

/*
Trade with both sides item selected
*/
void on_click_trade() {
  printf("Click Detected\n");
}
