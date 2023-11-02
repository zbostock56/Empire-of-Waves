#include <trade.h>
#include <dialog_str.h>

/* Init trade menu */
int init_trade() {
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

  trade.barter_range = 0.1;
  trade.merchant_item_selected = NULL;

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
    fprintf(stderr, "trade.c: Failed to allocate on-hover text buffer\n");
    return -1;
  }
  trade.ui_text_on_hover_item->text[0] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_on_hover_item->text,
         " [Name] P [Price] Q [Quantity] S [Num Select] ");

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
    fprintf(stderr, "trade.c: Failed to allocate merchant value buffer\n");
    return -1;
  }
  trade.ui_text_merchant_value->text[0] = '\0'; // Ensures null termination
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
    fprintf(stderr, "trade.c: Failed to allocate player value buffer\n");
    return -1;
  }
  trade.ui_text_player_value->text[0] = '\0'; // Ensures null termination
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
    fprintf(stderr, "trade.c: Failed to allocate event prompt buffer\n");
    return -1;
  }
  trade.ui_text_event_prompt->text[0] = '\0'; // Ensures null termination
  strcpy(trade.ui_text_event_prompt->text, "EVENT PROMPT");

  return 0;
}

void free_trade() {
  free(trade.ui_text_on_hover_item->text);
  free(trade.ui_text_merchant_value->text);
  free(trade.ui_text_player_value->text);
  free(trade.ui_text_event_prompt->text);

  trade.ui_text_on_hover_item->text = NULL;
  trade.ui_text_merchant_value = NULL;
  trade.ui_text_player_value = NULL;
  trade.ui_text_event_prompt = NULL;
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

  // Enable relationship bar
  dialog.ui_text_relationship->enabled = 1;
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

  if (trade.merchant_item_selected) {
    for (int i = 0; i < trade.merchant->num_listings; i++) {
      trade.merchant_item_selected[i] = 0;
    }
    free(trade.merchant_item_selected);
    trade.merchant_item_selected = NULL;
  }
  for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
    trade.player_item_selected[i] = 0;
  }

  for (int i = 0; i < 8; i++) {
    trade.ui_merchant_items[i]->text = "EMPTY";
    trade.ui_player_items[i]->text = "EMPTY";
  }

  // Set UI text back
  sprintf(trade.ui_text_merchant_value->text, "MERCHANT VALUE [%d]",
          trade.merchant_value);
  sprintf(trade.ui_text_player_value->text, "PLAYER VALUE [%d]",
          trade.player_value);
  sprintf(trade.ui_text_on_hover_item->text, " [Name] P [Price] Q [Quantity] S [Num Select] ");

  // Disable relationship bar
  dialog.ui_text_relationship->enabled = 0;
}

void prompt_plundered_trade_ship() {
  set_prompt("A trade ship has been plundered!");
}

