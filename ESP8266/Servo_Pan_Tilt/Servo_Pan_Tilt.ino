#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
 
char auth[] = "33268351e4404ba79ebf4e5c44206e59";                  // your Blynk Auth Token
const char* ssid = "N504";                     // your ssid
const char* password = "0222265277";                // your password
 
Servo myServo;
Servo myServo1;
 
void setup() {
  Blynk.begin(auth, ssid, password);
  myServo.attach(5);                                // pin of Servo Motor (D1 or GPIO5)
  myServo1.attach(4);                               // pin of Servo Motor1 (D2 or GPIO4)  
}
 
BLYNK_WRITE(V0) {                                   // Blynk Virtual pin, V0
  myServo.write(param.asInt());
}
 
BLYNK_WRITE(V1) {                                   // Blynk Virtual pin, V1
  myServo1.write(param.asInt());
}
 
void loop() {
  Blynk.run();
}
