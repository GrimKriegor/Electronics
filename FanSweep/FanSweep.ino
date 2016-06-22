#include <SoftwareServo.h> 
 
SoftwareServo SWEEPER; 

//Configuration
int ANGLE_MIN = 40;
int ANGLE_MAX = 150;
int SPEED_MIN = 10;
int SPEED_MAX = 100;
int SPEED = 10;
int WAIT = 50;
 
int POSITION = 0; 
 
void setup() { 
  SWEEPER.attach(1); 
  pinMode(A3, INPUT);
} 
 
void loop() { 
  
  for(POSITION = ANGLE_MIN; POSITION <= ANGLE_MAX; POSITION += 1) {
    SWEEPER.write(POSITION); 
    SPEED = map(analogRead(A3), 0, 1023, SPEED_MIN, SPEED_MAX);
    SoftwareServo::refresh();
    delay(SPEED); 
  } 
  delay(WAIT);
  for(POSITION = ANGLE_MAX; POSITION>=ANGLE_MIN; POSITION-=1) {                                
    SWEEPER.write(POSITION); 
    SPEED = map(analogRead(A3), 0, 1023, SPEED_MIN, SPEED_MAX);
    SoftwareServo::refresh();
    delay(SPEED);
  } 
  delay(WAIT);
  
} 

