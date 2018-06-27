
#include <SPI.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SoftwareSerial.h>
#include "musical_notes.h"


#define PIN_CS 10 // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
#define PIN_TX 2
#define PIN_RX 3  
#define PIN_SPEAKER 8 

#define ADDR_EEPROM_BRIGHT   0
#define ADDR_EEPROM_SPEED    1
#define ADDR_EEPROM_TEXT_LEN   2
#define ADDR_EEPROM_TEXT   3



/*  AT commands.
1. AT+NAME=CSK // (YOUR NAME)

2. AT+UART=57600,0,0 //( speed, STOP BIT, PARITY BIT)

3. AT+ROLE=1 (0 = SLAVE, 1= MASTER)

4. AT+PSWD=1234 OR (0000) <-- 반드시 PIN번호가 맞아야 페어링 됨

5. AT+CMODE=0

6. AT+BIND=XXXX,YY,ZZZZZZ (Mindwave Unique Number)

7. AT+IAC=9E8B33  // DEFAULT  

8. AT+CLASS=0

9. AT+INQM=1,9,48

AT+ROLE?
AT+VERSION?
*/
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(PIN_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
SoftwareSerial Bluetooth(PIN_TX, PIN_RX); // D2:TX D3:RX

String tape = "***** JUNBO *****";
int wait = 20; // In milliseconds.  It's scoll speed.
int brightness = 7;


int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels


//  buzzer sound  

void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave*2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
}     
 
void squeak() {
  for (int i=100; i<5000; i=i*1.45) {
    beep(PIN_SPEAKER,i,60);
  }
  delay(10);
  for (int i=100; i<6000; i=i*1.5) {
    beep(PIN_SPEAKER,i,20);
  }
}
void catcall() {
  for (int i=1000; i<5000; i=i*1.05) {
    beep(PIN_SPEAKER,i,10);
  }
 delay(300);
 
  for (int i=1000; i<3000; i=i*1.03) {
    beep(PIN_SPEAKER,i,10);
  }
  for (int i=3000; i>1000; i=i*.97) {
    beep(PIN_SPEAKER,i,10);
  }
}
void ohhh() {
  for (int i=1000; i<2000; i=i*1.02) {
    beep(PIN_SPEAKER,i,10);
  }
  for (int i=2000; i>1000; i=i*.98) {
    beep(PIN_SPEAKER,i,10);
  }
}


void setup() {
  Serial.begin(9600);
  Bluetooth.begin(38400); // Default communication rate of the Bluetooth module
  clearBluetoothBuffer();

  matrix.setIntensity(brightness); // Use a value between 0 and 15 for brightness
  pinMode(PIN_SPEAKER, OUTPUT);
  catcall();

// Adjust to your own needs
//  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>
//  matrix.setPosition(1, 1, 0); // The second display is at <1, 0>
//  matrix.setPosition(2, 2, 0); // The third display is at <2, 0>
//  matrix.setPosition(3, 3, 0); // And the last display is at <3, 0>
//  ...
//  matrix.setRotation(0, 1);    // The first display is position upside down
  matrix.setRotation(0, 1);    // The same hold for the last display
  matrix.setRotation(1, 1);    // The same hold for the last display
  matrix.setRotation(2, 1);    // The same hold for the last display
  matrix.setRotation(3, 1);    // The same hold for the last display

  renderText();

//  writeInfToEEPROM();
  readInfoFromEEPROM();
  matrix.setIntensity(brightness); // Use a value between 0 and 15 for brightness
}


void readInfoFromEEPROM(){
  brightness = EEPROM.read(ADDR_EEPROM_BRIGHT);
  Serial.print("Brightness : ");
  Serial.println(brightness);
  wait = EEPROM.read(ADDR_EEPROM_SPEED);
  Serial.print("Scroll Speed : ");
  Serial.println(wait);
  
  int len = EEPROM.read(ADDR_EEPROM_TEXT_LEN);
  Serial.print("Text Len : ");
  Serial.println(len);
  tape = "";
  for(int i = 0; i < len; i++){
    char ch = EEPROM.read(ADDR_EEPROM_TEXT + i);
    Serial.print(ch);
    tape += String(ch);
  }
}

void writeInfoToEEPROM(){
  EEPROM.write(ADDR_EEPROM_BRIGHT, brightness);
  EEPROM.write(ADDR_EEPROM_SPEED, wait);
  
  EEPROM.write(ADDR_EEPROM_TEXT_LEN, tape.length());
  for(int i = 0; i < tape.length(); i++){
    EEPROM.write(ADDR_EEPROM_TEXT + i, tape.charAt(i));
  }
}

String getStringFromBluetooth(){
  String out ="";      
  while(Bluetooth.available()){
    char ch = Bluetooth.read();
    if(ch == 0 )  break;
    out += String(ch); 
  }
  return out;
}

bool readBuletooth(){
  if (Bluetooth.available()) {   // Checks whether data is comming from the serial port
    byte indicator = Bluetooth.read();   // Starts reading the serial port, the first byte from the incoming data
    if(!Bluetooth.available())  return false;
    byte isSound = Bluetooth.read();   // Starts reading the serial port, the first byte from the incoming data
    if (indicator == '\1') {
      tape = getStringFromBluetooth();
      EEPROM.write(ADDR_EEPROM_TEXT_LEN, tape.length());
      for(int i = 0; i < tape.length(); i++){
        EEPROM.write(ADDR_EEPROM_TEXT + i, tape.charAt(i));
      }
  
      Serial.print( "Text Length is ");
      Serial.println( tape.length() );
      Serial.println( tape );
      if(isSound == '1') catcall();
      Bluetooth.println( tape );
    }
    // Adjusting the Scrolling Speed
    else if (indicator == '\2') {
      String sS = getStringFromBluetooth();
//      String sS = Bluetooth.readString();
      wait = sS.toInt(); // Milliseconds, subtraction because lower value means higher scrolling speed
      wait = wait < 0 ? 0 : ( wait > 100 ? 100 : wait);
      wait = 100 - wait; 

      EEPROM.write(ADDR_EEPROM_SPEED, wait);
      
      Serial.println( "Speed : " + sS );
      Bluetooth.println( "Speed : " + sS );
      if(isSound == '1') ohhh();
    }
    // Adjusting the brightness
    else if (indicator == '\3') {
      String sB = getStringFromBluetooth();
//      String sB = Bluetooth.readString();
      brightness = sB.toInt();
      matrix.setIntensity(brightness); // Use a value between 0 and 15 for brightness
      EEPROM.write(ADDR_EEPROM_BRIGHT, brightness);
      Serial.println( "Bright : " + sB );
      Bluetooth.println( "Bright : " + sB);
      if(isSound == '1') squeak();
    }
    else{
      char buf[3] = ""; 
      buf[0] = indicator;
      buf[1] = isSound;
      //Bluetooth.write( printf("%c%c%s", indicator, isSound, Serial.read() ) );
      Serial.println( buf + getStringFromBluetooth() );
    }
    
    return true;
  }
  if ( Serial.available() ) {
     Bluetooth.write( Serial.read() );
  }
  return false;
}


void clearBluetoothBuffer(){
    while (Bluetooth.available()>0) {
      Bluetooth.read();
    }
}

void loop() {
  if(readBuletooth()) return;
  renderText();
}

void renderText(){
  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}

