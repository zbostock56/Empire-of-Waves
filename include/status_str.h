/* DEFINES */
#define MAX_STATUS_STR_LENGTH (64) // STATUS :: Max length of status string

typedef struct status {
  UI_COMPONENT *ui_health_status;
  UI_COMPONENT *ui_money_status;
} STATUS;

typedef struct status_menu {
  UI_COMPONENT *ui_status_menu;
  UI_COMPONENT *ui_menu_money;
  UI_COMPONENT *ui_menu_health;
  UI_COMPONENT *ui_menu_attack;
  UI_COMPONENT *ui_menu_fire;
  UI_COMPONENT *ui_menu_defense;
  UI_COMPONENT *ui_menu_speed;
} STATUS_MENU;