/*
Set trade menu with given trade type, must call when open a trade menu
Args:
T_TRADE trade_type
  type of trade, e.g. BUY
*/
int set_trade(MERCHANT *merchant, T_TRADE trade_type) {
  if (merchant) {
    trade.merchant = merchant;
    trade.type = trade_type;
    // Set trade UI components with given trade type
    switch (trade_type) {
      // BUY: Set each listing slots with merchant listings
      case BUY: {
        for (int i = 0; i < 9; i++) {
          LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                                i);
          ITEM_IDS item_id = listing->item_id;
          trade.ui_listing[i]->text = get_item_name_by_ID(item_id);
        }
        return 1;
      }
      // SELL: Set each listing slots with invotory slots
      case SELL: {
        for (int i = 0; i < 9; i++) {
          I_SLOT *i_slot = get_player_inventory_slot_by_index(i);
          ITEM_IDS item_id_at_index_i = i_slot->item_id;
          trade.ui_listing[i]->text = get_item_name_by_ID(item_id_at_index_i);
        }
        return 1;
      }
      case WITH_ITEM: {
        trade.merchant_page = 0;
        trade.player_page = 0;

        trade.merchant_value = 0;
        trade.player_value = 0;

        trade.merchant_item_selected = malloc(sizeof(int) *
                                              merchant->listings_buf_size);
        if (trade.merchant_item_selected == NULL) {
          fprintf(stderr, "trade.c: Unable to allocate merchant selections\n");
          return -1;
        }
        for (int i = 0; i < merchant->num_listings; i++) {
          trade.merchant_item_selected[i] = 0;
        }

        for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
          trade.player_item_selected[i] = 0;
        }
        for (int i = 0; i < 8; i++) {
          LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                                i);
          ITEM_IDS listing_id = EMPTY;
          if (listing) {
            listing_id = listing->item_id;
          }
          trade.ui_merchant_items[i]->text = get_item_name_by_ID(listing_id);

          I_SLOT *i_slot = get_player_inventory_slot_by_index(i);
          ITEM_IDS i_slot_id = i_slot->item_id;
          trade.ui_player_items[i]->text = get_item_name_by_ID(i_slot_id);
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
    listing_slot = get_merchant_listing_item_by_index(trade.merchant,
                                                      listing_index);
    // If player have enough money to buy the listing item
    // Decrease the quatity of merchant listing slot
    if (e_player.money >= item.value && listing_slot->quantity > 0) {
      listing_slot->quantity -= 1;
      e_player.money -= item.value;
      // Add listing item to player inventory
      // If player already have this item, increase its quatity
      // Other wise, add the item to the first empty slot
      slot = search_player_inventory_by_ID(listing_slot->item_id);
      if (slot) {
        slot->quantity += 1;
      } else if (get_player_first_empty_inventory_slot()) {
        I_SLOT *empty_inventory_slot = get_player_first_empty_inventory_slot();
        empty_inventory_slot->item_id = listing_slot->item_id;
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
      trade.merchant->relationship += 10.0;
      if (trade.merchant->relationship > 100.0) {
        trade.merchant->relationship = 100.0;
      }

/*#ifndef __linux__
      printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
             listing_index + 1, trade.ui_listing[listing_index]->text,
             get_merchant_listing_item_by_index(trade.merchant,
                                                listing_index)->quantity);
#else
      printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
             listing_index + 1, trade.ui_listing[listing_index]->text,
             get_merchant_listing_item_by_index(trade.merchant,
                                                listing_index)->quantity);
#endif*/
    }
  } else if (trade.type == SELL &&
             get_player_inventory_slot_by_index(listing_index)->quantity > 0) {
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
      listing_slot = get_merchant_listing_item_by_index(trade.merchant, i);
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
    trade.merchant->relationship += 10.0;
    if (trade.merchant->relationship > 100.0) {
      trade.merchant->relationship = 100.0;
    }

/*#ifndef __linux__
    printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
           listing_index + 1, trade.ui_listing[listing_index]->text,
           get_player_inventory_slot_by_index(listing_index)->quantity);
#else
    printf("**** [SLOT %ld] [ITEM \"%s\"] [QUATITY %d] ****\n",
           listing_index + 1, trade.ui_listing[listing_index]->text,
           get_player_inventory_slot_by_index(listing_index)->quantity);
#endif*/
  }
}

/* Render buy menu, called by dialog "buy" button */
void open_buy() {
  close_dialog();
  if (set_trade(dialog.merchant, WITH_ITEM)) {
    open_trade();
  }
}

/* Render sell menu, called by dialog "sell" button */
void open_sell() {
  close_dialog();
  if (set_trade(dialog.merchant, WITH_ITEM)) {
    open_trade();
  }
}

/* Click lisnter of establish trade route button */
void trade_route_handler(int island_index) {
  MERCHANT *target_merch = dialog.merchant;

  // Check if player already have a trade route to this island
  if (target_merch->has_trade_route) {
    dialog.ui_text_schedule_trade_route_prompt->text = "Trade Route Cancelled";
    dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
    time_schdule_trade_toute_prompt = 1.0;
    restore_trade_ship_mercs(dialog.merchant->chunk, island_index);
    delete_trade_ship(dialog.merchant->chunk, island_index);
    target_merch->has_trade_route = 0;

    dialog.ui_button_trade_route->text = "2. Establish trade route";

    target_merch->relationship -= 10.0;
    if (target_merch->relationship < -100.0) {
      target_merch->relationship = -100.0;
    }
  } else {
    // Establish trade route and popup the prompt shows successful
    dialog.ui_text_schedule_trade_route_prompt->text = "Trade Route Established";
    dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
    time_schdule_trade_toute_prompt = 1.0;

    init_trade_ship(merchant_name_list[target_merch->name],
                    dialog.merchant->chunk, island_index);
    target_merch->has_trade_route = 1;

    dialog.ui_button_trade_route->text = "3. Cancel trade route";
  }
}

/*
Click listener of trade with item menu merchant items
Args:
unsigned int merchant_item_index
  current page merchant item index, from 0 to 7
*/
void on_click_merchant_item(void *merchant_item_index) {
  // printf("Merchant Item Page [%d] Index [%ld] [Click] Detected\n",
  //        trade.merchant_page, (uintptr_t)merchant_item_index);
  int index = (uintptr_t)merchant_item_index + trade.merchant_page * 8;
  LISTING *listing = get_merchant_listing_item_by_index(trade.merchant, index);
  if (listing) {
    ITEM_IDS item_id = listing->item_id;
    int price = get_item_info_by_ID(item_id).value;
    int quantity = listing->quantity;
    if (item_id != EMPTY && item_id != INVALID_ITEM) {
      if (trade.merchant_item_selected[index] + 1 <= listing->quantity) {
        // If do not reach the maximum quatity, add selection
        trade.merchant_item_selected[index] += 1;
        trade.merchant_value += price;
      } else {
        // If already reach the maximum quatity, reset to zero
        trade.merchant_item_selected[index] = 0;
        trade.merchant_value -= price * quantity;
      }
    }
    sprintf(trade.ui_text_merchant_value->text, "MERCHANT VALUE [%d]",
            trade.merchant_value);
    sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ",
            get_item_name_by_ID(item_id), price, quantity,
            trade.merchant_item_selected[index]);
  }
}

