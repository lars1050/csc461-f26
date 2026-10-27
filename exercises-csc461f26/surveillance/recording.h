#ifndef RECORD_SIGNAL_H
#define RECORD_SIGNAL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "states.h"
#include "leds.h"

#define RECORD_TIME_MS       12000

#define RECORD_DDR   DDRD
#define RECORD_PORT  PORTD
#define RECORD_PIN   2

extern volatile uint8_t record_signal_received;
extern volatile uint8_t finished_recording;

void initialize_recording();
void record_signal_send();
void record_signal_reset();

#endif
