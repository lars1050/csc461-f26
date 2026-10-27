#ifndef STATES_H
#define STATES_H

#include <inttypes.h>

typedef struct {
  void (*transition_to)(void);
  void (*run)(void);
  void (*clean_up)(void);
  uint8_t led_state;
  uint16_t led_period;
} STATE_struct;

extern STATE_struct states[];

#define WAIT_ENTRY 0
#define AUTHORIZING 1
#define WAIT_EXIT_AUTHORIZED 2
#define WAIT_RECORD 3
#define RECORDING 4
#define WAIT_EXIT_UNAUTHORIZED 5

#endif
