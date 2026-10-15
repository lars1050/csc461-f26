#include <avr/io.h>

#define BUILTIN_PIN 5	// PORT B

// status led
#define BB_DDR DDRD
#define BB_PORT PORTD
#define BB_PIN 3

// PIR on INT0 == Port D pin 2
#define PIR_DDR DDRD
#define PIR_PORTIN PIND
#define PIR_PIN 2

uint8_t pir_state;

void setup() {

  // setup the LEDs
  DDRB |= (1 << BUILTIN_PIN);
  BB_DDR |= (1 << BB_PIN);

  // set up the PIR to read in
  PIR_DDR &= ~(1<<PIR_PIN);
  
  int i;
  for (i=0; i<3; i++) {
    PORTB |= (1<<BUILTIN_PIN);
    BB_PORT |= (1<<BB_PIN);
    delay(500);
    PORTB &= ~(1<<BUILTIN_PIN);
    BB_PORT &= ~(1<<BB_PIN);
    delay(500);
  }

  // enable external interrupt 0 on pin change (mode 01 on bits 1,0)
  //EICRA |= (1<<1);
  EICRA &= ~(1<<1);
  EICRA |= (1<<0);
  EIMSK |= (1 << 0);  // enable int0
}


ISR(INT0_vect) { 
  uint8_t state = PIR_PORTIN &= (1<<PIR_PIN);
  state = (state != 0);
  // helpful to flash led (B5) to see when you are inside the ISR.
  // delay does not work in an interrupt
  volatile int i;
  volatile long int j;
  for (i=0; i<5; i++) {
    PORTB |= (1<<BUILTIN_PIN);  
    for (j=0; j<10000; j++) {}
    PORTB &= ~(1<<BUILTIN_PIN);
    for (j=0; j<10000; j++) {}
  }
  if (state==1) {
    BB_PORT |= (1<<BB_PIN);
  } else {
    BB_PORT &= ~(1<<BB_PIN);
  }
} 


void loop() {
  
  //Serial.println("in loop ");
  
  // give us a heartbeat to show we are in the loop
  delay(1000);
  PORTB ^= (1<<BUILTIN_PIN);
}

