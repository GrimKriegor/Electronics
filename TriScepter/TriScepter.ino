#include <LEDController.h>    // https://github.com/GrimKriegor/Electronics/tree/master/LEDController
#include <Wire.h>             // http://arduino.cc/en/Reference/Wire
#include <CapacitiveSensor.h> // https://github.com/PaulStoffregen/CapacitiveSensor
#include <LowPower.h>         // https://github.com/rocketscream/Low-Power

/*
 * CONFIGURATION
 */
 
//DEBUG MODE
const boolean DEBUG = false;

//LEDS
const uint8_t LED_PIN_RED = 3;
const uint8_t LED_PIN_GREEN = 5;
const uint8_t LED_PIN_BLUE = 6;

//TOUCH SENSOR
const uint8_t TOUCH_PIN_SENDER = 10;
const uint8_t TOUCH_PIN_RECEIVER = 9;
const unsigned int TOUCH_THRESHOLD = 20000;

//ACCELEROMETER
const int ACCELEROMETER_ADDRESS = 0x68;  // I2C address of the MPU-6050
const int MOVEMENT_THRESHOLD = 20;
const int MOVEMENT_ARRAY_READINGS = 60;
const int SPIKE_LEVEL = 50;

//COLORS
const uint8_t NUMBER_OF_COLORS = 7;
int COLORS[NUMBER_OF_COLORS][3] = {
  {255,0,0},       //Red
  {0,255,0},       //Green
  {0,0,255},       //Blue
  {255,255,255},   //White
  {80,0,80},       //Purple
  {255,255,0},     //Yellow
  {0,255,255}      //Aqua
};


/*
 * DECLARATIONS
 */

//Objects
LEDController LED_RED(LED_PIN_RED);
LEDController LED_GREEN(LED_PIN_GREEN);
LEDController LED_BLUE(LED_PIN_BLUE);
CapacitiveSensor TOUCH_SENSOR(TOUCH_PIN_SENDER, TOUCH_PIN_RECEIVER);

//Color information storage
int COLOR_PREVIOUS_RED;
int COLOR_PREVIOUS_GREEN;
int COLOR_PREVIOUS_BLUE;
int COLOR_RANDOM_RED;
int COLOR_RANDOM_GREEN;
int COLOR_RANDOM_BLUE;
int VALUE_RED;
int VALUE_GREEN;
int VALUE_BLUE;
unsigned int CROSSFADE_TIME = 2000;

//Movement information storage
int MOVEMENT_ARRAY[MOVEMENT_ARRAY_READINGS];
int MOVEMENT_ARRAY_PERIOD = MOVEMENT_ARRAY_READINGS / 60 * 1000;
int MOVEMENT_ARRAY_READING = 0;

//Process information storage
uint8_t STAGE = 3;



/*
 * TOUCH SENSOR
 */

//Returns true if there's absolute touch, by comparing the timing of the RC circuit (proximity of the user) to the TOUCH_THRESHOLD
boolean readTouch() {
  boolean TOUCH = false;
  unsigned int TOUCH_PROXIMITY = 0;
  
  TOUCH_PROXIMITY = TOUCH_SENSOR.capacitiveSensor(30);
  if ( TOUCH_PROXIMITY >= TOUCH_THRESHOLD ) { TOUCH = true; } 
  else { TOUCH = false; }
  
  return TOUCH;
}

//Returns the proximity of the used by reporting the timing of the RC circuit
unsigned int readTouchProximity() {
  unsigned int TOUCH_PROXIMITY = 0;
  TOUCH_PROXIMITY = TOUCH_SENSOR.capacitiveSensor(30);
  return TOUCH_PROXIMITY;
}

/*
 * ACCELERATION
 */

//Reads gyroscopic output values for X, Y and Z axis and returns the average.
unsigned int readMovementSpeed() {
  long GyX,GyY,GyZ;
  int SPEED;
  
  Wire.beginTransmission(ACCELEROMETER_ADDRESS);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(ACCELEROMETER_ADDRESS,6,true);

  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
  SPEED = (labs(GyX)+labs(GyY)+labs(GyZ))/100;
  return SPEED;
}

//Adds a new reading to the MOVEMENT_ARRAY
void sumMovement() {
  MOVEMENT_ARRAY[MOVEMENT_ARRAY_READING] = readMovementSpeed();
  MOVEMENT_ARRAY_READING++;
}

