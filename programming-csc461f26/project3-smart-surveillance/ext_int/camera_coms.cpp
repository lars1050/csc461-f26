#include "camera_coms.h"

// EICRA: External Interrupt Control Register A
// EIMSK: Enable Interrupt Mask

void initialize_INT0() {

  // set the interrupt pin to output, so the setting initializes a software interrupt
  INT0_INIT_PIN;

  // clear the pin to be ready for a rising edge
  INT0_CLEAR;

  // enable external interrupt 0 on rising edge
  EICRA |= INT0_ON_RISE;
  EIMSK |= (1 << INT0_ENABLE_PIN);
}

void initialize_INT1() {
  
  // set the interrupt pin to output, so the setting initializes a software interrupt
  INT1_INIT_PIN;

  // clear the pin to be ready for a rising edge
  INT1_CLEAR;

  // enable external interrupt 1 on rising edge
  EICRA |= INT1_ON_RISE;
  EIMSK |= (1<< INT1_ENABLE_PIN);
}

ISR(INT0_vect) { 
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

  // clear the interrupt pin (then ready for next on rising edge)
  INT0_CLEAR;
} 

ISR(INT1_vect) { 

  // helpful to flash leds to see when you are inside the ISR.
  // delay does not work in an interrupt
  volatile int i;
  volatile long int j;
  for (i=0; i<10; i++) {
    PORTB |= (1<<5);
    for (j=0; j<10000; j++) {}
    PORTB &= ~(1<<5);
    for (j=0; j<10000; j++) {}
  }

  // clear the interrupt pin (then ready for next on rising edge)
  INT1_CLEAR;
}
