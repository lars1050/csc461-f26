#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <Arduino.h>
#include "states.h"
#include "leds.h"

#define TIME_TO_RECOGNIZE_MS     1000
#define TOTAL_AUTHORIZED     25

#define CAMERA_DDR DDRB
#define CAMERA_OUT PORTB
#define CAMERA_IN PINB

#define CAMERA0 PCINT0
#define CAMERA1 PCINT1

extern volatile uint8_t person_authorized;
extern volatile uint8_t person_unauthorized;

void initialize_authorization();

#endif
