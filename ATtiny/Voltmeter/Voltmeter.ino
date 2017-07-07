#include <Wire.h>

#define PIN_VOLT A0
#define PIN_LED   0
#define R1  19800.0f
#define R2  5070.0f

#define BATTERY   1.5
float vout = 0.0f;
float vin = 0.0f;
void setup() 
{   
 pinMode(PIN_LED,OUTPUT);
 pinMode(PIN_VOLT, INPUT);   
 Serial.begin(9600);   
 Serial.println("Voltage: ");   
} 
void loop() 
{       
 int vol = analogRead(PIN_VOLT);       
 vout = (vol*5.0f)/1024.0f;
 vin = vout / (R2/(R1 + R2));       
 
 Serial.print(vol);          
 Serial.print("\t");          
 Serial.print(vout);          
 Serial.print("\t");          
 Serial.print(vin);          
 Serial.println("V");          
 delay(1000); 
}
