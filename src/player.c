#include <player.h>
#include <globals.h>

I_SLOT * get_player_inventory_slot_by_number(unsigned int item_number) {
    if (item_number > 0 || item_number < INV_SIZE) { // Argument validate
        return &e_player.inventory[item_number - 1];
    }
    return NULL;
}
