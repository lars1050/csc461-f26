#include "camera_coms.h"

volatile uint8_t camera0_confirmed = 0;

uint8_t pin0_prev_state = 0;

// TODO: add code to manage a 2nd camera on pin 1 of PCINT0
void PCINT0_clear(int pin_to_set) {
  if ((0!=pin_to_set) && (1 != pin_to_set)) {
    // ignoring this request
    return;
  }
  PCINT0_PORT &= ~(1<<pin_to_set);
}

void PCINT0_set(int pin_to_set) {
  if ((0!=pin_to_set) && (1 != pin_to_set)) {
    // ignoring this request
    return;
  }
  PCINT0_PORT |= (1<<pin_to_set);
}

void initialize_PCINT0() {

  // set pins as output to simulate external interrupt
  PCINT0_DDR |= (1<<0) | (1<<1);

  // clear the pin(s) to be ready for a rising edge interrupt
  PCINT0_clear(0);

  // set status of camera readiness
  camera0_confirmed = 0;

  // enable the PCINT0 interrupt for all unmasked pins
  PCICR |= (1<<0);

  // enable PCINT0 interrupt for pin 0 (i.e. unmask pin 0)
  PCMSK0 |= (1<<0);
}


ISR(PCINT0_vect) { 

  // read the pin(s) to determine what triggered the interrupt
  uint8_t pin0_state = (PINB & (1<<0)) != 0;

  // helpful to flash led (B5) to see when you are inside the ISR.
  // delay does not work in an interrupt
  volatile int i;
  volatile long int j;
  for (i=0; i<10; i++) {
    PORTB |= (1<<5);  
    for (j=0; j<10000; j++) {}
    PORTB &= ~(1<<5);
    for (j=0; j<10000; j++) {}
  }
  if (pin0_prev_state != pin0_state) {
    // this triggered an interrupt. is it on the rising edge?
    if (1==pin0_state) {  
        camera0_confirmed = 1;
    }
    pin0_prev_state = pin0_state;
  }
} 