//Goes through the MOVEMENT_ARRAY and returns the average
int averageMovement() {
  int MOVEMENT_ARRAY_SUM = 0;
  int MOVEMENT_ARRAY_AVERAGE = 0;
  
  for (int i=0; i<MOVEMENT_ARRAY_READINGS; i++) {
    MOVEMENT_ARRAY_SUM = MOVEMENT_ARRAY_SUM + MOVEMENT_ARRAY[i];
  }
  MOVEMENT_ARRAY_AVERAGE = MOVEMENT_ARRAY_SUM / MOVEMENT_ARRAY_READINGS;
  
  return MOVEMENT_ARRAY_AVERAGE;
}

//Goes through the MOVEMENT_ARRAY and returns true if one of the values went beyond the SPIKE_LEVEL
boolean checkSpikeMovement() {
  boolean SPIKE = false;
  for (int i=0; i<MOVEMENT_ARRAY_READINGS; i++) {
    if (MOVEMENT_ARRAY[i] >= SPIKE_LEVEL) {
      SPIKE = true;
    }
  }
  return SPIKE;
}


/*
 * LED COLOR
 */

//Calculates and returns the STEPs between the current LED duty cycle (color) and the target one
int calcColorStep(int COLOR_PREVIOUS, int COLOR_TARGET) {
  int STEP = COLOR_TARGET - COLOR_PREVIOUS;
  if (STEP) { STEP = 1020/STEP; }
  
  return STEP;
}

//Takes STEP and current color VALUE into account, increascing or decreascing its value depending on the loop count (i)
int calcColorValue(int STEP, int VALUE, int i) {
  if ((STEP) && i % STEP == 0) {
    if (STEP > 0) { VALUE++; }
    else if (STEP < 0) { VALUE--; }
  }

  //Keeping it within 0-255
  //if (VALUE > 255) { VALUE = 255; }
  //else if (VALUE < 0) { VALUE = 0; }
  VALUE = boundColorValue(VALUE);

  return VALUE;
}

void crossfadeColor(int COLOR_TARGET_RED=COLOR_RANDOM_RED, int COLOR_TARGET_GREEN=COLOR_RANDOM_GREEN, int COLOR_TARGET_BLUE=COLOR_RANDOM_BLUE) {

  if (DEBUG) {
    Serial.print("Fading from ");
    Serial.print(COLOR_PREVIOUS_RED); Serial.print(" "); Serial.print(COLOR_PREVIOUS_GREEN); Serial.print(" "); Serial.print(COLOR_PREVIOUS_BLUE);
    Serial.print(" to "); Serial.print(COLOR_TARGET_RED); Serial.print(" "); Serial.print(COLOR_TARGET_GREEN); Serial.print(" "); Serial.println(COLOR_TARGET_BLUE);
  }
  
  int STEP_RED = calcColorStep(COLOR_PREVIOUS_RED, COLOR_TARGET_RED);
  int STEP_GREEN = calcColorStep(COLOR_PREVIOUS_GREEN, COLOR_TARGET_GREEN);
  int STEP_BLUE = calcColorStep(COLOR_PREVIOUS_BLUE, COLOR_TARGET_BLUE);

  for (int i = 0; i <= 1020; i++) {
    VALUE_RED = calcColorValue(STEP_RED, VALUE_RED, i);
    VALUE_GREEN = calcColorValue(STEP_GREEN, VALUE_GREEN, i);
    VALUE_BLUE = calcColorValue(STEP_BLUE, VALUE_BLUE, i);
    
    LED_RED.dim(VALUE_RED);
    LED_GREEN.dim(VALUE_GREEN);
    LED_BLUE.dim(VALUE_BLUE);

    delay(CROSSFADE_TIME/1020);
  }
  
  //Refresh current values
  COLOR_PREVIOUS_RED = VALUE_RED;
  COLOR_PREVIOUS_GREEN = VALUE_GREEN;
  COLOR_PREVIOUS_BLUE = VALUE_BLUE;
}

