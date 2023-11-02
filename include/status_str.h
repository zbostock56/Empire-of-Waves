/* DEFINES */
#define MAX_STATUS_STR_LENGTH (64) // STATUS :: Max length of status string

typedef struct status {
  UI_COMPONENT *ui_health_status;
  UI_COMPONENT *ui_copper_icon;
  UI_COMPONENT *ui_copper_text;
  UI_COMPONENT *ui_silver_icon;
  UI_COMPONENT *ui_silver_text;
  UI_COMPONENT *ui_gold_icon;
  UI_COMPONENT *ui_gold_text;
} STATUS;
