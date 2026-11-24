// sensor_test.ino -- check the wiring with Tools > Serial Plotter at 9600 baud.
// Both lines should sit steady in normal light and drop when you shade each sensor.
void setup() { Serial.begin(9600); }
void loop() {
  Serial.print(analogRead(A0)); 
  Serial.print(',');
  Serial.println(analogRead(A1));
  delay(100);
}
