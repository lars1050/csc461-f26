#include <avr/io.h>

#include "camera_coms.h"

#define LED_PIN 5

// time to check each authorized person. 
#define FACE_REC_TIME 500

// total number of authorized persons to check in facial recognition
#define TOTAL_AUTHORIZED 25

// states
#define AUTHORIZING 1
#define WAIT_EXIT 2

void simulate_camera_response(int);
void simulate_feed_response(void);

// simulated call to facial recognition to see if image matches current person
int match(int person);

// variables needed for authorization
extern volatile uint8_t camera0_confirmed;

// for setting timers
unsigned long start_time;

// current person being checked against image during facial recognition
int person;

// for testing purposes. if match_at = ...
// -1, no authorized person will be recognized
// 1 to 25, this module will recognize when it checks that person
// 26 to 50, camera0 will recogize when it checks match_at-25
// 51 to 75, camera1 will recognize when it check match_at-50
int match_at = 35;

// start here since we aren't doing anything else
int state = AUTHORIZING;

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
  
  // simulated signals from camera modules to indicate a match
  initialize_PCINT0();

  // initialize authorizing state
  // this should go within the state that transitions to authorizing
  // but authorizing is the only state that is being implemented here
  int person = 1;
  start_time = millis();

  // let em know you are ready to go
  Serial.println("Initialized!");

  // enable all interrupts
  sei();
}

void loop() {

  switch(state) {

// ----------------------------------- AUTHORIZING STATE ------------------
    case(AUTHORIZING): 
    // did we get a confirmation from another camera module?
    if (camera0_confirmed) {
      Serial.println("Person is authorized. Camera 0 confirmed. Wait for them to leave.");
      camera0_confirmed = 0;
      state = WAIT_EXIT;
      PCINT0_clear(0);  // reset for a new interrupt 
    }
    // is facial recognition done for this person
    if (millis() - start_time >= FACE_REC_TIME) {
      // if the image matched this authorized person, we are good
      if (match(person)) {
        Serial.println("Person is authorized by this module. Wait for them to leave.");
        start_time = millis();
        state = WAIT_EXIT;
      } else {
        // image doesnt match this authorized person, go to the next
        person++;
        // did we make it through all of them with no match?
        if (person>TOTAL_AUTHORIZED) {
          Serial.println("Person is not authorized!");
          start_time = millis();
          state = WAIT_EXIT; // this is not right, but we aren't implementing the other states.
        } else {
          Serial.print("Checking authorized person ");
          Serial.println(person);
        }
        // this will simulate a response from the other camera modules
        simulate_camera_response(person);
      } // end if match person
      start_time = millis();
    } // end if time expiry
  break;
  
  // ----------------------------------- WAIT to EXIT STATE ------------------
  case(WAIT_EXIT):
    // give us a heartbeat to show we are in the loop
    PORTB |= (1<<LED_PIN);
    delay(500);
    PORTB &= ~(1<<LED_PIN);
    delay(500);
    
    // restart authorization after 6 seconds to ensure reset is functional
    if (millis() - start_time >= 6000) {
      state = AUTHORIZING;
      person = 1;
    }
  break;

  default:
    Serial.println("In a bad state man!");
  } // end switch
}

int match(int person) {
  // simulate facial recognition
  return person == match_at;
}


void simulate_camera_response(int person) {
  // are we matching on camera 0 or camera 1 module?
  if ((person+25)==match_at) {
    // send the signal to trigger camera 0 interrupt
    PCINT0_set(0);
  } else if ((person+50)==match_at) {
    // send the signal to trigger camera 1 interrupt
    PCINT0_set(1);
  }
}

void simulate_feed_response() {
  // trigger the external interrupt 0 to simulate signal from camera
  // it is time to record the feed
}

