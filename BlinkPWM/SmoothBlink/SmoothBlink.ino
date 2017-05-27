
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

#define LED_LEVEL 20
#define FREQUENCY 20
#define INTERVAL 25
unsigned long preCurMillis = 0;
unsigned long curMillis = 0;
unsigned long preInterMillis = 0;

void setHigh(int holdingTime){
  if(holdingTime == 0)  return;
  digitalWrite(LED_BUILTIN,HIGH);
  delay(holdingTime);
}

void loop() {
  for(int i=0;i<=LED_LEVEL;i++)
  {
    curMillis = millis();
    while(curMillis - preInterMillis < INTERVAL){
      curMillis = millis();
      digitalWrite(LED_BUILTIN,LOW);
      if(curMillis - preCurMillis > FREQUENCY){
        setHigh(i);
        preCurMillis = curMillis;
      }
    }
    preInterMillis = curMillis;
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  for(int i=LED_LEVEL;i>=0;i--)
  {
    curMillis = millis();
    while(curMillis - preInterMillis < INTERVAL){
      curMillis = millis();
      digitalWrite(LED_BUILTIN,LOW);
      if(curMillis - preCurMillis > FREQUENCY){
        setHigh(i);
        preCurMillis = curMillis;
      }
    }
    preInterMillis = curMillis;
  }
  delay(500);
}
