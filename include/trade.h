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

/* GLOBALS */
TRADE trade;
extern DIALOG dialog;
extern float time_schdule_trade_toute_prompt;
float time_trade_event_prompt;

/* ==================== INTERNALLY DEFINED FUNCITONS ================== */
void init_trade();
void open_trade();
void close_trade();
int set_trade(T_TRADE dialog_type);
void on_click_ui_listing(void *listing_ui_number);
void open_buy();
void open_sell();
void open_establish_trade_route();
void on_click_merchant_item(void *merchant_item_index);
void on_hover_merchant_item(void *merchant_item_index);
void on_click_player_item(void *player_item_index);
void on_hover_player_item(void *player_item_index);
void on_click_page_up(void *isMerchant);
void on_click_page_down(void *isMerchant);
void on_click_trade();

/* ==================== EXTERNALLY DEFINED FUNCTIONS ================== */
int add_chunk(ivec2);
void close_dialog();
int double_buffer(void **, unsigned int *, unsigned int);
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
I_SLOT * get_player_first_empty_inventory_slot();
I_SLOT * search_player_inventory_by_ID(ITEM_IDS item_id);
I_SLOT * get_player_inventory_slot_by_index(unsigned int item_index);
LISTING * get_merchant_listing_item_by_index(MERCHANT * merchant, unsigned int item_index);
LISTING * search_merchant_listing_by_ID(MERCHANT * merchant, ITEM_IDS item_id);
LISTING * get_merchant_first_empty_listing();
MERCHANT *get_closest_merchant(E_PLAYER e_player);
