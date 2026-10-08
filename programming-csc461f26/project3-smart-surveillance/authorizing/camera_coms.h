#ifndef COMS_H_
#define COMS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

extern volatile uint8_t camera0_confirmed;

// PCINT0 corresponds to PORT B Pins 7 to 0
#define PCINT0_PORT PORTB
#define PCINT0_DDR DDRB
#define PCINT0_PORTIN PINB


void initialize_PCINT0();
void PCINT0_clear(int);
void PCINT0_set(int);

#endif