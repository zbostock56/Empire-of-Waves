#include <container_ui.h>

void init_container_ui() {
  vec2 container1_pos = { -0.9, 0.5 };
  vec2 cur_container = GLM_VEC2_ZERO_INIT;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      cur_container[0] = container1_pos[0] + (((float) j) * 0.8 / 6.0);
      cur_container[1] = container1_pos[1] - (((float) i) * 0.8 / 6.0);
      size_t index = (i * 6) + j;
      init_menu(
        cur_container,
        c1_slot_on_click, NULL,
        (void *) index, NULL,
        "",
        0,
        1,
        0,
        0.0,
        0.25,
        0.8 / 6.0,
        0.8 / 6.0,
        PIVOT_TOP_LEFT,
        T_CENTER,
        get_ui_component_by_ID(CONTAINER_1_SLOTS + (i * 6) + j)
      );
    }
  }

  vec2 c1_next_pos = { -0.1, -0.35 };
  init_menu(
    c1_next_pos,
    container_next, NULL,
    (void *) 1, NULL,
    "Next",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP_RIGHT,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_1_NEXT)
  );

  vec2 c1_prev_pos = { -0.9, -0.35 };
  init_menu(
    c1_prev_pos,
    container_prev, NULL,
    (void *) 1, NULL,
    "Prev",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP_LEFT,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_1_PREV)
  );

  vec2 c1_move_pos = { -0.5, -0.35 };
  init_menu(
    c1_move_pos,
    move_c1_item, NULL,
    (void *) -1, NULL,
    " -> ",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_1_MOVE)
  );

  vec2 container2_pos = { 0.1, 0.5 };
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      cur_container[0] = container2_pos[0] + (((float) j) * 0.8 / 6.0);
      cur_container[1] = container2_pos[1] - (((float) i) * 0.8 / 6.0);
      size_t index = (i * 6) + j;
      init_menu(
        cur_container,
        c2_slot_on_click, NULL,
        (void *) index, NULL,
        "",
        0,
        1,
        0,
        0.0,
        0.25,
        0.8 / 6.0,
        0.8 / 6.0,
        PIVOT_TOP_LEFT,
        T_CENTER,
        get_ui_component_by_ID(CONTAINER_2_SLOTS + (i * 6) + j)
      );
    }
  }

  vec2 c2_next_pos = { 0.9, -0.35 };
  init_menu(
    c2_next_pos,
    container_next, NULL,
    (void *) 2, NULL,
    "Next",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP_RIGHT,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_2_NEXT)
  );

  vec2 c2_prev_pos = { 0.1, -0.35 };
  init_menu(
    c2_prev_pos,
    container_prev, NULL,
    (void *) 2, NULL,
    "Prev",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP_LEFT,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_2_PREV)
  );

  vec2 c2_move_pos = { 0.5, -0.35 };
  init_menu(
    c2_move_pos,
    move_c2_item, NULL,
    (void *) -1, NULL,
    " <- ",
    0,
    1,
    0,
    0.0,
    1.0,
    0.2,
    0.15,
    PIVOT_TOP,
    T_CENTER,
    get_ui_component_by_ID(CONTAINER_2_MOVE)
  );
}

void open_container(CONTAINER container_1, CONTAINER container_2) {
  c1 = container_1;
  c2 = container_2;
  c1_page = 0;
  c2_page = 0;
  refresh_containers();
  get_ui_component_by_ID(CONTAINER_1_NEXT)->enabled = 1;
  get_ui_component_by_ID(CONTAINER_1_PREV)->enabled = 1;
  get_ui_component_by_ID(CONTAINER_1_MOVE)->enabled = 1;
  get_ui_component_by_ID(CONTAINER_2_NEXT)->enabled = 1;
  get_ui_component_by_ID(CONTAINER_2_PREV)->enabled = 1;
  get_ui_component_by_ID(CONTAINER_2_MOVE)->enabled = 1;
  container_menu_open = 1;
}

void close_container() {
  c1_page = 0;
  c2_page = 0;
  for (int i = CONTAINER_1_SLOTS; i <= CONTAINER_2_MOVE; i++) {
    get_ui_component_by_ID(i)->enabled = 0;
  }
  get_ui_component_by_ID(CONTAINER_1_NEXT)->enabled = 0;
  get_ui_component_by_ID(CONTAINER_1_PREV)->enabled = 0;
  get_ui_component_by_ID(CONTAINER_1_MOVE)->enabled = 0;
  get_ui_component_by_ID(CONTAINER_2_NEXT)->enabled = 0;
  get_ui_component_by_ID(CONTAINER_2_PREV)->enabled = 0;
  get_ui_component_by_ID(CONTAINER_2_MOVE)->enabled = 0;
  container_menu_open = 0;
}

