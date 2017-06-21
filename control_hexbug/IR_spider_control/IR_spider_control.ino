#include <IRDump.h>
#include <NewPing.h>
#include "IR_signal.h"
#define PIN_TRIG 4 
#define PIN_ECHO 2

#define PIN_IR_OUTPUT 3
#define PIN_PIR       7

#define MAX_DISTANCE_POSSIBLE 500 
#define OBSTACLE_DISTANCE 15
#define SHOOTIN_DISTANCE  100



#define CMD_IDLE      0x00
#define CMD_LEFT      0x01
#define CMD_RIGHT     0x02
#define CMD_FORWARD   0x04
#define CMD_SHOOTING  0x08

#define MASK_IR       0x0F

unsigned int curAction = CMD_IDLE;

NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE_POSSIBLE);


IRDump *irdumper;

void setup() {
  irdumper = new IRDump();
  // initialize serial:
  Serial.begin(9600);

  Serial.println("Spider is ready.");
  pinMode(PIN_IR_OUTPUT, OUTPUT);
  pinMode(PIN_TRIG, INPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_PIR, INPUT);
  
  Serial.println("1");
  Serial.println("2");
//  delay(3000);
  Serial.println("3");
  curAction = CMD_IDLE;
  Serial.println("4");
  
}

void emitInfrared(unsigned int *p) {
  irdumper->Emit(PIN_IR_OUTPUT, p, 38);
  delay(100);
}

unsigned char chooseAction(){
  int dis = sonar.ping_cm();
  int thing = digitalRead(PIN_PIR);

  if(thing == HIGH){
    if(dis < SHOOTIN_DISTANCE){
      Serial.print("Distance = ");
      Serial.println(dis);
      return CMD_SHOOTING;
    }
  }
  
  if(dis < OBSTACLE_DISTANCE){
    Serial.print("Distance = ");
    Serial.println(dis);
    return CMD_RIGHT;
  }
  else{
    return CMD_FORWARD;
  }
  return CMD_IDLE;

}



void doEndAction() {
  if(curAction&MASK_IR != 0){
    for(int i = 0; i < NUM_END_SIGN; i++){
      irdumper->Emit(PIN_IR_OUTPUT, signalEnd[i], 38);
      delay(100);
    }
    Serial.println("End Action");
  }
}

void doAction(int cmd){
  switch(cmd){
    case CMD_LEFT:
    Serial.println("Left");
    emitInfrared((unsigned int *)signalL);
    delay(100);
    break;
    case CMD_RIGHT:
    Serial.println("Right");
    emitInfrared((unsigned int *)signalR);
    delay(200);
    break;
    case CMD_FORWARD:
    Serial.println("Forward");
    emitInfrared((unsigned int *)signalF);
    break;
    case CMD_SHOOTING:
    Serial.println("Shooting");
    emitInfrared((unsigned int *)signalS);
    delay(100);
    break;
  }
  return false;
}

void loop() {
  curAction = chooseAction();
  doAction(curAction);
  doEndAction();
}



void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print("Received "); Serial.println(inChar);
    switch (inChar) {
     case 'w':
       emitInfrared((unsigned int *)signalF); break;
     case 'a':
       emitInfrared((unsigned int *)signalL); break;
     case 'd':
       emitInfrared((unsigned int *)signalR); break;
     case 's':
       emitInfrared((unsigned int *)signalS); break;
     }
  }
}




