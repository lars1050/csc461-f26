#include "leds.h"
#include <Arduino.h>

extern unsigned long now;

unsigned long led_expiry = 0;

// local scope only
typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  uint8_t pin;
} LED_struct;

LED_struct debug_led = { &DDRB, &PORTB, 5};
LED_struct status_led = { &DDRD, &PORTD, 7};

int led_period = 0; 
int led_state = LED_STATE_OFF;

// configure as output
void configure(LED_struct led) {
  *led.ddr |= (1<<led.pin);
}

void on(LED_struct led) {
  *led.port |= (1<<led.pin);
}

void off(LED_struct led) {
  *led.port &= ~(1<<led.pin);
}

void toggle(LED_struct led) {
  *led.port ^= (1<<led.pin);
}

void initialize_leds() {

    configure(debug_led);
    off(debug_led);

    configure(status_led);
    off(status_led);
}

void led_transition() {

  // LED_ON (turn on). LED_OFF or TOGGLE (turn off)
  if (LED_STATE_ON == led_state) {
    on(status_led);
  } else {
    off(status_led);
  }

  // start timer if toggling
  if (LED_STATE_BLINK == led_state) {
    led_expiry = now + led_period;
  }
}

void manage_leds() {
  if ((LED_STATE_BLINK == led_state) && (now >= led_expiry)) {
    toggle(status_led);
    led_expiry = now + led_period;
  }
}


/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_leds() {
  int i;
  for (i=0; i<6; i++) {
    toggle(debug_led);
    toggle(status_led);
    delay(200);
  }
  // double check it is off
  off(debug_led);
  off(status_led);
}

