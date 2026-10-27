/* 
Amy Larson
csc461 Project 3 Surveillance

Prototype for surveillance of a room using locally networked camera modules.

Hardware:
- Breadboard button on D3 (External interrupt INT1)
- Breadboard led on D7 (system state)
- On-board led on B5 (debugging)

Simulated Signals from Camera 0 and Camera 1 module:
- PCINT0 pin0 to simulate camera0 authorization of person entering room (software generated)
- PCINT0 pin1 to simulate camera1 aughorization of person entering room (software generated)
- INT0 to simulate signal from camera module to begin timed recording of camera feed
*/

#include <avr/io.h>
#include "states.h"
#include "leds.h"
#include "keycard.h"
#include "authorization.h"
#include "recording.h"

const int number_of_states = 6;

// this will be populated during initialization of keycard, recording, and authorization
STATE_struct states[number_of_states];

int state;
int next_state;
int transition = 0;

// represents ms since system startup. set at start of each loop.
unsigned long now;

/* State Machine Implementation
WAITING:
- Function: inactive. No leds.
- Transition: button release indicating person entering room. New state: AUTHORIZING

AUTHORIZING:
- Function: 
  - simulated face recognition, matching "image" to database of known faces
  - status led on
- Transitions:
    - person recognized by this module or other cameras. New state: WAIT_EXIT_AUTHORIZED
    - person not recognized. New state: WAIT_RECORDING

WAIT_EXIT_AUTHORIZED:
- Function: status led blinking 0.5 Hz (toggle every 1 second)
- Transition: button release indicating person leaving room. New state: WAIT_ENTRY

WAIT_RECORDING:
- Function: status led blinking 2 Hz (toggle every 250 ms)
- Transition: recording signal through external interrupt. New state: RECORDING

RECORDING
- Function: status led blinking 5 Hz (toggle every 100 ms)
- Transition: recording time expired. New state: WAIT_EXIT_UNAUTHORIZED

WAIT_EXIT_UNAUTHORIZED:
- Function: status led blinking 2 Hz (toggle every 250 ms)
- Transition: button release indicating person leaving room. New state: WAIT_ENTRY

*/

void setup() {

  initialize_leds();
  flash_leds();

  Serial.begin(9600);

  // disable interrupts while setting things up
  cli();

  initialize_keycard();
  initialize_recording();
  initialize_authorization();

  // start in wait entry state
  state = WAIT_ENTRY;
  states[state].transition_to();
  led_state = states[state].led_state;
  led_period = states[state].led_period;
  led_transition();

  sei();

  Serial.println("System initialized.");
}

void loop() {

  // marke the start time of the loop
  now = millis();

  // call the function that does the work for current state
  states[state].run();

  // initialize to "no transition"
  transition = 0;

  // check for transition activity in whichever state is current
  switch (state) {

    case WAIT_ENTRY:
    if (keycard_activity) {
      Serial.println("Person entered. Authorization in process.");
      transition = 1;
      next_state = AUTHORIZING;
    };
    break;
    
    case AUTHORIZING:
    if (person_authorized) {
      Serial.println("Person authorized. Wait for exit.");
      transition = 1;
      next_state = WAIT_EXIT_AUTHORIZED;
    } else if (person_unauthorized) {
      Serial.println("Person unauthorized. Waiting to record.");
      transition = 1;
      next_state = WAIT_RECORD;
    } // it may be that neither of these have yet to be determined
    break;
    
    case WAIT_EXIT_AUTHORIZED:
    if (keycard_activity) {
      Serial.println("Authorized person exited.");
      transition = 1;
      next_state = WAIT_ENTRY;
    };
    break;

    case WAIT_RECORD:
    if (record_signal_received) {
      Serial.println("Starting to record.");
      transition = 1;
      next_state = RECORDING;
    }
    
    case RECORDING:
    if (finished_recording) {
      Serial.println("Finished recording. Wait for exit.");
      transition = 1;
      next_state = WAIT_EXIT_UNAUTHORIZED;
    }
    break;

    case WAIT_EXIT_UNAUTHORIZED:
    if (keycard_activity) {
      Serial.println("Unauthorized person exiting.");
      transition = 1;
      next_state = WAIT_ENTRY;
    };
    break;
    
    default:
    Serial.println("ERROR: in unknown default state.");
  }

  // are we transitioning to a new state ?
  if (transition) {
    states[state].clean_up();
    state = next_state;
    states[state].transition_to();

    // set the led behavior for this new state
    led_state = states[state].led_state;
    led_period = states[state].led_period;

    // set leds appropriately for this new state
    led_transition();
  }

  // manage leds (for blink state)
  manage_leds();
}


