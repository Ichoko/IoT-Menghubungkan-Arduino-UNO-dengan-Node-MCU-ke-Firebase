#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
float yaw = 0;

const int fsrpin = A0;
const int kypin = A2;
int kyreading;
int fsreading;

void setup() 
{
  Serial.begin(115200);

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
    fsreading = analogRead(fsrpin);
      
    Serial.print("FSR reading = "); 
    Serial.print(fsreading); 
  
  
    kyreading = analogRead(kypin);
      
    Serial.print(" KY reading = "); 
    Serial.print(kyreading); 
  
  
    timer = millis();
    
    Vector norm = mpu.readNormalizeGyro();
    
    pitch = pitch + norm.YAxis * timeStep;
    roll = roll + norm.XAxis * timeStep;
    yaw = yaw + norm.ZAxis * timeStep;
  
    Serial.print("  Pitch = ");
    Serial.println(pitch);
  
    delay((timeStep*1000) - (millis() - timer));
    
}
