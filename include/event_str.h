#define NUM_TIMERS (3)
#define C_CURSOR_TIME (0.25)
#define C_ERROR_TIME (1.5)
#define TRADE_ERROR_TIME (1.5)

typedef enum timer_list {
  CONS_CURSOR,
  CONS_ERROR,
  RELATIONSHIP_TOO_LOW,
} TIMER_LIST;