void refresh_containers() {
  UI_COMPONENT *slot = NULL;
  for (int i = 0; i < 36; i++) {
    if (i < c1.capacity) {
      slot = get_ui_component_by_ID(CONTAINER_1_SLOTS + i);
      slot->text = get_item_name_by_ID(c1.items[i].item_id);
      slot->enabled = 1;
    } else {
      get_ui_component_by_ID(CONTAINER_1_SLOTS + i)->enabled = 0;
    }

    if (i < c2.capacity) {
      slot = get_ui_component_by_ID(CONTAINER_2_SLOTS + i);
      slot->text = get_item_name_by_ID(c2.items[i].item_id);
      slot->enabled = 1;
    } else {
      get_ui_component_by_ID(CONTAINER_2_SLOTS + i)->enabled = 0;
    }
  }
}

void c1_slot_on_click(void *slot) {
  size_t index = (c1_page * 36) + ((size_t) slot);
  if (holding_shift) {
    for (int i = 0; i < c2.capacity; i++) {
      if (c2.items[i].item_id == c1.items[index].item_id) {
        move_item(c2.items + i, c1.items + index);
        break;
      } else if (c2.items[i].item_id == EMPTY) {
        c2.items[i].quantity = 0;
        move_item(c2.items + i, c1.items + index);
        break;
      }
    }
    refresh_containers();
  } else {
    get_ui_component_by_ID(CONTAINER_1_MOVE)->on_click_args = slot;
  }
}

void c2_slot_on_click(void *slot) {
  size_t index = (c2_page * 36) + ((size_t) slot);
  if (holding_shift) {
    for (int i = 0; i < c1.capacity; i++) {
      if (c1.items[i].item_id == c2.items[index].item_id) {
        move_item(c1.items + i, c2.items + index);
        break;
      } else if (c1.items[i].item_id == EMPTY) {
        c1.items[i].quantity = 0;
        move_item(c1.items + i, c2.items + index);
        break;
      }
    }
    refresh_containers();
  } else {
    get_ui_component_by_ID(CONTAINER_2_MOVE)->on_click_args = slot;
  }

}

void container_next(void *cont) {
  size_t container = (size_t) cont;
  size_t starting_index = 0;
  I_SLOT *items = NULL;
  UI_COMPONENT *slots = NULL;
  unsigned int capacity = 0;
  if (container == 1) {
    c1_page = (c1_page + 1) % ((c1.capacity / 36) + 1);
    starting_index = c1_page * 36;
    slots = get_ui_component_by_ID(CONTAINER_1_SLOTS);
    items = c1.items;
    capacity = c1.capacity;
  } else {
    c2_page = (c2_page + 1) % ((c2.capacity / 36) + 1);
    starting_index = c2_page * 36;
    slots = get_ui_component_by_ID(CONTAINER_2_SLOTS);
    items = c2.items;
    capacity = c2.capacity;
  }

  for (int i = 0; i < 36; i++) {
    int index = starting_index + i;
    if (index < capacity) {
      (slots + i)->text = get_item_name_by_ID(items[index].item_id);
      (slots + i)->enabled = 1;
    } else {
      (slots + i)->enabled = 0;
    }
  }
}

void container_prev(void *cont) {
  size_t container = (size_t) cont;
  size_t starting_index = 0;
  I_SLOT *items = NULL;
  UI_COMPONENT *slots = NULL;
  unsigned int capacity = 0;
  if (container == 1) {
    c1_page--;
    if (c1_page < 0) {
      c1_page = c1.capacity / 36;
    }

    starting_index = c1_page * 36;
    slots = get_ui_component_by_ID(CONTAINER_1_SLOTS);
    items = c1.items;
    capacity = c1.capacity;
  } else {
    c2_page--;
    if (c2_page < 0) {
      c2_page = c2.capacity / 36;
    }

    starting_index = c2_page * 36;
    slots = get_ui_component_by_ID(CONTAINER_2_SLOTS);
    items = c2.items;
    capacity = c2.capacity;
  }

  for (int i = 0; i < 36; i++) {
    int index = starting_index + i;
    if (index < capacity) {
      (slots + i)->text = get_item_name_by_ID(items[index].item_id);
      (slots + i)->enabled = 1;
    } else {
      (slots + i)->enabled = 0;
    }
  }
}

void move_c1_item(void *slot) {
  size_t index = (c1_page * 36) + ((size_t) slot);
  for (int i = 0; i < c2.capacity; i++) {
    if (c2.items[i].item_id == EMPTY) {
      c2.items[i].quantity = 0;
      move_item(c2.items + i, c1.items + index);
      break;
    }
  }
  refresh_containers();
}

void move_c2_item(void *slot) {
  size_t index = (c2_page * 36) + ((size_t) slot);
  for (int i = 0; i < c1.capacity; i++) {
    if (c1.items[i].item_id == 0) {
      c1.items[i].quantity = 0;
      move_item(c1.items + i, c2.items + index);
      break;
    }
  }
  refresh_containers();
}