/*
Hover listener of trade with item menu merchant items
Args:
unsigned int merchant_item_index
  current page merchant item index, from 0 to 7
*/
void on_hover_merchant_item(void *merchant_item_index) {
  // printf("Merchant Item Page [%d] Index [%ld] [Hover] Detected\n",
  //        trade.merchant_page, (uintptr_t)merchant_item_index);
  int index = (uintptr_t)merchant_item_index + trade.merchant_page * 8;
  LISTING *listing = get_merchant_listing_item_by_index(trade.merchant, index);
  if (listing) {
    ITEM_IDS item_id = listing->item_id;
    int price = get_item_info_by_ID(item_id).value;
    int quatity = listing->quantity;
    sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ",
            get_item_name_by_ID(item_id), price, quatity,
            trade.merchant_item_selected[index]);
  } else {
    sprintf(trade.ui_text_on_hover_item->text, " [-] P [-] Q [-] S [-] ");
  }
}

/*
Click listener of trade with item menu player items
Args:
unsigned int player_item_index
  current page player item index, from 0 to 7
*/
void on_click_player_item(void *player_item_index) {
  // printf("Player Item Page [%d] Index [%ld] [Click] Detected\n",
  //        trade.player_page, (uintptr_t)player_item_index);
  int index = (uintptr_t)player_item_index + trade.player_page * 8;
  I_SLOT *i_slot = get_player_inventory_slot_by_index(index);
  ITEM_IDS item_id = i_slot->item_id;
  int price = get_item_info_by_ID(item_id).value;
  int quantity = i_slot->quantity;
  if (item_id != EMPTY && item_id != INVALID_ITEM) {
    if (trade.player_item_selected[index] + 1 <= quantity) {
      // If do not reach the maximum quatity, add selection
      trade.player_item_selected[index] +=1;
      trade.player_value += price;
    } else {
      // If already reach the maximum quatity, reset to zero
      trade.player_item_selected[index] = 0;
      trade.player_value -= price * quantity;
    }
  }
  sprintf(trade.ui_text_player_value->text, "PLAYER VALUE [%d]",
          trade.player_value);
  sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ",
          get_item_name_by_ID(item_id), price, quantity,
          trade.player_item_selected[index]);
}

