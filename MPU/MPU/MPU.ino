#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MPU6050.h"

MPU6050 mpu;
// this constant won't change:
// const int buttonPin = 3; // the pin that the pushbutton is attached to
int gyroReading;
int fsrReading, brt, last;
int press, lastpress;
int tilted = 30;
int minVal = 265;
int maxVal = 402;
int hidungstate;
int sensorValue = 0;
float timeStep = 0.01;
float pitch = 0;
float sensorVoltage, kPa, PSI;
unsigned long timeBegin, timeEnd, time, totalTime;
unsigned long tstart, tend, ttime;
unsigned long fsrResistance;
unsigned long fsrConductance;
long fsrForce, mapped;
long timer = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(2500);
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
    mpu.calibrateGyro();
  mpu.setThreshold(3);
}


void gyro()
{
  timer = millis();
  Vector norm =  mpu.readNormalizeGyro();
  pitch = pitch + norm.YAxis * timeStep;
  delay((timeStep * 1000) - (millis() - timer));
}

void loop()
{
  time = millis();
        gyro();
  Serial.println(pitch);
  
}
