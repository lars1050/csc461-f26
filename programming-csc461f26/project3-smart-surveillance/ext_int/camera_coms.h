#ifndef COMS_H_
#define COMS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

// external interrupt 0 is on Port D pin 2
#define INT0_TRIGGER PORTD |= (1<<2)
#define INT0_CLEAR PORTD &= ~(1<<2)
#define INT0_INIT_PIN DDRD |= (1 << 2);

#define INT0_ENABLE_PIN 0
#define INT0_ON_RISE (1<<0) | (1<<1)

// external interrupt 0 is on Port D pin 3
#define INT1_TRIGGER PORTD |= (1<<3)
#define INT1_CLEAR PORTD &= ~(1<<3)
#define INT1_INIT_PIN DDRD |= (1 << 3)

#define INT1_ENABLE_PIN 1
#define INT1_ON_RISE (1<<2) | (1<<3)

void initialize_INT0();
void initialize_INT1();

#endif