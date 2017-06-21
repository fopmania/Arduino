
#define PIN_GROUND A1
int pins[]= {2,3,4,5};
#define NPINS 4
#define COMMAND_FOREWARD 3
#define COMMAND_LEFT 1
#define COMMAND_RIGHT 2
#define COMMAND_FIRE 0

volatile char inChar;
unsigned long lastCommand_millis = 0;
int commndDuration_millis = 500;


void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  // print help
  Serial.println("TestHexBugController: starting...");
  Serial.println("Commands Include: ");
  Serial.println("    'w' = Forward");
  Serial.println("    'a' = Left");
  Serial.println("    'd' = Right");
  Serial.println("    's' = Fire");
  
  //initialize the pins
  pinMode(PIN_GROUND,OUTPUT); digitalWrite(PIN_GROUND,LOW);
  stopAllPins();
}

void stopAllPins() {
  //stopping all pins means putting them into a high impedance state
  //Serial.println("Stopping All Pins...");
  for (int Ipin=0; Ipin < NPINS; Ipin++) {
    digitalWrite(pins[Ipin],LOW);
    pinMode(pins[Ipin],INPUT);
  }
}

void loop() {
  // print the string when a newline arrives:
  if (millis() > lastCommand_millis+commndDuration_millis) {
    lastCommand_millis = millis()+10000; //don't do this branch for a while
    stopAllPins();
  }
}

void doAction(int command_pin_ind) {
  if (command_pin_ind < NPINS) {
    stopAllPins();
    pinMode(pins[command_pin_ind],OUTPUT);
    digitalWrite(pins[command_pin_ind],LOW);
    lastCommand_millis = millis();  //time the command was received
  }
}
 

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print("Received "); Serial.println(inChar);
    switch (inChar) {
     case 'w':
       doAction(COMMAND_FOREWARD); break;
     case 'a':
       doAction(COMMAND_LEFT); break;
     case 'd':
       doAction(COMMAND_RIGHT); break;
     case 's':
       doAction(COMMAND_FIRE); break;
     }
  }
}


