#include "recording.h"
#include <util/delay.h>

extern unsigned long now;

volatile uint8_t record_signal_received = 0;
volatile uint8_t finished_recording = 0;

unsigned long wait_expiry = 0;
unsigned long record_expiry = 0;

void begin_record_wait();
void run_record_wait();
void cleanup_record_wait();
void begin_recording();
void run_recording();
void cleanup_recording();

STATE_struct wait_record_state = { 
  begin_record_wait, 
  run_record_wait,
  cleanup_record_wait,
  LED_STATE_BLINK,
  250 };

STATE_struct recording_state = { 
  begin_recording, 
  run_recording,
  cleanup_recording,
  LED_STATE_BLINK,
  100 };

// set up record signal on Port D, pin 2
// external interrupt INT0

void initialize_recording() {

  states[WAIT_RECORD] = wait_record_state;
  states[RECORDING] = recording_state;

  // configure the "cameras"
  // set as output so that it becomes a software interrupt
  DDRD |= (1<<1);

  // enable INT0 on rising edge
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT0);

  record_signal_received = 0;
}

void begin_record_wait() {
  wait_expiry = now + RECORD_TIME_MS;
}

void run_record_wait() {
  if (now >= wait_expiry) {
    // send record signals
    RECORD_PORT &= ~(1 << RECORD_PIN);
    _delay_us(10);
    RECORD_PORT |=  (1 << RECORD_PIN);
  }
}

void cleanup_record_wait() {
  record_signal_received = 0;
  // reset interrupt signal to low (ready for next time) 
  RECORD_PORT &= ~(1 << RECORD_PIN); 
}

void begin_recording() {
  finished_recording = 0;
  record_expiry = now + RECORD_TIME_MS;
}

void run_recording() {
  if (now >= record_expiry) {
    finished_recording = 1;
  }
}

void cleanup_recording() {
  finished_recording = 0;
}

// software interrupt simulating signal from other module to begin recording
ISR(INT0_vect) {
  record_signal_received = 1;
}
