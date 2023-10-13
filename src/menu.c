#include <menu.h>
#include <stdio.h>
#include <merchant.h>
#include <player.h>
#include <items.h>
#include <globals.h>
/*
                                     MENU.c
Defines the table of all global ui components that are used across the game,
and maps each ui_component with their respective handler. menu.h provides
external references to the component table and may be included in
any file which enables/disabled ui components.
*/

int double_buffer(void **, unsigned int *, unsigned int);
void chunk_from_coords(ivec2, CHUNK *);
float time_schdule_trade_toute_prompt;
int add_chunk(ivec2);

// Init global variables
DIALOG * dialog;
TRADE * trade;
STATUS * status;

// UI component table definition
UI_COMPONENT ui_tab[NUM_COMPONENTS];

UI_COMPONENT * get_ui_component_by_ID(UI_ID ui_id) {
  // Check for invalid ui_id
  if (ui_id < 0 || ui_id > NUM_COMPONENTS - 2) {
    return &ui_tab[0];
  }
  return &ui_tab[ui_id + 1]; // +1 to account for INVALID_MENU being -1
}

void init_menu(
  vec2 position, // Position of component anchor in screen space
                 // X: [-1.0, 1.0],
                 // Y: [-1.0, 1.0]
  void (*on_click)(void *), // Callback function for if component is clicked
                            // NULL if no action should be taken
  void *on_click_args, // Arguments that will be passed to on_click()
  char *text, // Text to be displayed over top the ui component
  int enabled, // Non-zero: Component is rendered
                // 0: Component is not rendered
  int textured, // Non-zero: Component will be rendered with a background
                // texture specified by "texture"
                // 0: Component will be rendered with a transparent
                // background
  unsigned int texture, // Background texture of component
  float text_padding, // Vertical padding of text inside ui component
                      // (units given in screen space)
  float text_scale, // Scale of ui component
  float width, // Non-zero: Fixed width of ui component
               // 0: Component width will automatically scale to fit the
               // width of the text
  float height, // Non-zero: fixed height of ui component
                // 0: Component height will automatically scale to fit the
                // height of the text
  PIVOT pivot, // Specifies the behavior of the "pivot" of the ui component
               // See PIVOT enum in ui_component.h
  TEXT_ANCHOR text_anchor, // Specifies the alignment of the text
                           // See TEXT_ANCHOR enum in ui_component.h
  UI_COMPONENT *dest
) {
  glm_vec2_copy(position, dest->position);
  dest->on_click = on_click;
  dest->on_click_args = on_click_args;
  dest->text = text;
  // sprintf(dest->text, "%s", text);
  dest->enabled = enabled;
  dest->textured = textured;
  dest->texture = texture;
  dest->text_padding = text_padding;
  dest->text_scale = text_scale;
  dest->width = width;
  dest->height = height;
  dest->pivot = pivot;
  dest->text_anchor = text_anchor;
}

// TEMPORARY TEST CALLBACK -- TO BE DELETED
void test_callback(void *args) {
  get_ui_component_by_ID(TEST_MENU)->text = "HIT!";
}

void init_menus() {
  // Populate ui_tab

  // TEMPORARY TEST UI COMPONENT -- TO BE DELETED
  vec2 test_position = { 0.0, -0.75 };
  init_menu(
    test_position, // position
    test_callback, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "Hello, this is a merchant!", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    2.0, // text_scale
    0, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_RIGHT, // text_anchor
    get_ui_component_by_ID(TEST_MENU) // dest
  );

  // Embark/Disembark prompt
  vec2 prompt_pos = { 0.0, -0.25 };
  init_menu(
    prompt_pos,
    NULL, NULL,
    "Press 'e' to embark",
    0,
    0,
    0,
    0.0,
    1.5,
    0.0,
    0.0,
    PIVOT_CENTER,
    T_CENTER,
    get_ui_component_by_ID(EMBARK_PROMPT)
  );

  // Interaction prompt
  init_menu(
    prompt_pos,
    NULL, NULL,
    "Press 'e' to interact",
    0,
    0,
    0,
    0.0,
    1.5,
    0.0,
    0.0,
    PIVOT_CENTER,
    T_CENTER,
    get_ui_component_by_ID(INTERACT_PROMPT)
  );

  // Init dialog menu
  dialog = init_dialog();
  // Init trade menu
  trade = init_trade();
  // Give Init Money
  e_player.money = 100;
  // Init status bar
  status = init_status_bar();
  // close_status_bar();
  open_status_bar();
}

