#include <container.h>

int init_containers() {
  int status = 0;
  status = init_container(&magic_box, 64);
  if (status) {
    fprintf(stderr, "container.c: Unable to allocate container items\n");
    return -1;
  }

  return 0;
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
