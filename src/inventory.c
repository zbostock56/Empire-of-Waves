#include <inventory.h>

/*
Initialize the inventory UI components
*/
int init_inventory_ui() {
  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i] = get_ui_component_by_ID(INVENTORY_BUTTON_PLAYER_ITEM_0 + i);
  }
  inventory.ui_equipment_weapon = get_ui_component_by_ID(INVENTORY_BUTTON_WEAPON);
  inventory.ui_equipment_armor = get_ui_component_by_ID(INVENTORY_BUTTON_ARMOR);
  inventory.ui_text_on_hover_item = get_ui_component_by_ID(INVENTORY_TEXT_HOVER_ITEM);
  inventory.ui_text_event_prompt = get_ui_component_by_ID(INVENTORY_TEXT_EVENT_PROMPT);

  // Init inventory items
  for (uintptr_t i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    float pos_x = -0.5 + (i % 4) * 0.25;
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

  // Init equipment items
  init_menu(
    (vec2) { 0.5, 0.20 }, // position
    on_click_weapon, // on_click
    on_hover_weapon, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "UI_WEAPON", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    inventory.ui_equipment_weapon // dest
  );

  init_menu(
    (vec2) { 0.5, -0.15 }, // position
    on_click_armor, // on_click
    on_hover_armor, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    "UI_ARMOR", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.2, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    inventory.ui_equipment_armor // dest
  );

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
  inventory.ui_equipment_weapon->enabled = 1;
  inventory.ui_equipment_armor->enabled = 1;
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
  inventory.ui_equipment_weapon->enabled = 0;
  inventory.ui_equipment_armor->enabled = 0;
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
  inventory.ui_equipment_weapon->text = get_item_name_by_ID(equipment.weapon_equipped);
  inventory.ui_equipment_armor->text = get_item_name_by_ID(equipment.armor_equipped);
}

/*
Click listener of inventory items
Args:
unsigned int inventory_item_index
  inventory item index, from 0 to 15
*/
void on_click_inventory_item(void *inventory_item_index) {
  int index = (uintptr_t)inventory_item_index;
  I_SLOT *i_slot = get_player_inventory_slot_by_index(index);
  ITEM_IDS item_id = i_slot->item_id;
  int quantity = i_slot->quantity;
  ITEM item = get_item_info_by_ID(item_id);
  // int price = item.value;
  float health_mod = item.health_mod;
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  // float max_health_mod = item.max_heath_mod;
  if (item_id != EMPTY && item_id != INVALID_ITEM && quantity >= 1 && (item.edible || item.equippable)) {
    if (item.edible) {
      e_player.health += health_mod;
      c_player.health += health_mod;
      c_player.fire_rate += firerate_mod;
      c_player.speed += speed_mod;
      i_slot->quantity -= 1;
      if (i_slot->quantity < 1) {
        i_slot->item_id = EMPTY;
      }
    } else if (item.equippable) {
      // No equipped weapon
      if (item_isWeapon(item_id)) {
        taken_off_weapon();
      } else if (item_isArmor(item_id)) {
        taken_off_armor();
      }
      equip_slot_item(i_slot);
    } else {
      sprintf(inventory.ui_text_event_prompt->text, " ITEM CANNOT BE INTERACTED WITH ");
      inventory.ui_text_event_prompt->enabled = 1;
      time_inventory_event_prompt = 2.0;
    }
  }
  update_inventory_ui();
}

/*
Helper function for on_click_inventory_item()
*/
void equip_slot_item(I_SLOT *i_slot) {
  ITEM_IDS item_id = i_slot->item_id;
  // int quantity = i_slot->quantity;
  ITEM item = get_item_info_by_ID(item_id);
  // int price = item.value;
  // float health_mod = item.health_mod;
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  float max_health_mod = item.max_heath_mod;
  if (item_isWeapon(item_id)) {
    if (equipment.weapon_equipped == EMPTY || equipment.weapon_equipped == INVALID_ITEM) {
      equipment.weapon_equipped = item_id;
      if (item_isMeleeWeapon(item_id)) {
        equipment.weapon_type = MELEE;
        c_player.weapon_type = MELEE;
      } else if (item_isRangedWeapon(item_id)) {
        equipment.weapon_type = RANGED;
        c_player.weapon_type = RANGED;
      }
      c_player.fire_rate += firerate_mod;
      c_player.speed += speed_mod;
      i_slot->quantity -= 1;
      if (i_slot->quantity < 1) {
        i_slot->item_id = EMPTY;
      }
    }
  } else if (item_isArmor(item_id)) {
    if (equipment.armor_equipped == EMPTY || equipment.armor_equipped == INVALID_ITEM) {
      equipment.armor_equipped = item_id;
      e_player.max_health += max_health_mod;
      c_player.fire_rate += firerate_mod;
      c_player.speed += speed_mod;
      c_player.max_health += max_health_mod;
      i_slot->quantity -= 1;
      if (i_slot->quantity < 1) {
        i_slot->item_id = EMPTY;
      }
    }
  } else {
    printf("\nErr: Slot item cannot be equipped!\n");
    return;
  }
  update_inventory_ui();
}

