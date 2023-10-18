#include <stdio.h>
#include <player.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <cglm/vec2.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <globals.h>

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
