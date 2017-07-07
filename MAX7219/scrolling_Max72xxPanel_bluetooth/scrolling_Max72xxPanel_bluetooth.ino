
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <SoftwareSerial.h>
#include "musical_notes.h"


#define PIN_CS 10 // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
#define PIN_TX 2
#define PIN_RX 3  
#define PIN_SPEAKER 8 
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
*/
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(PIN_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
SoftwareSerial Bluetooth(PIN_TX, PIN_RX); // D2:TX D3:RX

String tape = "Jun Kim";
int wait = 20; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels
int brightness = 7;

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
  Bluetooth.begin(9600); // Default communication rate of the Bluetooth module

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
}

void readBuletooth(){
  if (Bluetooth.available()) {   // Checks whether data is comming from the serial port
    char indicator = Bluetooth.read();   // Starts reading the serial port, the first byte from the incoming data
    Serial.write( indicator );
    // If we have pressed the "Send" button from the Android App, clear the previous text
    if (indicator == '1') {
      char message[128];
      int cnt = 0;
      for (int i = 0; i < 100; i++) {
        message[i] = 0;
        matrix.fillScreen(0);
      }
      // Read the whole data/string comming from the phone and put it into text[] array.
      while (Bluetooth.available()) {
        message[cnt] = Bluetooth.read();
        cnt++;
      }
      Serial.write( message );
      tape = message;
      catcall();
    }
    // Adjusting the Scrolling Speed
    else if (indicator == '2') {
      String sS = Bluetooth.readString();
      wait = sS.toInt(); // Milliseconds, subtraction because lower value means higher scrolling speed
      wait = wait < 0 ? 0 : ( wait > 1000 ? 1000 : wait); 
      Serial.write( wait );
      ohhh();
    }
    // Adjusting the brightness
    else if (indicator == '3') {
      String sB = Bluetooth.readString();
      brightness = sB.toInt();
      matrix.setIntensity(brightness); // Use a value between 0 and 15 for brightness
      Serial.write( brightness );
      squeak();
    }
  }
  if ( Serial.available() ) {
     Bluetooth.write( Serial.read() );
  }
}

void loop() {
  readBuletooth();


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