//Reduces or increasces (DIM) color strength by dividing or multiplying each LED's duty cycle by MULTIPLIER
void dimColor(boolean DIM, int MULTIPLIER) {

  int COLOR_DIM_RED;
  int COLOR_DIM_GREEN;
  int COLOR_DIM_BLUE;
  
  if (DIM) {
    COLOR_DIM_RED = COLOR_PREVIOUS_RED / MULTIPLIER;
    COLOR_DIM_GREEN = COLOR_PREVIOUS_GREEN / MULTIPLIER;
    COLOR_DIM_BLUE = COLOR_PREVIOUS_BLUE / MULTIPLIER;
  } else {
    COLOR_DIM_RED = COLOR_PREVIOUS_RED * MULTIPLIER;
    COLOR_DIM_GREEN = COLOR_PREVIOUS_GREEN * MULTIPLIER;
    COLOR_DIM_BLUE = COLOR_PREVIOUS_BLUE * MULTIPLIER;
  }
  
  //Keeping it within 0-255
  COLOR_DIM_RED = boundColorValue(COLOR_DIM_RED);
  COLOR_DIM_GREEN = boundColorValue(COLOR_DIM_GREEN);
  COLOR_DIM_BLUE = boundColorValue(COLOR_DIM_BLUE);

  if (DEBUG) {
    Serial.print("Dimming down: "); Serial.print(DIM); Serial.print(" || Multiplier: "); Serial.println(MULTIPLIER);
  }

  crossfadeColor(COLOR_DIM_RED, COLOR_DIM_GREEN, COLOR_DIM_BLUE);
}

//Processes VALUE and returns a value between 0 and 255
int boundColorValue(int VALUE) {
  if (VALUE > 255) { VALUE = 255; }
  else if (VALUE < 0) { VALUE = 0; }

  return VALUE;
}

//Picks a random color from the COLORS array (PURE=true) or fabricates a completely random one
void randomizeColor(boolean PURE=true) {
  if (PURE) {
    int RANDOM = random(NUMBER_OF_COLORS);
    COLOR_RANDOM_RED = COLORS[RANDOM][0];
    COLOR_RANDOM_GREEN = COLORS[RANDOM][1];
    COLOR_RANDOM_BLUE = COLORS[RANDOM][2];
  } else {
    COLOR_RANDOM_RED = random(0,255);
    COLOR_RANDOM_GREEN = random(0,255);
    COLOR_RANDOM_BLUE = random(0,255);
  }
}

//Randomizes the time the crossfade takes between MIN and MAX values
void randomizeColorCrossfadeTime(int MIN=500, int MAX=3000) {
  CROSSFADE_TIME = random(MIN,MAX);
}


void setup() {
  Wire.begin();
  Wire.beginTransmission(ACCELEROMETER_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  if (DEBUG) { Serial.begin(9600); }
}


void loop() {

  if (DEBUG) {
    Serial.print(">> Stage: "); Serial.print(STAGE); 
    Serial.print(" || Touch: "); Serial.print(readTouch()); Serial.print(" - "); Serial.print(readTouchProximity()); 
    Serial.print(" || Movement: "); Serial.println(readMovementSpeed());
  }

  //OFF Stage
  // Read touch
  if ( STAGE == 0 ) {
    if (!DEBUG) { LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); } 
    else { delay(1000); }
    
    LED_BLUE.off();
    if (readTouch()) { STAGE = 1; }
  }

  //GLOW IN Stage
  else if ( STAGE == 1 ) {
    CROSSFADE_TIME = 5000;
    crossfadeColor(255,255,255);
    CROSSFADE_TIME=500;
    crossfadeColor(255,0,0);
    crossfadeColor(0,255,0);
    crossfadeColor(0,0,255);
    crossfadeColor(0,0,0);
    STAGE = 2;
  }

  //PULSING Stage
  else if ( STAGE == 2 ) {
    while(readTouchProximity() < TOUCH_THRESHOLD) {
      LED_BLUE.dim(map(readTouchProximity(),100,TOUCH_THRESHOLD*0.75,1,255));
    }
    STAGE = 99;
  }

  //MUTATION Stage
  else if ( STAGE == 3 ) {
    //CROSSFADE_TIME = map(readMovementSpeed(),9,1000,1000,100);
    //randomizeColor(true);
    //crossfadeColor();
    int MOVEMENT_SPEED = map(readMovementSpeed(),9,300,0,255);
    LED_RED.dim(boundColorValue(MOVEMENT_SPEED+64));
    LED_GREEN.dim(255-MOVEMENT_SPEED);
  }

  //GLOW OFF Stage
  else if ( STAGE == 4 ) {
    
  }

  //
  else {
  randomizeColor(true);
  crossfadeColor();
  randomizeColorCrossfadeTime(1000);
  dimColor(true,4);
  dimColor(false,4);
  }

  
}
