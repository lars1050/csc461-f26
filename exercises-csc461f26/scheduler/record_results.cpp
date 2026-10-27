#include <Arduino.h>     
#include "record_results.h"

void initialize_record();
void run_record();

TASK_struct record_results_task = { 
  initialize_record,
  run_record,
  1000,
  0, // released set by interrupt
  0  // next release time not set
};


void initialize_record() {
  
  // set up interrupt
  // set up the semaphor
}

void run_record() {
  // poll for semaphor
  // raise interrupt
  // clear semaphor
}


