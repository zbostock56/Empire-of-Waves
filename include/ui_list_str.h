// Defines maximum number of ui components per page of a ui list
#define MAX_PER_PAGE (16)

typedef struct ui_list {
  char **listing_strings;
  /* One indexed (first page = page 1) */
  unsigned int page;
  unsigned int comp_per_page;
  unsigned int num_components;
} UI_LIST;
