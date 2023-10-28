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
  trade.ui_button_merchant_item_page_up = get_ui_component_by_ID(TRADE_BUTTON_MERCHANT_ITEM_PAGE_UP);
  trade.ui_button_merchant_item_page_down = get_ui_component_by_ID(TRADE_BUTTON_MERCHANT_ITEM_PAGE_DOWN);
  trade.ui_button_player_item_page_up = get_ui_component_by_ID(TRADE_BUTTON_PLAYER_ITEM_PAGE_UP);
  trade.ui_button_player_item_page_down = get_ui_component_by_ID(TRADE_BUTTON_PLAYER_ITEM_PAGE_DOWN);
  trade.ui_text_on_hover_item = get_ui_component_by_ID(TRADE_TEXT_ON_HOVER_ITEM);
  trade.ui_text_merchant_value = get_ui_component_by_ID(TRADE_TEXT_MERCHANT_VALUE);
  trade.ui_text_player_value = get_ui_component_by_ID(TRADE_TEXT_PLAYER_VALUE);
  trade.ui_button_trade = get_ui_component_by_ID(TRADE_BUTTON_TRADE);
  trade.ui_text_event_prompt = get_ui_component_by_ID(TRADE_TEXT_EVENT_PROMPT);

  trade.merchant_page = 0;
  trade.player_page = 0;

  trade.merchant_value = 0;
  trade.player_value = 0;

  for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
    trade.merchant_item_selected[i] = 0;
  }
  for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
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
    float pos_x = -0.8 + (i % 2) * 0.2;
    float pos_y = 0.4 - (i / 2) * 0.2;
    init_menu(
      (vec2) { pos_x, pos_y }, // position
      on_click_merchant_item, // on_click
      on_hover_merchant_item, // on_hover
      (void *) i, // on_click_args
      (void *) i, // on_hover_args
      "UI_M", // text
      0, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.2, // width
      0.2, // height
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      trade.ui_merchant_items[i] // dest
    );
  }

  // Init player items
  for (uintptr_t i = 0; i < MAX_ITEMS_PER_PAGE; i++) {
    float pos_x = 0.6 + (i % 2) * 0.2;
    float pos_y = 0.4 - (i / 2) * 0.2;
    init_menu(
      (vec2) { pos_x, pos_y }, // position
      on_click_player_item, // on_click
      on_hover_player_item, // on_hover
      (void *) i, // on_click_args
      (void *) i,// on_hover_args
      "UI_P", // text
      0, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.2, // width
      0.2, // height
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      trade.ui_player_items[i] // dest
    );
  }

  // Init merchant page up and down
  init_menu(
    (vec2) { -0.38, 0.3 }, // position
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
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_merchant_item_page_up // dest
  );

  init_menu(
    (vec2) { -0.38, -0.1 }, // position
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
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_merchant_item_page_down // dest
  );

  // Init player page up and down
  init_menu(
    (vec2) { 0.38, 0.3 }, // position
    on_click_page_up, // on_click
    NULL, // on_hover
    (void *) 0, // on_click_args
    NULL, // on_hover_args
    "PG_UP", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_player_item_page_up // dest
  );

  init_menu(
    (vec2) { 0.38, -0.1 }, // position
    on_click_page_down, // on_click
    NULL, // on_hover
    (void *) 0, // on_click_args
    NULL, // on_hover_args
    "PG_DN", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_player_item_page_down // dest
  );

  // Init on-hover prompt
  init_menu(
    (vec2) { 0, 0.62 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_on_hover_item // dest
  );
  trade.ui_text_on_hover_item->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!trade.ui_text_on_hover_item->text) {
    return;
  }
  trade.ui_text_on_hover_item->text[MAX_UI_TEXT_LENGTH - 1] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_on_hover_item->text, "[Name] P [Price] Q [Quantity] S [Num Select]");

  // Init merchant and player value text
  init_menu(
    (vec2) { -0.7, -0.42 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.4, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_merchant_value // dest
  );
  trade.ui_text_merchant_value->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!trade.ui_text_merchant_value) {
    return;
  }
  trade.ui_text_merchant_value->text[MAX_UI_TEXT_LENGTH - 1] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_merchant_value->text, "MERCHANT VALUE [0]");

  init_menu(
    (vec2) { 0.7, -0.42 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.4, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_player_value // dest
  );
  trade.ui_text_player_value->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!trade.ui_text_player_value) {
    return;
  }
  trade.ui_text_player_value->text[MAX_UI_TEXT_LENGTH - 1] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_player_value->text, "PLAYER VALUE [0]");

  // Init trade button
  init_menu(
    (vec2) { 0, -0.42 }, // position
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
    0.3, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_button_trade // dest
  );

  // Init trade event prompt
  init_menu(
    (vec2) { 0, 0.1 }, // position
    NULL, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_text_event_prompt // dest
  );
  trade.ui_text_event_prompt->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!trade.ui_text_event_prompt) {
    return;
  }
  trade.ui_text_event_prompt->text[MAX_UI_TEXT_LENGTH - 1] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_event_prompt->text, "EVENT PROMPT");

  e_player.money = 2000;
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
  trade.ui_text_event_prompt->enabled = 0;
}

