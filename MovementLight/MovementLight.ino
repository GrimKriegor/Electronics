//Movement Light

#include <LEDController.h>

byte LED_pin = 9;
byte IR_pin = A3;
byte LIGHTSENSOR_pin = A5;
byte POTENTIOMETER_pin = A4;


void setup() {
  pinMode(IR_pin, INPUT);
  pinMode(LED_pin, OUTPUT);
}

void loop() {
  int SENSITIVITY = map(analogRead(POTENTIOMETER_pin),0,1024,20,350);
  int LIGHT_LEVEL = analogRead(LIGHTSENSOR_pin);
  
  if ( digitalRead(IR_pin) && LIGHT_LEVEL < SENSITIVITY) {
    digitalWrite(LED_pin, HIGH);
    delay(30000);
    digitalWrite(LED_pin,LOW);

  }
}

