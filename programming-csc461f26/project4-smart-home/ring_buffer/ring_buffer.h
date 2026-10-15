#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <inttypes.h>

#define RB_SIZE 5

typedef struct {
    int16_t data[RB_SIZE];
    uint8_t head;    /* holds oldest value */
    uint8_t tail;   /* index where to add next */
    uint8_t count;   /* number of valid values (0..RB_SIZE)         */
} ring_buffer_t;

extern volatile ring_buffer_t rb;

void rb_init(void);
void rb_push(int16_t value);
int16_t rb_pop();
int16_t rb_peek();
int rb_is_full(void);
int rb_is_empty(void);

#endif