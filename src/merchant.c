#include <merchant.h>

/*
                                   MERCHANT.c
Implements the functionality for enabling/disabling merchant prompts/menus,
modifying merchant relationships, and conducting trade.
*/

/*
Get the merchant listing item by given int number
Args:
MERCHANT * merchant
  The merchant that you need a listing
unsigned int item_number
  The item_number in the listing array, start with index zero
*/
LISTING *get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int listing_index) {
  if (merchant) {
    if (listing_index >= 0 && listing_index < merchant->num_listings) {
      if (merchant->listings) {
        return &merchant->listings[listing_index];
      }
    }
  }
  return NULL;
}

/*
Find first listing that is item_id of the merchant's listing
Return NULL when not find
Args:
MERCHANT * merchant
  pointer to the merchant
ITEM_IDS item_id
  the item that you want to search
*/
LISTING * search_merchant_listing_by_ID(MERCHANT * merchant, ITEM_IDS item_id) {
  for (int i = 0; i < merchant->num_listings; i++) {
    if (merchant->listings[i].item_id == item_id) {
      return &merchant->listings[i];
    }
  }
  return NULL;
}

/*
Get merchant's first empty listing and return a LISTING pointer
Return NULL if not found such empty listing
Args:
MERCHANT * merchant
  pointer to the merchant
*/
LISTING * get_merchant_first_empty_listing(MERCHANT * merchant) {
  for (int i = 0; i < merchant->listings_buf_size; i++) {
    if (merchant->listings[i].item_id == EMPTY) {
      return &merchant->listings[i];
    }
  }
  return NULL;
}

char *get_merchant_name(short index) {
  return merchant_name_list[index];
}

/*
  Helper function for resizing merchant listings
*/
int resize_listings(MERCHANT *merchant) {
  unsigned int old_size = merchant->listings_buf_size;
  int status = double_buffer((void **) &merchant->listings,
                             &merchant->listings_buf_size,
                             sizeof(LISTING));
  if (status) {
    return -1;
  }

  for (int i = old_size; i < merchant->listings_buf_size; i++) {
    merchant->listings[i].item_id = EMPTY;
    merchant->listings[i].quantity = 0;
  }

  return 0;
}

void add_listing(MERCHANT *merchant, ITEM_IDS item, unsigned int quantity) {
  LISTING *exists = search_merchant_listing_by_ID(merchant, item);
  if (exists) {
    exists->quantity += quantity;
    return;
  } else {
    merchant->listings[merchant->num_listings].item_id = item;
    merchant->listings[merchant->num_listings].quantity = quantity;
    merchant->num_listings++;
  }
}

/*
  Clears a given listing index to EMPTY and replaces it with the last item
  in the listing buffer
*/
void clear_listing(MERCHANT *merchant, unsigned int index) {
  merchant->num_listings--;
  merchant->listings[index] = merchant->listings[merchant->num_listings];
  merchant->listings[merchant->num_listings].item_id = EMPTY;
  merchant->listings[merchant->num_listings].quantity = 0;
}