/* Derender trade menu */
void close_trade() {
  trade.type = INVALID_TRADE;
  for (int i = 0; i < 9; i++) {
    trade.ui_listing[i]->enabled = 0;
  }
  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i]->enabled = 0;
    trade.ui_player_items[i]->enabled = 0;
  }
  trade.ui_button_merchant_item_page_up->enabled = 0;
  trade.ui_button_merchant_item_page_down->enabled = 0;
  trade.ui_button_player_item_page_up->enabled = 0;
  trade.ui_button_player_item_page_down->enabled = 0;
  trade.ui_text_on_hover_item->enabled = 0;
  trade.ui_text_merchant_value->enabled = 0;
  trade.ui_text_player_value->enabled = 0;
  trade.ui_button_trade->enabled = 0;
  trade.ui_text_event_prompt->enabled = 0;

  trade.merchant_page = 0;
  trade.player_page = 0;

  trade.merchant_value = 0;
  trade.player_value = 0;

  for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
    trade.merchant_item_selected[i] = 0;
  }
  for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
    trade.player_item_selected[i] = 0;
  }

  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i]->text = "EMPTY";
    trade.ui_player_items[i]->text = "EMPTY";
  }
}

/*
Set trade menu with given trade type, must call when open a trade menu
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
          ITEM_IDS item_id_at_index_i = get_merchant_listing_item_by_index(cur_merchant, i)->item_id;
          trade.ui_listing[i]->text = get_item_name_by_ID(item_id_at_index_i);
        }
        return 1;
      }
      // SELL: Set each listing slots with invotory slots
      case SELL: {
        for (int i = 0; i < 9; i++) {
          ITEM_IDS item_id_at_index_i = get_player_inventory_slot_by_index(i)->item_id;
          trade.ui_listing[i]->text = get_item_name_by_ID(item_id_at_index_i);
        }
        return 1;
      }
      case WITH_ITEM: {
        trade.merchant_page = 0;
        trade.player_page = 0;

        trade.merchant_value = 0;
        trade.player_value = 0;
        
        for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
          trade.merchant_item_selected[i] = 0;
        }
        for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
          trade.player_item_selected[i] = 0;
        }
        for (int i = 0; i < 8; i++) {
          ITEM_IDS merchant_item_id_at_index_i = get_merchant_listing_item_by_index(cur_merchant, i)->item_id;
          trade.ui_merchant_items[i]->text = get_item_name_by_ID(merchant_item_id_at_index_i);
          ITEM_IDS player_item_id_at_index_i = get_player_inventory_slot_by_index(i)->item_id;
          trade.ui_player_items[i]->text = get_item_name_by_ID(player_item_id_at_index_i);
        }
      }
      default: {
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
      slot = search_player_inventory_by_ID(
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
  if (set_trade(WITH_ITEM)) {
    open_trade();
  }
}

/* Render sell menu, called by dialog "sell" button */
void open_sell() {
  close_dialog();
  if (set_trade(WITH_ITEM)) {
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
  for (int i = 0; i < 20; i++) {
    trade_ships[num_trade_ships].desc[i] = merchant_name_list[target_merch->name][i];
  }
  num_trade_ships++;

  if (num_trade_ships == trade_ship_buf_size) {
    double_buffer((void **) &trade_ships, &trade_ship_buf_size,
                  sizeof(TRADE_SHIP));
  }
}

/*
Click listener of trade with item menu merchant items
Args:
unsigned int merchant_item_index
  current page merchant item index, from 0 to 7
*/
void on_click_merchant_item(void *merchant_item_index) {
  printf("Merchant Item Page [%d] Index [%lld] [Click] Detected\n", trade.merchant_page, (uintptr_t)merchant_item_index);
  int index = (uintptr_t)merchant_item_index + trade.merchant_page * 8;
  if (get_merchant_listing_item_by_index(cur_merchant, index)->item_id != EMPTY && get_merchant_listing_item_by_index(cur_merchant, index)->item_id != INVALID_ITEM) {
    if (trade.merchant_item_selected[index] + 1 <= get_merchant_listing_item_by_index(cur_merchant, index)->quantity) {
      // If do not reach the maximum quatity, add selection
      trade.merchant_item_selected[index] += 1;
      trade.merchant_value += get_item_info_by_ID(get_merchant_listing_item_by_index(cur_merchant, index)->item_id).value;
    } else {
      // If already reach the maximum quatity, reset to zero
      trade.merchant_item_selected[index] = 0;
      trade.merchant_value -= get_item_info_by_ID(get_merchant_listing_item_by_index(cur_merchant, index)->item_id).value * get_merchant_listing_item_by_index(cur_merchant, index)->quantity;
    }
  }
  sprintf(trade.ui_text_merchant_value->text, "MERCHANT VALUE [%d]", trade.merchant_value);
  ITEM_IDS merchant_item_id_at_index = get_merchant_listing_item_by_index(cur_merchant, index)->item_id;
  int price = get_item_info_by_ID(merchant_item_id_at_index).value;
  int quatity = get_merchant_listing_item_by_index(cur_merchant, index)->quantity;
  sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ", get_item_name_by_ID(merchant_item_id_at_index), price, quatity, trade.merchant_item_selected[index]);
}

/*
Hover listener of trade with item menu merchant items
Args:
unsigned int merchant_item_index
  current page merchant item index, from 0 to 7
*/
void on_hover_merchant_item(void *merchant_item_index) {
  printf("Merchant Item Page [%d] Index [%lld] [Hover] Detected\n", trade.merchant_page, (uintptr_t)merchant_item_index);
  int index = (uintptr_t)merchant_item_index + trade.merchant_page * 8;
  ITEM_IDS merchant_item_id_at_index = get_merchant_listing_item_by_index(cur_merchant, index)->item_id;
  int price = get_item_info_by_ID(merchant_item_id_at_index).value;
  int quatity = get_merchant_listing_item_by_index(cur_merchant, index)->quantity;
  sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ", get_item_name_by_ID(merchant_item_id_at_index), price, quatity, trade.merchant_item_selected[index]);
}

/*
Click listener of trade with item menu player items
Args:
unsigned int player_item_index
  current page player item index, from 0 to 7
*/
void on_click_player_item(void *player_item_index) {
  printf("Player Item Page [%d] Index [%lld] [Click] Detected\n", trade.player_page, (uintptr_t)player_item_index);
  int index = (uintptr_t)player_item_index + trade.player_page * 8;
  if (get_player_inventory_slot_by_index(index)->item_id != EMPTY && get_player_inventory_slot_by_index(index)->item_id != INVALID_ITEM) {
    if (trade.player_item_selected[index] + 1 <= get_player_inventory_slot_by_index(index)->quantity) {
      // If do not reach the maximum quatity, add selection
      trade.player_item_selected[index] +=1;
      trade.player_value += get_item_info_by_ID(get_player_inventory_slot_by_index(index)->item_id).value;
    } else {
      // If already reach the maximum quatity, reset to zero
      trade.player_item_selected[index] = 0;
      trade.player_value -= get_item_info_by_ID(get_player_inventory_slot_by_index(index)->item_id).value * get_player_inventory_slot_by_index(index)->quantity;
    }
  }
  sprintf(trade.ui_text_player_value->text, "PLAYER VALUE [%d]", trade.player_value);
  ITEM_IDS player_item_id_at_index = get_player_inventory_slot_by_index(index)->item_id;
  int price = get_item_info_by_ID(player_item_id_at_index).value;
  int quatity = get_player_inventory_slot_by_index(index)->quantity;
  sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ", get_item_name_by_ID(player_item_id_at_index), price, quatity, trade.player_item_selected[index]);
}

/*
Hover listener of trade with item menu player items
Args:
unsigned int player_item_index
  current page player item index, from 0 to 7
*/
void on_hover_player_item(void *player_item_index) {
  printf("Player Item Page [%d] Index [%lld] [Hover] Detected\n", trade.player_page, (uintptr_t)player_item_index);
  int index = (uintptr_t)player_item_index + trade.player_page * 8;
  ITEM_IDS player_item_id_at_index = get_player_inventory_slot_by_index(index)->item_id;
  int price = get_item_info_by_ID(player_item_id_at_index).value;
  int quatity = get_player_inventory_slot_by_index(index)->quantity;
  sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ", get_item_name_by_ID(player_item_id_at_index), price, quatity, trade.player_item_selected[index]);
}

/*
Click listener of items page up
Args:
int isMerchant
  is clicked page up button belongs to merchant or player
*/
void on_click_page_up(void *isMerchant) {
  if (isMerchant) {
    if (cur_merchant->num_listings == 0) {
      return;
    }
    (trade.merchant_page == 0) ? trade.merchant_page = (((cur_merchant->num_listings / 8) == 0) ? 0 : (cur_merchant->num_listings/8) - 1) : (trade.merchant_page -= 1);
    printf("Merchant Page Down Detected | Current Page [%d]\n", trade.merchant_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      ITEM_IDS merchant_item_id_at_index_i = get_merchant_listing_item_by_index(cur_merchant, index)->item_id;
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(merchant_item_id_at_index_i);
    }
  } else {
    (trade.player_page == 0) ? (trade.player_page = 1) : (trade.player_page = 0);
    printf("Player Page Down Detected | Current Page [%d]\n", trade.player_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      ITEM_IDS player_item_id_at_index_i = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(player_item_id_at_index_i);
    }
  }
}

/*
Click listener of items page down
Args:
int isMerchant
  is clicked page down button belongs to merchant or player
*/
void on_click_page_down(void *isMerchant) {
  if (isMerchant) {
    if (cur_merchant->num_listings == 0) {
      return;
    }
    (trade.merchant_page + 1 == cur_merchant->num_listings / 8) ? (trade.merchant_page = 0) : (trade.merchant_page += 1);
    printf("Merchant Page Up Detected | Current Page [%d]\n", trade.merchant_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      ITEM_IDS merchant_item_id_at_index_i = get_merchant_listing_item_by_index(cur_merchant, index)->item_id;
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(merchant_item_id_at_index_i);
    }
  } else {
    (trade.player_page + 1 > 1) ? (trade.player_page = 1) : (trade.player_page += 1);
    printf("Player Page Up Detected | Current Page [%d]\n", trade.player_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.player_page * 8;
      ITEM_IDS player_item_id_at_index_i = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(player_item_id_at_index_i);
    }
  }
}

/*
Trade with both sides item selected
TODO: DO NOT DEAL WITH LIMITED INVENTORY SPACE
*/
void on_click_trade() {
  printf("Trade Button Click Detected\n");
  // Check player value > merchant value
  if (trade.player_value >= trade.merchant_value) {
    // Add the selected items to player inventory
    for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
      if (trade.merchant_item_selected[i] > 0) {
        ITEM_IDS item_id = get_merchant_listing_item_by_index(cur_merchant, i)->item_id;
        if (search_player_inventory_by_ID(item_id)) {
          search_player_inventory_by_ID(item_id)->quantity += trade.merchant_item_selected[i];
        } else {
          I_SLOT *empty_slot = get_player_first_empty_inventory_slot();
          empty_slot->item_id = item_id;
          empty_slot->quantity = trade.merchant_item_selected[i];
        }
      }
    }
    // Add the selected items to merchant listing
    for (int i = 0 ; i < cur_merchant->num_listings; i++) {
        if (trade.player_item_selected[i] > 0) {
          ITEM_IDS item_id = get_player_inventory_slot_by_index(i)->item_id;
        if (search_merchant_listing_by_ID(cur_merchant, item_id)) {
          search_merchant_listing_by_ID(cur_merchant, item_id)->quantity += trade.player_item_selected[i];
        } else {
          LISTING *empty_slot = get_merchant_first_empty_listing(cur_merchant);
          empty_slot->item_id = item_id;
          empty_slot->quantity = trade.player_item_selected[i];
          cur_merchant->num_listings ++;
          if (cur_merchant->num_listings == cur_merchant->listings_buf_size) {
            if (double_buffer((void  **)&cur_merchant->listings, &cur_merchant->listings_buf_size, sizeof(LISTING))) {
              fprintf(stderr, "Cannot double buffer size");
              exit(1);
            }
          }
        }
      }
    }
    // for (int i = 0; i < cur_merchant->num_listings; i++) {
    //   if (trade.player_item_selected[i] > 0) {
    //     ITEM_IDS item_id = get_player_inventory_slot_by_index(i)->item_id;
    //     if (search_merchant_listing_by_ID(cur_merchant, item_id)) {
    //       search_merchant_listing_by_ID(cur_merchant, item_id)->quantity += trade.player_item_selected[i];
    //     } else {
    //       LISTING *empty_slot = get_merchant_first_empty_listing(cur_merchant);
    //       empty_slot->item_id = item_id;
    //       empty_slot->quantity = trade.player_item_selected[i];
    //     }
    //   }
    // }
    // Remove the selected items from player inventory
    for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
      if (trade.player_item_selected[i] > 0) {
        get_player_inventory_slot_by_index(i)->quantity -= trade.player_item_selected[i];
        if (get_player_inventory_slot_by_index(i)->quantity <= 0) {
          get_player_inventory_slot_by_index(i)->item_id = EMPTY;
          get_player_inventory_slot_by_index(i)->quantity = 0;
        }
      }
    }
    // Remove the selected items from merchant inventory
    for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
      if (trade.merchant_item_selected[i] > 0) {
        get_merchant_listing_item_by_index(cur_merchant, i)->quantity -= trade.merchant_item_selected[i];
        if (get_merchant_listing_item_by_index(cur_merchant, i)->quantity <= 0) {
          get_merchant_listing_item_by_index(cur_merchant, i)->item_id = EMPTY;
          get_merchant_listing_item_by_index(cur_merchant, i)->quantity = 0;
        }
      }
    }
    // Set Values Back
    trade.player_value = 0;
    sprintf(trade.ui_text_player_value->text, "PLAYER VALUE [%d]", trade.player_value);
    trade.merchant_value = 0;
    sprintf(trade.ui_text_merchant_value->text, "MERCHANT VALUE [%d]", trade.merchant_value);

    for (int i = 0; i < MAX_MERCHANT_ITEM; i++) {
      trade.merchant_item_selected[i] = 0;
      trade.player_item_selected[i] = 0;
    }
    // Refresh the UI
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      ITEM_IDS merchant_item_id_at_index_i = get_merchant_listing_item_by_index(cur_merchant, index)->item_id;
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(merchant_item_id_at_index_i);
    }
    for (int i = 0; i < 8; i++) {
      int index = i + trade.player_page * 8;
      ITEM_IDS player_item_id_at_index_i = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(player_item_id_at_index_i);
    }
    // Show prompt
    sprintf(trade.ui_text_event_prompt->text, " Trade Successful ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
  } else {
    // Show prompt
    sprintf(trade.ui_text_event_prompt->text, " Player Value Insufficient ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
  }
}
