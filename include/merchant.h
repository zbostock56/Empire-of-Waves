#include <merchant_str.h>
#include <player_str.h>

MERCHANT * get_closest_merchant(E_PLAYER e_player);
LISTING * get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int listing_index);
LISTING * search_merchant_listing_by_ID(MERCHANT * merchant, ITEM_IDS item_id);
LISTING * get_merchant_first_empty_listing(MERCHANT * merchant);