/*
Hover listener of trade with item menu player items
Args:
unsigned int player_item_index
  current page player item index, from 0 to 7
*/
void on_hover_player_item(void *player_item_index) {
  // printf("Player Item Page [%d] Index [%ld] [Hover] Detected\n",
  //        trade.player_page, (uintptr_t)player_item_index);
  int index = (uintptr_t)player_item_index + trade.player_page * 8;
  I_SLOT *i_slot = get_player_inventory_slot_by_index(index);
  ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
  int price = get_item_info_by_ID(item_id).value;
  int quantity = i_slot->quantity;
  if (item_id != EMPTY) {
    sprintf(trade.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] S [%d] ",
          get_item_name_by_ID(item_id), price, quantity,
          trade.player_item_selected[index]);
  } else {
    sprintf(trade.ui_text_on_hover_item->text, " [-] P [-] Q [-] S [-] ");
  }
}

/*
Click listener of items page up, when click the button, page number minus one
Args:
int isMerchant
  is clicked page up button belongs to merchant or player
*/
void on_click_page_up(void *isMerchant) {
  if (isMerchant) {
    if (trade.merchant->num_listings == 0) {
      return;
    }
    if (trade.merchant_page == 0) {
      trade.merchant_page = (trade.merchant->num_listings - 1) / 8;
    } else {
      trade.merchant_page--;
    }
    // printf("Merchant Page Down Detected | Current Page [%d]\n",
    //        trade.merchant_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                            index);
      ITEM_IDS item_id = EMPTY;
      if (listing) {
        item_id = listing->item_id;
      }
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(item_id);
    }
  } else {
    if (trade.player_page == 0) {
      trade.player_page = 1;
    } else {
      trade.player_page = 0;
    }
    // printf("Player Page Down Detected | Current Page [%d]\n",
    //        trade.player_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.player_page * 8;
      ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(item_id);
    }
  }
}

/*
Click listener of items page down, when click the button, page number add one
Args:
int isMerchant
  is clicked page down button belongs to merchant or player
*/
void on_click_page_down(void *isMerchant) {
  if (isMerchant) {
    if (trade.merchant->num_listings == 0) {
      return;
    }
    int num_pages = ((trade.merchant->num_listings - 1) / 8) + 1;
    trade.merchant_page++;
    trade.merchant_page = trade.merchant_page % num_pages;
    //printf("Merchant Page Up Detected | Current Page [%d]\n",
    //       trade.merchant_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                            index);
      ITEM_IDS item_id = EMPTY;
      if (listing) {
        item_id = listing->item_id;
      }
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(item_id);
    }
  } else {
    if (trade.player_page == 1) {
      trade.player_page = 0;
    } else {
      trade.player_page = 1;
    }
    //printf("Player Page Up Detected | Current Page [%d]\n", trade.player_page);
    for (int i = 0; i < 8; i++) {
      int index = i + trade.player_page * 8;
      ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(item_id);
    }
  }
}

