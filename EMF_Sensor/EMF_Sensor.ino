//
// :: EMF Sensor ::
//
// Original concept by Aaron ALAI - http://www.aaronalai.com/emf-detector
// Grim Kriegor <grimkriegor@opmbx.org>
//
// Circuit suggestion:
// * 6 LED connected to {12,10,8,6,4,2} digital pins via 100 ohm resistors
// * Buzzer on digital pin 3
// * Probe connected to analog pin 7
// * Probe connected to ground via 3 Mohm resistor
// 


//Configuration
int NUMREADINGS = 15;
int SENSITIVITY = 15;
byte PROBE_PIN   = A7;
byte LEDS[] = {12,10,8,6,4,2}; // Replace with LED digital pins
byte BUZZER = 3;


//Initialization
byte LEDS_COUNT = sizeof(LEDS) / sizeof(byte);
int VALUE = 0;
int READINGS[15];
int INDEX = 0;
int TOTAL = 0;
int AVERAGE = 0;
int LEVEL = 0;


void setup() {
  
  //Clean READINGS array
  for (int i = 0; i < NUMREADINGS; i++) { READINGS[i] = 0; }
  
  //Set all LED pins to OUTPUT
  for (int i = 0; i < LEDS_COUNT; i++) { pinMode(LEDS[i], OUTPUT); }
  
  //Initialization LED Sequence
  for (int i = 0; i < LEDS_COUNT; i++) { digitalWrite(LEDS[i], HIGH); delay(100); digitalWrite(LEDS[i], LOW); }
  for (int i = LEDS_COUNT; i > 0; i--) { digitalWrite(LEDS[i], HIGH); delay(100); digitalWrite(LEDS[i], LOW); }

  //Initialize BUZZER
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW);

  //Initialize Serial for debugging
  Serial.begin(9600);
}

void loop() {
  
  //Analog read from PROBE_PIN
  VALUE = analogRead(PROBE_PIN);
  
  //Only compute if VALUE is 1 or greater
  if (VALUE >=1) {
    
    //Limits VALUE between 1 and SENSITIVITY then transforms it in an integer between 1 and 1023
    VALUE = constrain(VALUE, 1, SENSITIVITY);
    VALUE = map(VALUE, 1, SENSITIVITY, 1, 1023);
    
    //Append new reading to READINGS array
    TOTAL -= READINGS[INDEX];
    READINGS[INDEX] = VALUE;
    TOTAL += READINGS[INDEX];
    INDEX++;
    
    //Restart readings if INDEX >= maximum NUMREADINGS
    if (INDEX >= NUMREADINGS) { INDEX = 0; };
    
    AVERAGE = TOTAL / NUMREADINGS;
    
    //Converts AVERAGE to a number between 0 and the maximum number of LEDS (LEDS_COUNT)
    LEVEL = map(AVERAGE, 1, 1023, 0, LEDS_COUNT);
    
    //Every iteration, turn on LEVEL number LEDs
    for (int i = 0; i < LEVEL; i++) { digitalWrite(LEDS[i], HIGH); }
    for (int i = 0; i < LEDS_COUNT; i++) { digitalWrite(LEDS[i], LOW); }

    //Ring the BUZZER if activity reaches a certain level (DEFAULT = 4)
    if (LEVEL > 4) { analogWrite(BUZZER, random(100)w); } 
    else { analogWrite(BUZZER, 0); }
    
    Serial.println(VALUE);
  }
}
