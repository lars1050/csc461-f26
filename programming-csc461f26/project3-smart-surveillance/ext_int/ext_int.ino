#include <avr/io.h>

#include "camera_coms.h"

#define LED_PIN 5

unsigned long ms_ticks = 1;

void setup() {

  // setup the built-in LED
  DDRB |= (1 << LED_PIN);
  
  int i;
  for (i=0; i<3; i++) {
    PORTB |= (1<<LED_PIN);
    delay(250);
    PORTB &= ~(1<<LED_PIN);
    delay(250);
  }
  
  initialize_INT0();

  // enable all interrupts
  sei();
}

void loop() {
  
  //Serial.println("in loop ");
  //CLEAR_INT0;

  // give us a heartbeat to show we are in the loop
  delay(2000);
  PORTB |= (1<<LED_PIN);
  delay(500);
  PORTB &= ~(1<<LED_PIN);
  delay(500);;
    
  // signal the external interrupt (imagine that this really was external)
  INT0_CLEAR;
  delay(10);
  INT0_TRIGGER;
}

