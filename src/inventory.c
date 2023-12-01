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
  inventory.ui_drop = get_ui_component_by_ID(INVENTORY_BUTTON_DROP);

  // Init inventory items
  for (uintptr_t i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    float pos_x = -0.55 + (i % 4) * 0.25;
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
    (vec2) { 0.45, 0.3 }, // position
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
    (vec2) { 0.7, 0.3 }, // position
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

  // Init drop
  init_menu(
    (vec2) { 0.575, -0.3 }, // position
    on_click_drop, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    NULL, // on_hover_args
    " ENABLE DROP ", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.3, // width
    0.2, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    inventory.ui_drop // dest
  );

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
  inventory_open = 1;
  update_inventory_ui();

  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i]->enabled = 1;
  }
  inventory.ui_equipment_weapon->enabled = 1;
  inventory.ui_equipment_armor->enabled = 1;
  inventory.ui_text_on_hover_item->enabled = 1;
  inventory.ui_text_event_prompt->enabled = 0;
  inventory.ui_drop->enabled = 1;
}

/*
Close the inventory UI
*/
void close_inventory_ui() {
  inventory_open = 0;
  for (int i = 0; i < 16; i++) {
    inventory.ui_inventory_items[i]->enabled = 0;
  }
  inventory.ui_equipment_weapon->enabled = 0;
  inventory.ui_equipment_armor->enabled = 0;
  inventory.ui_text_on_hover_item->enabled = 0;
  inventory.ui_text_event_prompt->enabled = 0;
  inventory.ui_drop->enabled = 0;
  inventory.ui_drop->text = " ENABLE DROP ";
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
  float hunger_mod = item_food_value(item_id);
  float firerate_mod = item.firerate_mod;
  float speed_mod = item.speed_mod;
  // float max_health_mod = item.max_heath_mod;
  if (strcmp(inventory.ui_drop->text, " ENABLE DROP ") == 0) {
    if (item_id != EMPTY && item_id != INVALID_ITEM && quantity >= 1 && (item.edible || item.equippable)) {
      if (item.edible) {
        if (health_mod > 0.0 && c_player.health == c_player.max_health &&
            hunger_mod == -1.0) {
          sprintf(inventory.ui_text_event_prompt->text, " You are already at Maximum Health");
          inventory.ui_text_event_prompt->enabled = 1;
          time_inventory_event_prompt = 2.0;
          update_inventory_ui();
          return;
        }
        if (item_isPotion(item_id)) {
          if (item.speed_mod > 0) {
            increment_buff((void *)"Speed", (void *)&item.speed_mod);
          }
          if (item.firerate_mod > 0) {
            increment_buff((void*)"Fire Rate", (void*)&item.firerate_mod);
          }
        } else {
            e_player.health = glm_clamp(e_player.health + health_mod, 0.0, e_player.max_health);
            c_player.health = glm_clamp(c_player.health + health_mod, 0.0, c_player.max_health);
            if (hunger_mod != -1.0) {
              e_player.hunger += hunger_mod;
            }
            c_player.fire_rate += firerate_mod;
            c_player.speed += speed_mod;
        }
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
  } else if (strcmp(inventory.ui_drop->text, " DISABLE DROP ") == 0) {
    if (item_id != INVALID_ITEM && item_id != EMPTY && drop_item(item_id)) {
      i_slot->quantity -= 1;
      if (i_slot->quantity <= 0) {
        i_slot->item_id = EMPTY;
        i_slot->quantity = 0;
      }
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
    float health_mod = get_item_info_by_ID(equipment.weapon_equipped).health_mod;
    float firerate_mod = get_item_info_by_ID(equipment.weapon_equipped).firerate_mod;
    float speed_mod = get_item_info_by_ID(equipment.weapon_equipped).speed_mod;
    float max_health_mod = get_item_info_by_ID(equipment.weapon_equipped).max_heath_mod;
    // Have equipped weapon then taken off the equipped weapon
    if (is_dropping) {
      if (!drop_item(equipment.weapon_equipped)) {
        return;
      }
    } else if (search_player_inventory_by_ID(equipment.weapon_equipped) != NULL) {
      // Have same type of equipped weapon in inventory then increment its quantity
      search_player_inventory_by_ID(equipment.weapon_equipped)->quantity += 1;
    } else if (search_player_inventory_by_ID(equipment.weapon_equipped) == NULL) {
      // Do not have same type of equipped weapon in inventory but have empty slot
      if (get_player_first_empty_inventory_slot() != NULL) {
        get_player_first_empty_inventory_slot()->item_id = equipment.weapon_equipped;
        get_player_first_empty_inventory_slot()->quantity = 1;
      } else if (get_player_first_empty_inventory_slot() == NULL) {
        // Drop to the ground
        if (!drop_item(equipment.weapon_equipped)) {
          return;
        }

        // Show prompt
        sprintf(inventory.ui_text_event_prompt->text, " Weapon Dropped - Insufficient Inventory Space ");
        inventory.ui_text_event_prompt->enabled = 1;
        time_inventory_event_prompt = 2.0;
      }
    }
    e_player.health -= health_mod;
    c_player.health -= health_mod;
    c_player.fire_rate -= firerate_mod;
    e_player.speed -= speed_mod;
    c_player.speed -= speed_mod;
    e_player.max_health -= max_health_mod;
    c_player.max_health -= max_health_mod;

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
    float health_mod = get_item_info_by_ID(equipment.armor_equipped).health_mod;
    float firerate_mod = get_item_info_by_ID(equipment.armor_equipped).firerate_mod;
    float speed_mod = get_item_info_by_ID(equipment.armor_equipped).speed_mod;
    float max_health_mod = get_item_info_by_ID(equipment.armor_equipped).max_heath_mod;
    // Have equipped weapon then taken off the equipped weapon
    if (is_dropping) {
      if (!drop_item(equipment.armor_equipped)) {
        return;
      }
    } else if (search_player_inventory_by_ID(equipment.armor_equipped) != NULL) {
      // Have same type of equipped weapon in inventory then increment its quantity
      search_player_inventory_by_ID(equipment.armor_equipped)->quantity += 1;
    } else if (search_player_inventory_by_ID(equipment.armor_equipped) == NULL) {
      // Do not have same type of equipped weapon in inventory but have empty slot
      if (get_player_first_empty_inventory_slot() != NULL) {
        get_player_first_empty_inventory_slot()->item_id = equipment.armor_equipped;
        get_player_first_empty_inventory_slot()->quantity = 1;
      } else if (get_player_first_empty_inventory_slot() == NULL) {
        // Drop to the ground
        if (!drop_item(equipment.armor_equipped)) {
          return;
        }
        // Show prompt
        sprintf(inventory.ui_text_event_prompt->text, " Armor Dropped - Insufficient Inventory Space ");
        inventory.ui_text_event_prompt->enabled = 1;
        time_inventory_event_prompt = 2.0;
      }
    }
    e_player.health -= health_mod;
    c_player.health -= health_mod;
    c_player.fire_rate -= firerate_mod;
    e_player.speed -= speed_mod;
    c_player.speed -= speed_mod;
    e_player.max_health -= max_health_mod;
    c_player.max_health -= max_health_mod;

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

/*
Taken off weapon when equipped
*/
void on_click_weapon() {
  taken_off_weapon();
}

/*
Show weapon stats when equipped
*/
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

/*
Taken off armor when equipped
*/
void on_click_armor() {
  taken_off_armor();
}

/*
Show armor stats when equipped
*/
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

/*
Switch on/off for drop mode
*/
void on_click_drop() {
  if (strcmp(inventory.ui_drop->text, " ENABLE DROP ") == 0) {
    inventory.ui_drop->text = " DISABLE DROP ";
    is_dropping = 1;
  } else if (strcmp(inventory.ui_drop->text, " DISABLE DROP ") == 0) {
    inventory.ui_drop->text = " ENABLE DROP ";
    is_dropping = 0;
  }
}

/*
Drop an item
*/
int drop_item(ITEM_IDS item_id) {
  if (e_player.embarked) {
    /* item directly disappeared when on the ship */
    sprintf(inventory.ui_text_event_prompt->text, " Item Dropped to Ocean - %s ", get_item_name_by_ID(item_id));
    inventory.ui_text_event_prompt->enabled = 1;
    time_inventory_event_prompt = 2.0;
    return 1;
  } else {
    /* Find the current island that the player is on */
    vec2 coords = GLM_VEC2_ZERO_INIT;
    chunk_to_world(e_player.chunk, e_player.coords, coords);
    CHUNK *chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
    ISLAND *island = cur_island(chunk, coords);

    if (num_items_on_island(island) == island->num_items) {
      /* Items slots full in base buffer, start to add more as needed */
      if (++island->num_items > ITEM_BUFFER_SIZE) {
        island->num_items = ITEM_BUFFER_SIZE - 1;
        set_prompt("Cannot drop more items on island!");
        return 0;
      }
    }
    vec2 location = GLM_VEC2_ZERO_INIT;
    vec2 intra_island_pos = {island->coords[0], island->coords[1]};
    glm_vec2_sub(e_player.coords, intra_island_pos, location);
    /* Check if the player is about to drop an item on an  */
    /* existing other item */
    int slot = find_item_slot_specific_loc(island, location); 
    if (slot == -1) {
      if ((slot = find_first_avail_item_slot(island)) == -1) {
        set_prompt("ERR: could not drop item");
        return 0;
      }
    } else {
      return 0;
    }

    ITEM_TILES *drop_tile = island->item_tiles + slot;
    glm_vec2_copy(location, drop_tile->position);
    drop_tile->quantity = 1;
    drop_tile->resource = (int)item_id;
    sprintf(inventory.ui_text_event_prompt->text, " Item Dropped - %s ", get_item_name_by_ID(item_id));
    inventory.ui_text_event_prompt->enabled = 1;
    time_inventory_event_prompt = 2.0;
    return 1;
  }
}
