#include <inventory.h>

/*
Initialize the inventory UI components
*/
int init_inventory_ui() {
  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i] = get_ui_component_by_ID(INVENTORY_BUTTON_PLAYER_ITEM_0 + i);
  }

  inventory.ui_text_on_hover_item = get_ui_component_by_ID(INVENTORY_TEXT_HOVER_ITEM);
  inventory.ui_text_event_prompt = get_ui_component_by_ID(INVENTORY_TEXT_EVENT_PROMPT);

  // Init inventory items
  for (uintptr_t i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    float pos_x = -0.4 + (i % 4) * 0.25;
    float pos_y = 0.4 - (i / 4) * 0.25;
    init_menu(
      (vec2) { pos_x, pos_y }, // position
      on_click_inventory_item, // on_click
      on_hover_inventory_item, // on_hover
      (void *) i, // on_click_args
      (void *) i, // on_hover_args
      "UI_INV", // text
      0, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      0.5, // text_scale
      0.2, // width
      0.2, // height
      PIVOT_CENTER, // pivot
      T_CENTER, // text_anchor
      inventory.ui_inventory_items[i] // dest
    );
  }

  // Init on-hover prompt
  init_menu(
    (vec2) { 0, -0.6 }, // position
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
    inventory.ui_text_on_hover_item // dest
  );
  inventory.ui_text_on_hover_item->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!inventory.ui_text_on_hover_item->text) {
    fprintf(stderr, "trade.c: Failed to allocate on-hover text buffer\n");
    return -1;
  }
  inventory.ui_text_on_hover_item->text[0] = '\0'; // Ensures null termination
  strcpy(inventory.ui_text_on_hover_item->text, " [Name] P [Price] Q [Quantity] ");

  // Init event prompt
  init_menu(
    (vec2) { 0, 0.65 }, // position
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
    inventory.ui_text_event_prompt // dest
  );
  inventory.ui_text_event_prompt->text = malloc(MAX_UI_TEXT_LENGTH * sizeof(char));
  if (!inventory.ui_text_event_prompt->text) {
    fprintf(stderr, "trade.c: Failed to allocate event prompt buffer\n");
    return -1;
  }
  inventory.ui_text_event_prompt->text[0] = '\0'; // Ensures null termination
  strcpy(inventory.ui_text_event_prompt->text, " EVENT PROMPT ");

  return 0;
}

/*
Free the inventory UI components with text part malloced before
*/
void free_inventory_ui() {
  free(inventory.ui_text_on_hover_item->text);
  free(inventory.ui_text_event_prompt->text);

  inventory.ui_text_on_hover_item = NULL;
  inventory.ui_text_event_prompt = NULL;
}

/*
Open the inventory UI
*/
void open_inventory_ui() {
  update_inventory_ui();

  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i]->enabled = 1;
  }
  inventory.ui_text_on_hover_item->enabled = 1;
  inventory.ui_text_event_prompt->enabled = 0;
}

/*
Close the inventory UI
*/
void close_inventory_ui() {
  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i]->enabled = 0;
  }
  inventory.ui_text_on_hover_item->enabled = 0;
  inventory.ui_text_event_prompt->enabled = 0;
}

/*
Update the inventory ui make the player inventory consistent with the ui
*/
void update_inventory_ui() {
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    I_SLOT *i_slot = get_player_inventory_slot_by_index(i);
    ITEM_IDS i_slot_id = i_slot->item_id;
    inventory.ui_inventory_items[i]->text = get_item_name_by_ID(i_slot_id);
  }
}

void on_click_inventory_item(void *inventory_item_index) {

}

/*
Hover listener of trade with item menu player items
Args:
unsigned int player_item_index
  current page player item index, from 0 to 7
*/
void on_hover_inventory_item(void *inventory_item_index) {
  int index = (uintptr_t)inventory_item_index;
  I_SLOT *i_slot = get_player_inventory_slot_by_index(index);
  ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
  int price = get_item_info_by_ID(item_id).value;
  int quantity = i_slot->quantity;
  if (item_id != EMPTY) {
    sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] ",
          get_item_name_by_ID(item_id), price, quantity);
  } else {
    sprintf(inventory.ui_text_on_hover_item->text, " [-] P [-] Q [-] S [-] ");
  }
}
