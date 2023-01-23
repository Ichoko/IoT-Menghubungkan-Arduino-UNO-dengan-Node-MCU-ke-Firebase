const int fsrPin = A0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(115200);   
}
 
void loop(void) {
  fsrReading = analogRead(fsrPin);  
 
  Serial.print("Analog reading = "); 
  Serial.println(fsrReading);
  delay(500);
} 
