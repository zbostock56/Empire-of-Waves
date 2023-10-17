#include <stdio.h>
#include <globals.h>
#include <ui_component.h>
#include <merchant_str.h>
#include <dialog_str.h>
#include <trade_str.h>
#include <chunk_str.h>
#include <menu.h>

/*
                                   TRADING
Implements the functionality for buying and selling. Could be used for 
interaction with merchants.
*/

/* DEFINES  */
#define MAX_LISTING_STR_LENGTH (16) // TRADE :: Max length of listing string

/* GLOBALS */
TRADE trade;
extern DIALOG dialog;
extern float time_schdule_trade_toute_prompt;

/* ==================== INTERNALLY DEFINED FUNCITONS ================== */
void init_trade();
void free_trade();
void open_trade();
void close_trade();
int set_trade(T_TRADE dialog_type);
void on_click_ui_listing();
void open_buy();
void open_sell();
void open_establish_trade_route();

/* ==================== EXTERNALLY DEFINED FUNCTIONS ================== */
int add_chunk(ivec2);
void close_dialog();
int double_buffer(void **, unsigned int *, unsigned int);
void init_menu(vec2 position, void (*on_click)(void *), void *on_click_args,
               char *text, int enabled, int textured, unsigned int texture,
               float text_padding, float text_scale, float width, float height,
               PIVOT pivot, TEXT_ANCHOR text_anchor, UI_COMPONENT *dest);
I_SLOT * get_player_first_empty_inventory_slot();
I_SLOT * search_player_inventory_with_ID(ITEM_IDS item_id);
I_SLOT * get_player_inventory_slot_by_number(unsigned int item_number);
LISTING * get_merchant_listing_item_by_number(MERCHANT * merchant, unsigned int item_number);
MERCHANT *get_closest_merchant(E_PLAYER e_player);