DIALOG * init_dialog() {
  DIALOG *dialog = malloc(sizeof(DIALOG));
  if (!dialog) {
    // Handle memory allocation failure, e.g., return NULL or exit
    return NULL;
  }

  dialog->name = malloc(MAX_NAME_STR_LENGTH * sizeof(char));
  if (!dialog->name) {
    free(dialog);
    return NULL;
  }
  dialog->name[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination

  dialog->relationship = malloc(TEXT_BUFFER_LEN * sizeof(char));
  if (!dialog->relationship) {
    free(dialog->name);
    free(dialog);
    return NULL;
  }
  dialog->relationship[TEXT_BUFFER_LEN - 1] = '\0';

  dialog->content = malloc(MAX_CONTENT_STR_LENGTH * sizeof(char));
  if (!dialog->content) {
    free(dialog->relationship);
    free(dialog->name);
    free(dialog);
    return NULL;
  }
  dialog->content[MAX_CONTENT_STR_LENGTH - 1] = '\0'; // Ensures null termination

  dialog->type = INVALID_DIALOG;
  dialog->merchant = NULL;
  dialog->ui_text_name = get_ui_component_by_ID(DIALOG_NAME);
  dialog->ui_text_relationship = get_ui_component_by_ID(DIALOG_RELATION);
  dialog->ui_text_content = get_ui_component_by_ID(DIALOG_CONTENT);
  dialog->ui_button_buy = get_ui_component_by_ID(DIALOG_BUTTON_BUY);
  dialog->ui_button_sell = get_ui_component_by_ID(DIALOG_BUTTON_SELL);
  dialog->ui_button_establish_trade_route = get_ui_component_by_ID(DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE);
  dialog->ui_text_schedule_trade_route_prompt = get_ui_component_by_ID(SCHEDULED_TRADE_ROUTE_PROMPT);

  // Init content
  vec2 content_position = { -1.0, -0.75 };
  init_menu(
    content_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    dialog->content, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // height
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_text_content // dest
  );

  // Init name
  vec2 name_position = { -1.0, -0.5 };
  init_menu(
    name_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    dialog->name, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_text_name // dest
  );

  // Init relationship
  vec2 relationship_position = { 1.0, -0.5 };
  init_menu(
    relationship_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    dialog->relationship, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // height
    PIVOT_BOTTOM_RIGHT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_text_relationship // dest
  );

  // Init dialog button buy
  vec2 buy_position = { -1.0, -1.0 };
  init_menu(
    buy_position, // position
    open_buy, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "1. Buy", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0.5, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_buy // dest
  );

  // Init dialog button sell
  vec2 sell_position = { -0.5, -1.0 };
  init_menu(
    sell_position, // position
    open_sell, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "2. Sell", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0.5, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_sell // dest
  );

  // Init dialog button establish trade route
  vec2 establish_trade_route_position = { 0.0, -1.0 };
  init_menu(
    establish_trade_route_position, // position
    open_establish_trade_route, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "3. Establish trade route", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    1.0, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_establish_trade_route // dest
  );

  // Init schdule trade route prompt
  vec2 establish_trade_route_prompt_position = { 0.0, 0.0 };
  init_menu(
    establish_trade_route_prompt_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "TRADE_ROUTE_PROMPT", // text
    0, // enabled
    1, // textured
    0, // texture
    0.1, // text_padding
    1.5, // text_scale
    0, // width
    0, // heights
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    dialog->ui_text_schedule_trade_route_prompt // dest
  );

  return dialog;
}

void free_dialog() {
  if (dialog) {
    free(dialog->name);
    dialog->name = NULL;

    free(dialog->relationship);
    dialog->relationship = NULL;

    free(dialog->content);
    dialog->content = NULL;

    free(dialog);
    dialog = NULL;  // Set the global dialog to NULL
  }
}

void open_dialog() {
  if (dialog) {
    // MERCHANT *target_merch = dialog->merchant;
    // int found = 0;
    // for (int i = 0; i < num_trade_ships; i++) {
    //   if (trade_ships[i].target_chunk.coords[0] == target_merch->chunk[0] &&
    //       trade_ships[i].target_chunk.coords[1] == target_merch->chunk[1]) {
    //     dialog->ui_button_establish_trade_route->text = "Already Established";
    //     found = 1;
    //   }
    // }
    // if (!found) {
    //   dialog->ui_button_establish_trade_route->text = "3. Establish trade route";
    // }
    switch (dialog->type) {
      case TALK: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_text_relationship->enabled = 1;
        dialog->ui_button_buy->enabled = 0;
        dialog->ui_button_sell->enabled = 0;
        dialog->ui_button_establish_trade_route->enabled = 0;
        break;
      }
      case MERCHANT_OPTION: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_text_relationship->enabled = 1;
        dialog->ui_button_buy->enabled = 1;
        dialog->ui_button_sell->enabled = 1;
        dialog->ui_button_establish_trade_route->enabled = 1;
        break;
      }
      default: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_text_relationship->enabled = 1;
        dialog->ui_button_buy->enabled = 1;
        dialog->ui_button_sell->enabled = 1;
        dialog->ui_button_establish_trade_route->enabled = 1;
        break;
      }
    }
  }
}

