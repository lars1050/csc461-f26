#include "face_recognition.h"

void initialize_recognition();
void run_recognition();

TASK_struct recognition_task = { 
  initialize_recognition,
  run_recognition,
  1000,
  1, // always ready at start of period
  0  // next release time not set
};

void initialize_recognition() {
  // use on-board led for status
  DDRB |= (1<<5);
  PORTB &= ~(1<<5);
}

void run_recognition() {
  // recognize a single image
  // estimate 400 ms
  PORTB |= (1<<5);
  delay(500);
  PORTB &= ~(1<<5);
}