/*
Helper function for taken off weapon from equipped list
*/
void taken_off_weapon() {
  // No equipped weapon
  if (equipment.weapon_equipped == EMPTY || equipment.weapon_equipped == INVALID_ITEM) {
    return;
  } else if (equipment.weapon_equipped != EMPTY && equipment.weapon_equipped != INVALID_ITEM) {
    // Have equipped weapon then taken off the equipped weapon
    if (search_player_inventory_by_ID(equipment.weapon_equipped) != NULL) {
      // Have same type of equipped weapon in inventory then increment its quantity
      search_player_inventory_by_ID(equipment.weapon_equipped)->quantity += 1;
    } else if (search_player_inventory_by_ID(equipment.weapon_equipped) == NULL) {
      // Do not have same type of equipped weapon in inventory but have empty slot
      if (get_player_first_empty_inventory_slot() != NULL) {
        get_player_first_empty_inventory_slot()->item_id = equipment.weapon_equipped;
        get_player_first_empty_inventory_slot()->quantity = 1;
      } else if (get_player_first_empty_inventory_slot() == NULL) {
        // TODO: Drop to the ground
        // Currently the item will disappear
        // Show prompt
        sprintf(inventory.ui_text_event_prompt->text, " Weapon Dropped - Insufficient Inventory Space ");
        inventory.ui_text_event_prompt->enabled = 1;
        time_inventory_event_prompt = 2.0;
      }
    }
    equipment.weapon_type = MELEE;
    equipment.weapon_equipped = EMPTY;
  }
  update_inventory_ui();
}

/*
Helper function for taken off the armor from equipped list
*/
void taken_off_armor() {
  // No equipped armor
  if (equipment.armor_equipped == EMPTY || equipment.armor_equipped == INVALID_ITEM) {
    return;
  } else if (equipment.armor_equipped != EMPTY && equipment.armor_equipped != INVALID_ITEM) {
    // Have equipped weapon then taken off the equipped weapon
    if (search_player_inventory_by_ID(equipment.armor_equipped) != NULL) {
      // Have same type of equipped weapon in inventory then increment its quantity
      search_player_inventory_by_ID(equipment.armor_equipped)->quantity += 1;
    } else if (search_player_inventory_by_ID(equipment.armor_equipped) == NULL) {
      // Do not have same type of equipped weapon in inventory but have empty slot
      if (get_player_first_empty_inventory_slot() != NULL) {
        get_player_first_empty_inventory_slot()->item_id = equipment.armor_equipped;
        get_player_first_empty_inventory_slot()->quantity = 1;
      } else if (get_player_first_empty_inventory_slot() == NULL) {
        // TODO: Drop to the ground
        // Currently the item will disappear
        // Show prompt
        sprintf(inventory.ui_text_event_prompt->text, " Armor Dropped - Insufficient Inventory Space ");
        inventory.ui_text_event_prompt->enabled = 1;
        time_inventory_event_prompt = 2.0;
      }
    }
    equipment.armor_equipped = EMPTY;
  }
  update_inventory_ui();
}

