// Built in led on port B pin 5

#define BUILTIN_PIN 5

// breadboard led port and pin settings
// OC2B pin == PortD pin 3
#define BB_DDR DDRD
#define BB_PORT PORTD
#define BB_PIN 3 

// Use built-in LED for debugging
// Use the PWM to control the brightness of the led

// Use Fast PWM with 0xFF at the top (WGM mode 3 ==> 011)

float duty_cycle = .5;

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
  TCCR2A &= 0b11111100;
  TCCR2B &= 0b11110111;
  
  // Set as appropriate. PWM 0 1 1 with 0xFF as TOP
  //TCCR1B |= (1<<3);
   TCCR2A |= (1<<1);
   TCCR2A |= (1<<0);

  OCR2B = 0xFF * duty_cycle;

  // ----------------------------------   COM Mode ----------------------
  // For PWM, use Table 17-6 to control signal on OC2B pins
  // COM2B1 and COM2B0 are on TCCR2A bits 5 and 4
  // clear the bits
  TCCR2A &= 0b11001111;

  // set to clear on match (10) 
  TCCR2A |= (1<<5);
  //TCCR2A |= (1<<4);

	
  // ------------------  CLOCK SELECT BITS  -----------------------
  // CS12 CS11 and CS10 bits are on TCCR2B bits 2,1,0
  // Do not clear all (no clock)

  // set prescaler (see Table 17-9). Prescale 8 (010)
  //TCCR2B |= (1<<2);
  TCCR2B &= ~(1<<2);
  
  TCCR2B |= (1<<1);
  //TCCR2B &= ~(1<<1); 
  
  //TCCR2B |= (1<<0);
  TCCR2B &= ~(1<<0);
}

void loop() {

  // heartbeat
  PORTB ^= (1<<BUILTIN_PIN);
  delay(400);

  duty_cycle += 0.05;
  if (duty_cycle >= 1.0) {
    duty_cycle = 0.0;
  }
  OCR2B = 0xFF * duty_cycle;
}
