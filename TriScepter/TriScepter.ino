#include <LEDController.h>    // https://github.com/GrimKriegor/Electronics/tree/master/LEDController
#include <Wire.h>             // http://arduino.cc/en/Reference/Wire
#include <CapacitiveSensor.h> // https://github.com/PaulStoffregen/CapacitiveSensor
#include <LowPower.h>         // https://github.com/rocketscream/Low-Power

/*
 * CONFIGURATION
 */
 
//DEBUG MODE
const boolean DEBUG = true;

//LEDS
const uint8_t LED_PIN_RED = 3;
const uint8_t LED_PIN_GREEN = 5;
const uint8_t LED_PIN_BLUE = 6;

//TOUCH SENSOR
const uint8_t TOUCH_PIN_SENDER = 10;
const uint8_t TOUCH_PIN_RECEIVER = 8;
const unsigned int TOUCH_THRESHOLD = 20000;

//ACCELEROMETER
const int ACCELEROMETER_ADDRESS = 0x68;  // I2C address of the MPU-6050
const int MOVEMENT_THRESHOLD = 20;

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

//Machine state storage
uint8_t STAGE = 0;



/*
 * TOUCH SENSOR
 */
boolean readTouch() {
  boolean TOUCH = false;
  unsigned int TOUCH_PROXIMITY = 0;
  
  TOUCH_PROXIMITY = TOUCH_SENSOR.capacitiveSensor(30);
  if ( TOUCH_PROXIMITY >= TOUCH_THRESHOLD ) { TOUCH = true; } 
  else { TOUCH = false; }
  
  return TOUCH;
}

unsigned int readTouchProximity() {
  unsigned int TOUCH_PROXIMITY = 0;
  TOUCH_PROXIMITY = TOUCH_SENSOR.capacitiveSensor(30);
  return TOUCH_PROXIMITY;
}

/*
 * ACCELERATION
 */
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

/*
 * LED COLOR
 */

int calcColorStep(int COLOR_PREVIOUS, int COLOR_TARGET) {
  int STEP = COLOR_TARGET - COLOR_PREVIOUS;
  if (STEP) { STEP = 1020/STEP; }
  
  return STEP;
}

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
    
    //Write value to the LEDs
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

int boundColorValue(int VALUE) {
  if (VALUE > 255) { VALUE = 255; }
  else if (VALUE < 0) { VALUE = 0; }

  return VALUE;
}

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
    while(readTouchProximity() < 20000) {
      LED_BLUE.dim(map(readTouchProximity(),100,15000,1,255));
    }
    STAGE = 99;
  }

  //MUTATION Stage
  else if ( STAGE == 3 ) {
    
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
  }

  
}
