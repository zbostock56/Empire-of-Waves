#include <stdio.h>
#include <player.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <cglm/vec2.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <globals.h>
#include <trade_str.h>
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
  for (int i = 0; i < merchant->listings_buf_size; i++) {
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
    if (merchant->listings[i].item_id == 0 || merchant->listings[i].item_id == 0) {
      return &merchant->listings[i];
    }
  }
  return NULL;
}