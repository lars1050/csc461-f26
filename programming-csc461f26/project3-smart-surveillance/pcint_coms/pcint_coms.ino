#include <avr/io.h>

#include "camera_coms.h"

#define LED_PIN 5

unsigned long ms_ticks = 1;

void setup() {

  // setup the built-in LED
  DDRB |= (1 << LED_PIN);
  
  // flash to know we are in initialization
  int i;
  for (i=0; i<3; i++) {
    PORTB |= (1<<LED_PIN);
    delay(250);
    PORTB &= ~(1<<LED_PIN);
    delay(250);
  }

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  
  initialize_PCINT0();

  // prints title with ending line break
  Serial.println("Initialized!");

  // enable all interrupts
  sei();
}

void loop() {
  
  // slowing the control loop so we can see what is happening
  delay(2000);

  // this should trigger an interrupt, but it should be ignored
  PCINT0_clear(0);
  delay(1000);

  // camera should not be confirmed from this ISR
  if (camera0_confirmed) {
    Serial.println("camera 0 confirmed");
    // message received. reset for next message.
    camera0_confirmed = 0;
  } else {
    Serial.println("no go camera 0");
  }

  // give us a heartbeat to show we are in the loop
  PORTB |= (1<<LED_PIN);
  delay(500);
  PORTB &= ~(1<<LED_PIN);
  delay(500);;
    
  // signal the external interrupt (imagine that this really was external)
  // give the signal time to propagate and trigger the interrupt
  PCINT0_set(0);
  delay(10);

  // now we should see that the camera is confirmed
  if (camera0_confirmed) {
    Serial.println("camera 0 confirmed");
    // message received. reset for next message.
    camera0_confirmed = 0;
  } else {
    Serial.println("no go camera 0");
  }
}

