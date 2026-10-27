#ifndef BUTTONS_H
#define BUTTONS_H

#include <inttypes.h>
#include <avr/io.h>

extern int led_period;
extern int led_state;

#define LED_STATE_ON 0
#define LED_STATE_OFF 1
#define LED_STATE_BLINK 2

void initialize_leds();
void flash_leds();
void led_transition();
void manage_leds();

#endif
