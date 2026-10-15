#include "ring_buffer.h"

volatile ring_buffer_t rb;

void rb_init(void) {
    rb.head = 0;
    rb.tail = 0;
    rb.count = 0;
}

/* Add a value. If the buffer is full, the oldest value is overwritten. */
void rb_push(int16_t value) {

    rb.data[rb.tail] = value;
    rb.tail = (rb.tail + 1) % RB_SIZE;
 
    if (rb.count < RB_SIZE) {
        rb.count++;
    } else {
        /* full: we just overwrote the oldest item, so move the head forward */
        rb.head = (rb.head + 1) % RB_SIZE;
    }
}

/* Remove and return the oldest value. Returns false if empty. */
int16_t rb_pop() {
    if (rb.count == 0) {
        // error checking the pop. returning bogus value if empty
        return -1;
    }
    int16_t value = rb.data[rb.head];
    rb.head = (rb.head + 1) % RB_SIZE;
    rb.count--;
    return value;
}

/* Peek at next element to be popped [head] */
int16_t rb_peek() {
    if (rb.count==0) {
        return -1;
    }
    return rb.data[rb.head];
}

int rb_is_full(void)  { return rb.count == RB_SIZE; }
int rb_is_empty(void) { return rb.count == 0; }
