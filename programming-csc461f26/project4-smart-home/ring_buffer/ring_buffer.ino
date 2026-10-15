#include <inttypes.h>

#include "ring_buffer.h"

extern volatile ring_buffer_t rb;

void setup() {
  rb_init();
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // prints title with ending line break
  Serial.println("Testing Ring Buffer");
}

void loop() {
    if (rb_is_empty()) {
        Serial.println("Good. We are empty!");
    } else {
        Serial.println("Hmmm. Just starting and not empty.");
    }

    // push to full
    for (int16_t n = 1; n <= 5; n++) {
        rb_push(n);
    }
    if (rb_is_full()) {
        Serial.println("Good. We are now full!");
    } else {
        Serial.println("Hmmm. Pushed but not full.");
    }

    // Work on our overwrite
    Serial.print("About to overwrite ");
    Serial.println(rb_peek());
    rb_push(6);

    Serial.print("About to overwrite ");
    Serial.println(rb_peek());
    rb_push(7);

    // pop off (with one extra when empty)
    Serial.print("Popping ");
    for (int16_t i = 0; i < 6; i++) {
        Serial.print(rb_pop());
    }
    Serial.println();
    
    delay(3000);
}
