#include <Wire.h>
#include <SoftwareSerial.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
SoftwareSerial espserial(2, 3);

unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values

const int fsrpin = A0;
const int kypin = A2;
const int buttonpin = 5;


String data;

void setup()
{
    Wire.begin();

    Serial.begin(57600);
    espserial.begin(57600);
    pinMode(fsrpin, INPUT);
    pinMode(kypin, INPUT);
    pinMode(buttonpin, INPUT);

    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0)
    {
    } // stop everything if could not connect to MPU6050

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(true, true); // gyro and accelero
    Serial.println("Done!\n");
}

void senddata();

void loop()
{
    mpu.update();

    String req = "";

    while (espserial.available() > 0)
    {
        req += char(espserial.read());
    }

    req.trim();

    if (req == "Ya")
    {
        senddata();
    }

    req = "";
}

void senddata()
{
    int buttonstate = digitalRead(buttonpin);
    Serial.print("hidung = ");
    Serial.print(buttonstate);

    int fsreading = analogRead(fsrpin);
    Serial.print(" | tekanan = ");
    Serial.print(fsreading);

    int pitch = mpu.getAngleY();
    Serial.print(" | Y : ");
	Serial.print(pitch);

    int kyreading = analogRead(kypin);
    Serial.print(" | suara = ");
    Serial.println(kyreading);

    data = String(buttonstate) + "&" + String(fsreading) + "&" + String(kyreading) + "&" + String(pitch);
    espserial.println(data);
    Serial.println(data);
}
