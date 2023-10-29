#include <ui_list.h>

void init_ui_lists() {
  list.listing_strings = NULL;
  list.page = 0;
  list.num_components = 0;
  list.comp_per_page = 0;
}

/*
  Assumes that UI_COMPONENTS are already created

  General Idea:
   ----------------------------
  |                            |
  |  ------------------------  |
  |  |                      |  |
  |  ------------------------  |
  |  |                      |  |
  |  ------------------------  |
  |  |                      |  |
  |  ------------------------  |
  |                            |
  |   <-                  ->   |
  |                            |
   ----------------------------
*/
void init_ui_list(UI_LIST *list, unsigned int page,
                  unsigned int comp_page, unsigned int num_comps,
                  char **listing_strings) {
  list->listing_strings = listing_strings;
  list->page = page;
  list->comp_per_page = comp_page;
  if (comp_page > MAX_PER_PAGE) {
    list->comp_per_page = MAX_PER_PAGE;
  }
  list->num_components = num_comps;

}

void open_listing(UI_LIST *list) {
  unsigned int comp_bound = list->page * list->comp_per_page;
  unsigned int lower_bound = (list->page - 1) * list->comp_per_page;
  /*
    pages * comp_per_page = number of total components that can be shown
  */
  if (comp_bound > list->num_components) {
    comp_bound = list->num_components;
  }
  UI_COMPONENT *cur_comp = NULL;
  for (uintptr_t i = 0; i < list->num_components; i++) {
    cur_comp = get_ui_component_by_ID(LIST_ITEM) + i;
    vec2 listing_pos = {
      0.0,
      0.25 - (((float) (i % list->comp_per_page)) * (1.0 / (float) list->comp_per_page))
    };
    if (i >= lower_bound && i < comp_bound) {
      init_menu(
        listing_pos,
        (void (*)(void *)) select_listing_dispatcher,
        NULL,
        (void *) (i),
        NULL,
        list->listing_strings[i],
        1,
        1,
        0,
        0.05,
        0.75,
        1.0,
        0.2,
        PIVOT_CENTER,
        T_CENTER,
        cur_comp
      );
      cur_comp->enabled = 1;
      continue;
    }
    cur_comp->enabled = 0;
  }

  /* Create the forward and back arrow buttons */
  vec2 left_arrow = { -0.25, -0.75 };
  vec2 right_arrow = { 0.25, -0.75 };
  init_menu(
    left_arrow,
    (void (*)(void *)) scroll_left,
    NULL,
    (void *) list,
    NULL,
    "Previous",
    1,
    1,
    0,
    0.05,
    0.5,
    0.0,
    0.0,
    PIVOT_RIGHT,
    T_CENTER,
    get_ui_component_by_ID(LIST_BUTTON_LEFT)
  );

  init_menu(
    right_arrow,
    (void (*)(void *)) scroll_right,
    NULL,
    (void *) list,
    NULL,
    "  Next  ",
    1,
    1,
    0,
    0.05,
    0.5,
    0.0,
    0.0,
    PIVOT_LEFT,
    T_CENTER,
    get_ui_component_by_ID(LIST_BUTTON_RIGHT)
  );
  // Open mercenary context menu
  check_available_mercenaries();
}

void close_listings(UI_LIST *list) {
  UI_COMPONENT *left_arr = get_ui_component_by_ID(LIST_BUTTON_LEFT);
  UI_COMPONENT *right_arr = get_ui_component_by_ID(LIST_BUTTON_RIGHT);
  left_arr->enabled = 0;
  right_arr->enabled = 0;

  UI_COMPONENT *cur_listing;
  for (int i = 0; i < MAX_PER_PAGE; i++) {
    cur_listing = get_ui_component_by_ID(LIST_ITEM) + i;
    cur_listing->enabled = 0;
  }
  if (list->listing_strings) {
    for (int i = 0; i < num_trade_ships; i++) {
      if (list->listing_strings[i]) {
        free(list->listing_strings[i]);
        list->listing_strings[i] = NULL;
      }
    }
    free(list->listing_strings);
    list->listing_strings = NULL;
  }
}

/*
  Scrolls left in the lists and rolls
  over to the max page if the pages cannot
  be decremented
*/
void scroll_left(UI_LIST *list) {
  int max_page = (list->num_components / list->comp_per_page);
  /* Adjust for remainder in integer division */
  if (list->num_components % list->comp_per_page) {
    max_page++;
  }
  if (--list->page == 0) {
    list->page = max_page;
  }
  open_listing(list);
}

/*
  Scrolls right in the lists and rolls
  over to the min page if the pages cannot
  be incremented
*/
void scroll_right(UI_LIST *list) {
  int max_page = (list->num_components / list->comp_per_page);
  /* Adjust for remainder in integer division */
  if (list->num_components % list->comp_per_page) {
    max_page++;
  }
  if (++list->page > max_page) {
    list->page = 1;
  }
  open_listing(list);
}

void select_listing_dispatcher(int listing_selected) {
  /* Add checks for different use cases of ui_list */
  /* to determine which context the listing dispatcher*/
  /* needs to operate in */

  /* For example, if the below is true, set the listing */
  /* selection dispatcher to work in the context of the */
  /* mercenary reassignment menu */
  if (reassignment_menu_open) {
    mercenary_reassignment_selection(listing_selected);
  }
}
