#include <IRDump.h>

#define PIN_IR_OUTPUT 3
#define SIGN_COUNT    14

#define NUM_END_SUGN 8      
//right
unsigned int signalR[SIGN_COUNT] = {
  4328, 972,
  1068, 1016,
  1076, 1012,
  1076, 976,
  2140, 1040,
  1048, 984,
  2144, 0
};
//left
unsigned int signalL[SIGN_COUNT] = {
  4324, 1008,
  1040, 1036,
  1056, 976,
  2132, 1040,
  1056, 1004,
  1084, 984,
  2140, 0
};

//forward
unsigned int signalF[SIGN_COUNT] = {
  4344, 980,
  1048, 1040,
  2080, 1036,
  1076, 1012,
  1076, 984,
  1096, 988,
  2144, 0
};
//shooting
unsigned int signalS[SIGN_COUNT] = {
  4292, 1020,
  1064, 992,
  1060, 1016,
  1072, 1016,
  1072, 1008,
  2112, 1016,
  2104, 0
};


unsigned int signalEnd[NUM_END_SUGN][SIGN_COUNT]= {{
  4336, 1016,
  1056, 1036,
  1052, 1020,
  1080, 992,
  1100, 1016,
  1052, 1020,
  2124, 0
},{
  4316, 1020,
  1080, 1016,
  1076, 1016,
  1052, 1048,
  1052, 1012,
  1080, 1016,
  2128, 0
},{
  4372, 1012,
  1028, 1036,
  1088, 984,
  1108, 992,
  1072, 1044,
  1060, 984,
  2152, 0
},{
  4352, 984,
  1104, 984,
  1052, 1044,
  1088, 1004,
  1088, 988,
  1108, 980,
  2156, 0
},{
  4364, 984,
  1052, 1072,
  1060, 984,
  1108, 992,
  1104, 988,
  1072, 1016,
  2140, 0
},{
  4324, 1040,
  1084, 984,
  1072, 1044,
  1060, 1012,
  1088, 984,
  1104, 988,
  2164, 0
},{
  4356, 988,
  1072, 1016,
  1088, 992,
  1100, 992,
  1104, 984,
  1088, 992,
  2152, 0
},
{
  4364, 1012,
  1060, 984,
  1108, 992,
  1104, 988,
  1072, 1016,
  1096, 984,
  2160, 0
}  
};


IRDump *irdumper;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  irdumper = new IRDump();


  // print help
  Serial.println("TestHexBugController: starting...");
  Serial.println("Commands Include: ");
  Serial.println("    'w' = Forward");
  Serial.println("    'a' = Left");
  Serial.println("    'd' = Right");
  Serial.println("    's' = Fire");
  
}


void loop() {
}

void doAction(unsigned int *p) {
  Serial.println("Emit");
  irdumper->Emit(PIN_IR_OUTPUT, p, 38);
  for(int i = 0; i < NUM_END_SUGN; i++){
    delay(100);
    irdumper->Emit(PIN_IR_OUTPUT, signalEnd[i], 38);
  }
}
 
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print("Received "); Serial.println(inChar);
    switch (inChar) {
     case 'w':
       doAction((unsigned int *)signalF); break;
     case 'a':
       doAction((unsigned int *)signalL); break;
     case 'd':
       doAction((unsigned int *)signalR); break;
     case 's':
       doAction((unsigned int *)signalS); break;
     }
  }
}