void close_dialog() {
  if (dialog) {
    dialog->ui_text_content->enabled = 0;
    dialog->ui_text_name->enabled = 0;
    dialog->ui_text_relationship->enabled = 0;
    dialog->ui_button_buy->enabled = 0;
    dialog->ui_button_sell->enabled = 0;
    dialog->ui_button_establish_trade_route->enabled = 0;
    dialog->ui_text_schedule_trade_route_prompt->enabled = 0;
    dialog->merchant = NULL;
    time_schdule_trade_toute_prompt = 2.0;
  }
}

int set_dialog(T_DIALOG dialog_type, MERCHANT *merchant, char *name,
               char *content) {
  if (dialog && strlen(name) < MAX_NAME_STR_LENGTH && strlen(content) < MAX_CONTENT_STR_LENGTH) {
    dialog->type = dialog_type;
    strncpy(dialog->name, name, MAX_NAME_STR_LENGTH);
    dialog->name[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination
    strncpy(dialog->content, content, MAX_CONTENT_STR_LENGTH);
    dialog->content[MAX_CONTENT_STR_LENGTH - 1] = '\0'; // Ensures null termination
    dialog->merchant = merchant;
    return 1;
  }
  return 0;
}

void open_buy() {
  // get_ui_component_by_ID(DIALOG_BUTTON_BUY)->text = "HIT!";
  close_dialog();
  if (set_trade(BUY, get_closest_merchant(e_player))) {
    open_trade();
  }
}

void close_buy() {

}

void open_sell() {
  // get_ui_component_by_ID(DIALOG_BUTTON_SELL)->text = "HIT!";
  close_dialog();
  if (set_trade(SELL, get_closest_merchant(e_player))) {
    open_trade();
  }
}

void close_sell() {

}

void open_establish_trade_route() {
  // UI_COMPONENT *trade_route_button = get_ui_component_by_ID(
  //                                      DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE
  //                                    );
  MERCHANT *target_merch = dialog->merchant;
  CHUNK *target_chunk = NULL;
  for (int i = 0; i < num_trade_ships; i++) {
    target_chunk = chunk_buffer + trade_ships[i].target_chunk_index;
    if (target_chunk->coords[0] == target_merch->chunk[0] &&
        target_chunk->coords[1] == target_merch->chunk[1]) {
      dialog->ui_text_schedule_trade_route_prompt->text = "You Already Have a Trade Route";
      dialog->ui_text_schedule_trade_route_prompt->enabled = 1;
      return;
    }
  }
  // trade_route_button->text = "Already Established";
  dialog->ui_text_schedule_trade_route_prompt->text = "Trade Route Established";
  dialog->ui_text_schedule_trade_route_prompt->enabled = 1;
  time_schdule_trade_toute_prompt = 2.0;
  // delay

  TRADE_SHIP *trade_ship = trade_ships + num_trade_ships;

  ivec2 target_chunk_coords = { 0, 0 };
  glm_ivec2_copy(dialog->merchant->chunk, target_chunk_coords);
  trade_ship->target_chunk_index = add_chunk(target_chunk_coords);

  ivec2 cur_chunk_coords = { 0, 0 };
  trade_ship->cur_chunk_index = add_chunk(cur_chunk_coords);

  glm_vec2_copy(home_island_coords, trade_ship->coords);
  glm_ivec2_copy(cur_chunk_coords, trade_ship->chunk_coords);
  /*
  ivec2 target_chunk = { 0, 0 };
  glm_ivec2_copy(dialog->merchant->chunk, target_chunk);
  chunk_from_coords(target_chunk, &trade_ships[num_trade_ships].target_chunk);

  ivec2 cur_chunk = { 0, 0 };
  chunk_from_coords(cur_chunk, &trade_ships[num_trade_ships].chunk);

  vec2 trade_ship_coords = { 0, 0 };
  if (trade_ships[num_trade_ships].chunk.num_islands) {
    trade_ship_coords[0] = trade_ships[num_trade_ships].chunk.islands[0].coords[0];
    trade_ship_coords[1] = trade_ships[num_trade_ships].chunk.islands[0].coords[1];
  }
  glm_ivec2_copy(cur_chunk, trade_ships[num_trade_ships].chunk_coords);
  glm_vec2_copy(trade_ship_coords, trade_ships[num_trade_ships].coords);
  */
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

void close_establish_trade_route() {

}

TRADE * init_trade() {
  TRADE *trade = malloc(sizeof(TRADE));
  if (!trade) {
    // Handle memory allocation failure, e.g., return NULL or exit
    return NULL;
  }

  trade->type = INVALID_TRADE;
  trade->merchant = NULL;
  trade->ui_listing[0] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_0);
  trade->ui_listing[1] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_1);
  trade->ui_listing[2] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_2);
  trade->ui_listing[3] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_3);
  trade->ui_listing[4] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_4);
  trade->ui_listing[5] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_5);
  trade->ui_listing[6] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_6);
  trade->ui_listing[7] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_7);
  trade->ui_listing[8] = get_ui_component_by_ID(TRADE_BUTTON_LISTING_8);

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
    trade->ui_listing[0] // dest
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
    trade->ui_listing[1] // dest
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
    trade->ui_listing[2]// dest
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
    trade->ui_listing[3] // dest
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
    trade->ui_listing[4] // dest
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
    trade->ui_listing[5] // dest
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
    trade->ui_listing[6] // dest
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
    trade->ui_listing[7] // dest
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
    trade->ui_listing[8] // dest
  );

  return trade;
}