/*
Trade with both sides item selected
TODO: DO NOT DEAL WITH LIMITED INVENTORY SPACE
*/
void on_click_trade() {
  if (!check_fit(trade.merchant->listings, trade.merchant_item_selected,
                 trade.merchant->num_listings, e_player.inventory,
                 trade.player_item_selected, MAX_PLAYER_INV_SIZE)) {
    sprintf(trade.ui_text_event_prompt->text,
            " Not enough room in inventory ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
    return;
  }

  // printf("Trade Button Click Detected\n");
  // Check player value > merchant value
  if (trade.player_value == 0) {
    // Show prompt
    sprintf(trade.ui_text_event_prompt->text, " No Items Selected ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
    return;
  } else if (trade.player_value * (1 + trade.barter_range) >= trade.merchant_value) {
    // When player offer more value than expected, add relationship with the additional value
    if (trade.player_value > trade.merchant_value) {
      trade.merchant->relationship += (trade.player_value - trade.merchant_value);
      if (trade.merchant->relationship > 100.0) trade.merchant->relationship = 100.0;
    } else if (trade.player_value < trade.merchant_value) {
      // When relationship less than 20, cannot bartering
      if (trade.merchant->relationship < 20.0) {
        trade.merchant->relationship -= (trade.merchant_value - trade.player_value);
        if (trade.merchant->relationship < 0.0) trade.merchant->relationship = 0.0;
        // Show prompt
        sprintf(trade.ui_text_event_prompt->text, " Player Value Insufficient ");
        trade.ui_text_event_prompt->enabled = 1;
        time_trade_event_prompt = 2.0;
        return;
      }
      // When relationship less than 80 larger than 20, can bartering but
      // relationship will decrease with the additional value
      if (trade.merchant->relationship < 80.0) {
        trade.merchant->relationship -= (trade.merchant_value - trade.player_value);
      }
    }
    // Check player value > merchant value
    // Add the selected items to player inventory
    for (int i = 0; i < trade.merchant->num_listings; i++) {
      if (trade.merchant_item_selected[i] > 0) {
        LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                              i);
        ITEM_IDS item_id = listing->item_id;
        I_SLOT *i_slot = search_player_inventory_by_ID(item_id);
        if (i_slot) {
          i_slot->quantity += trade.merchant_item_selected[i];
        } else {
          I_SLOT *empty_slot = get_player_first_empty_inventory_slot();
          empty_slot->item_id = item_id;
          empty_slot->quantity = trade.merchant_item_selected[i];
        }
      }
    }
    // Add the selected items to merchant listing
    for (int i = 0 ; i < MAX_PLAYER_ITEM; i++) {
      if (trade.player_item_selected[i] > 0) {
        ITEM_IDS item_id = get_player_inventory_slot_by_index(i)->item_id;
        add_listing(trade.merchant, &trade.merchant_item_selected, item_id,
                    trade.player_item_selected[i]);
      }
    }
    // Remove the selected items from player inventory
    for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
      if (trade.player_item_selected[i] > 0) {
        I_SLOT *i_slot = get_player_inventory_slot_by_index(i);
        i_slot->quantity -= trade.player_item_selected[i];
        if (i_slot->quantity <= 0) {
          i_slot->item_id = EMPTY;
          i_slot->quantity = 0;
        }
      }
    }
    // Remove the selected items from merchant inventory
    for (int i = 0; i < trade.merchant->num_listings; i++) {
      if (trade.merchant_item_selected[i] > 0) {
        LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                              i);
        listing->quantity -= trade.merchant_item_selected[i];
        if (listing->quantity == 0) {
          listing->item_id = EMPTY;
        }
      }
    }
    for (int i = 0; i < trade.merchant->num_listings; i++) {
      if (trade.merchant->listings[i].item_id == EMPTY ||
          trade.merchant->listings[i].quantity == 0) {
        clear_listing(trade.merchant, i);
      }
    }

    // Currency Coalesce
    coalesce_currency(e_player.inventory, MAX_PLAYER_INV_SIZE);

    // Set Values Back
    trade.player_value = 0;
    sprintf(trade.ui_text_player_value->text, "PLAYER VALUE [%d]",
            trade.player_value);
    trade.merchant_value = 0;
    sprintf(trade.ui_text_merchant_value->text, "MERCHANT VALUE [%d]",
            trade.merchant_value);

    for (int i = 0; i < MAX_PLAYER_ITEM; i++) {
      trade.player_item_selected[i] = 0;
    }
    for (int i = 0; i < trade.merchant->num_listings; i++) {
      trade.merchant_item_selected[i] = 0;
    }
    // Refresh the UI
    for (int i = 0; i < 8; i++) {
      int index = i + trade.merchant_page * 8;
      LISTING *listing = get_merchant_listing_item_by_index(trade.merchant,
                                                            index);
      ITEM_IDS item_id = EMPTY;
      if (listing) {
        item_id = listing->item_id;
      }
      trade.ui_merchant_items[i]->text = get_item_name_by_ID(item_id);
    }
    for (int i = 0; i < 8; i++) {
      int index = i + trade.player_page * 8;
      ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
      trade.ui_player_items[i]->text = get_item_name_by_ID(item_id);
    }
    // Show prompt
    sprintf(trade.ui_text_event_prompt->text, " Trade Successful ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
    return;
  } else {
    // When player value is lower than barter range, decrease the relationship with the value
    trade.merchant->relationship -= (trade.merchant_value - trade.player_value);
    if (trade.merchant->relationship < 0.0) trade.merchant->relationship = 0.0;
    // Show prompt
    sprintf(trade.ui_text_event_prompt->text, " Player Value Insufficient ");
    trade.ui_text_event_prompt->enabled = 1;
    time_trade_event_prompt = 2.0;
    return;
  }
}
