#include <avr/sleep.h>

int led = 4;
boolean On = 1;
int Power __attribute__ ((section (".noinit")));

void PowerDown () {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA &= ~(1<<ADEN);       // Turn off ADC to save power
  sleep_enable();
  sleep_cpu();
}
  
void setup() {
  Power = !Power;
  if (!Power) PowerDown();
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, On = !On);  // Toggle the LED
  delay(1000);                  // Wait for a second
}