void free_trade() {
  if (trade) {
    free(trade);
    trade = NULL;  // Set the global trade to NULL
  }
}

void open_trade() {
  if (trade) {
    switch (trade->type) {
      case BUY: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->enabled = 1;
        }
        break;
      }
      case SELL: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->enabled = 1;
        }
        break;
      }
      default: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->enabled = 1;
        }
        break;
      }
    }
  }
}

void close_trade() {
  if (trade) {
    for (int i = 0; i < 9; i++) {
      trade->ui_listing[i]->enabled = 0;
    }
  }
}

int set_trade(T_TRADE dialog_type, MERCHANT * merchant) {
  if (trade && merchant) {
    trade->type = dialog_type;
    trade->merchant = merchant;
    switch (dialog_type) {
      case BUY: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->text = get_item_name_by_ID(get_merchant_listing_item_by_number(merchant, i + 1)->item_id);
        }
        return 1;
      }
  // Attacking
      case SELL: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->text = get_item_name_by_ID(get_player_inventory_slot_by_number(i + 1)->item_id);
        }
        return 1;
      }
      default: {
        for (int i = 0; i < 9; i++) {
          trade->ui_listing[i]->text = "default";
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
  if (trade->type == BUY) {
    item = get_item_info_by_name(trade->ui_listing[listing]->text);
    listing_slot = get_merchant_listing_item_by_number(trade->merchant, listing + 1); 
    if (e_player.money < item.value) {
      printf("**** Do not have enough money ****\n");
    } else if (e_player.money >= item.value && listing_slot->quantity > 0) {
      listing_slot->quantity -= 1;
      e_player.money -= item.value;
      // INVENTORY ADD
      slot = search_player_inventory_with_ID(
                                get_merchant_listing_item_by_number(
                                trade->merchant, listing + 1)->item_id);
      if (slot) {
        slot->quantity += 1;
      } else if (get_player_first_empty_inventory_slot()) {
        I_SLOT *empty_inventory_slot = get_player_first_empty_inventory_slot();
        empty_inventory_slot->item_id = get_merchant_listing_item_by_number(trade->merchant, listing + 1)->item_id;
        empty_inventory_slot->quantity = 1;
      }

      // Reset Listing UI Component
      if (listing_slot->quantity <= 0) {
        listing_slot->item_id = 0;
        trade->ui_listing[listing]->text = "SOLD";
      }

      printf("**** Money = %d, %s quatity = %d ****\n", e_player.money, trade->ui_listing[listing]->text, 
             get_merchant_listing_item_by_number(trade->merchant, listing + 1)->quantity);

      trade->merchant->relationship += 10.0;
      if (trade->merchant->relationship > 100.0) {
        trade->merchant->relationship = 100.0;
      }
    }
  } else if (trade->type == SELL && get_player_inventory_slot_by_number(listing + 1)->quantity > 0) {
    slot = get_player_inventory_slot_by_number(listing + 1); 
    slot->quantity -= 1;
    int item_identifier = slot->item_id;
    item = get_item_info_by_name(trade->ui_listing[listing]->text);
    e_player.money += item.value;
    if (slot->quantity <= 0) {
      slot->item_id = 0;
      trade->ui_listing[listing]->text = "SOLD";
    }

    /* Find the first available listing location in the merchant inventory and populate */
    /* with item that was just sold to it */
    for (int i = 0; i < 9; i++) {
      listing_slot = get_merchant_listing_item_by_number(trade->merchant, i + 1); 
      if (listing_slot->item_id == item_identifier) {
        listing_slot->quantity++; 
        break;
      } else if (listing_slot->quantity == 0) {
        listing_slot->quantity++;
        listing_slot->item_id = item_identifier;
        break;
      }
    }

    printf("**** Money = %d, %s quatity = %d ****\n", e_player.money, trade->ui_listing[listing]->text,
           get_player_inventory_slot_by_number(listing + 1)->quantity);

    trade->merchant->relationship += 10.0;
    if (trade->merchant->relationship > 100.0) {
      trade->merchant->relationship = 100.0;
    }
  }
}

/*
                                   STATUS
Implements the functionality for shows player status.
*/

STATUS * init_status_bar() {
  STATUS *status = malloc(sizeof(STATUS));
  if (!status) {
    // Handle memory allocation failure, e.g., return NULL or exit
    return NULL;
  }

  status->ui_health_status = get_ui_component_by_ID(STATUS_HEALTH);
  status->ui_money_status = get_ui_component_by_ID(STATUS_MONEY);

  vec2 top_left = { -1.0, 1.0 };
  init_menu(
    top_left, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status->ui_health_status // dest
  );

  status->ui_health_status->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status->ui_health_status->text) {
    free(status);
    return NULL;
  }
  status->ui_health_status->text[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination

  init_menu(
    top_left, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status->ui_money_status // dest
  );

  status->ui_money_status->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status->ui_money_status->text) {
    free(status);
    return NULL;
  }
  status->ui_money_status->text[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination

  return status;
}

void free_status_bar() {
  if (status) {
    free(status->ui_health_status);
    status->ui_health_status=NULL;

    free(status->ui_money_status);
    status->ui_money_status=NULL;

    free(status);
    status = NULL;  // Set the global status to NULL
  }
}

void update_status_bar() {
  open_status_bar();
  if (status) {
    sprintf(status->ui_health_status->text, " HEALTH %.1f / %.1f", c_player.health, c_player.max_health);
    sprintf(status->ui_money_status->text, " MONEY %d", e_player.money);
  }
}

void open_status_bar() {
  if (status) {
    if (mode == EXPLORATION) {
      status->ui_health_status->enabled = 0;
      status->ui_money_status->enabled = 1;
    } else {
  // Attacking
      status->ui_health_status->enabled = 1;
      status->ui_money_status->enabled = 0;
    }
  }
}

void close_status_bar() {
  if (status) {
    status->ui_health_status->enabled = 0;
    status->ui_money_status->enabled = 0;
  }
}
