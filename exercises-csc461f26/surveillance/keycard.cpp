#include <Arduino.h>     
#include "keycard.h"

extern unsigned long now;

// flag to indicate keycard activated
volatile uint8_t keycard_activity = 0;

// for those state functions that do not do anything
void empty_fn() {}
void reset_keycard_activity();

STATE_struct wait_entry_state = { 
  reset_keycard_activity, // transition to
  empty_fn, // run. waiting for interrupt
  reset_keycard_activity,  // clean up
  LED_STATE_OFF,
  0 };

STATE_struct wait_exit_authorized_state = { 
  reset_keycard_activity, 
  empty_fn,
  reset_keycard_activity,
  LED_STATE_BLINK,
  1000 };

STATE_struct wait_exit_unauthorized_state = { 
  reset_keycard_activity, 
  empty_fn,
  reset_keycard_activity,
  LED_STATE_BLINK,
  250 };

void initialize_keycard_states() {
  states[WAIT_ENTRY] = wait_entry_state;
  states[WAIT_EXIT_AUTHORIZED] = wait_exit_authorized_state;
  states[WAIT_EXIT_UNAUTHORIZED] = wait_exit_unauthorized_state;
}

// "KEYCARD" on Port D Pin 3
// external interrupt INT1

void initialize_keycard() {
  
  initialize_keycard_states();

  keycard_activity = 0;

  // set as input with pull-up activated
  DDRD &= ~(1<<3);
  PORTD |= (1<<3);

  // enabale interrupt rising edge on INT1
  EICRA |= (1 << ISC11) | (1 << ISC10); 

  // enable interrupt INT1
  EIMSK |= (1 << INT1); 
}


void reset_keycard_activity() {
  keycard_activity = 0;
}

// Activates on relase of button (rising edge)
ISR(INT1_vect) {

  PORTB ^= (1<<5);
  keycard_activity = 1;      
}

