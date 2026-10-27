#include <Arduino.h>     
#include "status_report.h"

uint8_t brightness = 0;

void initialize_status();
void run_status();

TASK_struct status_report_task = { 
  initialize_status,
  run_status,
  1000,
  1, // always ready at start of period
  0  // next release time not set
};

void initialize_status() {
  Serial.begin(9600);
  Serial.println("Status log initialized.");
}

void run_status() {
  Serial.print("Log ");
  Serial.print(millis()/1000);
  Serial.print(" ** ");
  Serial.print("LED brightness: ");
  Serial.print(brightness);
  Serial.println("%");
}

void log_lighting_status(int b) {
  brightness = b;
}


