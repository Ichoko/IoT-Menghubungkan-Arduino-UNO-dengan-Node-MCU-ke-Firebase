#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;
SoftwareSerial espserial(2,3);

unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

const int fsrpin = A0;
const int kypin = A2;
const int buttonpin = 5;
int buttonstate = 0; 
int kyreading;
int fsreading;

String data;

void setup() 
{
  Serial.begin(57600);
  espserial.begin(57600);
  pinMode(fsrpin, INPUT);
  pinMode(kypin, INPUT);
  pinMode(buttonpin, INPUT);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  mpu.calibrateGyro();

  mpu.setThreshold(1);
}

void loop()
{
  String req = "";

  while(espserial.available()>0)
  {
    req += char(espserial.read());
  }
  
  req.trim();

  if(req == "Ya")
  {
    senddata();
  }
  
  req = "";
}

void senddata()
{
  buttonstate = digitalRead(buttonpin);
  
  Serial.print("hidung = "); 
  Serial.println(buttonstate);

  
  fsreading = analogRead(fsrpin);
    
  Serial.print("tekanan = "); 
  Serial.println(fsreading); 


  kyreading = analogRead(kypin);
    
  Serial.print("suara = "); 
  Serial.println(kyreading); 


  timer = millis();
  
  Vector norm = mpu.readNormalizeGyro();
  
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  Serial.print("Kemringan = ");
  Serial.println(pitch);

  delay((timeStep*1000) - (millis() - timer));

  data = String(buttonstate) + "&" + String(fsreading) + "&" + String(kyreading) + "&" + String(pitch);
  espserial.println(data);
  Serial.println(data); 
}
