#include <Wire.h>

#define PIN_VOLT 1
#define PIN_LED   0
#define R1  19800
#define R2  5070

#define BATTERY   1.5
float vout = 0.0f;
float vin = 0.0f;
void setup() 
{   
 pinMode(PIN_LED,OUTPUT);
 pinMode(PIN_VOLT, INPUT);   
 Serial.begin(9600);   
 Serial.println("Voltage: ");   
 Serial.print("V"); 
} 
void loop() 
{       
 int vol = analogRead(PIN_VOLT);       
 vout = (vol*5.0)/1024.0;       
 vin = vout / (R2/ R1 + R2);       
 Serial.println(vin);          
 delay(1000); 
}
