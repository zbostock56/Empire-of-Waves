#include <stdio.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <globals.h>

MERCHANT * get_closest_merchant(E_PLAYER e_player);
LISTING * get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int listing_index);
