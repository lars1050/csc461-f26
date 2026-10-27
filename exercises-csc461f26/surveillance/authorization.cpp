#include "authorization.h"

extern unsigned long now;

// global status variables
volatile uint8_t person_authorized = 0;
volatile uint8_t person_unauthorized = 0;

// next time to confirm match to current person
unsigned long recognition_expiry = 0;

// PIN (PORT IN) value from previous reading 
uint8_t prev_cam_reading = 0;

// Testing 
//   -1        no one recognizes the person
//   1 to 25   this module recognizes person match_at
//   26 to 50  camera 0 recognizes person match_at - 25
//   51 to 75  camera 1 recognizes person match_at - 50
int match = 55;

int person = 1;

void initialize_authorization();
void begin_authorization();
void run_authorization();
void initialize_cameras();
void reset_cameras();

STATE_struct authorizing_state = { 
  begin_authorization, 
  run_authorization,
  reset_cameras,
  LED_STATE_ON,
  0 };

void initialize_authorization() {
  states[AUTHORIZING] = authorizing_state;
  initialize_cameras();
}

void begin_authorization() {
  person = 1;
  person_authorized = 0;
  person_unauthorized = 0;
  prev_cam_reading = CAMERA_IN;
  reset_cameras();
  recognition_expiry = now + TIME_TO_RECOGNIZE_MS;
}

// Simulated other cameras: raise camera 0 or 1's pin to send its signal.
void simulate_camera_response(int p) {
  if (p + 25 == match) {
    CAMERA_OUT |= (1 << CAMERA0);
  } else if (p + 50 == match) {
    CAMERA_OUT |= (1 << CAMERA1);
  }
}

// Compare the image to each authorized person (1 s each).
// Another camera can confirm the person at any time.
void run_authorization() {
  // is "face recognition processing" done
  if (now >= recognition_expiry) {
    Serial.print("checking user ");
    Serial.println(person);
    simulate_camera_response(person);
    if (match == person) {
      Serial.println("Authorized by this module.");
      person_authorized = 1;
    } else {
      person++;
      // checked all known persons in the database
      if (person > TOTAL_AUTHORIZED) {
        person_unauthorized = 1;
      }
    }
    recognition_expiry = now + TIME_TO_RECOGNIZE_MS;
  }
}


void initialize_cameras() {
  // set as input
  CAMERA_DDR |= (1 << CAMERA0) | (1 << CAMERA1); 
  // enable PCINT0 interrupt
  PCICR  |= (1 << PCIE0);
  // enable specific camera pin interrupts
  PCMSK0 |= (1 << CAMERA0) | (1 << CAMERA1);
}

void reset_cameras() {
  // clear signal, just in case
  CAMERA_OUT &= ~(1<<CAMERA0);
  CAMERA_OUT &= ~(1<<CAMERA1);
}

ISR(PCINT0_vect) {
  // which camera triggered the interrupt ?
  uint8_t current_reading = CAMERA_IN;
  uint8_t cam0_change = (current_reading & (1<<0)) - (prev_cam_reading & (1<<0));
  uint8_t cam1_change = (current_reading & (1<<1)) - (prev_cam_reading & (1<<1));

  // only pay attention to rising edge 1-0 (curr-prev > 1)
  if (cam0_change > 0 || cam1_change > 0) {
    person_authorized = 1;
  }
  // set up for next interrupt
  prev_cam_reading = current_reading;                 
}
