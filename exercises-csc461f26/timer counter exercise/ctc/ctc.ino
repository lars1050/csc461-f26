// Built in led on port B pin 5

#define BUILTIN_PIN 5

// breadboard led port and pin settings
#define BB_DDR DDRD
#define BB_PORT PORTD
#define BB_PIN 4 

// Use built-in LED for debugging
// Use breadboard LED on D4 in coordination with interrupts

// Use CTC Clear Timer Counter on Match Mode.
// This means that the timer counter counts to TOP (OCR1A), then resets

void setup() {

  // ------------------------------- INITIALIZE LEDs  ---------------------
  // initialize the breadboard led
  BB_DDR |= (1<<BB_PIN);

  // initialize the built in led
  DDRB |= (1<<BUILTIN_PIN);

  // flash to know we are in initialization
  int i;
  for (i=0; i<3; i++) {
    PORTB |= (1<<BUILTIN_PIN);
    BB_PORT |= (1<<BB_PIN);
    delay(500);
    PORTB &= ~(1<<BUILTIN_PIN);
    BB_PORT &= ~(1<<BB_PIN);
    delay(500);
  }

  // -----------------------------------     WAVEFORM GENERATION MODE --------
  // Metro sets pins in its initialization, so best to set and clear as needed.
  // WGM13       WGM12       WGM11       WGM10
  // TCCR1B 4    TCCR1B 3    TCCR1A 1    TCCR1A 0
  // Clear corresponding bits
  TCCR1A &= 0b11111100;
  TCCR1B &= 0b11100111;
  
  // Set as appropriate. CTC 0 1 0 0 with OCR1A as TOP
  // TCCR1B |= (1<<4);
  TCCR1B |= (1<<3);
  // TCCR1A |= (1<<1);
  // TCCR1A |= (1<<0);

  OCR1A = 1000;

  // ----------------------------------   COM Mode ----------------------
  // For CTC, use Table 15-2 to control signal on OC pins
  // COM1A1 and COM1A0 are on TCCR1A bits 7 and 6
  // clear the bits
  TCCR1A &= 0b00111111;

  // set as appropriate
  //TCCR1A |= (1<<7);
  //TCCR1A |= (1<<6);

	
  // ------------------  CLOCK SELECT BITS  -----------------------
  // CS12 CS11 and CS10 bits are on TCCR1B bits 2,1,0
  // Do not clear all (no clock) or set as 110 or 111 (external clocks)

  // set prescaler (see Table 15-6). For example, 256 is 100
  TCCR1B |= (1<<2);
  //TCCR1B &= ~(1<<2);
  
  //TCCR1B |= (1<<1);
  TCCR1B &= ~(1<<1); 
  
  //TCCR1B |= (1<<0);
  TCCR1B &= ~(1<<0);


  // here we go

  // --------------------------   INTERRUPT ENABLE. -------------------
  // enable interrupts through TIMSK1
  TIMSK1 &= 0b11111000; // clear the corresponding bits
  
  // Set as appropriate
  //TIMSK1 |= (1<<0);   // overflow
  TIMSK1 |= (1<<1);   // OCR1A match
  //TIMSK1 |= (1<<2);   // OCR1B match

  sei();
}

// potential interrupts associated with Timer/Counter 1

/*
ISR(TIMER1_OVF_vect) {

}
*/

ISR(TIMER1_COMPA_vect) {
  BB_PORT ^= (1<<BB_PIN); 
}

/*
ISR(TIMER1_COMPB_vect) {
 
}
*/

void loop() {

  // heartbeat
  PORTB ^= (1<<BUILTIN_PIN);
  delay(500);
}
