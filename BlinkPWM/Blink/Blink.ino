/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

#define LED_LEVEL 10
#define INTERVAL 10
#define FREQUENCY 10
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


void setHigh(int holdingTime){
  if(holdingTime == 0)  return;
  digitalWrite(LED_BUILTIN,HIGH);
  delay(holdingTime);
}

void loop() {
  for(int i=0;i<=LED_LEVEL;i++)
  {
    for(int j=0;j<INTERVAL;){
      currentMillis = millis();
      digitalWrite(LED_BUILTIN,LOW);
      if(currentMillis - previousMillis > FREQUENCY){
        j++;
        previousMillis = currentMillis;
        setHigh(i);
      }
    }
  }

  for(int i=LED_LEVEL;i>=0;i--)
  {
    for(int j=0;j<INTERVAL;){
      currentMillis = millis();
      digitalWrite(LED_BUILTIN,LOW);
      if(currentMillis - previousMillis > FREQUENCY){
        j++;
        previousMillis = currentMillis;
        setHigh(i);
      }
    }
  }
  delay(100);
}
