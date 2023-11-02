#define NUM_TIMERS (3)
#define C_CURSOR_TIME (0.25)
#define C_ERROR_TIME (1.5)
#define GENERAL_PROMPT_TIME (1.5)

typedef enum timer_list {
  CONS_CURSOR,
  CONS_ERROR,
  GENERAL_PROMPT,
} TIMER_LIST;

extern float timers[NUM_TIMERS];
extern int event_flags[NUM_TIMERS];
