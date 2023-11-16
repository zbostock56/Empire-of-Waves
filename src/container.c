#include <container.h>

int init_containers() {
  int status = 0;
  status = init_container(&home_box, 64);
  if (status) {
    fprintf(stderr, "container.c: Unable to allocate container items\n");
    return -1;
  }

  return 0;
}

void free_containers() {
  free_container(&home_box);
}

int init_container(CONTAINER *cont, unsigned int capacity) {
  cont->capacity = capacity;
  cont->items = malloc(sizeof(I_SLOT) * capacity);
  if (cont->items == NULL) {
    fprintf(stderr, "container.c: Failed to allocate container items\n");
    return -1;
  }
  for (unsigned int i = 0; i < capacity; i++) {
    cont->items[i].item_id = EMPTY;
    cont->items[i].quantity = 0;
  }
  return 0;
}

void free_container(CONTAINER *cont) {
  free(cont->items);
  cont->items = NULL;
}

int move_item(I_SLOT *to, I_SLOT *from) {
  if ((from->item_id == EMPTY || from->quantity == 0) ||
      (to->item_id != from->item_id && to->item_id != EMPTY)) {
    return -1;
  }

  if (to->item_id == EMPTY) {
    to->quantity = 0;
  }
  to->item_id = from->item_id;
  to->quantity++;

  from->quantity--;
  if (from->quantity == 0) {
    from->item_id = EMPTY;
  }
  return 0;
}

int move_all_item(I_SLOT *to, I_SLOT *from) {
  if ((from->item_id == EMPTY || from->quantity == 0) ||
      (to->item_id != from->item_id && to->item_id != EMPTY)) {
    return -1;
  }

  if (to->item_id == EMPTY) {
    to->quantity = 0;
  }
  to->item_id = from->item_id;
  to->quantity += from->quantity;

  from->item_id = EMPTY;
  from->quantity = 0;;
  return 0;
}

/*
  Finds the index of a random item in a given container

  Returns:
  - index of an item in the container. If the container is empty, the index of
    the first item slot is returned
*/
unsigned int get_random_item(CONTAINER *cont) {
  unsigned int num_items = 0;
  for (unsigned int i = 0; i < cont->capacity; i++) {
    if (cont->items[i].item_id != EMPTY) {
      num_items++;
    }
  }

  if (!num_items) {
    return 0;
  }

  unsigned int filled_item = rand() % num_items;
  unsigned int item_index = 0;
  for (unsigned int i = 0; i < cont->capacity; i++) {
    if (cont->items[i].item_id != EMPTY && item_index == filled_item) {
      return i;
    } else if (cont->items[i].item_id != EMPTY) {
      item_index++;
    }
  }

  return 0; // Should never happen
}
