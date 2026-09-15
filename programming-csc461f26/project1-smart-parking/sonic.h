#ifndef SONIC_H
#define SONIC_H

#include <avr/io.h>
#include <util/delay.h>

// Trigger Output to signal the sensor
#define TRIG_DDR DDRB
#define TRIG_PORT PORTB
#define TRIG_PIN 0

// Echo input: Sensor signals when echo received
#define ECHO_DDR DDRB
#define ECHO_INPORT PINB
#define ECHO_PIN 1

void sonic_init();
float sonic_read_distance();

#endif