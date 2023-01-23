  /*   
modified on June 5, 2018
by Arduino Examples from arduino.cc/en/Tutorial/ReadAnalogVoltage
Home 
*/ 
int pinKy = A0;
int kyReading = 0; 
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  kyReading = analogRead (pinKy);  
  Serial.print("Analog reading = ");
  Serial.println(kyReading);  
} 
