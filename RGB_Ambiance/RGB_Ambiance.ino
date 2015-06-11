//
// :: RGB Ambiance -- Color Mixer for the easily pleased ::
//
// Grim Kriegor <grimkriegor@opmbx.org>
//
// Circuit suggestion:
// 

#include <LEDController.h>


unsigned int MODE = 0;

LEDController RED(6);
LEDController GREEN(5);
LEDController BLUE (3);
LEDController COLORS[] = {RED, GREEN, BLUE};

byte BUTTONS_RGB[] = {A1, A2, A3};
byte POTENTIOMETER = A0;


void setup() {
  
  for (int i=0; i<3; i++) { pinMode(BUTTONS_RGB[i], INPUT_PULLUP); }
  pinMode(POTENTIOMETER, INPUT);
  
  for (int i=0; i<3; i++) { COLORS[i].cycleDim(200*(i+1)); }  
  
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(MODE);
  
  if (MODE == 0) {
    if (!digitalRead(BUTTONS_RGB[0])) { 
      int INT = map(analogRead(POTENTIOMETER), 0, 1023, 100, 0);
      RED.dim(INT);
    }
    
    if (!digitalRead(BUTTONS_RGB[1])) { 
      int INT = map(analogRead(POTENTIOMETER), 0, 1023, 100, 0);
      GREEN.dim(INT);
    }
    
    if (!digitalRead(BUTTONS_RGB[2])) { 
      int INT = map(analogRead(POTENTIOMETER), 0, 1023, 100, 0);
      BLUE.dim(INT);
    }
  }
  
  if (MODE == 1) {
    for (int i; i<3; i++) {
//      unsigned int SPEED = map(analogRead(POTENTIOMETER), 0, 1023, 500, 10000);
//      COLORS[i].cycleDim(SPEED);
      
      for (int j=0; j<100; j++) { 
        unsigned int SPEED = map(analogRead(POTENTIOMETER), 0, 1023, 1, 100);
        COLORS[i].dim(j);
        delay(SPEED);
      }
      for (int j=100; j>0; j--) { 
        unsigned int SPEED = map(analogRead(POTENTIOMETER), 0, 1023, 1, 100);
        COLORS[i].dim(j);
        delay(SPEED);
      }
    }
  }
  
  if (!digitalRead(BUTTONS_RGB[0]) and !digitalRead(BUTTONS_RGB[2])) { MODE = 1; }

}
