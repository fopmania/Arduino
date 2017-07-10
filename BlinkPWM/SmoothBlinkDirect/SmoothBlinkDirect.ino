#define MIN_LEVEL 1
#define LED_LEVEL 22
#define FREQUENCY LED_LEVEL
#define SPEED     100
#define HOLD      SPEED<<2

unsigned long preCurMillis = 0;
unsigned long curMillis = 0;
unsigned long preInterMillis = 0;

#define PORT_D12 0b00010000 //  D12
#define LED_PORT 0b00100000 //  D13


inline void setHigh(int holdingTime){
  if(holdingTime == 0)  return;
  digitalWrite(LED_BUILTIN,HIGH);
  PORTB = LED_PORT;
  delay(holdingTime);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  int led_b = digitalPinToBitMask(LED_BUILTIN);
  int led_p = digitalPinToPort(LED_BUILTIN);
  Serial.print("digitalPinToBitMask: ");
  Serial.println(led_b);
  Serial.print("digitalPinToPort: ");
  Serial.println(led_p);
  DDRB = LED_PORT;
}

void loop() {
  for(int i=0;i<=LED_LEVEL;i++)
  {
    curMillis = millis();
    while(curMillis - preInterMillis < SPEED){
      curMillis = millis();
//      digitalWrite(LED_BUILTIN,LOW);
      PORTB = 0;
      if(curMillis - preCurMillis > FREQUENCY){
        int k = MIN_LEVEL + (1-cos(PI*.5f*(i/(float)LED_LEVEL))) * (LED_LEVEL-MIN_LEVEL);
//        Serial.println(k);
        setHigh(k);
        preCurMillis = curMillis;
      }
    }
    preInterMillis = curMillis;
  }
//  digitalWrite(LED_BUILTIN, HIGH);
  PORTB = LED_PORT;
  delay(HOLD);
  for(int i=LED_LEVEL;i>=0;i--)
  {
    curMillis = millis();
    while(curMillis - preInterMillis < SPEED){
      curMillis = millis();
//      digitalWrite(LED_BUILTIN,LOW);
      PORTB = 0;
      if(curMillis - preCurMillis > FREQUENCY){
        int k = MIN_LEVEL + (1-cos(PI*.5f*(i/(float)LED_LEVEL))) * (LED_LEVEL-MIN_LEVEL);
        setHigh(k);
//        Serial.println(k);
        preCurMillis = curMillis;
      }
    }
    preInterMillis = curMillis;
  }
//  digitalWrite(LED_BUILTIN, LOW);
  PORTB = 0;
  delay(HOLD);
}
