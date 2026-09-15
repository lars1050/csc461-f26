// inspired from 
// Google Gemini with prompt: avr example hcsr04

// Trigger and Echo are on Port B, pins 0 and 1 (metro 8,9)

// Trigger Output to signal the sensor
#define TRIG_DDR DDRB
#define TRIG_PORT PORTB
#define TRIG_PIN 0

// Echo input: Sensor signals when echo received
#define ECHO_DDR DDRB
#define ECHO_INPORT PINB
#define ECHO_PIN 1

// Metro built in is Port B pin 5
#define BOARDLED 5

// count microsecond (us) waiting for return signal
uint16_t count;

// calculated distance in cm based on timing of signal
float distance;

void setup() {

  TRIG_DDR |= (1 << TRIG_PIN);  // Set Trig as output
  ECHO_DDR &= ~(1 << ECHO_PIN); // Set Echo as input

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {

  // Trigger the HCSR04 to send out a pulse
  TRIG_PORT &= ~(1 << TRIG_PIN);
  _delay_us(2);
  TRIG_PORT |= (1 << TRIG_PIN);  // HIGH pulse
  _delay_us(10);             // 10 microsecond width
  TRIG_PORT &= ~(1 << TRIG_PIN); // LOW pulse

  // Wait for Echo to go HIGH (to start timing)
  while (!(ECHO_INPORT & (1 << ECHO_PIN)));
    
  count = 0;
  
  // Count while Echo is HIGH (approximate microsecond calibration)
  while (ECHO_INPORT & (1 << ECHO_PIN)) {
    count++;
    _delay_us(1);
  }

  // signal travels 340 meters per second
  distance = count*.034/2;

  // display the results
  Serial.print("Distance: ");
  Serial.println(distance);

  // give sensor time to reset
  delay(100);
}
