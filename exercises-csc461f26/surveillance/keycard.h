#ifndef ENTRY_H
#define ENTRY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "states.h"
#include "leds.h"

// flag person entered room
extern volatile uint8_t keycard_activity;

void initialize_keycard();

void reset_keycard();

#endif