/*
Hover listener of inventory items
Args:
unsigned int inventory_item_index
  inventory item index, from 0 to 15
*/
void on_hover_inventory_item(void *inventory_item_index) {
  int index = (uintptr_t)inventory_item_index;
  I_SLOT *i_slot = get_player_inventory_slot_by_index(index);
  ITEM_IDS item_id = get_player_inventory_slot_by_index(index)->item_id;
  int quantity = i_slot->quantity;
  ITEM item = get_item_info_by_ID(item_id);
  int price = item.value;
  float health_mod = item.health_mod;
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  float max_health_mod = item.max_heath_mod;
  if (item_id != EMPTY) {
    if (item.edible) {
      switch (item_id) {
        case CITRUS: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [HP %+.1f] ", get_item_name_by_ID(item_id), price, quantity, health_mod);
          break;
        }
        case RUM: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [HP %+.1f] [FR %+.1f] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, quantity, health_mod, firerate_mod, speed_mod);
          break;
        }
        case LIFE_POTION: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [HP %+.1f] ", get_item_name_by_ID(item_id), price, quantity, health_mod);
          break;
        }
        case FIRERATE_POTION: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [FR %+.1f] ", get_item_name_by_ID(item_id), price, quantity, firerate_mod);
          break;
        }
        case SPEED_POTION: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, quantity, speed_mod);
          break;
        }
        default: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] ", get_item_name_by_ID(item_id), price, quantity);
          break;
        }
      }
    }
    else if (item.equippable) {
      switch (item_id) {
        case KNIFE:
        case SWORD:
        case AXE: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [MELEE] [FR %+.1f] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, quantity, firerate_mod, speed_mod);
          break;
        }
        case BOW:
        case CROSSBOW:
        case FLINTLOCK: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [RANGED] [FR %+.1f] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, quantity, firerate_mod, speed_mod);
          break;
        }
        case CLOTH_ARMOR:
        case LEATHER_ARMOR:
        case LIGHT_ARMOR:
        case MEDIUM_ARMOR:
        case HEAVY_ARMOR:
        case PLATE_ARMOR: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] [FR %+.1f] [SPD %+.1f] [MAX HP %+.1f]", get_item_name_by_ID(item_id), price, quantity, firerate_mod, speed_mod, max_health_mod);
          break;
        }
        default: {
          sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] ", get_item_name_by_ID(item_id), price, quantity);
          break;
        }
      }
    }
    else {
      sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] Q [%d] ", get_item_name_by_ID(item_id), price, quantity);
    }
  } else {
    sprintf(inventory.ui_text_on_hover_item->text, " [-] ");
  }
}

void on_click_weapon() {
  taken_off_weapon();
}

void on_hover_weapon() {
  ITEM_IDS item_id = equipment.weapon_equipped;
  ITEM item = get_item_info_by_ID(item_id);
  int price = item.value;
  // float health_mod = item.health_mod;
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  // float max_health_mod = item.max_heath_mod;
  if (item_id != EMPTY) {
    switch (item_id) {
      case KNIFE:
      case SWORD:
      case AXE: {
        sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] [MELEE] [FR %+.1f] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, firerate_mod, speed_mod);
        break;
      }
      case BOW:
      case CROSSBOW:
      case FLINTLOCK: {
        sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] [RANGED] [FR %+.1f] [SPD %+.1f] ", get_item_name_by_ID(item_id), price, firerate_mod, speed_mod);
        break;
      }
      default: {
        sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] ", get_item_name_by_ID(item_id), price);
        break;
      }
    }
  } else {
    sprintf(inventory.ui_text_on_hover_item->text, " [-] ");
  }
}

void on_click_armor() {
  taken_off_armor();
}

void on_hover_armor() {
  ITEM_IDS item_id = equipment.armor_equipped;
  ITEM item = get_item_info_by_ID(item_id);
  int price = item.value;
  // float health_mod = item.health_mod;
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  float max_health_mod = item.max_heath_mod;
  if (item_id != EMPTY) {
    switch (item_id) {
      case CLOTH_ARMOR:
      case LEATHER_ARMOR:
      case LIGHT_ARMOR:
      case MEDIUM_ARMOR:
      case HEAVY_ARMOR:
      case PLATE_ARMOR: {
        sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] [FR %+.1f] [SPD %+.1f] [MAX HP %+.1f]", get_item_name_by_ID(item_id), price, firerate_mod, speed_mod, max_health_mod);
        break;
      }
      default: {
        sprintf(inventory.ui_text_on_hover_item->text, " [%s] P [%d] ", get_item_name_by_ID(item_id), price);
        break;
      }
    }
  } else {
    sprintf(inventory.ui_text_on_hover_item->text, " [-] ");
  }
}
