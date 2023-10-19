#include <trade.h>

void init_trade() {
  trade.type = INVALID_TRADE;
  trade.ui_listing[0] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_0);
  trade.ui_listing[1] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_1);
  trade.ui_listing[2] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_2);
  trade.ui_listing[3] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_3);
  trade.ui_listing[4] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_4);
  trade.ui_listing[5] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_5);
  trade.ui_listing[6] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_6);
  trade.ui_listing[7] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_7);
  trade.ui_listing[8] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_8);

  // Init listings
  vec2 listing_0_position = { -0.5, 0.5 };
  init_menu(
    listing_0_position, // position
    on_click_ui_listing, // on_click
    (void *) 0, // on_click_args
    "UI_L_1", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 1, // on_click_args
    "UI_L_2", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 2, // on_click_args
    "UI_L_3", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 3, // on_click_args
    "UI_L_4", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 4, // on_click_args
    "UI_L_5", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 5, // on_click_args
    "UI_L_6", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 6, // on_click_args
    "UI_L_7", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 7, // on_click_args
    "UI_L_8", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
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
    (void *) 8, // on_click_args
    "UI_L_9", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
    0.5, // width
    0.5, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    trade.ui_listing[8] // dest
  );
}

void free_trade() {
  return;
}

void open_trade() {
  switch (trade.type) {
    case BUY: {
      for (int i = 0; i < 9; i++) {
        trade.ui_listing[i]->enabled = 1;
      }
      break;
    }
    case SELL: {
      for (int i = 0; i < 9; i++) {
        trade.ui_listing[i]->enabled = 1;
      }
      break;
    }
    default: {
      for (int i = 0; i < 9; i++) {
        trade.ui_listing[i]->enabled = 1;
      }
      break;
    }
  }
}

void close_trade() {
  for (int i = 0; i < 9; i++) {
    trade.ui_listing[i]->enabled = 0;
  }
}

int set_trade(T_TRADE dialog_type) {
  if (cur_merchant) {
    trade.type = dialog_type;
    switch (dialog_type) {
      case BUY: {
        for (int i = 0; i < 9; i++) {
          trade.ui_listing[i]->text = get_item_name_by_ID(
                                       get_merchant_listing_item_by_number(
                                       cur_merchant, i + 1)->item_id);
        }
        return 1;
      }
  // Attacking
      case SELL: {
        for (int i = 0; i < 9; i++) {
          trade.ui_listing[i]->text = get_item_name_by_ID(
                                       get_player_inventory_slot_by_number(
                                       i + 1)->item_id);
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

void on_click_ui_listing(void *listing_index) {
  long listing = (long) listing_index;
  LISTING *listing_slot;
  I_SLOT *slot;
  ITEM item;
  if (trade.type == BUY) {
    item = get_item_info_by_name(trade.ui_listing[listing]->text);
    listing_slot = get_merchant_listing_item_by_number(cur_merchant,
                                                       listing + 1);
    // TODO: NOT ENOUGH MONEY PROMPT
    if (e_player.money >= item.value && listing_slot->quantity > 0) {
      listing_slot->quantity -= 1;
      e_player.money -= item.value;
      // INVENTORY ADD
      slot = search_player_inventory_with_ID(
                                get_merchant_listing_item_by_number(
                                cur_merchant, listing + 1)->item_id);
      if (slot) {
        slot->quantity += 1;
      } else if (get_player_first_empty_inventory_slot()) {
        I_SLOT *empty_inventory_slot = get_player_first_empty_inventory_slot();
        empty_inventory_slot->item_id = get_merchant_listing_item_by_number(
                                        cur_merchant, listing + 1)->item_id;
        empty_inventory_slot->quantity = 1;
      }

      // Reset Listing UI Component
      if (listing_slot->quantity <= 0) {
        listing_slot->item_id = 0;
        trade.ui_listing[listing]->text = "SOLD";
      }

      cur_merchant->relationship += 10.0;
      if (cur_merchant->relationship > 100.0) {
        cur_merchant->relationship = 100.0;
      }
    }
  } else if (trade.type == SELL && get_player_inventory_slot_by_number(listing + 1)->quantity > 0) {
    slot = get_player_inventory_slot_by_number(listing + 1);
    slot->quantity -= 1;
    int item_identifier = slot->item_id;
    item = get_item_info_by_name(trade.ui_listing[listing]->text);
    e_player.money += item.value;
    if (slot->quantity <= 0) {
      slot->item_id = 0;
      trade.ui_listing[listing]->text = "SOLD";
    }

    /* Find the first available listing location in the merchant inventory and populate */
    /* with item that was just sold to it */
    for (int i = 0; i < 9; i++) {
      listing_slot = get_merchant_listing_item_by_number(cur_merchant, i + 1);
      if (listing_slot->item_id == item_identifier) {
        listing_slot->quantity++;
        break;
      } else if (listing_slot->quantity == 0) {
        listing_slot->quantity++;
        listing_slot->item_id = item_identifier;
        break;
      }
    }

    cur_merchant->relationship += 10.0;
    if (cur_merchant->relationship > 100.0) {
      cur_merchant->relationship = 100.0;
    }
  }
}

void open_buy() {
  close_dialog();
  if (set_trade(BUY)) {
    open_trade();
  }
}

void open_sell() {
  close_dialog();
  if (set_trade(SELL)) {
    open_trade();
  }
}

void open_establish_trade_route() {
  MERCHANT *target_merch = cur_merchant;
  CHUNK *target_chunk = NULL;
  for (int i = 0; i < num_trade_ships; i++) {
    target_chunk = chunk_buffer + trade_ships[i].target_chunk_index;
    if (target_chunk->coords[0] == target_merch->chunk[0] &&
        target_chunk->coords[1] == target_merch->chunk[1]) {
      dialog.ui_text_schedule_trade_route_prompt->text = "You Already Have a Trade Route";
      dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
      return;
    }
  }
  dialog.ui_text_schedule_trade_route_prompt->text = "Trade Route Established";
  dialog.ui_text_schedule_trade_route_prompt->enabled = 1;
  time_schdule_trade_toute_prompt = 2.0;
  // delay

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
