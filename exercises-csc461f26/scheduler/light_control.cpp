#include <Arduino.h>     
#include "light_control.h"

#define POLL_RATE 500

#define BUTTON_DDR DDRB
#define BUTTON_PULLUP PORTB
#define BUTTON_PIN 0
#define READ_BUTTON PINB & (1<<BUTTON_PIN)

// breadboard led port and pin for pwm
// OC2B pin == PortD pin 3
#define BB_DDR DDRD
#define BB_PIN 3 

// save previous state of button
volatile uint8_t prev_state;

// set brightness (percent)
int light_brightness = 50;

void initialize_lights();
void run_lights();
void setup_pwm();


TASK_struct light_control_task = { 
  initialize_lights,
  run_lights,
  POLL_RATE,
  1, // always ready at start of period
  0  // next release time not set
};


void initialize_lights() {
  // initialize button
  BUTTON_DDR &= ~(1<<BUTTON_PIN);
  BUTTON_PULLUP |= (1<<BUTTON_PIN);

  // read button to initialize state
  prev_state = READ_BUTTON;

  // ready the pwm to control led brightness
  setup_pwm();

  // ready the led
  BB_DDR |= (1<<BB_PIN);

  log_lighting_status(light_brightness);
}

void run_lights() {
  // poll the button (pause for debounce)
  uint8_t state0 = READ_BUTTON;
  delay(20);
  uint8_t state1 = READ_BUTTON;
  if (state0 != state1) {
    // its bouncing, ignore it
    return;
  }
  
  // has it changed from the last time?
  int8_t change = state1 - prev_state;

  if (0==change) {
    return;
  }

  // only pay attention to release (low to high)
  if (change > 0) {

    // button change high to low, released
    // adjust the duty cycle
    light_brightness = (light_brightness + 10) % 100;
    OCR2B = (uint8_t)(0xFF * light_brightness/100);

    log_lighting_status(light_brightness);

    prev_state = state1;

  } else {
    // button pressed
    prev_state = state1;
  }
}

void setup_pwm() {

  // -----------------------------------     WAVEFORM GENERATION MODE --------
  // Metro sets pins in its initialization, so best to set and clear as needed.
  // WGM13       WGM12       WGM11       WGM10
  // TCCR1B 4    TCCR1B 3    TCCR1A 1    TCCR1A 0
  // Clear corresponding bits
  TCCR2A &= 0b11111100;
  TCCR2B &= 0b11110111;
  
  // Set as appropriate. PWM 0 1 1 with 0xFF as TOP
  //TCCR2B |= (1<<WGM22)
  TCCR2A |= (1<<WGM21);
  TCCR2A |= (1<<WGM20);

  OCR2B = (uint8_t)(0xFF * light_brightness/100);

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
