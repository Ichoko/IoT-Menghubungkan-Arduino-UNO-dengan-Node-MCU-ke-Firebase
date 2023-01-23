#include <SoftwareSerial.h> 
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "cpr-jantung-paru-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "wKLPEfBPafJPdRQQayOfGtowLLtPfVlx12ADBrYG"
#define WIFI_SSID "kaum duafa"
#define WIFI_PASSWORD "namo lengkap wak"

SoftwareSerial inoserial(12,13);

unsigned long waktuawal = 0;
String ardata[4];
int val1, val2, val3, val4;

void setup() {
  Serial.begin(57600);
  inoserial.begin(57600);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  unsigned long waktusekarang = millis();

  if (waktusekarang-waktuawal>=10)
  {
    waktuawal=waktusekarang;

    String data = "";
    
    while(inoserial.available()>0)
    {
      data += char(inoserial.read());
    }
    
    data.trim();

    if(data != "")
    {
       int post = 0;

       for(int i=0; i<=data.length(); i++)
       {
        char pemisah = '&';

        if(data[i] != pemisah)
        {
          ardata[post] += data[i];
        }
        else
        {
          post++; 
        }
       }

       if(post == 3)
       {
        val1 = ardata[0].toInt();
        val2 = ardata[1].toInt();
        val3 = ardata[2].toInt();
        val4 = ardata[3].toInt();
        
        Serial.print("hidung = ");
        Serial.println(val1);
        Serial.print("tekanan = "); 
        Serial.println(val2);
        Serial.print("suara = "); 
        Serial.println(val3);
        Serial.print("Kemiringan = ");
        Serial.println(val4);
        
        Firebase.setInt("AlatA/ hidung", val1);
        Firebase.setInt("AlatA/ tekanan", val2);
        Firebase.setInt("AlatA/ suara", val3);
        Firebase.setInt("AlatA/ kemiringan", val4);
       }

       ardata[0] = "";
       ardata[1] = "";
       ardata[2] = "";
       ardata[3] = "";
    }

    inoserial.println("Ya");
  }

}
