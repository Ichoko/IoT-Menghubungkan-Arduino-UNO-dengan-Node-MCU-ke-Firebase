#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MPU6050.h"

LiquidCrystal_I2C lcd(0x27, 20, 2);
MPU6050 mpu;
// this constant won't change:
// const int buttonPin = 3; // the pin that the pushbutton is attached to
const int ledPin = 13; // the pin that the LED is attached to
const int pressGood = 3;
const int pressBad = 4;
int fsrPin = A0;
int hidungpin = 12;
int micpin = 11;
int micstate;
int buttonPushCounter = 0; // counter for the number of button presses
int buttonState = 0;       // current state of the button
int lastButtonState = 0;   // previous state of the button
int cprcount, gyroReading, fsrVoltage;
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
  lcd.print("Mulai Kompresi");
  delay(2500);
  lcd.clear();
  lcd.print("30x dlm 15 detik");
  delay(1000);

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
    mpu.calibrateGyro();
  mpu.setThreshold(3);
}
void gyro()
{
  lcd.clear();
  timer = millis();
  Vector norm =  mpu.readNormalizeGyro();
  pitch = pitch + norm.YAxis * timeStep;

  lcd.print(pitch);
  delay((timeStep * 1000) - (millis() - timer));
}
void loop()
{
  time = millis();
  fsrReading = analogRead(fsrPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  // delay(200);

  if (fsrReading > 75)
  {
    press = 1;
  }
  else if (fsrReading < 5)
  {
    press = 0;
  }
  if (fsrReading >= 75)
  {
    digitalWrite(pressGood, HIGH);
    digitalWrite(pressBad, LOW);
  }
  else if (fsrReading < 75)
  {
    digitalWrite(pressBad, HIGH);
    digitalWrite(pressGood, LOW);
  }

  if (press != lastpress)
  {
    if (press == 1)
    {
      cprcount++;
      Serial.print("total comp: ");
      Serial.println(cprcount);
      lcd.clear();
      lcd.print("total comp: ");
      lcd.print(cprcount);
      delay(50);
    }
    lastpress = press;
    if (cprcount == 1)
    {
      timeBegin = millis();
    }
    if (cprcount == 30)
    {
      cprcount = 0;
      timeEnd = millis();
      totalTime = (timeEnd - timeBegin) / 1000;
      lcd.clear();
      lcd.print("Total Time : ");
      lcd.print(totalTime);
      delay(1000);
      if (15 <= totalTime &&
          totalTime <= 18)
      {
        lcd.clear();
        lcd.print("Segera miringkan");
        lcd.setCursor(0, 1);
        lcd.print("kepala korban!!");
        delay(1000);
      }
      else
      {
        lcd.clear();
        lcd.print("Kompresi gagal! ");
        lcd.setCursor(0, 1);
        lcd.print("Ulangi kompresi");
      }
      while (15 <= totalTime &&
             totalTime <= 18)
      {
        gyro();
        hidungstate =
            digitalRead(hidungpin);

        if (pitch > tilted)
        {
          lcd.clear();
          lcd.print("Kemiringan kepala");
          lcd.setCursor(0, 1);
          lcd.print(" OK!");
          delay(1000);
          lcd.clear();
          lcd.print("Segera pencet ");
          lcd.setCursor(0, 1);
          lcd.print("hidung korban!");
          delay(1000);

          if (hidungstate ==
              HIGH)
          {
            lcd.clear();
            lcd.print("Berikan bantuan nafas ");
            lcd.setCursor(0, 1);
            lcd.print("pernapasan ke mulut! ");
            delay(1000);
            lcd.clear();
            lcd.print("2x dlm 5 detik!");

            break;
          }
        }
      }
      while (hidungstate ==
             HIGH)
      {
        micstate = digitalRead(micpin);
        if (micstate != last)
        {
          if (micstate == HIGH)
          {
            lcd.clear();
            delay(1000);
            brt++;
            lcd.print("nafas diberikan = ");
            lcd.setCursor(0, 1);
            lcd.print(brt);
          }
          delay(250);

          if (brt == 1)
          {
            tstart = millis();
          }
          if (brt == 2)
          {
            brt = 0;
            tend = millis();
            ttime = (tend - tstart) /
                    1000;
            lcd.clear();
            lcd.print("Total Time: ");
            lcd.print(ttime);
            delay(5000);
            if (ttime <= 5)
            {
              lcd.clear();
              lcd.print("RJP Berhasil");
              return;
            }
            else if (ttime > 5)
            {
              lcd.clear();

              lcd.print("Pemberian nafas");
              lcd.setCursor(0, 1);
              lcd.print("belum benar!");
              return;
            }
          }
        }
      }
    }
  }
}
